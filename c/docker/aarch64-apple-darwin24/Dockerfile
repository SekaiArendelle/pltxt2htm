FROM alpine:3.23

RUN apk add --no-cache ca-certificates wget xz cmake ninja llvm clang lld python3

ARG MACOS_SDK_URL=https://github.com/joseluisq/macosx-sdks/releases/download/15.2/MacOSX15.2.sdk.tar.xz
RUN wget -q "${MACOS_SDK_URL}" -O /tmp/macos-sdk.tar.xz \
    && mkdir -p /sysroots/aarch64-apple-darwin24 \
    && tar xf /tmp/macos-sdk.tar.xz -C /sysroots/aarch64-apple-darwin24 --strip-components=1 \
    && rm /tmp/macos-sdk.tar.xz

WORKDIR /pltxt2htm
COPY ../../../include /pltxt2htm/include/
COPY ../../../c /pltxt2htm/c/

WORKDIR /pltxt2htm/c
ARG mode=debug
RUN python3 dist.py --target=aarch64-apple-darwin24 --toolchain=clang --mode=${mode} \
    --sysroot=/sysroots/aarch64-apple-darwin24 \
    --cxxflags="-stdlib=libc++ -fuse-ld=lld" \
    --shflags="-stdlib=libc++ -fuse-ld=lld"
RUN tar -cJvf aarch64-apple-darwin24-pltxt2htm-c-${mode}.tar.xz aarch64-apple-darwin24-pltxt2htm-c-${mode}/
