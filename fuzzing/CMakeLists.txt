cmake_minimum_required(VERSION 3.20)
project(pltxt2htm-fuzzing LANGUAGES CXX VERSION 0.27.0)
include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/pltxt2htm_contract_mode.cmake")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()

# Default to optimized fuzzing; an AUTO contract mode still enforces here. project()
# pre-creates the cache entry (Windows toolchains set Debug), so FORCE is required.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Build type" FORCE)
endif()

option(PLTXT2HTM_ENABLE_STACKTRACE "Enable C++23 <stacktrace>" OFF)
set(PLTXT2HTM_SANITIZER "" CACHE STRING "Comma-separated sanitizers passed verbatim to -fsanitize= (e.g. address, address,undefined, memory), or empty")
set_property(CACHE PLTXT2HTM_SANITIZER PROPERTY STRINGS "" address undefined memory address,undefined)
if(NOT (PLTXT2HTM_SANITIZER STREQUAL "" OR PLTXT2HTM_SANITIZER MATCHES "^(address|undefined|memory)(,(address|undefined|memory))*$"))
    message(FATAL_ERROR "PLTXT2HTM_SANITIZER must be empty or a comma-separated list of: address, undefined, memory (got: ${PLTXT2HTM_SANITIZER})")
endif()

if(MINGW)
    message(FATAL_ERROR "Fuzzing is not supported on MinGW")
endif()

# Require clang
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "Only clang is supported for fuzzing (CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID})")
endif()

# On Windows the prebuilt clang_rt.fuzzer runtime only resolves its
# sanitizer-common dependencies (COE and dbghelp symbolization) through the
# ASan link path, so a fuzzer-only build fails to link; require ASan there.
if(WIN32 AND NOT PLTXT2HTM_SANITIZER MATCHES "address")
    message(FATAL_ERROR "Windows fuzzing requires -fsanitize=address; use the address or address,undefined sanitizer argument")
endif()

# clang_rt.fuzzer is built against the static release CRT (MT_StaticRelease), so
# the harnesses must match it; the build type does not select the CRT. Only the
# literal MultiThreaded value is accepted; anything else is rejected, not overridden.
if(WIN32)
    if(CMAKE_MSVC_RUNTIME_LIBRARY AND NOT CMAKE_MSVC_RUNTIME_LIBRARY STREQUAL "MultiThreaded")
        message(FATAL_ERROR
            "Fuzzing on Windows needs CMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded to match "
            "the prebuilt clang_rt.fuzzer runtime (MT_StaticRelease); got "
            "\"${CMAKE_MSVC_RUNTIME_LIBRARY}\".")
    endif()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    message(STATUS "Fuzzing: MSVC runtime library ${CMAKE_MSVC_RUNTIME_LIBRARY}")
endif()

# On Windows, locate the ASan dynamic runtime DLL shipped next to the clang
# resource directory so -fsanitize=address targets can copy it beside their
# executables at build time; the loader has no other way to find it on a
# machine whose PATH does not already contain the clang runtime directory.
if(WIN32 AND (NOT DEFINED PLTXT2HTM_ASAN_DYNAMIC_DLL OR NOT EXISTS "${PLTXT2HTM_ASAN_DYNAMIC_DLL}"))
    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" -print-resource-dir
        OUTPUT_VARIABLE _pltxt2htm_asan_resource_dir
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE _pltxt2htm_asan_resource_result
        ERROR_QUIET
    )
    if(_pltxt2htm_asan_resource_result EQUAL 0 AND EXISTS "${_pltxt2htm_asan_resource_dir}/lib/windows")
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(AMD64|x86_64)$")
            set(_pltxt2htm_asan_arch "x86_64")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(ARM64|aarch64)$")
            set(_pltxt2htm_asan_arch "aarch64")
        else()
            message(FATAL_ERROR "Unsupported Windows architecture for ASan: ${CMAKE_SYSTEM_PROCESSOR}")
        endif()
        # Recompute (FORCE) when the cached path no longer exists, e.g.
        # after a clang upgrade inside the same build tree.
        set(PLTXT2HTM_ASAN_DYNAMIC_DLL "${_pltxt2htm_asan_resource_dir}/lib/windows/clang_rt.asan_dynamic-${_pltxt2htm_asan_arch}.dll" CACHE INTERNAL "Path to the clang ASan dynamic runtime DLL" FORCE)
    else()
        message(FATAL_ERROR "Cannot locate the clang resource directory; required to run ASan-instrumented fuzzers on Windows")
    endif()
endif()

foreach(FUZZER_SRC common_parser fixedadv_parser plrichtext_parser fixedadv_roundtrip)
    add_executable(${FUZZER_SRC} src/${FUZZER_SRC}.cc)
    pltxt2htm_apply_contract_mode(${FUZZER_SRC} PRIVATE)
    target_include_directories(${FUZZER_SRC} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")
    target_compile_features(${FUZZER_SRC} PRIVATE cxx_std_23)

    if(PLTXT2HTM_ENABLE_STACKTRACE)
        target_compile_definitions(${FUZZER_SRC} PRIVATE PLTXT2HTM_ENABLE_STACKTRACE)
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
    )

    if(PLTXT2HTM_SANITIZER)
        target_compile_options(${FUZZER_SRC} PRIVATE "-fsanitize=${PLTXT2HTM_SANITIZER}")
        target_link_options(${FUZZER_SRC} PRIVATE "-fsanitize=${PLTXT2HTM_SANITIZER}")

        if(WIN32)
            # The prebuilt clang_rt.fuzzer static runtime carries no MSVC STL
            # container annotation directives while clang-compiled TUs do; the
            # mismatch trips LNK2038 at link time, so disable the annotations.
            target_compile_definitions(${FUZZER_SRC} PRIVATE
                _DISABLE_STRING_ANNOTATION
                _DISABLE_VECTOR_ANNOTATION
            )
            # The ASan dynamic runtime DLL must sit beside the executable for
            # the Windows loader to find it.
            add_custom_command(TARGET ${FUZZER_SRC} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${PLTXT2HTM_ASAN_DYNAMIC_DLL}"
                    "$<TARGET_FILE_DIR:${FUZZER_SRC}>"
                VERBATIM
            )
        endif()
    endif()
endforeach()
