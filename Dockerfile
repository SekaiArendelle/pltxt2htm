FROM ubuntu:26.04

RUN apt update && apt install -y --no-install-recommends \
    python3 git vim xz-utils wget ca-certificates \
    xmake \
    gcc g++ gdb \
    clang lld lldb libc++-dev libclang-rt-dev \
    lcov libcpanel-json-xs-perl

WORKDIR /pltxt2htm
COPY . .

ENV XMAKE_ROOT=y
CMD ["sleep", "infinity"]
