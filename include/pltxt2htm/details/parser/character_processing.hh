/**
 * @file character_processing.hh
 * @brief Process UTF-8 code points, AST characters, and HTML character references.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <fast_io/fast_io_dsal/string.h>
#include "../../ast/ast.hh"
#include "../../container/expected.hh"
#include "../../container/string_view.hh"
#include "../../contracts.hh"
#include "html_named_character_references.hh"

namespace pltxt2htm::details {

struct DecodeUtf8CodePointResult {
    ::std::size_t consumed_size;
    char32_t code_point;
    bool valid;
};

struct EncodedUtf8CodePoint {
    char8_t code_units[4];
    ::std::uint_least8_t size;
};

[[nodiscard]]
constexpr auto is_unicode_scalar_value(char32_t code_point) noexcept -> bool {
    return code_point <= char32_t{0x10FFFF} && (code_point < char32_t{0xD800} || char32_t{0xDFFF} < code_point);
}

[[nodiscard]]
constexpr auto is_ascii_control_code_point(char32_t code_point) noexcept -> bool {
    return code_point <= char32_t{0x1F} || code_point == char32_t{0x7F};
}

/**
 * @brief Decode the first UTF-8 code point in a view.
 * @details Invalid input reports how many bytes belong to the invalid prefix, preserving
 *          the parser's existing recovery behavior. An empty view consumes zero bytes.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto decode_utf8_code_point(::pltxt2htm::container::U8StringView text) noexcept -> DecodeUtf8CodePointResult {
    ::std::size_t const text_size{text.size()};
    if (text.empty()) {
        return {.consumed_size = 0, .code_point = char32_t{}, .valid = false};
    }

    char8_t const first{text.template index<ndebug>(0)};
    if ((first & 0x80) == 0) {
        return {.consumed_size = 1, .code_point = static_cast<char32_t>(first), .valid = true};
    }

    if ((first & 0xE0) == 0xC0) {
        if (text_size < 2) {
            return {.consumed_size = 1, .code_point = char32_t{}, .valid = false};
        }
        char8_t const second{text.template index<ndebug>(1)};
        if ((second & 0xC0) != 0x80) {
            return {.consumed_size = 1, .code_point = char32_t{}, .valid = false};
        }
        char32_t const code_point{static_cast<char32_t>(first & 0x1F) << 6 | static_cast<char32_t>(second & 0x3F)};
        bool const valid{char32_t{0x80} <= code_point};
        return {.consumed_size = 2, .code_point = code_point, .valid = valid};
    }

    if ((first & 0xF0) == 0xE0) {
        if (text_size < 2) {
            return {.consumed_size = 1, .code_point = char32_t{}, .valid = false};
        }
        char8_t const second{text.template index<ndebug>(1)};
        if ((second & 0xC0) != 0x80) {
            return {.consumed_size = 1, .code_point = char32_t{}, .valid = false};
        }
        if (text_size < 3) {
            return {.consumed_size = 2, .code_point = char32_t{}, .valid = false};
        }
        char8_t const third{text.template index<ndebug>(2)};
        if ((third & 0xC0) != 0x80) {
            return {.consumed_size = 2, .code_point = char32_t{}, .valid = false};
        }
        char32_t const code_point{static_cast<char32_t>(first & 0x0F) << 12 |
                                  static_cast<char32_t>(second & 0x3F) << 6 | static_cast<char32_t>(third & 0x3F)};
        bool const valid{char32_t{0x800} <= code_point && ::pltxt2htm::details::is_unicode_scalar_value(code_point)};
        return {.consumed_size = 3, .code_point = code_point, .valid = valid};
    }

    if ((first & 0xF8) == 0xF0) {
        if (text_size < 2) {
            return {.consumed_size = 1, .code_point = char32_t{}, .valid = false};
        }
        char8_t const second{text.template index<ndebug>(1)};
        if ((second & 0xC0) != 0x80) {
            return {.consumed_size = 1, .code_point = char32_t{}, .valid = false};
        }
        if (text_size < 3) {
            return {.consumed_size = 2, .code_point = char32_t{}, .valid = false};
        }
        char8_t const third{text.template index<ndebug>(2)};
        if ((third & 0xC0) != 0x80) {
            return {.consumed_size = 2, .code_point = char32_t{}, .valid = false};
        }
        if (text_size < 4) {
            return {.consumed_size = 3, .code_point = char32_t{}, .valid = false};
        }
        char8_t const fourth{text.template index<ndebug>(3)};
        if ((fourth & 0xC0) != 0x80) {
            return {.consumed_size = 3, .code_point = char32_t{}, .valid = false};
        }
        char32_t const code_point{static_cast<char32_t>(first & 0x07) << 18 |
                                  static_cast<char32_t>(second & 0x3F) << 12 |
                                  static_cast<char32_t>(third & 0x3F) << 6 | static_cast<char32_t>(fourth & 0x3F)};
        bool const valid{char32_t{0x10000} <= code_point && ::pltxt2htm::details::is_unicode_scalar_value(code_point)};
        return {.consumed_size = 4, .code_point = code_point, .valid = valid};
    }

    return {.consumed_size = 1, .code_point = char32_t{}, .valid = false};
}

/**
 * @brief Encode one Unicode scalar value as UTF-8.
 * @return An empty encoding when `code_point` is not a Unicode scalar value.
 */
