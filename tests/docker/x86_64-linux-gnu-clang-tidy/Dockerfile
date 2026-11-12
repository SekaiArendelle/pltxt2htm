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

RUN apt update && apt install -y --no-install-recommends clang clang-tidy cmake ninja-build python3
WORKDIR /pltxt2htm
COPY .clang-tidy /pltxt2htm/.clang-tidy
COPY include/ /pltxt2htm/include/
COPY cmake/ /pltxt2htm/cmake/
COPY scripts/ /pltxt2htm/scripts/
COPY examples/ /pltxt2htm/examples/
COPY cmd/ /pltxt2htm/cmd/
COPY c/ /pltxt2htm/c/
RUN python3 scripts/run_clang_tidy.py examples cmd c
