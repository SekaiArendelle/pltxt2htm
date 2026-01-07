if __name__ != "__main__":
    raise Exception("This file can't be imported")

import os
import shutil
import platform

if platform.system() != "Linux":
    raise Exception("This script only works on Linux")

if not shutil.which("lcov"):
    raise Exception("lcov not found, type \"sudo pacman -S lcov perl-cpanel-json-xs\" to install it")
if not shutil.which("xmake"):
    raise Exception("xmake not found, type \"sudo pacman -S xmake\" to install it")
if not shutil.which("g++"):
    raise Exception("g++ not found, are you kidding?")

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
BUILD_DIR = os.path.join(SCRIPT_DIR, "build")
XMAKE_DIR = os.path.join(SCRIPT_DIR, ".xmake")
LCOV_REPORT_DIR = os.path.join(SCRIPT_DIR, "lcov-report")

if os.path.exists(BUILD_DIR) and os.path.isdir(BUILD_DIR):
    shutil.rmtree(BUILD_DIR)
    print(f"removing {BUILD_DIR}")
if os.path.exists(XMAKE_DIR) and os.path.isdir(XMAKE_DIR):
    shutil.rmtree(XMAKE_DIR)
    print(f"removing {XMAKE_DIR}")
if os.path.exists(LCOV_REPORT_DIR) and os.path.isdir(LCOV_REPORT_DIR):
    shutil.rmtree(LCOV_REPORT_DIR)
    print(f"removing {LCOV_REPORT_DIR}")

os.chdir(SCRIPT_DIR)
print(f"entering {SCRIPT_DIR}")

err_code = os.system("xmake config --mode=coverage --toolchain=gcc")
if err_code != 0:
    raise Exception("xmake config failed")
err_code = os.system("xmake test")
if err_code != 0:
    raise Exception("xmake test failed")
err_code = os.system("lcov --rc geninfo_unexecuted_blocks=1 --capture --directory build/.objs --output-file build/coverage.info --exclude \"*/fast_io/*\" --exclude \"*/exception/*\" --exclude \"*/c++/*\" --exclude \"*/test/0*.cc\" | grep -v \"^Excluding file\"")
if err_code != 0:
    raise Exception("lcov failed")
err_code = os.system(f"genhtml build/coverage.info --output-directory {os.path.join(SCRIPT_DIR, 'lcov-report')}")
if shutil.which("firefox"):
    os.system(f"firefox {os.path.join(SCRIPT_DIR, 'lcov-report', 'index.html')} &")
