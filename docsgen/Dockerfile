FROM ubuntu:26.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    doxygen \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY README.md /src/
COPY include/pltxt2htm /src/include/pltxt2htm
COPY cxxmodule/pltxt2htm/pltxt2htm.cppm /src/cxxmodule/pltxt2htm/pltxt2htm.cppm
COPY docsgen /src/docsgen

WORKDIR /src/docsgen
RUN doxygen doxyfile
