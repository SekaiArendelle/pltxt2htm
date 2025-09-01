#include <pltxt2htm/pltxt2htm.h>

#if __has_attribute(visibility)
__attribute__((visibility("default")))
#endif
extern "C"
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
    auto common_parserd(char8_t const* pltext) /* throws */ -> char8_t const* {
    return ::pltxt2htm::common_parser<false>(pltext);
}

#if __has_attribute(visibility)
__attribute__((visibility("default")))
#endif
extern "C"
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
    auto common_parser(char8_t const* pltext) /* throws */ -> char8_t const* {
    return ::pltxt2htm::common_parser<true>(pltext);
}

#if __has_attribute(visibility)
__attribute__((visibility("default")))
#endif
extern "C"
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
    auto advanced_parserd(char8_t const* pltext, const char8_t* const host) /* throws */ -> char8_t const* {
    return ::pltxt2htm::advanced_parser<false>(pltext, host);
}

#if __has_attribute(visibility)
__attribute__((visibility("default")))
#endif
extern "C"
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
    auto advanced_parser(char8_t const* pltext, const char8_t* const host) /* throws */ -> char8_t const* {
    return ::pltxt2htm::advanced_parser<true>(pltext, host);
}

#if __has_attribute(visibility)
__attribute__((visibility("default")))
#endif
extern "C"
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
    auto fixedadv_parserd(char8_t const* pltext, const char8_t* const host) /* throws */ -> char8_t const* {
    return ::pltxt2htm::fixedadv_parser<false>(pltext, host);
}

#if __has_attribute(visibility)
__attribute__((visibility("default")))
#endif
extern "C"
#if __has_cpp_attribute(__gnu__::__used__)
    [[__gnu__::__used__]]
#endif
    auto fixedadv_parser(char8_t const* pltext, const char8_t* const host) /* throws */ -> char8_t const* {
    return ::pltxt2htm::fixedadv_parser<true>(pltext, host);
}
