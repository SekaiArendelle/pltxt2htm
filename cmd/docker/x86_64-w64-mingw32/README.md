# Build x86_64-w64-mingw32 pltxt2htm-cmd

## Debug mode
```sh
docker build -f cmd/docker/x86_64-w64-mingw32/Dockerfile --build-arg mode=debug -t x86_64-w64-mingw32-pltxt2htm-cmd-debug .
docker run --rm x86_64-w64-mingw32-pltxt2htm-cmd-debug cat /pltxt2htm/cmd/x86_64-w64-mingw32-pltxt2htm-cmd-debug.zip > x86_64-w64-mingw32-pltxt2htm-cmd-debug.zip
```

Remove docker image
```sh
docker rmi x86_64-w64-mingw32-pltxt2htm-cmd-debug
```

## Release mode
```sh
docker build -f cmd/docker/x86_64-w64-mingw32/Dockerfile --build-arg mode=release -t x86_64-w64-mingw32-pltxt2htm-cmd-release .
docker run --rm x86_64-w64-mingw32-pltxt2htm-cmd-release cat /pltxt2htm/cmd/x86_64-w64-mingw32-pltxt2htm-cmd-release.zip > x86_64-w64-mingw32-pltxt2htm-cmd-release.zip
```

Remove docker image
```sh
docker rmi x86_64-w64-mingw32-pltxt2htm-cmd-release
```
