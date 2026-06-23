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
#include "../../ast/ast.hh"
#include "../push_macro.hh"

/**
 * @brief Namespace for internal implementation details.
 */
namespace pltxt2htm::details {

/**
 * @brief Switch to a markdown punctuation character for escape sequences.
 *
 * This function maps escape character sequences (prefixed with backslash) to their corresponding
 * AST node types. It handles all common markdown punctuation characters that can be escaped.
 *
 * @param[in] u8char The UTF-8 character following the backslash in an escape sequence.
 * @return An optional PlTxtNode containing the corresponding escape node, or nullopt if the character
 *         is not a valid escapable character in markdown.
 * @note Supported escape characters include: \\ \! \" \# \$ \% \& \' \( \) \* \+ \, \- \. \/ \: \; \< \= \> \? \@ \[ \]
 * \^ \_ \` \{ \| \} \~
 * @see https://spec.commonmark.org/0.31.2/#backslash-escapes
 */
template<::pltxt2htm::Contracts ndebug>
struct TryParseMdEscapeResult {
    ::pltxt2htm::PlTxtNode<ndebug> node;
    ::std::size_t advance_count;
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_escape(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdEscapeResult<ndebug>> {
    if (pltext.empty()) {
        return ::exception::nullopt_t{};
    }
    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 0) != u8'\\') {
        return ::exception::nullopt_t{};
    }
    if (pltext.size() == 1) {
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{u8'\\'}), 1};
    }
    switch (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1)) {
    case u8'\\':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeBackslash{}), 2};
    case u8'!':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeExclamation{}), 2};
    case u8'\"':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeDoubleQuote{}), 2};
    case u8'#':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeHash{}), 2};
    case u8'$':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeDollar{}), 2};
    case u8'%':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapePercent{}), 2};
    case u8'&':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeAmpersand{}), 2};
    case u8'\'':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeSingleQuote{}), 2};
    case u8'(':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeLeftParen{}), 2};
    case u8')':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeRightParen{}), 2};
    case u8'*':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeAsterisk{}), 2};
    case u8'+':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapePlus{}), 2};
    case u8',':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeComma{}), 2};
    case u8'-':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeHyphen{}), 2};
    case u8'.':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeDot{}), 2};
    case u8'/':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeSlash{}), 2};
    case u8':':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeColon{}), 2};
    case u8';':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeSemicolon{}), 2};
    case u8'<':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeLessThan{}), 2};
    case u8'=':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeEquals{}), 2};
    case u8'>':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeGreaterThan{}), 2};
    case u8'?':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeQuestion{}), 2};
    case u8'@':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeAt{}), 2};
    case u8'[':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeLeftBracket{}), 2};
    case u8']':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeRightBracket{}), 2};
    case u8'^':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeCaret{}), 2};
    case u8'_':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeUnderscore{}), 2};
    case u8'`':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeBacktick{}), 2};
    case u8'{':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeLeftBrace{}), 2};
    case u8'|':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapePipe{}), 2};
    case u8'}':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeRightBrace{}), 2};
    case u8'~':
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeTilde{}), 2};
    default:
        return ::pltxt2htm::details::TryParseMdEscapeResult<ndebug>{
            ::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{u8'\\'}), 1};
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
 * @return Total number of bytes consumed (1..4). The caller should advance by `return_value`.
 * @note ASCII bytes append one U8Char and return 1.
 * @note Control characters 0x00-0x1F and 0x7F produce an InvalidU8Char node.
 * @warning Previously these were silently dropped, which caused a crash when they appeared
 *          inside emphasis structures (***...***, **...**, *...*, etc.): the inline parser
 *          accepted them as valid content, but the sub-AST ended up empty because no node
 *          was emitted, triggering an assertion in the optimizer
 *          ("md_triple_emphasis subast must not be empty"). Emitting InvalidU8Char ensures
 *          the sub-AST is never empty for structural nodes.
 * @note Invalid sequences append one InvalidU8Char. The return value may be greater than 1 when
 *       continuation bytes are consumed as part of one invalid sequence.
 * @see https://en.wikipedia.org/wiki/UTF-8
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_utf8_code_point(::fast_io::u8string_view const& pltext, ::pltxt2htm::Ast<ndebug>& result) noexcept
    -> ::std::size_t {
    ::std::size_t const pltext_size{pltext.size()};
    char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 0)};

    if (chr <= 0x1f || chr == 0x7f) {
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidU8Char{}));
        return 1;
    }
    if ((chr & 0x80) == 0) {
        // normal utf-8 characters
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{chr}));
        return 1;
    }
    if ((chr & 0xE0) == 0xC0) {
        if (1 >= pltext_size) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidU8Char{}));
            return 1;
        }
        auto next_char = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1);
        if ((next_char & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidU8Char{}));
            return 1;
        }
        char32_t const combine{static_cast<char32_t>(chr & 0x1F) << 6 | static_cast<char32_t>(next_char & 0x3F)};
        if (combine < 0x80 || combine > 0x7FF) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidU8Char{}));
            return 2;
        }

        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{chr}));
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{next_char}));
        return 2;
    }
    if ((chr & 0xF0) == 0xE0) {
        if (2 >= pltext_size) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidU8Char{}));
            if (pltext_size != 2) {
                return 1;
            }
            auto next_char = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1);
            if ((next_char & 0xC0) == 0x80) {
                return 2;
            }
            return 1;
        }
        auto next_char = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1);
        if ((next_char & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidU8Char{}));
            return 1;
        }
        auto next_char2 = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 2);
        if ((next_char2 & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidU8Char{}));
            return 2;
        }
        char32_t const combine{static_cast<char32_t>(chr & 0x0f) << 12 | static_cast<char32_t>(next_char & 0x3f) << 6 |
                               static_cast<char32_t>(next_char2 & 0x3f)};
        if (combine < 0x800 || combine > 0xffff) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidU8Char{}));
            return 3;
        }
        if (0xd800 <= combine && combine <= 0xdfff) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidU8Char{}));
            return 3;
        }

        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{chr}));
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{next_char}));
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{next_char2}));
        return 3;
    }
    if ((chr & 0xF8) == 0xF0) {
        if (3 >= pltext_size) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidU8Char{}));
            if (pltext_size < 2) {
                return 1;
            }
            auto next_char = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1);
            if ((next_char & 0xC0) != 0x80) {
                return 1;
            }
            if (pltext_size < 3) {
                return 2;
            }
            auto next_char2 = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 2);
            if ((next_char2 & 0xC0) == 0x80) {
                return 3;
            }
            return 2;
        }
        auto next_char = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1);
        if ((next_char & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidU8Char{}));
            return 1;
        }
        auto next_char2 = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 2);
        if ((next_char2 & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidU8Char{}));
            return 2;
        }
        auto next_char3 = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 3);
        if ((next_char3 & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidU8Char{}));
            return 3;
        }
        char32_t const combine{static_cast<char32_t>(chr & 0x07) << 18 | static_cast<char32_t>(next_char & 0x3F) << 12 |
                               static_cast<char32_t>(next_char2 & 0x3F) << 6 |
                               static_cast<char32_t>(next_char3 & 0x3F)};
        if (combine < 0x10000 || combine > 0x10FFFF) {
            result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidU8Char{}));
            return 4;
        }
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{chr}));
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{next_char}));
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{next_char2}));
        result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{next_char3}));
        return 4;
    }
    result.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::InvalidU8Char{}));
    return 1;
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
        if (forward_chr != u8' ' && forward_chr != u8'\t') {
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
[[nodiscard]]
constexpr auto try_parse_li_tag(::fast_io::u8string_view pltext, ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::exception::optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_bare_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"i"}>(pltext);
    if (!opt_tag_len.has_value()) {
        return ::exception::nullopt_t{};
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_ul && nested_tag_type != ::pltxt2htm::NodeKind::html_ol) {
        return ::exception::nullopt_t{};
    }
    return opt_tag_len;
}

