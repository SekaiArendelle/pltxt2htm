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

ENV mode=release

RUN apt-get update && apt-get install -y --no-install-recommends ca-certificates wget unzip cmake ninja-build xz-utils

WORKDIR /llvm

# Temporarily use r30 beta 2 because r29's clang-r563880c crashes while parsing
# valid C++23 explicit-object member calls in container::Expected. The
# clang-r574158b toolchain has been verified with both shared and static builds.
RUN wget https://dl.google.com/android/repository/android-ndk-r30-beta2-linux.zip \
    && unzip -q android-ndk-r30-beta2-linux.zip \
    && rm android-ndk-r30-beta2-linux.zip
ENV PATH=/llvm/android-ndk-r30-beta2/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH

WORKDIR /pltxt2htm
COPY ../../../include /pltxt2htm/include/
COPY ../../../cmake /pltxt2htm/cmake/
COPY ../../../c /pltxt2htm/c/

WORKDIR /pltxt2htm/c
RUN cmake -S . -B build-shared -G Ninja \
    -DCMAKE_BUILD_TYPE=${mode} \
    -DBUILD_SHARED_LIBS=ON \
    -DCMAKE_TOOLCHAIN_FILE=/llvm/android-ndk-r30-beta2/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=armeabi-v7a \
    -DANDROID_PLATFORM=android-30
RUN cmake --build build-shared -v
RUN cmake --install build-shared --prefix armv7-linux-androideabi30-pltxt2htm-c-${mode}

RUN cmake -S . -B build-static -G Ninja \
    -DCMAKE_BUILD_TYPE=${mode} \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_TOOLCHAIN_FILE=/llvm/android-ndk-r30-beta2/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=armeabi-v7a \
    -DANDROID_PLATFORM=android-30
RUN cmake --build build-static -v
RUN cmake --install build-static --prefix armv7-linux-androideabi30-pltxt2htm-c-${mode}
RUN cp -r include armv7-linux-androideabi30-pltxt2htm-c-${mode}/
RUN tar cJf armv7-linux-androideabi30-pltxt2htm-c-${mode}.tar.xz armv7-linux-androideabi30-pltxt2htm-c-${mode}/
