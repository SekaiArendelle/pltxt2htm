FROM ubuntu:26.04

# Route apt through authoritative Ubuntu archive mirrors from several countries
# (US origin first, then CN / JP / DE / UK); apt falls back to the next mirror
# automatically when one is unavailable.
RUN set -eux; \
    printf '%b\n' \
        'http://archive.ubuntu.com/ubuntu/\tpriority:1' \
        'http://mirrors.tuna.tsinghua.edu.cn/ubuntu/\tpriority:2' \
        'http://ftp.jaist.ac.jp/pub/Linux/ubuntu/\tpriority:2' \
        'http://de.archive.ubuntu.com/ubuntu/\tpriority:2' \
        'http://uk.archive.ubuntu.com/ubuntu/\tpriority:2' \
        > /etc/apt/pltxt2htm-ubuntu-mirrors.lst; \
    sed -i 's|^URIs: .*|URIs: mirror+file:/etc/apt/pltxt2htm-ubuntu-mirrors.lst|' \
        /etc/apt/sources.list.d/ubuntu.sources; \
    grep -q '^URIs: mirror+file:' /etc/apt/sources.list.d/ubuntu.sources

RUN apt-get update && apt-get install -y --no-install-recommends g++ cmake ninja-build lcov python3 xz-utils libcpanel-json-xs-perl && rm -rf /var/lib/apt/lists/*

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY cmake/ /pltxt2htm/cmake/
COPY tests/ /pltxt2htm/tests/

RUN cmake -S tests -B build -GNinja -DCMAKE_BUILD_TYPE=Debug -DPLTXT2HTM_ENABLE_COVERAGE=ON -DCMAKE_CXX_COMPILER=g++ \
    && cmake --build build -v \
    && ctest --test-dir build -V -j $(nproc)
RUN mkdir /codecov && lcov --rc geninfo_unexecuted_blocks=1 --capture --directory build --output-file /codecov/coverage.info --exclude "*/fast_io/*" --exclude "*/c++/*" --exclude "*/tests/vendor/doctest/*"
RUN genhtml /codecov/coverage.info --output-directory /codecov/html
RUN tar cJf /pltxt2htm-codecov.tar.xz /codecov
