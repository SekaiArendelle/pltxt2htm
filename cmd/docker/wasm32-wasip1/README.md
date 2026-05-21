# Build wasm32-wasip1 pltxt2htm-cmd

## Release mode
```sh
docker build -f cmd/docker/wasm32-wasip1/Dockerfile --build-arg mode=release -t wasm32-wasip1-pltxt2htm-cmd-release .
docker run --rm wasm32-wasip1-pltxt2htm-cmd-release cat /pltxt2htm/cmd/wasm32-wasip1-pltxt2htm-cmd-release.tar.xz > wasm32-wasip1-pltxt2htm-cmd-release.tar.xz
```

Remove docker image
```sh
docker rmi wasm32-wasip1-pltxt2htm-cmd-release
```
