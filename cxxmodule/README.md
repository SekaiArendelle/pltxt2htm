## Use pltxt2htm in C++20 module

Requires: clang++ >= 22, g++ >= 16

## Build example.cc
```sh
cmake -S examples -B examples/build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++
cmake --build examples/build
./examples/build/example
```
