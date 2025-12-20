# Build wasm32-unknown-emscripten pltxt2htm-wasm

## Debug mode
```sh
docker build -f wasm/docker/wasm32-unknown-emscripten/Dockerfile --build-arg mode=debug -t wasm32-unknown-emscripten-pltxt2htm-wasm-debug .
docker run -d --name wasm32-unknown-emscripten-pltxt2htm-wasm-debug wasm32-unknown-emscripten-pltxt2htm-wasm-debug
docker cp wasm32-unknown-emscripten-pltxt2htm-wasm-debug:/pltxt2htm/wasm/wasm32-unknown-emscripten-pltxt2htm-wasm-debug.zip .
```

### Remove docker container and image
```sh
docker rm wasm32-unknown-emscripten-pltxt2htm-wasm-debug
docker rmi wasm32-unknown-emscripten-pltxt2htm-wasm-debug
```

## Release mode
```sh
docker build -f wasm/docker/wasm32-unknown-emscripten/Dockerfile --build-arg mode=release -t wasm32-unknown-emscripten-pltxt2htm-wasm-release .
docker run -d --name wasm32-unknown-emscripten-pltxt2htm-wasm-release wasm32-unknown-emscripten-pltxt2htm-wasm-release
docker cp wasm32-unknown-emscripten-pltxt2htm-wasm-release:/pltxt2htm/wasm/wasm32-unknown-emscripten-pltxt2htm-wasm-release.zip .
```

### Remove docker container and image
```sh
docker rm wasm32-unknown-emscripten-pltxt2htm-wasm-release
docker rmi wasm32-unknown-emscripten-pltxt2htm-wasm-release
```
