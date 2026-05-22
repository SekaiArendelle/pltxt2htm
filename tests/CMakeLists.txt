cmake_minimum_required(VERSION 3.28)
project(pltxt2htm-tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(PLTXT2HTM_ENABLE_STACKTRACE "Enable C++23 <stacktrace>" OFF)
option(PLTXT2HTM_ENABLE_COVERAGE "Enable code coverage" OFF)

set(PLTXT2HTM_SANITIZER "" CACHE STRING "Sanitizer: address, undefined, memory")

if(PLTXT2HTM_ENABLE_STACKTRACE)
    add_compile_definitions(PLTXT2HTM_EXPERIMENTAL_ENABLE_STACKTRACE)
endif()

if(PLTXT2HTM_SANITIZER)
    if(NOT PLTXT2HTM_SANITIZER MATCHES "^(address|undefined|memory)$")
        message(FATAL_ERROR "PLTXT2HTM_SANITIZER must be one of: address, undefined, memory (got: ${PLTXT2HTM_SANITIZER})")
    endif()
    add_compile_options(-fsanitize=${PLTXT2HTM_SANITIZER})
    add_link_options(-fsanitize=${PLTXT2HTM_SANITIZER})
endif()

if(PLTXT2HTM_ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "lcov only support gcc, please reconfigure with -DCMAKE_CXX_COMPILER=g++")
    endif()
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/utf-8)
endif()

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include")

find_program(HAVE_LD_LLD NAMES ld.lld)

add_library(precompile STATIC precompile.cpp)
target_include_directories(precompile PUBLIC "${INCLUDE_DIR}")
if(WIN32)
    target_link_libraries(precompile PUBLIC ntdll)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(precompile PRIVATE
        -g -Wall -Wextra -Wshadow
        -Werror=return-type -Werror=switch -Werror=implicit-fallthrough
    )
endif()

file(GLOB TEST_SOURCES LIST_DIRECTORIES FALSE "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE precompile)
    target_include_directories(${TEST_NAME} PRIVATE "${INCLUDE_DIR}")
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

enable_testing()
