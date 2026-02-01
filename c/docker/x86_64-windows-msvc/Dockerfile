FROM ubuntu:26.04

ARG mode=debug

RUN apt-get update && apt-get install -y --no-install-recommends ca-certificates wget zip xz-utils xmake llvm clang lld python3

WORKDIR /sysroots
RUN wget https://github.com/trcrsired/windows-msvc-sysroot/releases/download/2026-01-16/windows-msvc-sysroot.tar.xz \
  && tar xJf windows-msvc-sysroot.tar.xz \
  && rm windows-msvc-sysroot.tar.xz

WORKDIR /pltxt2htm
COPY ../../../ .
ENV XMAKE_ROOT=y

WORKDIR /pltxt2htm/c
RUN python3 dist.py --mode=${mode} --toolchain=clang --target=x86_64-windows-msvc \
    --cxxflags='-I/sysroots/windows-msvc-sysroot/include/c++/v1 -I/sysroots/windows-msvc-sysroot/include/' \
    --shflags='-L/sysroots/windows-msvc-sysroot/lib/x86_64-unknown-windows-msvc/'
RUN zip -r x86_64-windows-msvc-pltxt2htm-c-${mode}.zip x86_64-windows-msvc-pltxt2htm-c-${mode}/
