#pragma push_macro("PLTXT2HTM_DETAIL_DLLIMPORT")
#pragma push_macro("PLTXT2HTM_DETAIL_STDCALL")
#pragma push_macro("PLTXT2HTM_DETAIL_CDECL")
#pragma push_macro("PLTXT2HTM_DETAIL_ASM_NAME")
#pragma push_macro("PLTXT2HTM_DETAIL_CDECL_ASM_NAME")
#undef PLTXT2HTM_DETAIL_DLLIMPORT
#undef PLTXT2HTM_DETAIL_STDCALL
#undef PLTXT2HTM_DETAIL_CDECL
#undef PLTXT2HTM_DETAIL_ASM_NAME
#undef PLTXT2HTM_DETAIL_CDECL_ASM_NAME

#if defined(_MSC_VER)
    #define PLTXT2HTM_DETAIL_DLLIMPORT __declspec(dllimport)
#else
    #define PLTXT2HTM_DETAIL_DLLIMPORT __attribute__((dllimport))
#endif

#if defined(_MSC_VER)
    #define PLTXT2HTM_DETAIL_CDECL __cdecl
#else
    #define PLTXT2HTM_DETAIL_CDECL
#endif

#if defined(_MSC_VER)
    #define PLTXT2HTM_DETAIL_STDCALL __stdcall
#elif defined(__i386__)
    #define PLTXT2HTM_DETAIL_STDCALL __attribute__((stdcall))
#else
    #define PLTXT2HTM_DETAIL_STDCALL
#endif

#if defined(_MSC_VER) && !defined(__clang__)
    #define PLTXT2HTM_DETAIL_ASM_NAME(name, count)
    #define PLTXT2HTM_DETAIL_CDECL_ASM_NAME(name)
#elif defined(__clang__) || defined(__GNUC__)
    #if defined(_M_HYBRID)
        #define PLTXT2HTM_DETAIL_ASM_NAME(name, count) __asm__("#" #name "@" #count)
        #define PLTXT2HTM_DETAIL_CDECL_ASM_NAME(name) __asm__("#" #name)
    #elif defined(__arm64ec__) || defined(_M_ARM64EC)
        #define PLTXT2HTM_DETAIL_ASM_NAME(name, count) __asm__("#" #name)
        #define PLTXT2HTM_DETAIL_CDECL_ASM_NAME(name) __asm__("#" #name)
    #elif defined(__i386__) || defined(_M_IX86)
        #if defined(__clang__)
            #define PLTXT2HTM_DETAIL_ASM_NAME(name, count) __asm__("_" #name "@" #count)
            #define PLTXT2HTM_DETAIL_CDECL_ASM_NAME(name) __asm__("_" #name)
        #else
            #define PLTXT2HTM_DETAIL_ASM_NAME(name, count) __asm__(#name "@" #count)
            #define PLTXT2HTM_DETAIL_CDECL_ASM_NAME(name) __asm__(#name)
        #endif
    #else
        #define PLTXT2HTM_DETAIL_ASM_NAME(name, count) __asm__(#name)
        #define PLTXT2HTM_DETAIL_CDECL_ASM_NAME(name) __asm__(#name)
    #endif
#endif
