# Build aarch64-linux-musl pltxt2htm-cmd

## Debug mode
```sh
docker build -f cmd/docker/aarch64-linux-musl/Dockerfile --build-arg mode=debug -t aarch64-linux-musl-pltxt2htm-cmd-debug .
docker run -d --name aarch64-linux-musl-pltxt2htm-cmd-debug aarch64-linux-musl-pltxt2htm-cmd-debug
docker cp aarch64-linux-musl-pltxt2htm-cmd-debug:/pltxt2htm/cmd/aarch64-linux-musl-pltxt2htm-cmd-debug.tar.xz .
```

Clean up docker container and image
```sh
docker rm -f aarch64-linux-musl-pltxt2htm-cmd-debug
docker rmi aarch64-linux-musl-pltxt2htm-cmd-debug
```

## Release mode
```sh
docker build -f cmd/docker/aarch64-linux-musl/Dockerfile --build-arg mode=release -t aarch64-linux-musl-pltxt2htm-cmd-release .
docker run -d --name aarch64-linux-musl-pltxt2htm-cmd-release aarch64-linux-musl-pltxt2htm-cmd-release
docker cp aarch64-linux-musl-pltxt2htm-cmd-release:/pltxt2htm/cmd/aarch64-linux-musl-pltxt2htm-cmd-release.tar.xz .
```

Clean up docker container and image
```sh
docker rm -f aarch64-linux-musl-pltxt2htm-cmd-release
docker rmi aarch64-linux-musl-pltxt2htm-cmd-release
```
