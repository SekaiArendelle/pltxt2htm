# Use `pltxt2htm` in command line

## Usage:
```sh
pltxt2htm --target fixedadv_html --host localhost < $your_input_file
```
pltxt2htm will print output html in stdout

```sh
pltxt2htm --target fixedadv_html --host localhost -o $your_output_file < $your_input_file
```
pltxt2htm will write output html to $your_output_file

## Build

### x86_64-windows-gnu (aka. x86_64-w64-mingw32)

See [Docker build](docker/x86_64-w64-mingw32/README.md).

### x86_64-windows-msvc

See [Docker build](docker/x86_64-windows-msvc/README.md).

### aarch64-windows-msvc

See [Docker build](docker/aarch64-windows-msvc/README.md).

### i686-windows-msvc

See [Docker build](docker/i686-windows-msvc/README.md).

### x86_64-linux-musl

See [Docker build](docker/x86_64-linux-musl/README.md).

### i586-linux-musl

See [Docker build](docker/i586-linux-musl/README.md).

### aarch64-linux-musl

See [Docker build](docker/aarch64-linux-musl/README.md).

### aarch64-apple-darwin24

See [Docker build](docker/aarch64-apple-darwin24/README.md).

### loongarch64-linux-musl

See [Docker build](docker/loongarch64-linux-musl/README.md).

### wasm32-wasip1

See [Docker build](docker/wasm32-wasip1/README.md).
