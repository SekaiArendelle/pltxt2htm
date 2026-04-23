# C# Translator Build Guide

This directory contains the clang/LLVM-based translator that converts `include/pltxt2htm/pltxt2htm.hh` APIs into C# code.

## Prerequisite

Install `pixi` first.

## Build

From `translang/csharp`:

```powershell
pixi run install
```

(`pixi run build` is still available if you only want to run the build step.)

## Generate translated C# output

`--output-dir` is required.

```powershell
.\dist\pltxt2htm_csharp_translator.exe --output-dir .\generated
```

Generated file:

`generated/Pltxt2htm.Generated.cs`