/**
 * @brief Parse &lt;caption&gt; and validate parent container type.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting after `<c`.
 * @param[in] nested_tag_type Current parent tag type from parsing context.
 * @return Matched tag length when valid under &lt;table&gt;; otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_caption_tag(::fast_io::u8string_view pltext,
                                     ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::exception::optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_bare_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"aption"}>(pltext);
    if (!opt_tag_len.has_value()) {
        return ::exception::nullopt_t{};
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_table) {
        return ::exception::nullopt_t{};
    }
    return opt_tag_len;
}

/**
 * @brief Parse &lt;colgroup&gt; and validate parent container type.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting after `<c`.
 * @param[in] nested_tag_type Current parent tag type from parsing context.
 * @return Matched tag length when valid under &lt;table&gt;; otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_colgroup_tag(::fast_io::u8string_view pltext,
                                      ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::exception::optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_bare_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"olgroup"}>(pltext);
    if (!opt_tag_len.has_value()) {
        return ::exception::nullopt_t{};
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_table) {
        return ::exception::nullopt_t{};
    }
    return opt_tag_len;
}

/**
 * @brief Parse &lt;thead&gt; and validate parent container type.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting after `<t`.
 * @param[in] nested_tag_type Current parent tag type from parsing context.
 * @return Matched tag length when valid under &lt;table&gt;; otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_thead_tag(::fast_io::u8string_view pltext,
                                   ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::exception::optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_bare_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"head"}>(pltext);
    if (!opt_tag_len.has_value()) {
        return ::exception::nullopt_t{};
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_table) {
        return ::exception::nullopt_t{};
    }
    return opt_tag_len;
}

/**
 * @brief Parse &lt;tbody&gt; and validate parent container type.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting after `<t`.
 * @param[in] nested_tag_type Current parent tag type from parsing context.
 * @return Matched tag length when valid under &lt;table&gt;; otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_tbody_tag(::fast_io::u8string_view pltext,
                                   ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::exception::optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_bare_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"body"}>(pltext);
    if (!opt_tag_len.has_value()) {
        return ::exception::nullopt_t{};
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_table) {
        return ::exception::nullopt_t{};
    }
    return opt_tag_len;
}

/**
 * @brief Parse &lt;tfoot&gt; and validate parent container type.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting after `<t`.
 * @param[in] nested_tag_type Current parent tag type from parsing context.
 * @return Matched tag length when valid under &lt;table&gt;; otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_tfoot_tag(::fast_io::u8string_view pltext,
                                   ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::exception::optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_bare_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"foot"}>(pltext);
    if (!opt_tag_len.has_value()) {
        return ::exception::nullopt_t{};
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_table) {
        return ::exception::nullopt_t{};
    }
    return opt_tag_len;
}

/**
 * @brief Parse &lt;tr&gt; and validate parent container type.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting after `<t`.
 * @param[in] nested_tag_type Current parent tag type from parsing context.
 * @return Matched tag length when valid under &lt;table&gt;,&lt;thead&gt;,&lt;tbody&gt;,or &lt;tfoot&gt;; otherwise
 * nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_tr_tag(::fast_io::u8string_view pltext, ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::exception::optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_bare_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"r"}>(pltext);
    if (!opt_tag_len.has_value()) {
        return ::exception::nullopt_t{};
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_table && nested_tag_type != ::pltxt2htm::NodeKind::html_thead &&
        nested_tag_type != ::pltxt2htm::NodeKind::html_tbody && nested_tag_type != ::pltxt2htm::NodeKind::html_tfoot) {
        return ::exception::nullopt_t{};
    }
    return opt_tag_len;
}

/**
 * @brief Parse &lt;th&gt; and validate parent container type.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting after `<t`.
 * @param[in] nested_tag_type Current parent tag type from parsing context.
 * @return Matched tag length when valid under &lt;tr&gt;; otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_th_tag(::fast_io::u8string_view pltext, ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::exception::optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_bare_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"h"}>(pltext);
    if (!opt_tag_len.has_value()) {
        return ::exception::nullopt_t{};
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_tr) {
        return ::exception::nullopt_t{};
    }
    return opt_tag_len;
}

/**
 * @brief Parse a CSS text-align value to a MdTableAlign.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled.
 * @param[in] value The text-align value string (e.g. "center", "CENTER").
 * @return The corresponding alignment on success, or nullopt on failure.
 */
[[nodiscard]]
constexpr auto parse_text_align_value(::fast_io::u8string_view value) noexcept
    -> ::exception::optional<::pltxt2htm::MdTableAlign> {
    // only exact lowercase values accepted
    if (value == ::fast_io::u8string_view{u8"left"}) {
        return ::pltxt2htm::MdTableAlign::left;
    }
    if (value == ::fast_io::u8string_view{u8"center"}) {
        return ::pltxt2htm::MdTableAlign::center;
    }
    if (value == ::fast_io::u8string_view{u8"right"}) {
        return ::pltxt2htm::MdTableAlign::right;
    }
    return ::exception::nullopt_t{};
}

struct TryParseTdTagResult {
    ::std::size_t tag_len; ///< Length of the matched tag up to the closing `>`.
    ::pltxt2htm::MdTableAlign align; ///< Cell alignment parsed from `style="text-align:..."`.
};

