FROM ubuntu:26.04

RUN apt update && apt install -y --no-install-recommends \
    doxygen ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

WORKDIR /src/docsgen
RUN doxygen doxyfile
