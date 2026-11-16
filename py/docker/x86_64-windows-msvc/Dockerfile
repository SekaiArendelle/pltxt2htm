FROM ubuntu:26.04 AS builder

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

ARG PYTHON_VERSION

RUN apt-get update && apt-get install -y --no-install-recommends --fix-missing \
    ca-certificates curl xz-utils zip wget unzip cabextract msitools p7zip-full \
    llvm clang lld python3 cmake ninja-build

# Download and install xwin (Windows CRT and SDK)
RUN set -eux; \
    XWIN_VERSION=0.10.0; \
    curl -fsSL "https://github.com/Jake-Shadle/xwin/releases/download/${XWIN_VERSION}/xwin-${XWIN_VERSION}-x86_64-unknown-linux-musl.tar.gz" \
    | tar xz -C /usr/local/bin --strip-components=1 "xwin-${XWIN_VERSION}-x86_64-unknown-linux-musl/xwin"

RUN xwin --accept-license --sdk-version 10.0.26100 splat --output /opt/xwin

# Download and extract MSVC tools (headers and import libs for MS STL)
# vsdownload.py downloads the Visual Studio Build Tools from Microsoft and extracts
# the MSVC compiler, headers, and libraries — only the files we need for MS STL.
# Pin the toolset: the latest MSVC STL (14.52) requires Clang 22, while Ubuntu 26.04
# ships Clang 21. 14.51 still accepts Clang 20 and later, and maps to SDK 10.0.26100.
RUN wget -q "https://raw.githubusercontent.com/mstorsjo/msvc-wine/master/vsdownload.py" -O /tmp/vsdownload.py \
    && python3 /tmp/vsdownload.py --accept-license --major 18 --msvc-version 14.51 --dest /opt/msvc \
    && rm /tmp/vsdownload.py

# Download Python for Windows (NuGet package provides headers and import libs)
RUN wget -q "https://www.nuget.org/api/v2/package/python/${PYTHON_VERSION}" -O /tmp/python.nupkg \
    && mkdir -p /opt/python-win \
    && unzip -q /tmp/python.nupkg -d /opt/python-win \
    && rm /tmp/python.nupkg

# Discover MSVC version directory and compute Python version variables
RUN MSVC_DIR=$(ls -d /opt/msvc/VC/Tools/MSVC/*/ 2>/dev/null | head -1) \
    && PY_MINOR=$(echo "${PYTHON_VERSION}" | cut -d. -f1,2) \
    && PY_VER_SHORT=$(echo "${PY_MINOR}" | tr -d '.') \
    && echo "MSVC_INCLUDE=${MSVC_DIR}include" >> /tmp/build-vars \
    && echo "MSVC_LIB=${MSVC_DIR}lib/x64" >> /tmp/build-vars \
    && echo "PYTHON_INCLUDE_DIR=/opt/python-win/tools/include" >> /tmp/build-vars \
    && echo "PYTHON_LIB=python${PY_VER_SHORT}" >> /tmp/build-vars \
    && echo "CPYTHON_ABIFLAGS=cpython-${PY_VER_SHORT}" >> /tmp/build-vars \
    && echo "CPYTHON_ABIFLAGS_SHORT=cp${PY_VER_SHORT}" >> /tmp/build-vars

ENV TARGET=x86_64-windows-msvc

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY cmake/ /pltxt2htm/cmake/
COPY py/ /pltxt2htm/py/

# Configure and build the Python extension
RUN . /tmp/build-vars \
    && cmake -B /pltxt2htm/py/build \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DCMAKE_CXX_COMPILER_TARGET=${TARGET} \
        -DCMAKE_SYSTEM_NAME=Windows \
        -DCMAKE_SYSTEM_PROCESSOR=AMD64 \
        -DCMAKE_CXX_FLAGS="-isystem ${MSVC_INCLUDE} -isystem /opt/xwin/crt/include -isystem /opt/xwin/sdk/include/ucrt -isystem /opt/xwin/sdk/include/um -isystem /opt/xwin/sdk/include/shared -L${MSVC_LIB} -L/opt/xwin/crt/lib/x86_64 -L/opt/xwin/sdk/lib/ucrt/x86_64 -L/opt/xwin/sdk/lib/um/x86_64 -L/opt/python-win/tools/libs -fuse-ld=lld -fms-runtime-lib=dll -D_CRT_SECURE_NO_WARNINGS" \
        -DCMAKE_EXE_LINKER_FLAGS="-L${MSVC_LIB} -L/opt/xwin/crt/lib/x86_64 -L/opt/xwin/sdk/lib/ucrt/x86_64 -L/opt/xwin/sdk/lib/um/x86_64 -L/opt/python-win/tools/libs" \
        -DCMAKE_SHARED_LINKER_FLAGS="-L${MSVC_LIB} -L/opt/xwin/crt/lib/x86_64 -L/opt/xwin/sdk/lib/ucrt/x86_64 -L/opt/xwin/sdk/lib/um/x86_64 -L/opt/python-win/tools/libs" \
        -GNinja \
        -DPYTHON_INCLUDE_DIR="${PYTHON_INCLUDE_DIR}" \
        -DPYTHON_EXT_SUFFIX=".${CPYTHON_ABIFLAGS}-win_amd64.pyd" \
        -DPYTHON_LIB="${PYTHON_LIB}" \
        -S /pltxt2htm/py \
    && cmake --build /pltxt2htm/py/build -j$(nproc)

# Package the extension into a tarball
RUN . /tmp/build-vars \
    && cd /pltxt2htm/py \
    && mkdir -p "x86_64-windows-msvc-pltxt2htm-py-cpython${PYTHON_VERSION}-release" \
    && cp "/pltxt2htm/py/build/pltxt2htm.${CPYTHON_ABIFLAGS}-win_amd64.pyd" \
        "x86_64-windows-msvc-pltxt2htm-py-cpython${PYTHON_VERSION}-release/pltxt2htm.${CPYTHON_ABIFLAGS_SHORT}-win_amd64.pyd" \
    && zip -r "x86_64-windows-msvc-pltxt2htm-py-cpython${PYTHON_VERSION}-release.zip" \
        "x86_64-windows-msvc-pltxt2htm-py-cpython${PYTHON_VERSION}-release"

FROM ubuntu:26.04
WORKDIR /pltxt2htm/py
COPY --from=builder /pltxt2htm/py/x86_64-windows-msvc-pltxt2htm-py-*.zip .
