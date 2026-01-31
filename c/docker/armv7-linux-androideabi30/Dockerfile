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
RUN xmake config -p android -m ${mode} -k static --toolchain=clang --cxxflags='--target=armv7-linux-androideabi30'
RUN xmake build -v
RUN xmake install -o armv7-linux-androideabi30-pltxt2htm-c-${mode}
RUN rm -rf .xmake build
RUN xmake config -p android -m ${mode} -k shared --toolchain=clang --linker=clang --cxxflags='--target=armv7-linux-androideabi30'
RUN xmake build -v
RUN xmake install -o armv7-linux-androideabi30-pltxt2htm-c-${mode}
RUN tar cJf armv7-linux-androideabi30-pltxt2htm-c-${mode}.tar.xz armv7-linux-androideabi30-pltxt2htm-c-${mode}/

CMD [ "sleep", "infinity" ]
