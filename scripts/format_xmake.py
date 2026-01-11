import os
import shutil


def format_files_in_a_dir(dir: str) -> None:
    for root, dirs, files in os.walk(dir):
        for file in files:
            if file == "xmake.lua":
                cmd = f'lua-format -i "{os.path.join(root, file)}"'
                os.system(cmd)
                print(">> ", cmd)


def main():
    SCRIPT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

    # lua-format from https://github.com/Koihik/LuaFormatter
    if not shutil.which("lua-format"):
        raise Exception("lua-format not found")

    format_files_in_a_dir(SCRIPT_DIR)


if __name__ == "__main__":
    main()
