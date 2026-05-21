# Build wasm32-wasip1 pltxt2htm-cmd

## Debug mode
```sh
docker build -f cmd/docker/wasm32-wasip1/Dockerfile --build-arg mode=debug -t wasm32-wasip1-pltxt2htm-cmd-debug .
docker run --rm wasm32-wasip1-pltxt2htm-cmd-debug cat /pltxt2htm/cmd/wasm32-wasip1-pltxt2htm-cmd-debug.tar.xz > wasm32-wasip1-pltxt2htm-cmd-debug.tar.xz
```

Remove docker image
```sh
docker rmi wasm32-wasip1-pltxt2htm-cmd-debug
```

## Release mode
```sh
docker build -f cmd/docker/wasm32-wasip1/Dockerfile --build-arg mode=release -t wasm32-wasip1-pltxt2htm-cmd-release .
docker run --rm wasm32-wasip1-pltxt2htm-cmd-release cat /pltxt2htm/cmd/wasm32-wasip1-pltxt2htm-cmd-release.tar.xz > wasm32-wasip1-pltxt2htm-cmd-release.tar.xz
```

Remove docker image
```sh
docker rmi wasm32-wasip1-pltxt2htm-cmd-release
```
