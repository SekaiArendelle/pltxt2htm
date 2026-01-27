# Build i586-linux-musl pltxt2htm-cmd

## Debug mode
```sh
docker build -f cmd/docker/i586-linux-musl/Dockerfile --build-arg mode=debug -t i586-linux-musl-pltxt2htm-cmd-debug .
docker run -d --name i586-linux-musl-pltxt2htm-cmd-debug i586-linux-musl-pltxt2htm-cmd-debug
docker cp i586-linux-musl-pltxt2htm-cmd-debug:/pltxt2htm/cmd/i586-linux-musl-pltxt2htm-cmd-debug.tar.xz .
```

Clean up docker container and image
```sh
docker rm -f i586-linux-musl-pltxt2htm-cmd-debug
docker rmi i586-linux-musl-pltxt2htm-cmd-debug
```

## Release mode
```sh
docker build -f cmd/docker/i586-linux-musl/Dockerfile --build-arg mode=release -t i586-linux-musl-pltxt2htm-cmd-release .
docker run -d --name i586-linux-musl-pltxt2htm-cmd-release i586-linux-musl-pltxt2htm-cmd-release
docker cp i586-linux-musl-pltxt2htm-cmd-release:/pltxt2htm/cmd/i586-linux-musl-pltxt2htm-cmd-release.tar.xz .
```

Clean up docker container and image
```sh
docker rm -f i586-linux-musl-pltxt2htm-cmd-release
docker rmi i586-linux-musl-pltxt2htm-cmd-release
```
