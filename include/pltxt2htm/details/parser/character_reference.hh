/**
 * @file character_reference.hh
 * @brief Decode HTML character references without AST dependencies.
 */

#pragma once

#include <cstddef>
#include "../../container/optional.hh"
#include "../../container/string.hh"
#include "../../container/string_view.hh"
#include "../../contracts.hh"
#include "../utf8.hh"
#include "html_named_character_references.hh"

namespace pltxt2htm::details {

/**
 * @brief Apply HTML's legacy numeric-character-reference replacements.
 * @details HTML maps selected C1 control values to the corresponding Windows-1252
 *          characters for compatibility with legacy content. Values absent from the
 *          replacement table are returned unchanged.
 * @param code_point Valid numeric character-reference code point.
 * @return The HTML replacement code point, or `code_point` when no replacement applies.
 */
[[nodiscard]]
constexpr auto remap_html_numeric_character_reference(char32_t code_point) noexcept -> char32_t {
    switch (code_point) {
    case char32_t{0x80}: {
        return char32_t{0x20AC};
    }
    case char32_t{0x82}: {
        return char32_t{0x201A};
    }
    case char32_t{0x83}: {
        return char32_t{0x0192};
    }
    case char32_t{0x84}: {
        return char32_t{0x201E};
    }
    case char32_t{0x85}: {
        return char32_t{0x2026};
    }
    case char32_t{0x86}: {
        return char32_t{0x2020};
    }
    case char32_t{0x87}: {
        return char32_t{0x2021};
    }
    case char32_t{0x88}: {
        return char32_t{0x02C6};
    }
    case char32_t{0x89}: {
        return char32_t{0x2030};
    }
    case char32_t{0x8A}: {
        return char32_t{0x0160};
    }
    case char32_t{0x8B}: {
        return char32_t{0x2039};
    }
    case char32_t{0x8C}: {
        return char32_t{0x0152};
    }
    case char32_t{0x8E}: {
        return char32_t{0x017D};
    }
    case char32_t{0x91}: {
        return char32_t{0x2018};
    }
    case char32_t{0x92}: {
        return char32_t{0x2019};
    }
    case char32_t{0x93}: {
        return char32_t{0x201C};
    }
    case char32_t{0x94}: {
        return char32_t{0x201D};
    }
    case char32_t{0x95}: {
        return char32_t{0x2022};
    }
    case char32_t{0x96}: {
        return char32_t{0x2013};
    }
    case char32_t{0x97}: {
        return char32_t{0x2014};
    }
    case char32_t{0x98}: {
        return char32_t{0x02DC};
    }
    case char32_t{0x99}: {
        return char32_t{0x2122};
    }
    case char32_t{0x9A}: {
        return char32_t{0x0161};
    }
    case char32_t{0x9B}: {
        return char32_t{0x203A};
    }
    case char32_t{0x9C}: {
        return char32_t{0x0153};
    }
    case char32_t{0x9E}: {
        return char32_t{0x017E};
    }
    case char32_t{0x9F}: {
        return char32_t{0x0178};
    }
    default: {
        return code_point;
    }
    }
}

/**
 * @brief A successfully decoded, semicolon-terminated HTML character reference.
 */
struct TryDecodeCharacterReferenceResult {
    /** Number of input UTF-8 code units consumed, including the leading `&` and trailing `;`. */
    ::std::size_t consumed_size;
    /** The first decoded Unicode code point, which is always present. */
    char32_t first_code_point;
    /** The second decoded Unicode code point, or zero when absent. */
    char32_t second_code_point;

