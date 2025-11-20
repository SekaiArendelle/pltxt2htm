#pragma once

#include <exception/exception.hh>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../utils.hh"
#include "../../heap_guard.hh"
#include "../../astnode/basic.hh"
#include "../../astnode/node_type.hh"
#include "../../astnode/html_node.hh"
#include "../../astnode/markdown_node.hh"

namespace pltxt2htm::details {

/**
 * @brief Switch to a markdown punctuation character.
 *
 * @param u8char The UTF-8 character to switch.
 * @return An optional HeapGuard containing the corresponding PlTxtNode, or nullopt if no match is found.
 */
constexpr ::exception::optional<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>> switch_escape_char(
    char8_t u8char) noexcept {
    switch (u8char) {
    case u8'\\': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeBackslash>{});
    }
    case u8'!': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeExclamation>{});
    }
    case u8'\"': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeDoubleQuote>{});
    }
    case u8'#': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeHash>{});
    }
    case u8'$': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeDollar>{});
    }
    case u8'%': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapePercent>{});
    }
    case u8'&': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeAmpersand>{});
    }
    case u8'\'': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeSingleQuote>{});
    }
    case u8'(': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeLeftParen>{});
    }
    case u8')': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeRightParen>{});
    }
    case u8'*': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeAsterisk>{});
    }
    case u8'+': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapePlus>{});
    }
    case u8',': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeComma>{});
    }
    case u8'-': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeHyphen>{});
    }
    case u8'.': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeDot>{});
    }
    case u8'/': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeSlash>{});
    }
    case u8':': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeColon>{});
    }
    case u8';': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeSemicolon>{});
    }
    case u8'<': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeLessThan>{});
    }
    case u8'=': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeEquals>{});
    }
    case u8'>': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeGreaterThan>{});
    }
    case u8'?': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeQuestion>{});
    }
    case u8'@': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeAt>{});
    }
    case u8'[': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeLeftBracket>{});
    }
    case u8']': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeRightBracket>{});
    }
    case u8'^': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeCaret>{});
    }
    case u8'_': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeUnderscore>{});
    }
    case u8'`': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeBacktick>{});
    }
    case u8'{': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeLeftBrace>{});
    }
    case u8'|': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapePipe>{});
    }
    case u8'}': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeRightBrace>{});
    }
    case u8'~': {
        return static_cast<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::HeapGuard<::pltxt2htm::EscapeTilde>{});
    }
    default:
        return ::exception::nullopt_t{};
    }
}

/**
 * @brief Parse a single UTF-8 code point and append the corresponding AST node(s).
 *
 * @tparam ndebug When `true`, runtime assertions are disabled.
 * @param pltext The complete input text being parsed.
 * @param result The AST to which parsed nodes are appended.
 * @return The number of UTF-8 code units consumed.
 */
template<bool ndebug>
[[nodiscard]]
constexpr auto parse_utf8_code_point(::fast_io::u8string_view const& pltext, ::pltxt2htm::Ast& result) noexcept
    -> ::std::size_t {
    ::std::size_t const pltext_size{pltext.size()};
    char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 0)};

    if (chr <= 0x1f || (0x7f <= chr && chr <= 0x9f)) {
        return 0;
    }
    if ((chr & 0x80) == 0) {
        // normal utf-8 characters
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{chr});
        return 0;
    } else if ((chr & 0xE0) == 0xC0) {
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
            return 0;
        }

        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{chr});
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{next_char});
        return 1;
    } else if ((chr & 0xF0) == 0xE0) {
        if (2 >= pltext_size) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
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
            return 0;
        }
        char32_t combine{static_cast<char32_t>(chr & 0x0f) << 12 | static_cast<char32_t>(next_char & 0x3f) << 6 |
                         static_cast<char32_t>(next_char2 & 0x3f)};
        if (combine < 0x800 || combine > 0xffff) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 0;
        }
        if (0xd800 <= combine && combine <= 0xdfff) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 0;
        }

        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{chr});
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{next_char});
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{next_char2});
        return 2;
    } else if ((chr & 0xF8) == 0xF0) {
        if (3 >= pltext_size) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 0;
        }
        auto next_char = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1);
        if ((next_char & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 0;
        }
        auto next_char2 = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 2);
        if ((next_char & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 0;
        }
        auto next_char3 = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 3);
        if ((next_char3 & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 0;
        }
        char32_t combine{static_cast<char32_t>(chr & 0x07) << 18 | static_cast<char32_t>(next_char & 0x3F) << 12 |
                         static_cast<char32_t>(next_char2 & 0x3F) << 6 | static_cast<char32_t>(next_char3 & 0x3F)};
        if (combine < 0x10000 || combine > 0x10FFFF) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 0;
        }
        if (0xd800 <= combine && combine <= 0xdfff) {
            result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return 0;
        }

        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{chr});
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{next_char});
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{next_char2});
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{next_char3});
        return 3;
    } else {
        result.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::InvalidU8Char>{});
        return 0;
    }
}

