FROM ubuntu:26.04

RUN apt-get update && apt-get install -y --no-install-recommends cmake ninja-build clang

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY cxxmodule/ /pltxt2htm/cxxmodule/

WORKDIR /pltxt2htm/cxxmodule

RUN cmake -S examples -B examples/build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++
RUN cmake --build examples/build -v
