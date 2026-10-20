/**
 * @file msvc_win32_imports.hh
 * @brief Locally scoped definitions of the Win32 import/calling-convention
 *        attribute macros, remapped from fast_io's `push_macros.h`
 *        (`FAST_IO_DLLIMPORT`, `FAST_IO_WINSTDCALL`, `FAST_IO_WINSTDCALL_RENAME`,
 *        `FAST_IO_GNU_MALLOC`, `FAST_IO_GNU_CONST`,
 *        `FAST_IO_GNU_ALWAYS_INLINE_ARTIFICIAL_CONST`).
 * @details Not self-guarded: each including backend header pairs this with a
 *          matching `#pragma pop_macro` block so the macros never leak.
 */

#pragma push_macro("pltxt2htm_dllimport")
#undef pltxt2htm_dllimport
#if defined(_MSC_VER) && !defined(__clang__)
    #define pltxt2htm_dllimport __declspec(dllimport)
#elif __has_cpp_attribute(__gnu__::__dllimport__) && !defined(__WINE__) && !defined(__arm64ec__)
    #define pltxt2htm_dllimport [[__gnu__::__dllimport__]]
#else
    #define pltxt2htm_dllimport
#endif

#pragma push_macro("pltxt2htm_win_stdcall")
#undef pltxt2htm_win_stdcall
#if defined(_MSC_VER) && (!__has_cpp_attribute(__gnu__::__stdcall__) && !defined(__WINE__))
    #define pltxt2htm_win_stdcall __stdcall
#elif __has_cpp_attribute(__gnu__::__stdcall__) && !defined(__WINE__)
    #define pltxt2htm_win_stdcall __attribute__((__stdcall__))
#else
    #define pltxt2htm_win_stdcall
#endif

#pragma push_macro("pltxt2htm_win_stdcall_name")
#undef pltxt2htm_win_stdcall_name
#if defined(__clang__) || defined(__GNUC__)
    #if defined(_M_HYBRID)
        #define pltxt2htm_win_stdcall_name(name, count) __asm__("#" #name "@" #count)
    #elif defined(__arm64ec__)
        #define pltxt2htm_win_stdcall_name(name, count) __asm__("#" #name)
    #elif SIZE_MAX <= UINT_LEAST32_MAX && (defined(__x86__) || defined(_M_IX86) || defined(__i386__))
        #if !defined(__clang__)
            #define pltxt2htm_win_stdcall_name(name, count) __asm__(#name "@" #count)
        #else
            #define pltxt2htm_win_stdcall_name(name, count) __asm__("_" #name "@" #count)
        #endif
    #else
        #define pltxt2htm_win_stdcall_name(name, count) __asm__(#name)
    #endif
#else
    #define pltxt2htm_win_stdcall_name(name, count)
#endif

#pragma push_macro("pltxt2htm_gnu_malloc")
#undef pltxt2htm_gnu_malloc
#if __has_cpp_attribute(__gnu__::__malloc__)
    #define pltxt2htm_gnu_malloc [[__gnu__::__malloc__]]
#else
    #define pltxt2htm_gnu_malloc
#endif

#pragma push_macro("pltxt2htm_gnu_const")
#undef pltxt2htm_gnu_const
#if __has_cpp_attribute(__gnu__::__const__)
    #define pltxt2htm_gnu_const [[__gnu__::__const__]]
#else
    #define pltxt2htm_gnu_const
#endif

#pragma push_macro("pltxt2htm_always_inline")
#undef pltxt2htm_always_inline
#if __has_cpp_attribute(__gnu__::__always_inline__)
    #define pltxt2htm_always_inline [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
    #define pltxt2htm_always_inline [[msvc::forceinline]]
#else
    #define pltxt2htm_always_inline
#endif

#pragma push_macro("pltxt2htm_artificial")
#undef pltxt2htm_artificial
#if __has_cpp_attribute(__gnu__::__artificial__)
    #define pltxt2htm_artificial [[__gnu__::__artificial__]]
#else
    #define pltxt2htm_artificial
#endif

#define pltxt2htm_always_inline_artificial_const \
    pltxt2htm_always_inline [[nodiscard]] pltxt2htm_artificial pltxt2htm_gnu_const