/**
 * @brief Check if the input matches a bare tag pattern.
 *
 * @tparam tag_name The prefix string representing the tag name.
 * @param pltext The string to be checked.
 * @return The length of the matched tag, or nullopt if no match is found.
 */
template<bool ndebug, char8_t... tag_name>
[[nodiscard]]
constexpr auto try_parse_bare_tag(::fast_io::u8string_view pltext) noexcept -> ::exception::optional<::std::size_t> {
    if (::pltxt2htm::details::is_prefix_match<ndebug, tag_name...>(pltext) == false) {
        return ::exception::nullopt_t{};
    }

    for (::std::size_t i{sizeof...(tag_name)}; i < pltext.size(); ++i) {
        auto forward_chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i);
        if (forward_chr == u8'>') {
            return i;
        } else if (forward_chr != ' ') {
            return ::exception::nullopt_t{};
        }
    }
    return ::exception::nullopt_t{};
}

struct TryParseEqualSignTagResult {
    ::std::size_t tag_len; ///< Length of the tag.
    ::fast_io::u8string substr; ///< Substring extracted from the tag.
};

/**
 * @brief Parse a tag with an equal sign, e.g., `<tag=value>`.
 *
 * @tparam ndebug When `true`, runtime assertions are disabled.
 * @tparam prefix_str The prefix string before the equal sign.
 * @tparam Func A callable to validate characters in the tag value.
 * @param pltext The input text to parse.
 * @param func The validation function for tag value characters.
 * @return The parsed result, or nullopt if parsing fails.
 */
template<bool ndebug, char8_t... prefix_str, typename Func>
    requires requires(Func&& func, char8_t chr) {
        { func(chr) } -> ::std::same_as<bool>;
    }
[[nodiscard]]
constexpr auto try_parse_equal_sign_tag(::fast_io::u8string_view pltext, Func&& func) noexcept
    -> ::exception::optional<TryParseEqualSignTagResult> {
    if (::pltxt2htm::details::is_prefix_match<ndebug, prefix_str..., u8'='>(pltext) == false) {
        return ::exception::nullopt_t{};
    }

    // substr: str of $1
    ::fast_io::u8string substr{};

    for (::std::size_t forward_index{sizeof...(prefix_str) + 1}; forward_index < pltext.size(); ++forward_index) {
        char8_t const forward_chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index)};
        if (forward_chr == u8'>') {
            if (substr.empty()) {
                // test/0030.fuzzing-crash1.cc
                // e.g. `<size=>text` is invalid
                return ::exception::nullopt_t{};
            }
            return ::pltxt2htm::details::TryParseEqualSignTagResult{forward_index, ::std::move(substr)};
        } else if (forward_chr == u8' ') {
            while (true) {
                if (forward_index + 1 >= pltext.size()) {
                    return ::exception::nullopt_t{};
                }

                if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index + 1) == u8' ') {
                    ++forward_index;
                } else if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index + 1) == u8'>') {
                    if (substr.empty()) {
                        // test/0030.fuzzing-crassh1.cc
                        // <size= >text
                        return ::exception::nullopt_t{};
                    }
                    return ::pltxt2htm::details::TryParseEqualSignTagResult{forward_index + 1, ::std::move(substr)};
                } else {
                    return ::exception::nullopt_t{};
                }
            }
        } else if (func(forward_chr)) {
            substr.push_back(forward_chr);
        } else {
            return ::exception::nullopt_t{};
        }
    }
    return ::exception::nullopt_t{};
}

/**
 * @brief Try to parse a self-closing tag, e.g., `<tag/>`.
 *
 * @tparam tag_name The tag name to match.
 * @param pltext The input text to parse.
 * @return The length of the matched tag, or nullopt if no match is found.
 */
