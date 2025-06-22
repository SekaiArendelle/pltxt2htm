#include <pltxt2htm/pltxt2htm.h>

#if defined(__GNUC__) || defined(__clang__)
__attribute__((visibility("default")))
#elif defined(_MSC_VER) && !defined(__clang__)
__declspec(dllexport)
#endif
extern "C" char8_t const* advanced_parser(char8_t const* const text, char8_t const* const host) noexcept {
    return ::pltxt2htm::advanced_parser<
#ifdef NDEBUG
        true
#else
        false
#endif
        >(text, host);
}

#if defined(__GNUC__) || defined(__clang__)
__attribute__((visibility("default")))
#elif defined(_MSC_VER) && !defined(__clang__)
__declspec(dllexport)
#endif
extern "C" char8_t const* common_parser(char8_t const* text, char8_t const* const host) noexcept {
    return ::pltxt2htm::common_parser<
#ifdef NDEBUG
        true
#else
        false
#endif
        >(text, host);
}
