/**
 * @file html_escape.hh
 * @brief HTML escaping helpers.
 */

#pragma once

#include <cstddef>
#include "../../ast/plain_text.hh"
#include "../../container/string.hh"
#include "../../container/string_view.hh"
#include "../../contracts.hh"

namespace pltxt2htm::details {

/**
 * @brief Append one character, escaping characters with HTML syntax significance.
 * Contract mode follows PLTXT2HTM_CONTRACTS.
 */
constexpr void append_html_escaped_character(::pltxt2htm::container::U8String& out, char8_t const character) noexcept {
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
 * Contract mode follows PLTXT2HTM_CONTRACTS.
 * @param[out] result Output buffer receiving the escaped attribute value.
 * @param[in] value Semantic attribute value to append.
 */
constexpr void append_html_escaped_attribute_value(::pltxt2htm::container::U8String& result,
                                                   ::pltxt2htm::container::U8StringView value) noexcept {
    ::std::size_t const value_size{value.size()};
    for (::std::size_t index{}; index < value_size; ++index) {
        ::pltxt2htm::details::append_html_escaped_character(result, value.index(index));
    }
}

/**
 * @brief Append semantic plain text as a canonical HTML attribute value.
 */
constexpr void append_plain_text_as_html_attribute(::pltxt2htm::container::U8String& result,
                                                   ::pltxt2htm::PlainText const& value) noexcept {
    auto const& text = value.as_string();
    ::std::size_t const text_size{text.size()};
    for (::std::size_t index{}; index < text_size; ++index) {
        char8_t const character{text.index(index)};
        if (character == char8_t{0xC2} && index + 1 < text_size &&
            text.index(index + 1) == char8_t{0xA0}) {
            result.append(u8"&nbsp;");
            ++index;
            continue;
        }
        if (character == u8'\t') {
            result.append(u8"&nbsp;&nbsp;&nbsp;&nbsp;");
            continue;
        }
        ::pltxt2htm::details::append_html_escaped_character(result, character);
    }
}

} // namespace pltxt2htm::details