/**
 * @brief Parse &lt;td&gt; (optionally with `style="text-align:..."`) and validate parent container type.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting after `<t`.
 * @param[in] nested_tag_type Current parent tag type from parsing context.
 * @return Parsed tag length and alignment when valid under &lt;tr&gt;; otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_td_tag(::fast_io::u8string_view pltext, ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::exception::optional<TryParseTdTagResult> {
    if (!::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"d"}>(pltext)) {
        return ::exception::nullopt_t{};
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_tr) {
        return ::exception::nullopt_t{};
    }

    ::std::size_t pos{1}; // skip past "d"
    ::pltxt2htm::MdTableAlign align{::pltxt2htm::MdTableAlign::left};

    while (pos < pltext.size()) {
        // skip whitespace
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt_t{};
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'>') {
            return TryParseTdTagResult{pos, align};
        }

        // parse attribute name
        ::std::size_t const attr_start{pos};
        while (pos < pltext.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'=' &&
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'>' &&
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8' ' &&
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'\t' &&
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'/') {
            ++pos;
        }
        ::fast_io::u8string_view const attr_name{pltext.data() + attr_start, pos - attr_start};

        // skip whitespace before '='
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size() || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'=') {
            return ::exception::nullopt_t{};
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt_t{};
        }

        // parse quoted attribute value
        char8_t const quote{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos)};
        if (quote != u8'"' && quote != u8'\'') {
            return ::exception::nullopt_t{};
        }
        ++pos; // skip opening quote
        ::std::size_t const val_start{pos};
        while (pos < pltext.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != quote) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt_t{};
        }
        ::fast_io::u8string_view const attr_val{pltext.data() + val_start, pos - val_start};
        ++pos; // skip closing quote

        // only lowercase "style" attribute is checked for text-align
        if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"style"}>(
                attr_name) &&
            attr_name.size() == 5) {
            // parse CSS property:value pairs from the style value
            ::std::size_t css_pos{};
            while (css_pos < attr_val.size()) {
                // skip leading whitespace
                while (css_pos < attr_val.size() &&
                       (::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, css_pos) == u8' ' ||
                        ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, css_pos) == u8'\t')) {
                    ++css_pos;
                }
                if (css_pos >= attr_val.size()) {
                    break;
                }

                // find next ';' or end of value
                ::std::size_t const seg_start{css_pos};
                while (css_pos < attr_val.size() &&
                       ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, css_pos) != u8';') {
                    ++css_pos;
                }
                auto segment = ::fast_io::u8string_view{attr_val.data() + seg_start, css_pos - seg_start};
                if (css_pos < attr_val.size() &&
                    ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, css_pos) == u8';') {
                    ++css_pos; // skip ';'
                }

                // trim trailing whitespace
                while (!segment.empty() && (segment.back() == u8' ' || segment.back() == u8'\t')) {
                    segment = ::fast_io::u8string_view{segment.data(), segment.size() - 1};
                }
                if (segment.empty()) {
                    continue;
                }

                // find ':' to split property:value
                ::std::size_t colon{};
                while (colon < segment.size() &&
                       ::pltxt2htm::details::u8string_view_index<ndebug>(segment, colon) != u8':') {
                    ++colon;
                }
                if (colon == 0 || colon >= segment.size()) {
                    continue; // malformed CSS segment, ignore
                }

                // extract property name
                auto prop = ::fast_io::u8string_view{segment.data(), colon};
                while (!prop.empty() && (prop.back() == u8' ' || prop.back() == u8'\t')) {
                    prop = ::fast_io::u8string_view{prop.data(), prop.size() - 1};
                }

                // extract value
                ::std::size_t val_idx{colon + 1};
                while (val_idx < segment.size() &&
                       (::pltxt2htm::details::u8string_view_index<ndebug>(segment, val_idx) == u8' ' ||
                        ::pltxt2htm::details::u8string_view_index<ndebug>(segment, val_idx) == u8'\t')) {
                    ++val_idx;
                }
                auto val = ::fast_io::u8string_view{segment.data() + val_idx, segment.size() - val_idx};
                while (!val.empty() && (val.back() == u8' ' || val.back() == u8'\t')) {
                    val = ::fast_io::u8string_view{val.data(), val.size() - 1};
                }

                // only lowercase "text-align" property is recognized
                if (::pltxt2htm::details::is_prefix_match<ndebug, u8"text-align">(prop) && prop.size() == 10) {
                    auto opt_align_val = ::pltxt2htm::details::parse_text_align_value(val);
                    if (opt_align_val.has_value()) {
                        align = opt_align_val.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                    }
                    else {
                        // invalid text-align value → reject tag
                        return ::exception::nullopt_t{};
                    }
                }
                else {
                    // unknown CSS property → reject tag
                    return ::exception::nullopt_t{};
                }
            }
        }
        // else: unknown attribute, ignore
    }
    return ::exception::nullopt_t{};
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
    ::std::size_t const prefix_size{prefix_str.size()};
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
                // e.g. `<size=>text` is invalid (empty value in equal-sign tag)
                return ::exception::nullopt_t{};
            }
            return ::pltxt2htm::details::TryParseEqualSignTagResult{forward_index, ::std::move(substr)};
        }
        if (forward_chr == u8' ' || forward_chr == u8'\t') {
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
                        // <size= >text is invalid (empty value in equal-sign tag)
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
    ::fast_io::stack<::pltxt2htm::details::ParserFrameContext<ndebug>> const& call_stack) noexcept
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
        if (nested_tag_type == ::pltxt2htm::NodeKind::pl_experiment ||
            nested_tag_type == ::pltxt2htm::NodeKind::pl_discussion ||
            nested_tag_type == ::pltxt2htm::NodeKind::pl_external) {
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

struct TryParseSpanTagResult {
    ::std::size_t tag_len;
    ::fast_io::u8string color;
    ::exception::optional<::std::size_t> font_size;
};

/**
 * @brief Parse a CSS font-size value to a numeric size_t.
 * @details Accepts integer values optionally followed by "px" (case-insensitive).
 *          Rejects values with decimals or non-px units.
 * @tparam ndebug When set to Contracts::ignore, runtime assertions are disabled.
 * @param[in] value The font-size value string (e.g. "20px", "20").
 * @return The numeric value on success, or nullopt on failure.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_font_size_value(::fast_io::u8string_view value) noexcept -> ::exception::optional<::std::size_t> {
    if (value.empty()) {
        return ::exception::nullopt_t{};
    }
    ::std::size_t len{value.size()};
    // strip optional "px" suffix (lowercase only)
    if (len >= 2 && ::pltxt2htm::details::u8string_view_index<ndebug>(value, len - 2) == u8'p' &&
        ::pltxt2htm::details::u8string_view_index<ndebug>(value, len - 1) == u8'x') {
        len -= 2;
    }
    ::exception::optional<::std::size_t> result{
        ::pltxt2htm::details::u8str2size_t(::fast_io::u8string_view{value.data(), len})};
    if (result == 0) {
        return ::exception::nullopt_t{};
    }
    return result;
}

/**
 * @brief Parse &lt;span style="color:V;font-size:S"&gt; and reject any other attributes or CSS properties.
 * @tparam ndebug When set to Contracts::ignore, runtime assertions are disabled for performance.
 * @param[in] pltext Input text starting at "pan ..." (after "<s").
 * @return Parsed tag result with color and/or font_size on success; nullopt on failure.
 * @note Only the lowercase "style" attribute is accepted. Within style, only lowercase "color" and
 *       "font-size" CSS properties are accepted. Any other attribute or CSS property causes parse failure.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_span_tag(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<TryParseSpanTagResult> {
    // match "pan" prefix (case-insensitive)
    if (!::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"pan"}>(pltext)) {
        return ::exception::nullopt_t{};
    }

    ::std::size_t pos{4}; // skip past "span" (the 's' was consumed by the trie dispatch)
    bool found_style{false};
    ::fast_io::u8string color{};
    ::exception::optional<::std::size_t> font_size{::exception::nullopt_t{}};

    while (pos < pltext.size()) {
        // skip whitespace
        while (pos < pltext.size() &&
               (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt_t{};
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'>') {
            break;
        }

        // parse attribute name
        ::std::size_t const attr_start = pos;
        while (pos < pltext.size() &&
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'=' &&
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'>' &&
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8' ' &&
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'\t' &&
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'/') {
            ++pos;
        }
        ::fast_io::u8string_view const attr_name{pltext.data() + attr_start, pos - attr_start};

        // skip whitespace before '='
        while (pos < pltext.size() &&
               (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size() ||
            ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'=') {
            return ::exception::nullopt_t{};
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext.size() &&
               (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt_t{};
        }

        // parse quoted attribute value
        char8_t const quote{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos)};
        if (quote != u8'"' && quote != u8'\'') {
            return ::exception::nullopt_t{};
        }
        ++pos; // skip opening quote
        ::std::size_t const val_start = pos;
        while (pos < pltext.size() &&
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != quote) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt_t{};
        }
        ::fast_io::u8string_view const attr_val{pltext.data() + val_start, pos - val_start};
        ++pos; // skip closing quote

        // only lowercase "style" attribute is allowed
        if (attr_name != ::fast_io::u8string_view{u8"style"}) {
            return ::exception::nullopt_t{};
        }
        found_style = true;

        // parse CSS property:value pairs from the style value
        ::std::size_t css_pos = 0;
        while (css_pos < attr_val.size()) {
            // skip leading whitespace
            while (css_pos < attr_val.size() &&
                   (::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, css_pos) == u8' ' ||
                    ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, css_pos) == u8'\t')) {
                ++css_pos;
            }
            if (css_pos >= attr_val.size()) {
                break;
            }
            // find next ';' or end of value
            ::std::size_t prop_end = css_pos;
            while (prop_end < attr_val.size() &&
                   ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, prop_end) != u8';') {
                ++prop_end;
            }
            auto segment = ::fast_io::u8string_view{attr_val.data() + css_pos, prop_end - css_pos};
            css_pos = prop_end;
            if (css_pos < attr_val.size() &&
                ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, css_pos) == u8';') {
                ++css_pos; // skip ';'
            }

            // trim trailing whitespace from segment
            while (!segment.empty() && (segment.back() == u8' ' || segment.back() == u8'\t')) {
                segment = ::fast_io::u8string_view{segment.data(), segment.size() - 1};
            }
            if (segment.empty()) {
                continue;
            }

            // find ':' to split property:value
            ::std::size_t colon = 0;
            while (colon < segment.size() &&
                   ::pltxt2htm::details::u8string_view_index<ndebug>(segment, colon) != u8':') {
                ++colon;
            }
            if (colon == 0 || colon >= segment.size()) {
                return ::exception::nullopt_t{};
            }

            // extract property name (lowercase only)
            auto prop = ::fast_io::u8string_view{segment.data(), colon};
            while (!prop.empty() && (prop.back() == u8' ' || prop.back() == u8'\t')) {
                prop = ::fast_io::u8string_view{prop.data(), prop.size() - 1};
            }

            // extract value
            ::std::size_t val_idx = colon + 1;
            while (val_idx < segment.size() &&
                   (::pltxt2htm::details::u8string_view_index<ndebug>(segment, val_idx) == u8' ' ||
                    ::pltxt2htm::details::u8string_view_index<ndebug>(segment, val_idx) == u8'\t')) {
                ++val_idx;
            }
            auto val = ::fast_io::u8string_view{segment.data() + val_idx, segment.size() - val_idx};

            // trim trailing whitespace
            while (!val.empty() && (val.back() == u8' ' || val.back() == u8'\t')) {
                val = ::fast_io::u8string_view{val.data(), val.size() - 1};
            }

            // match property
            if (prop == ::fast_io::u8string_view{u8"color"}) {
                if (!color.empty()) {
                    return ::exception::nullopt_t{}; // duplicate color
                }
                if (!::pltxt2htm::details::is_valid_color<ndebug>(val)) {
                    return ::exception::nullopt_t{};
                }
                color = ::fast_io::u8string{val};
            }
            else if (prop == ::fast_io::u8string_view{u8"font-size"}) {
                if (font_size.has_value()) {
                    return ::exception::nullopt_t{}; // duplicate font-size
                }
                auto opt_fs = ::pltxt2htm::details::parse_font_size_value<ndebug>(val);
                if (!opt_fs.has_value()) {
                    return ::exception::nullopt_t{};
                }
                font_size = opt_fs.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
            else {
                return ::exception::nullopt_t{}; // unknown CSS property
            }
        }
    }

    if (!found_style || (color.empty() && !font_size.has_value())) {
        return ::exception::nullopt_t{};
    }
    if (pos >= pltext.size() || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'>') {
        return ::exception::nullopt_t{};
    }
    return TryParseSpanTagResult{pos + 1, ::std::move(color), ::std::move(font_size)};
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
            return forward_index + 1;
        }
        if (forward_chr == u8'/' && forward_index + 1 < pltext.size() &&
            ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index + 1) == u8'>') {
            return forward_index + 2;
        }
        if (forward_chr != u8' ' && forward_chr != u8'\t') {
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
 * @return Length of the matched self-closing tag, otherwise nullopt.
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
            return forward_index + 1;
        }
        if (forward_chr == u8'/' && forward_index + 1 < pltext.size() &&
            ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, forward_index + 1) == u8'>') {
            return forward_index + 2;
        }
        if (forward_chr != u8' ' && forward_chr != u8'\t') {
            return ::exception::nullopt_t{};
        }
    }
    return ::exception::nullopt_t{};
}

/**
 * @brief Parse &lt;col&gt; self-closing tag and validate parent container type.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting after `<c`.
 * @param[in] nested_tag_type Current parent tag type from parsing context.
 * @return Matched tag length when valid under &lt;colgroup&gt;; otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_col_tag(::fast_io::u8string_view pltext, ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::exception::optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"ol"}>(pltext);
    if (!opt_tag_len.has_value()) {
        return ::exception::nullopt_t{};
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_colgroup) {
        return ::exception::nullopt_t{};
    }
    return opt_tag_len;
}

/**
 * @brief Match a PL-text line terminator: either `\n` or a `<br` self-closing tag.
 *
 * Returns the byte length of the terminator (1 for `\n`, or the total tag length for `<br>`, `<br/>`, `<br />`, etc.).
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext Input view starting at the current parser position.
 * @return Length of the matched line terminator, or nullopt if neither `\n` nor `<br` tag is found.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_pltext_line_break(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::std::size_t> {
    if (pltext.empty()) {
        return ::exception::nullopt_t{};
    }
    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 0) == u8'\n') {
        return ::std::size_t{1};
    }
    return ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"<br"}>(
        pltext);
}

struct TryParseMdAtxHeadingResult {
    ::std::size_t start_index; ///< Start index of the heading content.
    ::std::size_t sublength; ///< Length of the heading content.
    ::std::size_t advance_count; ///< Number of characters consumed.
    ::pltxt2htm::NodeKind md_atx_heading_type; ///< Type of the ATX heading.
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
    ::std::size_t md_atx_heading_type{static_cast<::std::size_t>(::pltxt2htm::NodeKind::md_atx_h1) - 1};
    while (true) {
        if (start_index >= pltext_size) {
            // https://spec.commonmark.org/0.31.2/#example-79
            if (static_cast<::std::size_t>(::pltxt2htm::NodeKind::md_atx_h1) <= md_atx_heading_type &&
                md_atx_heading_type <= static_cast<::std::size_t>(::pltxt2htm::NodeKind::md_atx_h6)) {
                return ::pltxt2htm::details::TryParseMdAtxHeadingResult{
                    start_index, 0, start_index, static_cast<::pltxt2htm::NodeKind>(md_atx_heading_type)};
            }
            return ::exception::nullopt_t{};
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index) != u8'#') {
            break;
        }
        ++start_index;
        ++md_atx_heading_type;
    }
    if (md_atx_heading_type < static_cast<::std::size_t>(::pltxt2htm::NodeKind::md_atx_h1) ||
        static_cast<::std::size_t>(::pltxt2htm::NodeKind::md_atx_h6) < md_atx_heading_type ||
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
                start_index, 0, start_index, static_cast<::pltxt2htm::NodeKind>(md_atx_heading_type)};
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
        if (auto opt_line_break = ::pltxt2htm::details::try_parse_pltext_line_break<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, end_index));
            opt_line_break.has_value()) {
            extra_length = opt_line_break.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            break;
        }
    }
    return ::pltxt2htm::details::TryParseMdAtxHeadingResult{
        .start_index = start_index,
        .sublength = end_index - start_index,
        .advance_count = end_index + extra_length,
        .md_atx_heading_type = static_cast<::pltxt2htm::NodeKind>(md_atx_heading_type)};
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
    ::std::size_t thematic_break_count{};
    ::std::size_t i{};
    for (; i < text.size(); ++i) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(text, i)};
        if (chr == u8' ' || chr == u8'\t') {
            continue;
        }
        if (chr == u8'*') {
            if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::asterisk) {
                ++thematic_break_count;
                continue;
            }
            if (thematic_break_type != ::pltxt2htm::details::ThematicBreakType::none) {
                return ::exception::nullopt_t{};
            }
            thematic_break_type = ::pltxt2htm::details::ThematicBreakType::asterisk;
            ++thematic_break_count;
            continue;
        }
        if (chr == u8'-') {
            if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::hyphen) {
                ++thematic_break_count;
                continue;
            }
            if (thematic_break_type != ::pltxt2htm::details::ThematicBreakType::none) {
                return ::exception::nullopt_t{};
            }
            thematic_break_type = ::pltxt2htm::details::ThematicBreakType::hyphen;
            ++thematic_break_count;
            continue;
        }
        if (chr == u8'_') {
            if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::underscore) {
                ++thematic_break_count;
                continue;
            }
            if (thematic_break_type != ::pltxt2htm::details::ThematicBreakType::none) {
                return ::exception::nullopt_t{};
            }
            thematic_break_type = ::pltxt2htm::details::ThematicBreakType::underscore;
            ++thematic_break_count;
            continue;
        }
        if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::none) {
            return ::exception::nullopt_t{};
        }
        auto opt_line_break = ::pltxt2htm::details::try_parse_pltext_line_break<ndebug>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(text, i));
        if (opt_line_break.has_value() == false) {
            return ::exception::nullopt_t{};
        }
        if (thematic_break_count < 3) {
            return ::exception::nullopt_t{};
        }
        return i + opt_line_break.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    }
    if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::none) {
        return ::exception::nullopt_t{};
    }
    if (thematic_break_count < 3) {
        return ::exception::nullopt_t{};
    }
    return i;
}

template<::pltxt2htm::Contracts ndebug>
struct SimplyParsePLtextResult {
    ::std::size_t advance_count; ///< Number of characters consumed.
    ::pltxt2htm::Ast<ndebug> ast; ///< Parsed AST.
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
    -> ::pltxt2htm::details::SimplyParsePLtextResult<ndebug> {
    ::pltxt2htm::Ast<ndebug> ast{};
    ::std::size_t current_index{};
    constexpr ::std::size_t end_size{end_string.size()};

    while (current_index < pltext.size()) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};

        if (::pltxt2htm::details::is_prefix_match<ndebug, end_string>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index))) {
            current_index += end_size;
            break;
        }

        if (chr == u8'\n') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LineBreak{}));
            ++current_index;
            continue;
        }
        if (chr == u8' ') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Space{}));
            ++current_index;
            continue;
        }
        if (chr == u8'&') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Ampersand{}));
            ++current_index;
            continue;
        }
        if (chr == u8'\'') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::SingleQuote{}));
            ++current_index;
            continue;
        }
        if (chr == u8'\"') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::DoubleQuote{}));
            ++current_index;
            continue;
        }
        if (chr == u8'>') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::GreaterThan{}));
            ++current_index;
            continue;
        }
        if (chr == u8'\t') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Tab{}));
            ++current_index;
            continue;
        }
        if (auto opt_escape = ::pltxt2htm::details::try_parse_md_escape<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_escape.has_value()) {
            auto&& [node, advance_count] = opt_escape.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            ast.push_back(::std::move(node));
            current_index += advance_count;
            continue;
        }
        if (chr == u8'<') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
            ++current_index;
            continue;
        }
        auto advance_count = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index), ast);
        current_index += advance_count;
        continue;
    }
    return {.advance_count = current_index, .ast = ::std::move(ast)};
}

template<::pltxt2htm::Contracts ndebug>
struct TryParseMdCodeFenceResult {
    ::pltxt2htm::PlTxtNode<ndebug> node; ///< Parsed code fence node.
    ::std::size_t advance_count; ///< Number of characters consumed.
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
    -> ::exception::optional<::pltxt2htm::details::TryParseMdCodeFenceResult<ndebug>> {
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
                    return ::pltxt2htm::details::TryParseMdCodeFenceResult<ndebug>{
                        .node =
                            ::pltxt2htm::MdCodeFenceBacktick<ndebug>{::pltxt2htm::Ast<ndebug>{}, ::std::move(opt_lang)},
                        .advance_count = current_index + 3,
                    };
                }
                else {
                    return ::pltxt2htm::details::TryParseMdCodeFenceResult<ndebug>{
                        .node =
                            ::pltxt2htm::MdCodeFenceTilde<ndebug>{::pltxt2htm::Ast<ndebug>{}, ::std::move(opt_lang)},
                        .advance_count = current_index + 3,
                    };
                }
            }
            break;
        }
        if (chr == u8' ' || chr == u8'\t') {
            ++current_index;
            if (current_index == pltext_size) {
                // space/tab at end of input after language: not a valid code fence
                return ::exception::nullopt_t{};
            }
            while (current_index != pltext_size &&
                   ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8'\n') {
                ++current_index;
            }
            if (current_index == pltext_size) {
                // content after space/tab without newline: not a valid code fence
                return ::exception::nullopt_t{};
            }
            ++current_index;
            break;
        }
        if constexpr (is_backtick) {
            if (::pltxt2htm::details::is_prefix_match<ndebug, fence>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index))) {
                return ::exception::nullopt_t{};
            }
        }
        lang.push_back(chr);
    }

    // parsing context of code fence
    ::pltxt2htm::Ast<ndebug> ast{};
    if constexpr (is_backtick) {
        constexpr auto end_string = ::pltxt2htm::details::concat(::pltxt2htm::details::U8LiteralString{u8"\n"}, fence);
        auto&& [advance_count, ast_] = ::pltxt2htm::details::simply_parse_pltext<ndebug, end_string>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
        ast = ::std::move(ast_);
        current_index += advance_count;
    }
    else {
        constexpr auto end_string = ::pltxt2htm::details::concat(::pltxt2htm::details::U8LiteralString{u8"\n"}, fence);
        auto&& [advance_count, ast_] = ::pltxt2htm::details::simply_parse_pltext<ndebug, end_string>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
        ast = ::std::move(ast_);
        current_index += advance_count;
    }

    ::exception::optional<::fast_io::u8string> opt_lang{::exception::nullopt_t{}};
    if (!lang.empty()) {
        opt_lang = ::std::move(lang);
    }
    if constexpr (is_backtick) {
        return ::pltxt2htm::details::TryParseMdCodeFenceResult<ndebug>{
            .node = ::pltxt2htm::MdCodeFenceBacktick<ndebug>{::std::move(ast), ::std::move(opt_lang)},
            .advance_count = current_index};
    }
    else {
        return ::pltxt2htm::details::TryParseMdCodeFenceResult<ndebug>{
            .node = ::pltxt2htm::MdCodeFenceTilde<ndebug>{::std::move(ast), ::std::move(opt_lang)},
            .advance_count = current_index};
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
    -> ::exception::optional<::pltxt2htm::details::TryParseMdCodeFenceResult<ndebug>> {
    if (auto opt_code_fence_backtick = ::pltxt2htm::details::try_parse_md_code_fence_<ndebug, true>(pltext);
        opt_code_fence_backtick.has_value()) {
        return opt_code_fence_backtick;
    }
#if 1
    return ::pltxt2htm::details::try_parse_md_code_fence_<ndebug, false>(pltext);
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
            return result;
        }
    }
    return ::exception::nullopt_t{};
}

struct TryParseMdBlockQuotesResult {
    ::std::size_t advance_count; ///< Number of characters consumed.
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
        current_index = temp_index + 1;
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
            if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8'\n') {
                break;
            }
        }
        if (current_index == pltext_size) {
            break;
        }
        subpltext.push_back(u8'\n');
    }

    if (subpltext.empty()) {
        return ::exception::nullopt_t{};
    }
    if (subpltext.back_unchecked() == u8'\n') {
        subpltext.pop_back();
    }
    return ::pltxt2htm::details::TryParseMdBlockQuotesResult{.advance_count = current_index,
                                                             .subpltext = ::std::move(subpltext)};
}

template<::pltxt2htm::Contracts ndebug>
struct TryParseMdCodeSpanResult {
    ::std::size_t advance_count; ///< Number of characters consumed.
    ::pltxt2htm::Ast<ndebug> subast; ///< Parsed AST for the code span.
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
    -> ::exception::optional<::pltxt2htm::details::TryParseMdCodeSpanResult<ndebug>> {
    constexpr ::std::size_t embraced_size{embraced_string.size()};
    if (!::pltxt2htm::details::is_prefix_match<ndebug, embraced_string>(pltext)) {
        return ::exception::nullopt_t{};
    }

    auto&& [advance_count, ast] = ::pltxt2htm::details::simply_parse_pltext<ndebug, embraced_string>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, embraced_size));

    if constexpr (embraced_size == 1) {
        return ::pltxt2htm::details::TryParseMdCodeSpanResult<ndebug>{.advance_count = advance_count + embraced_size,
                                                                      .subast = ::std::move(ast)};
    }
    else if constexpr (embraced_size == 2) {
        return ::pltxt2htm::details::TryParseMdCodeSpanResult<ndebug>{.advance_count = advance_count + embraced_size,
                                                                      .subast = ::std::move(ast)};
    }
    else if constexpr (embraced_size == 3) {
        return ::pltxt2htm::details::TryParseMdCodeSpanResult<ndebug>{.advance_count = advance_count + embraced_size,
                                                                      .subast = ::std::move(ast)};
    }
    else {
        pltxt2htm_unreachable(u8"Unexpected embraced size for code span");
    }
}

template<::pltxt2htm::Contracts ndebug>
struct TryParseMdLatexResult {
    ::std::size_t advance_count; ///< Number of characters consumed (includes both delimiters).
    ::pltxt2htm::Ast<ndebug> subast; ///< Parsed AST inside the latex delimiters.
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
    -> ::exception::optional<::pltxt2htm::details::TryParseMdLatexResult<ndebug>> {
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

    ::pltxt2htm::Ast<ndebug> ast{};
    for (::std::size_t idx{}; idx < close_pos;) {
        if (::pltxt2htm::details::u8string_view_index<ndebug>(body, idx) == u8'\n') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{u8'\n'}));
            ++idx;
        }
        else {
            auto forward = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(body, idx), ast);
            idx += forward;
        }
    }

    return ::pltxt2htm::details::TryParseMdLatexResult<ndebug>{.advance_count = close_pos + 4,
                                                               .subast = ::std::move(ast)};
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
    -> ::exception::optional<::pltxt2htm::details::TryParseMdLatexResult<ndebug>> {
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
        if (chr == u8'$') {
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

    ::pltxt2htm::Ast<ndebug> ast{};
    for (::std::size_t idx{}; idx < close_pos;) {
        auto&& sub = ::pltxt2htm::details::u8string_view_subview<ndebug>(body, idx);
        auto forward = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(sub, ast);
        idx += forward;
    }

    return ::pltxt2htm::details::TryParseMdLatexResult<ndebug>{.advance_count = close_pos + 2,
                                                               .subast = ::std::move(ast)};
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
 * @brief Parse and validate the authority part (scheme + domain + port) of a URL.
 *
 * Supports optional `http://` or `https://` scheme, domain validation, and optional port.
 * Does NOT parse the path, query, or fragment — that is the caller's responsibility.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text that begins at a URL candidate.
 * @return The index after the port (or after the domain if no port), or nullopt when
 *         scheme/domain/port validation fails.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url(::fast_io::u8string_view pltext) noexcept -> ::exception::optional<::std::size_t> {
    auto current_index = ::std::size_t{[pltext] constexpr noexcept -> ::std::size_t {
        if (::pltxt2htm::details::is_prefix_match<ndebug, u8"http">(pltext)) {
            auto const after_http = ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 4);
            if (::pltxt2htm::details::is_prefix_match<ndebug, u8"://">(after_http)) {
                return ::std::size_t{7};
            }
            if (::pltxt2htm::details::is_prefix_match<ndebug, u8"s://">(after_http)) {
                return ::std::size_t{8};
            }
        }
        return 0;
    }()};
    auto const domain_start = current_index;

    while (true) {
        if (current_index >= pltext.size()) {
            if (::pltxt2htm::details::validate_url_domain<ndebug>(pltext, domain_start, current_index)) {
                return current_index;
            }
            return ::exception::nullopt_t{};
        }
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if ((u8'A' <= chr && chr <= u8'Z') || (u8'a' <= chr && chr <= u8'z') || (u8'0' <= chr && chr <= u8'9') ||
            chr == u8'.' || chr == u8'-') {
            ++current_index;
            continue;
        }
        if (chr == u8'/') {
            if (::pltxt2htm::details::validate_url_domain<ndebug>(pltext, domain_start, current_index) == false) {
                return ::exception::nullopt_t{};
            }
            return current_index;
        }
        if (chr == u8':') {
            if (::pltxt2htm::details::validate_url_domain<ndebug>(pltext, domain_start, current_index) == false) {
                return ::exception::nullopt_t{};
            }
            ::std::uint_least32_t port{};
            ::std::size_t port_index{};
            for (char8_t const c : ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1)) {
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
                    return ::exception::nullopt_t{};
                }
            }
            return current_index;
        }
        // non-domain, non-'/', non-':' character
        if (::pltxt2htm::details::validate_url_domain<ndebug>(pltext, domain_start, current_index)) {
            return current_index;
        }
        return ::exception::nullopt_t{};
    }
}

/**
 * @brief Build a URL AST from a raw URL string, decoding `&amp;` entities.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] parsed_url The raw URL string to convert into an AST.
 * @param[in] consumed_size The number of bytes consumed (may differ from parsed_url.size() due to trailing garbage).
 * @return A TryParseUrlResult containing the URL object and consumed size.
 */
