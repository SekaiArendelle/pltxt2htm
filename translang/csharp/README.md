# C# Translator

This directory provides workflows to generate translated C# code from `include/pltxt2htm/pltxt2htm.hh`.

## Local (pixi)

```sh
cd translang/csharp
pixi run install
./dist/pltxt2htm_csharp_translator.exe --output-dir generated
```

## Docker

```sh
docker build -f ./translang/csharp/Dockerfile -t pltxt2htm-csharp-translator .
docker run --rm pltxt2htm-csharp-translator cat /pltxt2htm-translang-csharp.tar.xz > pltxt2htm-translang-csharp.tar.xz
```

```sh
docker rmi pltxt2htm-csharp-translator
```
