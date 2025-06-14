import os
import sys
import sysconfig

if __name__ != "__main__":
    raise Exception("This file can't be imported")

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

py_version_nodot = f"{sys.version_info.major}{sys.version_info.minor}"
lib_plat_cpy_dir = os.path.join(SCRIPT_DIR, "build", f"lib.{sysconfig.get_platform()}-cpython-{py_version_nodot}")
if not os.path.exists(lib_plat_cpy_dir) and not os.path.isdir(lib_plat_cpy_dir):
    os.mkdir(lib_plat_cpy_dir)
