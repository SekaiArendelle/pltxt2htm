/**
 * @file html_url.hh
 * @brief HTML serialization helpers for semantic URL values.
 */

#pragma once

#include <cstddef>
#include "../../container/string.hh"
#include "../../container/string_view.hh"
#include "../../contracts.hh"

namespace pltxt2htm::details {

/**
 * @brief Append a semantic URL value with HTML escaping.
 * @details `url` is a URL value, not an already HTML-escaped fragment. Every ampersand is
 *          therefore escaped, even when the following characters resemble an entity reference.
 * @tparam ndebug Contract checking mode.
 * @param[out] result Output buffer receiving the escaped URL.
 * @param[in] url Semantic URL value to append.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void append_html_escaped_url(::pltxt2htm::container::U8String& result,
                                       ::pltxt2htm::container::U8StringView url) noexcept {
    ::std::size_t const url_size{url.size()};
    for (::std::size_t index{}; index < url_size; ++index) {
        auto const chr = url.template index<ndebug>(index);
        switch (chr) {
        case u8'&': {
            result.append(u8"&amp;");
            break;
        }
        case u8'\"': {
            result.append(u8"&quot;");
            break;
        }
        case u8'\'': {
            result.append(u8"&apos;");
            break;
        }
        case u8'<': {
            result.append(u8"&lt;");
            break;
        }
        case u8'>': {
            result.append(u8"&gt;");
            break;
        }
        default: {
            result.push_back(chr);
            break;
        }
        }
    }
}

} // namespace pltxt2htm::details
