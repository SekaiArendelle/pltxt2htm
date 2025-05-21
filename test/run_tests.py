if __name__ != "__main__":
    raise RuntimeError("This file can't be imported")

import os
import shutil
import argparse

TEST_DIR = os.path.dirname(os.path.abspath(__file__))

parser = argparse.ArgumentParser()
parser.add_argument("--rebuild-all", action=argparse.BooleanOptionalAction, help="deleting all cached test files")
args = parser.parse_args()

os.chdir(TEST_DIR)
print(f"entering dir \"{TEST_DIR}\"")

if args.rebuild_all:
    if os.path.exists(os.path.join(TEST_DIR, ".xmake")):
        shutil.rmtree(os.path.join(TEST_DIR, ".xmake"))
        print(f"removing dir \"{os.path.join(TEST_DIR, '.xmake')}\"")
    if os.path.exists(os.path.join(TEST_DIR, "build")):
        shutil.rmtree(os.path.join(TEST_DIR, "build"))
        print(f"removing dir \"{os.path.join(TEST_DIR, 'build')}\"")

os.system("xmake config --arch=x64 --plat=mingw --mode=debug --toolchain=x86_64-windows-gnu-clang")
os.system("xmake test")
