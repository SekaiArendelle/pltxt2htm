# TODO use v2.23 when it's released (which contains clang21)
FROM alpine:latest

ARG _MINIMAL_IMAGE

RUN apk update
# Install basic tools
RUN apk add --no-cache g++ xmake python3 linux-headers
# Install development tools
RUN if [ -z $_MINIMAL_IMAGE ] ; then apk add --no-cache git vim ; fi
# Install coverage report tools
RUN if [ -z $_MINIMAL_IMAGE ] ; then apk add --no-cache gzip lcov perl-cpanel-json-xs ; fi

WORKDIR /pltxt2htm
COPY . .

ENV XMAKE_ROOT=y
CMD ["sleep", "infinity"]
