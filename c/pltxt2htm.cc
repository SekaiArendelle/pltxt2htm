#include <pltxt2htm/pltxt2htm.h>

__attribute__((visibility("default"))) extern "C" char8_t const* common_parserd(char8_t const* pltext) noexcept {
    return ::pltxt2htm::common_parser<false>(pltext);
}

__attribute__((visibility("default"))) extern "C" char8_t const* common_parser(char8_t const* pltext) noexcept {
    return ::pltxt2htm::common_parser<true>(pltext);
}

__attribute__((visibility("default"))) extern "C" char8_t const* advanced_parserd(char8_t const* pltext,
                                                                                  const char8_t* const host) noexcept {
    return ::pltxt2htm::advanced_parser<false>(pltext, host);
}

__attribute__((visibility("default"))) extern "C" char8_t const* advanced_parser(char8_t const* pltext,
                                                                                 const char8_t* const host) noexcept {
    return ::pltxt2htm::advanced_parser<true>(pltext, host);
}

__attribute__((visibility("default"))) extern "C" char8_t const* fixedadv_parserd(char8_t const* pltext,
                                                                                  const char8_t* const host) noexcept {
    return ::pltxt2htm::fixedadv_parser<false>(pltext, host);
}

__attribute__((visibility("default"))) extern "C" char8_t const* fixedadv_parser(char8_t const* pltext,
                                                                                 const char8_t* const host) noexcept {
    return ::pltxt2htm::fixedadv_parser<true>(pltext, host);
}
