/**
 * @file try_parse.hh
 * @brief Low-level parser helpers for probing specific Physics-Lab / Markdown token patterns.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "frame_concext.hh"
#include "../utils.hh"
#include "../../contracts.hh"
#include "../../heap_guard.hh"
#include "../../astnode/basic.hh"
#include "../../astnode/node_type.hh"
#include "../../astnode/html_node.hh"
#include "../../astnode/markdown_node.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Switch to a markdown punctuation character for escape sequences.
 *
 * This function maps escape character sequences (prefixed with backslash) to their corresponding
 * AST node types. It handles all common markdown punctuation characters that can be escaped.
 *
 * @param[in] u8char The UTF-8 character following the backslash in an escape sequence.
 * @return An optional HeapGuard containing the corresponding escape PlTxtNode, or nullopt if the character
 *         is not a valid escapable character in markdown.
 * @note Supported escape characters include: \\ \! \" \# \$ \% \& \' \( \) \* \+ \, \- \. \/ \: \; \< \= \> \? \@ \[ \]
 * \^ \_ \` \{ \| \} \~
 * @see https://spec.commonmark.org/0.31.2/#backslash-escapes
 */
[[nodiscard]]
constexpr ::exception::optional<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>> switch_escape_char(
    char8_t u8char) noexcept {
    switch (u8char) {
    case u8'\\': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeBackslash>{});
    }
    case u8'!': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeExclamation>{});
    }
    case u8'\"': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeDoubleQuote>{});
    }
    case u8'#': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeHash>{});
    }
    case u8'$': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeDollar>{});
    }
    case u8'%': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapePercent>{});
    }
    case u8'&': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeAmpersand>{});
    }
    case u8'\'': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeSingleQuote>{});
    }
    case u8'(': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeLeftParen>{});
    }
    case u8')': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeRightParen>{});
    }
    case u8'*': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeAsterisk>{});
    }
    case u8'+': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapePlus>{});
    }
    case u8',': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeComma>{});
    }
    case u8'-': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeHyphen>{});
    }
    case u8'.': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeDot>{});
    }
    case u8'/': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeSlash>{});
    }
    case u8':': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeColon>{});
    }
    case u8';': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeSemicolon>{});
    }
    case u8'<': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeLessThan>{});
    }
    case u8'=': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeEquals>{});
    }
    case u8'>': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeGreaterThan>{});
    }
    case u8'?': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeQuestion>{});
    }
    case u8'@': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeAt>{});
    }
    case u8'[': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeLeftBracket>{});
    }
    case u8']': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeRightBracket>{});
    }
    case u8'^': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeCaret>{});
    }
    case u8'_': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeUnderscore>{});
    }
    case u8'`': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeBacktick>{});
    }
    case u8'{': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeLeftBrace>{});
    }
    case u8'|': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapePipe>{});
    }
    case u8'}': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeRightBrace>{});
    }
    case u8'~': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::MdEscapeTilde>{});
    }
    default:
        return ::exception::nullopt_t{};
    }
}

/**
 * @brief Parse a single UTF-8 code point and append the corresponding AST node(s).
 *
 * This function inspects the first byte of `pltext` and appends either UTF-8 bytes
 * (as U8Char nodes) or one InvalidU8Char node to `result`.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext Input view starting at the current parser position.
 * @param[out] result The AST to which parsed character nodes are appended.
 * @return Number of additional bytes consumed after the first byte (0..3).
 *         The caller should advance by `return_value + 1`.
 * @note ASCII bytes append one U8Char and return 0.
 * @note Control characters 0x00-0x1F and 0x7F are ignored (no node appended, return 0).
 * @note Invalid sequences append one InvalidU8Char. The return value may be non-zero when
 *       continuation bytes are consumed as part of one invalid sequence.
 * @see https://en.wikipedia.org/wiki/UTF-8
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_utf8_code_point(::fast_io::u8string_view const& pltext, ::pltxt2htm::Ast& result) noexcept
    -> ::std::size_t {
    ::std::size_t const pltext_size{pltext.size()};
    char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 0)};

    if (chr <= 0x1f || chr == 0x7f) {
        return 0;
    }
    if ((chr & 0x80) == 0) {
        // normal utf-8 characters
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{chr});
        return 0;
    }
    else if ((chr & 0xE0) == 0xC0) {
        if (1 >= pltext_size) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 0;
        }
        auto next_char = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1);
        if ((next_char & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 0;
        }
        char32_t combine{static_cast<char32_t>(chr & 0x1F) << 6 | static_cast<char32_t>(next_char & 0x3F)};
        if (combine < 0x80 || combine > 0x7FF) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 1;
        }

        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{chr});
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{next_char});
        return 1;
    }
    else if ((chr & 0xF0) == 0xE0) {
        if (2 >= pltext_size) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            if (pltext_size != 2) {
                return 0;
            }
            auto next_char = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1);
            if ((next_char & 0xC0) == 0x80) {
                return 1;
            }
            return 0;
        }
        auto next_char = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1);
        if ((next_char & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 0;
        }
        auto next_char2 = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 2);
        if ((next_char2 & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 1;
        }
        char32_t combine{static_cast<char32_t>(chr & 0x0f) << 12 | static_cast<char32_t>(next_char & 0x3f) << 6 |
                         static_cast<char32_t>(next_char2 & 0x3f)};
        if (combine < 0x800 || combine > 0xffff) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 2;
        }
        if (0xd800 <= combine && combine <= 0xdfff) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 2;
        }

        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{chr});
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{next_char});
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{next_char2});
        return 2;
    }
    else if ((chr & 0xF8) == 0xF0) {
        if (3 >= pltext_size) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            if (pltext_size < 2) {
                return 0;
            }
            auto next_char = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1);
            if ((next_char & 0xC0) != 0x80) {
                return 0;
            }
            if (pltext_size < 3) {
                return 1;
            }
            auto next_char2 = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 2);
            if ((next_char2 & 0xC0) == 0x80) {
                return 2;
            }
            return 1;
        }
        auto next_char = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1);
        if ((next_char & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 0;
        }
        auto next_char2 = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 2);
        if ((next_char2 & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 1;
        }
        auto next_char3 = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 3);
        if ((next_char3 & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 2;
        }
        char32_t combine{static_cast<char32_t>(chr & 0x07) << 18 | static_cast<char32_t>(next_char & 0x3F) << 12 |
                         static_cast<char32_t>(next_char2 & 0x3F) << 6 | static_cast<char32_t>(next_char3 & 0x3F)};
        if (combine < 0x10000 || combine > 0x10FFFF) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 3;
        }
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{chr});
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{next_char});
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{next_char2});
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{next_char3});
        return 3;
    }
    else {
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
        return 0;
    }
}

/**
 * @brief Parse a bare HTML tag without attributes (e.g., `<tag>`).
 *
 * This template function attempts to match and parse simple HTML tags that consist only of
 * the tag name without any attributes. The tag name is specified as a template parameter.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @tparam tag_name The exact tag name to match as a compile-time string literal.
 * @param[in] pltext The input text to parse, starting at the current position.
 * @return The length of the matched tag including the closing `>`, or nullopt if no match is found
 *         or if the tag contains non-space characters before the closing `>`.
 * @note The function allows optional whitespace between the tag name and the closing `>`.
 * @note Only accepts alphabetic characters and digits in the tag name.
 * @par Example
 * &lt;div&gt;, &lt;span&gt;, and &lt;p&gt; are valid bare tags.
 */
template<::pltxt2htm::Contracts ndebug,
         ::pltxt2htm::details::U8LiteralString tag_name = ::pltxt2htm::details::U8LiteralString<0>{}>
[[nodiscard]] constexpr auto try_parse_bare_tag(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::std::size_t> {
    constexpr ::std::size_t tag_name_size{tag_name.size()};
    if (::pltxt2htm::details::is_prefix_match<ndebug, tag_name>(pltext) == false) {
        return ::exception::nullopt_t{};
    }

    for (::std::size_t i{tag_name_size}; i < pltext.size(); ++i) {
        auto forward_chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i);
        if (forward_chr == u8'>') {
            return i;
        }
        else if (forward_chr != u8' ' && forward_chr != u8'\t') {
            return ::exception::nullopt_t{};
        }
    }
    return ::exception::nullopt_t{};
}

