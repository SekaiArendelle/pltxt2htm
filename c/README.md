# Use `pltxt2htm` in Programming language C

This folder supply C's header-source style interface. (build source code to library and include [pltxt2htm.h](./pltxt2htm.h) in a C project)

## build

> Note: building library still requires C++ toolchains

```sh
xmake config -k [static|shared]
xmake build
xmake install -o $PREFIX
```

## Usage
view [example.c](examples/example.c)

compile `example.c`:
```sh
gcc example.c -o example -L $PATH_OF_PLTXT2HTM_LIB -lpltxt2htm
./example
```
