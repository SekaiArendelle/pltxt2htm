FROM alpine:3.23 AS builder

ARG MACOS_SDK_URL=https://github.com/joseluisq/macosx-sdks/releases/download/15.2/MacOSX15.2.sdk.tar.xz
ARG mode=release

RUN apk add --no-cache clang git lld wget xz xmake

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

ENV XMAKE_ROOT=y

WORKDIR /pltxt2htm/cmd
RUN xmake f -m ${mode} --toolchain=aarch64-apple-darwin24-clang --plat=macosx --arch=arm64 --sysroot=${SYSROOT}
RUN xmake build -v
RUN xmake install -o aarch64-apple-darwin24-pltxt2htm-cmd-${mode}
RUN tar cJf aarch64-apple-darwin24-pltxt2htm-cmd-${mode}.tar.xz aarch64-apple-darwin24-pltxt2htm-cmd-${mode}/

FROM alpine:3.23
WORKDIR /pltxt2htm/cmd
COPY --from=builder /pltxt2htm/cmd/aarch64-apple-darwin24-pltxt2htm-cmd-*.tar.xz .
