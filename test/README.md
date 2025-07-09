## run all tests:
```sh
python ./run_all_tests.py
```

or:
```sh
xmake config
xmake test
```
You can also pass `--toolchain=`, `--sysroot=`, `--arch=`, `--plat=` to `xmake config`.

## enable asan in test:
```sh
xmake config --policies=build.sanitizer.address
xmake test
```

show other snitizers in [xmake doc](https://xmake.io/api/description/builtin-policies.html#build-sanitizer-address)

## enable codecov
```sh
xmake config --mode=coverage --toolchain=gcc
xmake test
lcov --capture --directory build/.objs --output-file build/coverage.info --exclude "*/fast_io/*" --exclude "*/exception/*" --exclude "*/c++/*"
genhtml build/coverage.info --output-directory lcov-report
```
