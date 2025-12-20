FROM ubuntu:26.04

RUN apt-get update && apt-get install -y --no-install-recommends xmake clang

WORKDIR /pltxt2htm
COPY . .

ENV XMAKE_ROOT=y

WORKDIR /pltxt2htm/cxxmodule

RUN xmake config -m debug --toolchain=clang
RUN xmake build -v
