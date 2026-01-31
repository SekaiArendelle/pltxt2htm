# Build aarch64-linux-musl pltxt2htm-cmd

## Debug mode
```sh
docker build -f cmd/docker/aarch64-linux-musl/Dockerfile --build-arg mode=debug -t aarch64-linux-musl-pltxt2htm-cmd-debug .
docker run --rm aarch64-linux-musl-pltxt2htm-cmd-debug cat /pltxt2htm/cmd/aarch64-linux-musl-pltxt2htm-cmd-debug.tar.xz > aarch64-linux-musl-pltxt2htm-cmd-debug.tar.xz
```

Clean up docker image
```sh
docker rmi aarch64-linux-musl-pltxt2htm-cmd-debug
```

## Release mode
```sh
docker build -f cmd/docker/aarch64-linux-musl/Dockerfile --build-arg mode=release -t aarch64-linux-musl-pltxt2htm-cmd-release .
docker run --rm aarch64-linux-musl-pltxt2htm-cmd-release cat /pltxt2htm/cmd/aarch64-linux-musl-pltxt2htm-cmd-release.tar.xz > aarch64-linux-musl-pltxt2htm-cmd-release.tar.xz
```

Clean up docker image
```sh
docker rmi aarch64-linux-musl-pltxt2htm-cmd-release
```
