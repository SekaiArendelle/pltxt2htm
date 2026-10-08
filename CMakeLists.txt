# Entry point for consuming pltxt2htm from CMake.
#
# pltxt2htm is header-only, so this project only defines the interface target that
# carries the headers, the C++ standard and the platform flags the headers need,
# plus the install and export rules behind find_package(pltxt2htm CONFIG).
#
# The sub-projects (tests/, cmd/, examples/, py/, c/, wasm/, ...) keep their own
# standalone CMakeLists.txt and are deliberately not wired into this one.

cmake_minimum_required(VERSION 3.25)

project(pltxt2htm LANGUAGES CXX VERSION 0.26.0)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# With add_subdirectory()/FetchContent() the parent project owns the install rules,
# so only a standalone build generates them.
option(PLTXT2HTM_INSTALL "Generate the install and export rules of pltxt2htm" ${PROJECT_IS_TOP_LEVEL})

# fast_io is vendored next to pltxt2htm and included as <fast_io/...>, so both trees
# have to remain under a single include directory.
set(PLTXT2HTM_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(pltxt2htm INTERFACE)
add_library(pltxt2htm::pltxt2htm ALIAS pltxt2htm)

target_compile_features(pltxt2htm INTERFACE cxx_std_23)
target_include_directories(pltxt2htm INTERFACE
    "$<BUILD_INTERFACE:${PLTXT2HTM_HEADER_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# The headers are compiled as part of every consumer translation unit, so the
# platform flags they rely on have to travel with the target.
if(MSVC)
    # Universal character names inside u8 literals must keep their UTF-8 encoding.
    # /EHsc is deliberately not propagated here: the exception model is the
    # consumer's choice, and the library itself is exception-free.
    target_compile_options(pltxt2htm INTERFACE /utf-8 /Zc:u8EscapeEncoding)
endif()

if(WIN32 OR MINGW)
    target_link_libraries(pltxt2htm INTERFACE ntdll)
endif()

if(PLTXT2HTM_INSTALL)
    install(DIRECTORY "${PLTXT2HTM_HEADER_DIR}/pltxt2htm" "${PLTXT2HTM_HEADER_DIR}/fast_io"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/licenses/pltxt2htm")

    # Header-only: the target has no artifacts, it only has to be part of the export
    # set so that pltxt2htmTargets.cmake defines pltxt2htm::pltxt2htm.
    install(TARGETS pltxt2htm EXPORT pltxt2htmTargets)

    install(EXPORT pltxt2htmTargets
        NAMESPACE pltxt2htm::
        FILE pltxt2htmTargets.cmake
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/pltxt2htm")

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/pltxt2htmConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/pltxt2htmConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/pltxt2htm")

    # SameMinorVersion states the versioning intent of this library: releases within
    # one 0.x minor may break the API, patch releases may not. ARCH_INDEPENDENT
    # because a header-only package does not care about the pointer size it was
    # installed from.
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/pltxt2htmConfigVersion.cmake"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY SameMinorVersion
        ARCH_INDEPENDENT)

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/pltxt2htmConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/pltxt2htmConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/pltxt2htm")
endif()
