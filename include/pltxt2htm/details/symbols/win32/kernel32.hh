#pragma once

#include "definitions.hh"

#if !defined(_WIN32)
    #error "pltxt2htm Win32 symbols are only available on Windows"
#endif

#pragma push_macro("PLTXT2HTM_DETAIL_WIN32_DLLIMPORT")
#pragma push_macro("PLTXT2HTM_DETAIL_WIN32_CALL")
#pragma push_macro("PLTXT2HTM_DETAIL_WIN32_ASM_NAME")

#undef PLTXT2HTM_DETAIL_WIN32_DLLIMPORT
#undef PLTXT2HTM_DETAIL_WIN32_CALL
#undef PLTXT2HTM_DETAIL_WIN32_ASM_NAME

#if defined(_MSC_VER) && !defined(__clang__)
    #define PLTXT2HTM_DETAIL_WIN32_DLLIMPORT __declspec(dllimport)
    #define PLTXT2HTM_DETAIL_WIN32_CALL __stdcall
    #define PLTXT2HTM_DETAIL_WIN32_ASM_NAME(name, count)
#elif defined(__clang__) || defined(__GNUC__)
    #if defined(_MSC_VER)
        #define PLTXT2HTM_DETAIL_WIN32_DLLIMPORT __declspec(dllimport)
        #define PLTXT2HTM_DETAIL_WIN32_CALL __stdcall
    #elif defined(__i386__)
        #define PLTXT2HTM_DETAIL_WIN32_DLLIMPORT __attribute__((dllimport))
        #define PLTXT2HTM_DETAIL_WIN32_CALL __attribute__((stdcall))
    #else
        #define PLTXT2HTM_DETAIL_WIN32_DLLIMPORT __attribute__((dllimport))
        #define PLTXT2HTM_DETAIL_WIN32_CALL
    #endif
#else
    #error "unsupported compiler for pltxt2htm Win32 symbols"
#endif

namespace pltxt2htm::details::symbols::win32 {
using Procedure = IntPtr(PLTXT2HTM_DETAIL_WIN32_CALL*)();

#if defined(__GNUC__) || defined(__clang__)
    #if defined(_M_HYBRID)
        #define PLTXT2HTM_DETAIL_WIN32_ASM_NAME(name, count) __asm__("#" #name "@" #count)
    #elif defined(__arm64ec__) || defined(_M_ARM64EC)
        #define PLTXT2HTM_DETAIL_WIN32_ASM_NAME(name, count) __asm__("#" #name)
    #elif defined(__i386__) || defined(_M_IX86)
        #if defined(__clang__)
            #define PLTXT2HTM_DETAIL_WIN32_ASM_NAME(name, count) __asm__("_" #name "@" #count)
        #else
            #define PLTXT2HTM_DETAIL_WIN32_ASM_NAME(name, count) __asm__(#name "@" #count)
        #endif
    #else
        #define PLTXT2HTM_DETAIL_WIN32_ASM_NAME(name, count) __asm__(#name)
    #endif
#endif

#include "kernel32.inc"

} // namespace pltxt2htm::details::symbols::win32

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

#pragma pop_macro("PLTXT2HTM_DETAIL_WIN32_ASM_NAME")
#pragma pop_macro("PLTXT2HTM_DETAIL_WIN32_CALL")
#pragma pop_macro("PLTXT2HTM_DETAIL_WIN32_DLLIMPORT")
