#include <pltxt2htm/pltxt2htm.h>

extern "C"
char8_t* pltxt2html(char8_t const* text) noexcept {
    return ::c_pltxt2html(text);
}
