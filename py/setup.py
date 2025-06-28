import os
import shutil
import platform
import setuptools
from setuptools.command.build_ext import build_ext

# I do not plan to distribute this package to pypi
# Therefore, I'm ensure this script is fucking hard to use
# do not try to `python -m build` to expect this script
# would reduce your works.
# Instead, use xmake to build this package directly

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

class XMakeBuild(build_ext):
    def build_extension(self, ext):
        if not shutil.which("xmake"):
            raise Exception("xmake not found")

        xmake_config_flags = os.environ.get("PLTXT2HTM_XMAKE_CONFIG_FLAGS", "")
        config_cmd = f"xmake config --mode=release {xmake_config_flags}"
        print(">>", config_cmd)
        config_result = os.system(config_cmd)
        if config_result != 0:
            raise Exception("xmake config failed")

        build_cmd = "xmake build"
        print(">>", build_cmd)
        build_result = os.system(build_cmd)
        if build_result != 0:
            raise Exception("xmake build failed")

setuptools.setup(
    name="pltxt2htm",
    version="0.0.0",
    author="Arendelle",
    author_email="2381642961@qq.com",
    description="Parse Quantam-Physics's text to html",
    long_description="",
    long_description_content_type="text/markdown",
    url="https://github.com/GoodenoughPhysicsLab/pltxt2htm",
    python_requires=">=3.8",
    cmdclass={
        "build_ext": XMakeBuild,
    },
    ext_modules=[
        # build rules in xmake.lua
        setuptools.Extension("_", sources=["_"])
    ]
)
