# Define the source-tree target once for both the repository build and legacy
# standalone sub-project builds. An installed package uses pltxt2htmTargets.cmake.
include(GNUInstallDirs)

set(PLTXT2HTM_HEADER_DIR "${CMAKE_CURRENT_LIST_DIR}/../include")

add_library(pltxt2htm_headers INTERFACE)
set_target_properties(pltxt2htm_headers PROPERTIES EXPORT_NAME pltxt2htm)
add_library(pltxt2htm::pltxt2htm ALIAS pltxt2htm_headers)

target_compile_features(pltxt2htm_headers INTERFACE cxx_std_23)
target_include_directories(pltxt2htm_headers INTERFACE
    "$<BUILD_INTERFACE:${PLTXT2HTM_HEADER_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

include("${CMAKE_CURRENT_LIST_DIR}/pltxt2htmTargetOptions.cmake")
pltxt2htm_apply_platform_options(pltxt2htm_headers)
