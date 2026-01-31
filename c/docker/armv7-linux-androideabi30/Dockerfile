FROM ubuntu:26.04

ARG mode=debug

RUN apt-get update && apt-get install -y --no-install-recommends ca-certificates wget unzip xmake xz-utils

WORKDIR /llvm

RUN wget https://dl.google.com/android/repository/android-ndk-r29-linux.zip
RUN unzip android-ndk-r29-linux.zip
ENV PATH=/llvm/android-ndk-r29/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH
ENV XMAKE_ROOT=y

WORKDIR /pltxt2htm
COPY ../../../ .

WORKDIR /pltxt2htm/c
RUN xmake config -m ${mode} -k static --toolchain=clang --cxxflags='--target=armv7-linux-androideabi30'
RUN xmake build -v
RUN xmake install -o armv7-linux-androideabi30-pltxt2htm-c-${mode}
RUN rm -rf .xmake build
RUN xmake config -m ${mode} -k shared --toolchain=clang --cxxflags='--target=armv7-linux-androideabi30'
RUN xmake build -v
RUN xmake install -o armv7-linux-androideabi30-pltxt2htm-c-${mode}
RUN tar cJf armv7-linux-androideabi30-pltxt2htm-c-${mode}.tar.xz armv7-linux-androideabi30-pltxt2htm-c-${mode}/

CMD [ "sleep", "infinity" ]
