FROM alpine:3.23 AS sysroot

ENV SYSROOT=/sysroots/i586-linux-musl
WORKDIR ${SYSROOT}

RUN wget https://dl-cdn.alpinelinux.org/alpine/v3.23/releases/x86/alpine-minirootfs-3.23.0-x86.tar.gz \
    && tar xf alpine-minirootfs-3.23.0-x86.tar.gz \
    && rm alpine-minirootfs-3.23.0-x86.tar.gz

RUN apk add --no-cache --no-scripts \
    --repository https://dl-cdn.alpinelinux.org/alpine/v3.23/main \
    --repository https://dl-cdn.alpinelinux.org/alpine/v3.23/community \
    --arch x86 \
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
RUN xmake config --toolchain=clang --arch=x86 --plat=linux -m ${mode} --cxxflags='--target=i586-alpine-linux-musl --sysroot=/sysroots/i586-linux-musl' --ldflags='-static --target=i586-alpine-linux-musl --sysroot=/sysroots/i586-linux-musl/'
RUN xmake build -v
RUN xmake install -o i586-linux-musl-pltxt2htm-cmd-${mode}
RUN tar cJf i586-linux-musl-pltxt2htm-cmd-${mode}.tar.xz i586-linux-musl-pltxt2htm-cmd-${mode}/
