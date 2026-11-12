FROM ubuntu:26.04

# Route apt through authoritative Ubuntu archive mirrors from several countries
# (US origin first, then CN / JP / DE / UK); apt falls back to the next mirror
# automatically when one is unavailable.
RUN set -eux; \
    printf '%b\n' \
        'http://archive.ubuntu.com/ubuntu/\tpriority:1' \
        'http://mirrors.tuna.tsinghua.edu.cn/ubuntu/\tpriority:2' \
        'http://ftp.jaist.ac.jp/pub/Linux/ubuntu/\tpriority:2' \
        'http://de.archive.ubuntu.com/ubuntu/\tpriority:2' \
        'http://uk.archive.ubuntu.com/ubuntu/\tpriority:2' \
        > /etc/apt/pltxt2htm-ubuntu-mirrors.lst; \
    sed -i 's|^URIs: .*|URIs: mirror+file:/etc/apt/pltxt2htm-ubuntu-mirrors.lst|' \
        /etc/apt/sources.list.d/ubuntu.sources; \
    grep -q '^URIs: mirror+file:' /etc/apt/sources.list.d/ubuntu.sources

RUN apt-get update && apt-get install -y --no-install-recommends --fix-missing \
    ca-certificates curl git xz-utils cmake ninja-build

RUN set -eux; \
    WASI_SDK_VERSION=27.0; \
    curl -fsSL --retry 5 --retry-delay 10 "https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-${WASI_SDK_VERSION%.*}/wasi-sdk-${WASI_SDK_VERSION}-x86_64-linux.tar.gz" \
    | tar xz -C /opt

ENV PATH=/opt/wasi-sdk-27.0-x86_64-linux/bin:$PATH

WORKDIR /pltxt2htm
# Copy the entire project to preserve git tracking history.
# A partial copy would distort pltxt2htm's git-tracked modification detection.
COPY ../../../ .

WORKDIR /pltxt2htm/cmd

RUN cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_COMPILER=/opt/wasi-sdk-27.0-x86_64-linux/bin/clang++ \
    -DCMAKE_C_COMPILER=/opt/wasi-sdk-27.0-x86_64-linux/bin/clang \
    -DCMAKE_SYSTEM_NAME=WASI
RUN cmake --build build -v
RUN cmake --install build --prefix wasm32-wasip1-pltxt2htm-cmd-release
RUN tar cJvf wasm32-wasip1-pltxt2htm-cmd-release.tar.xz wasm32-wasip1-pltxt2htm-cmd-release
