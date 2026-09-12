FROM ubuntu:26.04
RUN apt update && apt install -y --no-install-recommends clang clang-tidy cmake ninja-build python3
WORKDIR /pltxt2htm
COPY .clang-tidy /pltxt2htm/.clang-tidy
COPY include/ /pltxt2htm/include/
COPY scripts/ /pltxt2htm/scripts/
COPY examples/ /pltxt2htm/examples/
COPY cmd/ /pltxt2htm/cmd/
COPY c/ /pltxt2htm/c/
RUN python3 scripts/run_clang_tidy.py examples cmd c
