Clang Fuzzer for exported APIs in pltxt2htm

## Launch fuzzing
```sh
python launch-fuzzing.py
```

## Manually Build
chdir to fuzzing/

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++
cmake --build build
```
