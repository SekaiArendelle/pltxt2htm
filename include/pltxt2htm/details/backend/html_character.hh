/**
 * @file html_character.hh
 * @brief HTML character escaping helpers.
 */

#pragma once

#include <fast_io/fast_io_dsal/string.h>

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

} // namespace pltxt2htm::details
