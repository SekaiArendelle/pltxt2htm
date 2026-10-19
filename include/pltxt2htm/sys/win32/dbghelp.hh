#pragma once

#include "dbghelp_definitions.hh"
#include "../push_macro.hh"

#if !defined(_WIN32)
    #error "DbgHelp is only available on Windows"
#endif

namespace pltxt2htm::sys_win32 {

#include "dbghelp.inc"

} // namespace pltxt2htm::sys_win32

#if defined(_MSC_VER) && !defined(__clang__)
    #if defined(_M_ARM64EC)
        #include "dbghelp_msvc_linker_arm64ec.inc"
    #elif defined(_M_X64)
        #include "dbghelp_msvc_linker_x64.inc"
    #elif defined(_M_IX86)
        #include "dbghelp_msvc_linker_i686.inc"
    #elif defined(_M_ARM64)
        #include "dbghelp_msvc_linker_arm64.inc"
    #elif defined(_M_ARM)
        #include "dbghelp_msvc_linker_arm32.inc"
    #else
        #error "unsupported MSVC target for pltxt2htm DbgHelp symbols"
    #endif
#endif

#include "../pop_macro.hh"
