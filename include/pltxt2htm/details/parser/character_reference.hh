/**
 * @file character_reference.hh
 * @brief Decode semicolon-terminated HTML character references.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <fast_io/fast_io_dsal/string.h>
#include "../../container/expected.hh"
#include "../../container/string_view.hh"
#include "../../contracts.hh"
#include "html_named_character_references.hh"

namespace pltxt2htm::details {

struct TryDecodeCharacterReferenceResult {
    ::std::size_t consumed_size;
    char32_t first_code_point;
    char32_t second_code_point;
    ::std::uint_least8_t code_point_count;
};

struct DecodedCharacterReference {
    char32_t first_code_point;
    char32_t second_code_point;
    ::std::uint_least8_t code_point_count;
};

[[nodiscard]]
constexpr auto remap_html_numeric_character_reference(char32_t code_point) noexcept -> char32_t {
    switch (code_point) {
    case char32_t{0x80}:
        return char32_t{0x20AC};
    case char32_t{0x82}:
        return char32_t{0x201A};
    case char32_t{0x83}:
        return char32_t{0x0192};
    case char32_t{0x84}:
        return char32_t{0x201E};
    case char32_t{0x85}:
        return char32_t{0x2026};
    case char32_t{0x86}:
        return char32_t{0x2020};
    case char32_t{0x87}:
        return char32_t{0x2021};
    case char32_t{0x88}:
        return char32_t{0x02C6};
    case char32_t{0x89}:
        return char32_t{0x2030};
    case char32_t{0x8A}:
        return char32_t{0x0160};
    case char32_t{0x8B}:
        return char32_t{0x2039};
    case char32_t{0x8C}:
        return char32_t{0x0152};
    case char32_t{0x8E}:
        return char32_t{0x017D};
    case char32_t{0x91}:
        return char32_t{0x2018};
    case char32_t{0x92}:
        return char32_t{0x2019};
    case char32_t{0x93}:
        return char32_t{0x201C};
    case char32_t{0x94}:
        return char32_t{0x201D};
    case char32_t{0x95}:
        return char32_t{0x2022};
    case char32_t{0x96}:
        return char32_t{0x2013};
    case char32_t{0x97}:
        return char32_t{0x2014};
    case char32_t{0x98}:
        return char32_t{0x02DC};
    case char32_t{0x99}:
        return char32_t{0x2122};
    case char32_t{0x9A}:
        return char32_t{0x0161};
    case char32_t{0x9B}:
        return char32_t{0x203A};
    case char32_t{0x9C}:
        return char32_t{0x0153};
    case char32_t{0x9E}:
        return char32_t{0x017E};
    case char32_t{0x9F}:
        return char32_t{0x0178};
    default:
        return code_point;
    }
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto compare_html_named_character_reference(::pltxt2htm::container::U8StringView name,
                                                      HtmlNamedCharacterReference const& candidate) noexcept -> int {
    ::std::size_t const name_size{name.size()};
    ::std::size_t const candidate_size{candidate.name_size};
    ::std::size_t const common_size{name_size < candidate_size ? name_size : candidate_size};
    for (::std::size_t index{}; index < common_size; ++index) {
        auto const lhs{name.template index<ndebug>(index)};
        auto const rhs{html_named_character_reference_names[candidate.name_offset + index]};
        if (lhs < rhs) {
            return -1;
        }
        if (lhs > rhs) {
            return 1;
        }
    }
    if (name_size < candidate_size) {
        return -1;
    }
    return name_size > candidate_size ? 1 : 0;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_find_html_named_character_reference(::pltxt2htm::container::U8StringView name) noexcept
    -> HtmlNamedCharacterReference const* {
    ::std::size_t first{};
    ::std::size_t last{sizeof(html_named_character_references) / sizeof(html_named_character_references[0])};
    while (first < last) {
        ::std::size_t const middle{first + (last - first) / 2};
        auto const comparison = ::pltxt2htm::details::compare_html_named_character_reference<ndebug>(
            name, html_named_character_references[middle]);
        if (comparison < 0) {
            last = middle;
        }
        else if (comparison > 0) {
            first = middle + 1;
        }
        else {
            return html_named_character_references + middle;
        }
    }
    return nullptr;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_decode_character_reference_value(::pltxt2htm::container::U8StringView value) noexcept
    -> ::pltxt2htm::container::Optional<DecodedCharacterReference> {
    ::std::size_t const value_size{value.size()};
    if (value.empty()) {
        return ::pltxt2htm::container::nullopt;
    }

    if (value.template index<ndebug>(0) == u8'#') {
        ::std::size_t index{1};
        bool const hexadecimal{index < value_size && (value.template index<ndebug>(index) == u8'x' ||
                                                      value.template index<ndebug>(index) == u8'X')};
        if (hexadecimal) {
            ++index;
        }
        ::std::size_t const digit_begin{index};
        char32_t code_point{};
        bool out_of_range{};
        char32_t const base{hexadecimal ? char32_t{16} : char32_t{10}};
        for (; index < value_size; ++index) {
            auto const chr{value.template index<ndebug>(index)};
            char32_t digit{};
            if (u8'0' <= chr && chr <= u8'9') {
                digit = static_cast<char32_t>(chr - u8'0');
            }
            else if (hexadecimal && u8'a' <= chr && chr <= u8'f') {
                digit = static_cast<char32_t>(chr - u8'a') + 10;
            }
            else if (hexadecimal && u8'A' <= chr && chr <= u8'F') {
                digit = static_cast<char32_t>(chr - u8'A') + 10;
            }
            else {
                return ::pltxt2htm::container::nullopt;
            }
            if (out_of_range == false) {
                if (code_point > (char32_t{0x10FFFF} - digit) / base) {
                    out_of_range = true;
                }
                else {
                    code_point = code_point * base + digit;
                }
            }
        }
        if (index == digit_begin) {
            return ::pltxt2htm::container::nullopt;
        }
        if (out_of_range || code_point == char32_t{} ||
            (char32_t{0xD800} <= code_point && code_point <= char32_t{0xDFFF})) {
            code_point = char32_t{0xFFFD};
        }
        else {
            code_point = ::pltxt2htm::details::remap_html_numeric_character_reference(code_point);
        }
        return DecodedCharacterReference{
            .first_code_point = code_point, .second_code_point = char32_t{}, .code_point_count = 1};
    }

    if (value_size > 31) {
        return ::pltxt2htm::container::nullopt;
    }
    for (::std::size_t index{}; index < value_size; ++index) {
        auto const chr{value.template index<ndebug>(index)};
        bool const alphanumeric{(u8'A' <= chr && chr <= u8'Z') || (u8'a' <= chr && chr <= u8'z') ||
                                (u8'0' <= chr && chr <= u8'9')};
        if (alphanumeric == false) {
            return ::pltxt2htm::container::nullopt;
        }
    }
    auto const* const entity = ::pltxt2htm::details::try_find_html_named_character_reference<ndebug>(value);
    if (entity == nullptr) {
        return ::pltxt2htm::container::nullopt;
    }
    return DecodedCharacterReference{
        .first_code_point = entity->first_code_point,
        .second_code_point = entity->second_code_point,
        .code_point_count = static_cast<::std::uint_least8_t>(entity->second_code_point == 0 ? 1 : 2)};
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_decode_character_reference(::pltxt2htm::container::U8StringView text) noexcept
    -> ::pltxt2htm::container::Optional<TryDecodeCharacterReferenceResult> {
    ::std::size_t const text_size{text.size()};
    if (text_size < 3 || text.template index<ndebug>(0) != u8'&') {
        return ::pltxt2htm::container::nullopt;
    }
    ::std::size_t index{1};
    bool const numeric{text.template index<ndebug>(index) == u8'#'};
    bool hexadecimal{};
    if (numeric) {
        ++index;
        if (index < text_size &&
            (text.template index<ndebug>(index) == u8'x' || text.template index<ndebug>(index) == u8'X')) {
            hexadecimal = true;
            ++index;
        }
    }
    ::std::size_t const content_begin{index};
    for (; index < text_size && text.template index<ndebug>(index) != u8';'; ++index) {
        auto const chr{text.template index<ndebug>(index)};
        bool const digit{u8'0' <= chr && chr <= u8'9'};
        bool const hex_alpha{hexadecimal && ((u8'a' <= chr && chr <= u8'f') || (u8'A' <= chr && chr <= u8'F'))};
        bool const named_alphanumeric{(u8'A' <= chr && chr <= u8'Z') || (u8'a' <= chr && chr <= u8'z') || digit};
        if ((numeric && digit == false && hex_alpha == false) ||
            (numeric == false && (named_alphanumeric == false || index > 31))) {
            return ::pltxt2htm::container::nullopt;
        }
    }
    if (index == content_begin || index >= text_size) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const decoded =
        ::pltxt2htm::details::try_decode_character_reference_value<ndebug>(text.template subview<ndebug>(1, index - 1));
    if (decoded.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const& value = decoded.template value<ndebug>();
    return TryDecodeCharacterReferenceResult{.consumed_size = index + 1,
                                             .first_code_point = value.first_code_point,
                                             .second_code_point = value.second_code_point,
                                             .code_point_count = value.code_point_count};
}

constexpr void append_utf8_code_point(::fast_io::u8string& result, char32_t code_point) noexcept {
    if (code_point < char32_t{0x80}) {
        result.push_back(static_cast<char8_t>(code_point));
        return;
    }
    if (code_point < char32_t{0x800}) {
        result.push_back(static_cast<char8_t>(0xC0 | static_cast<unsigned>(code_point >> 6)));
        result.push_back(static_cast<char8_t>(0x80 | static_cast<unsigned>(code_point & 0x3F)));
        return;
    }
    if (code_point < char32_t{0x10000}) {
        result.push_back(static_cast<char8_t>(0xE0 | static_cast<unsigned>(code_point >> 12)));
        result.push_back(static_cast<char8_t>(0x80 | static_cast<unsigned>((code_point >> 6) & 0x3F)));
        result.push_back(static_cast<char8_t>(0x80 | static_cast<unsigned>(code_point & 0x3F)));
        return;
    }
    result.push_back(static_cast<char8_t>(0xF0 | static_cast<unsigned>(code_point >> 18)));
    result.push_back(static_cast<char8_t>(0x80 | static_cast<unsigned>((code_point >> 12) & 0x3F)));
    result.push_back(static_cast<char8_t>(0x80 | static_cast<unsigned>((code_point >> 6) & 0x3F)));
    result.push_back(static_cast<char8_t>(0x80 | static_cast<unsigned>(code_point & 0x3F)));
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto decode_character_references(::pltxt2htm::container::U8StringView text) noexcept -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    ::std::size_t const text_size{text.size()};
    result.reserve(text_size);
    for (::std::size_t index{}; index < text_size; ++index) {
        if (text.template index<ndebug>(index) == u8'&') {
            auto const decoded =
                ::pltxt2htm::details::try_decode_character_reference<ndebug>(text.template subview<ndebug>(index));
            if (decoded.has_value()) {
                auto const& reference = decoded.template value<ndebug>();
                ::pltxt2htm::details::append_utf8_code_point(result, reference.first_code_point);
                if (reference.code_point_count == 2) {
                    ::pltxt2htm::details::append_utf8_code_point(result, reference.second_code_point);
                }
                index += reference.consumed_size - 1;
                continue;
            }
        }
        result.push_back(text.template index<ndebug>(index));
    }
    return result;
}

} // namespace pltxt2htm::details
