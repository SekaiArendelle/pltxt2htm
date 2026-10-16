cmake_minimum_required(VERSION 3.25)
project(pltxt2htm LANGUAGES CXX VERSION 0.27.0)
if(NOT TARGET pltxt2htm::pltxt2htm)
    include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/pltxt2htmSourceTarget.cmake")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()

option(PLTXT2HTM_ENABLE_STACKTRACE "Enable C++23 <stacktrace>" OFF)

add_library(pltxt2htm_c src/pltxt2htm.cc)
set_target_properties(pltxt2htm_c PROPERTIES
    OUTPUT_NAME pltxt2htm
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

target_include_directories(pltxt2htm_c INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(pltxt2htm_c PRIVATE pltxt2htm::pltxt2htm)

if(PLTXT2HTM_ENABLE_STACKTRACE)
    target_compile_definitions(pltxt2htm_c PRIVATE PLTXT2HTM_ENABLE_STACKTRACE)
endif()

if(MINGW)
    set_target_properties(pltxt2htm_c PROPERTIES PREFIX "")
endif()

if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    target_compile_options(pltxt2htm_c PRIVATE
        /GR-
        -clang:-fno-unwind-tables
        -clang:-fno-asynchronous-unwind-tables
        -clang:-fvisibility=hidden
        -clang:-fvisibility-inlines-hidden
    )
    target_compile_options(pltxt2htm_c PRIVATE /EHs-c-)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(pltxt2htm_c PRIVATE
        -fno-rtti
        -fno-unwind-tables
        -fno-asynchronous-unwind-tables
        -fvisibility=hidden
        -fvisibility-inlines-hidden
        -fno-exceptions
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(pltxt2htm_c PRIVATE -fno-ident)
        if(BUILD_SHARED_LIBS)
            include(CheckIPOSupported)
            check_ipo_supported(RESULT _pltxt2htm_ipo_supported LANGUAGES CXX)
            if(_pltxt2htm_ipo_supported)
                set_property(TARGET pltxt2htm_c PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
            endif()
        endif()
    endif()
endif()

set_target_properties(pltxt2htm_c PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION}
)

install(TARGETS pltxt2htm_c
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING
        PATTERN "*.h"
)
