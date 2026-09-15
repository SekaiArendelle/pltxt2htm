## layout

The build scripts and the shared precompiled header sit at the top level of `tests/`;
every test case lives under `src/`, grouped by subject. `src/syntax/<family>/` mirrors
the syntax families of the library (the same split as `NodeKind`), and the remaining
directories group tests by the internal component they exercise.

```text
src/
├── syntax/
│   ├── html/      HTML tag syntax
│   ├── md/        Markdown syntax (including LaTeX math)
│   ├── pl/        Physics-Lab tag syntax
│   ├── unity/     Unity TextMeshPro syntax
│   └── text/      text-level syntax shared by all families: URLs, escaping,
│                  whitespace, UTF-8 handling, ...
├── parser/        tokenizer and parser internals
├── container/     container types (array, optional, vector, string_view, ...)
├── ast/           AST node semantics (copying, nesting depth, operators)
└── infra/         platform and diagnostics helpers
```

Everything under `src/` is a test, so no file name needs a `test_` prefix, and a test's
ctest name is simply its file name without `.cc` - moving a test between directories
never renames it. Sources are globbed recursively with `CONFIGURE_DEPENDS`, so adding or
moving one does not require editing `CMakeLists.txt`.

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
