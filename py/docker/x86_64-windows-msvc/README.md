# Build x86_64-windows-msvc pltxt2htm-py

## Debug mode

```sh
docker build -f py/docker/x86_64-windows-msvc/Dockerfile --build-arg PYTHON_VERSION=3.12.10 --build-arg mode=debug -t x86_64-windows-msvc-pltxt2htm-py-debug .
docker run --rm x86_64-windows-msvc-pltxt2htm-py-debug cat /pltxt2htm/py/x86_64-windows-msvc-pltxt2htm-py-cpython3.12.10-debug.zip > x86_64-windows-msvc-pltxt2htm-py-cpython3.12.10-debug.zip
```

## Release mode

```sh
docker build -f py/docker/x86_64-windows-msvc/Dockerfile --build-arg PYTHON_VERSION=3.12.10 --build-arg mode=release -t x86_64-windows-msvc-pltxt2htm-py-release .
docker run --rm x86_64-windows-msvc-pltxt2htm-py-release cat /pltxt2htm/py/x86_64-windows-msvc-pltxt2htm-py-cpython3.12.10-release.zip > x86_64-windows-msvc-pltxt2htm-py-cpython3.12.10-release.zip
```

## Clean up

```sh
docker rmi x86_64-windows-msvc-pltxt2htm-py-debug
docker rmi x86_64-windows-msvc-pltxt2htm-py-release
```
