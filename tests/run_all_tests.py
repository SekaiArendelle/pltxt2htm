if __name__ != "__main__":
    raise Exception("This file can't be imported")

import os
import shutil
import argparse
import subprocess

TEST_DIR: str = os.path.dirname(os.path.abspath(__file__))
BUILD_DIR: str = os.path.join(TEST_DIR, "build")

parser = argparse.ArgumentParser()
parser.add_argument("--compiler", choices=("clang", "gcc", "msvc"), help="compiler to use")
parser.add_argument("--target", help="target triplet")
parser.add_argument("--sysroot", help="sysroot to use")
parser.add_argument("--sanitizer", choices=("address", "undefined", "memory"))
args = parser.parse_args()

if os.path.exists(BUILD_DIR) and os.path.isdir(BUILD_DIR):
    shutil.rmtree(BUILD_DIR)
    print(f"removing {BUILD_DIR}")

if args.compiler is None:
    for candidate in ("clang++", "g++"):
        if shutil.which(candidate) is not None:
            args.compiler = "clang" if candidate == "clang++" else "gcc"
            break
    else:
        if shutil.which("cl.exe") is not None:
            args.compiler = "msvc"
        else:
            raise Exception("no compiler found (tried clang++, g++, cl.exe)")

print(f"-- using compiler \"{args.compiler}\"")

cmake_cmd: list[str] = ["cmake", "-S", TEST_DIR, "-B", BUILD_DIR, "-DCMAKE_BUILD_TYPE=Debug"]

if args.compiler == "msvc":
    cmake_cmd += ["-DCMAKE_CXX_COMPILER=cl"]
else:
    cmake_cmd += [
        "-GNinja",
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
build_cmd: list[str] = ["cmake", "--build", BUILD_DIR, "-j", str(os.cpu_count() or 1)]
if args.compiler != "msvc":
    build_cmd += ["--", "-v"]
ret = subprocess.run(build_cmd)
if ret.returncode != 0:
    raise Exception("CMake build fail")

print("-- running tests ...")
ctest_cmd: list[str] = ["ctest", "--test-dir", BUILD_DIR, "-V", "-j", str(os.cpu_count() or 1)]
if args.compiler == "msvc":
    ctest_cmd += ["-C", "Debug"]
ret = subprocess.run(ctest_cmd)
if ret.returncode != 0:
    raise Exception("CTest test fail")