template<::pltxt2htm::Contracts ndebug>
struct TryParseUrlResult {
    ::std::size_t consumed_size;
    ::pltxt2htm::Url<ndebug> url;
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto make_try_parse_url_result(::fast_io::u8string_view const parsed_url,
                                         ::std::size_t consumed_size) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseUrlResult<ndebug>> {
    ::pltxt2htm::Ast<ndebug> ast{};
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
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Ampersand{}));
            break;
        case u8'\'':
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::SingleQuote{}));
            break;
        case u8'\"':
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::DoubleQuote{}));
            break;
        case u8'<':
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
            break;
        case u8'>':
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::GreaterThan{}));
            break;
        default:
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{chr}));
            break;
        }
    }
    return ::pltxt2htm::details::TryParseUrlResult<ndebug>{.consumed_size = consumed_size,
                                                           .url = ::pltxt2htm::Url<ndebug>{::std::move(ast)}};
}

/**
 * @brief Parse the simple URL path: printable ASCII, stops at `<` `>` `"` or non-printable characters.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The full input text view.
 * @param[in] start_index The index at which to start parsing the path (typically after authority).
 * @return The index at which the path ends.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_path_simple(::fast_io::u8string_view pltext, ::std::size_t start_index) noexcept
    -> ::std::size_t {
    while (start_index < pltext.size()) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index);
        if (chr < u8'!' || chr > u8'~' || chr == u8'<' || chr == u8'>' || chr == u8'\"') {
            break;
        }
        ++start_index;
    }
    return start_index;
}

/**
 * @brief Parse an auto-detected bare URL (http/https) with context guards.
 *
 * Detects a URL starting with `http://` or `https://` at `current_index` and rejects it
 * when preceded by `](` (i.e. inside markdown link syntax) or `=` (i.e. inside
 * `<tag=url...>`).
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The full input text view.
 * @param[in] current_index Offset into `pltext` at which to probe for a URL.
 * @return Parsed URL payload on success; nullopt if no valid auto-link is found.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_auto_url(::fast_io::u8string_view pltext, ::std::size_t current_index) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseUrlResult<ndebug>> {
    auto url_vw = ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index);
    auto opt_auth_end = ::pltxt2htm::details::try_parse_url<ndebug>(url_vw);
    if (!opt_auth_end.has_value()) {
        return ::exception::nullopt_t{};
    }
    auto auth_end = opt_auth_end.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    if (::pltxt2htm::details::is_prefix_match<ndebug, u8"http">(url_vw)) {
        auto const after_http = ::pltxt2htm::details::u8string_view_subview<ndebug>(url_vw, 4);
        if (!::pltxt2htm::details::is_prefix_match<ndebug, u8"://">(after_http) &&
            !::pltxt2htm::details::is_prefix_match<ndebug, u8"s://">(after_http)) {
            return ::exception::nullopt_t{};
        }
    }
    else {
        return ::exception::nullopt_t{};
    }
    if (current_index >= 2 && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index - 1) == u8'(' &&
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index - 2) == u8']') {
        return ::exception::nullopt_t{};
    }
    if (current_index >= 1 && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index - 1) == u8'=') {
        return ::exception::nullopt_t{};
    }
    auto path_end = ::pltxt2htm::details::try_parse_url_path_simple<ndebug>(url_vw, auth_end);
    return ::pltxt2htm::details::make_try_parse_url_result<ndebug>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(url_vw, 0, path_end), path_end);
}

/**
 * @brief Parse `<external=...>` tag and validate its URL payload.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text starting at the `external` tag payload.
 * @param[in] call_stack Active parser frames used to reject invalid nested contexts.
 * @return Parsed tag length and extracted URL on success; nullopt if invalid or disallowed nesting.
 */