/**
 * @brief Parse &lt;li&gt; payload in the parser's compact tag form and validate parent container type.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting after `<l`.
 * @param[in] nested_tag_type Current parent tag type from parsing context.
 * @return Matched tag length when valid under &lt;ul&gt;/&lt;ol&gt;; otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]] constexpr auto try_parse_li_tag(::fast_io::u8string_view pltext,
                                              ::pltxt2htm::NodeType const nested_tag_type) noexcept
    -> ::exception::optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_bare_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"i"}>(pltext);
    if (!opt_tag_len.has_value()) {
        return ::exception::nullopt_t{};
    }
    if (nested_tag_type != ::pltxt2htm::NodeType::html_ul && nested_tag_type != ::pltxt2htm::NodeType::html_ol) {
        return ::exception::nullopt_t{};
    }
    return opt_tag_len;
}

struct TryParseEqualSignTagResult {
    ::std::size_t tag_len; ///< Length of the tag.
    ::fast_io::u8string substr; ///< Substring extracted from the tag.
};

/**
 * @brief Validate a parsed color token for `<color=...>` tags.
 *
 * Accepted forms:
 * - CSS named color (ASCII letters only), e.g. `red`, `LightBlue`.
 * - Hex colors prefixed by `#` with 3/4/6/8 hex digits, e.g. `#fff`, `#66CcFf`, `#11223344`.
 *
 * Rejected forms include punctuation-rich payloads (e.g. `red;...`, `url(...)`)
 * and malformed hex strings to reduce style-injection/XSS risks.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] color Parsed color value from tag payload.
 * @return true if the color token is valid; otherwise false.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr bool is_valid_color(::fast_io::u8string_view color) noexcept {
    if (color.empty()) {
        return false;
    }

    auto const color_size{color.size()};
    auto const first_chr{::pltxt2htm::details::u8string_view_index<ndebug>(color, 0)};
    if (first_chr == u8'#') {
        auto const hex_len{color_size - 1};
        if (hex_len != 3 && hex_len != 4 && hex_len != 6 && hex_len != 8) {
            return false;
        }
        for (::std::size_t i{1}; i < color_size; ++i) {
            auto const chr{::pltxt2htm::details::u8string_view_index<ndebug>(color, i)};
            if (!((u8'0' <= chr && chr <= u8'9') || (u8'a' <= chr && chr <= u8'f') || (u8'A' <= chr && chr <= u8'F'))) {
                return false;
            }
        }
        return true;
    }

    for (::std::size_t i{}; i < color_size; ++i) {
        auto const chr{::pltxt2htm::details::u8string_view_index<ndebug>(color, i)};
        if (!((u8'a' <= chr && chr <= u8'z') || (u8'A' <= chr && chr <= u8'Z'))) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Parse an HTML tag with a single attribute value (e.g., `<tag=value>`).
 *
 * This function parses HTML tags that have a simple attribute with an equals sign and a value.
 * It uses a validation function to determine which characters are valid in the attribute value.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @tparam prefix_str The complete tag prefix including the attribute name and equals sign
 *                    (e.g., "size=" for `<size=value>`).
 * @tparam Func A callable type that validates characters in the tag value.
 * @param[in] pltext The input text to parse, starting at the current position.
 * @param[in] func A validation function that returns true for valid characters in the value.
 * @return The parsed result containing the tag length and extracted substring, or nullopt if parsing fails.
 * @note The function stops parsing at the first invalid character, space, or the closing `>`.
 * @note Empty values (e.g., `<size=>`) are considered invalid.
 * @note Leading/trailing spaces in the value are trimmed.
 * @note The validation function must be callable with a char8_t and return a boolean.
 */
template<::pltxt2htm::Contracts ndebug, ::pltxt2htm::details::U8LiteralString prefix_str, typename Func>
    requires requires(Func&& func, char8_t chr) {
        { func(chr) } -> ::std::same_as<bool>;
    }
[[nodiscard]]
constexpr auto try_parse_equal_sign_tag(::fast_io::u8string_view pltext, Func&& func) noexcept
    -> ::exception::optional<TryParseEqualSignTagResult> {
    ::std::size_t prefix_size{prefix_str.size()};
    constexpr auto prefix_with_equal =
        ::pltxt2htm::details::concat(prefix_str, ::pltxt2htm::details::U8LiteralString{u8"="});
    if (::pltxt2htm::details::is_prefix_match<ndebug, prefix_with_equal>(pltext) == false) {
        return ::exception::nullopt_t{};
    }

    // substr: str of $1
    ::fast_io::u8string substr{};

    for (::std::size_t forward_index{prefix_size + 1}; forward_index < pltext.size(); ++forward_index) {
        char8_t const forward_chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index)};
        if (forward_chr == u8'>') {
            if (substr.empty()) {
                // test/0030.fuzzing-crash1.cc
                // e.g. `<size=>text` is invalid
                return ::exception::nullopt_t{};
            }
            return ::pltxt2htm::details::TryParseEqualSignTagResult{forward_index, ::std::move(substr)};
        }
        else if (forward_chr == u8' ' || forward_chr == u8'\t') {
            while (true) {
                if (forward_index + 1 >= pltext.size()) {
                    return ::exception::nullopt_t{};
                }

                if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index + 1) == u8' ' ||
                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index + 1) == u8'\t') {
                    ++forward_index;
                }
                else if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index + 1) == u8'>') {
                    if (substr.empty()) {
                        // test/0030.fuzzing-crassh1.cc
                        // <size= >text
                        return ::exception::nullopt_t{};
                    }
                    return ::pltxt2htm::details::TryParseEqualSignTagResult{forward_index + 1, ::std::move(substr)};
                }
                else {
                    return ::exception::nullopt_t{};
                }
            }
        }
        else if (func(forward_chr)) {
            substr.push_back(forward_chr);
        }
        else {
            return ::exception::nullopt_t{};
        }
    }
    return ::exception::nullopt_t{};
}

