/**
 * @file common.hh
 * @brief Shared code-syntax cursor helpers.
 */

#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../../utils.hh"

namespace pltxt2htm::details {

class ParsedCodeSyntaxUnit {
public:
    ::std::size_t advance_count{};
    char8_t ascii{};
};

constexpr void append_invalid_code_point(::fast_io::u8string& destination) noexcept {
    destination.append(u8"\uFFFD");
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_code_syntax_unit(::fast_io::u8string_view const input, ::fast_io::u8string& destination) noexcept
    -> ParsedCodeSyntaxUnit {
    ::std::size_t const input_size{input.size()};
    char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(input, 0)};
    if (chr == u8'\n' || chr == u8'\t' || (chr > 0x1f && chr != 0x7f && (chr & 0x80) == 0)) {
        destination.push_back(chr);
        return {.advance_count = 1, .ascii = chr};
    }
    if (chr <= 0x1f || chr == 0x7f) {
        ::pltxt2htm::details::append_invalid_code_point(destination);
        return {.advance_count = 1, .ascii = char8_t{}};
    }

    ::std::size_t count{};
    char32_t value{};
    char32_t minimum{};
    if ((chr & 0xE0) == 0xC0) {
        count = 2;
        value = chr & 0x1F;
        minimum = 0x80;
    }
    else if ((chr & 0xF0) == 0xE0) {
        count = 3;
        value = chr & 0x0F;
        minimum = 0x800;
    }
    else if ((chr & 0xF8) == 0xF0) {
        count = 4;
        value = chr & 0x07;
        minimum = 0x10000;
    }
    else {
        ::pltxt2htm::details::append_invalid_code_point(destination);
        return {.advance_count = 1, .ascii = char8_t{}};
    }

    ::std::size_t consumed{1};
    while (consumed != count && consumed != input_size) {
        char8_t const continuation{::pltxt2htm::details::u8string_view_index<ndebug>(input, consumed)};
        if ((continuation & 0xC0) != 0x80) {
            break;
        }
        value = value << 6 | static_cast<char32_t>(continuation & 0x3F);
        ++consumed;
    }
    if (consumed != count || value < minimum || value > 0x10FFFF || (0xD800 <= value && value <= 0xDFFF)) {
        ::pltxt2htm::details::append_invalid_code_point(destination);
        return {.advance_count = consumed, .ascii = char8_t{}};
    }
    for (::std::size_t index{}; index != count; ++index) {
        destination.push_back(::pltxt2htm::details::u8string_view_index<ndebug>(input, index));
    }
    return {.advance_count = count, .ascii = char8_t{}};
}

constexpr void append_code_syntax_text(::fast_io::u8string& source, ::fast_io::u8string& destination) noexcept {
    destination.append(source);
    source.clear();
}

} // namespace pltxt2htm::details
