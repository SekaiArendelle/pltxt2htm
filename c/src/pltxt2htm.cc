#include <pltxt2htm/pltxt2htm.h>
#include "../include/push_macros.h"

extern "C"
#if __has_attribute(visibility)
    __attribute__((visibility("default")))
#endif
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
#if defined(_WIN32)
    __declspec(dllexport)
#endif // defined(_WIN32)
    auto PLTXT2HTM_C_NAME_MANGLING(common_parser)(char8_t const* pltext) /* throws */ -> char8_t const* {
    return ::pltxt2htm::common_parser<
#if defined(NDEBUG)
        true
#else
        false
#endif
        >(pltext);
}

extern "C"
#if __has_attribute(visibility)
    __attribute__((visibility("default")))
#endif
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
#if defined(_WIN32)
    __declspec(dllexport)
#endif // defined(_WIN32)
    auto PLTXT2HTM_C_NAME_MANGLING(advanced_parser)(char8_t const* pltext,
                                                    const char8_t* const host) /* throws */ -> char8_t const* {
    return ::pltxt2htm::advanced_parser<
#if defined(NDEBUG)
        true
#else
        false
#endif
        >(pltext, host);
}

extern "C"
#if __has_attribute(visibility)
    __attribute__((visibility("default")))
#endif
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
#if defined(_WIN32)
    __declspec(dllexport)
#endif // defined(_WIN32)
    auto PLTXT2HTM_C_NAME_MANGLING(fixedadv_parser)(char8_t const* pltext,
                                                    const char8_t* const host) /* throws */ -> char8_t const* {
    return ::pltxt2htm::fixedadv_parser<
#if defined(NDEBUG)
        true
#else
        false
#endif
        >(pltext, host);
}

extern "C"
#if __has_attribute(visibility)
    __attribute__((visibility("default")))
#endif
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
#if defined(_WIN32)
    __declspec(dllexport)
#endif // defined(_WIN32)
    auto get_version_major() noexcept -> ::std::size_t {
    return ::pltxt2htm::version::major;
}

extern "C"
#if __has_attribute(visibility)
    __attribute__((visibility("default")))
#endif
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
#if defined(_WIN32)
    __declspec(dllexport)
#endif // defined(_WIN32)
    auto get_version_minor() noexcept -> ::std::size_t {
    return ::pltxt2htm::version::minor;
}

extern "C"
#if __has_attribute(visibility)
    __attribute__((visibility("default")))
#endif
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
#if defined(_WIN32)
    __declspec(dllexport)
#endif // defined(_WIN32)
    auto get_version_patch() noexcept -> ::std::size_t {
    return ::pltxt2htm::version::patch;
}
