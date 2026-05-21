# Build aarch64-apple-darwin24 pltxt2htm-cmd

## Prerequisites

A macOS SDK tarball (e.g. `MacOSX15.2.sdk.tar.xz`) is required. A default download URL
pointing to [macosx-sdks](https://github.com/joseluisq/macosx-sdks) is set in the
`MACOS_SDK_URL` build arg. Override it if you need a different SDK version or mirror:

```sh
--build-arg MACOS_SDK_URL=<url-to-macos-sdk>
```

## Debug mode
```sh
docker build -f cmd/docker/aarch64-apple-darwin24/Dockerfile --build-arg mode=debug -t aarch64-apple-darwin24-pltxt2htm-cmd-debug .
docker run --rm aarch64-apple-darwin24-pltxt2htm-cmd-debug cat /pltxt2htm/cmd/aarch64-apple-darwin24-pltxt2htm-cmd-debug.tar.xz > aarch64-apple-darwin24-pltxt2htm-cmd-debug.tar.xz
```

Clean up docker image
```sh
docker rmi aarch64-apple-darwin24-pltxt2htm-cmd-debug
```

## Release mode
```sh
docker build -f cmd/docker/aarch64-apple-darwin24/Dockerfile --build-arg mode=release -t aarch64-apple-darwin24-pltxt2htm-cmd-release .
docker run --rm aarch64-apple-darwin24-pltxt2htm-cmd-release cat /pltxt2htm/cmd/aarch64-apple-darwin24-pltxt2htm-cmd-release.tar.xz > aarch64-apple-darwin24-pltxt2htm-cmd-release.tar.xz
```

Clean up docker image
```sh
docker rmi aarch64-apple-darwin24-pltxt2htm-cmd-release
```
