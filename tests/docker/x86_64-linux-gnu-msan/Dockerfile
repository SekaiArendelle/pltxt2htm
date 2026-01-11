FROM ubuntu:26.04

RUN apt update && apt install -y --no-install-recommends xmake python3 clang libclang-rt-dev

WORKDIR /pltxt2htm
COPY ../../../include/ /pltxt2htm/include/
COPY ../../../xmake/ /pltxt2htm/xmake/
COPY ../../../tests/ /pltxt2htm/tests/

ENV XMAKE_ROOT=y
CMD ["python3", "tests/run_all_tests.py", "--compiler=clang", "--sanitizer=memory"]
