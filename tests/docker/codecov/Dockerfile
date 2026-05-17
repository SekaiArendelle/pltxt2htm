FROM ubuntu:26.04

RUN apt-get update && apt-get install -y --no-install-recommends g++ xmake lcov python3 xz-utils libcpanel-json-xs-perl && rm -rf /var/lib/apt/lists/*

WORKDIR /pltxt2htm
COPY ../../../include/ /pltxt2htm/include/
COPY ../../../xmake/ /pltxt2htm/xmake/
COPY ../../../tests/ /pltxt2htm/tests/

ENV XMAKE_ROOT=y

WORKDIR /pltxt2htm/tests
RUN xmake config --mode=coverage --toolchain=gcc && xmake test
RUN mkdir /codecov && lcov --rc geninfo_unexecuted_blocks=1 --capture --directory build/.objs --output-file /codecov/coverage.info --exclude "*/fast_io/*" --exclude "*/exception/*" --exclude "*/c++/*"
RUN genhtml /codecov/coverage.info --output-directory /codecov/html
RUN tar cJf /pltxt2htm-codecov.tar.xz /codecov
