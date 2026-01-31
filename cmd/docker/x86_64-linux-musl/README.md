# Build x86_64-linux-musl pltxt2htm-cmd

## Debug mode
```sh
docker build -f cmd/docker/x86_64-linux-musl/Dockerfile --build-arg mode=debug -t x86_64-linux-musl-pltxt2htm-cmd-debug .
docker run --rm x86_64-linux-musl-pltxt2htm-cmd-debug cat /pltxt2htm/cmd/x86_64-linux-musl-pltxt2htm-cmd-debug.tar.xz > x86_64-linux-musl-pltxt2htm-cmd-debug.tar.xz
```

Clean up docker image
```sh
docker rmi x86_64-linux-musl-pltxt2htm-cmd-debug
```

## Release mode
```sh
docker build -f cmd/docker/x86_64-linux-musl/Dockerfile --build-arg mode=release -t x86_64-linux-musl-pltxt2htm-cmd-release .
docker run --rm x86_64-linux-musl-pltxt2htm-cmd-release cat /pltxt2htm/cmd/x86_64-linux-musl-pltxt2htm-cmd-release.tar.xz > x86_64-linux-musl-pltxt2htm-cmd-release.tar.xz
```

Clean up docker image
```sh
docker rmi x86_64-linux-musl-pltxt2htm-cmd-release
```
