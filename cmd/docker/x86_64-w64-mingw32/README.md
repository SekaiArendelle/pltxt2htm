# Build x86_64-w64-mingw32 pltxt2htm-cmd

## Debug mode
```sh
docker build -f cmd/docker/x86_64-w64-mingw32/Dockerfile --build-arg mode=debug -t x86_64-w64-mingw32-pltxt2htm-cmd-debug .
docker run -d --name x86_64-w64-mingw32-pltxt2htm-cmd-debug x86_64-w64-mingw32-pltxt2htm-cmd-debug
docker cp x86_64-w64-mingw32-pltxt2htm-cmd-debug:/pltxt2htm/cmd/x86_64-w64-mingw32-pltxt2htm-cmd-debug.tar.xz .
# docker rm -f x86_64-w64-mingw32-pltxt2htm-cmd-debug
# docker rmi x86_64-w64-mingw32-pltxt2htm-cmd-debug
```

## Release mode
```sh
docker build -f cmd/docker/x86_64-w64-mingw32/Dockerfile --build-arg mode=release -t x86_64-w64-mingw32-pltxt2htm-cmd-release .
docker run -d --name x86_64-w64-mingw32-pltxt2htm-cmd-release x86_64-w64-mingw32-pltxt2htm-cmd-release
docker cp x86_64-w64-mingw32-pltxt2htm-cmd-release:/pltxt2htm/cmd/x86_64-w64-mingw32-pltxt2htm-cmd-release.tar.xz .
# docker rm -f x86_64-w64-mingw32-pltxt2htm-cmd-release
# docker rmi x86_64-w64-mingw32-pltxt2htm-cmd-release
```
