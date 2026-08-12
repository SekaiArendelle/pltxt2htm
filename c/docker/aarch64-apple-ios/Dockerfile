FROM alpine:3.23

RUN apk add --no-cache ca-certificates wget unzip cmake ninja llvm clang lld

ARG IOS_SDK_URL=https://github.com/xybp888/iOS-SDKs/releases/download/iOS26.5-SDKs/iPhoneOS26.5.sdk.zip
RUN wget -q "${IOS_SDK_URL}" -O /tmp/iphoneos-sdk.zip \
    && unzip -q /tmp/iphoneos-sdk.zip -d /sysroots \
    && rm /tmp/iphoneos-sdk.zip

WORKDIR /pltxt2htm
COPY ../../../include /pltxt2htm/include/
COPY ../../../c /pltxt2htm/c/

WORKDIR /pltxt2htm/c
ARG mode=debug
# iOS does not allow dynamic libraries, so only the static library is built.
RUN case "${mode}" in debug) bt=Debug ;; *) bt=Release ;; esac; \
    cmake -S . -B build -G Ninja \
        -DCMAKE_BUILD_TYPE=${bt} \
        -DCMAKE_SYSTEM_NAME=Darwin \
        -DCMAKE_SYSTEM_PROCESSOR=arm64 \
        -DCMAKE_OSX_ARCHITECTURES=arm64 \
        -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
        -DBUILD_SHARED_LIBS=OFF \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DCMAKE_CXX_FLAGS="--target=aarch64-apple-ios --sysroot=/sysroots/iPhoneOS26.5.sdk -stdlib=libc++"
RUN cmake --build build -v
RUN cmake --install build --prefix aarch64-apple-ios-pltxt2htm-c-${mode}
RUN tar -cJvf aarch64-apple-ios-pltxt2htm-c-${mode}.tar.xz aarch64-apple-ios-pltxt2htm-c-${mode}/