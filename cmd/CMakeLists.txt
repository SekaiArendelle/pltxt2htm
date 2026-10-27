cmake_minimum_required(VERSION 3.20)
project(pltxt2htm-cmd LANGUAGES CXX VERSION 0.27.0)
include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/pltxt2htm_contract_mode.cmake")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()

option(PLTXT2HTM_ENABLE_STACKTRACE "Enable native stacktrace" OFF)

add_executable(pltxt2htm pltxt2htm.cc)
pltxt2htm_apply_contract_mode(pltxt2htm PRIVATE)

target_include_directories(pltxt2htm PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_compile_features(pltxt2htm PRIVATE cxx_std_23)

if(PLTXT2HTM_ENABLE_STACKTRACE)
    if(NOT WIN32)
        message(FATAL_ERROR "Native stacktrace currently supports Windows only")
    endif()
    target_compile_definitions(pltxt2htm PRIVATE PLTXT2HTM_ENABLE_STACKTRACE)
    target_link_libraries(pltxt2htm PRIVATE ntdll dbghelp)
endif()

if(WIN32 OR MINGW OR CMAKE_CXX_COMPILER_TARGET MATCHES "windows-msvc$")
    target_link_libraries(pltxt2htm PRIVATE ntdll)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(pltxt2htm PRIVATE
            -fno-rtti
            $<$<NOT:$<BOOL:${PLTXT2HTM_ENABLE_STACKTRACE}>>:-fno-unwind-tables>
            $<$<NOT:$<BOOL:${PLTXT2HTM_ENABLE_STACKTRACE}>>:-fno-asynchronous-unwind-tables>
            -fno-ident
            -fno-exceptions
        )
        include(CheckIPOSupported)
        check_ipo_supported(RESULT _pltxt2htm_ipo_supported LANGUAGES CXX)
        if(_pltxt2htm_ipo_supported)
            set_property(TARGET pltxt2htm PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
        endif()
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(pltxt2htm PRIVATE -Wall -Wextra -fexceptions)
    endif()
endif()

# Generate repo_info.ignore with git info at configure time
set(REPO_INFO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/repo_info.ignore")
execute_process(
    COMMAND git status --porcelain
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/..
    OUTPUT_VARIABLE GIT_STATUS
    RESULT_VARIABLE GIT_RESULT
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(GIT_RESULT EQUAL 0 AND GIT_STATUS)
    execute_process(
        COMMAND git status
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/..
        OUTPUT_VARIABLE GIT_FULL_STATUS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string(REPLACE "\"" "\\\"" GIT_FULL_STATUS "${GIT_FULL_STATUS}")
    string(REPLACE "\n" "\\n" GIT_FULL_STATUS "${GIT_FULL_STATUS}")
    file(WRITE ${REPO_INFO_FILE} "\"* repo status: UNCOMMITED\\n${GIT_FULL_STATUS}\\n\"")
else()
    execute_process(
        COMMAND git rev-parse HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/..
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    file(WRITE ${REPO_INFO_FILE} "\"* build commit: ${GIT_COMMIT_HASH}\\n\"")
endif()

install(TARGETS pltxt2htm RUNTIME DESTINATION .)
