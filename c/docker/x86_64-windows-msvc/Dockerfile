FROM ubuntu:26.04

RUN apt-get update && apt-get install -y --no-install-recommends --fix-missing \
    ca-certificates curl xz-utils zip python3 \
    cmake ninja-build llvm clang lld

RUN set -eux; \
    XWIN_VERSION=0.9.0; \
    curl -fsSL "https://github.com/Jake-Shadle/xwin/releases/download/${XWIN_VERSION}/xwin-${XWIN_VERSION}-x86_64-unknown-linux-musl.tar.gz" \
    | tar xz -C /usr/local/bin --strip-components=1 "xwin-${XWIN_VERSION}-x86_64-unknown-linux-musl/xwin"

RUN xwin --accept-license --include-debug-runtime splat --include-debug-libs --output /opt/xwin

WORKDIR /pltxt2htm
COPY ../../../include /pltxt2htm/include/
COPY ../../../c /pltxt2htm/c/

WORKDIR /pltxt2htm/c
ARG mode=debug
ARG crt_kind=dynamic

RUN if [ "${crt_kind}" = "dynamic" ]; then \
        CXXFLAGS="-fms-runtime-lib=dll -isystem/opt/xwin/crt/include -isystem/opt/xwin/sdk/include/ucrt -isystem/opt/xwin/sdk/include/um -isystem/opt/xwin/sdk/include/shared -D_CRT_SECURE_NO_WARNINGS"; \
        SHFLAGS="-fuse-ld=lld -L/opt/xwin/crt/lib/x86_64 -L/opt/xwin/sdk/lib/ucrt/x86_64 -L/opt/xwin/sdk/lib/um/x86_64"; \
    elif [ "${crt_kind}" = "static" ]; then \
        CXXFLAGS="-fms-runtime-lib=static -isystem/opt/xwin/crt/include -isystem/opt/xwin/sdk/include/ucrt -isystem/opt/xwin/sdk/include/um -isystem/opt/xwin/sdk/include/shared -D_CRT_SECURE_NO_WARNINGS"; \
        SHFLAGS="-fuse-ld=lld -L/opt/xwin/crt/lib/x86_64 -L/opt/xwin/sdk/lib/ucrt/x86_64 -L/opt/xwin/sdk/lib/um/x86_64"; \
    else \
        echo "Error: crt_kind must be 'static' or 'dynamic', got '${crt_kind}'" && exit 1; \
    fi && \
    python3 dist.py --mode=${mode} --toolchain=clang --target=x86_64-windows-msvc \
        --cxxflags="${CXXFLAGS}" --shflags="${SHFLAGS}"
RUN zip -r x86_64-windows-msvc-pltxt2htm-c-${mode}.zip x86_64-windows-msvc-pltxt2htm-c-${mode}/
