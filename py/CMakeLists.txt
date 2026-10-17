cmake_minimum_required(VERSION 3.25)
project(pltxt2htm LANGUAGES CXX VERSION 0.27.0)
if(NOT TARGET pltxt2htm::pltxt2htm)
    include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/pltxt2htmSourceTarget.cmake")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PLTXT2HTM_ENABLE_STACKTRACE OFF CACHE BOOL "Enable C++23 <stacktrace>")

# Python include dir and extension suffix — can be passed externally (e.g. cross-compile in docker),
# or auto-detected from the python on PATH
set(PYTHON_INCLUDE_DIR "" CACHE PATH "Python include dir (auto-detected if empty)")
set(PYTHON_EXT_SUFFIX "" CACHE STRING "Python extension suffix (auto-detected if empty)")
set(PYTHON_LIB "" CACHE STRING "Python import library name (e.g. python312; auto-detected if empty)")
set(PYTHON_PREFIX "" CACHE PATH "Python prefix (auto-detected if empty)")

if(PYTHON_INCLUDE_DIR AND PYTHON_EXT_SUFFIX)
    message(STATUS "Using user-provided Python: INC=${PYTHON_INCLUDE_DIR} SUFFIX=${PYTHON_EXT_SUFFIX}")
    if(NOT PYTHON_PREFIX)
        set(PYTHON_PREFIX "${PYTHON_INCLUDE_DIR}/..")
    endif()
else()
    find_program(PYTHON_EXECUTABLE NAMES python3 python)
    if(NOT PYTHON_EXECUTABLE)
        message(FATAL_ERROR "python3/python not found on PATH")
    endif()
    message(STATUS "Using Python: ${PYTHON_EXECUTABLE}")
    execute_process(
        COMMAND "${PYTHON_EXECUTABLE}" -c "import sysconfig; print(sysconfig.get_config_var('INCLUDEPY'), end='')"
        OUTPUT_VARIABLE PYTHON_INCLUDE_DIR
        RESULT_VARIABLE _PYTHON_INCLUDE_DIR_RESULT
    )
    if(NOT _PYTHON_INCLUDE_DIR_RESULT EQUAL 0 OR NOT PYTHON_INCLUDE_DIR)
        message(FATAL_ERROR "Failed to detect Python include directory via '${PYTHON_EXECUTABLE}'")
    endif()
    execute_process(
        COMMAND "${PYTHON_EXECUTABLE}" -c "import sysconfig; print(sysconfig.get_config_var('EXT_SUFFIX'), end='')"
        OUTPUT_VARIABLE PYTHON_EXT_SUFFIX
        RESULT_VARIABLE _PYTHON_EXT_SUFFIX_RESULT
    )
    if(NOT _PYTHON_EXT_SUFFIX_RESULT EQUAL 0 OR NOT PYTHON_EXT_SUFFIX)
        message(FATAL_ERROR "Failed to detect Python extension suffix via '${PYTHON_EXECUTABLE}'")
    endif()
    execute_process(
        COMMAND "${PYTHON_EXECUTABLE}" -c "import sysconfig; print(sysconfig.get_config_var('prefix'), end='')"
        OUTPUT_VARIABLE PYTHON_PREFIX
        RESULT_VARIABLE _PYTHON_PREFIX_RESULT
    )
    if(NOT _PYTHON_PREFIX_RESULT EQUAL 0 OR NOT PYTHON_PREFIX)
        message(FATAL_ERROR "Failed to detect Python prefix via '${PYTHON_EXECUTABLE}'")
    endif()
    if(NOT PYTHON_LIB)
        execute_process(
            COMMAND "${PYTHON_EXECUTABLE}" -c "import sys; print(f'python{sys.version_info.major}{sys.version_info.minor}', end='')"
            OUTPUT_VARIABLE PYTHON_LIB
            RESULT_VARIABLE _PYTHON_LIB_RESULT
        )
        if(NOT _PYTHON_LIB_RESULT EQUAL 0 OR NOT PYTHON_LIB)
            message(FATAL_ERROR "Failed to detect Python library name via '${PYTHON_EXECUTABLE}'")
        endif()
    endif()
    message(STATUS "Python: INC=${PYTHON_INCLUDE_DIR} SUFFIX=${PYTHON_EXT_SUFFIX} PREFIX=${PYTHON_PREFIX} LIB=${PYTHON_LIB}")
endif()

add_library(pltxt2htm_py MODULE pltxt2htm.cc)

set_target_properties(pltxt2htm_py PROPERTIES
    PREFIX ""
    OUTPUT_NAME pltxt2htm
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    SUFFIX "${PYTHON_EXT_SUFFIX}"
)

target_include_directories(pltxt2htm_py PRIVATE
    "${PYTHON_INCLUDE_DIR}"
)

target_link_libraries(pltxt2htm_py PRIVATE pltxt2htm::pltxt2htm)

if(PLTXT2HTM_ENABLE_STACKTRACE)
    target_compile_definitions(pltxt2htm_py PRIVATE PLTXT2HTM_ENABLE_STACKTRACE)
endif()

# Compile/link flags for GCC/Clang-style toolchains
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(pltxt2htm_py PRIVATE
        -fno-rtti
        -fno-unwind-tables
        -fno-asynchronous-unwind-tables
        -fvisibility=hidden
        -fvisibility-inlines-hidden
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(pltxt2htm_py PRIVATE -fno-ident)
    endif()
endif()

# macOS: allow undefined symbols (Python symbols resolved at runtime)
if(APPLE)
    target_link_options(pltxt2htm_py PRIVATE -undefined dynamic_lookup)
endif()

# Windows / MinGW
if(WIN32)
    if(PYTHON_PREFIX)
        target_link_directories(pltxt2htm_py PRIVATE "${PYTHON_PREFIX}/libs")
    endif()
    if(PYTHON_LIB)
        target_link_libraries(pltxt2htm_py PRIVATE "${PYTHON_LIB}")
    endif()
endif()
