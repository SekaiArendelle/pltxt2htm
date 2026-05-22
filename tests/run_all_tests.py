if __name__ != "__main__":
    raise Exception("This file can't be imported")

import os
import shutil
import argparse
import subprocess

TEST_DIR = os.path.dirname(os.path.abspath(__file__))
BUILD_DIR = os.path.join(TEST_DIR, "build")

parser = argparse.ArgumentParser()
parser.add_argument("--compiler", choices=("clang", "gcc"), help="compiler to use")
parser.add_argument("--target", help="target triplet")
parser.add_argument("--sysroot", help="sysroot to use")
parser.add_argument("--sanitizer", choices=("address", "undefined", "memory"))
args = parser.parse_args()

if args.compiler is None:
    for candidate in ("clang++", "g++"):
        if shutil.which(candidate) is not None:
            args.compiler = "clang" if candidate == "clang++" else "gcc"
            break
    else:
        raise Exception("no compiler found (tried clang++, g++)")

print(f"-- using compiler \"{args.compiler}\"")

cmake_cmd = [
    "cmake", "-S", TEST_DIR, "-B", BUILD_DIR,
    "-GNinja",
    "-DCMAKE_BUILD_TYPE=Debug",
    f"-DCMAKE_CXX_COMPILER={'clang++' if args.compiler == 'clang' else 'g++'}",
]

if args.target is not None:
    cmake_cmd += [f"-DCMAKE_CXX_COMPILER_TARGET={args.target}"]

if args.sysroot is not None:
    cmake_cmd += [f"-DCMAKE_SYSROOT={args.sysroot}"]

if args.sanitizer is not None:
    cmake_cmd += [f"-DPLTXT2HTM_SANITIZER={args.sanitizer}"]

print("-- configuring ...", " ".join(cmake_cmd))
ret = subprocess.run(cmake_cmd)
if ret.returncode != 0:
    raise Exception("CMake configure fail")

print("-- building ...")
ret = subprocess.run(["cmake", "--build", BUILD_DIR, "-v", "-j", str(os.cpu_count())])
if ret.returncode != 0:
    raise Exception("CMake build fail")

print("-- running tests ...")
ret = subprocess.run(["ctest", "--test-dir", BUILD_DIR, "-V", "-j", str(os.cpu_count())])
if ret.returncode != 0:
    raise Exception("CTest test fail")
