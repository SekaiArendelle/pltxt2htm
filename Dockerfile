FROM ubuntu:26.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    python3 npm cargo git vim xz-utils wget curl ca-certificates file \
    cmake ninja-build \
    gcc g++ gdb \
    clang lld lldb libc++-dev libclang-rt-dev \
    lcov libcpanel-json-xs-perl

WORKDIR /pltxt2htm

CMD ["sleep", "infinity"]
