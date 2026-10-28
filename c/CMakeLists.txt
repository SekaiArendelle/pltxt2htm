cmake_minimum_required(VERSION 3.20)
project(pltxt2htm LANGUAGES CXX VERSION 0.27.0)
include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/pltxt2htm_contract_mode.cmake")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()

add_library(pltxt2htm src/pltxt2htm.cc)
pltxt2htm_apply_contract_mode(pltxt2htm PRIVATE)

target_include_directories(pltxt2htm PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(pltxt2htm PUBLIC cxx_std_23)

if(WIN32)
    # Panic diagnostics resolve symbols through DbgHelp and capture through ntdll.
    target_link_libraries(pltxt2htm PRIVATE ntdll dbghelp)
endif()

if(MINGW)
    set_target_properties(pltxt2htm PROPERTIES PREFIX "")
endif()

# Only enforce builds print panic diagnostics, so the other modes can drop the unwind
# tables that native stack capture needs.
pltxt2htm_resolve_contract_mode(_pltxt2htm_contract_mode)

if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    target_compile_options(pltxt2htm PRIVATE
        /GR-
        -clang:-fvisibility=hidden
        -clang:-fvisibility-inlines-hidden
    )
    if(NOT _pltxt2htm_contract_mode STREQUAL "ENFORCE")
        target_compile_options(pltxt2htm PRIVATE
            -clang:-fno-unwind-tables
            -clang:-fno-asynchronous-unwind-tables
        )
    endif()
    target_compile_options(pltxt2htm PRIVATE /EHs-c-)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(pltxt2htm PRIVATE
        -fno-rtti
        -fvisibility=hidden
        -fvisibility-inlines-hidden
        -fno-exceptions
    )
    if(NOT _pltxt2htm_contract_mode STREQUAL "ENFORCE")
        target_compile_options(pltxt2htm PRIVATE
            -fno-unwind-tables
            -fno-asynchronous-unwind-tables
        )
    endif()
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
