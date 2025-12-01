# TODO: Update to Ubuntu 26.04 LTS
FROM ubuntu:25.04 AS toolchain

RUN apt update && apt install -y --no-install-recommends wget ca-certificates xz-utils

WORKDIR /llvm
RUN wget https://github.com/trcrsired/llvm-releases/releases/download/llvm22-20251125/x86_64-linux-gnu.tar.xz \
    && tar xJf x86_64-linux-gnu.tar.xz \
    && wget https://github.com/trcrsired/llvm-releases/releases/download/llvm22-20251125/loongarch64-linux-musl.tar.xz \
    && tar xJf loongarch64-linux-musl.tar.xz \
    && cp -r loongarch64-linux-musl/builtins/lib /llvm/x86_64-linux-gnu/llvm/lib/clang/22/ \
    && mkdir /sysroots \
    && cp -r loongarch64-linux-musl/loongarch64-linux-musl /sysroots/ \
    && rm -rf /llvm/loongarch64-linux-musl \
    && rm -rf /llvm/x86_64-linux-gnu/x86_64-linux-gnu \
    && rm /llvm/x86_64-linux-gnu.tar.xz loongarch64-linux-musl.tar.xz

FROM ubuntu:25.04 AS builder

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
RUN xmake config --toolchain=clang --arch=loong64 --plat=linux -m ${mode} --cxxflags='--sysroot=/sysroots/loongarch64-linux-musl -stdlib=libc++ -rtlib=compiler-rt -unwindlib=libunwind' --ldflags='-static --sysroot=/sysroots/loongarch64-linux-musl/ -stdlib=libc++ -rtlib=compiler-rt -unwindlib=libunwind -lunwind -lc++abi'
RUN xmake build -v
RUN xmake install -o loongarch64-linux-musl-pltxt2htm-cmd-${mode}
RUN tar cJf loongarch64-linux-musl-pltxt2htm-cmd-${mode}.tar.xz loongarch64-linux-musl-pltxt2htm-cmd-${mode}/

FROM ubuntu:25.04

WORKDIR /pltxt2htm
COPY --from=builder /pltxt2htm /pltxt2htm

CMD ["sleep", "infinity"]
