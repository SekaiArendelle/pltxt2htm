if __name__ != "__main__":
    raise Exception("This file can't be imported")

import os
import shutil
import platform

if platform.system() != "Linux":
    raise Exception("This script only works on Linux")

if not shutil.which("lcov"):
    raise Exception("lcov not found, type \"sudo pacman -S lcov perl-cpanel-json-xs\" to install it")
if not shutil.which("cmake"):
    raise Exception("cmake not found")
if not shutil.which("g++"):
    raise Exception("g++ not found, are you kidding?")

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
BUILD_DIR = os.path.join(SCRIPT_DIR, "build")
LCOV_REPORT_DIR = os.path.join(SCRIPT_DIR, "lcov-report")

if os.path.exists(BUILD_DIR) and os.path.isdir(BUILD_DIR):
    shutil.rmtree(BUILD_DIR)
    print(f"removing {BUILD_DIR}")
if os.path.exists(LCOV_REPORT_DIR) and os.path.isdir(LCOV_REPORT_DIR):
    shutil.rmtree(LCOV_REPORT_DIR)
    print(f"removing {LCOV_REPORT_DIR}")

os.chdir(SCRIPT_DIR)
print(f"entering {SCRIPT_DIR}")

err_code = os.system(f"cmake -S \"{SCRIPT_DIR}\" -B \"{BUILD_DIR}\" {"-GNinja" if shutil.which("ninja") else ""} -DCMAKE_BUILD_TYPE=Debug -DPLTXT2HTM_ENABLE_COVERAGE=ON -DCMAKE_CXX_COMPILER=g++")
if err_code != 0:
    raise Exception("cmake config failed")
err_code = os.system(f"cmake --build \"{BUILD_DIR}\" -j {os.process_cpu_count()} -v")
if err_code != 0:
    raise Exception("cmake build failed")
err_code = os.system(f"ctest --test-dir \"{BUILD_DIR}\" -j {os.process_cpu_count()}")
if err_code != 0:
    raise Exception("ctest failed")
err_code = os.system(f"lcov --rc geninfo_unexecuted_blocks=1 --capture --directory \"{BUILD_DIR}\" --output-file \"{BUILD_DIR}/coverage.info\" --exclude \"*/fast_io/*\" --exclude \"*/exception/*\" --exclude \"*/c++/*\" | grep -v \"^Excluding file\"")
if err_code != 0:
    raise Exception("lcov failed")
err_code = os.system(f"genhtml \"{BUILD_DIR}/coverage.info\" --output-directory {os.path.join(SCRIPT_DIR, 'lcov-report')}")
if shutil.which("firefox"):
    os.system(f"firefox {os.path.join(SCRIPT_DIR, 'lcov-report', 'index.html')} &")
