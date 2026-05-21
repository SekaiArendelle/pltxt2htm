# Build i686-windows-msvc pltxt2htm-cmd

CRT is always linked statically (no `VCRUNTIME140.dll` dependency).

## Debug mode
```sh
docker build -f cmd/docker/i686-windows-msvc/Dockerfile --build-arg mode=debug -t i686-windows-msvc-pltxt2htm-cmd-debug .
docker run --rm i686-windows-msvc-pltxt2htm-cmd-debug cat /pltxt2htm/cmd/i686-windows-msvc-pltxt2htm-cmd-debug.zip > i686-windows-msvc-pltxt2htm-cmd-debug.zip
```

Remove docker image
```sh
docker rmi i686-windows-msvc-pltxt2htm-cmd-debug
```

## Release mode
```sh
docker build -f cmd/docker/i686-windows-msvc/Dockerfile --build-arg mode=release -t i686-windows-msvc-pltxt2htm-cmd-release .
docker run --rm i686-windows-msvc-pltxt2htm-cmd-release cat /pltxt2htm/cmd/i686-windows-msvc-pltxt2htm-cmd-release.zip > i686-windows-msvc-pltxt2htm-cmd-release.zip
```

Remove docker image
```sh
docker rmi i686-windows-msvc-pltxt2htm-cmd-release
```
