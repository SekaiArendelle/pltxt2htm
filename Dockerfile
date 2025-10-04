# TODO: Update to Ubuntu 26.04 LTS
FROM ubuntu:25.04

ARG _MINIMAL_IMAGE

RUN apt update
# Install basic tools
RUN apt install -y --no-install-recommends g++ xmake python3
# Install development tools
RUN if [ -z ${_MINIMAL_IMAGE} ] ; then \
    apt install -y --no-install-recommends gdb git vim; \
    fi
# Install clang tools
RUN if [ -z ${_MINIMAL_IMAGE} ] ; then \
    apt install -y --no-install-recommends clang lld lldb libc++-dev libclang-rt-dev; \
    fi
# Install coverage report tools
RUN if [ -z ${_MINIMAL_IMAGE} ] ; then \
    apt install -y --no-install-recommends lcov libcpanel-json-xs-perl; \
    fi

WORKDIR /pltxt2htm
COPY . .

ENV XMAKE_ROOT=y
CMD ["sleep", "infinity"]