template<::pltxt2htm::Contracts ndebug>
struct TryParseExternalTagResult {
    ::std::size_t tag_len;
    ::pltxt2htm::Url<ndebug> url;
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_external_tag(
    ::fast_io::u8string_view pltext,
    ::fast_io::stack<::pltxt2htm::details::ParserFrameContext<ndebug>> const& call_stack) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseExternalTagResult<ndebug>> {
    auto result = ::pltxt2htm::details::try_parse_non_nestable_equal_sign_tag<ndebug, u8"xternal">(
        pltext, [](char8_t u8chr) static constexpr noexcept { return u8'!' <= u8chr && u8chr <= u8'~'; }, call_stack);
    if (result.has_value() == false) {
        return ::exception::nullopt_t{};
    }

    auto&& [_, url_str] = result.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    auto url_vw = ::fast_io::u8string_view{url_str.data(), url_str.size()};
    auto opt_auth_end = ::pltxt2htm::details::try_parse_url<ndebug>(url_vw);
    if (!opt_auth_end.has_value()) {
        return ::exception::nullopt_t{};
    }
    auto auth_end = opt_auth_end.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    auto path_end = ::pltxt2htm::details::try_parse_url_path_simple<ndebug>(url_vw, auth_end);
    if (path_end != url_vw.size()) {
        return ::exception::nullopt_t{};
    }
    auto opt_url = ::pltxt2htm::details::make_try_parse_url_result<ndebug>(url_vw, url_vw.size());
    if (!opt_url.has_value()) {
        return ::exception::nullopt_t{};
    }

    return ::pltxt2htm::details::TryParseExternalTagResult<ndebug>{
        result.template value<ndebug == ::pltxt2htm::Contracts::ignore>().tag_len,
        ::std::move(opt_url.template value<ndebug == ::pltxt2htm::Contracts::ignore>().url)};
}

template<::pltxt2htm::Contracts ndebug>
struct TryParseMdUrlResult {
    ::std::size_t consumed_size;
    ::pltxt2htm::Url<ndebug> url;
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_url(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdUrlResult<ndebug>> {
    // First attempt: authority + path with `)` as terminator, then verify `)` follows
    // Save path_end so the fallback doesn't re-scan content already parsed.
    ::std::size_t path_end{};
    auto opt_auth_end = ::pltxt2htm::details::try_parse_url<ndebug>(pltext);
    if (opt_auth_end.has_value()) {
        auto auth_end = opt_auth_end.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        path_end = auth_end;
        while (path_end < pltext.size()) {
            auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, path_end);
            if (chr == u8')') {
                break;
            }
            if (chr < u8'!' || chr > u8'~' || chr == u8'<' || chr == u8'>' || chr == u8'\"') {
                break;
            }
            ++path_end;
        }
        if (path_end < pltext.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, path_end) == u8')') {
            auto url_vw = ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 0, path_end);
            auto opt_result = ::pltxt2htm::details::make_try_parse_url_result<ndebug>(url_vw, path_end);
            if (opt_result.has_value()) {
                auto&& result = opt_result.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                return ::pltxt2htm::details::TryParseMdUrlResult<ndebug>{.consumed_size = path_end,
                                                                         .url = ::std::move(result.url)};
            }
        }
    }

