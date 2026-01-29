#include <pltxt2htm/pltxt2htm.h>
#include "../include/push_macros.h"

#if __has_attribute(visibility)
    #define PLTXT2HTM_C_SRC_VISIBILITY_DEFAULT __attribute__((visibility("default")))
#else
    #define PLTXT2HTM_C_SRC_VISIBILITY_DEFAULT
#endif

#if __has_cpp_attribute(__gnu__::__used__)
    #define PLTXT2HTM_C_SRC_USED [[__gnu__::__used__]]
#else
    #define PLTXT2HTM_C_SRC_USED
#endif

#if defined(_WIN32)
    #define PLTXT2HTM_C_SRC_WIN32_DLLEXPORT __declspec(dllexport)
#else
    #define PLTXT2HTM_C_SRC_WIN32_DLLEXPORT
#endif

#define PLTXT2HTM_C_SRC_EXPORT_FUNCTION \
    extern "C" PLTXT2HTM_C_SRC_VISIBILITY_DEFAULT PLTXT2HTM_C_SRC_USED PLTXT2HTM_C_SRC_WIN32_DLLEXPORT auto

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
PLTXT2HTM_C_NAME_MANGLING(common_parser)(char8_t const* const pltext) noexcept -> char8_t const* {
    return ::pltxt2htm::common_parser<
#if defined(NDEBUG)
        true
#else
        false
#endif
        >(pltext);
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
PLTXT2HTM_C_NAME_MANGLING(advanced_parser)(char8_t const* const pltext) noexcept -> char8_t const* {
    return ::pltxt2htm::advanced_parser<
#if defined(NDEBUG)
        true
#else
        false
#endif
        >(pltext);
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
PLTXT2HTM_C_NAME_MANGLING(fixedadv_parser)(char8_t const* const pltext, char8_t const* const host,
                                           char8_t const* const project, char8_t const* const visitor,
                                           char8_t const* const author, char8_t const* const coauthors) noexcept
    -> char8_t const* {
    return ::pltxt2htm::fixedadv_parser<
#if defined(NDEBUG)
        true
#else
        false
#endif
        >(pltext, host, project, visitor, author, coauthors);
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
PLTXT2HTM_C_NAME_MANGLING(plrichtext_parser)(char8_t const* const text, char8_t const* const project,
                                             char8_t const* const visitor, char8_t const* const author,
                                             char8_t const* const coauthors) noexcept -> char8_t const* {
    return ::pltxt2htm::plrichtext_parser<
#if defined(NDEBUG)
        true
#else
        false
#endif
        >(text, project, visitor, author, coauthors);
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
pltxt2htm_version_major() noexcept -> ::std::size_t {
    return ::pltxt2htm::version::major;
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
pltxt2htm_version_minor() noexcept -> ::std::size_t {
    return ::pltxt2htm::version::minor;
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
pltxt2htm_version_patch() noexcept -> ::std::size_t {
    return ::pltxt2htm::version::patch;
}
