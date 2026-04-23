# C# Translator (Docker Only)

This directory provides a Docker-based workflow to generate translated C# code from `include/pltxt2htm/pltxt2htm.hh`.

```sh
docker build -f ./translang/csharp/Dockerfile -t pltxt2htm-csharp-translator .
docker run --rm pltxt2htm-csharp-translator cat /pltxt2htm-translang-csharp.tar.xz > pltxt2htm-translang-csharp.tar.xz
```
