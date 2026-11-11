FROM emscripten/emsdk:4.0.22

# Route apt through authoritative Ubuntu archive mirrors from several countries
# (US origin first, then CN / JP / DE / UK); apt falls back to the next mirror
# automatically when one is unavailable. The emsdk image is ubuntu:22.04-based,
# whose sources are the one-line /etc/apt/sources.list format.
RUN set -eux; \
    printf '%b\n' \
        'http://archive.ubuntu.com/ubuntu/\tpriority:1' \
        'http://mirrors.tuna.tsinghua.edu.cn/ubuntu/\tpriority:2' \
        'http://ftp.jaist.ac.jp/pub/Linux/ubuntu/\tpriority:2' \
        'http://de.archive.ubuntu.com/ubuntu/\tpriority:2' \
        'http://uk.archive.ubuntu.com/ubuntu/\tpriority:2' \
        > /etc/apt/pltxt2htm-ubuntu-mirrors.lst; \
    sed -i -e 's|^deb http://archive.ubuntu.com/ubuntu/ |deb mirror+file:/etc/apt/pltxt2htm-ubuntu-mirrors.lst |' \
           -e 's|^deb http://security.ubuntu.com/ubuntu/ |deb mirror+file:/etc/apt/pltxt2htm-ubuntu-mirrors.lst |' \
        /etc/apt/sources.list; \
    grep -q '^deb mirror+file:' /etc/apt/sources.list

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY cmake/ /pltxt2htm/cmake/
COPY wasm/ /pltxt2htm/wasm/

WORKDIR /pltxt2htm/wasm

ARG mode=debug

RUN apt-get update && apt-get install -y --no-install-recommends ninja-build && rm -rf /var/lib/apt/lists/*
RUN emcmake cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=${mode}
RUN cmake --build build -v
RUN cmake --install build --prefix wasm32-unknown-emscripten-pltxt2htm-wasm-${mode}
RUN node examples/basic.mjs wasm32-unknown-emscripten-pltxt2htm-wasm-${mode}/pltxt2htm.js
RUN zip -r wasm32-unknown-emscripten-pltxt2htm-wasm-${mode}.zip wasm32-unknown-emscripten-pltxt2htm-wasm-${mode}/