/**
 * @brief Parse `<tag=value>` and reject it when nested inside non-nestable PL tags.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @tparam prefix_str Tag-name prefix used by `try_parse_equal_sign_tag`.
 * @tparam Func Character validator for the `value` part.
 * @param[in] pltext The input text to parse at current position.
 * @param[in] func Predicate that validates each value character.
 * @param[in] call_stack Active parser stack used to detect forbidden nesting.
 * @return Parsed tag result on success, otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug, ::pltxt2htm::details::U8LiteralString prefix_str, typename Func>
    requires requires(Func&& func, char8_t chr) {
        { func(chr) } -> ::std::same_as<bool>;
    }
[[nodiscard]]
constexpr auto try_parse_non_nestable_equal_sign_tag(
    ::fast_io::u8string_view pltext, Func&& func,
    ::fast_io::stack<::pltxt2htm::details::BasicFrameContext<ndebug>> const& call_stack) noexcept
    -> ::exception::optional<TryParseEqualSignTagResult> {
    auto result =
        ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, prefix_str>(pltext, ::std::forward<Func>(func));
    if (result.has_value() == false) {
        return ::exception::nullopt_t{};
    }
    for (auto const& v : call_stack.container) {
        // skip
        // e.g. <experiment><experiment>test</experiment>text</experiment>
        // e.g. <experiment><a><experiment>test</experiment>text</a>text</experiment>
        auto const nested_tag_type = v.get_nested_tag_type();
        if (nested_tag_type == ::pltxt2htm::NodeType::pl_experiment ||
            nested_tag_type == ::pltxt2htm::NodeType::pl_discussion ||
            nested_tag_type == ::pltxt2htm::NodeType::pl_external) {
            return ::exception::nullopt_t{};
        }
    }
    return result;
}

/**
 * @brief Parse `<color=...>` and validate value against a strict color grammar.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext Input text starting at `olor=...`.
 * @return Parsed tag result when valid; otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_color_tag(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<TryParseEqualSignTagResult> {
    auto result = ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, u8"olor">(
        pltext, [](char8_t u8chr) static constexpr noexcept {
            return (u8'0' <= u8chr && u8chr <= u8'9') || (u8'a' <= u8chr && u8chr <= u8'z') ||
                   (u8'A' <= u8chr && u8chr <= u8'Z') || u8chr == u8'#';
        });
    if (!result.has_value()) {
        return ::exception::nullopt_t{};
    }

    auto const& color{result.template value<ndebug == ::pltxt2htm::Contracts::ignore>().substr};
    if (!::pltxt2htm::details::is_valid_color<ndebug>(::fast_io::u8string_view{color.data(), color.size()})) {
        return ::exception::nullopt_t{};
    }
    return result;
}

/**
 * @brief Parse a self-closing HTML tag without a specific tag name (e.g., `<tag/>`).
 *
 * This function attempts to parse any self-closing HTML tag by looking for the pattern
 * of tag content followed by optional whitespace and then `/>`. It doesn't validate
 * the tag name itself, only the structural pattern.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting at the current position.
 * @return The length of the matched self-closing tag, or nullopt if no match is found.
 * @note The function allows any content between the opening `<` and closing `/>`,
 *       but stops at the first non-space character that isn't part of the `/>` pattern.
 * @note Returns the position of the closing `>` in `/>` on success.
 * @warning This function does not validate the tag name or syntax - it only matches the pattern.
 */

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_self_closing_tag(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::std::size_t> {
    for (::std::size_t forward_index{}; forward_index < pltext.size(); ++forward_index) {
        char8_t const forward_chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index)};
        if (forward_chr == u8'>') {
            return forward_index;
        }
        else if (forward_chr == u8'/' && forward_index + 1 < pltext.size() &&
                 ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index + 1) == u8'>') {
            return forward_index + 1;
        }
        else if (forward_chr != u8' ' && forward_chr != u8'\t') {
            return ::exception::nullopt_t{};
        }
    }
    return ::exception::nullopt_t{};
}

/**
 * @brief Parse a named self-closing tag form like `<br/>` with optional spaces before closure.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @tparam tag_name Compile-time tag prefix (e.g., `"<br"`).
 * @param[in] pltext The input text to parse from current position.
 * @return Position of closing `>` on success, otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug, ::pltxt2htm::details::U8LiteralString tag_name>
[[nodiscard]]
constexpr auto try_parse_self_closing_tag(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::std::size_t> {
    constexpr ::std::size_t tag_name_size{tag_name.size()};
    if (::pltxt2htm::details::is_prefix_match<ndebug, tag_name>(pltext) == false) {
        return ::exception::nullopt_t{};
    }

    for (::std::size_t forward_index{tag_name_size}; forward_index < pltext.size(); ++forward_index) {
        char8_t const forward_chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index)};
        if (forward_chr == u8'>') {
            return forward_index;
        }
        else if (forward_chr == u8'/' && forward_index + 1 < pltext.size() &&
                 ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index + 1) == u8'>') {
            return forward_index + 1;
        }
        else if (forward_chr != u8' ' && forward_chr != u8'\t') {
            return ::exception::nullopt_t{};
        }
    }
    return ::exception::nullopt_t{};
}

struct TryParseMdAtxHeadingResult {
    ::std::size_t start_index; ///< Start index of the heading content.
    ::std::size_t sublength; ///< Length of the heading content.
    ::std::size_t forward_index; ///< Index to continue parsing from.
    ::pltxt2htm::NodeType md_atx_heading_type; ///< Type of the ATX heading.
};

/**
 * @brief Parse Markdown ATX headings (e.g., `# Heading`, `## Subheading`).
 *
 * This function parses ATX-style markdown headings according to the CommonMark specification.
 * It supports headings from level 1 (`#`) to level 6 (`######`) with optional leading/trailing spaces.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting at the current position.
 * @return The parsed result containing heading level, content boundaries, and continuation index,
 *         or nullopt if the input doesn't represent a valid ATX heading.
 * @note Leading spaces are allowed before the hash characters.
 * @note Exactly one space is required after the hash characters before the heading content.
 * @note Trailing spaces and hash characters are allowed but not included in the content.
 * @note Empty headings (only hash characters) are valid according to CommonMark spec.
 * @see https://spec.commonmark.org/0.31.2/#atx-headings
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_atx_heading(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdAtxHeadingResult> {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t start_index{};
    // spaces before the first #
    while (true) {
        if (start_index >= pltext_size) {
            return ::exception::nullopt_t{};
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index) != u8' ' &&
            ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index) != u8'\t') {
            break;
        }
        ++start_index;
    }

    // count how many `#` characters
    ::std::size_t md_atx_heading_type{static_cast<::std::size_t>(::pltxt2htm::NodeType::md_atx_h1) - 1};
    while (true) {
        if (start_index >= pltext_size) {
            // https://spec.commonmark.org/0.31.2/#example-79
            if (static_cast<::std::size_t>(::pltxt2htm::NodeType::md_atx_h1) <= md_atx_heading_type &&
                md_atx_heading_type <= static_cast<::std::size_t>(::pltxt2htm::NodeType::md_atx_h6)) {
                return ::pltxt2htm::details::TryParseMdAtxHeadingResult{
                    start_index, 0, start_index, static_cast<::pltxt2htm::NodeType>(md_atx_heading_type)};
            }
            else {
                return ::exception::nullopt_t{};
            }
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index) != u8'#') {
            break;
        }
        ++start_index;
        ++md_atx_heading_type;
    }
    if (md_atx_heading_type < static_cast<::std::size_t>(::pltxt2htm::NodeType::md_atx_h1) ||
        static_cast<::std::size_t>(::pltxt2htm::NodeType::md_atx_h6) < md_atx_heading_type ||
        (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index) != u8' ' &&
         ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index) != u8'\t')) {
        // invalid atx header
        return ::exception::nullopt_t{};
    }
    ++start_index;
    // spaces after the last #
    while (true) {
        if (start_index >= pltext_size) {
            // https://spec.commonmark.org/0.31.2/#example-79
            return ::pltxt2htm::details::TryParseMdAtxHeadingResult{
                start_index, 0, start_index, static_cast<::pltxt2htm::NodeType>(md_atx_heading_type)};
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index) != u8' ' &&
            ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index) != u8'\t') {
            break;
        }
        ++start_index;
    }
    // end of the atx header
    ::std::size_t end_index{start_index};
    ::std::size_t extra_length{};
    for (; end_index < pltext_size; ++end_index) {
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, end_index) == u8'\n') {
            extra_length = 1;
            break;
        }
        else if (auto opt =
                     ::pltxt2htm::details::try_parse_self_closing_tag<ndebug,
                                                                      ::pltxt2htm::details::U8LiteralString{u8"<br"}>(
                         ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, end_index));
                 opt.has_value()) {
            extra_length = opt.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1;
            break;
        }
    }
    return ::pltxt2htm::details::TryParseMdAtxHeadingResult{
        .start_index = start_index,
        .sublength = end_index - start_index,
        .forward_index = end_index + extra_length,
        .md_atx_heading_type = static_cast<::pltxt2htm::NodeType>(md_atx_heading_type)};
}

enum class ThematicBreakType : ::std::uint_least32_t {
    none = 0, ///< No thematic break.
    hyphen, ///< Thematic break using hyphens (`---`).
    underscore, ///< Thematic break using underscores (`___`).
    asterisk, ///< Thematic break using asterisks (`***`).
};

/**
 * @brief Parse Markdown thematic breaks (horizontal rules).
 *
 * This function parses thematic breaks using three different character types:
 * hyphens (`---`), underscores (`___`), or asterisks (`***`). The break must
 * contain at least three consecutive identical characters, with optional spaces
 * between them, and must be terminated by a newline or line break tag.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] text The input text to parse, starting at the current position.
 * @return The length of the parsed thematic break including terminator, or nullopt if parsing fails.
 * @note Only one type of character (hyphen, underscore, or asterisk) is allowed in a single break.
 * @note Spaces between the characters are allowed and ignored.
 * @note The break must be terminated by a newline character or `<br/>` tag.
 * @note Mixed character types (e.g., `-*-`) result in parsing failure.
 * @see https://spec.commonmark.org/0.31.2/#thematic-breaks
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_thematic_break(::fast_io::u8string_view text) noexcept
    -> ::exception::optional<::std::size_t> {
    if (text.size() < 3) {
        return ::exception::nullopt_t{};
    }
    ::pltxt2htm::details::ThematicBreakType thematic_break_type{::pltxt2htm::details::ThematicBreakType::none};
    ::std::size_t i{};
    for (; i < text.size(); ++i) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(text, i)};
        if (chr == u8' ' || chr == u8'\t') {
            continue;
        }
        if (chr == u8'*') {
            if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::none) {
                thematic_break_type = ::pltxt2htm::details::ThematicBreakType::asterisk;
                continue;
            }
            else if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::asterisk) {
                continue;
            }
            else {
                return ::exception::nullopt_t{};
            }
        }
        else if (chr == u8'-') {
            if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::none) {
                thematic_break_type = ::pltxt2htm::details::ThematicBreakType::hyphen;
                continue;
            }
            else if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::hyphen) {
                continue;
            }
            else {
                return ::exception::nullopt_t{};
            }
        }
        else if (chr == u8'_') {
            if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::none) {
                thematic_break_type = ::pltxt2htm::details::ThematicBreakType::underscore;
                continue;
            }
            else if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::underscore) {
                continue;
            }
            else {
                return ::exception::nullopt_t{};
            }
        }
        else if (thematic_break_type != ::pltxt2htm::details::ThematicBreakType::none) {
            if (chr == u8'\n') {
                return i + 1;
            }
            else if (auto opt_tag_len = ::pltxt2htm::details::try_parse_self_closing_tag<
                         ndebug, ::pltxt2htm::details::U8LiteralString{u8"<br"}>(
                         ::pltxt2htm::details::u8string_view_subview<ndebug>(text, i));
                     opt_tag_len.has_value()) {
                return i + opt_tag_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>() + 1;
            }
            else {
                return ::exception::nullopt_t{};
            }
        }
        else {
            return ::exception::nullopt_t{};
        }
    }
    if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::none) {
        return ::exception::nullopt_t{};
    }
    else {
        return i;
    }
}

struct SimplyParsePLtextResult {
    ::std::size_t forward_index; ///< Index to continue parsing from.
    ::pltxt2htm::Ast ast; ///< Parsed AST.
};

/**
 * @brief Parse plain text content into an AST until a termination string is encountered.
 *
 * This function processes plain text content character by character, converting special
 * characters and escape sequences into appropriate AST nodes. It stops parsing when it
 * encounters the specified termination string.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @tparam end_string The exact string that marks the end of parsing.
 * @param[in] pltext The input text to parse.
 * @return A structure containing the parsed AST and the index to continue parsing from.
 * @note Special characters such as newline, space, ampersand, quotes,
 *       greater-than, and tab are converted to specific AST nodes.
 * @note Backslash escape sequences are processed and converted to their escaped equivalents.
 * @note UTF-8 multi-byte characters are properly handled and converted to U8Char nodes.
 * @note The function consumes the termination string and stops parsing immediately after it.
 */
