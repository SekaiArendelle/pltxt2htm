cmake_minimum_required(VERSION 3.20)
project(pltxt2htm-benches LANGUAGES CXX VERSION 0.22.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()

add_library(pltxt2htm_bench_base INTERFACE)
target_include_directories(pltxt2htm_bench_base INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/../include")

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(pltxt2htm_bench_base INTERFACE /utf-8 /EHsc /O2 /GL /wd4626 /wd5027)
    target_link_options(pltxt2htm_bench_base INTERFACE /LTCG)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(pltxt2htm_bench_base INTERFACE
        -finput-charset=utf-8 -fexec-charset=utf-8 -fexceptions
        -O3 -DNDEBUG -flto=thin
    )
    target_link_options(pltxt2htm_bench_base INTERFACE -fuse-ld=lld -flto=thin)
endif()

add_library(bench_precompile STATIC precompile.cpp)
target_link_libraries(bench_precompile PUBLIC pltxt2htm_bench_base)
if(WIN32)
    target_link_libraries(bench_precompile PUBLIC ntdll)
endif()

file(GLOB BENCH_SOURCES LIST_DIRECTORIES FALSE "${CMAKE_CURRENT_SOURCE_DIR}/bench_*.cc")

foreach(BENCH_SOURCE ${BENCH_SOURCES})
    get_filename_component(BENCH_NAME ${BENCH_SOURCE} NAME_WE)
    add_executable(${BENCH_NAME} ${BENCH_SOURCE})
    target_link_libraries(${BENCH_NAME} PRIVATE bench_precompile)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${BENCH_NAME} PRIVATE -g)
    endif()
endforeach()
