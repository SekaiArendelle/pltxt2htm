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
    extern "C" PLTXT2HTM_C_SRC_VISIBILITY_DEFAULT PLTXT2HTM_C_SRC_USED PLTXT2HTM_C_SRC_WIN32_DLLEXPORT

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
auto PLTXT2HTM_C_NAME_MANGLING(common_parser)(char8_t const* const pltext) /* throws */ -> char8_t const* {
    return ::pltxt2htm::common_parser<
#if defined(NDEBUG)
        true
#else
        false
#endif
        >(pltext);
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
auto PLTXT2HTM_C_NAME_MANGLING(advanced_parser)(char8_t const* const pltext,
                                                char8_t const* const host) /* throws */ -> char8_t const* {
    return ::pltxt2htm::advanced_parser<
#if defined(NDEBUG)
        true
#else
        false
#endif
        >(pltext, host);
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
auto PLTXT2HTM_C_NAME_MANGLING(fixedadv_parser)(char8_t const* const pltext,
                                                char8_t const* const host) /* throws */ -> char8_t const* {
    return ::pltxt2htm::fixedadv_parser<
#if defined(NDEBUG)
        true
#else
        false
#endif
        >(pltext, host);
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
auto pltxt2htm_version_major() noexcept -> ::std::size_t {
    return ::pltxt2htm::version::major;
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
auto pltxt2htm_version_minor() noexcept -> ::std::size_t {
    return ::pltxt2htm::version::minor;
}

PLTXT2HTM_C_SRC_EXPORT_FUNCTION
auto pltxt2htm_version_patch() noexcept -> ::std::size_t {
    return ::pltxt2htm::version::patch;
}
