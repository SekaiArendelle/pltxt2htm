#pragma once

#include "definitions.hh"
#include "../push_macro.hh"

#if !defined(_WIN32)
    #error "pltxt2htm NT symbols are only available on Windows"
#endif

namespace pltxt2htm::sys::nt {

#include "ntdll.inc"

} // namespace pltxt2htm::sys::nt

#if defined(_MSC_VER) && !defined(__clang__)
    #if defined(_M_ARM64EC)
        #include "ntdll_msvc_linker_arm64ec.inc"
    #elif defined(_M_X64)
        #include "ntdll_msvc_linker_x64.inc"
    #elif defined(_M_IX86)
        #include "ntdll_msvc_linker_i686.inc"
    #elif defined(_M_ARM64)
        #include "ntdll_msvc_linker_arm64.inc"
    #elif defined(_M_ARM)
        #include "ntdll_msvc_linker_arm32.inc"
    #else
        #error "unsupported MSVC target for pltxt2htm NT symbols"
    #endif
#endif

#include "../pop_macro.hh"
