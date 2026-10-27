#pragma once

#include "definitions.hh"
#include "../push_macro.hh"

#if !defined(_WIN32)
    #error "pltxt2htm Win32 symbols are only available on Windows"
#endif

namespace pltxt2htm::sys_win32 {

#include "kernel32.inc"

} // namespace pltxt2htm::sys_win32

#if defined(_MSC_VER) && !defined(__clang__)
    #if defined(_M_ARM64EC)
        #include "kernel32_msvc_linker_arm64ec.inc"
    #elif defined(_M_X64)
        #include "kernel32_msvc_linker_x64.inc"
    #elif defined(_M_IX86)
        #include "kernel32_msvc_linker_i686.inc"
    #elif defined(_M_ARM64)
        #include "kernel32_msvc_linker_arm64.inc"
    #elif defined(_M_ARM)
        #include "kernel32_msvc_linker_arm32.inc"
    #else
        #error "unsupported MSVC target for pltxt2htm Win32 symbols"
    #endif
#endif

#include "../pop_macro.hh"
