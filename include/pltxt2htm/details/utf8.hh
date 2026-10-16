/**
 * @file utf8.hh
 * @brief UTF-8 encoding and decoding primitives.
 */

#pragma once

#include <cstddef>

#include "../container/string.hh"
#include "../container/string_view.hh"
#include "../contracts.hh"

namespace pltxt2htm::details {

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
 * @brief Test whether a code point is a Unicode scalar value.
 * @details Unicode scalar values range from U+0000 through U+10FFFF, excluding
 *          the UTF-16 surrogate range U+D800 through U+DFFF.
 */
[[nodiscard]]
constexpr auto is_unicode_scalar_value(char32_t code_point) noexcept -> bool {
    return code_point <= char32_t{0x10FFFF} && (code_point < char32_t{0xD800} || char32_t{0xDFFF} < code_point);
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
 * @details Invalid input reports how many bytes belong to the invalid prefix. An empty view consumes zero bytes.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto decode_utf8_code_point(::pltxt2htm::container::U8StringView text) noexcept -> DecodeUtf8CodePointResult {
    ::std::size_t const text_size{text.size()};
    if (text.is_empty()) {
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
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void append_utf8_code_point(::pltxt2htm::container::U8String& result, char32_t code_point) noexcept {
    auto const encoded = ::pltxt2htm::details::encode_utf8_code_point(code_point);
    for (::std::size_t index{}; index < encoded.size; ++index) {
        result.push_back<ndebug>(encoded.code_units[index]);
    }
}

} // namespace pltxt2htm::details
