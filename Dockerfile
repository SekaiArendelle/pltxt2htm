FROM ubuntu:26.04

# Route apt through authoritative Ubuntu archive mirrors from several countries
# (US origin first, then CN / JP / DE / UK); apt falls back to the next mirror
# automatically when one is unavailable.
RUN set -eux; \
    printf '%b\n' \
        'http://archive.ubuntu.com/ubuntu/\tpriority:1' \
        'http://mirrors.tuna.tsinghua.edu.cn/ubuntu/\tpriority:2' \
        'http://ftp.jaist.ac.jp/pub/Linux/ubuntu/\tpriority:2' \
        'http://de.archive.ubuntu.com/ubuntu/\tpriority:2' \
        'http://uk.archive.ubuntu.com/ubuntu/\tpriority:2' \
        > /etc/apt/pltxt2htm-ubuntu-mirrors.lst; \
    sed -i 's|^URIs: .*|URIs: mirror+file:/etc/apt/pltxt2htm-ubuntu-mirrors.lst|' \
        /etc/apt/sources.list.d/ubuntu.sources; \
    grep -q '^URIs: mirror+file:' /etc/apt/sources.list.d/ubuntu.sources

# Compilers and language toolchains come from the Pixi environments declared in
# pixi.toml, so apt only provides the bootstrap and interactive utilities that
# Pixi does not manage.
RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates curl git vim xz-utils wget file \
    gdb lcov libcpanel-json-xs-perl \
    && rm -rf /var/lib/apt/lists/*

# Install Pixi. It provisions every toolchain (clang, gcc, rust, emscripten,
# dotnet, python, ...) from pixi.lock, e.g. `pixi run -e clang test-clang`.
# The pinned release asset is downloaded directly instead of piping a moving
# installer script, so the build is reproducible. Bump PIXI_VERSION when the
# lock file requires a newer Pixi; override PIXI_REPOURL with a GitHub mirror
# (for example https://<mirror>/https://github.com/prefix-dev/pixi) when
# GitHub is unreachable from the build host.
ARG PIXI_VERSION=v0.84.2
ARG PIXI_REPOURL=https://github.com/prefix-dev/pixi
RUN set -eux; \
    arch="$(uname -m)"; \
    case "$arch" in \
        x86_64|aarch64) ;; \
        *) echo "unsupported architecture: ${arch}" >&2; exit 1 ;; \
    esac; \
    tag="v${PIXI_VERSION#v}"; \
    curl -fsSL --retry 5 --retry-all-errors --retry-delay 5 \
        -o /tmp/pixi.tar.gz \
        "${PIXI_REPOURL%/}/releases/download/${tag}/pixi-${arch}-unknown-linux-musl.tar.gz"; \
    tar -xzf /tmp/pixi.tar.gz -C /usr/local/bin pixi; \
    rm -f /tmp/pixi.tar.gz; \
    pixi --version

WORKDIR /pltxt2htm

# The source tree is bind-mounted here at runtime (see AGENTS.md), so Pixi
# environments are created inside the mounted checkout on first use.
CMD ["sleep", "infinity"]