template<::pltxt2htm::Contracts ndebug, ::pltxt2htm::details::U8LiteralString end_string>
[[nodiscard]]
constexpr auto simply_parse_pltext(::fast_io::u8string_view pltext) noexcept
    -> ::pltxt2htm::details::SimplyParsePLtextResult {
    ::pltxt2htm::Ast ast{};
    ::std::size_t current_index{};
    ::std::size_t const pltext_size{pltext.size()};
    constexpr ::std::size_t end_size{end_string.size()};

    for (; current_index < pltext.size(); ++current_index) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};

        if (::pltxt2htm::details::is_prefix_match<ndebug, end_string>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index))) {
            current_index += end_size;
            break;
        }

        if (chr == u8'\n') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LineBreak>{});
        }
        else if (chr == u8' ') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Space>{});
            continue;
        }
        else if (chr == u8'&') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Ampersand>{});
            continue;
        }
        else if (chr == u8'\'') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::SingleQuotationMark>{});
            continue;
        }
        else if (chr == u8'\"') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::DoubleQuotationMark>{});
            continue;
        }
        else if (chr == u8'>') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::GreaterThan>{});
            continue;
        }
        else if (chr == u8'\t') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Tab>{});
            continue;
        }
        else if (chr == u8'\\') {
            if (current_index + 1 == pltext_size) {
                ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{u8'\\'});
                continue;
            }
            auto escape_node = ::pltxt2htm::details::switch_escape_char(
                ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 1));
            if (escape_node.has_value()) {
                ast.push_back(::std::move(escape_node.template value<ndebug == ::pltxt2htm::Contracts::ignore>()));
                ++current_index;
            }
            else {
                ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{u8'\\'});
            }
            continue;
        }
        else if (chr == u8'<') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
        }
        else {
            auto forward_index = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index), ast);
            current_index += forward_index;
            continue;
        }
    }
    return {.forward_index = current_index, .ast = ::std::move(ast)};
}

struct TryParseMdCodeFenceResult {
    ::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode> node; ///< Parsed code fence node.
    ::std::size_t forward_index; ///< Index to continue parsing from.
};

/**
 * @brief Parse Markdown code fences with language specification.
 *
 * This function parses fenced code blocks using either backticks or tildes
 * as delimiters. It extracts the language identifier and code content between the fences.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @tparam is_backtick When true, uses backtick delimiters; otherwise uses tilde delimiters.
 * @param[in] pltext The input text to parse, starting at the opening fence.
 * @return The parsed result containing the code fence node and continuation index, or nullopt if parsing fails.
 * @note The opening fence must be at least three delimiter characters long.
 * @note An optional language identifier can follow the opening fence, separated by spaces.
 * @note The content ends at the first occurrence of the matching closing fence on its own line.
 * @note Empty language identifiers are allowed and result in no language specification.
 * @see https://spec.commonmark.org/0.31.2/#fenced-code-blocks
 */
