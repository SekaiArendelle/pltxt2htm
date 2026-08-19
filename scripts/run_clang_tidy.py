"""
Run clang-tidy over the project's CMake sub-projects.

Each sub-project is configured (configure-only, no compile) into
<sub>/build-tidy with -DCMAKE_EXPORT_COMPILE_COMMANDS=ON, and clang-tidy is
then run on every translation unit recorded in its compile_commands.json.
This way clang-tidy uses the exact same flags as the real build (including
machine-specific ones such as -stdlib=libc++, passed via CXXFLAGS).

Usage:
    python scripts/run_clang_tidy.py [-j N] [--refresh] [subproject ...]
"""

import argparse
import json
import os
import shlex
import shutil
import subprocess
import sys
import threading
from concurrent.futures import ThreadPoolExecutor

SCRIPT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

BUILD_DIR = "build-tidy"
COMPILE_DB = "compile_commands.json"
COMPILER_MARKER = ".clang_tidy_compiler"
DEFAULT_SUBPROJECTS = ["examples", "cmd", "c"]

_print_lock = threading.Lock()


def log(message):
    with _print_lock:
        print(message)


def find_clang_tidy():
    return os.environ.get("CLANG_TIDY", "clang-tidy")


def find_clang_pp(clang_tidy):
    """Pick the clang++ matching clang-tidy so flags resolve to the same
    standard library installation (e.g. libc++)."""
    exe = os.environ.get("CLANG_CXX")
    if exe:
        return exe
    resolved = shutil.which(clang_tidy)
    if resolved:
        sibling = os.path.join(os.path.dirname(resolved), "clang++")
        if os.path.isfile(sibling):
            return sibling
    return "clang++"


def configure(subproject, refresh, compiler, cxxflags):
    """Ensure <sub>/build-tidy has a compile_commands.json (configure only)."""
    build_dir = os.path.join(subproject, BUILD_DIR)
    db_path = os.path.join(build_dir, COMPILE_DB)
    marker_path = os.path.join(build_dir, COMPILER_MARKER)

    needs_configure = refresh or not os.path.isfile(db_path) or not os.path.isfile(marker_path)
    if not needs_configure:
        with open(marker_path, encoding="utf-8") as f:
            lines = f.read().splitlines()
        stored_compiler = lines[0] if lines else ""
        stored_flags = lines[1] if len(lines) > 1 else ""
        needs_configure = stored_compiler != compiler or stored_flags != (cxxflags or "")
    if not needs_configure:
        return build_dir

    cmd = [
        "cmake", "-S", subproject, "-B", build_dir, "-G", "Ninja",
        "-DCMAKE_BUILD_TYPE=Debug",
        "-DCMAKE_CXX_COMPILER=" + compiler,
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
    ]
    if cxxflags:
        cmd.append("-DCMAKE_CXX_FLAGS=" + cxxflags)

    log(f"Configuring {os.path.basename(subproject)} -> {build_dir}")
    result = subprocess.run(cmd)
    if result.returncode != 0:
        return None
    with open(marker_path, "w", encoding="utf-8") as f:
        f.write(f"{compiler}\n{cxxflags or ''}\n")
    return build_dir


def load_entries(build_dir):
    with open(os.path.join(build_dir, COMPILE_DB), encoding="utf-8") as f:
        return json.load(f)


def scrub_compile_command(entry):
    """Extract compiler flags from a compile_commands.json entry, dropping
    the driver, output/-c flags, dependency files and the source file, so the
    flags can be passed to clang-tidy directly."""
    if "arguments" in entry:
        args = list(entry["arguments"])
    else:
        args = shlex.split(entry["command"])
    args = args[1:]  # drop the compiler driver

    flags = []
    consume_next = False
    for arg in args:
        if consume_next:
            consume_next = False
            continue
        if arg in ("-c", "-o", "-MF", "-MT", "-MQ", "-MJ"):
            if arg != "-c":
                consume_next = True
            continue
        if arg.startswith(("-o", "-MF", "-MT", "-MQ", "-MJ")):
            continue
        if arg.startswith("-"):
            flags.append(arg)
        # bare tokens are the source file (and its dependencies); drop them
    return flags


def run_one(clang_tidy, entry):
    file_path = entry["file"]
    flags = scrub_compile_command(entry)
    cmd = [clang_tidy, "--quiet", file_path, "--", *flags]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True)
    except OSError as exc:
        log(f"error: cannot run {clang_tidy}: {exc}")
        return file_path, False
    output = result.stdout + result.stderr
    if output:
        log(output.rstrip())
    return file_path, result.returncode == 0


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "-j", "--jobs", type=int, default=os.cpu_count(),
        help="number of parallel clang-tidy jobs (default: number of CPUs)",
    )
    parser.add_argument(
        "--refresh", action="store_true",
        help="re-configure sub-projects even if a compile_commands.json exists",
    )
    parser.add_argument(
        "subprojects", nargs="*", default=DEFAULT_SUBPROJECTS,
        help="sub-projects to tidy (default: %(default)s)",
    )
    args = parser.parse_args()

    clang_tidy = find_clang_tidy()
    compiler = find_clang_pp(clang_tidy)
    cxxflags = os.environ.get("CXXFLAGS")
    log(f"clang-tidy: {shutil.which(clang_tidy) or clang_tidy}")
    log(f"clang++: {compiler}")

    tasks = []
    skipped = []
    for name in args.subprojects:
        subproject = os.path.join(SCRIPT_DIR, name)
        if not os.path.isdir(subproject) or not os.path.isfile(
            os.path.join(subproject, "CMakeLists.txt")
        ):
            print(f"Warning: '{name}' is not a CMake sub-project, skipping.",
                  file=sys.stderr)
            skipped.append(name)
            continue
        build_dir = configure(subproject, args.refresh, compiler, cxxflags)
        if build_dir is None:
            print(f"Error: failed to configure '{name}'", file=sys.stderr)
            sys.exit(1)
        entries = load_entries(build_dir)
        tasks.extend(entries)
        log(f"{name}: {len(entries)} translation unit(s)")

    if not tasks:
        print("No translation units to tidy.", file=sys.stderr)
        sys.exit(1 if skipped else 0)

    failures = []
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        futures = [pool.submit(run_one, clang_tidy, entry) for entry in tasks]
        for future in futures:
            file_path, ok = future.result()
            if not ok:
                failures.append(file_path)

    if failures:
        print(f"\nclang-tidy failed for {len(failures)} file(s):",
              file=sys.stderr)
        for file_path in failures:
            print(f"  {file_path}", file=sys.stderr)
        sys.exit(1)

    print("\nclang-tidy: OK")


if __name__ == "__main__":
    main()
