#include <pltxt2htm/pltxt2htm.h>

extern "C"
char8_t const* common_parser(char8_t const* text) noexcept {
    return ::pltxt2htm::common_parser<
#ifdef NDEBUG
        true
#else
        false
#endif
    , true>(text);
}
