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
parser.add_argument("--toolchain", required=True, choices=("clang", "gcc"), help="compiler to use")
parser.add_argument("--target", help="target triplet")
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

if os.path.exists(BUILD_DIR):
    shutil.rmtree(BUILD_DIR)

os.system(f"xmake config -m {args.mode} -k static --toolchain={args.toolchain}")

