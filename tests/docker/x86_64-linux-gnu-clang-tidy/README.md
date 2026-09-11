Run `clang-tidy` on all sub-projects (`examples`, `cmd`, `c`) in CI.
clang-tidy runs during `docker build` (single `RUN` command invoking `scripts/run_clang_tidy.py`, which fails the build when any translation unit fails).
Build:
```
docker build -f tests/docker/x86_64-linux-gnu-clang-tidy/Dockerfile -t x86_64-linux-gnu-clang-tidy .
```
