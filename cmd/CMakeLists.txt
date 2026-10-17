cmake_minimum_required(VERSION 3.25)
project(pltxt2htm-cmd LANGUAGES CXX VERSION 0.27.0)
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

add_executable(pltxt2htm_cmd pltxt2htm.cc)
set_target_properties(pltxt2htm_cmd PROPERTIES
    OUTPUT_NAME pltxt2htm
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

target_link_libraries(pltxt2htm_cmd PRIVATE pltxt2htm::pltxt2htm)

if(PLTXT2HTM_ENABLE_STACKTRACE)
    target_compile_definitions(pltxt2htm_cmd PRIVATE PLTXT2HTM_ENABLE_STACKTRACE)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(pltxt2htm_cmd PRIVATE
            -fno-rtti
            -fno-unwind-tables
            -fno-asynchronous-unwind-tables
            -fno-ident
            -fno-exceptions
        )
        include(CheckIPOSupported)
        check_ipo_supported(RESULT _pltxt2htm_ipo_supported LANGUAGES CXX)
        if(_pltxt2htm_ipo_supported)
            set_property(TARGET pltxt2htm_cmd PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
        endif()
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(pltxt2htm_cmd PRIVATE -Wall -Wextra -fexceptions)
    endif()
endif()

# Generate repo_info.ignore with git info at configure time
set(REPO_INFO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/repo_info.ignore")
execute_process(
    COMMAND git status --porcelain
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
    OUTPUT_VARIABLE GIT_STATUS
    RESULT_VARIABLE GIT_RESULT
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(GIT_RESULT EQUAL 0 AND GIT_STATUS)
    execute_process(
        COMMAND git status
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
        OUTPUT_VARIABLE GIT_FULL_STATUS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string(REPLACE "\"" "\\\"" GIT_FULL_STATUS "${GIT_FULL_STATUS}")
    string(REPLACE "\n" "\\n" GIT_FULL_STATUS "${GIT_FULL_STATUS}")
    file(WRITE ${REPO_INFO_FILE} "\"* repo status: UNCOMMITED\\n${GIT_FULL_STATUS}\\n\"")
else()
    execute_process(
        COMMAND git rev-parse HEAD
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    file(WRITE ${REPO_INFO_FILE} "\"* build commit: ${GIT_COMMIT_HASH}\\n\"")
endif()

install(TARGETS pltxt2htm_cmd RUNTIME DESTINATION .)
