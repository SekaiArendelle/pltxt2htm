cmake_minimum_required(VERSION 3.20)
project(pltxt2htm-examples LANGUAGES CXX VERSION 0.21.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

file(GLOB EXAMPLE_SOURCES LIST_DIRECTORIES FALSE "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})
    target_include_directories(${EXAMPLE_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")
    target_compile_features(${EXAMPLE_NAME} PRIVATE cxx_std_23)

    if(WIN32 OR MINGW)
        target_link_libraries(${EXAMPLE_NAME} PRIVATE ntdll)
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        find_program(HAVE_LD_LLD NAMES ld.lld)
        if(HAVE_LD_LLD)
            target_link_options(${EXAMPLE_NAME} PRIVATE -fuse-ld=lld)
        endif()
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${EXAMPLE_NAME} PRIVATE /Zc:u8EscapeEncoding)
    endif()
endforeach()
