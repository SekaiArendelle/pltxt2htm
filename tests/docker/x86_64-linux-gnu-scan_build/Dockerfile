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

RUN apt update && apt install -y --no-install-recommends clang clang-tools cmake ninja-build lld perl

WORKDIR /pltxt2htm
COPY include/ /pltxt2htm/include/
COPY cmake/ /pltxt2htm/cmake/
COPY tests/ /pltxt2htm/tests/

RUN scan-build --status-bugs --use-cc=clang --use-c++=clang++ -o /scan-build-report \
        cmake -S tests -B build -GNinja -DCMAKE_BUILD_TYPE=Debug \
    && scan-build --status-bugs --use-cc=clang --use-c++=clang++ -o /scan-build-report \
        cmake --build build -v \
    && ctest --test-dir build -V -j $(nproc)
