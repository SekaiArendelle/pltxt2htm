# -*- coding: utf-8 -*-
if __name__ != "__main__":
    raise Exception("This file can not be imported")

import os
import shutil
import argparse

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

if not shutil.which("xmake"):
    raise Exception("xmake not found, type `sudo pacman -S xmake` to install it")

parser = argparse.ArgumentParser()
parser.add_argument("--compiler", choices=("clang", "gcc"), help="compiler to use")
parser.add_argument("--target", help="target triplet")
parser.add_argument("--sysroot", help="sysroot to use")
args = parser.parse_args()

if args.compiler == "gcc" and not shutil.which("g++"):
    raise Exception("g++ not found")
elif args.compiler == "clang" and not shutil.which("clang++"):
    raise Exception("clang++ not found")

