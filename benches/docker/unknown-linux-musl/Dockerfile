FROM alpine:3.23

RUN apk add --no-cache cmake ninja g++ git ca-certificates

WORKDIR /app
COPY benches/ benches/
COPY include/ include/

RUN cmake -S benches -B benches/build -G Ninja -DCMAKE_BUILD_TYPE=Release && \
    cmake --build benches/build
