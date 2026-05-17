Build docker image that generates lcov coverage report.

```sh
docker build -f tests/docker/codecov/Dockerfile -t pltxt2htm-codecov .
docker run --rm pltxt2htm-codecov cat /pltxt2htm-codecov.tar.xz > pltxt2htm-codecov.tar.xz
```

Remove docker image
```sh
docker rmi pltxt2htm-codecov
```
