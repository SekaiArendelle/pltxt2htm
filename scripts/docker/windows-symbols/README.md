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

repository to be mounted at `/pltxt2htm`. The generator cross-compiles temporary
objects for its MSVC targets and does not need a Windows SDK or linker.

It compiles the same definitions headers used by the library. The probes only
need `<cstddef>` and `<cstdint>`, which `scripts/probe_headers` supplies without
depending on a host C++ standard library. Those shims intentionally provide only
the types the definitions headers use; extend them if those headers start using
more of the standard library.
