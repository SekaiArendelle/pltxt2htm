cmake_minimum_required(VERSION 3.25)
project(pltxt2htm-consumer-add-subdirectory LANGUAGES CXX)

# Smoke test of the in-tree usage: consume the source tree directly. FetchContent()
# lands in the same code path (the root project defines pltxt2htm::pltxt2htm and
# skips the install rules when it is not the top-level project).

set(PLTXT2HTM_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
add_subdirectory("${PLTXT2HTM_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/pltxt2htm")

# Without a package config the expected version comes from the tree itself.
file(STRINGS "${PLTXT2HTM_SOURCE_DIR}/VERSION" PLTXT2HTM_VERSION)
string(REPLACE "." ";" PLTXT2HTM_VERSION_COMPONENTS "${PLTXT2HTM_VERSION}")
list(GET PLTXT2HTM_VERSION_COMPONENTS 0 PLTXT2HTM_VERSION_MAJOR)
list(GET PLTXT2HTM_VERSION_COMPONENTS 1 PLTXT2HTM_VERSION_MINOR)
list(GET PLTXT2HTM_VERSION_COMPONENTS 2 PLTXT2HTM_VERSION_PATCH)

add_executable(install_add_subdirectory ../main.cc)
target_link_libraries(install_add_subdirectory PRIVATE pltxt2htm::pltxt2htm)

target_compile_definitions(install_add_subdirectory PRIVATE
    PLTXT2HTM_CONSUMER_EXPECTED_MAJOR=${PLTXT2HTM_VERSION_MAJOR}
    PLTXT2HTM_CONSUMER_EXPECTED_MINOR=${PLTXT2HTM_VERSION_MINOR}
    PLTXT2HTM_CONSUMER_EXPECTED_PATCH=${PLTXT2HTM_VERSION_PATCH})
