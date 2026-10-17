cmake_minimum_required(VERSION 3.28)
project(pltxt2htm-cxxmodule LANGUAGES CXX VERSION 0.27.0)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../.." "${CMAKE_CURRENT_BINARY_DIR}/pltxt2htm-core")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

option(PLTXT2HTM_ENABLE_STACKTRACE "Enable C++23 <stacktrace>" OFF)

add_executable(example example.cc)

target_include_directories(example PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include/fast_io"
    "${CMAKE_CURRENT_SOURCE_DIR}/../fast_io"
)

target_link_libraries(example PRIVATE pltxt2htm::pltxt2htm)

if(PLTXT2HTM_ENABLE_STACKTRACE)
    target_compile_definitions(example PRIVATE PLTXT2HTM_ENABLE_STACKTRACE)
endif()

# Register C++20 module sources
target_sources(example PRIVATE
    FILE_SET CXX_MODULES
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../pltxt2htm/pltxt2htm.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/../fast_io/fast_io.cppm
)
