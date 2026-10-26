## Run tests

From the repository root:

```sh
pixi run -e clang test-clang
pixi run -e gcc test-gcc
```

Windows symbol declarations and MSVC linker aliases are generated with Clang's
MSVC targets. After changing their manifest, regenerate and verify them with the
[Windows symbol generator image](../scripts/docker/windows-symbols/README.md).

The C++ standard defaults to `AUTO`: GCC 16 or newer uses C++26 so the tests
exercise supported C++26 code paths, while other compilers use C++23. Override
it with `-DCMAKE_CXX_STANDARD=23` or `-DCMAKE_CXX_STANDARD=26`.

## enable sanitizer:

With pixi (pass the sanitizer name as a positional task argument):

```sh
pixi run -e clang test-clang address
pixi run -e gcc test-gcc address
```

`undefined` and `memory` are accepted where the compiler supports them.

## available cmake options:
| Option | Description |
|---|---|
| `CMAKE_CXX_STANDARD` | C++ standard: `23` or `26`; when unset, GCC 16+ uses C++26 and other compilers use C++23 |
| `PLTXT2HTM_ENABLE_STACKTRACE` | Enable C++23 `<stacktrace>`: `AUTO` (default, detect by compiling/running a probe), `ON` (force), `OFF` (disable). libstdc++ requires linking `libstdc++exp`, which is detected automatically |
| `PLTXT2HTM_SANITIZER` | Sanitizer: `address`, `undefined`, `memory` |
| `PLTXT2HTM_ENABLE_COVERAGE` | Enable code coverage |

## Generate code coverage
Requires GCC plus `lcov` and `genhtml`. Use the container image in [`docker/codecov/`](./docker/codecov/README.md), or configure a local build with `-DPLTXT2HTM_ENABLE_COVERAGE=ON` and run `lcov` + `genhtml` over it.
