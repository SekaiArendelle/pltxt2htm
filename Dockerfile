FROM alpine:latest

RUN apk update && apk add --no-cache g++ git xmake python3 linux-headers

RUN export XMAKE_ROOT=y

WORKDIR /pltxt2htm
COPY . .
CMD ["sleep", "infinity"]
