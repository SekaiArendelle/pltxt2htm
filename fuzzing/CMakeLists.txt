cmake_minimum_required(VERSION 3.20)
project(pltxt2htm-fuzzing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()

# Force debug mode for fuzzing
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

option(PLTXT2HTM_ENABLE_STACKTRACE "Enable C++23 <stacktrace>" OFF)

if(WIN32 OR MINGW)
    message(FATAL_ERROR "Fuzzing is not supported on Windows")
endif()

# Require clang
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "Only clang is supported for fuzzing (CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID})")
endif()

foreach(FUZZER_SRC common_parser fixedadv_parser plrichtext_parser)
    add_executable(${FUZZER_SRC} src/${FUZZER_SRC}.cc)
    target_include_directories(${FUZZER_SRC} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")
    target_compile_features(${FUZZER_SRC} PRIVATE cxx_std_23)

    if(PLTXT2HTM_ENABLE_STACKTRACE)
        target_compile_definitions(${FUZZER_SRC} PRIVATE PLTXT2HTM_EXPERIMENTAL_ENABLE_STACKTRACE)
    endif()

    set_target_properties(${FUZZER_SRC} PROPERTIES
        CXX_EXTENSIONS OFF
        EXCEPTIONS OFF
    )

    target_compile_options(${FUZZER_SRC} PRIVATE
        -fno-rtti
        -fno-omit-frame-pointer
        -fsanitize=fuzzer
    )
    target_link_options(${FUZZER_SRC} PRIVATE
        -fsanitize=fuzzer
        -fuse-ld=lld
    )

    install(TARGETS ${FUZZER_SRC} RUNTIME DESTINATION bin)
endforeach()
