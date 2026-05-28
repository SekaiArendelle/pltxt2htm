FROM alpine:3.23 AS builder

RUN apk add --no-cache cmake ninja-build git zip mingw-w64-gcc

WORKDIR /pltxt2htm
# Copy the entire project to preserve git tracking history.
# A partial copy would distort pltxt2htm's git-tracked modification detection.
COPY ../../../ .

WORKDIR /pltxt2htm/cmd
ARG mode=debug
RUN cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=${mode} \
    -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
    -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
    -DCMAKE_SYSTEM_NAME=Windows
RUN cmake --build build -v
RUN cmake --install build --prefix x86_64-w64-mingw32-pltxt2htm-cmd-${mode}
RUN if [ "${mode}" = "debug" ]; then \
        cp /usr/x86_64-w64-mingw32/bin/libgcc_s_seh-1.dll ./x86_64-w64-mingw32-pltxt2htm-cmd-${mode}/; \
        cp /usr/x86_64-w64-mingw32/bin/libstdc++-6.dll ./x86_64-w64-mingw32-pltxt2htm-cmd-${mode}/; \
        cp /usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll ./x86_64-w64-mingw32-pltxt2htm-cmd-${mode}/; \
    fi
RUN zip -r x86_64-w64-mingw32-pltxt2htm-cmd-${mode}.zip x86_64-w64-mingw32-pltxt2htm-cmd-${mode}/

FROM alpine:3.23

WORKDIR /pltxt2htm
COPY --from=builder /pltxt2htm /pltxt2htm
