# For js extension (wasm)

Build pltxt2htm to wasm that people can use it in javascript, requires `emcc`.

## build
```sh
cd js
xmake config -p wasm -m [debug|release]
xmake build
```
