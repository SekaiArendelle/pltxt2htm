FROM ubuntu:26.04

ENV mode=release

RUN apt-get update && apt-get install -y --no-install-recommends ca-certificates wget unzip xmake xz-utils

WORKDIR /llvm

RUN wget https://dl.google.com/android/repository/android-ndk-r29-linux.zip
RUN unzip -q android-ndk-r29-linux.zip
ENV PATH=/llvm/android-ndk-r29/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH
ENV XMAKE_ROOT=y

WORKDIR /pltxt2htm
COPY ../../../ .

WORKDIR /pltxt2htm/c
RUN xmake config -a arm64 -p android -m ${mode} -k static --toolchain=clang --cxxflags='--target=aarch64-linux-android30'
RUN xmake build -v
RUN xmake install -o aarch64-linux-android30-pltxt2htm-c-${mode}
RUN rm -rf .xmake build
RUN xmake config -a arm64 -p android -m ${mode} -k shared --toolchain=clang --ld=clang --cxxflags='--target=aarch64-linux-android30' --shflags='--target=aarch64-linux-android30 -nostdlib -ldl -lc'
RUN xmake build -v
RUN xmake install -o aarch64-linux-android30-pltxt2htm-c-${mode}
RUN cp -r include aarch64-linux-android30-pltxt2htm-c-${mode}/
RUN tar cJf aarch64-linux-android30-pltxt2htm-c-${mode}.tar.xz aarch64-linux-android30-pltxt2htm-c-${mode}/
