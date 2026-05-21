# Build aarch64-windows-msvc pltxt2htm-cmd

CRT is always linked statically (no `VCRUNTIME140.dll` dependency).

## Debug mode
```sh
docker build -f cmd/docker/aarch64-windows-msvc/Dockerfile --build-arg mode=debug -t aarch64-windows-msvc-pltxt2htm-cmd-debug .
docker run --rm aarch64-windows-msvc-pltxt2htm-cmd-debug cat /pltxt2htm/cmd/aarch64-windows-msvc-pltxt2htm-cmd-debug.zip > aarch64-windows-msvc-pltxt2htm-cmd-debug.zip
```

Remove docker image
```sh
docker rmi aarch64-windows-msvc-pltxt2htm-cmd-debug
```

## Release mode
```sh
docker build -f cmd/docker/aarch64-windows-msvc/Dockerfile --build-arg mode=release -t aarch64-windows-msvc-pltxt2htm-cmd-release .
docker run --rm aarch64-windows-msvc-pltxt2htm-cmd-release cat /pltxt2htm/cmd/aarch64-windows-msvc-pltxt2htm-cmd-release.zip > aarch64-windows-msvc-pltxt2htm-cmd-release.zip
```

Remove docker image
```sh
docker rmi aarch64-windows-msvc-pltxt2htm-cmd-release
```
