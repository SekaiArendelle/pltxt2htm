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
if not shutil.which("cmake"):
    raise Exception("cmake not found, type \"sudo pacman -S cmake\" to install it")
if not shutil.which("ninja"):
    raise Exception("ninja not found, type \"sudo pacman -S ninja\" to install it")
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
INSTALL_DIR = os.path.join(CMD_DIR, "x86_64-linux-gnu-pltxt2htm-cmd-debug")

try_rmdir(BUILD_DIR)
try_rmdir(INSTALL_DIR)

err_code = os.system("cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++")
if err_code != 0:
    raise Exception("cmake config failed")
err_code = os.system("cmake --build build -v")
if err_code != 0:
    raise Exception("cmake build failed")
err_code = os.system(f"cmake --install build --prefix \"{INSTALL_DIR}\"")
if err_code != 0:
    raise Exception("cmake install fail")

PERF_DIR = os.path.join(SCRIPT_DIR, "build")
EXE_PATH = os.path.join(INSTALL_DIR, "pltxt2htm")

try_rmdir(PERF_DIR)
os.mkdir(PERF_DIR)
os.chdir(PERF_DIR)

parser = argparse.ArgumentParser()
parser.add_argument("--source", required=True)
args = parser.parse_args()

os.system(f"perf record -F 99 -g -- sh -c 'echo \"{args.source * 10000}\" | {EXE_PATH} --target html4unittest > /dev/null'")
os.system("perf script > out.perf")
os.system("stackcollapse-perf.pl out.perf > out.folded")
os.system("flamegraph.pl out.folded > flame.svg")

if shutil.which("firefox"):
    os.system("firefox flame.svg")
