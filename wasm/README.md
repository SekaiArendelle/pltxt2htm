# Use `pltxt2htm` in js (By wasm)

Build pltxt2htm to wasm that people can use it in javascript, requires `emcc`.

## build
```sh
cd wasm
emcmake cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=[Debug|Release]
cmake --build build
cmake --install build --prefix dist
```
