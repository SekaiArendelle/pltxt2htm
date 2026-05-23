FROM ubuntu:26.04

RUN apt update && apt install -y --no-install-recommends g++ cmake ninja-build

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY tests/ /pltxt2htm/tests/

RUN cmake -S tests -B build -GNinja -DCMAKE_BUILD_TYPE=Debug -DPLTXT2HTM_SANITIZER=address -DCMAKE_CXX_COMPILER=g++ \
    && cmake --build build \
    && ctest --test-dir build
