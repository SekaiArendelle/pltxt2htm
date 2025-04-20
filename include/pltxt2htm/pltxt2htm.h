#pragma once

#include <cstdlib>
#include <cstring>
#include "pltxt2htm.hh"

/**
 * @brief C interface for pltxt2htm::pltxt2html
 */
extern "C"
inline char8_t* c_pltxt2html(char8_t const*const text) noexcept {
    auto html = pltxt2htm::pltxt2html(::fast_io::mnp::basic_os_c_str<char8_t>(text));
    char8_t* result = reinterpret_cast<char8_t*>(::std::malloc(html.size() + 1));
    ::std::memcpy(result, html.data(), html.size());
    result[html.size()] = u8'\0';
    return result;
}
