# Build i586-linux-musl pltxt2htm-cmd

## Debug mode
```sh
docker build -f cmd/docker/i586-linux-musl/Dockerfile --build-arg mode=debug -t i586-linux-musl-pltxt2htm-cmd-debug .
docker run --rm i586-linux-musl-pltxt2htm-cmd-debug cat /pltxt2htm/cmd/i586-linux-musl-pltxt2htm-cmd-debug.tar.xz > i586-linux-musl-pltxt2htm-cmd-debug.tar.xz
```

Clean up docker image
```sh
docker rmi i586-linux-musl-pltxt2htm-cmd-debug
```

## Release mode
```sh
docker build -f cmd/docker/i586-linux-musl/Dockerfile --build-arg mode=release -t i586-linux-musl-pltxt2htm-cmd-release .
docker run --rm i586-linux-musl-pltxt2htm-cmd-release cat /pltxt2htm/cmd/i586-linux-musl-pltxt2htm-cmd-release.tar.xz > i586-linux-musl-pltxt2htm-cmd-release.tar.xz
```

Clean up docker image
```sh
docker rmi i586-linux-musl-pltxt2htm-cmd-release
```
