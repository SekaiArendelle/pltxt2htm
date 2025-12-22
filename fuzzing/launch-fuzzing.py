import os
import shutil
import platform

if platform.system() != "Linux":
    raise Exception("This script is only for Linux")

if shutil.which("clang") is None:
    raise Exception("clang found")
if shutil.which("xmake") is None:
    raise Exception("xmake not found")
if shutil.which("nohup") is None:
    raise Exception("nohup not found")

SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
BUILD_DIR = os.path.join(SCRIPT_DIR, "build")
XMAKE_DIR = os.path.join(SCRIPT_DIR, ".xmake")
INSTALL_DIR = os.path.join(SCRIPT_DIR, "exe.ignore")
EXECUTABLE_DIR = os.path.join(INSTALL_DIR, "bin")

os.chdir(SCRIPT_DIR)

if os.path.exists(BUILD_DIR) and os.path.isdir(BUILD_DIR):
    shutil.rmtree("build")
    print(f"Removing {BUILD_DIR}")
if os.path.exists(XMAKE_DIR) and os.path.isdir(XMAKE_DIR):
    print(f"Removing {XMAKE_DIR}")
    shutil.rmtree(XMAKE_DIR)
if os.path.exists(INSTALL_DIR) and os.path.isdir(INSTALL_DIR):
    print(f"Removing {INSTALL_DIR}")
    shutil.rmtree(INSTALL_DIR)

error_code = os.system("xmake config --toolchain=clang --policies=build.sanitizer.address")
if error_code != 0:
    raise Exception("xmake config failed")
error_code = os.system("xmake build -v")
if error_code != 0:
    raise Exception("xmake build failed")
error_code = os.system(f"xmake install -o \"{INSTALL_DIR}\"")
if error_code != 0:
    raise Exception("xmake install failed")

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
launch_fuzzing("advanced_parser")
