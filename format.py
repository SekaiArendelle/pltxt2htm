if __name__ != "__main__":
    raise Exception("This file can't be imported")

import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TEST_DIR = os.path.join(SCRIPT_DIR, "test")
SRC_DIR = os.path.join(SCRIPT_DIR, "include", "pltxt2htm")
JS_DIR = os.path.join(SCRIPT_DIR, "js")
CMD_DIR = os.path.join(SCRIPT_DIR, "cmd")

def format_files_in_a_dir(dir: str) -> None:
    for root, dirs, files in os.walk(dir):
        for file in files:
            if file.endswith(".cc"):
                os.system(f"clang-format -i \"{os.path.join(root, file)}\"")
            elif file.endswith(".h"):
                os.system(f"clang-format -i \"{os.path.join(root, file)}\"")

format_files_in_a_dir(TEST_DIR)
format_files_in_a_dir(SRC_DIR)
format_files_in_a_dir(JS_DIR)
format_files_in_a_dir(CMD_DIR)

