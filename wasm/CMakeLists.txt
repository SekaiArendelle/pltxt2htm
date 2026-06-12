cmake_minimum_required(VERSION 3.20)
project(pltxt2htm-wasm LANGUAGES CXX VERSION 0.21.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()

option(PLTXT2HTM_ENABLE_STACKTRACE "Enable C++23 <stacktrace>" OFF)

add_executable(pltxt2htm pltxt2htm.cc)

target_include_directories(pltxt2htm PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_compile_features(pltxt2htm PRIVATE cxx_std_23)

if(PLTXT2HTM_ENABLE_STACKTRACE)
    target_compile_definitions(pltxt2htm PRIVATE PLTXT2HTM_EXPERIMENTAL_ENABLE_STACKTRACE)
endif()

# Emscripten-specific: output .js with a .wasm side module
set_target_properties(pltxt2htm PROPERTIES SUFFIX ".js")

target_compile_options(pltxt2htm PRIVATE
    -fvisibility=hidden
    -fno-rtti
    -fno-unwind-tables
    -fno-asynchronous-unwind-tables
    -fno-exceptions
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(pltxt2htm PRIVATE -fno-ident)
    target_link_options(pltxt2htm PRIVATE -flto)
endif()

target_link_options(pltxt2htm PRIVATE
    -fuse-ld=lld
    "-sEXPORTED_FUNCTIONS=['_common_parser','_fixedadv_parser','_free','_ver_major','_ver_minor','_ver_patch']"
    "-sEXPORTED_RUNTIME_METHODS=['cwrap','UTF8ToString']"
    -sMODULARIZE=1
    -sEXPORT_ES6=1
    -sEXPORT_NAME=pltxt2htm
)

# Install step: copy all pltxt2htm.* outputs to install dir
install(TARGETS pltxt2htm
    BUNDLE DESTINATION .
    RUNTIME DESTINATION .
)

# Emscripten also produces a .wasm file alongside the .js
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pltxt2htm.wasm
    DESTINATION .
    OPTIONAL
)
