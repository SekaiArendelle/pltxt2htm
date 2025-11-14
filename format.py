if __name__ != "__main__":
    raise Exception("This file can't be imported")

import os
import shutil

# C++
if shutil.which("clang-format"):
    HAS_CLANG_FORMAT = True
else:
    print("warning: clang-format not found")
    HAS_CLANG_FORMAT = False

# Rust
if shutil.which("rustfmt"):
    HAS_RUST_FMT = True
else:
    print("warning: rustfmt not found")
    HAS_RUST_FMT = False

# Lua
# lua-format from https://github.com/Koihik/LuaFormatter
if shutil.which("lua-format"):
    HAS_LUA_FORMAT = True
else:
    print("warning: lua-format not found")
    HAS_LUA_FORMAT = False

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
                if HAS_CLANG_FORMAT:
                    os.system(f'clang-format -i "{os.path.join(root, file)}"')
            elif file.endswith(".rs"):
                if HAS_RUST_FMT:
                    os.system(f'rustfmt "{os.path.join(root, file)}"')
            elif file.endswith(".lua"):
                if HAS_LUA_FORMAT:
                    os.system(f'lua-format -i "{os.path.join(root, file)}"')

for a_dir in dirs:
    format_files_in_a_dir(a_dir)

