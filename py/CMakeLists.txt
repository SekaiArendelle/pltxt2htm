cmake_minimum_required(VERSION 3.20)
project(pltxt2htm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PLTXT2HTM_ENABLE_STACKTRACE OFF CACHE BOOL "Enable C++23 <stacktrace>")

# Python include dir and extension suffix — can be passed externally (e.g. cross-compile in docker),
# or auto-detected from the python on PATH
set(PYTHON_INCLUDE_DIR "" CACHE PATH "Python include dir (auto-detected if empty)")
set(PYTHON_EXT_SUFFIX "" CACHE STRING "Python extension suffix (auto-detected if empty)")

if(PYTHON_INCLUDE_DIR AND PYTHON_EXT_SUFFIX)
    message(STATUS "Using user-provided Python: INC=${PYTHON_INCLUDE_DIR} SUFFIX=${PYTHON_EXT_SUFFIX}")
else()
    find_program(PYTHON_EXECUTABLE python3 python)
    if(NOT PYTHON_EXECUTABLE)
        message(FATAL_ERROR "python3/python not found on PATH")
    endif()
    message(STATUS "Using Python: ${PYTHON_EXECUTABLE}")
    execute_process(
        COMMAND "${PYTHON_EXECUTABLE}" -c "import sysconfig; print(sysconfig.get_config_var('INCLUDEPY'), end='')"
        OUTPUT_VARIABLE PYTHON_INCLUDE_DIR
    )
    execute_process(
        COMMAND "${PYTHON_EXECUTABLE}" -c "import sysconfig; print(sysconfig.get_config_var('EXT_SUFFIX'), end='')"
        OUTPUT_VARIABLE PYTHON_EXT_SUFFIX
    )
    message(STATUS "Python: INC=${PYTHON_INCLUDE_DIR} SUFFIX=${PYTHON_EXT_SUFFIX}")
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

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(pltxt2htm PRIVATE -fno-ident)
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
