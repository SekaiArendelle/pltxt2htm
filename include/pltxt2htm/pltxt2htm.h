#pragma once

#include <cstdlib>
#include <cstring>
#include <concepts>
#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/string_view.h>
#include "pltxt2htm.hh"

/**
 * @brief C-Style interface for pltxt2htm::pltxt2html
 */
template<bool ndebug = false>
constexpr char8_t const* c_pltxt2html(char8_t const* const text, bool is_inline = false) noexcept {
    auto html = ::pltxt2htm::pltxt2html(::fast_io::mnp::os_c_str(text), is_inline);
    char8_t* result = reinterpret_cast<char8_t*>(::std::malloc(html.size() + 1));
    if (result == nullptr) [[unlikely]] {
        // bad alloc error should never be an exception or err-code
        ::exception::terminate();
    }
    ::std::memcpy(result, html.data(), html.size());
    result[html.size()] = u8'\0';
    return result;
}
