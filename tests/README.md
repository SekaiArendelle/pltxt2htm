## run all tests:
```sh
python ./run_all_tests.py
```

or (with cmake + ninja):
```sh
cmake -S tests -B tests/build -GNinja
cmake --build tests/build
ctest --test-dir tests/build
```

You can also pass compiler via `-DCMAKE_CXX_COMPILER=clang++` or `-DCMAKE_CXX_COMPILER=g++`.

## enable sanitizer:
```sh
cmake -S tests -B tests/build -GNinja -DPLTXT2HTM_SANITIZER=address
cmake --build tests/build
ctest --test-dir tests/build
```

or `-DPLTXT2HTM_SANITIZER=undefined` or `-DPLTXT2HTM_SANITIZER=memory`.

## available cmake options:
| Option | Description |
|---|---|
| `PLTXT2HTM_ENABLE_STACKTRACE` | Enable C++23 `<stacktrace>` |
| `PLTXT2HTM_SANITIZER` | Sanitizer: `address`, `undefined`, `memory` |
| `PLTXT2HTM_ENABLE_COVERAGE` | Enable code coverage |

## Generate Code Coverage
```sh
python ./codecov.py
```
