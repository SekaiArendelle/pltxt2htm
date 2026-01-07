FROM ubuntu:26.04 AS builder

RUN apt-get update && apt-get install -y --no-install-recommends \
    xmake xz-utils \
    gcc-mingw-w64 g++-mingw-w64 \
    mingw-w64-tools && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /pltxt2htm
COPY ../../../ /pltxt2htm/

ENV XMAKE_ROOT=y

WORKDIR /pltxt2htm/cmd
ARG mode=debug
RUN xmake config --toolchain=x86_64-w64-mingw32-gcc --arch=x64 --plat=mingw -m ${mode}
RUN xmake build -v
RUN xmake install -o x86_64-w64-mingw32-pltxt2htm-cmd-${mode}
RUN tar cJf x86_64-w64-mingw32-pltxt2htm-cmd-${mode}.tar.xz x86_64-w64-mingw32-pltxt2htm-cmd-${mode}/

FROM ubuntu:26.04

WORKDIR /pltxt2htm
COPY --from=builder /pltxt2htm /pltxt2htm
