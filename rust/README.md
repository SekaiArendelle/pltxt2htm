# Use `pltxt2htm` in Rust
Note that this is experimental.

`libpltxt2htm-sys` now compiles the C bridge automatically at build time, so no manual static-library build/copy step is needed.

The C bridge (`c/src/pltxt2htm.cc`) is compiled during Rust builds. Use the
Pixi Rust environment from the repository root:

```sh
pixi run -e rust build-rust
pixi run -e rust test-rust
```

## examples

example for libpltxt2htm-sys, which is not recommended:
```sh
pixi run -e rust run-rust-example libpltxt2htm-sys bare
```

example for pltxt2htm:
```sh
pixi run -e rust run-rust-example pltxt2htm recommend
```
