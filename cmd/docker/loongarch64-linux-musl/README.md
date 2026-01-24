# Build loongarch64-linux-musl pltxt2htm-cmd

## Debug mode
```sh
docker build -f cmd/docker/loongarch64-linux-musl/Dockerfile --build-arg mode=debug -t loongarch64-linux-musl-pltxt2htm-cmd-debug .
docker run -d --name loongarch64-linux-musl-pltxt2htm-cmd-debug loongarch64-linux-musl-pltxt2htm-cmd-debug
docker cp loongarch64-linux-musl-pltxt2htm-cmd-debug:/pltxt2htm/cmd/loongarch64-linux-musl-pltxt2htm-cmd-debug.tar.xz .
```

Clean up docker container and image
```sh
docker rm -f loongarch64-linux-musl-pltxt2htm-cmd-debug
docker rmi loongarch64-linux-musl-pltxt2htm-cmd-debug
```

## Release mode
```sh
docker build -f cmd/docker/loongarch64-linux-musl/Dockerfile --build-arg mode=release -t loongarch64-linux-musl-pltxt2htm-cmd-release .
docker run -d --name loongarch64-linux-musl-pltxt2htm-cmd-release loongarch64-linux-musl-pltxt2htm-cmd-release
docker cp loongarch64-linux-musl-pltxt2htm-cmd-release:/pltxt2htm/cmd/loongarch64-linux-musl-pltxt2htm-cmd-release.tar.xz .
```

Clean up docker container and image
```sh
docker rm -f loongarch64-linux-musl-pltxt2htm-cmd-release
docker rmi loongarch64-linux-musl-pltxt2htm-cmd-release
```
