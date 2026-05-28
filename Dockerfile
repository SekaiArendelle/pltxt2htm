FROM ubuntu:26.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    python3 git vim xz-utils wget ca-certificates \
    xmake cmake \
    gcc g++ gdb \
    clang lld lldb libc++-dev libclang-rt-dev \
    lcov libcpanel-json-xs-perl

WORKDIR /pltxt2htm

ENV XMAKE_ROOT=y
CMD ["sleep", "infinity"]
