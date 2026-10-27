cmake_minimum_required(VERSION 3.20)
project(pltxt2htm LANGUAGES CXX VERSION 0.27.0)
include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/pltxt2htm_contract_mode.cmake")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()

option(PLTXT2HTM_ENABLE_STACKTRACE "Enable native stacktrace" OFF)

add_library(pltxt2htm src/pltxt2htm.cc)
pltxt2htm_apply_contract_mode(pltxt2htm PRIVATE)

target_include_directories(pltxt2htm PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(pltxt2htm PUBLIC cxx_std_23)

if(PLTXT2HTM_ENABLE_STACKTRACE)
    if(NOT WIN32)
        message(FATAL_ERROR "Native stacktrace currently supports Windows only")
    endif()
    target_compile_definitions(pltxt2htm PRIVATE PLTXT2HTM_ENABLE_STACKTRACE)
    target_link_libraries(pltxt2htm PRIVATE ntdll dbghelp)
endif()

if(WIN32)
    target_link_libraries(pltxt2htm PRIVATE ntdll)
endif()

if(MINGW)
    set_target_properties(pltxt2htm PROPERTIES PREFIX "")
    target_link_libraries(pltxt2htm PRIVATE ntdll)
endif()

if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    target_compile_options(pltxt2htm PRIVATE
        /GR-
        $<$<NOT:$<BOOL:${PLTXT2HTM_ENABLE_STACKTRACE}>>:-clang:-fno-unwind-tables>
        $<$<NOT:$<BOOL:${PLTXT2HTM_ENABLE_STACKTRACE}>>:-clang:-fno-asynchronous-unwind-tables>
        -clang:-fvisibility=hidden
        -clang:-fvisibility-inlines-hidden
    )
    target_compile_options(pltxt2htm PRIVATE /EHs-c-)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(pltxt2htm PRIVATE
        -fno-rtti
        $<$<NOT:$<BOOL:${PLTXT2HTM_ENABLE_STACKTRACE}>>:-fno-unwind-tables>
        $<$<NOT:$<BOOL:${PLTXT2HTM_ENABLE_STACKTRACE}>>:-fno-asynchronous-unwind-tables>
        -fvisibility=hidden
        -fvisibility-inlines-hidden
        -fno-exceptions
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(pltxt2htm PRIVATE -fno-ident)
        if(BUILD_SHARED_LIBS)
            include(CheckIPOSupported)
            check_ipo_supported(RESULT _pltxt2htm_ipo_supported LANGUAGES CXX)
            if(_pltxt2htm_ipo_supported)
                set_property(TARGET pltxt2htm PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
            endif()
        endif()
    endif()
endif()

set_target_properties(pltxt2htm PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION}
)

install(TARGETS pltxt2htm
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING
        PATTERN "*.h"
)