template<::pltxt2htm::Contracts ndebug, bool is_backtick>
[[nodiscard]]
constexpr auto try_parse_md_code_fence_(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdCodeFenceResult> {
    if (pltext.size() < 7) {
        return ::exception::nullopt_t{};
    }

    constexpr auto fence = []() static constexpr noexcept {
        if constexpr (is_backtick) {
            return ::pltxt2htm::details::U8LiteralString{u8"```"};
        }
        else {
            return ::pltxt2htm::details::U8LiteralString{u8"~~~"};
        }
    }();
    constexpr ::std::size_t fence_size{fence.size()};

    if (::pltxt2htm::details::is_prefix_match<ndebug, fence>(pltext) == false) {
        return ::exception::nullopt_t{};
    }

    ::fast_io::u8string lang{};
    ::std::size_t current_index{fence_size};
    ::std::size_t const pltext_size{pltext.size()};

    // Parsing language string

    // Skipping spaces before language string
    while (current_index != pltext_size &&
           (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8' ' ||
            ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8'\t')) {
        ++current_index;
    }
    // parsing language string
    for (; current_index != pltext_size; ++current_index) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if (chr == u8'\n') {
            ++current_index;
            if (::pltxt2htm::details::is_prefix_match<ndebug, fence>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index))) {
                // https://github.com/SekaiArendelle/pltxt2htm/issues/137
                // e.g.
                // ```py
                // ```
                ::exception::optional<::fast_io::u8string> opt_lang{::exception::nullopt_t{}};
                if (!lang.empty()) {
                    opt_lang = ::std::move(lang);
                }
                if constexpr (is_backtick) {
                    return ::pltxt2htm::details::TryParseMdCodeFenceResult{
                        .node = ::pltxt2htm::HeapGuard<::pltxt2htm::MdCodeFenceBacktick>{::pltxt2htm::Ast{},
                                                                                         ::std::move(opt_lang)},
                        .forward_index = current_index + 3,
                    };
                }
                else {
                    return ::pltxt2htm::details::TryParseMdCodeFenceResult{
                        .node = ::pltxt2htm::HeapGuard<::pltxt2htm::MdCodeFenceTilde>{::pltxt2htm::Ast{},
                                                                                      ::std::move(opt_lang)},
                        .forward_index = current_index + 3,
                    };
                }
            }
            break;
        }
        else if (chr == u8' ' || chr == u8'\t') {
            ++current_index;
            if (current_index == pltext_size) {
                // 0042.fuzzing-crash3
                return ::exception::nullopt_t{};
            }
            while (current_index != pltext_size &&
                   ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8'\n') {
                ++current_index;
            }
            if (current_index == pltext_size) {
                // 0047.fuzzing-crash
                return ::exception::nullopt_t{};
            }
            ++current_index;
            break;
        }
        else {
            if constexpr (is_backtick) {
                if (::pltxt2htm::details::is_prefix_match<ndebug, fence>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index))) {
                    return ::exception::nullopt_t{};
                }
            }
            lang.push_back(chr);
        }
    }

    // parsing context of code fence
    ::pltxt2htm::Ast ast{};
    if constexpr (is_backtick) {
        constexpr auto end_string = ::pltxt2htm::details::concat(::pltxt2htm::details::U8LiteralString{u8"\n"}, fence);
        auto&& [forward_index, ast_] = ::pltxt2htm::details::simply_parse_pltext<ndebug, end_string>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
        ast = ::std::move(ast_);
        current_index += forward_index;
    }
    else {
        constexpr auto end_string = ::pltxt2htm::details::concat(::pltxt2htm::details::U8LiteralString{u8"\n"}, fence);
        auto&& [forward_index, ast_] = ::pltxt2htm::details::simply_parse_pltext<ndebug, end_string>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
        ast = ::std::move(ast_);
        current_index += forward_index;
    }

    ::exception::optional<::fast_io::u8string> opt_lang{::exception::nullopt_t{}};
    if (!lang.empty()) {
        opt_lang = ::std::move(lang);
    }
    if constexpr (is_backtick) {
        return ::pltxt2htm::details::TryParseMdCodeFenceResult{
            .node = ::pltxt2htm::HeapGuard<::pltxt2htm::MdCodeFenceBacktick>{::std::move(ast), ::std::move(opt_lang)},
            .forward_index = current_index};
    }
    else {
        return ::pltxt2htm::details::TryParseMdCodeFenceResult{
            .node = ::pltxt2htm::HeapGuard<::pltxt2htm::MdCodeFenceTilde>{::std::move(ast), ::std::move(opt_lang)},
            .forward_index = current_index};
    }
}

/**
 * @brief Parse Markdown code fences with automatic delimiter detection.
 *
 * This function attempts to parse a fenced code block by first trying backtick delimiters,
 * and if that fails, trying tilde delimiters. It automatically determines which type of
 * fence is being used.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting at the opening fence.
 * @return The parsed result containing the code fence node and continuation index, or nullopt if parsing fails.
 * @note First attempts to parse with backtick delimiters, then falls back to tilde delimiters.
 * @note Returns nullopt only if neither backtick nor tilde fences match at the current position.
 * @see try_parse_md_code_fence_ for detailed fence parsing logic
 * @see https://spec.commonmark.org/0.31.2/#fenced-code-blocks
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_code_fence(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdCodeFenceResult> {
    if (auto opt_code_fence_backtick = ::pltxt2htm::details::try_parse_md_code_fence_<ndebug, true>(pltext);
        opt_code_fence_backtick.has_value()) {
        return opt_code_fence_backtick;
    }
#if 1
    else {
        return ::pltxt2htm::details::try_parse_md_code_fence_<ndebug, false>(pltext);
    }
#else
    // Above code equals to below code
    else if (auto opt_code_fence_tilde = ::pltxt2htm::details::try_parse_md_code_fence_<ndebug, false>(pltext);
             opt_code_fence_tilde.has_value()) {
        return opt_code_fence_tilde;
    }
    else {
        return ::exception::nullopt_t{};
    }
#endif
}

/**
 * @brief Parse Markdown inline elements enclosed by matching delimiter characters.
 *
 * This function parses inline markdown elements that are wrapped by pairs of identical
 * delimiter characters, such as emphasis and inline code spans.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @tparam embraced_chars The delimiter string that encloses the inline element.
 * @param[in] pltext The input text to parse, starting at the opening delimiter.
 * @return The length of the content between the delimiters (excluding the delimiters themselves),
 *         or nullopt if no valid enclosed element is found.
 * @note The function looks for the first occurrence of the closing delimiter after the opening one.
 * @note Empty content between delimiters is considered invalid and returns nullopt.
 * @note Newline characters within the enclosed content cause parsing to fail.
 * @note The returned length is the size of the content only, not including the delimiters.
 * @see https://spec.commonmark.org/0.31.2/#emphasis-and-strong-emphasis
 * @see https://spec.commonmark.org/0.31.2/#code-spans
 */
template<::pltxt2htm::Contracts ndebug, ::pltxt2htm::details::U8LiteralString embraced_chars>
[[nodiscard]]
constexpr auto try_parse_md_inlines(::fast_io::u8string_view pltext) noexcept -> ::exception::optional<::std::size_t> {
    constexpr ::std::size_t embraced_size{embraced_chars.size()};
    if (!::pltxt2htm::details::is_prefix_match<ndebug, embraced_chars>(pltext)) {
        return ::exception::nullopt_t{};
    }

    for (::std::size_t current_index{embraced_size}; current_index < pltext.size(); ++current_index) {
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8'\n') {
            return ::exception::nullopt_t{};
        }
        if (::pltxt2htm::details::is_prefix_match<ndebug, embraced_chars>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index))) {
            ::std::size_t result{current_index - embraced_size};
            if (result == 0) {
                return ::exception::nullopt_t{};
            }
            else {
                return result;
            }
        }
    }
    return ::exception::nullopt_t{};
}

struct TryParseMdBlockQuotesResult {
    ::std::size_t forward_index; ///< Index to continue parsing from.
    ::fast_io::u8string subpltext; ///< Parsed block quote content.
};

