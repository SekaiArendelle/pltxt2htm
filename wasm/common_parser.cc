#if __cpp_exceptions >= 199711L
    #error "Enable exception in wasm is not recommended"
#endif

#include <pltxt2htm/pltxt2htm.h>

extern "C" char8_t const* common_parser(char8_t const* text, char8_t const* const host) noexcept {
    return ::pltxt2htm::common_parser<
#ifdef NDEBUG
        true
#else
        false
#endif
        >(text, host);
}
