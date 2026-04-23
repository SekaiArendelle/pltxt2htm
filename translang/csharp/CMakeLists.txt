cmake_minimum_required(VERSION 3.29)
project(pltxt2htm_csharp_translator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

add_executable(pltxt2htm_csharp_translator src/main.cc)

target_include_directories(pltxt2htm_csharp_translator PRIVATE
    ${LLVM_INCLUDE_DIRS}
    ${CLANG_INCLUDE_DIRS}
)

add_definitions(${LLVM_DEFINITIONS})

target_link_libraries(pltxt2htm_csharp_translator PRIVATE
    clangAST
    clangBasic
    clangFrontend
    clangLex
    clangSerialization
    clangTooling
    LLVMCore
    LLVMSupport
)

set(PLTXT2HTM_HEADER "${CMAKE_CURRENT_LIST_DIR}/../../include/pltxt2htm/pltxt2htm.hh")
set(OUTPUT_CS "${CMAKE_CURRENT_LIST_DIR}/dist/Pltxt2htm.Generated.cs")
set(INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../include")

add_custom_target(translate
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_LIST_DIR}/dist"
    COMMAND $<TARGET_FILE:pltxt2htm_csharp_translator>
        --header "${PLTXT2HTM_HEADER}"
        --output "${OUTPUT_CS}"
        --include "${INCLUDE_DIR}"
    DEPENDS pltxt2htm_csharp_translator
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    COMMENT "Translating include/pltxt2htm to C# with clang/LLVM C++ API"
)
