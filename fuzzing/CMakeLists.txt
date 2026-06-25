cmake_minimum_required(VERSION 3.20)
project(pltxt2htm-fuzzing LANGUAGES CXX VERSION 0.21.0)

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
set(PLTXT2HTM_SANITIZER "" CACHE STRING "Sanitizer to enable: asan, msan, ubsan, or empty")
set_property(CACHE PLTXT2HTM_SANITIZER PROPERTY STRINGS "" asan msan ubsan)

if(WIN32 OR MINGW)
    message(FATAL_ERROR "Fuzzing is not supported on Windows")
endif()

# Require clang
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "Only clang is supported for fuzzing (CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID})")
endif()

foreach(FUZZER_SRC common_parser fixedadv_parser plrichtext_parser fixedadv_roundtrip)
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

    if(PLTXT2HTM_SANITIZER STREQUAL "asan")
        target_compile_options(${FUZZER_SRC} PRIVATE -fsanitize=address)
        target_link_options(${FUZZER_SRC} PRIVATE -fsanitize=address)
    elseif(PLTXT2HTM_SANITIZER STREQUAL "msan")
        target_compile_options(${FUZZER_SRC} PRIVATE -fsanitize=memory)
        target_link_options(${FUZZER_SRC} PRIVATE -fsanitize=memory)
    elseif(PLTXT2HTM_SANITIZER STREQUAL "ubsan")
        target_compile_options(${FUZZER_SRC} PRIVATE -fsanitize=undefined)
        target_link_options(${FUZZER_SRC} PRIVATE -fsanitize=undefined)
    elseif(PLTXT2HTM_SANITIZER)
        message(FATAL_ERROR "Unknown sanitizer: ${PLTXT2HTM_SANITIZER}. Supported: asan, msan, ubsan")
    endif()

    install(TARGETS ${FUZZER_SRC} RUNTIME DESTINATION bin)
endforeach()
