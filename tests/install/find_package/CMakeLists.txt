cmake_minimum_required(VERSION 3.25)
project(pltxt2htm-consumer-find-package LANGUAGES CXX)

# Smoke test of an installed pltxt2htm: configure with
# -DCMAKE_PREFIX_PATH=<prefix the library was installed into>.

# Passing a version exercises the compatibility policy of the package as well;
# tests/docker/x86_64-linux-gnu-install passes the repository VERSION here.
set(PLTXT2HTM_FIND_VERSION "" CACHE STRING "Version to request from find_package (empty: any version)")

if(PLTXT2HTM_FIND_VERSION STREQUAL "")
    find_package(pltxt2htm CONFIG REQUIRED)
else()
    find_package(pltxt2htm ${PLTXT2HTM_FIND_VERSION} CONFIG REQUIRED)
endif()

add_executable(install_find_package ../main.cc)
target_link_libraries(install_find_package PRIVATE pltxt2htm::pltxt2htm)

target_compile_definitions(install_find_package PRIVATE
    PLTXT2HTM_CONSUMER_EXPECTED_MAJOR=${pltxt2htm_VERSION_MAJOR}
    PLTXT2HTM_CONSUMER_EXPECTED_MINOR=${pltxt2htm_VERSION_MINOR}
    PLTXT2HTM_CONSUMER_EXPECTED_PATCH=${pltxt2htm_VERSION_PATCH})
