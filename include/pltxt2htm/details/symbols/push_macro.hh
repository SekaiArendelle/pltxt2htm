#pragma push_macro("PLTXT2HTM_DETAIL_DLLIMPORT")
#pragma push_macro("PLTXT2HTM_DETAIL_STDCALL")
#undef PLTXT2HTM_DETAIL_DLLIMPORT
#undef PLTXT2HTM_DETAIL_STDCALL

#if defined(_MSC_VER)
    #define PLTXT2HTM_DETAIL_DLLIMPORT __declspec(dllimport)
#else
    #define PLTXT2HTM_DETAIL_DLLIMPORT __attribute__((dllimport))
#endif

#if defined(_MSC_VER)
    #define PLTXT2HTM_DETAIL_STDCALL __stdcall
#elif defined(__i386__)
    #define PLTXT2HTM_DETAIL_STDCALL __attribute__((stdcall))
#else
    #define PLTXT2HTM_DETAIL_STDCALL
#endif
