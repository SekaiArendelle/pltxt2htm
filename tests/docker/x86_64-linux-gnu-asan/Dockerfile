FROM ubuntu:26.04

RUN apt update && apt install -y --no-install-recommends g++ cmake ninja-build git ca-certificates libdw-dev pkg-config

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY cmake/ /pltxt2htm/cmake/
COPY tests/ /pltxt2htm/tests/

RUN cmake -S tests -B build -GNinja -DCMAKE_BUILD_TYPE=Debug -DPLTXT2HTM_SANITIZER=address \
        -DPLTXT2HTM_ENABLE_STACKTRACE=ON -DCMAKE_CXX_COMPILER=g++ 2>&1 | tee /tmp/pltxt2htm-configure.log \
    && grep -F "pltxt2htm stacktrace backend: execinfo + libdwfl" /tmp/pltxt2htm-configure.log \
    && cmake --build build -v \
    && ctest --test-dir build -V -j $(nproc)
