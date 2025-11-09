# -*- coding: utf-8 -*-
if __name__ != "__main__":
    raise Exception("This file can not be imported")

import os
import shutil
import argparse

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
XMAKE_DIR = os.path.join(SCRIPT_DIR, ".xmake")
BUILD_DIR = os.path.join(SCRIPT_DIR, "build")

if not shutil.which("xmake"):
    raise Exception("xmake not found, type `sudo pacman -S xmake` to install it")

parser = argparse.ArgumentParser()
parser.add_argument("--toolchain", required=True, choices=("clang", "gcc", "clang-cl"), help="compiler to use")
parser.add_argument("--target", required=True, help="target triplet")
parser.add_argument("--sysroot", help="sysroot to use")
parser.add_argument("--mode", required=True, choices=("release", "debug"), help="build mode")
args = parser.parse_args()

if args.toolchain == "gcc" and not shutil.which("g++"):
    raise Exception("g++ not found")
elif args.toolchain == "clang" and not shutil.which("clang++"):
    raise Exception("clang++ not found")

os.chdir(SCRIPT_DIR)

INSTALL_DIR = os.path.join(SCRIPT_DIR, f"{args.target}-pltxt2htm-c-{args.mode}")

if os.path.exists(INSTALL_DIR):
    shutil.rmtree(INSTALL_DIR)
    print(f"remove directory: {INSTALL_DIR}")

if os.path.exists(XMAKE_DIR):
    shutil.rmtree(XMAKE_DIR)
    print(f"remove directory: {XMAKE_DIR}")

if os.path.exists(BUILD_DIR):
    shutil.rmtree(BUILD_DIR)
    print(f"remove directory: {BUILD_DIR}")

if args.sysroot:
    args.sysroot = f"--sysroot={args.sysroot}"
else:
    args.sysroot = ""

if args.toolchain != "clang-cl":
    args.toolchain = f"{args.target}-{args.toolchain}"

if args.toolchain == "clang-cl" or args.target.endswith("-windows-msvc"):
    plat_flag = "-p windows"
elif args.target.endswith("-w64-mingw32") or args.target.endswith("-windows-gnu"):
    plat_flag = "-p mingw"
elif "-linux-" in args.target:
    plat_flag = "-p linux"
elif "-apple-darwin" in args.target:
    plat_flag = "-p macosx"
else:
    plat_flag = ""

if args.target.startswith("x86_64"):
    arch_flag = "-a x64"
elif args.target.startswith("i686"):
    arch_flag = "-a x86"
elif args.target.startswith("aarch64"):
    arch_flag = "-a arm64"
elif args.target.startswith("wasm32"):
    arch_flag = "-a wasm"
else:
    arch_flag = ""

shared_config_cmd = f"xmake config -m {args.mode} -k shared --toolchain={args.toolchain} {args.sysroot} {plat_flag} {arch_flag}"
print(">> ", shared_config_cmd)
err_code = os.system(shared_config_cmd)
if err_code != 0:
    raise Exception("xmake config failed")
err_code = os.system("xmake build -v")
if err_code != 0:
    raise Exception("xmake build failed")
err_code = os.system(f"xmake install -o \"{INSTALL_DIR}\"")
if err_code != 0:
    raise Exception("xmake install failed")

if plat_flag == "-p windows":
    shutil.move(os.path.join(INSTALL_DIR, "lib", "pltxt2htm.lib"), os.path.join(INSTALL_DIR, "bin"))
elif plat_flag == "-p mingw":
    shutil.move(os.path.join(INSTALL_DIR, "lib", "pltxt2htm.dll.a"), os.path.join(INSTALL_DIR, "bin"))

shutil.rmtree(XMAKE_DIR)
shutil.rmtree(BUILD_DIR)

static_config_cmd = f"xmake config -m {args.mode} -k static --toolchain={args.toolchain} {args.sysroot} {plat_flag} {arch_flag}"
print(">> ", static_config_cmd)
err_code = os.system(static_config_cmd)
if err_code != 0:
    raise Exception("xmake config failed")
err_code = os.system("xmake build -v")
if err_code != 0:
    raise Exception("xmake build failed")
err_code = os.system(f"xmake install -o \"{INSTALL_DIR}\"")
if err_code != 0:
    raise Exception("xmake install failed")

# Copy headers
shutil.copytree(os.path.join(SCRIPT_DIR, "include"), os.path.join(INSTALL_DIR, "include"))
