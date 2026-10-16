# Consumer-side check of the CMake package: install the header-only library, then
# build two independent projects against the exported target.
FROM ubuntu:26.04 AS package

RUN apt update && apt install -y --no-install-recommends cmake ninja-build ca-certificates

WORKDIR /pltxt2htm

# Only the files the package itself consists of, so an installation that needs
# anything else fails to build here.
COPY CMakeLists.txt VERSION LICENSE /pltxt2htm/
COPY cmake/ /pltxt2htm/cmake/
COPY include/ /pltxt2htm/include/
COPY tests/install/ /pltxt2htm/tests/install/

ENV PLTXT2HTM_PREFIX=/opt/pltxt2htm

# 1. Install without a C++ compiler and check the layout.
RUN ! command -v c++ && ! command -v g++ && ! command -v clang++ \
    && cmake -S . -B build -G Ninja -DCMAKE_INSTALL_PREFIX=${PLTXT2HTM_PREFIX} \
    && cmake --install build \
    && test -f "${PLTXT2HTM_PREFIX}/include/pltxt2htm/pltxt2htm.hh" \
    && test -f "${PLTXT2HTM_PREFIX}/include/fast_io/fast_io_core.h" \
    && test -f "${PLTXT2HTM_PREFIX}/lib/cmake/pltxt2htm/pltxt2htmConfig.cmake" \
    && test -f "${PLTXT2HTM_PREFIX}/lib/cmake/pltxt2htm/pltxt2htmConfigVersion.cmake" \
    && test -f "${PLTXT2HTM_PREFIX}/lib/cmake/pltxt2htm/pltxt2htmTargets.cmake" \
    && test -f "${PLTXT2HTM_PREFIX}/lib/cmake/pltxt2htm/pltxt2htmTargetOptions.cmake" \
    && cmake -S tests/install/platform_options -B build-platform-options -G Ninja \
        -DCMAKE_PREFIX_PATH=${PLTXT2HTM_PREFIX}

FROM ubuntu:26.04 AS consumers

ARG compiler=g++

RUN apt update && apt install -y --no-install-recommends g++ clang cmake ninja-build ca-certificates

COPY --from=package /opt/pltxt2htm /opt/pltxt2htm

WORKDIR /pltxt2htm
COPY CMakeLists.txt VERSION LICENSE /pltxt2htm/
COPY cmake/ /pltxt2htm/cmake/
COPY include/ /pltxt2htm/include/
COPY tests/install/ /pltxt2htm/tests/install/

ENV PLTXT2HTM_PREFIX=/opt/pltxt2htm

# 2. The in-tree consumer needs the source tree, so it runs before that is removed.
RUN cat VERSION > /tmp/pltxt2htm-version \
    && cmake -S tests/install/add_subdirectory -B build-in-tree -G Ninja -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_CXX_COMPILER=${compiler} \
    && cmake --build build-in-tree \
    && ./build-in-tree/install_add_subdirectory | grep -q '<h1' \
    && cp -r tests/install /tmp/consumer-project

# 3. The find_package consumer has to work from the installation alone, so it is
#    configured after the source tree is gone.
WORKDIR /tmp

RUN rm -rf /pltxt2htm \
    && cmake -S /tmp/consumer-project/find_package -B /tmp/build-find-package -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_CXX_COMPILER=${compiler} \
        -DCMAKE_PREFIX_PATH=${PLTXT2HTM_PREFIX} \
        -DPLTXT2HTM_FIND_VERSION="$(cat /tmp/pltxt2htm-version)" \
    && cmake --build /tmp/build-find-package \
    && /tmp/build-find-package/install_find_package | grep -q '<h1'
