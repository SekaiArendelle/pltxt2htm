FROM ubuntu:26.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    clang-21 \
    llvm-21 \
    python3 \
    && rm -rf /var/lib/apt/lists/*

ENV PATH="/usr/lib/llvm-21/bin:${PATH}"

WORKDIR /pltxt2htm

# The probes use the repository's freestanding <cstddef>/<cstdint> shims, so the
# image does not need a C++ standard library for the MSVC target headers.
CMD ["python3", "scripts/gen_windows_symbols.py", "--check"]
