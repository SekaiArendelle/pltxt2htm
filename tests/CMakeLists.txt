cmake_minimum_required(VERSION 3.25)
project(pltxt2htm-tests LANGUAGES CXX VERSION 0.26.0)
include("${CMAKE_CURRENT_LIST_DIR}/../cmake/pltxt2htm_stacktrace.cmake")

enable_testing()

# Use the vendored doctest without network access. Mark it SYSTEM to suppress third-party warnings.
add_library(pltxt2htm_doctest INTERFACE)
add_library(doctest::doctest ALIAS pltxt2htm_doctest)
target_include_directories(pltxt2htm_doctest SYSTEM INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor")

if(NOT DEFINED CMAKE_CXX_STANDARD)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16)
        set(CMAKE_CXX_STANDARD 26)
    else()
        set(CMAKE_CXX_STANDARD 23)
    endif()
    set(_pltxt2htm_cxx_standard_source "auto-detected")
else()
    set(_pltxt2htm_cxx_standard_source "explicit")
endif()

if(NOT CMAKE_CXX_STANDARD MATCHES "^(23|26)$")
    message(FATAL_ERROR "CMAKE_CXX_STANDARD must be one of: 23, 26 (got: ${CMAKE_CXX_STANDARD})")
endif()

message(STATUS "CMAKE_CXX_STANDARD: C++${CMAKE_CXX_STANDARD} (${_pltxt2htm_cxx_standard_source})")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()

set(PLTXT2HTM_ENABLE_STACKTRACE "AUTO" CACHE STRING
    "Enable native stacktrace: AUTO (detect), ON (require), OFF (disable)")
set_property(CACHE PLTXT2HTM_ENABLE_STACKTRACE PROPERTY STRINGS AUTO ON OFF)
option(PLTXT2HTM_ENABLE_COVERAGE "Enable code coverage" OFF)
set(PLTXT2HTM_SANITIZER "" CACHE STRING "Sanitizer: address, undefined, memory")

if(NOT PLTXT2HTM_ENABLE_STACKTRACE MATCHES "^(AUTO|ON|OFF)$")
    message(FATAL_ERROR "PLTXT2HTM_ENABLE_STACKTRACE must be AUTO, ON, or OFF")
endif()

