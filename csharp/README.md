# Use `pltxt2htm` in CSharp
From the repository root, Pixi builds the C shared library and the C# project. The run task adds the native library directory to the platform's library search path:

```sh
pixi run build-csharp
pixi run run-csharp
```

## Run example without Pixi

Build the C shared library manually and add its directory to `PATH` (Windows), `LD_LIBRARY_PATH` (Linux), or `DYLD_LIBRARY_PATH` (macOS) before running. Copying the library beside the .NET executable also works:

```sh
dotnet run
```
