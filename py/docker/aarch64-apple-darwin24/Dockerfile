FROM alpine:3.23 AS builder

ARG MACOS_SDK_URL=https://github.com/joseluisq/macosx-sdks/releases/download/15.2/MacOSX15.2.sdk.tar.xz
ARG PYTHON_VERSION=3.12.10
ARG mode=release

RUN apk add --no-cache clang cmake git lld python3 wget xz xar cpio

# Download and extract macOS SDK
RUN wget -q "${MACOS_SDK_URL}" -O /tmp/macos-sdk.tar.xz \
    && mkdir -p /sysroots/aarch64-apple-darwin24 \
    && tar xf /tmp/macos-sdk.tar.xz -C /sysroots/aarch64-apple-darwin24 --strip-components=1 \
    && rm /tmp/macos-sdk.tar.xz

# Download Python macOS installer and extract pyconfig.h
RUN wget -q "https://www.python.org/ftp/python/${PYTHON_VERSION}/python-${PYTHON_VERSION}-macos11.pkg" -O /tmp/py.pkg \
    && cd /tmp && xar -xf /tmp/py.pkg \
    && mkdir -p /tmp/framework \
    && cd /tmp/framework \
    && cat /tmp/Python_Framework.pkg/Payload | gzip -d | cpio -idm 2>/dev/null \
    && rm -rf /tmp/py.pkg /tmp/Python_*.pkg /tmp/Resources /tmp/Distribution

# Download Python source tarball and extract Include/ headers
RUN wget -q "https://www.python.org/ftp/python/${PYTHON_VERSION}/Python-${PYTHON_VERSION}.tar.xz" -O /tmp/python.tar.xz \
    && mkdir -p /tmp/python-include \
    && tar xf /tmp/python.tar.xz -C /tmp \
    && cp -r "/tmp/Python-${PYTHON_VERSION}/Include/." /tmp/python-include/ \
    && rm -rf "/tmp/Python-${PYTHON_VERSION}" /tmp/python.tar.xz

# Copy pyconfig.h from extracted macOS framework
RUN cp "/tmp/framework/Versions/${PYTHON_VERSION%.*}/include/python${PYTHON_VERSION%.*}/pyconfig.h" /tmp/python-include/ \
    && rm -rf /tmp/framework

RUN PY_VER_SHORT=$(echo "${PYTHON_VERSION}" | cut -d. -f1,2 | tr -d '.') \
    && echo "CPYTHON_ABIFLAGS=cpython-${PY_VER_SHORT}" >> /tmp/build-vars

ENV SYSROOT=/sysroots/aarch64-apple-darwin24
ENV TARGET=aarch64-apple-darwin24

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY py/ /pltxt2htm/py/

# Configure and build with CMake
RUN . /tmp/build-vars \
    && cmake -B /pltxt2htm/py/build \
        -DCMAKE_BUILD_TYPE=$(echo "${mode}" | sed 's/^release$/Release/; s/^debug$/Debug/') \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DCMAKE_CXX_COMPILER_TARGET=${TARGET} \
        -DCMAKE_CXX_FLAGS="--sysroot=${SYSROOT} -stdlib=libc++" \
        -DPYTHON_INCLUDE_DIR=/tmp/python-include \
        -DPYTHON_EXT_SUFFIX=".${CPYTHON_ABIFLAGS}-darwin.so" \
        -S /pltxt2htm/py \
    && cmake --build /pltxt2htm/py/build -j$(nproc)

# Package
RUN . /tmp/build-vars \
    && cd /pltxt2htm/py \
    && mkdir -p "aarch64-apple-darwin24-pltxt2htm-py-${mode}" \
    && cp "/pltxt2htm/py/build/pltxt2htm.${CPYTHON_ABIFLAGS}-darwin.so" \
        "aarch64-apple-darwin24-pltxt2htm-py-${mode}/" \
    && tar cJf "aarch64-apple-darwin24-pltxt2htm-py-${mode}.tar.xz" \
        "aarch64-apple-darwin24-pltxt2htm-py-${mode}"

FROM alpine:3.23
WORKDIR /pltxt2htm/py
COPY --from=builder /pltxt2htm/py/aarch64-apple-darwin24-pltxt2htm-py-*.tar.xz .
