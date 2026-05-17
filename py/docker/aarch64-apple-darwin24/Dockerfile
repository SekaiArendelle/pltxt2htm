FROM alpine:3.23 AS builder

ARG MACOS_SDK_URL=https://github.com/joseluisq/macosx-sdks/releases/download/15.2/MacOSX15.2.sdk.tar.xz
ARG PYTHON_VERSION=3.12.10
ARG mode=release

RUN apk add --no-cache clang git lld python3 wget xz

# Download and extract macOS SDK
RUN wget -q "${MACOS_SDK_URL}" -O /tmp/macos-sdk.tar.xz \
    && mkdir -p /sysroots/aarch64-apple-darwin24 \
    && tar xf /tmp/macos-sdk.tar.xz -C /sysroots/aarch64-apple-darwin24 --strip-components=1 \
    && rm /tmp/macos-sdk.tar.xz

# Download Python source and run configure with cross-compilation to generate pyconfig.h
RUN export CC="clang --target=aarch64-apple-darwin24 --sysroot=/sysroots/aarch64-apple-darwin24 -fuse-ld=lld" \
    && export CPP="clang -E --target=aarch64-apple-darwin24 --sysroot=/sysroots/aarch64-apple-darwin24" \
    && export MACHDEP=darwin \
    && wget -q "https://www.python.org/ftp/python/${PYTHON_VERSION}/Python-${PYTHON_VERSION}.tar.xz" -O /tmp/python.tar.xz \
    && tar xf /tmp/python.tar.xz -C /tmp \
    && cd "/tmp/Python-${PYTHON_VERSION}" \
    && sed -i '/# for now, limit cross builds to known configurations/,/as_fn_error/{/MACHDEP="unknown"/s/.*/:/; /as_fn_error/s/.*/:/}' configure \
    && ./configure --host=aarch64-apple-darwin24 --build=x86_64-pc-linux-musl \
        --with-build-python=$(which python3) --without-ensurepip --disable-ipv6 \
        ac_cv_file__dev_ptmx=yes ac_cv_file__dev_ptc=yes \
    && mkdir -p /tmp/python-include \
    && cp -r Include/. pyconfig.h /tmp/python-include/ \
    && rm -rf "/tmp/Python-${PYTHON_VERSION}" /tmp/python.tar.xz

RUN PY_VER_SHORT=$(echo "${PYTHON_VERSION}" | cut -d. -f1,2 | tr -d '.') \
    && echo "CPYTHON_ABIFLAGS=cpython-${PY_VER_SHORT}" >> /tmp/build-vars

ENV SYSROOT=/sysroots/aarch64-apple-darwin24
ENV TARGET=aarch64-apple-darwin24
ENV CXXFLAGS_COMMON="--target=${TARGET} --sysroot=${SYSROOT} -stdlib=libc++ -std=c++23 -fPIC -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables -fvisibility=hidden -fvisibility-inlines-hidden -fno-ident -I/tmp/python-include -I/pltxt2htm/include -DNDEBUG"

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY py/ /pltxt2htm/py/

# Compile to object file
RUN cd /pltxt2htm/py \
    && clang++ -c ${CXXFLAGS_COMMON} -O3 -flto \
        -o pltxt2htm.cc.o \
        pltxt2htm.cc

# Link macOS bundle
RUN . /tmp/build-vars \
    && cd /pltxt2htm/py \
    && clang++ --target=${TARGET} --sysroot=${SYSROOT} \
        -stdlib=libc++ -fuse-ld=lld \
        -bundle -undefined dynamic_lookup -flto \
        -o "pltxt2htm.${CPYTHON_ABIFLAGS}-darwin.so" \
        pltxt2htm.cc.o

# Package
RUN . /tmp/build-vars \
    && cd /pltxt2htm/py \
    && mkdir -p "aarch64-apple-darwin24-pltxt2htm-py-${mode}" \
    && cp "pltxt2htm.${CPYTHON_ABIFLAGS}-darwin.so" \
        "aarch64-apple-darwin24-pltxt2htm-py-${mode}/" \
    && tar cJf "aarch64-apple-darwin24-pltxt2htm-py-${mode}.tar.xz" \
        "aarch64-apple-darwin24-pltxt2htm-py-${mode}"

FROM alpine:3.23
WORKDIR /pltxt2htm/py
COPY --from=builder /pltxt2htm/py/aarch64-apple-darwin24-pltxt2htm-py-*.tar.xz .
