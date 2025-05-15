#include <pltxt2htm/pltxt2htm.h>

extern "C"
char8_t const* pltxt2html(char8_t const* text, bool is_inline = false) noexcept {
    return ::c_pltxt2html<
#ifdef NDEBUG
        true
#else
        false
#endif
    >(text, is_inline);
}
