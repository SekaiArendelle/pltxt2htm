FROM ubuntu:26.04

RUN apt update && apt install -y --no-install-recommends cmake ninja-build clang libclang-rt-dev

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY tests/ /pltxt2htm/tests/

RUN cmake -S tests -B build -GNinja -DCMAKE_BUILD_TYPE=Debug -DPLTXT2HTM_SANITIZER=memory -DCMAKE_CXX_COMPILER=clang++ \
    && cmake --build build -v

# Tests run via `docker run --security-opt seccomp=unconfined` because MSan needs
# personality(ADDR_NO_RANDOMIZE) to disable ASLR, which Docker's default seccomp
# profile blocks (see README.md).
CMD ["/bin/sh", "-c", "ctest --test-dir build -V -j $(nproc)"]
