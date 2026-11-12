# OSS-Fuzz provides an MSan-instrumented libc++ under /usr/msan. Pin the image so
# the compiler and standard library update together; see README.md for details.
FROM gcr.io/oss-fuzz-base/base-clang@sha256:f096cfbe8c8b7734bf8cd990171d2c53a8dd97f2bdcc515bd9f594c4b9f6b248

# Route apt through authoritative Ubuntu archive mirrors from several countries
# (US origin first, then CN / JP / DE / UK); apt falls back to the next mirror
# automatically when one is unavailable. The base image is ubuntu:20.04, whose
# sources are the one-line /etc/apt/sources.list format.
RUN set -eux; \
    printf '%b\n' \
        'http://archive.ubuntu.com/ubuntu/\tpriority:1' \
        'http://mirrors.tuna.tsinghua.edu.cn/ubuntu/\tpriority:2' \
        'http://ftp.jaist.ac.jp/pub/Linux/ubuntu/\tpriority:2' \
        'http://de.archive.ubuntu.com/ubuntu/\tpriority:2' \
        'http://uk.archive.ubuntu.com/ubuntu/\tpriority:2' \
        > /etc/apt/pltxt2htm-ubuntu-mirrors.lst; \
    sed -i -e 's|^deb http://archive.ubuntu.com/ubuntu/ |deb mirror+file:/etc/apt/pltxt2htm-ubuntu-mirrors.lst |' \
           -e 's|^deb http://security.ubuntu.com/ubuntu/ |deb mirror+file:/etc/apt/pltxt2htm-ubuntu-mirrors.lst |' \
        /etc/apt/sources.list; \
    grep -q '^deb mirror+file:' /etc/apt/sources.list

# The compiler image includes CMake but not Ninja.
RUN apt-get update \
    && apt-get install -y --no-install-recommends ninja-build \
    && rm -rf /var/lib/apt/lists/*

# Replace the OSS-Fuzz build flags and select the matching libc++ headers and library.
ENV CXXFLAGS="-O1 -fno-omit-frame-pointer -stdlib=libc++ -nostdinc++ -isystem /usr/msan/include/c++/v1 -L/usr/msan/lib"

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY cmake/ /pltxt2htm/cmake/
COPY tests/ /pltxt2htm/tests/

RUN cmake -S tests -B build -GNinja -DCMAKE_BUILD_TYPE=Debug -DPLTXT2HTM_SANITIZER=memory -DCMAKE_CXX_COMPILER=clang++ \
    && cmake --build build -v

# Tests run via `docker run --security-opt seccomp=unconfined` because MSan needs
# personality(ADDR_NO_RANDOMIZE) to disable ASLR, which Docker's default seccomp
# profile blocks (see README.md).
CMD ["/bin/sh", "-c", "ctest --test-dir build -V -j $(nproc)"]
