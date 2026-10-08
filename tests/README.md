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
| `PLTXT2HTM_ENABLE_STACKTRACE` | Enable native stack capture: `AUTO` (default, compile/link detection), `ON` (require a backend), `OFF` (disable). Windows links ntdll; Linux requires execinfo/backtrace. No standard-library stacktrace dependency. |
| `PLTXT2HTM_SANITIZER` | Sanitizer: `address`, `undefined`, `memory` |
| `PLTXT2HTM_ENABLE_COVERAGE` | Enable code coverage |

The native stacktrace API follows the C++23 stacktrace interface:

```cpp
using pltxt2htm::details::stacktrace::Stacktrace;
auto const trace = Stacktrace::current(0, 128);
for (auto const& entry : trace) {
    auto const name = entry.description();
    auto const file = entry.source_file();
    auto const line = entry.source_line();
    auto const address = entry.native_handle();
}
```

`BasicStacktrace<Allocator>` owns dynamically allocated entries using a stateless
fast_io allocator; `Stacktrace` selects `fast_io::native_global_allocator`.
`current()`, `current(skip)` and `current(skip, max_depth)` also accept an allocator
argument. Unlimited capture grows its buffer until the native trace fits or the
backend limit is reached; `possibly_truncated()` reports a reached depth limit.
Unlike the standard library, `skip` counts native frames including implementation
helpers. Forwarding `current()` overloads can add frames in unoptimized builds;
their prefixes need not match `current(skip, max_depth)`. Inlining and tail calls
also affect frame identity, as in the low-level capture API.
Entry text queries return owning `fast_io::string` values. Resolution remains
explicit through those queries (or `resolve(entry.native_handle())`), and copying
or iterating a snapshot does not capture or resolve it again. `at()` terminates
on an invalid index rather than throwing; fast_io allocation failure also
terminates. The low-level `capture`, `resolve` and `ResolvedFrame` APIs remain
available for bounded capture and one-time resolution.

## Generate code coverage
Requires GCC plus `lcov` and `genhtml`. Use the container image in [`docker/codecov/`](./docker/codecov/README.md), or configure a local build with `-DPLTXT2HTM_ENABLE_COVERAGE=ON` and run `lcov` + `genhtml` over it.