/**
 * @brief Parse Markdown block quotes with continuation lines.
 *
 * This function parses block quote lines that start with `>` and can span multiple lines.
 * It handles optional whitespace after the quote marker and supports lazy continuation lines
 * (lines without explicit quote markers that continue the quote context).
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting at the block quote marker.
 * @return The parsed result containing the quote content and continuation index, or nullopt if parsing fails.
 * @note Each line must start with optional whitespace followed by `>` (the quote marker).
 * @note Additional whitespace after the quote marker is consumed but not included in the content.
 * @note Lines are joined with newline characters in the resulting content.
 * @note The final newline is removed from the content if present.
 * @note Empty quotes (no content after markers) are considered invalid.
 * @see https://spec.commonmark.org/0.31.2/#block-quotes
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_block_quotes(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdBlockQuotesResult> {
    ::fast_io::u8string subpltext{};

    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t current_index{};
    for (; current_index < pltext_size; ++current_index) {
        ::std::size_t temp_index{current_index};
        while (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, temp_index) == u8' ' ||
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, temp_index) == u8'\t') {
            ++temp_index;
            pltxt2htm_assert(temp_index <= pltext_size, u8"temp_index out of range");
            if (temp_index == pltext_size) { // equals to `temp_index >= pltext_size`
                return ::exception::nullopt_t{};
            }
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, temp_index) != u8'>') {
            break;
        }
        else {
            current_index = temp_index + 1;
        }
        while (current_index < pltext_size &&
               (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8' ' ||
                ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8'\t')) {
            ++current_index;
        }
        if (current_index == pltext_size) {
            break;
        }
        while (true) {
            subpltext.push_back(::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index));
            ++current_index;
            pltxt2htm_assert(current_index <= pltext_size, u8"current_index exceeds pltext_size");
            if (current_index == pltext_size) {
                break;
            }
            else if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8'\n') {
                break;
            }
        }
        if (current_index == pltext_size) {
            break;
        }
        else {
            subpltext.push_back(u8'\n');
        }
    }

    if (subpltext.empty()) {
        return ::exception::nullopt_t{};
    }
    else {
        if (subpltext.back_unchecked() == u8'\n') {
            subpltext.pop_back();
        }
        return ::pltxt2htm::details::TryParseMdBlockQuotesResult{.forward_index = current_index,
                                                                 .subpltext = ::std::move(subpltext)};
    }
}

struct TryParseMdCodeSpanResult {
    ::std::size_t forward_index; ///< Index to continue parsing from.
    ::pltxt2htm::Ast subast; ///< Parsed AST for the code span.
};

/**
 * @brief Parse Markdown code spans with configurable delimiter length.
 *
 * This function parses inline code spans that can use varying numbers of delimiters,
 * allowing code content that itself contains delimiter characters. It supports
 * one, two, or three backticks as delimiters.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @tparam embraced_string The delimiter string enclosing the code span.
 * @param[in] pltext The input text to parse, starting at the opening delimiter.
 * @return The parsed result containing the code content AST and continuation index, or nullopt if parsing fails.
 * @note The delimiter length determines the minimum number of consecutive backticks that can appear
 *       in the code content without prematurely ending the span.
 * @note The content is parsed as plain text and converted to appropriate AST nodes.
 * @note Code spans cannot contain newline characters - they must be single-line.
 * @note Empty code spans are valid and will be parsed.
 * @see https://spec.commonmark.org/0.31.2/#code-spans
 */
template<::pltxt2htm::Contracts ndebug, ::pltxt2htm::details::U8LiteralString embraced_string>
[[nodiscard]]
constexpr auto try_parse_md_code_span(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdCodeSpanResult> {
    constexpr ::std::size_t embraced_size{embraced_string.size()};
    if (!::pltxt2htm::details::is_prefix_match<ndebug, embraced_string>(pltext)) {
        return ::exception::nullopt_t{};
    }

    auto&& [forward_index, ast] = ::pltxt2htm::details::simply_parse_pltext<ndebug, embraced_string>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, embraced_size));

    if constexpr (embraced_size == 1) {
        return ::pltxt2htm::details::TryParseMdCodeSpanResult{.forward_index = forward_index + embraced_size,
                                                              .subast = ::std::move(ast)};
    }
    else if constexpr (embraced_size == 2) {
        return ::pltxt2htm::details::TryParseMdCodeSpanResult{.forward_index = forward_index + embraced_size,
                                                              .subast = ::std::move(ast)};
    }
    else if constexpr (embraced_size == 3) {
        return ::pltxt2htm::details::TryParseMdCodeSpanResult{.forward_index = forward_index + embraced_size,
                                                              .subast = ::std::move(ast)};
    }
    else {
        ::exception::unreachable();
    }
}

struct TryParseMdLatexResult {
    ::std::size_t forward_index; ///< Index to continue parsing from (includes both delimiters).
    ::pltxt2htm::Ast subast; ///< Parsed AST inside the latex delimiters.
};

/**
 * @brief Parse block LaTeX math expressions delimited by double dollar signs.
 *
 * This function parses display math LaTeX expressions that are enclosed in `$$` delimiters
 * on both sides. It extracts the mathematical content and converts it to appropriate AST nodes,
 * preserving newlines within the expression.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting at the opening `$$`.
 * @return The parsed result containing the LaTeX content AST and continuation index, or nullopt if parsing fails.
 * @note The opening `$$` must be at the very beginning of the input text.
 * @note The expression must be terminated by a matching `$$` delimiter.
 * @note Newlines within the LaTeX expression are preserved as U8Char nodes.
 * @note Empty expressions (e.g., `$$$$`) are considered invalid and return nullopt.
 * @note The function returns the position after the closing `$$` on success.
 * @see https://github.com/cben/mathdown/wiki/math-in-markdown
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_latex_block_dollar(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdLatexResult> {
    constexpr auto double_dollar = ::pltxt2htm::details::U8LiteralString{u8"$$"};
    if (pltext.size() < 4 || ::pltxt2htm::details::is_prefix_match<ndebug, double_dollar>(pltext) == false) {
        return ::exception::nullopt_t{};
    }

    auto body = ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 2);
    ::std::size_t const body_size{body.size()};
    ::std::size_t close_pos{body_size};
    for (::std::size_t i{}; i < body_size; ++i) {
        if (::pltxt2htm::details::is_prefix_match<ndebug, double_dollar>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(body, i))) {
            close_pos = i;
            break;
        }
    }
    if (close_pos == body_size) {
        return ::exception::nullopt_t{};
    }

    ::pltxt2htm::Ast ast{};
    for (::std::size_t idx{}; idx < close_pos;) {
        if (::pltxt2htm::details::u8string_view_index<ndebug>(body, idx) == u8'\n') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{u8'\n'});
            ++idx;
        }
        else {
            auto forward = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(body, idx), ast);
            idx += forward + 1;
        }
    }

    return ::pltxt2htm::details::TryParseMdLatexResult{.forward_index = close_pos + 4, .subast = ::std::move(ast)};
}

/**
 * @brief Parse inline LaTeX math expressions delimited by single dollar signs.
 *
 * This function parses inline math LaTeX expressions that are enclosed in single `$` delimiters.
 * It extracts the mathematical content and converts it to appropriate AST nodes, stopping at
 * the first closing `$` delimiter.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting at the opening `$`.
 * @return The parsed result containing the LaTeX content AST and continuation index, or nullopt if parsing fails.
 * @note The opening `$` must be at the very beginning of the input text.
 * @note The expression must be terminated by a matching `$` delimiter on the same line.
 * @note Newlines within the LaTeX expression cause parsing to fail (inline math must be single-line).
 * @note Empty expressions (e.g., `$$`) are considered invalid and return nullopt.
 * @note The function returns the position after the closing `$` on success.
 * @see https://github.com/cben/mathdown/wiki/math-in-markdown
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_latex_inline(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdLatexResult> {
    if (pltext.size() < 3 || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 0) != u8'$') {
        return ::exception::nullopt_t{};
    }

    auto body = ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 1);
    ::std::size_t const body_size{body.size()};
    ::std::size_t close_pos{body_size};
    for (::std::size_t i{}; i < body_size; ++i) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(body, i);
        if (chr == u8'\n') {
            return ::exception::nullopt_t{};
        }
        else if (chr == u8'$') {
            close_pos = i;
            break;
        }
    }
    if (close_pos == body_size) {
        return ::exception::nullopt_t{};
    }
    if (close_pos == 0) {
        return ::exception::nullopt_t{};
    }

    ::pltxt2htm::Ast ast{};
    for (::std::size_t idx{}; idx < close_pos;) {
        auto&& sub = ::pltxt2htm::details::u8string_view_subview<ndebug>(body, idx);
        auto forward = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(sub, ast);
        idx += forward + 1;
    }

    return ::pltxt2htm::details::TryParseMdLatexResult{.forward_index = close_pos + 2, .subast = ::std::move(ast)};
}

/**
 * @brief Validate URL domain and top-level domain portion.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext Original URL text.
 * @param[in] domain_start Start index (inclusive) of the domain segment.
 * @param[in] domain_end End index (exclusive) of the domain segment.
 * @return `true` if domain labels and TLD are accepted by project rules; otherwise `false`.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto validate_url_domain(::fast_io::u8string_view pltext, ::std::size_t domain_start,
                                   ::std::size_t domain_end) noexcept -> bool {
    if (domain_end > pltext.size() || domain_end <= domain_start) {
        return false;
    }
    auto domain_vw = ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 0, domain_end);
    if (domain_vw.ends_with(u8".com") == false && domain_vw.ends_with(u8".net") == false &&
        domain_vw.ends_with(u8".org") == false && domain_vw.ends_with(u8".cn") == false &&
        domain_vw.ends_with(u8".edu") == false && domain_vw.ends_with(u8".gov") == false &&
        domain_vw.ends_with(u8".io") == false && domain_vw.ends_with(u8".ai") == false &&
        domain_vw.ends_with(u8".co") == false && domain_vw.ends_with(u8".me") == false &&
        domain_vw.ends_with(u8".cc") == false && domain_vw.ends_with(u8".tv") == false &&
        domain_vw.ends_with(u8".info") == false && domain_vw.ends_with(u8".biz") == false &&
        domain_vw.ends_with(u8".us") == false && domain_vw.ends_with(u8".uk") == false &&
        domain_vw.ends_with(u8".jp") == false && domain_vw.ends_with(u8".hk") == false &&
        domain_vw.ends_with(u8".tw") == false && domain_vw.ends_with(u8".xyz") == false &&
        domain_vw.ends_with(u8".top") == false) {
        return false;
    }

    auto const domain =
        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, domain_start, domain_end - domain_start);
    bool label_has_char{};
    bool label_ended_with_hyphen{};
    for (auto chr : domain) {
        if (chr == u8'.') {
            if (label_has_char == false || label_ended_with_hyphen) {
                return false;
            }
            label_has_char = false;
            label_ended_with_hyphen = false;
            continue;
        }

        if ((u8'A' <= chr && chr <= u8'Z') || (u8'a' <= chr && chr <= u8'z') || (u8'0' <= chr && chr <= u8'9')) {
            label_has_char = true;
            label_ended_with_hyphen = false;
            continue;
        }

        if (chr == u8'-') {
            if (label_has_char == false) {
                return false;
            }
            label_ended_with_hyphen = true;
            continue;
        }

        return false;
    }

    return label_has_char && !label_ended_with_hyphen;
}

/**
 * @brief Parse and validate an URL starting with optional `http://` or `https://`.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @tparam regard_right_parent_as_end_of_url Whether `)` is treated as a hard URL terminator.
 * @param[in] pltext The input text that begins at a URL candidate.
 * @return Parsed URL payload (URL object + consumed size) on success; nullopt when domain/port/path validation fails.
 */