template<bool ndebug, char8_t... tag_name>
[[nodiscard]]
constexpr auto try_parse_self_closing_tag(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::std::size_t> {
    if (::pltxt2htm::details::is_prefix_match<ndebug, tag_name...>(pltext) == false) {
        return ::exception::nullopt_t{};
    }

    for (::std::size_t forward_index{sizeof...(tag_name)}; forward_index < pltext.size(); ++forward_index) {
        char8_t const forward_chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index)};
        if (forward_chr == u8'>') {
            return forward_index;
        } else if (forward_chr == u8'/' && forward_index + 1 < pltext.size() &&
                   ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index + 1) == u8'>') {
            return forward_index + 1;
        } else if (forward_chr != u8' ') {
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
 * @brief Parse Markdown ATX headings (e.g., `# Heading`).
 *
 * @param pltext The input text to parse.
 * @return The parsed result, or nullopt if parsing fails.
 */
template<bool ndebug>
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
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index) != u8' ') {
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
            } else {
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
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index) != u8' ') {
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
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index) != u8' ') {
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
        } else if (auto opt = ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, u8'<', u8'b', u8'r'>(
                       ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, end_index));
                   opt.has_value()) {
            extra_length = opt.template value<ndebug>() + 1;
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
 * @brief Parse Markdown thematic breaks (e.g., `---`, `___`, `***`).
 *
 * @param text The input text to parse.
 * @return The length of the parsed thematic break, or nullopt if parsing fails.
 */
template<bool ndebug>
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
        if (chr == u8' ') {
            continue;
        }
        if (chr == u8'*') {
            if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::none) {
                thematic_break_type = ::pltxt2htm::details::ThematicBreakType::asterisk;
                continue;
            } else if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::asterisk) {
                continue;
            } else {
                return ::exception::nullopt_t{};
            }
        } else if (chr == u8'-') {
            if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::none) {
                thematic_break_type = ::pltxt2htm::details::ThematicBreakType::hyphen;
                continue;
            } else if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::hyphen) {
                continue;
            } else {
                return ::exception::nullopt_t{};
            }
        } else if (chr == u8'_') {
            if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::none) {
                thematic_break_type = ::pltxt2htm::details::ThematicBreakType::underscore;
                continue;
            } else if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::underscore) {
                continue;
            } else {
                return ::exception::nullopt_t{};
            }
        } else if (thematic_break_type != ::pltxt2htm::details::ThematicBreakType::none) {
            if (chr == u8'\n') {
                return i + 1;
            } else if (auto opt_tag_len = ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, u8'<', u8'b', u8'r'>(
                           ::pltxt2htm::details::u8string_view_subview<ndebug>(text, i));
                       opt_tag_len.has_value()) {
                return i + opt_tag_len.template value<ndebug>() + 1;
            } else {
                return ::exception::nullopt_t{};
            }
        } else {
            return ::exception::nullopt_t{};
        }
    }
    if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::none) {
        return ::exception::nullopt_t{};
    } else {
        return i;
    }
}

struct SimplyParsePLtextResult {
    ::std::size_t forward_index; ///< Index to continue parsing from.
    ::pltxt2htm::Ast ast; ///< Parsed AST.
};

/**
 * @brief Parse plain text into an AST.
 *
 * @tparam end_string The string that marks the end of parsing.
 * @param pltext The input text to parse.
 * @return The parsed result.
 */