    /** @return Whether this reference decodes to a second Unicode code point. */
    [[nodiscard]]
    constexpr auto has_second_code_point(this TryDecodeCharacterReferenceResult const& self) noexcept -> bool {
        return self.second_code_point != char32_t{};
    }
};

/**
 * @brief Decode one semicolon-terminated HTML character reference.
 * @details Decimal and hexadecimal numeric references follow HTML replacement rules:
 *          zero, overflow, and non-scalar values become U+FFFD, while selected C1 values
 *          receive the legacy Windows-1252 mapping. Named references must exactly match
 *          an entry in HtmlNamedCharacterReferenceTable. This parser intentionally rejects
 *          the legacy semicolon-omission forms.
 * @tparam ndebug Contract checking mode used for input and table access.
 * @param text Input view beginning with `&`.
 * @return Decoded code points and consumed size, or nullopt when no complete valid reference begins at `text`.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_decode_character_reference(::pltxt2htm::container::U8StringView text) noexcept
    -> ::pltxt2htm::container::Optional<TryDecodeCharacterReferenceResult> {
    ::std::size_t const text_size{text.size()};
    if (text_size < 3 || text.template index<ndebug>(0) != u8'&') {
        return ::pltxt2htm::container::nullopt;
    }

    if (text.template index<ndebug>(1) == u8'#') {
        ::std::size_t index{2};
        bool hexadecimal{};
        if (index < text_size &&
            (text.template index<ndebug>(index) == u8'x' || text.template index<ndebug>(index) == u8'X')) {
            hexadecimal = true;
            ++index;
        }
        ::std::size_t const digit_begin{index};
        char32_t code_point{};
        bool out_of_range{};
        char32_t const base{hexadecimal ? char32_t{16} : char32_t{10}};
        for (; index < text_size && text.template index<ndebug>(index) != u8';'; ++index) {
            auto const chr = text.template index<ndebug>(index);
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
        if (index == digit_begin || index >= text_size) {
            return ::pltxt2htm::container::nullopt;
        }
        if (out_of_range || code_point == char32_t{} ||
            ::pltxt2htm::details::is_unicode_scalar_value(code_point) == false) {
            code_point = char32_t{0xFFFD};
        }
        else {
            code_point = ::pltxt2htm::details::remap_html_numeric_character_reference(code_point);
        }
        return TryDecodeCharacterReferenceResult{
            .consumed_size = index + 1, .first_code_point = code_point, .second_code_point = char32_t{}};
    }

    ::std::size_t index{1};
    for (; index < text_size && text.template index<ndebug>(index) != u8';'; ++index) {
        auto const chr = text.template index<ndebug>(index);
        bool const alphanumeric{(u8'A' <= chr && chr <= u8'Z') || (u8'a' <= chr && chr <= u8'z') ||
                                (u8'0' <= chr && chr <= u8'9')};
        if (alphanumeric == false || index > 31) {
            return ::pltxt2htm::container::nullopt;
        }
    }
    if (index == 1 || index >= text_size) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const name = text.template subview<ndebug>(1, index - 1);
    auto const* const entity = ::pltxt2htm::details::HtmlNamedCharacterReferenceTable::try_find<ndebug>(name);
    if (entity == nullptr) {
        return ::pltxt2htm::container::nullopt;
    }
    return TryDecodeCharacterReferenceResult{.consumed_size = index + 1,
                                             .first_code_point = entity->first_code_point,
                                             .second_code_point = entity->second_code_point};
}

/**
 * @brief Append a decoded character-reference code point to a UTF-8 string.
 * @details ASCII controls are normalized to U+FFFD before encoding. Character-reference
 *          decoding guarantees that every other input is a Unicode scalar value.
 * @tparam ndebug Contract checking mode.
 * @param[out] result Output string receiving the encoded code point.
 * @param code_point Decoded character-reference code point.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void append_character_reference_code_point(::pltxt2htm::container::U8String& result,
                                                     char32_t code_point) noexcept {
    if (code_point <= char32_t{0x1F} || code_point == char32_t{0x7F}) {
        code_point = char32_t{0xFFFD};
    }
    ::pltxt2htm::details::append_utf8_code_point<ndebug>(result, code_point);
}

/**
 * @brief Decode every supported HTML character reference in a string.
 * @details Unknown, malformed, and unterminated references are copied literally. Decoded
 *          ASCII controls are emitted as U+FFFD to match parser output normalization.
 * @tparam ndebug Contract checking mode used for input and reference-table access.
 * @param text Input text that may contain character references.
 * @return UTF-8 text with all supported references decoded.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto decode_character_references(::pltxt2htm::container::U8StringView text) noexcept
    -> ::pltxt2htm::container::U8String {
    ::pltxt2htm::container::U8String result{};
    ::std::size_t const text_size{text.size()};
    result.template reserve<ndebug>(text_size);
    for (::std::size_t index{}; index < text_size;) {
        if (text.template index<ndebug>(index) == u8'&') {
            auto const decoded =
                ::pltxt2htm::details::try_decode_character_reference<ndebug>(text.template subview<ndebug>(index));
            if (decoded.has_value()) {
                auto const& reference = decoded.template value<ndebug>();
                ::pltxt2htm::details::append_character_reference_code_point<ndebug>(result, reference.first_code_point);
                if (reference.has_second_code_point()) {
                    ::pltxt2htm::details::append_character_reference_code_point<ndebug>(result,
                                                                                        reference.second_code_point);
                }
                index += reference.consumed_size;
                continue;
            }
        }
        result.push_back<ndebug>(text.template index<ndebug>(index));
        ++index;
    }
    return result;
}

} // namespace pltxt2htm::details
