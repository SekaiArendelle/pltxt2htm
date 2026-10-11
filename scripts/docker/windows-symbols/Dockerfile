FROM ubuntu:26.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    clang-21 \
    libc++-21-dev \
    llvm-21 \
    python3 \
    && rm -rf /var/lib/apt/lists/*

ENV PATH="/usr/lib/llvm-21/bin:${PATH}"

WORKDIR /pltxt2htm

CMD ["python3", "scripts/gen_windows_symbols.py", "--check"]
