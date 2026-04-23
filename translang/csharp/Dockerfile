FROM ubuntu:26.04

RUN apt-get update && apt-get install -y --no-install-recommends curl ca-certificates xz-utils

RUN curl -fsSL https://pixi.sh/install.sh | bash
ENV PATH=/root/.pixi/bin:$PATH

WORKDIR /pltxt2htm
COPY . .

WORKDIR /pltxt2htm/translang/csharp
RUN pixi install && pixi run install

RUN ./dist/pltxt2htm_csharp_translator --output-dir /pltxt2htm-translang-csharp

WORKDIR /
RUN tar cJf pltxt2htm-translang-csharp.tar.xz pltxt2htm-translang-csharp