    // Fallback: continue from path_end (skips content already parsed above)
    auto raw_len = path_end;
    while (raw_len < pltext.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, raw_len) != u8')') {
        ++raw_len;
    }
    auto const pltext_size = pltext.size();
    pltxt2htm_assert(raw_len <= pltext_size, u8"raw_len should not exceed pltext size");
    if (raw_len == pltext_size) {
        return ::exception::nullopt_t{};
    }
    auto raw_url = ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 0, raw_len);
    ::fast_io::u8string encoded{};
    for (::std::size_t i = 0; i < raw_url.size(); ++i) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(raw_url, i);
        if (chr >= u8'!' && chr <= u8'~' && chr != u8'<' && chr != u8'>' && chr != u8'\"') {
            encoded.push_back(chr);
        }
        else {
            encoded.push_back(u8'%');
            auto const hi = static_cast<unsigned>(chr) >> 4;
            auto const lo = static_cast<unsigned>(chr) & 0x0F;
            encoded.push_back(hi < 10 ? u8'0' + hi : u8'A' + (hi - 10));
            encoded.push_back(lo < 10 ? u8'0' + lo : u8'A' + (lo - 10));
        }
    }
    auto encoded_vw = ::fast_io::u8string_view{encoded.data(), encoded.size()};
    auto opt_retry_auth = ::pltxt2htm::details::try_parse_url<ndebug>(encoded_vw);
    if (!opt_retry_auth.has_value()) {
        return ::exception::nullopt_t{};
    }
    auto retry_auth_end = opt_retry_auth.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    auto retry_path_end = ::pltxt2htm::details::try_parse_url_path_simple<ndebug>(encoded_vw, retry_auth_end);
    if (retry_path_end != encoded_vw.size()) {
        return ::exception::nullopt_t{};
    }
    auto opt_result = ::pltxt2htm::details::make_try_parse_url_result<ndebug>(encoded_vw, encoded_vw.size());
    if (!opt_result.has_value()) {
        return ::exception::nullopt_t{};
    }
    return ::pltxt2htm::details::TryParseMdUrlResult<ndebug>{
        .consumed_size = raw_len,
        .url = ::std::move(opt_result.template value<ndebug == ::pltxt2htm::Contracts::ignore>().url)};
}

