Build and run `cxxmodule/tests` with GCC 16 in CI.

## Build Docker image

```sh
docker build -f cxxmodule/docker/x86_64-linux-gnu-cxxmodule-tests/Dockerfile \
  -t pltxt2htm-cxxmodule-tests .
```
