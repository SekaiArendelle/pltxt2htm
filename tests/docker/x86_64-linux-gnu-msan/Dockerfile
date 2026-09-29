# OSS-Fuzz provides an MSan-instrumented libc++ under /usr/msan. Pin the image so
# the compiler and standard library update together; see README.md for details.
FROM gcr.io/oss-fuzz-base/base-clang@sha256:f096cfbe8c8b7734bf8cd990171d2c53a8dd97f2bdcc515bd9f594c4b9f6b248

# The compiler image includes CMake but not Ninja.
RUN apt-get update \
    && apt-get install -y --no-install-recommends ninja-build \
    && rm -rf /var/lib/apt/lists/*

# Replace the OSS-Fuzz build flags and select the matching libc++ headers and library.
ENV CXXFLAGS="-O1 -fno-omit-frame-pointer -stdlib=libc++ -nostdinc++ -isystem /usr/msan/include/c++/v1 -L/usr/msan/lib"

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY tests/ /pltxt2htm/tests/

RUN cmake -S tests -B build -GNinja -DCMAKE_BUILD_TYPE=Debug -DPLTXT2HTM_SANITIZER=memory -DCMAKE_CXX_COMPILER=clang++ \
    && cmake --build build -v

# Tests run via `docker run --security-opt seccomp=unconfined` because MSan needs
# personality(ADDR_NO_RANDOMIZE) to disable ASLR, which Docker's default seccomp
# profile blocks (see README.md).
CMD ["/bin/sh", "-c", "ctest --test-dir build -V -j $(nproc)"]
