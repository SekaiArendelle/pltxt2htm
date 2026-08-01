MemorySanitizer test image

Build (compiles the tests with `-fsanitize=memory`):

```
docker build -f tests/docker/x86_64-linux-gnu-msan/Dockerfile -t x86_64-linux-gnu-tests-msan .
```

Run (executes ctest inside the image):

```
docker run --rm --security-opt seccomp=unconfined x86_64-linux-gnu-tests-msan
```

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
