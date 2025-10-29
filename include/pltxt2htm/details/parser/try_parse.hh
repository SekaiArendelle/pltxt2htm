#pragma once

#include <exception/exception.hh>
#include "../utils.hh"
#include "../../heap_guard.hh"
#include "../../astnode/basic.hh"
#include "../../astnode/node_type.hh"
#include "../../astnode/html_node.hh"
#include "../../astnode/markdown_node.hh"

namespace pltxt2htm::details {

/**
 * @brief Switch to a markdown punctuation character.
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
 * @tparam ndebug  When `true`, runtime assertions are disabled.
 * @param pltext   The complete input text being parsed.
 */
template<bool ndebug>
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
 * @brief same as regex /^tag_name *\>/
 * @example parsing <i>, <b>, <h3> e.t.c
 * @tparam tag_name: The prefix string.
 * @param pltext: The string to be checked.
 * @return Whether the string is a prefix of the pl-text.
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
    ::std::size_t tag_len;
    ::fast_io::u8string substr;
};

/**
 * @brief parsing `Tag=$1>`
 * @param func: Check whether the character append to substr is valid
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
 * @brief try to parsing `<tag_name>` or `<tag_name/>`
 * @param[in] pltext: source text
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
    ::std::size_t start_index;
    ::std::size_t sublength;
    ::std::size_t forward_index;
    ::pltxt2htm::NodeType md_atx_heading_type;
};

/**
 * @brief Parsing markdown ATX headings
 * @param[in] pltext: input string
 * @param[out] start_index_: start index
 * @param[out] sublength_: length of the heading context
 * @param[out] md_atx_heading_type_: md_atx_h1 ~ md_atx_h6
 * @param[out] ending_type: indicates how the heading ends
 * @return true if the parsing is successful
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
    none = 0,
    // -
    hyphen,
    // _
    underscore,
    // *
    asterisk,
};

/**
 * @brief Parsing markdown thematic breaks (e.g. `---`, `___`, `***`)
 * @param text The text to parse
 * @return length of the parsed markdown thematic breaks
 */
template<bool ndebug>
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

struct TryParseMdCodeFenceResult {
    ::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode> node;
    ::std::size_t forward_index;
};

template<bool ndebug, bool is_backtick>
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

    ::pltxt2htm::Ast ast{};
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
            if (current_index + 1 == pltext_size) {
                // 0042.fuzzing-crash3
                return ::exception::nullopt_t{};
            }
            ++current_index;
            do {
                ++current_index;
            } while (current_index != pltext_size &&
                     ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8'\n');
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
    for (; current_index < pltext.size(); ++current_index) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};

        if (chr == u8'\n') {
            if constexpr (is_backtick) {
                if (::pltxt2htm::details::is_prefix_match<ndebug, u8'`', u8'`', u8'`'>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1))) {
                    break;
                }
            } else {
                if (::pltxt2htm::details::is_prefix_match<ndebug, u8'~', u8'~', u8'~'>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1))) {
                    break;
                }
            }
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

    ::exception::optional<::fast_io::u8string> opt_lang{::exception::nullopt_t{}};
    if (!lang.empty()) {
        opt_lang = ::std::move(lang);
    }
    if constexpr (is_backtick) {
        return ::pltxt2htm::details::TryParseMdCodeFenceResult{
            .node = ::pltxt2htm::HeapGuard<::pltxt2htm::MdCodeFenceBacktick>{::std::move(ast), ::std::move(opt_lang)},
            .forward_index = current_index + 4};
    } else {
        return ::pltxt2htm::details::TryParseMdCodeFenceResult{
            .node = ::pltxt2htm::HeapGuard<::pltxt2htm::MdCodeFenceTilde>{::std::move(ast), ::std::move(opt_lang)},
            .forward_index = current_index + 4};
    }
}

template<bool ndebug>
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

template<bool ndebug, char8_t... embraced_chars>
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
    ::std::size_t forward_index;
    ::fast_io::u8string subpltext;
};

template<bool ndebug>
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
        while (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8' ' &&
               current_index + 1 < pltext_size) {
            ++current_index;
        }
        do {
            subpltext.push_back(::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index));
            ++current_index;
        } while (current_index < pltext_size &&
                 ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8'\n');
    }
    if (subpltext.empty()) {
        return ::exception::nullopt_t{};
    } else {
        return ::pltxt2htm::details::TryParseMdBlockQuotesResult{.forward_index = current_index,
                                                                 .subpltext = ::std::move(subpltext)};
    }
}

} // namespace pltxt2htm::details
