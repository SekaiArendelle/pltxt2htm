Build and run all tests in CI

```sh
docker build -f test/docker/x86_64-linux-gnu/Dockerfile -t x86_64-linux-gnu-test-asan .
docker run -it --name x86_64-linux-gnu-test-asan x86_64-linux-gnu-test-asan
```