struct TryParseUrlResult {
    ::std::size_t consumed_size;
    ::pltxt2htm::Url url;
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto make_try_parse_url_result(::fast_io::u8string_view const parsed_url,
                                         ::std::size_t consumed_size) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseUrlResult> {
    ::pltxt2htm::Ast ast{};
    for (::std::size_t index{}; index < parsed_url.size(); ++index) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(parsed_url, index);
        if (chr == u8'&') {
            if (index + 5 <= parsed_url.size() &&
                ::pltxt2htm::details::u8string_view_subview<ndebug>(parsed_url, index, 5) == u8"&amp;") {
                chr = u8'&';
                index += 4;
            }
        }
        switch (chr) {
        case u8'&':
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Ampersand>{});
            break;
        case u8'\'':
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::SingleQuotationMark>{});
            break;
        case u8'\"':
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::DoubleQuotationMark>{});
            break;
        case u8'<':
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
            break;
        case u8'>':
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::GreaterThan>{});
            break;
        default:
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{chr});
            break;
        }
    }
    return ::pltxt2htm::details::TryParseUrlResult{.consumed_size = consumed_size,
                                                   .url = ::pltxt2htm::Url{::std::move(ast)}};
}

template<::pltxt2htm::Contracts ndebug, bool regard_right_parent_as_end_of_url = false>
[[nodiscard]]
constexpr auto try_parse_url(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseUrlResult> {
    ::std::size_t current_index{[pltext] constexpr noexcept -> ::std::size_t {
        if (constexpr auto http = ::pltxt2htm::details::U8LiteralString{u8"http://"};
            ::pltxt2htm::details::is_prefix_match<ndebug, http>(pltext)) {
            return http.size();
        }
        else if (constexpr auto https = ::pltxt2htm::details::U8LiteralString{u8"https://"};
                 ::pltxt2htm::details::is_prefix_match<ndebug, https>(pltext)) {
            return https.size();
        }
        else {
            return 0;
        }
    }()};
    auto const domain_start = current_index;

    // parsing domain name
    while (true) {
        if (current_index >= pltext.size()) {
            if (::pltxt2htm::details::validate_url_domain<ndebug>(pltext, domain_start, current_index) == false) {
                return ::exception::nullopt_t{};
            }
            break;
        }
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if ((u8'A' <= chr && chr <= u8'Z') || (u8'a' <= chr && chr <= u8'z') || (u8'0' <= chr && chr <= u8'9') ||
            chr == u8'.' || chr == u8'-') {
            ++current_index;
            continue;
        }
        else if (chr == u8'/') {
            if (::pltxt2htm::details::validate_url_domain<ndebug>(pltext, domain_start, current_index) == false) {
                return ::exception::nullopt_t{};
            }
            ++current_index;
            break;
        }
        else if (chr == u8':') {
            if (::pltxt2htm::details::validate_url_domain<ndebug>(pltext, domain_start, current_index) == false) {
                return ::exception::nullopt_t{};
            }
            ::std::uint_least32_t port{};
            ::std::size_t port_index{};
            for (char8_t c : ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1)) {
                if (c < u8'0' || c > u8'9') {
                    break;
                }
                port = port * 10 + (c - '0');
                ++port_index;
                if (port_index > 5) {
                    return ::exception::nullopt_t{};
                }
            }
            if (port_index == 0) {
                return ::exception::nullopt_t{};
            }
            if (port > 65535) {
                return ::exception::nullopt_t{};
            }
            current_index += port_index + 1;
            if (current_index < pltext.size()) {
                auto const next_chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
                if (next_chr != u8'/' && next_chr != u8'?' && next_chr != u8'#') {
                    if constexpr (regard_right_parent_as_end_of_url) {
                        if (next_chr == u8')') {
                            return ::pltxt2htm::details::make_try_parse_url_result<ndebug>(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 0, current_index),
                                current_index);
                        }
                    }
                    return ::exception::nullopt_t{};
                }
            }
            break;
        }
        else {
            if constexpr (regard_right_parent_as_end_of_url) {
                if (chr != u8')') {
                    return ::exception::nullopt_t{};
                }
                if (::pltxt2htm::details::validate_url_domain<ndebug>(pltext, domain_start, current_index) == false) {
                    return ::exception::nullopt_t{};
                }
                return ::pltxt2htm::details::make_try_parse_url_result<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 0, current_index), current_index);
            }
            return ::exception::nullopt_t{};
        }
    }
    for (; current_index < pltext.size(); ++current_index) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if (chr < u8'!' || chr > u8'~' || chr == u8'<' || chr == u8'>' || chr == u8'\"') {
            return ::pltxt2htm::details::make_try_parse_url_result<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 0, current_index), current_index);
        }
        if constexpr (regard_right_parent_as_end_of_url) {
            if (chr == u8')') {
                return ::pltxt2htm::details::make_try_parse_url_result<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 0, current_index), current_index);
            }
        }
    }
    return ::pltxt2htm::details::make_try_parse_url_result<ndebug>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 0, current_index), current_index);
}

/**
 * @brief Parse `<external=...>` tag and validate its URL payload.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text starting at the `external` tag payload.
 * @param[in] call_stack Active parser frames used to reject invalid nested contexts.
 * @return Parsed tag length and extracted URL on success; nullopt if invalid or disallowed nesting.
 */
