# -*- coding: utf-8 -*-
if __name__ != "__main__":
    raise Exception("This file can not be imported")

import os
import shutil
import argparse

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
BUILD_DIR = os.path.join(SCRIPT_DIR, "build")

if not shutil.which("cmake"):
    raise Exception("cmake not found, type `sudo pacman -S cmake` to install it")
if not shutil.which("ninja"):
    raise Exception("ninja not found, type `sudo pacman -S ninja` to install it")

parser = argparse.ArgumentParser()
parser.add_argument("--toolchain", required=True, choices=("clang", "gcc", "clang-cl"), help="compiler to use")
parser.add_argument("--target", required=True, help="target triplet")
parser.add_argument("--sysroot", help="sysroot to use")
parser.add_argument("--mode", required=True, choices=("release", "debug"), help="build mode")
parser.add_argument("--cxxflags", help="additional C++ flags")
parser.add_argument("--arflags", help="additional archive library flags")
parser.add_argument("--shflags", help="additional shared library flags")
args = parser.parse_args()

if args.toolchain == "gcc" and not shutil.which("g++"):
    raise Exception("g++ not found")
elif args.toolchain == "clang" and not shutil.which("clang++"):
    raise Exception("clang++ not found")
elif args.toolchain == "clang-cl" and not shutil.which("clang-cl"):
    raise Exception("clang-cl not found")

os.chdir(SCRIPT_DIR)

INSTALL_DIR = os.path.join(SCRIPT_DIR, f"{args.target}-pltxt2htm-c-{args.mode}")

if os.path.exists(INSTALL_DIR):
    shutil.rmtree(INSTALL_DIR)
    print(f"remove directory: {INSTALL_DIR}")

if os.path.exists(BUILD_DIR):
    shutil.rmtree(BUILD_DIR)
    print(f"remove directory: {BUILD_DIR}")

# Determine platform and architecture for cmake
if args.target.endswith("-windows-msvc") or args.toolchain == "clang-cl":
    plat = "Windows"
elif args.target.endswith("-w64-mingw32") or args.target.endswith("-windows-gnu"):
    plat = "MinGW"
elif "-linux-" in args.target:
    plat = "Linux"
elif "-apple-darwin" in args.target:
    plat = "Darwin"
else:
    plat = ""

# Build shared library
shared_cmake_cmd = (
    f"cmake -S . -B build-shared -G Ninja "
    f"-DCMAKE_BUILD_TYPE={args.mode} "
    f"-DBUILD_SHARED_LIBS=ON "
)
if plat:
    shared_cmake_cmd += f"-DCMAKE_SYSTEM_NAME={plat} "

if args.toolchain == "clang":
    if args.cxxflags:
        cxxflags = args.cxxflags
        if args.target and not args.target.startswith("native"):
            cxxflags += f" --target={args.target}"
        if args.sysroot:
            cxxflags += f" --sysroot={args.sysroot}"
        shared_cmake_cmd += f"-DCMAKE_CXX_FLAGS=\"{cxxflags}\" "
    if args.shflags:
        shflags = args.shflags
        if args.target and not args.target.startswith("native"):
            shflags += f" --target={args.target}"
        shared_cmake_cmd += f"-DCMAKE_SHARED_LINKER_FLAGS=\"{shflags}\" "
        shared_cmake_cmd += f"-DCMAKE_EXE_LINKER_FLAGS=\"{shflags}\" "
elif args.toolchain == "clang-cl":
    shared_cmake_cmd += "-DCMAKE_CXX_COMPILER=clang-cl "

print(">> ", shared_cmake_cmd)
err_code = os.system(shared_cmake_cmd)
if err_code != 0:
    raise Exception("cmake config (shared) failed")
err_code = os.system("cmake --build build-shared -v")
if err_code != 0:
    raise Exception("cmake build (shared) failed")
err_code = os.system(f"cmake --install build-shared --prefix \"{INSTALL_DIR}\"")
if err_code != 0:
    raise Exception("cmake install (shared) failed")
shutil.rmtree("build-shared")

# Build static library
static_cmake_cmd = (
    f"cmake -S . -B build-static -G Ninja "
    f"-DCMAKE_BUILD_TYPE={args.mode} "
    f"-DBUILD_SHARED_LIBS=OFF "
)
if plat:
    static_cmake_cmd += f"-DCMAKE_SYSTEM_NAME={plat} "

if args.toolchain == "clang":
    if args.cxxflags:
        cxxflags = args.cxxflags
        if args.target and not args.target.startswith("native"):
            cxxflags += f" --target={args.target}"
        if args.sysroot:
            cxxflags += f" --sysroot={args.sysroot}"
        static_cmake_cmd += f"-DCMAKE_CXX_FLAGS=\"{cxxflags}\" "
    if args.arflags:
        arflags = args.arflags
        if args.target and not args.target.startswith("native"):
            arflags += f" --target={args.target}"
        static_cmake_cmd += f"-DCMAKE_STATIC_LINKER_FLAGS=\"{arflags}\" "
elif args.toolchain == "clang-cl":
    static_cmake_cmd += "-DCMAKE_CXX_COMPILER=clang-cl "

print(">> ", static_cmake_cmd)
err_code = os.system(static_cmake_cmd)
if err_code != 0:
    raise Exception("cmake config (static) failed")
err_code = os.system("cmake --build build-static -v")
if err_code != 0:
    raise Exception("cmake build (static) failed")
err_code = os.system(f"cmake --install build-static --prefix \"{INSTALL_DIR}\"")
if err_code != 0:
    raise Exception("cmake install (static) failed")
shutil.rmtree("build-static")

# Copy headers
shutil.copytree(os.path.join(SCRIPT_DIR, "include"), os.path.join(INSTALL_DIR, "include"), dirs_exist_ok=True)
