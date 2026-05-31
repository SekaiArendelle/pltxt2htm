import os
import shutil
import platform

if platform.system() != "Linux":
    raise Exception("This script is only for Linux")

if shutil.which("clang") is None:
    raise Exception("clang not found")
if shutil.which("cmake") is None:
    raise Exception("cmake not found")
if shutil.which("nohup") is None:
    raise Exception("nohup not found")

SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
BUILD_DIR = os.path.join(SCRIPT_DIR, "build")
INSTALL_DIR = os.path.join(SCRIPT_DIR, "exe.ignore")
EXECUTABLE_DIR = os.path.join(INSTALL_DIR, "bin")

os.chdir(SCRIPT_DIR)

if os.path.exists(BUILD_DIR) and os.path.isdir(BUILD_DIR):
    shutil.rmtree("build")
    print(f"Removing {BUILD_DIR}")

if os.path.exists(INSTALL_DIR) and os.path.isdir(INSTALL_DIR):
    shutil.rmtree(INSTALL_DIR)
    print(f"Removing {INSTALL_DIR}")

error_code = os.system("cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ -DPLTXT2HTM_SANITIZER=asan")
if error_code != 0:
    raise Exception("cmake configure failed")
error_code = os.system("cmake --build build -v")
if error_code != 0:
    raise Exception("cmake build failed")
error_code = os.system(f"cmake --install build --prefix \"{INSTALL_DIR}\"")
if error_code != 0:
    raise Exception("cmake install failed")

def launch_fuzzing(fuzzer: str):
    executable_path = os.path.join(EXECUTABLE_DIR, fuzzer)
    if not shutil.which(executable_path):
        raise Exception(f"{executable_path} not found")
    stdout_file = os.path.join(SCRIPT_DIR, f"{fuzzer}.stdout.ignore")
    if os.path.exists(stdout_file) and os.path.isfile(stdout_file):
        os.remove(stdout_file)
        print(f"Removing {stdout_file}")
    stderr_file = os.path.join(SCRIPT_DIR, f"{fuzzer}.stderr.ignore")
    if os.path.exists(stderr_file) and os.path.isfile(stderr_file):
        os.remove(stderr_file)
        print(f"Removing {stderr_file}")
    command = f"nohup \"{executable_path}\" 1> \"{stdout_file}\" 2> \"{stderr_file}\" &"
    os.system(command)
    print(f">> {command}")

launch_fuzzing("common_parser")
launch_fuzzing("fixedadv_parser")
launch_fuzzing("plrichtext_parser")
