FROM alpine:latest

RUN apk update && apk add --no-cache g++ git xmake python3 linux-headers

WORKDIR /pltxt2htm
COPY . .

ENV XMAKE_ROOT=y
CMD ["sleep", "infinity"]
