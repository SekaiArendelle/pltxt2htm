Clang Fuzzer for exported APIs in pltxt2htm

## Launch fuzzing
```sh
python launch-fuzzing.py
```

## Manually Build
chdir to fuzzing/

```sh
xmake config --toolchain=clang --policies=build.sanitizer.address
# Asan costs a lot, and you can switch to other sanitizers
# xmake config --toolchain=clang --policies=build.sanitizer.undefined
xmake build
```
