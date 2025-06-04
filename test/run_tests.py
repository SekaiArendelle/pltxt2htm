if __name__ != "__main__":
    raise RuntimeError("This file can't be imported")

import os
import shutil
import argparse
import platform

# config section
TEST_DIR = os.path.dirname(os.path.abspath(__file__))

parser = argparse.ArgumentParser()
parser.add_argument("--rebuild-all", action=argparse.BooleanOptionalAction, help="deleting all cached test files")
parser.add_argument("--compiler", required=True, choices=("clang", "gcc"), help="compiler to use")
parser.add_argument("--target", help="target triplet")
parser.add_argument("--sysroot", help="sysroot to use")
args = parser.parse_args()

if args.target is not None:
    toolchain = f"{args.target}-{args.compiler}"
else:
    if platform.system() == "Windows" and args.compiler == "clang":
        if shutil.which("gcc") is not None:
            toolchain = "x86_64-windows-gnu-clang"
        else:
            toolchain = "x86_64-windows-msvc-clang"
    else:
        toolchain = args.compiler

print(f"-- using toolchain \"{toolchain}\"")

# build section
os.chdir(TEST_DIR)
print(f"entering dir \"{TEST_DIR}\"")

if args.rebuild_all:
    if os.path.exists(os.path.join(TEST_DIR, ".xmake")):
        shutil.rmtree(os.path.join(TEST_DIR, ".xmake"))
        print(f"removing dir \"{os.path.join(TEST_DIR, '.xmake')}\"")
    if os.path.exists(os.path.join(TEST_DIR, "build")):
        shutil.rmtree(os.path.join(TEST_DIR, "build"))
        print(f"removing dir \"{os.path.join(TEST_DIR, 'build')}\"")

# if args.compiler == "clang" and toolchain != "x86_64-windows-msvc-clang":
#     clang_runtime = "--runtimes=libc++_shared --rtlib=compiler-rt --unwindlib=libunwind"
# else:
#     clang_runtime = ""

if args.sysroot is not None:
    sysroot = f"--sysroot={args.sysroot}"
else:
    sysroot = ""

os.system(f"xmake config --mode=debug --toolchain={toolchain} {sysroot}")
os.system("xmake test")
