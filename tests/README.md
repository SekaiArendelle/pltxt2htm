## Run tests

From the repository root:

```sh
pixi run -e clang test-clang
pixi run -e gcc test-gcc
```

For a clean rebuild of the Pixi build trees, run `pixi run clean-tests` before the
test task. For other compilers or custom CMake options, configure the test
project directly.

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

Or configure the test project directly:

```sh
cmake -S tests -B tests/build -GNinja -DPLTXT2HTM_SANITIZER=address
cmake --build tests/build
ctest --test-dir tests/build --interactive-debug-mode 0
```

or `-DPLTXT2HTM_SANITIZER=undefined` or `-DPLTXT2HTM_SANITIZER=memory`.

On Windows targets using the MSVC ABI (including clang), sanitizer builds link the release
dynamic CRT (`MultiThreadedDLL`) even when `CMAKE_BUILD_TYPE` is `Debug`. The debug CRT
selected by Debug, together with the `_DEBUG`-only allocator paths, is not tracked by the
sanitizer runtime, so most tests abort with a bogus "attempting free on address which was
not malloc()-ed" report. The Debug optimization level and the library's contract checks are
kept, but `_DEBUG` is not defined, so `_ITERATOR_DEBUG_LEVEL` falls back to `0`. A plain
release `CMAKE_MSVC_RUNTIME_LIBRARY` (for example `MultiThreaded` for a static CRT) is
respected; debug and per-config generator-expression values are rejected at configure time.
Add sanitizer flags through `PLTXT2HTM_SANITIZER` only - `-fsanitize=address` in
`CMAKE_CXX_FLAGS` or `CXXFLAGS` bypasses this check and brings the failure back.

## available cmake options:
| Option | Description |
|---|---|
| `CMAKE_CXX_STANDARD` | C++ standard: `23` or `26`; when unset, GCC 16+ uses C++26 and other compilers use C++23 |
| `PLTXT2HTM_ENABLE_STACKTRACE` | Enable C++23 `<stacktrace>`: `AUTO` (default, detect by compiling/running a probe), `ON` (force), `OFF` (disable). libstdc++ requires linking `libstdc++exp`, which is detected automatically |
| `PLTXT2HTM_SANITIZER` | Sanitizer: `address`, `undefined`, `memory` |
| `PLTXT2HTM_ENABLE_COVERAGE` | Enable code coverage |

## Generate code coverage
Requires GCC plus `lcov` and `genhtml`. Use the container image in [`docker/codecov/`](./docker/codecov/README.md), or configure a local build with `-DPLTXT2HTM_ENABLE_COVERAGE=ON` and run `lcov` + `genhtml` over it.
