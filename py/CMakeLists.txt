cmake_minimum_required(VERSION 3.20)
project(pltxt2htm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Python runtime info passed via CLI (not via find_package)
set(PYTHON_INCLUDE_DIR "" CACHE PATH "Path to Python include directory containing Python.h (required)")
set(PYTHON_EXT_SUFFIX "" CACHE STRING "Python extension suffix, e.g. .cpython-312-x86_64-linux-gnu.so (required)")
set(PLTXT2HTM_ENABLE_STACKTRACE OFF CACHE BOOL "Enable C++23 <stacktrace>")

if(NOT PYTHON_INCLUDE_DIR)
    message(FATAL_ERROR "PYTHON_INCLUDE_DIR is required (e.g. -DPYTHON_INCLUDE_DIR=/usr/include/python3.12)")
endif()
if(NOT PYTHON_EXT_SUFFIX)
    message(FATAL_ERROR "PYTHON_EXT_SUFFIX is required (e.g. -DPYTHON_EXT_SUFFIX=.cpython-312-x86_64-linux-gnu.so)")
endif()

add_library(pltxt2htm MODULE pltxt2htm.cc)

set_target_properties(pltxt2htm PROPERTIES
    PREFIX ""
    SUFFIX "${PYTHON_EXT_SUFFIX}"
)

target_include_directories(pltxt2htm PRIVATE
    "${PYTHON_INCLUDE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

target_compile_features(pltxt2htm PRIVATE cxx_std_23)

if(PLTXT2HTM_ENABLE_STACKTRACE)
    target_compile_definitions(pltxt2htm PRIVATE PLTXT2HTM_EXPERIMENTAL_ENABLE_STACKTRACE)
endif()

# Compile/link flags matching xmake.lua for GCC/Clang-style toolchains
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(pltxt2htm PRIVATE
        -fno-rtti
        -fno-unwind-tables
        -fno-asynchronous-unwind-tables
        -fvisibility=hidden
        -fvisibility-inlines-hidden
    )

    # -fuse-ld=lld if available (matching xmake.lua behavior)
    find_program(LLD_LD ld.lld)
    if(LLD_LD)
        target_link_options(pltxt2htm PRIVATE -fuse-ld=lld)
    endif()

    # Release-specific flags
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(pltxt2htm PRIVATE -fno-ident)
        include(CheckCXXCompilerFlag)
        check_cxx_compiler_flag("-flto" LTO_SUPPORTED)
        if(LTO_SUPPORTED)
            target_compile_options(pltxt2htm PRIVATE -flto)
            target_link_options(pltxt2htm PRIVATE -flto)
        endif()
    endif()
endif()

# macOS: allow undefined symbols (Python symbols resolved at runtime)
if(APPLE)
    target_link_options(pltxt2htm PRIVATE -undefined dynamic_lookup)
endif()

# Windows / MinGW
if(WIN32)
    target_link_libraries(pltxt2htm PRIVATE ntdll)
endif()
