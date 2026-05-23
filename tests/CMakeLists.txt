cmake_minimum_required(VERSION 3.20)
project(pltxt2htm-tests LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()

option(PLTXT2HTM_ENABLE_STACKTRACE "Enable C++23 <stacktrace>" OFF)
option(PLTXT2HTM_ENABLE_COVERAGE "Enable code coverage" OFF)

set(PLTXT2HTM_SANITIZER "" CACHE STRING "Sanitizer: address, undefined, memory")

add_library(pltxt2htm_base INTERFACE)
target_include_directories(pltxt2htm_base INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/../include")

if(PLTXT2HTM_ENABLE_STACKTRACE)
    target_compile_definitions(pltxt2htm_base INTERFACE PLTXT2HTM_EXPERIMENTAL_ENABLE_STACKTRACE)
endif()

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
        target_compile_options(${TEST_NAME} PRIVATE -g)
    endif()
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${TEST_NAME} PRIVATE /Zc:u8EscapeEncoding)
    endif()
    if(HAVE_LD_LLD AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_link_options(${TEST_NAME} PRIVATE -fuse-ld=lld)
    endif()
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
