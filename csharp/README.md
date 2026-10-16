# Use `pltxt2htm` in CSharp
From the repository root, Pixi builds the C shared library and the C# project. The run task adds the native library directory to the platform's library search path:

```sh
pixi run -e csharp build-csharp
pixi run -e csharp run-csharp
```
