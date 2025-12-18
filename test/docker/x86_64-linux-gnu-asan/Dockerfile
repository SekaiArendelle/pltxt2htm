# TODO: Update to Ubuntu 26.04 LTS
FROM ubuntu:25.04

RUN apt update && apt install -y --no-install-recommends g++ xmake python3

WORKDIR /pltxt2htm
COPY ../../../include/ /pltxt2htm/include/
COPY ../../../xmake/ /pltxt2htm/xmake/
COPY ../../../test/ /pltxt2htm/test/

ENV XMAKE_ROOT=y
CMD ["python3", "test/run_all_tests.py", "--compiler=gcc", "--sanitizer=address"]