template<::pltxt2htm::Contracts ndebug>
struct TryParseMdLinkResult {
    ::std::size_t advance_count;
    ::fast_io::u8string_view link_text;
    ::pltxt2htm::Url<ndebug> link_url;
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
    -> ::exception::optional<::pltxt2htm::details::TryParseMdLinkResult<ndebug>> {
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
    ::std::size_t const link_text_end{current_index};
    ++current_index;

    // Ensure the next character is '('
    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8'(') {
        return ::exception::nullopt_t{};
    }
    ++current_index;
    auto opt_md_url = ::pltxt2htm::details::try_parse_md_url<ndebug>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
    if (opt_md_url.has_value() == false) {
        return ::exception::nullopt_t{};
    }
    auto&& md_url_result = opt_md_url.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    current_index += md_url_result.consumed_size;
    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8')') {
        return ::exception::nullopt_t{};
    }
    ++current_index;
    return ::pltxt2htm::details::TryParseMdLinkResult<ndebug>{.advance_count = current_index,
                                                              .link_text = pltext.subview(1, link_text_end - 1),
                                                              .link_url = ::std::move(md_url_result.url)};
}

template<::pltxt2htm::Contracts ndebug>
struct TryParseMdImageResult {
    ::std::size_t advance_count;
    ::pltxt2htm::Ast<ndebug> link_text;
    ::pltxt2htm::Url<ndebug> link_url;
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
    -> ::exception::optional<::pltxt2htm::details::TryParseMdImageResult<ndebug>> {
    if (pltext.size() < 5 || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 0) != u8'!' ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1) != u8'[') {
        return ::exception::nullopt_t{};
    }

    ::std::size_t current_index{2};

    // Parse link text
    ::pltxt2htm::Ast<ndebug> link_text_ast{};
    while (current_index < pltext.size()) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};

        if (chr == u8']') {
            break;
        }

        if (chr == u8'\n') {
            return ::exception::nullopt_t{};
        }
        if (chr == u8' ') {
            link_text_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Space{}));
            ++current_index;
            continue;
        }
        if (chr == u8'&') {
            link_text_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Ampersand{}));
            ++current_index;
            continue;
        }
        if (chr == u8'\'') {
            link_text_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::SingleQuote{}));
            ++current_index;
            continue;
        }
        if (chr == u8'\"') {
            link_text_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::DoubleQuote{}));
            ++current_index;
            continue;
        }
        if (chr == u8'>') {
            link_text_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::GreaterThan{}));
            ++current_index;
            continue;
        }
        if (chr == u8'\t') {
            link_text_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::Tab{}));
            ++current_index;
            continue;
        }
        if (auto opt_escape = ::pltxt2htm::details::try_parse_md_escape<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            opt_escape.has_value()) {
            auto&& [node, advance_count] = opt_escape.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            link_text_ast.push_back(::std::move(node));
            current_index += advance_count;
            continue;
        }
        if (chr == u8'<') {
            link_text_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
            ++current_index;
            continue;
        }
        auto advance_count = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index), link_text_ast);
        current_index += advance_count;
        continue;
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
    auto opt_md_url = ::pltxt2htm::details::try_parse_md_url<ndebug>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
    if (opt_md_url.has_value() == false) {
        return ::exception::nullopt_t{};
    }
    auto&& md_url_result = opt_md_url.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    current_index += md_url_result.consumed_size;
    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8')') {
        return ::exception::nullopt_t{};
    }
    return ::pltxt2htm::details::TryParseMdImageResult<ndebug>{.advance_count = current_index + 1,
                                                               .link_text = ::std::move(link_text_ast),
                                                               .link_url = ::std::move(md_url_result.url)};
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
