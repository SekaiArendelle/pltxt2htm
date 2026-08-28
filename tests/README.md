## run all tests:
```sh
python ./run_all_tests.py
```

or (with cmake + ninja):
```sh
cmake -S tests -B tests/build -GNinja
cmake --build tests/build
ctest --test-dir tests/build --interactive-debug-mode 0
```

You can also pass compiler via `-DCMAKE_CXX_COMPILER=clang++` or `-DCMAKE_CXX_COMPILER=g++`.

The C++ standard defaults to `AUTO`: GCC 16 or newer uses C++26 so the tests
exercise supported C++26 code paths, while other compilers use C++23. Override
it with `-DCMAKE_CXX_STANDARD=23` or `-DCMAKE_CXX_STANDARD=26`. When
using `run_all_tests.py`, pass `--cxx-standard 23` or `--cxx-standard 26`.

## enable sanitizer:
```sh
cmake -S tests -B tests/build -GNinja -DPLTXT2HTM_SANITIZER=address
cmake --build tests/build
ctest --test-dir tests/build --interactive-debug-mode 0
```

or `-DPLTXT2HTM_SANITIZER=undefined` or `-DPLTXT2HTM_SANITIZER=memory`.

## available cmake options:
| Option | Description |
|---|---|
| `CMAKE_CXX_STANDARD` | C++ standard: `23` or `26`; when unset, GCC 16+ uses C++26 and other compilers use C++23 |
| `PLTXT2HTM_ENABLE_STACKTRACE` | Enable C++23 `<stacktrace>`: `AUTO` (default, detect by compiling/running a probe), `ON` (force), `OFF` (disable). libstdc++ requires linking `libstdc++exp`, which is detected automatically |
| `PLTXT2HTM_SANITIZER` | Sanitizer: `address`, `undefined`, `memory` |
| `PLTXT2HTM_ENABLE_COVERAGE` | Enable code coverage |

## Generate Code Coverage
```sh
python ./codecov.py
```
