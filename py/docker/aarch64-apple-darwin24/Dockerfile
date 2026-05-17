FROM alpine:3.23 AS builder

ARG MACOS_SDK_URL=https://github.com/joseluisq/macosx-sdks/releases/download/15.2/MacOSX15.2.sdk.tar.xz
ARG PYTHON_VERSION=3.12.10
ARG mode=release

RUN apk add --no-cache clang git lld wget xz

# Download and extract macOS SDK
RUN wget -q "${MACOS_SDK_URL}" -O /tmp/macos-sdk.tar.xz \
    && mkdir -p /sysroots/aarch64-apple-darwin24 \
    && tar xf /tmp/macos-sdk.tar.xz -C /sysroots/aarch64-apple-darwin24 --strip-components=1 \
    && rm /tmp/macos-sdk.tar.xz

# Download Python headers and generate pyconfig.h for the target
RUN wget -q "https://www.python.org/ftp/python/${PYTHON_VERSION}/Python-${PYTHON_VERSION}.tar.xz" -O /tmp/python.tar.xz \
    && tar xf /tmp/python.tar.xz -C /tmp \
    && mv "/tmp/Python-${PYTHON_VERSION}/Include" /tmp/python-include \
    && rm -rf "/tmp/Python-${PYTHON_VERSION}" /tmp/python.tar.xz \
    && cat > /tmp/python-include/pyconfig.h << 'PYCONFIGEOF'
#ifndef Py_CONFIG_H
#define Py_CONFIG_H
#define SIZEOF_SHORT 2
#define SIZEOF_INT 4
#define SIZEOF_LONG 8
#define SIZEOF_LONG_LONG 8
#define SIZEOF_VOID_P 8
#define SIZEOF_SIZE_T 8
#define SIZEOF_TIME_T 8
#define SIZEOF_DOUBLE 8
#define SIZEOF_FLOAT 4
#define SIZEOF_WCHAR_T 4
#define SIZEOF__BOOL 1
#define SIZEOF_PID_T 4
#define SIZEOF_OFF_T 8
#define SIZEOF_FPOS_T 8
#define SIZEOF_HKEY 8
#define ALIGNOF_SIZE_T 8
#define ALIGNOF_LONG 8
#define ALIGNOF_MAX_ALIGN_T 8
#define DOUBLE_IS_LITTLE_ENDIAN_IEEE754 1
#define HAVE_PROTOTYPES
#define STDC_HEADERS 1
#define HAVE_STDDEF_H 1
#define HAVE_STDINT_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_STRING_H 1
#define HAVE_STRINGS_H 1
#define HAVE_UNISTD_H 1
#define HAVE_DLFCN_H 1
#define HAVE_FCNTL_H 1
#define HAVE_SIGNAL_H 1
#define HAVE_ERRNO_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_SELECT_H 1
#define HAVE_SYS_MMAN_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_SYS_UN_H 1
#define HAVE_UTIME_H 1
#define HAVE_WCHAR_H 1
#define HAVE_LANGINFO_H 1
#define HAVE_PTHREAD_H 1
#define HAVE_NETDB_H 1
#define HAVE_NETINET_IN_H 1
#define HAVE_ARPA_INET_H 1
#define HAVE_DYNAMIC_LOADING 1
#define HAVE_LARGEFILE_SUPPORT 1
#define WITH_THREAD 1
#define WITH_PYMALLOC 1
#define WITH_FREELISTS 1
#define WITH_DOC_STRINGS 1
#define WITH_DECIMAL_CONTEXTVAR 1
#define HAVE_GETHOSTNAME 1
#define HAVE_GETHOSTBYADDR 1
#define HAVE_GETHOSTBYNAME 1
#define HAVE_GETPROTOBYNAME 1
#define HAVE_GETSERVBYNAME 1
#define HAVE_GETSERVBYPORT 1
#define HAVE_INET_PTON 1
#define HAVE_INET_NTOA 1
#define HAVE_ACCEPT 1
#define HAVE_BIND 1
#define HAVE_CONNECT 1
#define HAVE_GETSOCKNAME 1
#define HAVE_LISTEN 1
#define HAVE_RECVFROM 1
#define HAVE_SENDTO 1
#define HAVE_SETSOCKOPT 1
#define HAVE_SOCKET 1
#define HAVE_SHUTDOWN 1
#define HAVE_CLOCK 1
#define HAVE_CLOCK_GETTIME 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_GETPID 1
#define HAVE_GETCWD 1
#define HAVE_STRERROR 1
#define HAVE_MKTIME 1
#define HAVE_SETVBUF 1
#define HAVE_DUP 1
#define HAVE_PUTENV 1
#define HAVE_UMASK 1
#define HAVE_READLINK 1
#define HAVE_SYMLINK 1
#define HAVE_LSTAT 1
#define HAVE_TIMES 1
#define HAVE_UNAME 1
#define HAVE_WAITPID 1
#define HAVE_GETPGRP 1
#define HAVE_SETPGRP 1
#define HAVE_SETSID 1
#define HAVE_SETPGID 1
#define HAVE_TCGETPGRP 1
#define HAVE_TCSETPGRP 1
#define HAVE_NICE 1
#define HAVE_SIGINTERRUPT 1
#define HAVE_ERF 1
#define HAVE_ERFC 1
#define HAVE_ZLIB_COPY 1
#define HAVE_TZNAME 1
#define HAVE_WCSFTIME 1
#define HAVE_WCSCOLL 1
#define HAVE_WCSXFRM 1
#define HAVE_UINTPTR_T 1
#define HAVE_INTPTR_T 1
#define HAVE_DECL_TZNAME 1
#define PY_LONG_LONG long long
#define PY_LLONG_MAX LLONG_MAX
#define PY_LLONG_MIN LLONG_MIN
#define PY_ULLONG_MAX ULLONG_MAX
#define PY_UINT32_T uint32_t
#define PY_UINT64_T uint64_t
#define PY_INT32_T int32_t
#define PY_INT64_T int64_t
#define Py_ssize_t ssize_t
#define HAVE_PY_SSIZE_T 1
#define PLATFORM "darwin"
#define PYD_PLATFORM_TAG "macosx_arm64"
#define COMPILER "[clang]"
#define PY_SUPPORT_TIER 1
#define RETSIGTYPE void
#define _PYTHONFRAMEWORK ""
#endif
PYCONFIGEOF

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
