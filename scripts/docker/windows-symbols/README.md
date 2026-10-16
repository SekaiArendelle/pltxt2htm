# Windows symbol generator

Build the dedicated Clang and LLVM image from the repository root:

```sh
docker build -t pltxt2htm-windows-symbols scripts/docker/windows-symbols
```

Verify the checked-in declarations and linker aliases without modifying the repository:

```sh
docker run --rm -v "${PWD}:/pltxt2htm:ro" pltxt2htm-windows-symbols
```

After editing the symbol manifest, regenerate the files with a writable mount:

```sh
docker run --rm -v "${PWD}:/pltxt2htm" pltxt2htm-windows-symbols python3 scripts/gen_windows_symbols.py --cxx-include-dir /usr/include/c++/v1
docker run --rm -v "${PWD}:/pltxt2htm:ro" pltxt2htm-windows-symbols
```

For a quick check of one DLL during development, pass `--library kernel32` (or
`ntdll` / `dbghelp`) to `gen_windows_symbols.py` with either command above.
The default still checks every library.

The generator cross-compiles temporary objects for its MSVC targets and does
not need a Windows SDK or linker in the container.
It compiles the same definitions headers used by the library. Outside the image,
pass `--cxx-include-dir` when the target cannot find compatible C++ headers.
