cmake_minimum_required(VERSION 3.25)
project(pltxt2htm-tests LANGUAGES CXX VERSION 0.26.0)

enable_testing()

if(NOT DEFINED CMAKE_CXX_STANDARD)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16)
        set(CMAKE_CXX_STANDARD 26)
    else()
        set(CMAKE_CXX_STANDARD 23)
    endif()
    set(_pltxt2htm_cxx_standard_source "auto-detected")
else()
    set(_pltxt2htm_cxx_standard_source "explicit")
endif()

if(NOT CMAKE_CXX_STANDARD MATCHES "^(23|26)$")
    message(FATAL_ERROR "CMAKE_CXX_STANDARD must be one of: 23, 26 (got: ${CMAKE_CXX_STANDARD})")
endif()

message(STATUS "CMAKE_CXX_STANDARD: C++${CMAKE_CXX_STANDARD} (${_pltxt2htm_cxx_standard_source})")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()

set(PLTXT2HTM_ENABLE_STACKTRACE "AUTO" CACHE STRING
    "Enable C++23 <stacktrace>: AUTO (detect), ON (force), OFF (disable)")
set_property(CACHE PLTXT2HTM_ENABLE_STACKTRACE PROPERTY STRINGS AUTO ON OFF)
option(PLTXT2HTM_ENABLE_COVERAGE "Enable code coverage" OFF)

set(PLTXT2HTM_SANITIZER "" CACHE STRING "Sanitizer: address, undefined, memory")

# libstdc++ ships <stacktrace> (experimental), but it requires linking against a
# separate libstdc++exp library, so a mere __has_include/feature check is not enough.
# Probe compile+link+run a small <stacktrace> program and only enable the feature
# (defining PLTXT2HTM_ENABLE_STACKTRACE) when the whole chain actually works.
include(CheckCXXSourceCompiles)
include(CheckCXXSourceRuns)

set(_pltxt2htm_stacktrace_probe [[
#include <stacktrace>
int main() {
    ::std::stacktrace st = ::std::stacktrace::current();
    return 0;
}
]])

macro(pltxt2htm_probe_stacktrace result_var)
    set(CMAKE_REQUIRED_LIBRARIES "${ARGN}")
    if(CMAKE_CROSSCOMPILING)
        check_cxx_source_compiles("${_pltxt2htm_stacktrace_probe}" ${result_var})
    else()
        check_cxx_source_runs("${_pltxt2htm_stacktrace_probe}" ${result_var})
    endif()
endmacro()

set(_pltxt2htm_saved_required_libs "${CMAKE_REQUIRED_LIBRARIES}")

set(PLTXT2HTM_USE_STACKTRACE FALSE)
set(PLTXT2HTM_STACKTRACE_LINK_LIB "")

if(PLTXT2HTM_ENABLE_STACKTRACE STREQUAL "ON")
    set(PLTXT2HTM_USE_STACKTRACE TRUE)
    message(STATUS "PLTXT2HTM_ENABLE_STACKTRACE: enabled (forced)")
elseif(PLTXT2HTM_ENABLE_STACKTRACE STREQUAL "AUTO")
    pltxt2htm_probe_stacktrace(_pltxt2htm_stacktrace_ok)
    if(_pltxt2htm_stacktrace_ok)
        set(PLTXT2HTM_USE_STACKTRACE TRUE)
        message(STATUS "PLTXT2HTM_ENABLE_STACKTRACE: enabled (auto-detected)")
    else()
        pltxt2htm_probe_stacktrace(_pltxt2htm_stacktrace_ok_exp "stdc++exp")
        if(_pltxt2htm_stacktrace_ok_exp)
            set(PLTXT2HTM_USE_STACKTRACE TRUE)
            set(PLTXT2HTM_STACKTRACE_LINK_LIB "stdc++exp")
            message(STATUS "PLTXT2HTM_ENABLE_STACKTRACE: enabled (auto-detected, linking stdc++exp)")
        else()
            message(STATUS "PLTXT2HTM_ENABLE_STACKTRACE: disabled (C++23 <stacktrace> unavailable)")
        endif()
    endif()
else()
    message(STATUS "PLTXT2HTM_ENABLE_STACKTRACE: disabled (forced)")
endif()

set(CMAKE_REQUIRED_LIBRARIES "${_pltxt2htm_saved_required_libs}")

add_library(pltxt2htm_base INTERFACE)
target_include_directories(pltxt2htm_base INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/../include")

if(PLTXT2HTM_USE_STACKTRACE)
    target_compile_definitions(pltxt2htm_base INTERFACE PLTXT2HTM_ENABLE_STACKTRACE)
    if(PLTXT2HTM_STACKTRACE_LINK_LIB)
        target_link_libraries(pltxt2htm_base INTERFACE "${PLTXT2HTM_STACKTRACE_LINK_LIB}")
    endif()
endif()

target_compile_definitions(pltxt2htm_base INTERFACE PLTXT2HTM_CONTEXT_BRANCH_INSTRUMENT)

if(PLTXT2HTM_SANITIZER)
    if(NOT PLTXT2HTM_SANITIZER MATCHES "^(address|undefined|memory)$")
        message(FATAL_ERROR "PLTXT2HTM_SANITIZER must be one of: address, undefined, memory (got: ${PLTXT2HTM_SANITIZER})")
    endif()
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(FATAL_ERROR "PLTXT2HTM_SANITIZER is not supported on MSVC (use clang or gcc instead)")
    endif()
    target_compile_options(pltxt2htm_base INTERFACE -fsanitize=${PLTXT2HTM_SANITIZER})
    target_link_options(pltxt2htm_base INTERFACE -fsanitize=${PLTXT2HTM_SANITIZER})
endif()

if(PLTXT2HTM_ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "lcov only support gcc, please reconfigure with -DCMAKE_CXX_COMPILER=g++")
    endif()
    target_compile_options(pltxt2htm_base INTERFACE --coverage)
    target_link_options(pltxt2htm_base INTERFACE --coverage)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(pltxt2htm_base INTERFACE /utf-8 /EHsc)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(pltxt2htm_base INTERFACE -finput-charset=utf-8 -fexec-charset=utf-8 -fexceptions)
endif()

find_program(HAVE_LD_LLD NAMES ld.lld)

add_library(precompile STATIC precompile.cpp)
target_link_libraries(precompile PUBLIC pltxt2htm_base)
if(WIN32)
    target_link_libraries(precompile PUBLIC ntdll)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(precompile PRIVATE
        -g -Wall -Wextra -Wshadow
        -Werror=return-type -Werror=switch -Werror=implicit-fallthrough
    )
endif()

file(GLOB TEST_SOURCES LIST_DIRECTORIES FALSE "${CMAKE_CURRENT_SOURCE_DIR}/test_*.cc")

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE precompile)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${TEST_NAME} PRIVATE -g -Werror=switch)
    endif()
    if(HAVE_LD_LLD AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_link_options(${TEST_NAME} PRIVATE -fuse-ld=lld)
    endif()
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