template<bool ndebug, char8_t... end_string>
[[nodiscard]]
constexpr auto simply_parse_pltext(::fast_io::u8string_view pltext) noexcept
    -> ::pltxt2htm::details::SimplyParsePLtextResult {
    ::pltxt2htm::Ast ast{};
    ::std::size_t current_index{};
    ::std::size_t const pltext_size{pltext.size()};

    for (; current_index < pltext.size(); ++current_index) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};

        if (::pltxt2htm::details::is_prefix_match<ndebug, end_string...>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index))) {
            current_index += sizeof...(end_string);
            break;
        }

        if (chr == u8'\n') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LineBreak>{});
        } else if (chr == u8' ') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Space>{});
            continue;
        } else if (chr == u8'&') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Ampersand>{});
            continue;
        } else if (chr == u8'\'') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::SingleQuotationMark>{});
            continue;
        } else if (chr == u8'\"') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::DoubleQuotationMark>{});
            continue;
        } else if (chr == u8'>') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::GreaterThan>{});
            continue;
        } else if (chr == u8'\t') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::Tab>{});
            continue;
        } else if (chr == u8'\\') {
            if (current_index + 1 == pltext_size) {
                ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{u8'\\'});
                continue;
            }
            auto escape_node = ::pltxt2htm::details::switch_escape_char(
                ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 1));
            if (escape_node.has_value()) {
                ast.push_back(::std::move(escape_node.template value<ndebug>()));
                ++current_index;
            } else {
                ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::U8Char>{u8'\\'});
            }
            continue;
        } else if (chr == u8'<') {
            ast.push_back(::pltxt2htm::HeapGuard<::pltxt2htm::LessThan>{});
        } else {
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
 * @brief Parse Markdown code fences (e.g., ````` or `~~~`).
 *
 * @tparam is_backtick Whether the code fence uses backticks.
 * @param pltext The input text to parse.
 * @return The parsed result, or nullopt if parsing fails.
 */
template<bool ndebug, bool is_backtick>
[[nodiscard]]
constexpr auto try_parse_md_code_fence_(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdCodeFenceResult> {
    if (pltext.size() <= 7) {
        return ::exception::nullopt_t{};
    }

    if constexpr (is_backtick) {
        if (::pltxt2htm::details::is_prefix_match<ndebug, u8'`', u8'`', u8'`'>(pltext) == false) {
            return ::exception::nullopt_t{};
        }
    } else {
        if (::pltxt2htm::details::is_prefix_match<ndebug, u8'~', u8'~', u8'~'>(pltext) == false) {
            return ::exception::nullopt_t{};
        }
    }

    ::fast_io::u8string lang{};
    ::std::size_t current_index{3};
    ::std::size_t const pltext_size{pltext.size()};

    // Parsing language string

    // Skipping spaces before language string
    while (current_index != pltext_size &&
           ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8' ') {
        ++current_index;
    }
    // parsing language string
    for (; current_index != pltext_size; ++current_index) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if (chr == u8'\n') {
            ++current_index;
            break;
        } else if (chr == u8' ') {
            ++current_index;
            if (current_index == pltext_size) {
                // 0042.fuzzing-crash3
                return ::exception::nullopt_t{};
            }
            do {
                ++current_index;
            } while (current_index != pltext_size &&
                     ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8'\n');
            if (current_index == pltext_size) {
                // 0047.fuzzing-crash
                return ::exception::nullopt_t{};
            }
            ++current_index;
            break;
        } else {
            if constexpr (is_backtick) {
                if (::pltxt2htm::details::is_prefix_match<ndebug, u8'`', u8'`', u8'`'>(
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
        auto&& [forward_index, ast_] = ::pltxt2htm::details::simply_parse_pltext<ndebug, u8'\n', u8'`', u8'`', u8'`'>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
        ast = ::std::move(ast_);
        current_index += forward_index;
    } else {
        auto&& [forward_index, ast_] = ::pltxt2htm::details::simply_parse_pltext<ndebug, u8'\n', u8'~', u8'~', u8'~'>(
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
    } else {
        return ::pltxt2htm::details::TryParseMdCodeFenceResult{
            .node = ::pltxt2htm::HeapGuard<::pltxt2htm::MdCodeFenceTilde>{::std::move(ast), ::std::move(opt_lang)},
            .forward_index = current_index};
    }
}

/**
 * @brief Parse Markdown code fences (both backticks and tildes).
 *
 * @param pltext The input text to parse.
 * @return The parsed result, or nullopt if parsing fails.
 */
template<bool ndebug>
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
    } else {
        return ::exception::nullopt_t{};
    }
#endif
}

/**
 * @brief Parse Markdown inline elements enclosed by specific characters.
 *
 * @tparam embraced_chars The characters enclosing the inline elements.
 * @param pltext The input text to parse.
 * @return The length of the parsed inline element, or nullopt if parsing fails.
 */
template<bool ndebug, char8_t... embraced_chars>
[[nodiscard]]
constexpr auto try_parse_md_inlines(::fast_io::u8string_view pltext) noexcept -> ::exception::optional<::std::size_t> {
    if (!::pltxt2htm::details::is_prefix_match<ndebug, embraced_chars...>(pltext)) {
        return ::exception::nullopt_t{};
    }

    for (::std::size_t current_index{sizeof...(embraced_chars)}; current_index < pltext.size(); ++current_index) {
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8'\n') {
            return ::exception::nullopt_t{};
        }
        if (::pltxt2htm::details::is_prefix_match<ndebug, embraced_chars...>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index))) {
            ::std::size_t result{current_index - sizeof...(embraced_chars)};
            if (result == 0) {
                return ::exception::nullopt_t{};
            } else {
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
 * @brief Parse Markdown block quotes (e.g., `> Block quote`).
 *
 * @param pltext The input text to parse.
 * @return The parsed result, or nullopt if parsing fails.
 */
template<bool ndebug>
[[nodiscard]]
constexpr auto try_parse_md_block_quotes(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdBlockQuotesResult> {
    ::fast_io::u8string subpltext{};

    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t current_index{};
    for (; current_index < pltext_size; ++current_index) {
        ::std::size_t temp_index{current_index};
        while (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, temp_index) == u8' ' &&
               temp_index + 1 < pltext_size) {
            ++temp_index;
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, temp_index) != u8'>') {
            if (subpltext.empty()) {
                return ::exception::nullopt_t{};
            } else {
                return ::pltxt2htm::details::TryParseMdBlockQuotesResult{.forward_index = current_index,
                                                                         .subpltext = ::std::move(subpltext)};
            }
        } else {
            current_index = temp_index + 1;
        }
        while (current_index < pltext_size &&
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8' ') {
            ++current_index;
        }
        if (current_index == pltext_size) {
            break;
        }
        do {
            subpltext.push_back(::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index));
            ++current_index;
        } while (current_index < pltext_size &&
                 ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8'\n');
        if (current_index == pltext_size) {
            break;
        } else {
            subpltext.push_back(u8'\n');
        }
    }
    if (subpltext.empty()) {
        return ::exception::nullopt_t{};
    } else {
        return ::pltxt2htm::details::TryParseMdBlockQuotesResult{.forward_index = current_index,
                                                                 .subpltext = ::std::move(subpltext)};
    }
}

