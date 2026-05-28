# Use `pltxt2htm` in py (By [c extension](https://docs.python.org/3/extending/index.html))

## build (with pixi)

Requires [pixi](https://pixi.sh) to be installed.

```sh
# install environment & build (default: Python 3.12)
pixi run build
```

Specify Python version via environment:

```sh
pixi run -e py310  build
pixi run -e py311  build
pixi run -e py312  build   # default
pixi run -e py313  build
```

The built `.so`/`.pyd` will be placed under `build/`.

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
