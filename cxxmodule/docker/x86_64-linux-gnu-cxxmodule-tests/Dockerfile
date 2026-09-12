FROM gcc:16.2.0-trixie

RUN apt-get update && apt-get install -y --no-install-recommends cmake ninja-build \
	&& rm -rf /var/lib/apt/lists/*

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY cxxmodule/ /pltxt2htm/cxxmodule/

RUN cmake -S cxxmodule/tests -B cxxmodule/tests/build -G Ninja \
	-DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++
RUN cmake --build cxxmodule/tests/build -v
RUN ./cxxmodule/tests/build/check_public_symbols
