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

RUN apt-get update && apt-get install -y --no-install-recommends \
    python3 npm cargo git vim xz-utils wget curl ca-certificates file \
    cmake ninja-build \
    gcc g++ gdb \
    clang lld lldb clang-format clang-tidy libc++-dev libclang-rt-dev \
    lcov libcpanel-json-xs-perl

WORKDIR /pltxt2htm

CMD ["sleep", "infinity"]
