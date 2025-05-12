if __name__ != "__main__":
    raise RuntimeError("This file can't be imported")

import os
import shutil

TEST_DIR = os.path.dirname(os.path.abspath(__file__))

os.chdir(TEST_DIR)
print(f"entering dir \"{TEST_DIR}\"")
if os.path.exists(os.path.join(TEST_DIR, ".xmake")):
    shutil.rmtree(os.path.join(TEST_DIR, ".xmake"))
    print(f"removing dir \"{os.path.join(TEST_DIR, '.xmake')}\"")
if os.path.exists(os.path.join(TEST_DIR, "build")):
    shutil.rmtree(os.path.join(TEST_DIR, "build"))
    print(f"removing dir \"{os.path.join(TEST_DIR, 'build')}\"")

os.system("xmake config --mode=debug")
os.system("xmake test")
