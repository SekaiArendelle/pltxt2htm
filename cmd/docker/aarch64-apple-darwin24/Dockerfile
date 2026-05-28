FROM alpine:3.23 AS builder

ARG MACOS_SDK_URL=https://github.com/joseluisq/macosx-sdks/releases/download/15.2/MacOSX15.2.sdk.tar.xz
ARG mode=release

RUN apk add --no-cache clang git lld wget xz cmake ninja-build

# Download and extract macOS SDK
RUN wget -q "${MACOS_SDK_URL}" -O /tmp/macos-sdk.tar.xz \
    && mkdir -p /sysroots/aarch64-apple-darwin24 \
    && tar xf /tmp/macos-sdk.tar.xz -C /sysroots/aarch64-apple-darwin24 --strip-components=1 \
    && rm /tmp/macos-sdk.tar.xz

ENV SYSROOT=/sysroots/aarch64-apple-darwin24

WORKDIR /pltxt2htm
# Copy the entire project to preserve git tracking history.
# A partial copy would distort pltxt2htm's git-tracked modification detection.
COPY ../../../ .

WORKDIR /pltxt2htm/cmd
RUN cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=${mode} \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_C_FLAGS="--target=aarch64-apple-darwin24 --sysroot=${SYSROOT}" \
    -DCMAKE_CXX_FLAGS="--target=aarch64-apple-darwin24 --sysroot=${SYSROOT}" \
    -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld --target=aarch64-apple-darwin24 --sysroot=${SYSROOT}"
RUN cmake --build build -v
RUN cmake --install build --prefix aarch64-apple-darwin24-pltxt2htm-cmd-${mode}
RUN tar cJf aarch64-apple-darwin24-pltxt2htm-cmd-${mode}.tar.xz aarch64-apple-darwin24-pltxt2htm-cmd-${mode}/

FROM alpine:3.23
WORKDIR /pltxt2htm/cmd
COPY --from=builder /pltxt2htm/cmd/aarch64-apple-darwin24-pltxt2htm-cmd-*.tar.xz .
