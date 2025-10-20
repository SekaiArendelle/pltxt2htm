// This macro is a part of API in c/
#pragma push_macro("PLTXT2HTM_C_NAME_MANGLING")
#undef PLTXT2HTM_C_NAME_MANGLING
#if defined(NDEBUG)
    #define PLTXT2HTM_C_NAME_MANGLING(name) name
#else
    #define PLTXT2HTM_C_NAME_MANGLING(name) name##d
#endif

#pragma push_macro("PLTXT2HTM_C_NOEXCEPT")
#undef PLTXT2HTM_C_NOEXCEPT
#if __cpp_noexcept_function_type >= 201510L
    #define PLTXT2HTM_C_NOEXCEPT noexcept
#else
    #define PLTXT2HTM_C_NOEXCEPT
#endif
