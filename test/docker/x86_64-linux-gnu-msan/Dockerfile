FROM ubuntu:26.04

RUN apt update && apt install -y --no-install-recommends xmake python3 clang libclang-rt-dev

WORKDIR /pltxt2htm
COPY ../../../include/ /pltxt2htm/include/
COPY ../../../xmake/ /pltxt2htm/xmake/
COPY ../../../test/ /pltxt2htm/test/

ENV XMAKE_ROOT=y
CMD ["python3", "test/run_all_tests.py", "--compiler=clang", "--sanitizer=memory"]
