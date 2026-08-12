## debug mode

Build docker image
```sh
docker build -f c/docker/i686-windows-msvc/Dockerfile --build-arg mode=debug -t i686-windows-msvc-pltxt2htm-c-debug .
docker run --rm i686-windows-msvc-pltxt2htm-c-debug cat /pltxt2htm/c/i686-windows-msvc-pltxt2htm-c-debug.zip > i686-windows-msvc-pltxt2htm-c-debug.zip
```

Remove docker image
```sh
docker rmi i686-windows-msvc-pltxt2htm-c-debug
```

## release mode

```sh
docker build -f c/docker/i686-windows-msvc/Dockerfile --build-arg mode=release -t i686-windows-msvc-pltxt2htm-c-release .
docker run --rm i686-windows-msvc-pltxt2htm-c-release cat /pltxt2htm/c/i686-windows-msvc-pltxt2htm-c-release.zip > i686-windows-msvc-pltxt2htm-c-release.zip
```

Remove docker image
```sh
docker rmi i686-windows-msvc-pltxt2htm-c-release
```