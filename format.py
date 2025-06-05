if __name__ != "__main__":
    raise Exception("This file can't be imported")

import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TEST_DIR = os.path.join(SCRIPT_DIR, "test")
SRC_DIR = os.path.join(SCRIPT_DIR, "include", "pltxt2htm")

for root, dirs, files in os.walk(TEST_DIR):
    for file in files:
        if file.endswith(".cc"):
            os.system(f"clang-format -i \"{os.path.join(root, file)}\"")

for root, dirs, files in os.walk(SRC_DIR):
    for file in files:
        if file.endswith(".hh"):
            os.system(f"clang-format -i \"{os.path.join(root, file)}\"")
