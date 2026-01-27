FROM alpine:3.23 AS sysroot

ENV SYSROOT=/sysroots/x86_64-linux-musl
WORKDIR ${SYSROOT}

RUN wget https://dl-cdn.alpinelinux.org/alpine/v3.23/releases/x86_64/alpine-minirootfs-3.23.0-x86_64.tar.gz \
    && tar xf alpine-minirootfs-3.23.0-x86_64.tar.gz \
    && rm alpine-minirootfs-3.23.0-x86_64.tar.gz

RUN apk add --no-cache --no-scripts \
    --repository https://dl-cdn.alpinelinux.org/alpine/v3.23/main \
    --repository https://dl-cdn.alpinelinux.org/alpine/v3.23/community \
    --arch x86_64 \
    --root "${SYSROOT}" \
    linux-headers musl-dev libstdc++-dev gcc

FROM alpine:3.23 AS builder

RUN apk update && apk add --no-cache git xz xmake clang lld

WORKDIR /pltxt2htm
COPY ../../../ /pltxt2htm/

COPY --from=sysroot /sysroots /sysroots

ENV XMAKE_ROOT=y

WORKDIR /pltxt2htm/cmd
ARG mode=debug
RUN xmake config --toolchain=clang --arch=x64 --plat=linux -m ${mode} --cxxflags='--target=x86_64-alpine-linux-musl --sysroot=/sysroots/x86_64-linux-musl' --ldflags='-static --target=x86_64-alpine-linux-musl --sysroot=/sysroots/x86_64-linux-musl/'
RUN xmake build -v
RUN xmake install -o x86_64-linux-musl-pltxt2htm-cmd-${mode}
RUN tar cJf x86_64-linux-musl-pltxt2htm-cmd-${mode}.tar.xz x86_64-linux-musl-pltxt2htm-cmd-${mode}/