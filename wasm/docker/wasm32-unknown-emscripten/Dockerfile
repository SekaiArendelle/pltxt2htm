FROM emscripten/emsdk:4.0.22

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY wasm/ /pltxt2htm/wasm/

WORKDIR /pltxt2htm/wasm

ARG mode=debug

RUN apt-get update && apt-get install -y --no-install-recommends ninja-build && rm -rf /var/lib/apt/lists/*
RUN emcmake cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=${mode}
RUN cmake --build build -v
RUN cmake --install build --prefix wasm32-unknown-emscripten-pltxt2htm-wasm-${mode}
RUN zip -r wasm32-unknown-emscripten-pltxt2htm-wasm-${mode}.zip wasm32-unknown-emscripten-pltxt2htm-wasm-${mode}/
