MemorySanitizer test image

Tests run during `docker build` (single RUN command with cmake + ctest).

Build:
```
docker build -t pltxt2htm-msan .
```

## Seccomp issue

Docker's default seccomp profile only allows `personality()` with arguments
`PER_LINUX(0)`, `PER_LINUX32(1024)`, or `PER_MASK(0xFFFFFFFF)`.
MSan calls `personality(ADDR_NO_RANDOMIZE=0x40000)` at startup to disable ASLR,
which is blocked by the default profile, causing a CHECK failure during build:

```
MemorySanitizer: CHECK failed: msan_linux.cpp:193
  "((personality(old_personality | ADDR_NO_RANDOMIZE))) != ((-1))"
```

This only affects Docker Desktop (Windows/macOS) and environments with strict
seccomp profiles. GitHub Actions runners are unaffected.

## Workarounds

Since tests run at build time, runtime `--security-opt` flags do not apply.
Options:

1. **Split build and test** — move `ctest` to `CMD`, then `docker run --security-opt seccomp=unconfined`
2. **BuildKit `--security=insecure`** — requires `# syntax=docker/dockerfile:1` and
   `DOCKER_BUILDKIT=1`:
   ```dockerfile
   RUN --security=insecure cmake ... && cmake --build build -v && ctest ...
   ```
