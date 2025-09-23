# Use `pltxt2htm` in py (By [c extension](https://docs.python.org/3/extending/index.html))

## build
```sh
xmake config --mode=[debug|release]
```
you can specify C++ toolchain e.g. `--toolchain=gcc`, `--toolchain=clang`, `--toolchain=x86_64-w64-mingw32-clang`

> Note: `--toolchain=x86_64-w64-mingw32-clang` means `clang++ --target=x86_64-w64-mingw32`

You can also specify python executable path e.g. `--python=/usr/bin/python3`

then build:
```sh
xmake build
```

> Note: `pip install .` or `python -m build --wheel` based on xmake build script, use `PLTXT2HTM_XMAKE_CONFIG_FLAGS` env var to specify xmake config flags

## Usage
```py
import pltxt2htm

html = pltxt2htm.advanced_parser('''
# Hello Quantum PhysicsLab
With markdown supports
''',
    host="localhost"
)

print(html)
```
