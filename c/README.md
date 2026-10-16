# Use `pltxt2htm` in Programming language C

This folder supply C's header-source style interface. (build source code to library and include [pltxt2htm.h](./pltxt2htm.h) in a C project)

## Build

> Note: building library still requires C++ toolchains

From the repository root, build either library variant with Pixi:

```sh
pixi run -e clang build-c-static
pixi run -e clang build-c-shared
```

These tasks build the library only. To install it, configure the desired
target and installation prefix with CMake.

## Usage
view [example.c](examples/example.c)

compile `example.c`:
```sh
gcc example.c -o example -L $PATH_OF_PLTXT2HTM_LIB -lpltxt2htm
./example
```
