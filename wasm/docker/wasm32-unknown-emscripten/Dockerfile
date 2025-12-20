FROM emscripten/emsdk:4.0.22 AS xmake

WORKDIR /xmake-builder

RUN wget https://github.com/xmake-io/xmake/releases/download/v3.0.5/xmake-v3.0.5.tar.gz
RUN tar xzf xmake-v3.0.5.tar.gz

WORKDIR /xmake-builder/xmake-3.0.5
RUN ./configure --toolchain=gcc --arch=x86_64 --plat=linux --prefix=/xmake
RUN make -j$(nproc)
RUN make install

FROM emscripten/emsdk:4.0.22

ENV PATH=/xmake/bin:$PATH
ENV XMAKE_ROOT=y

COPY --from=xmake /xmake /xmake

WORKDIR /pltxt2htm
COPY . .

COPY --from=xmake /xmake /xmake

WORKDIR /pltxt2htm/wasm

ARG mode=debug

RUN xmake config -p wasm -m ${mode} --toolchain=emcc
RUN xmake build -v
RUN xmake install -o wasm32-unknown-emscripten-pltxt2htm-wasm-${mode}
RUN zip -r wasm32-unknown-emscripten-pltxt2htm-wasm-${mode}.zip wasm32-unknown-emscripten-pltxt2htm-wasm-${mode}/
