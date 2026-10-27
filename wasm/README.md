# Use `pltxt2htm` in js (By wasm)

Build pltxt2htm to wasm for use in JavaScript.

## Build

From the repository root, Pixi provides Emscripten and the build tools:

```sh
pixi run -e wasm build-wasm
pixi run -e wasm build-wasm release
pixi run -e wasm test-wasm
```

The output is in `wasm/build-pixi-debug/` or `wasm/build-pixi-release/`.
`test-wasm` runs `wasm/examples/basic.mjs`, which the Docker build also uses to check its installed package.
The Pixi environment supports Linux x86-64, macOS x86-64 and ARM64, and Windows x86-64.
The existing Docker build remains available in `wasm/docker/wasm32-unknown-emscripten/`.
