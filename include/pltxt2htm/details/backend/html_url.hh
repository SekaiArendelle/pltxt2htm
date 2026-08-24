/**
 * @file html_url.hh
 * @brief HTML serialization helpers for semantic attribute values.
 */

#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/string.h>
#include "../../container/string_view.hh"
#include "../../contracts.hh"

namespace pltxt2htm::details {

/**
 * @brief Append a semantic attribute value with HTML escaping.
 * @details `value` is data, not an already HTML-escaped fragment. Every ampersand is
 *          therefore escaped, even when the following characters resemble a character reference.
 * @tparam ndebug Contract checking mode.
 * @param[out] result Output buffer receiving the escaped URL.
 * @param[in] value Semantic attribute value to append.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void append_html_attr_escaped(::fast_io::u8string& result,
                                        ::pltxt2htm::container::U8StringView value) noexcept {
    ::std::size_t const value_size{value.size()};
    for (::std::size_t index{}; index < value_size; ++index) {
        auto const chr{value.template index<ndebug>(index)};
        switch (chr) {
        case u8'&':
            result.append(u8"&amp;");
            break;
        case u8'\"':
            result.append(u8"&quot;");
            break;
        case u8'\'':
            result.append(u8"&apos;");
            break;
        case u8'<':
            result.append(u8"&lt;");
            break;
        case u8'>':
            result.append(u8"&gt;");
            break;
        default:
            result.push_back(chr);
            break;
        }
    }
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_html_escaped_url(::fast_io::u8string& result, ::pltxt2htm::container::U8StringView url) noexcept {
    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result, url);
}

} // namespace pltxt2htm::details
