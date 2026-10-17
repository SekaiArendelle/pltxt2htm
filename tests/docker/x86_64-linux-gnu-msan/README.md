# MemorySanitizer test image

The tests are compiled with `-fsanitize=memory` and linked against an
MSan-instrumented libc++.

Build the image:

```sh
docker build -f tests/docker/x86_64-linux-gnu-msan/Dockerfile -t x86_64-linux-gnu-tests-msan .
```

Run the tests:

```sh
docker run --rm --security-opt seccomp=unconfined x86_64-linux-gnu-tests-msan
```

## Instrumented C++ standard library

Code in an uninstrumented C++ standard library does not reliably update MSan's
shadow memory and can cause false positives. Distribution-provided libstdc++ and
libc++ are not built for MSan, so the image uses the instrumented libc++ provided by
`gcr.io/oss-fuzz-base/base-clang` under `/usr/msan`:

| Path | Contents |
| ---- | -------- |
| `/usr/msan/include/c++/v1` | matching libc++ headers |
| `/usr/msan/lib/libc++.a` | instrumented libc++ static library |

The Dockerfile selects them with:

```text
-stdlib=libc++ -nostdinc++ -isystem /usr/msan/include/c++/v1 -L/usr/msan/lib
```

`-nostdinc++` prevents Clang from selecting distribution C++ headers, while the
explicit include and library paths keep the headers and instrumented library paired.

## Updating the base image

The image is pinned by digest so that its compiler and libc++ do not change
independently. To update it, find the digest shown by:

```sh
docker buildx imagetools inspect gcr.io/oss-fuzz-base/base-clang:latest
```

Replace the digest in the Dockerfile, then rebuild and run the image locally.

Native stacktrace support uses glibc's `backtrace`, independently of libc++'s
`<stacktrace>` support. Configure `-DPLTXT2HTM_ENABLE_STACKTRACE=OFF` when a
sanitizer setup cannot safely unwind through its runtime.

## Seccomp issue

MSan calls `personality(ADDR_NO_RANDOMIZE)` at startup to disable ASLR. Docker's
default seccomp profile can block that call, causing MSan to abort before the tests
start. Tests therefore run via `docker run --security-opt seccomp=unconfined`
instead of during `docker build`.