struct TryParseMdCodeSpanResult {
    ::std::size_t forward_index; ///< Index to continue parsing from.
    ::pltxt2htm::Ast subast; ///< Parsed AST for the code span.
};

/**
 * @brief Parse Markdown code spans (e.g., `` `code` ``).
 *
 * @tparam embraced_string The string enclosing the code span.
 * @param pltext The input text to parse.
 * @return The parsed result, or nullopt if parsing fails.
 */
template<bool ndebug, char8_t... embraced_string>
[[nodiscard]]
constexpr auto try_parse_md_code_span(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdCodeSpanResult> {
    if (!::pltxt2htm::details::is_prefix_match<ndebug, embraced_string...>(pltext)) {
        return ::exception::nullopt_t{};
    }

    auto&& [forward_index, ast] = ::pltxt2htm::details::simply_parse_pltext<ndebug, embraced_string...>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, sizeof...(embraced_string)));

    if constexpr (sizeof...(embraced_string) == 1) {
        return ::pltxt2htm::details::TryParseMdCodeSpanResult{
            .forward_index = forward_index + sizeof...(embraced_string), .subast = ::std::move(ast)};
    } else if constexpr (sizeof...(embraced_string) == 2) {
        return ::pltxt2htm::details::TryParseMdCodeSpanResult{
            .forward_index = forward_index + sizeof...(embraced_string), .subast = ::std::move(ast)};
    } else if constexpr (sizeof...(embraced_string) == 3) {
        return ::pltxt2htm::details::TryParseMdCodeSpanResult{
            .forward_index = forward_index + sizeof...(embraced_string), .subast = ::std::move(ast)};
    } else {
        ::exception::unreachable();
    }
}

struct TryParseMdLinkResult {
    ::std::size_t forward_index;
    ::fast_io::u8string_view link_text;
    ::fast_io::u8string link_url;
};

template<bool ndebug>
[[nodiscard]]
constexpr auto try_parse_md_link(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdLinkResult> {
    if (pltext.size() < 4 || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 0) != u8'[') {
        return ::exception::nullopt_t{};
    }

    ::std::size_t current_index{1};

    // Parse link text
    while (current_index < pltext.size() &&
           ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8']') {
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8'\\') {
            ++current_index;
        }
        ++current_index;
    }

    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8']') {
        return ::exception::nullopt_t{};
    }
    ::std::size_t link_text_end = current_index;
    ++current_index;

    // Ensure the next character is '('
    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8'(') {
        return ::exception::nullopt_t{};
    }
    ++current_index;
    ::std::size_t link_url_start = current_index;

    // Parse link URL
    while (current_index < pltext.size() &&
           ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8')') {
        ++current_index;
    }

    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8')') {
        return ::exception::nullopt_t{};
    }
    ::std::size_t link_url_end = current_index;
    ++current_index;
    auto link_url_view = pltext.subview(link_url_start, link_url_end - link_url_start);
    auto link_url = ::fast_io::u8string(link_url_view.begin(), link_url_view.end());
    return ::pltxt2htm::details::TryParseMdLinkResult{.forward_index = current_index,
                                                      .link_text = pltext.subview(1, link_text_end - 1),
                                                      .link_url = ::std::move(link_url)};
}

} // namespace pltxt2htm::details
