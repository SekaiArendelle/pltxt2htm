/**
 * @file character_processing.hh
 * @brief Process UTF-8 code points, AST characters, and HTML character references.
 */

#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/string.h>
#include "../../ast/ast.hh"
#include "../../contracts.hh"
#include "../../container/string_view.hh"
#include "html_named_character_references.hh"

namespace pltxt2htm::details {

/**
 * @brief Test whether a code point is a Unicode scalar value.
 * @details Unicode scalar values range from U+0000 through U+10FFFF, excluding
 *          the UTF-16 surrogate range U+D800 through U+DFFF.
 * @param code_point Code point to test.
 * @return `true` when the code point is a Unicode scalar value.
 */
[[nodiscard]]
constexpr auto is_unicode_scalar_value(char32_t code_point) noexcept -> bool {
    return code_point <= char32_t{0x10FFFF} && (code_point < char32_t{0xD800} || char32_t{0xDFFF} < code_point);
}

/**
 * @brief Test whether a code point is an ASCII control character.
 * @param code_point Code point to test.
 * @return `true` for U+0000 through U+001F and U+007F.
 */
[[nodiscard]]
constexpr auto is_ascii_control_code_point(char32_t code_point) noexcept -> bool {
    return code_point <= char32_t{0x1F} || code_point == char32_t{0x7F};
}

/**
 * @brief Result of decoding the first UTF-8 sequence in a view.
 */
struct DecodeUtf8CodePointResult {
    /** Number of input code units consumed by the valid sequence or invalid prefix. */
    ::std::size_t consumed_size;
    /** Decoded scalar value, or zero when decoding failed. */
    char32_t code_point;
    /** Whether the input begins with a valid, minimally encoded UTF-8 scalar value. */
    bool valid;
};

/**
 * @brief Decode the first UTF-8 code point in a view.
 * @details Invalid input reports how many bytes belong to the invalid prefix, preserving
 *          the parser's existing recovery behavior. An empty view consumes zero bytes.
 * @tparam ndebug Contract checking mode used for indexed access.
 * @param text Input view whose first UTF-8 sequence is decoded.
 * @return The decoded scalar value, validity flag, and number of consumed code units.
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
 * @brief Fixed-capacity result of encoding one Unicode scalar value as UTF-8.
 */
struct EncodedUtf8CodePoint {
    /** Encoded UTF-8 code units; only the first `size` entries are active. */
    char8_t code_units[4];
    /** Number of active code units, or zero when the input was not a scalar value. */
    unsigned size;
};

/**
 * @brief Encode one Unicode scalar value as UTF-8.
 * @param code_point Code point to encode.
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

/**
 * @brief Append a Unicode scalar value to a UTF-8 string.
 * @details An invalid scalar value produces no output.
 * @param[out] result Output string receiving the encoded code units.
 * @param code_point Code point to encode and append.
 */
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
 * @tparam ndebug Contract checking mode used for input and AST access.
 * @param text Non-empty input view beginning at the code point to parse.
 * @param[out] result AST receiving character nodes or one InvalidUtf8 node.
 * @return Number of consumed UTF-8 code units.
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
 *          encoded as UTF-8 and appended as U8Char nodes. ASCII controls and invalid scalar
 *          values append one InvalidUtf8 node.
 * @tparam ndebug Contract checking mode used for AST operations.
 * @param code_point Semantic code point to append.
 * @param[out] result AST receiving the corresponding node or nodes.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void append_code_point_to_ast(char32_t code_point, ::pltxt2htm::Ast<ndebug>& result) noexcept {
    switch (code_point) {
    case U'\n': {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LineBreak{}));
        return;
    }
    case U' ':
    case char32_t{0xA0}: {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Space{}));
        return;
    }
    case U'&': {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Ampersand{}));
        return;
    }
    case U'\'': {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::SingleQuote{}));
        return;
    }
    case U'"': {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::DoubleQuote{}));
        return;
    }
    case U'<': {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
        return;
    }
    case U'>': {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::GreaterThan{}));
        return;
    }
    case U'\t': {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Tab{}));
        return;
    }
    default: {
        break;
    }
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

/**
 * @brief Append a decoded character-reference code point to a UTF-8 string.
 * @details ASCII controls are normalized to U+FFFD before encoding. Character-reference
 *          decoding guarantees that every other input is a Unicode scalar value.
 * @param[out] result Output string receiving the encoded code point.
 * @param code_point Decoded character-reference code point.
 */
constexpr void append_character_reference_code_point(::fast_io::u8string& result, char32_t code_point) noexcept {
    if (::pltxt2htm::details::is_ascii_control_code_point(code_point)) {
        code_point = char32_t{0xFFFD};
    }
    ::pltxt2htm::details::append_utf8_code_point(result, code_point);
}

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
 * @brief Append a decoded character reference to an AST.
 * @tparam ndebug Contract checking mode used for AST operations.
 * @param reference Previously decoded one- or two-code-point reference.
 * @param[out] result AST receiving the semantic character nodes.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void append_character_reference_to_ast(TryDecodeCharacterReferenceResult const& reference,
                                                 ::pltxt2htm::Ast<ndebug>& result) noexcept {
    ::pltxt2htm::details::append_code_point_to_ast<ndebug>(reference.first_code_point, result);
    if (reference.has_second_code_point()) {
        ::pltxt2htm::details::append_code_point_to_ast<ndebug>(reference.second_code_point, result);
    }
}

/**
 * @brief Decode and append the character reference at the start of a view.
 * @tparam ndebug Contract checking mode used for parsing and AST operations.
 * @param text Input view expected to begin with a character reference.
 * @param[out] result AST receiving decoded semantic character nodes on success.
 * @return Consumed input size, or nullopt when `text` does not begin with a supported reference.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_append_character_reference(::pltxt2htm::container::U8StringView text,
                                              ::pltxt2htm::Ast<ndebug>& result) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    auto const reference = ::pltxt2htm::details::try_decode_character_reference<ndebug>(text);
    if (reference.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const& decoded = reference.template value<ndebug>();
    ::pltxt2htm::details::append_character_reference_to_ast<ndebug>(decoded, result);
    return decoded.consumed_size;
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
                if (reference.has_second_code_point()) {
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