[[nodiscard]]
constexpr auto encode_utf8_code_point(char32_t code_point) noexcept -> EncodedUtf8CodePoint {
    EncodedUtf8CodePoint result{};
    if (::pltxt2htm::details::is_unicode_scalar_value(code_point) == false) {
        return result;
    }
    if (code_point < char32_t{0x80}) {
        result.code_units[0] = static_cast<char8_t>(code_point);
        result.size = 1;
        return result;
    }
    if (code_point < char32_t{0x800}) {
        result.code_units[0] = static_cast<char8_t>(0xC0 | static_cast<unsigned>(code_point >> 6));
        result.code_units[1] = static_cast<char8_t>(0x80 | static_cast<unsigned>(code_point & 0x3F));
        result.size = 2;
        return result;
    }
    if (code_point < char32_t{0x10000}) {
        result.code_units[0] = static_cast<char8_t>(0xE0 | static_cast<unsigned>(code_point >> 12));
        result.code_units[1] = static_cast<char8_t>(0x80 | static_cast<unsigned>((code_point >> 6) & 0x3F));
        result.code_units[2] = static_cast<char8_t>(0x80 | static_cast<unsigned>(code_point & 0x3F));
        result.size = 3;
        return result;
    }
    result.code_units[0] = static_cast<char8_t>(0xF0 | static_cast<unsigned>(code_point >> 18));
    result.code_units[1] = static_cast<char8_t>(0x80 | static_cast<unsigned>((code_point >> 12) & 0x3F));
    result.code_units[2] = static_cast<char8_t>(0x80 | static_cast<unsigned>((code_point >> 6) & 0x3F));
    result.code_units[3] = static_cast<char8_t>(0x80 | static_cast<unsigned>(code_point & 0x3F));
    result.size = 4;
    return result;
}

constexpr void append_utf8_code_point(::fast_io::u8string& result, char32_t code_point) noexcept {
    auto const encoded = ::pltxt2htm::details::encode_utf8_code_point(code_point);
    for (::std::size_t index{}; index < encoded.size; ++index) {
        result.push_back(encoded.code_units[index]);
    }
}

/**
 * @brief Parse one UTF-8 code point and append its original code units to an AST.
 * @details Parser-disallowed ASCII control characters and invalid UTF-8 prefixes append one
 *          InvalidUtf8 node. The returned size preserves the existing invalid-prefix recovery.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_utf8_code_point(::pltxt2htm::container::U8StringView text,
                                     ::pltxt2htm::Ast<ndebug>& result) noexcept -> ::std::size_t {
    char8_t const first{text.template index<ndebug>(0)};
    if (::pltxt2htm::details::is_ascii_control_code_point(static_cast<char32_t>(first))) {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidUtf8{}));
        return 1;
    }

    auto const decoded = ::pltxt2htm::details::decode_utf8_code_point<ndebug>(text);
    if (decoded.valid == false) {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidUtf8{}));
        return decoded.consumed_size;
    }
    for (::std::size_t index{}; index < decoded.consumed_size; ++index) {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{text.template index<ndebug>(index)}));
    }
    return decoded.consumed_size;
}

/**
 * @brief Append one semantic Unicode code point to an AST.
 * @details Characters with dedicated semantic nodes use those nodes. Other scalar values are
 *          encoded as UTF-8 and appended as U8Char nodes.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void append_code_point_to_ast(char32_t code_point, ::pltxt2htm::Ast<ndebug>& result) noexcept {
    switch (code_point) {
    case U'\n':
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LineBreak{}));
        return;
    case U' ':
    case char32_t{0xA0}:
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Space{}));
        return;
    case U'&':
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Ampersand{}));
        return;
    case U'\'':
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::SingleQuote{}));
        return;
    case U'"':
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::DoubleQuote{}));
        return;
    case U'<':
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
        return;
    case U'>':
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::GreaterThan{}));
        return;
    case U'\t':
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Tab{}));
        return;
    default:
        break;
    }

    if (::pltxt2htm::details::is_ascii_control_code_point(code_point)) {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidUtf8{}));
        return;
    }

    auto const encoded = ::pltxt2htm::details::encode_utf8_code_point(code_point);
    if (encoded.size == 0) {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidUtf8{}));
        return;
    }
    for (::std::size_t index{}; index < encoded.size; ++index) {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{encoded.code_units[index]}));
    }
}

constexpr void append_character_reference_code_point(::fast_io::u8string& result, char32_t code_point) noexcept {
    if (::pltxt2htm::details::is_ascii_control_code_point(code_point)) {
        code_point = char32_t{0xFFFD};
    }
    ::pltxt2htm::details::append_utf8_code_point(result, code_point);
}

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
            ::pltxt2htm::details::is_unicode_scalar_value(code_point) == false) {
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
                ::pltxt2htm::details::append_character_reference_code_point(result, reference.first_code_point);
                if (reference.code_point_count == 2) {
                    ::pltxt2htm::details::append_character_reference_code_point(result, reference.second_code_point);
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
