FROM alpine:3.23 AS builder

RUN apk add --no-cache xmake git zip mingw-w64-gcc

WORKDIR /pltxt2htm
COPY ../../../ /pltxt2htm/

ENV XMAKE_ROOT=y

WORKDIR /pltxt2htm/cmd
ARG mode=debug
RUN xmake config --toolchain=x86_64-w64-mingw32-gcc --arch=x64 --plat=mingw -m ${mode}
RUN xmake build -v
RUN xmake install -o x86_64-w64-mingw32-pltxt2htm-cmd-${mode}
RUN if [ "${mode}" = "debug" ]; then \
        cp /usr/x86_64-w64-mingw32/bin/libgcc_s_seh-1.dll ./x86_64-w64-mingw32-pltxt2htm-cmd-${mode}/; \
        cp /usr/x86_64-w64-mingw32/bin/libstdc++-6.dll ./x86_64-w64-mingw32-pltxt2htm-cmd-${mode}/; \
        cp /usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll ./x86_64-w64-mingw32-pltxt2htm-cmd-${mode}/; \
    fi
RUN zip -r x86_64-w64-mingw32-pltxt2htm-cmd-${mode}.zip x86_64-w64-mingw32-pltxt2htm-cmd-${mode}/

FROM alpine:3.23

WORKDIR /pltxt2htm
COPY --from=builder /pltxt2htm /pltxt2htm
