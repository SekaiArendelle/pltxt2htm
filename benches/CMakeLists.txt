cmake_minimum_required(VERSION 3.20)
project(pltxt2htm-benches LANGUAGES CXX VERSION 0.22.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Google Benchmark from source
include(FetchContent)
FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.5
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

add_library(pltxt2htm_bench_base INTERFACE)
target_include_directories(pltxt2htm_bench_base INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(pltxt2htm_bench_base INTERFACE benchmark::benchmark benchmark::benchmark_main)
if(WIN32)
    target_link_libraries(pltxt2htm_bench_base INTERFACE ntdll)
endif()

target_compile_options(pltxt2htm_bench_base INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8 /O2 /GL>
    $<$<CXX_COMPILER_ID:GNU>:-finput-charset=utf-8 -fexec-charset=utf-8 -O3 -DNDEBUG>
    $<$<CXX_COMPILER_ID:Clang>:-finput-charset=utf-8 -fexec-charset=utf-8 -O3 -DNDEBUG -flto=thin>
)

target_compile_options(pltxt2htm_bench_base INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/GR- /D_HAS_EXCEPTIONS=0>
    $<$<CXX_COMPILER_ID:GNU>:-fno-exceptions -fno-rtti>
    $<$<CXX_COMPILER_ID:Clang>:-fno-exceptions -fno-rtti>
)

target_link_options(pltxt2htm_bench_base INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/LTCG>
    $<$<CXX_COMPILER_ID:Clang>:-flto=thin>
)

file(GLOB BENCH_SOURCES LIST_DIRECTORIES FALSE "${CMAKE_CURRENT_SOURCE_DIR}/src/bench_*.cc")

foreach(BENCH_SOURCE ${BENCH_SOURCES})
    get_filename_component(BENCH_NAME ${BENCH_SOURCE} NAME_WE)
    add_executable(${BENCH_NAME} ${BENCH_SOURCE})
    target_link_libraries(${BENCH_NAME} PRIVATE pltxt2htm_bench_base)
endforeach()
