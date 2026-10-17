cmake_minimum_required(VERSION 3.25)
project(pltxt2htm-platform-options LANGUAGES NONE)

# Exercise the installed package's Windows/MSVC settings at configure time,
# without requiring a Windows compiler on the test host.
set(WIN32 TRUE)
set(MSVC TRUE)
find_package(pltxt2htm CONFIG REQUIRED)

get_target_property(compile_options pltxt2htm::pltxt2htm INTERFACE_COMPILE_OPTIONS)
foreach(option IN ITEMS /utf-8 /Zc:u8EscapeEncoding)
    if(NOT option IN_LIST compile_options)
        message(FATAL_ERROR "The installed target is missing ${option}")
    endif()
endforeach()

get_target_property(link_libraries pltxt2htm::pltxt2htm INTERFACE_LINK_LIBRARIES)
if(NOT ntdll IN_LIST link_libraries)
    message(FATAL_ERROR "The installed target is missing ntdll")
endif()
