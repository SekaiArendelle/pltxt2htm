FROM ubuntu:26.04

RUN apt-get update && apt-get install -y --no-install-recommends g++ cmake ninja-build lcov python3 xz-utils libcpanel-json-xs-perl && rm -rf /var/lib/apt/lists/*

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY tests/ /pltxt2htm/tests/

RUN cmake -S tests -B build -GNinja -DCMAKE_BUILD_TYPE=Debug -DPLTXT2HTM_ENABLE_COVERAGE=ON -DCMAKE_CXX_COMPILER=g++ \
    && cmake --build build \
    && ctest --test-dir build
RUN mkdir /codecov && lcov --rc geninfo_unexecuted_blocks=1 --capture --directory build --output-file /codecov/coverage.info --exclude "*/fast_io/*" --exclude "*/exception/*" --exclude "*/c++/*"
RUN genhtml /codecov/coverage.info --output-directory /codecov/html
RUN tar cJf /pltxt2htm-codecov.tar.xz /codecov