struct TryParseExternalTagResult {
    ::std::size_t tag_len;
    ::pltxt2htm::Url url;
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_external_tag(
    ::fast_io::u8string_view pltext,
    ::fast_io::stack<::pltxt2htm::details::BasicFrameContext<ndebug>> const& call_stack) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseExternalTagResult> {
    auto result = ::pltxt2htm::details::try_parse_non_nestable_equal_sign_tag<ndebug, u8"xternal">(
        pltext, [](char8_t u8chr) static constexpr noexcept { return u8'!' <= u8chr && u8chr <= u8'~'; }, call_stack);
    if (result.has_value() == false) {
        return ::exception::nullopt_t{};
    }

    auto&& [_, url] = result.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    auto opt_url = ::pltxt2htm::details::try_parse_url<ndebug>(::fast_io::u8string_view{url.data(), url.size()});
    if (opt_url.has_value() == false) {
        return ::exception::nullopt_t{};
    }
    auto&& parsed_url_result = opt_url.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    if (parsed_url_result.consumed_size != url.size()) {
        return ::exception::nullopt_t{};
    }

    return ::pltxt2htm::details::TryParseExternalTagResult{
        result.template value<ndebug == ::pltxt2htm::Contracts::ignore>().tag_len, ::std::move(parsed_url_result.url)};
}

struct TryParseMdLinkResult {
    ::std::size_t forward_index;
    ::fast_io::u8string_view link_text;
    ::pltxt2htm::Url link_url;
};

/**
 * @brief Parse Markdown inline links with text and URL components.
 *
 * This function parses standard markdown links in the format `[link text](URL)`. It extracts
 * both the link text (displayed to users) and the link URL (the destination), handling
 * escaped characters within the link text portion.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting at the opening `[`.
 * @return The parsed result containing the link text, URL, and continuation index, or nullopt if parsing fails.
 * @note The link text is contained in square brackets `[...]` and can include escaped characters.
 * @note The URL is contained in parentheses `(...)` immediately following the link text.
 * @note Backslash escapes in the link text are properly handled and consumed.
 * @note Empty link text is allowed, but empty URLs are not validated (left as-is).
 * @note The function requires both opening and closing brackets and parentheses to be present.
 * @see https://spec.commonmark.org/0.31.2/#links
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_link(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdLinkResult> {
    if (pltext.size() < 4 || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 0) != u8'[') {
        return ::exception::nullopt_t{};
    }

    ::std::size_t current_index{1};

    // Parse link text
    while (current_index < pltext.size()) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};
        if (chr == u8'\\') {
            // No need to handle escape here
            // Because the result of `link_text` is string_view
            // `::pltxt2htm::details::parse_pltxt` will handle the escape when converting to ast
            ++current_index;
        }
        else if (chr == u8'\n') {
            return ::exception::nullopt_t{};
        }
        else if (chr == u8']') {
            break;
        }
        ++current_index;
    }

    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8']') {
        return ::exception::nullopt_t{};
    }
    ::std::size_t link_text_end{current_index};
    ++current_index;

    // Ensure the next character is '('
    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8'(') {
        return ::exception::nullopt_t{};
    }
    ++current_index;
    auto opt_link_url = ::pltxt2htm::details::try_parse_url<ndebug, true>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
    if (opt_link_url.has_value() == false) {
        return ::exception::nullopt_t{};
    }
    auto&& [consumed_size, urlobj] = opt_link_url.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    current_index += consumed_size;
    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8')') {
        return ::exception::nullopt_t{};
    }
    ++current_index;
    return ::pltxt2htm::details::TryParseMdLinkResult{.forward_index = current_index,
                                                      .link_text = pltext.subview(1, link_text_end - 1),
                                                      .link_url = ::std::move(urlobj)};
}

struct TryParseMdImageResult {
    ::std::size_t forward_index;
    ::pltxt2htm::Ast link_text;
    ::pltxt2htm::Url link_url;
};

/**
 * @brief Parse Markdown image syntax.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text beginning with an exclamation mark and an opening bracket.
 * @return Parsed image payload (alt text AST + URL + continuation index), or nullopt if invalid.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_image(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdImageResult> {
    if (pltext.size() < 5 || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 0) != u8'!' ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1) != u8'[') {
        return ::exception::nullopt_t{};
    }

    ::std::size_t current_index{2};

    // Parse link text
    ::pltxt2htm::Ast link_text_ast{};
    for (; current_index < pltext.size() &&
           ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8']';
         ++current_index) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};

        if (chr == u8'\n') {
            return ::exception::nullopt_t{};
        }
        else if (chr == u8' ') {
            link_text_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Space>{});
            continue;
        }
        else if (chr == u8'&') {
            link_text_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Ampersand>{});
            continue;
        }
        else if (chr == u8'\'') {
            link_text_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::SingleQuotationMark>{});
            continue;
        }
        else if (chr == u8'\"') {
            link_text_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::DoubleQuotationMark>{});
            continue;
        }
        else if (chr == u8'>') {
            link_text_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::GreaterThan>{});
            continue;
        }
        else if (chr == u8'\t') {
            link_text_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Tab>{});
            continue;
        }
        else if (chr == u8'\\') {
            if (current_index + 1 == pltext.size()) {
                link_text_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{u8'\\'});
                continue;
            }
            auto escape_node = ::pltxt2htm::details::switch_escape_char(
                ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 1));
            if (escape_node.has_value()) {
                link_text_ast.push_back(
                    ::std::move(escape_node.template value<ndebug == ::pltxt2htm::Contracts::ignore>()));
                ++current_index;
            }
            else {
                link_text_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{u8'\\'});
            }
            continue;
        }
        else if (chr == u8'<') {
            link_text_ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
        }
        else {
            auto forward_index = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index), link_text_ast);
            current_index += forward_index;
            continue;
        }
    }

    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8']') {
        return ::exception::nullopt_t{};
    }
    ++current_index;

    // Ensure the next character is '('
    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8'(') {
        return ::exception::nullopt_t{};
    }
    ++current_index;
    auto opt_link_url = ::pltxt2htm::details::try_parse_url<ndebug, true>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
    if (opt_link_url.has_value() == false) {
        return ::exception::nullopt_t{};
    }
    auto&& link_url_result = opt_link_url.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    current_index += link_url_result.consumed_size;
    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8')') {
        return ::exception::nullopt_t{};
    }
    return ::pltxt2htm::details::TryParseMdImageResult{.forward_index = current_index + 1,
                                                       .link_text = ::std::move(link_text_ast),
                                                       .link_url = ::std::move(link_url_result.url)};
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_entity_reference(::fast_io::u8string_view text) noexcept
    -> ::exception::optional<::std::size_t> {
    if (text.empty() || ::pltxt2htm::details::u8string_view_index<ndebug>(text, 0) != u8'&') {
        return ::exception::nullopt_t{};
    }
    auto const max = text.size();
    auto index = ::std::size_t{1};
    if (index >= max) {
        return ::exception::nullopt_t{};
    }
    if (::pltxt2htm::details::u8string_view_index<ndebug>(text, index) == u8'#') {
        ++index;
        if (index >= max) {
            return ::exception::nullopt_t{};
        }
        bool hex{};
        auto const prefix = ::pltxt2htm::details::u8string_view_index<ndebug>(text, index);
        if (prefix == u8'x' || prefix == u8'X') {
            hex = true;
            ++index;
        }
        if (index >= max) {
            return ::exception::nullopt_t{};
        }
        auto const begin = index;
        for (; index < max; ++index) {
            auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(text, index);
            if (chr == u8';') {
                break;
            }
            if (hex ? !::pltxt2htm::details::is_ascii_hexdigit(chr) : !::pltxt2htm::details::is_ascii_digit(chr)) {
                return ::exception::nullopt_t{};
            }
        }
        if (index == begin || index >= max || ::pltxt2htm::details::u8string_view_index<ndebug>(text, index) != u8';') {
            return ::exception::nullopt_t{};
        }
        return index + 1;
    }

    if (!::pltxt2htm::details::is_ascii_alpha(::pltxt2htm::details::u8string_view_index<ndebug>(text, index))) {
        return ::exception::nullopt_t{};
    }
    ++index;
    for (; index < max; ++index) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(text, index);
        if (chr == u8';') {
            return index + 1;
        }
        if (!::pltxt2htm::details::is_ascii_alpha(chr) && !::pltxt2htm::details::is_ascii_digit(chr)) {
            return ::exception::nullopt_t{};
        }
    }
    return ::exception::nullopt_t{};
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
