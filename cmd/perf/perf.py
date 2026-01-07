if __name__ != "__main__":
    raise Exception("This file can't be imported")

import os
import shutil
import platform
import argparse

if platform.system() != "Linux":
    raise Exception("This script only works on Linux")

if not shutil.which("perf"):
    raise Exception("perf not found, type \"sudo pacman -S perf\" to install it")
if not shutil.which("xmake"):
    raise Exception("xmake not found, type \"sudo pacman -S xmake\" to install it")
if not shutil.which("stackcollapse-perf.pl") or not shutil.which("flamegraph.pl"):
    raise Exception("FlameGraph not found, download it from 'https://github.com/brendangregg/FlameGraph'")
if not shutil.which("g++"):
    raise Exception("g++ not found, are you kidding?")

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
CMD_DIR = os.path.dirname(SCRIPT_DIR)

def try_rmdir(dir_name: str) -> None:
    if os.path.exists(dir_name) and os.path.isdir(dir_name):
        shutil.rmtree(dir_name)
        print(f"removing {dir_name}")

os.chdir(CMD_DIR)

BUILD_DIR = os.path.join(CMD_DIR, "build")
XMAKE_DIR = os.path.join(CMD_DIR, ".xmake")
INSTALL_DIR = os.path.join(CMD_DIR, "x86_64-linux-gnu-pltxt2htm-cmd-debug")

try_rmdir(BUILD_DIR)
try_rmdir(XMAKE_DIR)
try_rmdir(INSTALL_DIR)

err_code = os.system("xmake config --toolchain=gcc -m debug")
if err_code != 0:
    raise Exception("xmake config failed")
err_code = os.system("xmake build -v")
if err_code != 0:
    raise Exception("xmake build failed")
err_code = os.system(f"xmake install -o \"{INSTALL_DIR}\"")
if err_code != 0:
    raise Exception("xmake install fail")

PERF_DIR = os.path.join(SCRIPT_DIR, "build")
EXE_PATH = os.path.join(INSTALL_DIR, "pltxt2htm")

try_rmdir(PERF_DIR)
os.mkdir(PERF_DIR)
os.chdir(PERF_DIR)

parser = argparse.ArgumentParser()
parser.add_argument("--source", required=True)
args = parser.parse_args()

os.system(f"perf record -F 99 -g -- sh -c 'echo \"{args.source * 10000}\" | {EXE_PATH} --target advanced_html > /dev/null'")
os.system("perf script > out.perf")
os.system("stackcollapse-perf.pl out.perf > out.folded")
os.system("flamegraph.pl out.folded > flame.svg")

if shutil.which("firefox"):
    os.system("firefox flame.svg")
