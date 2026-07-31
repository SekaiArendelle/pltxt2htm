FROM ubuntu:26.04

RUN apt update && apt install -y --no-install-recommends clang clang-tools cmake ninja-build lld perl

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY tests/ /pltxt2htm/tests/

RUN scan-build --status-bugs --use-cc=clang --use-c++=clang++ -o /scan-build-report \
        cmake -S tests -B build -GNinja -DCMAKE_BUILD_TYPE=Debug \
    && scan-build --status-bugs --use-cc=clang --use-c++=clang++ -o /scan-build-report \
        cmake --build build -v \
    && ctest --test-dir build -V -j $(nproc)
