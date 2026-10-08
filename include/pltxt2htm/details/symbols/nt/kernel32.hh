#pragma once

#if defined(_MSC_VER)
    #define PLTXT2HTM_NT_IMPORT __declspec(dllimport)
    #define PLTXT2HTM_NT_CALL __stdcall
#elif defined(__i386__)
    #define PLTXT2HTM_NT_IMPORT __attribute__((dllimport))
    #define PLTXT2HTM_NT_CALL __attribute__((stdcall))
#else
    #define PLTXT2HTM_NT_IMPORT __attribute__((dllimport))
    #define PLTXT2HTM_NT_CALL
#endif

#if defined(__clang__) || defined(__GNUC__)
    #if defined(__i386__) && defined(__clang__)
        #define PLTXT2HTM_NT_SYMBOL(name, size) __asm__("_" #name "@" #size)
    #elif defined(__i386__)
        #define PLTXT2HTM_NT_SYMBOL(name, size) __asm__(#name "@" #size)
    #else
        #define PLTXT2HTM_NT_SYMBOL(name, size) __asm__(#name)
    #endif
#else
    #define PLTXT2HTM_NT_SYMBOL(name, size)
#endif

namespace pltxt2htm::details::symbols::nt {
using Procedure = void(PLTXT2HTM_NT_CALL*)();
extern "C" {
[[nodiscard]]
PLTXT2HTM_NT_IMPORT void* PLTXT2HTM_NT_CALL pltxt2htm_nt_load_library_ex_w(wchar_t const*, void*,
                                                                           unsigned long) noexcept
    PLTXT2HTM_NT_SYMBOL(LoadLibraryExW, 12);
[[nodiscard]]
PLTXT2HTM_NT_IMPORT Procedure PLTXT2HTM_NT_CALL pltxt2htm_nt_get_proc_address(void*, char const*) noexcept
    PLTXT2HTM_NT_SYMBOL(GetProcAddress, 8);
[[nodiscard]]
PLTXT2HTM_NT_IMPORT int PLTXT2HTM_NT_CALL pltxt2htm_nt_free_library(void*) noexcept PLTXT2HTM_NT_SYMBOL(FreeLibrary, 4);
}
} // namespace pltxt2htm::details::symbols::nt

#undef PLTXT2HTM_NT_IMPORT
#undef PLTXT2HTM_NT_CALL

#undef PLTXT2HTM_NT_SYMBOL

#if defined(_MSC_VER) && !defined(__clang__)
    #if defined(_M_IX86)
        #pragma comment(linker, "/alternatename:__imp__pltxt2htm_nt_load_library_ex_w@12=__imp__LoadLibraryExW@12")
        #pragma comment(linker, "/alternatename:__imp__pltxt2htm_nt_get_proc_address@8=__imp__GetProcAddress@8")
        #pragma comment(linker, "/alternatename:__imp__pltxt2htm_nt_free_library@4=__imp__FreeLibrary@4")
    #else
        #pragma comment(linker, "/alternatename:__imp_pltxt2htm_nt_load_library_ex_w=__imp_LoadLibraryExW")
        #pragma comment(linker, "/alternatename:__imp_pltxt2htm_nt_get_proc_address=__imp_GetProcAddress")
        #pragma comment(linker, "/alternatename:__imp_pltxt2htm_nt_free_library=__imp_FreeLibrary")
    #endif
#endif
