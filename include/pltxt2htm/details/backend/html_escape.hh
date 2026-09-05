/**
 * @file html_escape.hh
 * @brief HTML escaping helpers.
 */

#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/string.h>
#include "../../container/string_view.hh"
#include "../../contracts.hh"

namespace pltxt2htm::details {

/**
 * @brief Append one character, escaping characters with HTML syntax significance.
 */
constexpr void append_html_escaped_character(::fast_io::u8string& out, char8_t const character) noexcept {
    switch (character) {
    case u8'&': {
        out.append(u8"&amp;");
        return;
    }
    case u8'\'': {
        out.append(u8"&apos;");
        return;
    }
    case u8'"': {
        out.append(u8"&quot;");
        return;
    }
    case u8'<': {
        out.append(u8"&lt;");
        return;
    }
    case u8'>': {
        out.append(u8"&gt;");
        return;
    }
    default: {
        out.push_back(character);
        return;
    }
    }
}

/**
 * @brief Append a semantic attribute value with HTML escaping.
 * @details `value` is data, not an already HTML-escaped fragment. Every ampersand is
 *          therefore escaped, even when the following characters resemble a character reference.
 * @tparam ndebug Contract checking mode.
 * @param[out] result Output buffer receiving the escaped attribute value.
 * @param[in] value Semantic attribute value to append.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void append_html_escaped_attribute_value(::fast_io::u8string& result,
                                                   ::pltxt2htm::container::U8StringView value) noexcept {
    ::std::size_t const value_size{value.size()};
    for (::std::size_t index{}; index < value_size; ++index) {
        ::pltxt2htm::details::append_html_escaped_character(result, value.template index<ndebug>(index));
    }
}

} // namespace pltxt2htm::details
