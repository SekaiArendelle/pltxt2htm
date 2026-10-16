# Entry point for consuming pltxt2htm from CMake.
#
# pltxt2htm is header-only, so this project only defines the interface target that
# carries the headers, the C++ standard and the platform flags the headers need,
# plus the install and export rules behind find_package(pltxt2htm CONFIG).
#
# Regular development components can be enabled in one build tree. Toolchain-specific
# components such as wasm and C++ modules keep their own CMake entry points.

cmake_minimum_required(VERSION 3.25)

project(pltxt2htm LANGUAGES NONE VERSION 0.27.0)

# GNUInstallDirs cannot infer a multiarch libdir without a compiler. This
# package is architecture-independent, so use lib unless the caller chose one.
if(PROJECT_IS_TOP_LEVEL AND NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "Library installation directory")
endif()

option(PLTXT2HTM_BUILD_TESTS "Build the C++ tests" OFF)
option(PLTXT2HTM_BUILD_EXAMPLES "Build the C++ examples" OFF)
option(PLTXT2HTM_BUILD_CMD "Build the command-line tool" OFF)
option(PLTXT2HTM_BUILD_C "Build the C binding" OFF)
option(PLTXT2HTM_BUILD_PY "Build the Python binding" OFF)
option(PLTXT2HTM_BUILD_BENCHES "Build benchmarks" OFF)
option(PLTXT2HTM_BUILD_FUZZING "Build fuzz targets" OFF)

if(PLTXT2HTM_BUILD_TESTS OR PLTXT2HTM_BUILD_EXAMPLES OR PLTXT2HTM_BUILD_CMD
        OR PLTXT2HTM_BUILD_C OR PLTXT2HTM_BUILD_PY OR PLTXT2HTM_BUILD_BENCHES
        OR PLTXT2HTM_BUILD_FUZZING)
    enable_language(CXX)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# With add_subdirectory()/FetchContent() the parent project owns the install rules,
# so only a standalone build generates them.
option(PLTXT2HTM_INSTALL "Generate the install and export rules of pltxt2htm" ${PROJECT_IS_TOP_LEVEL})

# fast_io is vendored next to pltxt2htm and included as <fast_io/...>, so both
# header trees share one include directory in the source and installed targets.
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/pltxt2htmSourceTarget.cmake")

if(PLTXT2HTM_INSTALL)
    install(DIRECTORY "${PLTXT2HTM_HEADER_DIR}/pltxt2htm" "${PLTXT2HTM_HEADER_DIR}/fast_io"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/licenses/pltxt2htm")

    # Header-only: the target has no artifacts, it only has to be part of the export
    # set so that pltxt2htmTargets.cmake defines pltxt2htm::pltxt2htm.
    install(TARGETS pltxt2htm_headers EXPORT pltxt2htmTargets)

    install(EXPORT pltxt2htmTargets
        NAMESPACE pltxt2htm::
        FILE pltxt2htmTargets.cmake
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/pltxt2htm")

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/pltxt2htmConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/pltxt2htmConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/pltxt2htm")

    # SameMinorVersion states the versioning intent of this library: releases within
    # one 0.x minor may break the API, patch releases may not. ARCH_INDEPENDENT
    # because a header-only package does not care about the pointer size it was
    # installed from.
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/pltxt2htmConfigVersion.cmake"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY SameMinorVersion
        ARCH_INDEPENDENT)

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/pltxt2htmConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/pltxt2htmConfigVersion.cmake"
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/pltxt2htmTargetOptions.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/pltxt2htm")
endif()

if(PLTXT2HTM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
if(PLTXT2HTM_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if(PLTXT2HTM_BUILD_CMD)
    add_subdirectory(cmd)
endif()
if(PLTXT2HTM_BUILD_C)
    add_subdirectory(c)
endif()
if(PLTXT2HTM_BUILD_PY)
    add_subdirectory(py)
endif()
if(PLTXT2HTM_BUILD_BENCHES)
    add_subdirectory(benches)
endif()
if(PLTXT2HTM_BUILD_FUZZING)
    add_subdirectory(fuzzing)
endif()
