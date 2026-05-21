FROM ubuntu:26.04

RUN apt-get update && apt-get install -y --no-install-recommends --fix-missing \
    ca-certificates curl git xz-utils xmake

RUN set -eux; \
    WASI_SDK_VERSION=27.0; \
    curl -fsSL --retry 5 --retry-delay 10 "https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-${WASI_SDK_VERSION%.*}/wasi-sdk-${WASI_SDK_VERSION}-x86_64-linux.tar.gz" \
    | tar xz -C /opt

ENV PATH=/opt/wasi-sdk-27.0-x86_64-linux/bin:$PATH

WORKDIR /pltxt2htm
# Copy the entire project to preserve git tracking history.
# A partial copy would distort pltxt2htm's git-tracked modification detection.
COPY ../../../ .

ENV XMAKE_ROOT=y

WORKDIR /pltxt2htm/cmd
ARG mode=debug

RUN xmake config --plat=wasm --arch=wasm32 --toolchain=wasm32-wasip1-clang -m ${mode}
RUN xmake build -v
RUN xmake install -o wasm32-wasip1-pltxt2htm-cmd-${mode}
RUN tar cJvf wasm32-wasip1-pltxt2htm-cmd-${mode}.tar.xz wasm32-wasip1-pltxt2htm-cmd-${mode}
