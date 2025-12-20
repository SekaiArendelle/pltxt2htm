FROM ubuntu:26.04 AS toolchain

RUN apt update && apt install -y --no-install-recommends wget ca-certificates xz-utils

WORKDIR /llvm
RUN wget https://github.com/trcrsired/llvm-releases/releases/download/llvm22-20251125/x86_64-linux-gnu.tar.xz \
    && tar xJf x86_64-linux-gnu.tar.xz \
    && wget https://github.com/trcrsired/llvm-releases/releases/download/llvm22-20251125/x86_64-linux-musl.tar.xz \
    && tar xJf x86_64-linux-musl.tar.xz \
    && cp -r x86_64-linux-musl/builtins/lib /llvm/x86_64-linux-gnu/llvm/lib/clang/22/ \
    && mkdir /sysroots \
    && cp -r x86_64-linux-musl/x86_64-linux-musl /sysroots/ \
    && rm -rf /llvm/x86_64-linux-musl \
    && rm -rf /llvm/x86_64-linux-gnu/x86_64-linux-gnu \
    && rm /llvm/x86_64-linux-gnu.tar.xz x86_64-linux-musl.tar.xz

FROM ubuntu:26.04 AS builder

RUN apt update && apt install -y --no-install-recommends binutils xmake git xz-utils

WORKDIR /pltxt2htm
COPY ../../../ /pltxt2htm/

COPY --from=toolchain /llvm /llvm
COPY --from=toolchain /sysroots /sysroots

ENV PATH=/llvm/x86_64-linux-gnu/llvm/bin:$PATH
ENV LD_LIBRARY_PATH=/llvm/x86_64-linux-gnu/runtimes/lib:/clang/x86_64-linux-gnu/llvm/lib

ENV XMAKE_ROOT=y

WORKDIR /pltxt2htm/cmd
ARG mode=debug
RUN xmake config --toolchain=clang --arch=x64 --plat=linux -m ${mode} --cxxflags='--target=x86_64-linux-musl --sysroot=/sysroots/x86_64-linux-musl -stdlib=libc++ -rtlib=compiler-rt -unwindlib=libunwind' --ldflags='-static --target=x86_64-linux-musl --sysroot=/sysroots/x86_64-linux-musl/ -stdlib=libc++ -rtlib=compiler-rt -unwindlib=libunwind -lunwind -lc++abi'
RUN xmake build -v
RUN xmake install -o x86_64-linux-musl-pltxt2htm-cmd-${mode}
RUN tar cJf x86_64-linux-musl-pltxt2htm-cmd-${mode}.tar.xz x86_64-linux-musl-pltxt2htm-cmd-${mode}/

FROM ubuntu:26.04

WORKDIR /pltxt2htm
COPY --from=builder /pltxt2htm /pltxt2htm
