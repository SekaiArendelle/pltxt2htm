# Build loongarch64-linux-musl pltxt2htm-cmd

## Debug mode
```sh
docker build -f cmd/docker/loongarch64-linux-musl/Dockerfile --build-arg mode=debug -t loongarch64-linux-musl-pltxt2htm-cmd-debug .
docker run --rm loongarch64-linux-musl-pltxt2htm-cmd-debug cat /pltxt2htm/cmd/loongarch64-linux-musl-pltxt2htm-cmd-debug.tar.xz > loongarch64-linux-musl-pltxt2htm-cmd-debug.tar.xz
```

Clean up docker image
```sh
docker rmi loongarch64-linux-musl-pltxt2htm-cmd-debug
```

## Release mode
```sh
docker build -f cmd/docker/loongarch64-linux-musl/Dockerfile --build-arg mode=release -t loongarch64-linux-musl-pltxt2htm-cmd-release .
docker run --rm loongarch64-linux-musl-pltxt2htm-cmd-release cat /pltxt2htm/cmd/loongarch64-linux-musl-pltxt2htm-cmd-release.tar.xz > loongarch64-linux-musl-pltxt2htm-cmd-release.tar.xz
```

Clean up docker image
```sh
docker rmi loongarch64-linux-musl-pltxt2htm-cmd-release
```
