# Build aarch64-apple-darwin24 pltxt2htm-py

## Prerequisites

A macOS SDK tarball (e.g. `MacOSX15.2.sdk.tar.xz`) is required. A default download URL
pointing to [macosx-sdks](https://github.com/joseluisq/macosx-sdks) is set in the
`MACOS_SDK_URL` build arg. Override it if you need a different SDK version or mirror:

```sh
--build-arg MACOS_SDK_URL=<url-to-macos-sdk>
```

## Debug mode

```sh
docker build -f py/docker/aarch64-apple-darwin24/Dockerfile \
    --build-arg mode=debug \
    -t aarch64-apple-darwin24-pltxt2htm-py-debug .
docker run --rm aarch64-apple-darwin24-pltxt2htm-py-debug \
    cat /pltxt2htm/py/aarch64-apple-darwin24-pltxt2htm-py-debug.tar.xz \
    > aarch64-apple-darwin24-pltxt2htm-py-debug.tar.xz
```

## Release mode

```sh
docker build -f py/docker/aarch64-apple-darwin24/Dockerfile \
    --build-arg mode=release \
    -t aarch64-apple-darwin24-pltxt2htm-py-release .
docker run --rm aarch64-apple-darwin24-pltxt2htm-py-release \
    cat /pltxt2htm/py/aarch64-apple-darwin24-pltxt2htm-py-release.tar.xz \
    > aarch64-apple-darwin24-pltxt2htm-py-release.tar.xz
```

## Custom Python version

```sh
docker build -f py/docker/aarch64-apple-darwin24/Dockerfile \
    --build-arg PYTHON_VERSION=3.13 \
    --build-arg mode=release \
    -t aarch64-apple-darwin24-pltxt2htm-py-313 .
```

## Clean up

```sh
docker rmi aarch64-apple-darwin24-pltxt2htm-py-debug
docker rmi aarch64-apple-darwin24-pltxt2htm-py-release
```
