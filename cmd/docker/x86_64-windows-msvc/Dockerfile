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

RUN apt-get update && apt-get install -y --no-install-recommends --fix-missing \
    ca-certificates curl git xz-utils zip python3 \
    cmake make llvm clang lld

RUN set -eux; \
    XWIN_VERSION=0.10.0; \
    curl -fsSL "https://github.com/Jake-Shadle/xwin/releases/download/${XWIN_VERSION}/xwin-${XWIN_VERSION}-x86_64-unknown-linux-musl.tar.gz" \
    | tar xz -C /usr/local/bin --strip-components=1 "xwin-${XWIN_VERSION}-x86_64-unknown-linux-musl/xwin"

RUN xwin --accept-license --sdk-version 10.0.26100 splat --output /opt/xwin

WORKDIR /pltxt2htm
# Copy the entire project to preserve git tracking history.
# A partial copy would distort pltxt2htm's git-tracked modification detection.
COPY ../../../ .

WORKDIR /pltxt2htm/cmd
ARG mode=debug

RUN CFLAGS="--target=x86_64-windows-msvc -fms-runtime-lib=static -isystem/opt/xwin/crt/include -isystem/opt/xwin/sdk/include/ucrt -isystem/opt/xwin/sdk/include/um -isystem/opt/xwin/sdk/include/shared -D_CRT_SECURE_NO_WARNINGS" && \
    CXXFLAGS="${CFLAGS}" && \
    LDFLAGS="--target=x86_64-windows-msvc -nostdlib -fuse-ld=lld -L/opt/xwin/crt/lib/x86_64 -L/opt/xwin/sdk/lib/ucrt/x86_64 -L/opt/xwin/sdk/lib/um/x86_64" && \
    cmake -S . -B build \
        -DCMAKE_BUILD_TYPE=${mode} \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DCMAKE_CXX_COMPILER_TARGET=x86_64-windows-msvc \
        -DCMAKE_BUILD_WITH_INSTALL_RPATH=TRUE \
        -DCMAKE_CXX_FLAGS="${CXXFLAGS}" \
        -DCMAKE_EXE_LINKER_FLAGS="${LDFLAGS}"
RUN cmake --build build -v
RUN cmake --install build --prefix x86_64-windows-msvc-pltxt2htm-cmd-${mode}
RUN zip -r x86_64-windows-msvc-pltxt2htm-cmd-${mode}.zip x86_64-windows-msvc-pltxt2htm-cmd-${mode}/
