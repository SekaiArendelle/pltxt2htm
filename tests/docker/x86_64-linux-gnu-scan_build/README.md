Build and run clang static analyzer (scan-build) on all tests in CI.

Tests run during `docker build` (single RUN command with cmake + scan-build + ctest).

Build:
```
docker build -f tests/docker/x86_64-linux-gnu-scan_build/Dockerfile -t x86_64-linux-gnu-scan-build .
```
