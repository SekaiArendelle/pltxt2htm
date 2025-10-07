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

if os.path.exists(XMAKE_DIR):
    shutil.rmtree(XMAKE_DIR)
    print(f"removed directory: {XMAKE_DIR}")

if os.path.exists(BUILD_DIR):
    shutil.rmtree(BUILD_DIR)
    print(f"removed directory: {BUILD_DIR}")

if args.sysroot:
    args.sysroot = f"--sysroot={args.sysroot}"
else:
    args.sysroot = ""

if args.toolchain != "clang-cl":
    args.toolchain = f"{args.target}-{args.toolchain}"

static_config_cmd = f"xmake config -m {args.mode} -k static --toolchain={args.toolchain} {args.sysroot}"
print(">> ", static_config_cmd)
err_code = os.system(static_config_cmd)
if err_code != 0:
    raise Exception("xmake config failed")
err_code = os.system("xmake build -v")
if err_code != 0:
    raise Exception("xmake build failed")
err_code = os.system(f"xmake install -o \"{args.target}-pltxt2htm-c-{args.mode}\"")
if err_code != 0:
    raise Exception("xmake install failed")

shutil.rmtree(XMAKE_DIR)
shutil.rmtree(BUILD_DIR)

shared_config_cmd = f"xmake config -m {args.mode} -k shared --toolchain={args.toolchain} {args.sysroot}"
print(">> ", shared_config_cmd)
err_code = os.system(shared_config_cmd)
if err_code != 0:
    raise Exception("xmake config failed")
err_code = os.system("xmake build -v")
if err_code != 0:
    raise Exception("xmake build failed")
err_code = os.system(f"xmake install -o \"{args.target}-pltxt2htm-c-{args.mode}\"")
if err_code != 0:
    raise Exception("xmake install failed")
