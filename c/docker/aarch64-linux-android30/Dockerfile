FROM ubuntu:26.04

ENV mode=release

RUN apt-get update && apt-get install -y --no-install-recommends ca-certificates wget unzip cmake ninja-build xz-utils

WORKDIR /llvm

RUN wget https://dl.google.com/android/repository/android-ndk-r29-linux.zip \
    && unzip -q android-ndk-r29-linux.zip \
    && rm android-ndk-r29-linux.zip
ENV PATH=/llvm/android-ndk-r29/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH

WORKDIR /pltxt2htm
COPY ../../../include /pltxt2htm/include/
COPY ../../../c /pltxt2htm/c/

WORKDIR /pltxt2htm/c
RUN cmake -S . -B build-shared -G Ninja \
    -DCMAKE_BUILD_TYPE=${mode^} \
    -DBUILD_SHARED_LIBS=ON \
    -DCMAKE_TOOLCHAIN_FILE=/llvm/android-ndk-r29/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-30
RUN cmake --build build-shared -v
RUN cmake --install build-shared --prefix aarch64-linux-android30-pltxt2htm-c-${mode}

RUN cmake -S . -B build-static -G Ninja \
    -DCMAKE_BUILD_TYPE=${mode^} \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_TOOLCHAIN_FILE=/llvm/android-ndk-r29/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-30
RUN cmake --build build-static -v
RUN cmake --install build-static --prefix aarch64-linux-android30-pltxt2htm-c-${mode}
RUN cp -r include aarch64-linux-android30-pltxt2htm-c-${mode}/
RUN tar cJf aarch64-linux-android30-pltxt2htm-c-${mode}.tar.xz aarch64-linux-android30-pltxt2htm-c-${mode}/
