# Use `pltxt2htm` in py (By [c extension](https://docs.python.org/3/extending/index.html))

## build (with pixi)

Requires [pixi](https://pixi.sh) to be installed.

```sh
# from the repository root, build with Python 3.12
pixi run -e py312 build-py
```

The same task supports `py37` through `py315` environments. Python 3.7 is unavailable on macOS ARM64.
Each environment uses its own build directory: `py/build-pixi-root-pyXY/`.

## Usage
```py
import pltxt2htm

html = pltxt2htm.fixedadv_parser('''
# Hello Quantum PhysicsLab
With markdown supports
''',
    host="localhost",
    project="project",
    visitor="visitor",
    author="author",
    coauthors="coauthors"
)

print(html)
```
