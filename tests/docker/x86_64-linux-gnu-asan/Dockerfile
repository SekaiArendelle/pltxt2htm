FROM ubuntu:26.04

RUN apt update && apt install -y --no-install-recommends g++ cmake ninja-build git ca-certificates

WORKDIR /pltxt2htm
COPY CMakeLists.txt LICENSE VERSION /pltxt2htm/
COPY cmake/ /pltxt2htm/cmake/
COPY include/ /pltxt2htm/include/
COPY tests/ /pltxt2htm/tests/

RUN cmake -S . -B build -GNinja -DCMAKE_BUILD_TYPE=Debug -DPLTXT2HTM_BUILD_TESTS=ON -DPLTXT2HTM_SANITIZER=address -DCMAKE_CXX_COMPILER=g++ \
    && cmake --build build -v \
    && ctest --test-dir build -V -j $(nproc)
