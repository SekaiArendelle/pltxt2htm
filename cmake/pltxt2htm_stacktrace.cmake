include_guard(GLOBAL)

function(pltxt2htm_enable_stacktrace target visibility)
    target_compile_definitions(${target} ${visibility} PLTXT2HTM_ENABLE_STACKTRACE)

    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
        return()
    endif()

    if(NOT TARGET PkgConfig::PLTXT2HTM_LIBDW)
        find_package(PkgConfig QUIET)
        if(PkgConfig_FOUND)
            pkg_check_modules(PLTXT2HTM_LIBDW QUIET IMPORTED_TARGET GLOBAL libdw)
        endif()
    endif()

    if(NOT TARGET PkgConfig::PLTXT2HTM_LIBDW AND NOT TARGET pltxt2htm_libdw)
        find_path(PLTXT2HTM_LIBDW_INCLUDE_DIR elfutils/libdwfl.h)
        find_library(PLTXT2HTM_LIBDW_LIBRARY NAMES dw)
        if(PLTXT2HTM_LIBDW_INCLUDE_DIR AND PLTXT2HTM_LIBDW_LIBRARY)
            add_library(pltxt2htm_libdw UNKNOWN IMPORTED GLOBAL)
            set_target_properties(pltxt2htm_libdw PROPERTIES
                IMPORTED_LOCATION "${PLTXT2HTM_LIBDW_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${PLTXT2HTM_LIBDW_INCLUDE_DIR}"
            )
        endif()
    endif()

    if(TARGET PkgConfig::PLTXT2HTM_LIBDW)
        set(_pltxt2htm_libdw_target PkgConfig::PLTXT2HTM_LIBDW)
    elseif(TARGET pltxt2htm_libdw)
        set(_pltxt2htm_libdw_target pltxt2htm_libdw)
    endif()

    if(DEFINED _pltxt2htm_libdw_target)
        target_compile_definitions(${target} ${visibility} PLTXT2HTM_DETAIL_STACKTRACE_HAS_LIBDWFL)
        target_link_libraries(${target} ${visibility} ${_pltxt2htm_libdw_target})
        set(_pltxt2htm_stacktrace_backend "execinfo + libdwfl")
    else()
        set(_pltxt2htm_stacktrace_backend "execinfo")
    endif()

    get_property(_pltxt2htm_stacktrace_reported GLOBAL PROPERTY PLTXT2HTM_STACKTRACE_BACKEND_REPORTED)
    if(NOT _pltxt2htm_stacktrace_reported)
        message(STATUS "pltxt2htm stacktrace backend: ${_pltxt2htm_stacktrace_backend}")
        set_property(GLOBAL PROPERTY PLTXT2HTM_STACKTRACE_BACKEND_REPORTED TRUE)
    endif()
endfunction()
