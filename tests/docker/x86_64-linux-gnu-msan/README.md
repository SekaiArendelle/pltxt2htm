MemorySanitizer test image

The tests are compiled with `-fsanitize=memory` and linked against an
MSan-instrumented libc++, because MSan reports false positives when part of the
program is not instrumented.

Build (compiles the tests with `-fsanitize=memory`):

```
docker build -f tests/docker/x86_64-linux-gnu-msan/Dockerfile -t x86_64-linux-gnu-tests-msan .
```

Run (executes ctest inside the image):

```
docker run --rm --security-opt seccomp=unconfined x86_64-linux-gnu-tests-msan
```

## Instrumented C++ standard library

MSan tracks initializedness in shadow memory, and only instrumented code propagates
that shadow: a value written by uninstrumented code stays "uninitialized", so the
first instrumented read of it is a false positive. The C++ standard library counts
as uninstrumented code: Ubuntu's libstdc++ and libc++ are not built with
`-fsanitize=memory`, so a plain `ubuntu:*` image cannot run this job. The false
positive is neither subtle nor specific to the library under test: with Ubuntu's
libstdc++, even `std::map<std::pair<int, std::string>>::insert` reports

```
WARNING: MemorySanitizer: use-of-uninitialized-value
    #0 ... std::_Rb_tree<...>::_S_left(std::_Rb_tree_node_base*) .../c++/15/bits/stl_tree.h:1425:9
    #4 doctest::detail::registerReporterImpl(...) tests/vendor/doctest/doctest.h:7101:28
SUMMARY: MemorySanitizer: use-of-uninitialized-value ... in doctest::detail::registerReporterImpl
```

which aborts during doctest's global reporter registration, before any test body
runs; `syntax_tests` fails while the other test binaries happen not to hit it.

The Dockerfile therefore builds on `gcr.io/oss-fuzz-base/base-clang`, the OSS-Fuzz
compiler image, which builds libc++ with `-DLLVM_USE_SANITIZER=Memory` and installs
it under `/usr/msan`:

| Path | Contents |
| ---- | -------- |
| `/usr/msan/include/c++/v1` | instrumented libc++ headers |
| `/usr/msan/lib/libc++.a` | instrumented libc++ static library |

`CXXFLAGS` points the compiler and linker at it:

```
-stdlib=libc++ -nostdinc++ -isystem /usr/msan/include/c++/v1 -L/usr/msan/lib
```

`-nostdinc++` is required: without it the C++ headers still come from the
uninstrumented libstdc++, and the false positives return.

`libc++.a` is the only C++ library in `/usr/msan/lib`; there is no separate
`libc++abi.a`, because OSS-Fuzz builds libc++ with `LIBCXX_ENABLE_STATIC_ABI_LIBRARY=ON`
and folds the libc++abi objects into it (it defines the `__cxa_*` entry points). The
image definition is
[`infra/base-images/base-clang/checkout_build_install_llvm.sh`](https://github.com/google/oss-fuzz/blob/master/infra/base-images/base-clang/checkout_build_install_llvm.sh)
in OSS-Fuzz.

The base image is pinned by digest so that the compiler and libc++ versions do not
change under us. `docker manifest inspect gcr.io/oss-fuzz-base/base-clang` prints the
digest of the current `latest` tag if it ever needs bumping, and upstream does not
promise that an older digest stays pullable: if the pinned digest stops resolving, bump
it to the current one and re-run this image locally first, because that is a base image
upgrade rather than a like-for-like replacement.

Two consequences of using libc++: the configure step does not find a working
`<stacktrace>` and reports `PLTXT2HTM_ENABLE_STACKTRACE: disabled`, so the tests run
without that optional feature; and base-clang is a compiler image rather than a
general build environment, so the Dockerfile installs `ninja-build` itself.

## Seccomp issue

MSan calls `personality(ADDR_NO_RANDOMIZE)` at startup to disable ASLR. Docker's
default seccomp profile only allows `personality()` with arguments `PER_LINUX(0)`,
`PER_LINUX32(1024)`, or `PER_MASK(0xFFFFFFFF)`, so this call is blocked and MSan
aborts during startup:

```
MemorySanitizer: CHECK failed: msan_linux.cpp:193
  "((personality(old_personality | ADDR_NO_RANDOMIZE))) != ((-1))"
```

Tests therefore do not run during `docker build`; they run via
`docker run --security-opt seccomp=unconfined`. This affects Docker Desktop
(Windows/macOS) and environments with strict seccomp profiles. GitHub Actions
runners are unaffected, but the `--security-opt` flag is harmless there.
