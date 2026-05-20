## debug mode

Build docker image
```sh
docker build -f c/docker/x86_64-windows-msvc/Dockerfile --build-arg mode=debug -t x86_64-windows-msvc-pltxt2htm-c-debug .
docker run --rm x86_64-windows-msvc-pltxt2htm-c-debug cat /pltxt2htm/c/x86_64-windows-msvc-pltxt2htm-c-debug.zip > x86_64-windows-msvc-pltxt2htm-c-debug.zip
```

Remove docker image
```sh
docker rmi x86_64-windows-msvc-pltxt2htm-c-debug
```

## release mode

By default, the CRT is linked dynamically (depends on `VCRUNTIME140.dll`). To link it statically
(no `VCRUNTIME140.dll` dependency), set `--build-arg crt_kind=static`.

```sh
# dynamic CRT (default)
docker build -f c/docker/x86_64-windows-msvc/Dockerfile --build-arg mode=release -t x86_64-windows-msvc-pltxt2htm-c-release .
# static CRT
docker build -f c/docker/x86_64-windows-msvc/Dockerfile --build-arg mode=release --build-arg crt_kind=static -t x86_64-windows-msvc-pltxt2htm-c-release .
docker run --rm x86_64-windows-msvc-pltxt2htm-c-release cat /pltxt2htm/c/x86_64-windows-msvc-pltxt2htm-c-release.zip > x86_64-windows-msvc-pltxt2htm-c-release.zip
```

Remove docker image
```sh
docker rmi x86_64-windows-msvc-pltxt2htm-c-release
```
