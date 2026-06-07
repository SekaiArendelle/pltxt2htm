cmake_minimum_required(VERSION 3.28)
project(pltxt2htm-cxxmodule LANGUAGES CXX VERSION 0.20.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

option(PLTXT2HTM_ENABLE_STACKTRACE "Enable C++23 <stacktrace>" OFF)

add_executable(example example.cc)

target_include_directories(example PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include/fast_io"
    "${CMAKE_CURRENT_SOURCE_DIR}/../fast_io"
)

target_compile_features(example PRIVATE cxx_std_23)

if(PLTXT2HTM_ENABLE_STACKTRACE)
    target_compile_definitions(example PRIVATE PLTXT2HTM_EXPERIMENTAL_ENABLE_STACKTRACE)
endif()

# Register C++20 module sources
target_sources(example PRIVATE
    FILE_SET CXX_MODULES
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../pltxt2htm/pltxt2htm.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/../fast_io/fast_io.cppm
)

if(WIN32 OR MINGW)
    target_link_libraries(example PRIVATE ntdll)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    find_program(HAVE_LD_LLD NAMES ld.lld)
    if(HAVE_LD_LLD)
        target_link_options(example PRIVATE -fuse-ld=lld)
    endif()
endif()
