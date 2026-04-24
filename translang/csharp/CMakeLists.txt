cmake_minimum_required(VERSION 3.29)
project(pltxt2htm_csharp_translator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ZLIB REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(zstd CONFIG REQUIRED)
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

# Conda-forge LLVM on Windows may export a VSINSTALLDIR DIA path that is
# unavailable in pixi non-VS environments.
if(WIN32 AND TARGET LLVMDebugInfoCodeView)
    set_property(TARGET LLVMDebugInfoCodeView PROPERTY
        INTERFACE_LINK_LIBRARIES "LLVMBinaryFormat;LLVMObject;LLVMSupport;LLVMDebugInfoMSF"
    )
endif()
if(WIN32 AND TARGET LLVMDebugInfoPDB)
    set_property(TARGET LLVMDebugInfoPDB PROPERTY
        INTERFACE_LINK_LIBRARIES "LLVMBinaryFormat;LLVMObject;LLVMSupport;LLVMDebugInfoCodeView;LLVMDebugInfoMSF"
    )
endif()

add_executable(pltxt2htm_csharp_translator src/main.cc)

target_include_directories(pltxt2htm_csharp_translator PRIVATE
    ${LLVM_INCLUDE_DIRS}
    ${CLANG_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

add_definitions(${LLVM_DEFINITIONS})

target_link_libraries(pltxt2htm_csharp_translator PRIVATE
    clangAST
    clangBasic
    clangFrontend
    clangFrontendTool
    clangLex
    clangSerialization
    clangTooling
    clangToolingCore
    LLVMCore
    LLVMSupport
    ZLIB::ZLIB
    zstd::libzstd
)

if(NOT WIN32)
    target_link_libraries(pltxt2htm_csharp_translator PRIVATE LibXml2::LibXml2)
endif()

install(TARGETS pltxt2htm_csharp_translator
    RUNTIME DESTINATION dist
)

if(WIN32)
    install(FILES $<TARGET_RUNTIME_DLLS:pltxt2htm_csharp_translator>
        DESTINATION dist
    )

    if(DEFINED ENV{CONDA_PREFIX})
        set(_conda_prefix "$ENV{CONDA_PREFIX}")
        file(TO_CMAKE_PATH "${_conda_prefix}" _conda_prefix)
        set(_conda_bin "${_conda_prefix}/Library/bin")
        install(FILES
            "${_conda_bin}/zlib.dll"
            "${_conda_bin}/zstd.dll"
            "${_conda_bin}/libzstd.dll"
            DESTINATION dist
            OPTIONAL
        )
    endif()
endif()
