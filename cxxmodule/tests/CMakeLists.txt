cmake_minimum_required(VERSION 3.28)
project(pltxt2htm-cxxmodule-tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_executable(check_public_symbols check_public_symbols.cc)

target_include_directories(check_public_symbols PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include/fast_io"
    "${CMAKE_CURRENT_SOURCE_DIR}/../fast_io"
)

target_compile_features(check_public_symbols PRIVATE cxx_std_26)

target_sources(check_public_symbols PRIVATE
    FILE_SET CXX_MODULES
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../pltxt2htm/pltxt2htm.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/../fast_io/fast_io.cppm
)

if(WIN32 OR MINGW)
    target_link_libraries(check_public_symbols PRIVATE ntdll)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(check_public_symbols PRIVATE -freflection)
endif()
