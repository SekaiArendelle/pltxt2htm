# Use `pltxt2htm` in Rust
Note that this is experimental.

`libpltxt2htm-sys` now compiles the C bridge automatically at build time, so no manual static-library build/copy step is needed.

It requires a C++23 toolchain on your machine because the C bridge (`c/src/pltxt2htm.cc`) is compiled during `cargo build`/`cargo run`.

## examples

example for libpltxt2htm-sys, which is not recommended:
```sh
cargo run --example bare
```

example for pltxt2htm:
```sh
cargo run --example recommend
```
