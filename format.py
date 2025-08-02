if __name__ != "__main__":
    raise Exception("This file can't be imported")

import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TEST_DIR = os.path.join(SCRIPT_DIR, "test")
SRC_DIR = os.path.join(SCRIPT_DIR, "include", "pltxt2htm")
WASM_DIR = os.path.join(SCRIPT_DIR, "wasm")
CMD_DIR = os.path.join(SCRIPT_DIR, "cmd")
PY_DIR = os.path.join(SCRIPT_DIR, "py")
FUZZING_DIR = os.path.join(SCRIPT_DIR, "fuzzing")
C_DIR = os.path.join(SCRIPT_DIR, "c")

def format_files_in_a_dir(dir: str) -> None:
    for root, dirs, files in os.walk(dir):
        for file in files:
            if file.endswith(".cc") or file.endswith(".hh") or file.endswith(".h") or file.endswith(".cpp"):
                os.system(f"clang-format -i \"{os.path.join(root, file)}\"")

format_files_in_a_dir(TEST_DIR)
format_files_in_a_dir(SRC_DIR)
format_files_in_a_dir(WASM_DIR)
format_files_in_a_dir(CMD_DIR)
format_files_in_a_dir(PY_DIR)
format_files_in_a_dir(FUZZING_DIR)
format_files_in_a_dir(C_DIR)
