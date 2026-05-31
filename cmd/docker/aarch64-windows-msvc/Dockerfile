FROM ubuntu:26.04

RUN apt-get update && apt-get install -y --no-install-recommends --fix-missing \
    ca-certificates curl git xz-utils zip python3 \
    cmake ninja-build llvm clang lld

RUN set -eux; \
    XWIN_VERSION=0.9.0; \
    curl -fsSL "https://github.com/Jake-Shadle/xwin/releases/download/${XWIN_VERSION}/xwin-${XWIN_VERSION}-x86_64-unknown-linux-musl.tar.gz" \
    | tar xz -C /usr/local/bin --strip-components=1 "xwin-${XWIN_VERSION}-x86_64-unknown-linux-musl/xwin"

RUN xwin --accept-license --arch aarch64 splat --output /opt/xwin

WORKDIR /pltxt2htm
# Copy the entire project to preserve git tracking history.
# A partial copy would distort pltxt2htm's git-tracked modification detection.
COPY ../../../ .

WORKDIR /pltxt2htm/cmd
ARG mode=debug

RUN CFLAGS="--target=aarch64-windows-msvc -fms-runtime-lib=static -isystem/opt/xwin/crt/include -isystem/opt/xwin/sdk/include/ucrt -isystem/opt/xwin/sdk/include/um -isystem/opt/xwin/sdk/include/shared -D_CRT_SECURE_NO_WARNINGS" && \
    CXXFLAGS="${CFLAGS}" && \
    LDFLAGS="--target=aarch64-windows-msvc -nostdlib -fuse-ld=lld -L/opt/xwin/crt/lib/aarch64 -L/opt/xwin/sdk/lib/ucrt/aarch64 -L/opt/xwin/sdk/lib/um/aarch64" && \
    cmake -S . -B build -G Ninja \
        -DCMAKE_BUILD_TYPE=${mode^} \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DCMAKE_CXX_COMPILER_TARGET=aarch64-windows-msvc \
        -DCMAKE_BUILD_WITH_INSTALL_RPATH=TRUE \
        -DCMAKE_CXX_FLAGS="${CXXFLAGS}" \
        -DCMAKE_EXE_LINKER_FLAGS="${LDFLAGS}"
RUN cmake --build build -v
RUN cmake --install build --prefix aarch64-windows-msvc-pltxt2htm-cmd-${mode}
RUN zip -r aarch64-windows-msvc-pltxt2htm-cmd-${mode}.zip aarch64-windows-msvc-pltxt2htm-cmd-${mode}/
