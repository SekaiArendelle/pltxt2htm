"""
Generate a merged compile_commands.json for LSP (clangd, etc.).

Each CMake sub-project is configured (configure-only, no compile) into
<sub>/build-lsp with -DCMAKE_EXPORT_COMPILE_COMMANDS=ON, and all resulting
compile_commands.json entries are merged into <root>/compile_commands.json,
which clangd discovers by climbing parent directories. The library is
header-only, so it needs entries from every sub-project that compiles the
core headers, not just one.

Usage:
    python scripts/gen_compile_commands.py [subproject ...]
"""

import json
import os
import shutil
import subprocess
import sys

SCRIPT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

BUILD_DIR = "build-lsp"
COMPILE_DB = "compile_commands.json"
DEFAULT_SUBPROJECTS = ["examples", "cmd", "c", "tests"]


def find_compiler():
    return os.environ.get("CXX") or shutil.which("clang++") or "clang++"


def configure(subproject, compiler):
    build_dir = os.path.join(subproject, BUILD_DIR)
    cmd = [
        "cmake", "-S", subproject, "-B", build_dir, "-G", "Ninja",
        "-DCMAKE_BUILD_TYPE=Debug",
        "-DCMAKE_CXX_COMPILER=" + compiler,
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
    ]
    print(f"Configuring {os.path.basename(subproject)} -> {build_dir}")
    result = subprocess.run(cmd)
    if result.returncode != 0:
        return None
    return build_dir


def main():
    subprojects = sys.argv[1:] or DEFAULT_SUBPROJECTS
    compiler = find_compiler()
    print(f"compiler: {compiler}")

    entries = []
    for name in subprojects:
        subproject = os.path.join(SCRIPT_DIR, name)
        if not os.path.isdir(subproject) or not os.path.isfile(
            os.path.join(subproject, "CMakeLists.txt")
        ):
            print(f"Warning: '{name}' is not a CMake sub-project, skipping.",
                  file=sys.stderr)
            continue
        build_dir = configure(subproject, compiler)
        if build_dir is None:
            print(f"Error: failed to configure '{name}'", file=sys.stderr)
            sys.exit(1)
        with open(os.path.join(build_dir, COMPILE_DB), encoding="utf-8") as f:
            entries.extend(json.load(f))

    entries.sort(key=lambda entry: (entry.get("file", ""), entry.get("directory", "")))
    output_path = os.path.join(SCRIPT_DIR, COMPILE_DB)
    with open(output_path, "w", encoding="utf-8") as f:
        json.dump(entries, f, indent=1)
    print(f"Wrote {len(entries)} entries -> {output_path}")


if __name__ == "__main__":
    main()
