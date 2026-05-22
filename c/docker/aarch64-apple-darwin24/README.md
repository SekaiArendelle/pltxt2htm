## debug mode

```sh
docker build -f c/docker/aarch64-apple-darwin24/Dockerfile --build-arg mode=debug -t aarch64-apple-darwin24-pltxt2htm-c-debug .
docker run --rm aarch64-apple-darwin24-pltxt2htm-c-debug cat /pltxt2htm/c/aarch64-apple-darwin24-pltxt2htm-c-debug.tar.xz > aarch64-apple-darwin24-pltxt2htm-c-debug.tar.xz
```

## release mode

```sh
docker build -f c/docker/aarch64-apple-darwin24/Dockerfile --build-arg mode=release -t aarch64-apple-darwin24-pltxt2htm-c-release .
docker run --rm aarch64-apple-darwin24-pltxt2htm-c-release cat /pltxt2htm/c/aarch64-apple-darwin24-pltxt2htm-c-release.tar.xz > aarch64-apple-darwin24-pltxt2htm-c-release.tar.xz
```

Remove docker image
```sh
docker rmi aarch64-apple-darwin24-pltxt2htm-c-release
```
