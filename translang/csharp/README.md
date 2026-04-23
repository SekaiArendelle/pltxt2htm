# C# Translator Build Guide

This directory contains the clang/LLVM-based translator that converts `include/pltxt2htm/pltxt2htm.hh` APIs into C# code.

## Prerequisite

Install `pixi` first.

## Build

From `translang/csharp`:

```powershell
pixi install
pixi run build
```

## Generate translated C# output

```sh
./build/pltxt2htm_csharp_translator
```

Generated file:

`dist/Pltxt2htm.Generated.cs`
