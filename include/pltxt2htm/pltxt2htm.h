#pragma once

#include <cstdlib>
#include <cstring>
#include <exception/exception.hh>
#include "pltxt2htm.hh"

/**
 * @brief C-Style interface for pltxt2htm::pltxt2html
 */
template<bool ndebug = false>
inline char8_t* c_pltxt2html(char8_t const* const text) noexcept {
    auto html = ::pltxt2htm::pltxt2html(::fast_io::mnp::basic_os_c_str<char8_t>(text));
    char8_t* result = reinterpret_cast<char8_t*>(::std::malloc(html.size() + 1));
    if (result == nullptr) [[unlikely]] {
        ::exception::terminate();
    }
    ::std::memcpy(result, html.data(), html.size());
    result[html.size()] = u8'\0';
    return result;
}