include(CheckCXXSourceCompiles)
set(PLTXT2HTM_USE_STACKTRACE FALSE)
if(NOT PLTXT2HTM_ENABLE_STACKTRACE STREQUAL "OFF")
    set(_pltxt2htm_saved_required_libs "${CMAKE_REQUIRED_LIBRARIES}")
    set(_pltxt2htm_saved_required_includes "${CMAKE_REQUIRED_INCLUDES}")
    set(_pltxt2htm_saved_required_definitions "${CMAKE_REQUIRED_DEFINITIONS}")
    set(CMAKE_REQUIRED_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/../include")
    set(CMAKE_REQUIRED_DEFINITIONS -DPLTXT2HTM_ENABLE_STACKTRACE)
    if(WIN32)
        set(CMAKE_REQUIRED_LIBRARIES ntdll)
        set(_pltxt2htm_native_available TRUE)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        check_cxx_source_compiles("#include <execinfo.h>
            int main() { void* frames[1]; return backtrace(frames, 1) < 0; }"
            _pltxt2htm_execinfo_available)
        set(_pltxt2htm_native_available ${_pltxt2htm_execinfo_available})
    else()
        set(_pltxt2htm_native_available FALSE)
    endif()
    if(_pltxt2htm_native_available)
        check_cxx_source_compiles("#include <pltxt2htm/details/stacktrace/capture.hh>
            #include <pltxt2htm/details/stacktrace/resolve.hh>
            int main() { void* frames[1];
                (void)pltxt2htm::details::stacktrace::resolve(nullptr);
                return pltxt2htm::details::stacktrace::capture(frames).size == 0; }"
            _pltxt2htm_native_stacktrace_ok)
        set(PLTXT2HTM_USE_STACKTRACE ${_pltxt2htm_native_stacktrace_ok})
    endif()
    set(CMAKE_REQUIRED_LIBRARIES "${_pltxt2htm_saved_required_libs}")
    set(CMAKE_REQUIRED_INCLUDES "${_pltxt2htm_saved_required_includes}")
    set(CMAKE_REQUIRED_DEFINITIONS "${_pltxt2htm_saved_required_definitions}")
    if(NOT PLTXT2HTM_USE_STACKTRACE AND PLTXT2HTM_ENABLE_STACKTRACE STREQUAL "ON")
        message(FATAL_ERROR "Native stacktrace backend unavailable")
    endif()
endif()
message(STATUS "PLTXT2HTM_ENABLE_STACKTRACE: ${PLTXT2HTM_USE_STACKTRACE}")

add_library(pltxt2htm_base INTERFACE)
# Include tests/ so nested tests can use "precompile.hh" directly.
target_include_directories(pltxt2htm_base INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    "${CMAKE_CURRENT_SOURCE_DIR}")

if(PLTXT2HTM_USE_STACKTRACE)
    pltxt2htm_enable_stacktrace(pltxt2htm_base INTERFACE)
endif()

target_compile_definitions(pltxt2htm_base INTERFACE PLTXT2HTM_ENABLE_CONTEXT_BRANCH_CHECK)

if(PLTXT2HTM_SANITIZER)
    if(NOT PLTXT2HTM_SANITIZER MATCHES "^(address|undefined|memory)$")
        message(FATAL_ERROR "PLTXT2HTM_SANITIZER must be one of: address, undefined, memory (got: ${PLTXT2HTM_SANITIZER})")
    endif()
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(FATAL_ERROR "PLTXT2HTM_SANITIZER is not supported on MSVC (use clang or gcc instead)")
    endif()
    target_compile_options(pltxt2htm_base INTERFACE -fsanitize=${PLTXT2HTM_SANITIZER})
    target_link_options(pltxt2htm_base INTERFACE -fsanitize=${PLTXT2HTM_SANITIZER})
endif()

if(PLTXT2HTM_ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "lcov only support gcc, please reconfigure with -DCMAKE_CXX_COMPILER=g++")
    endif()
    target_compile_options(pltxt2htm_base INTERFACE --coverage)
    target_link_options(pltxt2htm_base INTERFACE --coverage)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(pltxt2htm_base INTERFACE /utf-8 /EHsc)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(pltxt2htm_base INTERFACE -finput-charset=utf-8 -fexec-charset=utf-8 -fexceptions)
endif()

find_program(HAVE_LD_LLD NAMES ld.lld)

add_library(precompile STATIC precompile.cpp)
target_link_libraries(precompile PUBLIC pltxt2htm_base)
if(WIN32)
    target_link_libraries(precompile PUBLIC ntdll)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(precompile PRIVATE
        -g -Wall -Wextra -Wshadow
        -Werror=return-type -Werror=switch -Werror=implicit-fallthrough
    )
endif()

# Each .cc under src/ becomes a test executable. Header-only cases are collected below.
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS LIST_DIRECTORIES FALSE
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")

# Generate doctest_suites includes from case headers; rewrite only when the list changes.
# Keep helper headers outside these globs (for example, under syntax/detail/).
set(_pltxt2htm_case_globs
    "${CMAKE_CURRENT_SOURCE_DIR}/src/syntax/*/*.hh"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/parser/*.hh"
)
file(GLOB CASES_HEADERS CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src"
     ${_pltxt2htm_case_globs})
list(SORT CASES_HEADERS)
set(_pltxt2htm_cases "// Generated by tests/CMakeLists.txt, do not edit.\n")
foreach(_pltxt2htm_case IN LISTS CASES_HEADERS)
    string(APPEND _pltxt2htm_cases "#include \"${_pltxt2htm_case}\"\n")
endforeach()
set(_pltxt2htm_cases_inc "${CMAKE_CURRENT_BINARY_DIR}/generated/cases.inc")
set(_pltxt2htm_cases_previous "")
if(EXISTS "${_pltxt2htm_cases_inc}")
    file(READ "${_pltxt2htm_cases_inc}" _pltxt2htm_cases_previous)
endif()
if(NOT _pltxt2htm_cases_previous STREQUAL _pltxt2htm_cases)
    file(WRITE "${_pltxt2htm_cases_inc}" "${_pltxt2htm_cases}")
endif()

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE precompile doctest::doctest)
    # doctest_suites.cc consumes the generated include list.
    if(TEST_NAME STREQUAL "doctest_suites")
        target_include_directories(${TEST_NAME} PRIVATE
            "${CMAKE_CURRENT_BINARY_DIR}/generated"
            "${CMAKE_CURRENT_SOURCE_DIR}/src")
    endif()
    # Tests are built as pltxt2htm itself, so external-only APIs must stay unavailable to them.
    # The deep-copy, copy-assignment, ast-interface and container-subscript tests are the
    # exception: they stand in for a downstream user and cover the copy operations and
    # external-only container APIs that are part of the public API.
    if(NOT TEST_NAME STREQUAL "deep_copy" AND NOT TEST_NAME STREQUAL "copy_assignment"
       AND NOT TEST_NAME STREQUAL "ast_interface" AND NOT TEST_NAME STREQUAL "container_subscript")
        target_compile_definitions(${TEST_NAME} PRIVATE PLTXT2HTM_INTERNAL_USE)
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${TEST_NAME} PRIVATE -g -Werror=switch)
    endif()
    # The node layout test uses the reflection implementation shipped by GCC 16.
    if(TEST_NAME STREQUAL "node_layout" AND CMAKE_CXX_STANDARD STREQUAL "26"
       AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16)
        target_compile_options(${TEST_NAME} PRIVATE -freflection)
    endif()
    if(HAVE_LD_LLD AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_link_options(${TEST_NAME} PRIVATE -fuse-ld=lld)
    endif()
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
