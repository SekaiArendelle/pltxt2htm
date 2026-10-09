#pragma once

#include "definitions.hh"

#if !defined(_WIN32)
    #error "pltxt2htm NT symbols are only available on Windows"
#endif

#pragma push_macro("PLTXT2HTM_DETAIL_NT_DLLIMPORT")
#pragma push_macro("PLTXT2HTM_DETAIL_NT_CALL")
#pragma push_macro("PLTXT2HTM_DETAIL_NT_ASM_NAME")

#undef PLTXT2HTM_DETAIL_NT_DLLIMPORT
#undef PLTXT2HTM_DETAIL_NT_CALL
#undef PLTXT2HTM_DETAIL_NT_ASM_NAME

namespace pltxt2htm::details::symbols::nt {
extern "C" {

#if defined(_MSC_VER) && !defined(__clang__)
    #define PLTXT2HTM_DETAIL_NT_DLLIMPORT __declspec(dllimport)
    #define PLTXT2HTM_DETAIL_NT_CALL __stdcall
    #define PLTXT2HTM_DETAIL_NT_ASM_NAME(name, count)
#elif defined(__clang__) || defined(__GNUC__)
    #if defined(_MSC_VER)
        #define PLTXT2HTM_DETAIL_NT_DLLIMPORT __declspec(dllimport)
        #define PLTXT2HTM_DETAIL_NT_CALL __stdcall
    #elif defined(__i386__)
        #define PLTXT2HTM_DETAIL_NT_DLLIMPORT __attribute__((dllimport))
        #define PLTXT2HTM_DETAIL_NT_CALL __attribute__((stdcall))
    #else
        #define PLTXT2HTM_DETAIL_NT_DLLIMPORT __attribute__((dllimport))
        #define PLTXT2HTM_DETAIL_NT_CALL
    #endif

    #if defined(_M_HYBRID)
        #define PLTXT2HTM_DETAIL_NT_ASM_NAME(name, count) __asm__("#" #name "@" #count)
    #elif defined(__arm64ec__) || defined(_M_ARM64EC)
        #define PLTXT2HTM_DETAIL_NT_ASM_NAME(name, count) __asm__("#" #name)
    #elif defined(__i386__) || defined(_M_IX86)
        #if defined(__clang__)
            #define PLTXT2HTM_DETAIL_NT_ASM_NAME(name, count) __asm__("_" #name "@" #count)
        #else
            #define PLTXT2HTM_DETAIL_NT_ASM_NAME(name, count) __asm__(#name "@" #count)
        #endif
    #else
        #define PLTXT2HTM_DETAIL_NT_ASM_NAME(name, count) __asm__(#name)
    #endif
#else
    #error "unsupported compiler for pltxt2htm NT symbols"
#endif

#include "ntdll.inc"

} // extern "C"
} // namespace pltxt2htm::details::symbols::nt

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

#pragma pop_macro("PLTXT2HTM_DETAIL_NT_ASM_NAME")
#pragma pop_macro("PLTXT2HTM_DETAIL_NT_CALL")
#pragma pop_macro("PLTXT2HTM_DETAIL_NT_DLLIMPORT")
