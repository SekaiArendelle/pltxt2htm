if __name__ != "__main__":
    raise Exception("This file can't be imported")

import os
import shutil

if not shutil.which("clang-format"):
    raise Exception("clang-format not found")

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

dirs = [
    os.path.join(SCRIPT_DIR, "test"),
    os.path.join(SCRIPT_DIR, "include", "pltxt2htm"),
    os.path.join(SCRIPT_DIR, "wasm"),
    os.path.join(SCRIPT_DIR, "cmd"),
    os.path.join(SCRIPT_DIR, "py"),
    os.path.join(SCRIPT_DIR, "fuzzing"),
    os.path.join(SCRIPT_DIR, "c"),
    os.path.join(SCRIPT_DIR, "cxxmodule", "pltxt2htm"),
    os.path.join(SCRIPT_DIR, "rust", "pltxt2htm"),
    os.path.join(SCRIPT_DIR, "rust", "libpltxt2htm-sys"),
]


def format_files_in_a_dir(dir: str) -> None:
    for root, dirs, files in os.walk(dir):
        for file in files:
            if (
                file.endswith(".cc")
                or file.endswith(".hh")
                or file.endswith(".h")
                or file.endswith(".cpp")
                or file.endswith(".cppm")
            ):
                os.system(f'clang-format -i "{os.path.join(root, file)}"')
            elif file.endswith(".rs"):
                os.system(f'rustfmt "{os.path.join(root, file)}"')

for a_dir in dirs:
    format_files_in_a_dir(a_dir)
