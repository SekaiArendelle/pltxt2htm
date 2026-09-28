# MSan only reports real bugs when every library linked into the test binaries is
# instrumented: a value written by uninstrumented code keeps its "uninitialized"
# shadow, so instrumented code reading it reports a false positive. That includes
# the C++ standard library, and Ubuntu's libstdc++/libc++ are both uninstrumented
# (the false positive shows up in doctest's global reporter registration, before any
# test body runs). The OSS-Fuzz clang image ships an MSan-instrumented libc++ under
# /usr/msan, so use it as the base (see README.md).
#
# The base image is pinned by digest so that the compiler and libc++ versions the tests
# are built against do not change under us (packages installed further down still come
# from the base image's own apt repositories). To update it:
#   docker manifest inspect gcr.io/oss-fuzz-base/base-clang
# and replace the digest below with the reported one; re-run this image locally, since
# that is a base image upgrade rather than a like-for-like replacement.
FROM gcr.io/oss-fuzz-base/base-clang@sha256:f096cfbe8c8b7734bf8cd990171d2c53a8dd97f2bdcc515bd9f594c4b9f6b248

# base-clang is a compiler image, not a general build environment: it provides clang
# and cmake, but neither ninja nor git.
RUN apt-get update \
    && apt-get install -y --no-install-recommends ninja-build \
    && rm -rf /var/lib/apt/lists/*

# Replace the CXXFLAGS baked into base-clang (they target OSS-Fuzz builds) with what
# this project needs. -nostdinc++ is required, otherwise the C++ headers come from
# the uninstrumented libstdc++ even with -stdlib=libc++ set; -L/usr/msan/lib makes
# the link step pick the instrumented libc++ over any system one.
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
