# Windows symbol generator

Build the dedicated Clang, LLVM, and Python image from the repository root:

```sh
docker build -t pltxt2htm-windows-symbols scripts/docker/windows-symbols
```

Verify the checked-in declarations and linker aliases without modifying the repository:

```sh
docker run --rm -v "${PWD}:/pltxt2htm:ro" pltxt2htm-windows-symbols
```

After editing the symbol manifest, regenerate the files with a writable mount:

```sh
docker run --rm -v "${PWD}:/pltxt2htm" pltxt2htm-windows-symbols python3 scripts/gen_windows_symbols.py
docker run --rm -v "${PWD}:/pltxt2htm:ro" pltxt2htm-windows-symbols
```

Alternatively, install the `symbols` pixi environment (see the tasks in
`pixi.toml`) and run the generator directly:

```sh
pixi install -e symbols
pixi run -e symbols windows-symbols-check
```

For a quick check of one DLL during development, pass `--library kernel32` (or
`ntdll` / `dbghelp`) to `gen_windows_symbols.py` with either command above.
The default still checks every library.

The generator cross-compiles temporary objects for its MSVC targets and does
not need a Windows SDK or linker in the container.
It compiles the same definitions headers used by the library. The probes only
need `<cstddef>` and `<cstdint>`, which `scripts/probe_headers` supplies without
depending on a host C++ standard library. Those shims intentionally provide only
the types the definitions headers use; extend them if those headers start using
more of the standard library. Pass `--cxx-include-dir` to use a real standard
library header set instead.
