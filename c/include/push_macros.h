// This macro is a part of API in c/
#pragma push_macro("PLTXT2HTM_C_NAME_MANGLING")
#undef PLTXT2HTM_C_NAME_MANGLING
#if defined(NDEBUG)
    #define PLTXT2HTM_C_NAME_MANGLING(name) pltxt2htm_##name
#else
    #define PLTXT2HTM_C_NAME_MANGLING(name) pltxt2htm_##name##d
#endif
