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
#include "frame_context.hh"
#include "../utils.hh"
#include "../../contracts.hh"
#include "../../ast/ast.hh"
#include "../../ast/font_size_value.hh"
#include "../../ast/vertical_align_value.hh"
#include "../push_macro.hh"

/**
 * @brief Namespace for internal implementation details.
 */
namespace pltxt2htm::details {

/**
 * @brief Return type of try_parse_md_escape: the parsed node and consumed bytes.
 * @tparam ndebug Contract checking mode.
 */
template<::pltxt2htm::Contracts ndebug>
struct TryParseMdEscapeResult {
    ::pltxt2htm::PlTxtNode<ndebug> node; ///< The parsed escape node.
    ::std::size_t advance_count; ///< Bytes consumed from input.
};

/**
 * @brief Parse a markdown backslash-escape sequence.
 *
 * Maps the character following a backslash to the corresponding markdown escape AST node.
 *
 * @param[in] pltext Input starting with a backslash escape sequence.
 * @return An optional TryParseMdEscapeResult containing the parsed node and bytes consumed,
 *         or nullopt if the input does not start with a valid escape.
 * @note Supported escape characters include: \\ \! \" \# \$ \% \& \' \( \) \* \+ \, \- \. \/ \: \; \< \= \> \? \@ \[ \]
 * \^ \_ \` \{ \| \} \~
 * @see https://spec.commonmark.org/0.31.2/#backslash-escapes
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_escape(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdEscapeResult<ndebug>> {
    if (pltext.empty()) {
        return ::exception::nullopt;
    }
    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 0) != u8'\\') {
        return ::exception::nullopt;
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
        return ::exception::nullopt;
    }

    for (::std::size_t i{tag_name_size}; i < pltext.size(); ++i) {
        auto forward_chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i);
        if (forward_chr == u8'>') {
            return i;
        }
        if (forward_chr != u8' ' && forward_chr != u8'\t') {
            return ::exception::nullopt;
        }
    }
    return ::exception::nullopt;
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
    if (opt_tag_len.has_value() == false) {
        return ::exception::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_ul && nested_tag_type != ::pltxt2htm::NodeKind::html_ol) {
        return ::exception::nullopt;
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
    if (opt_tag_len.has_value() == false) {
        return ::exception::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_table) {
        return ::exception::nullopt;
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
    if (opt_tag_len.has_value() == false) {
        return ::exception::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_table) {
        return ::exception::nullopt;
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
    if (opt_tag_len.has_value() == false) {
        return ::exception::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_table) {
        return ::exception::nullopt;
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
    if (opt_tag_len.has_value() == false) {
        return ::exception::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_table) {
        return ::exception::nullopt;
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
    if (opt_tag_len.has_value() == false) {
        return ::exception::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_table) {
        return ::exception::nullopt;
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
    if (opt_tag_len.has_value() == false) {
        return ::exception::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_table && nested_tag_type != ::pltxt2htm::NodeKind::html_thead &&
        nested_tag_type != ::pltxt2htm::NodeKind::html_tbody && nested_tag_type != ::pltxt2htm::NodeKind::html_tfoot) {
        return ::exception::nullopt;
    }
    return opt_tag_len;
}

/**
 * @brief Parse a CSS text-align value to a TableAlign.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled.
 * @param[in] value The text-align value string (e.g. "center", "CENTER").
 * @return The corresponding alignment on success, or nullopt on failure.
 */
[[nodiscard]]
constexpr auto parse_text_align_value(::fast_io::u8string_view value) noexcept
    -> ::exception::optional<::pltxt2htm::TableAlign> {
    // only exact lowercase values accepted
    if (value == ::fast_io::u8string_view{u8"left"}) {
        return ::pltxt2htm::TableAlign::left;
    }
    if (value == ::fast_io::u8string_view{u8"center"}) {
        return ::pltxt2htm::TableAlign::center;
    }
    if (value == ::fast_io::u8string_view{u8"right"}) {
        return ::pltxt2htm::TableAlign::right;
    }
    return ::exception::nullopt;
}

/**
 * @brief Parse a td text-align style directly from the original tag input.
 * @param[in] pltext Input containing the quoted style value.
 * @param[in,out] pos Current position after the opening quote; returns at the closing quote.
 * @param[in] quote Attribute quote character.
 * @param[out] align Last valid text-align value in the style.
 * @return true when the complete style value is valid; otherwise false.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr bool try_parse_td_style(::fast_io::u8string_view pltext, ::std::size_t& pos, char8_t const quote,
                                  ::pltxt2htm::TableAlign& align) noexcept {
    while (pos < pltext.size()) {
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return false;
        }

        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos);
        if (chr == quote) {
            return true;
        }
        if (chr == u8';') {
            ++pos;
            continue;
        }

        auto const segment_start{pos};
        auto property_end{segment_start};
        ::std::size_t colon{};
        bool has_colon{};
        ::std::size_t value_start{};
        ::std::size_t value_end{};
        bool value_started{};
        while (pos < pltext.size()) {
            auto const segment_chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos);
            if (segment_chr == u8';' || segment_chr == quote) {
                break;
            }
            bool const is_whitespace{segment_chr == u8' ' || segment_chr == u8'\t'};
            if (has_colon == false) {
                if (segment_chr == u8':') {
                    colon = pos;
                    has_colon = true;
                    value_start = pos + 1;
                    value_end = value_start;
                }
                else if (is_whitespace == false) {
                    property_end = pos + 1;
                }
            }
            else if (is_whitespace == false) {
                if (value_started == false) {
                    value_start = pos;
                    value_started = true;
                }
                value_end = pos + 1;
            }
            ++pos;
        }

        // Preserve the existing behavior of ignoring declarations without a usable separator.
        if (has_colon && colon != segment_start) {
            auto const property = ::fast_io::u8string_view{pltext.data() + segment_start, property_end - segment_start};
            if (property != ::fast_io::u8string_view{u8"text-align"}) {
                return false;
            }
            auto const value = ::fast_io::u8string_view{pltext.data() + value_start, value_end - value_start};
            auto const opt_align = ::pltxt2htm::details::parse_text_align_value(value);
            if (opt_align.has_value() == false) {
                return false;
            }
            align = opt_align.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        }

        if (pos >= pltext.size()) {
            return false;
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8';') {
            ++pos;
            continue;
        }
        return ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == quote;
    }
    return false;
}

/**
 * @brief Return type of try_parse_th_tag and try_parse_td_tag: tag length and cell alignment.
 */
struct TryParseTdTagResult {
    ::std::size_t tag_len; ///< Length of the matched tag up to the closing `>`.
    ::pltxt2htm::TableAlign align; ///< Cell alignment parsed from `style="text-align:..."`.
};

/**
 * @brief Parse &lt;th&gt; (optionally with `style="text-align:..."`) and validate parent container type.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting after `<t`.
 * @param[in] nested_tag_type Current parent tag type from parsing context.
 * @return Parsed tag length and alignment when valid under &lt;tr&gt;; otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_th_tag(::fast_io::u8string_view pltext, ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::exception::optional<TryParseTdTagResult> {
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"h"}>(pltext) == false) {
        return ::exception::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_tr) {
        return ::exception::nullopt;
    }

    ::std::size_t pos{1}; // skip past "h"
    ::pltxt2htm::TableAlign align{::pltxt2htm::TableAlign::left};

    while (pos < pltext.size()) {
        // skip whitespace
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'>') {
            return TryParseTdTagResult{.tag_len = pos, .align = align};
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
            return ::exception::nullopt;
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }

        // parse quoted attribute value
        char8_t const quote{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos)};
        if (quote != u8'\"' && quote != u8'\'') {
            return ::exception::nullopt;
        }
        ++pos; // skip opening quote
        ::std::size_t const val_start{pos};
        while (pos < pltext.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != quote) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }
        ::fast_io::u8string_view const attr_val{pltext.data() + val_start, pos - val_start};
        ++pos; // skip closing quote

        // only lowercase "style" attribute is checked for text-align
        if (attr_name != u8"style") {
            return ::exception::nullopt;
        }
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

            // parse CSS property name (text-align)
            ::std::size_t const css_prop_start{css_pos};
            while (css_pos < attr_val.size() &&
                   ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, css_pos) != u8':') {
                ++css_pos;
            }
            if (css_pos >= attr_val.size()) {
                break;
            }
            ::fast_io::u8string_view const css_prop{attr_val.data() + css_prop_start, css_pos - css_prop_start};
            ++css_pos; // skip ':'

            // skip whitespace before value
            while (css_pos < attr_val.size() &&
                   (::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, css_pos) == u8' ' ||
                    ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, css_pos) == u8'\t')) {
                ++css_pos;
            }

            // parse CSS value
            ::std::size_t const css_val_start{css_pos};
            while (css_pos < attr_val.size() &&
                   ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, css_pos) != u8';') {
                ++css_pos;
            }
            ::fast_io::u8string_view const css_val{attr_val.data() + css_val_start, css_pos - css_val_start};
            if (css_pos < attr_val.size()) {
                ++css_pos; // skip ';'
            }

            // check text-align property
            if (css_prop != u8"text-align") {
                return ::exception::nullopt;
            }
            auto opt_align = ::pltxt2htm::details::parse_text_align_value(css_val);
            if (opt_align.has_value() == false) {
                return ::exception::nullopt;
            }
            align = opt_align.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        }
    }

    return ::exception::nullopt;
}

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
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"d"}>(pltext) == false) {
        return ::exception::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_tr) {
        return ::exception::nullopt;
    }

    ::std::size_t pos{1}; // skip past "d"
    ::pltxt2htm::TableAlign align{::pltxt2htm::TableAlign::left};

    while (pos < pltext.size()) {
        // skip whitespace
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'>') {
            return TryParseTdTagResult{.tag_len = pos, .align = align};
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
            return ::exception::nullopt;
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }

        // parse quoted attribute value
        char8_t const quote{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos)};
        if (quote != u8'"' && quote != u8'\'') {
            return ::exception::nullopt;
        }
        if (attr_name != u8"style") {
            return ::exception::nullopt;
        }
        ++pos;
        if (::pltxt2htm::details::try_parse_td_style<ndebug>(pltext, pos, quote, align) == false) {
            return ::exception::nullopt;
        }
        ++pos;
    }
    return ::exception::nullopt;
}

/**
 * @brief Return type of try_parse_equal_sign_tag: tag length and extracted value.
 */
struct TryParseEqualSignTagResult {
    ::std::size_t tag_len; ///< Length of the tag.
    ::fast_io::u8string_view substr; ///< View of the value extracted from the tag.
};

/**
 * @brief Find the end of a value whose characters are accepted by a named predicate.
 */
template<::pltxt2htm::Contracts ndebug, auto value_char_predicate>
    requires requires(char8_t chr) {
        { value_char_predicate(chr) } -> ::std::same_as<bool>;
    }
[[nodiscard]]
constexpr auto find_value_end(::fast_io::u8string_view pltext, ::std::size_t pos) noexcept -> ::std::size_t {
    while (pos < pltext.size()) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos);
        if (chr == u8'>' || chr == u8' ' || chr == u8'\t' || value_char_predicate(chr) == false) {
            break;
        }
        ++pos;
    }
    return pos;
}

/**
 * @brief Check whether the input starts with an equals-sign tag prefix.
 */
template<::pltxt2htm::Contracts ndebug, ::pltxt2htm::details::U8LiteralString prefix_str>
[[nodiscard]]
constexpr bool is_equal_sign_tag_prefix(::fast_io::u8string_view pltext) noexcept {
    constexpr auto prefix_with_equal =
        ::pltxt2htm::details::concat(prefix_str, ::pltxt2htm::details::U8LiteralString{u8"="});
    return ::pltxt2htm::details::is_prefix_match<ndebug, prefix_with_equal>(pltext);
}

/**
 * @brief Parse trailing whitespace and the closing bracket of an equals-sign tag.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_equal_sign_tag_suffix(::fast_io::u8string_view pltext, ::std::size_t const value_start,
                                               ::std::size_t pos) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseEqualSignTagResult> {
    if (pos == value_start) {
        return ::exception::nullopt;
    }
    auto const value_size = pos - value_start;
    while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                   ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
        ++pos;
    }
    if (pos >= pltext.size() || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'>') {
        return ::exception::nullopt;
    }
    return ::pltxt2htm::details::TryParseEqualSignTagResult{
        .tag_len = pos, .substr = ::fast_io::u8string_view{pltext.data() + value_start, value_size}};
}

/**
 * @brief Parse an equals-sign tag whose value uses a stateless character grammar.
 */
template<::pltxt2htm::Contracts ndebug, ::pltxt2htm::details::U8LiteralString prefix_str, auto value_char_predicate>
[[nodiscard]]
constexpr auto try_parse_equal_sign_tag(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseEqualSignTagResult> {
    if (::pltxt2htm::details::is_equal_sign_tag_prefix<ndebug, prefix_str>(pltext) == false) {
        return ::exception::nullopt;
    }
    constexpr auto value_start = prefix_str.size() + 1;
    auto const value_end = ::pltxt2htm::details::find_value_end<ndebug, value_char_predicate>(pltext, value_start);
    return ::pltxt2htm::details::try_parse_equal_sign_tag_suffix<ndebug>(pltext, value_start, value_end);
}

/**
 * @brief Parse `<tag=value>` and reject it when nested inside non-nestable PL tags.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @tparam prefix_str Tag-name prefix used by `try_parse_equal_sign_tag`.
 * @param[in] pltext The input text to parse at current position.
 * @param[in] call_stack Active parser stack used to detect forbidden nesting.
 * @return Parsed tag result on success, otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug, ::pltxt2htm::details::U8LiteralString prefix_str, auto value_char_predicate>
[[nodiscard]]
constexpr auto try_parse_non_nestable_equal_sign_tag(
    ::fast_io::u8string_view pltext,
    ::fast_io::stack<::pltxt2htm::details::ParserFrameContext<ndebug>> const& call_stack) noexcept
    -> ::exception::optional<TryParseEqualSignTagResult> {
    auto result = ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, prefix_str, value_char_predicate>(pltext);
    if (result.has_value() == false) {
        return ::exception::nullopt;
    }
    for (auto const& v : call_stack.container) {
        // skip
        // e.g. <experiment><experiment>test</experiment>text</experiment>
        // e.g. <experiment><a><experiment>test</experiment>text</a>text</experiment>
        auto const nested_tag_type = v.get_nested_tag_type();
        if (nested_tag_type == ::pltxt2htm::NodeKind::pl_experiment ||
            nested_tag_type == ::pltxt2htm::NodeKind::pl_discussion ||
            nested_tag_type == ::pltxt2htm::NodeKind::pl_external ||
            nested_tag_type == ::pltxt2htm::NodeKind::pl_link) {
            return ::exception::nullopt;
        }
    }
    return result;
}

/**
 * @brief Parse a color value and return the first character after it.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_color_value(::fast_io::u8string_view pltext, ::std::size_t const start) noexcept
    -> ::exception::optional<::std::size_t> {
    if (start >= pltext.size()) {
        return ::exception::nullopt;
    }
    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start) != u8'#') {
        auto const end =
            ::pltxt2htm::details::find_value_end<ndebug, ::pltxt2htm::details::is_ascii_alpha>(pltext, start);
        if (end == start) {
            return ::exception::nullopt;
        }
        return end;
    }

    auto const hex_start = start + 1;
    auto const end =
        ::pltxt2htm::details::find_value_end<ndebug, ::pltxt2htm::details::is_ascii_hexdigit>(pltext, hex_start);
    auto const hex_size = end - hex_start;
    if (hex_size != 3 && hex_size != 4 && hex_size != 6 && hex_size != 8) {
        return ::exception::nullopt;
    }
    return end;
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
    constexpr auto prefix_str = ::pltxt2htm::details::U8LiteralString{u8"olor"};
    if (::pltxt2htm::details::is_equal_sign_tag_prefix<ndebug, prefix_str>(pltext) == false) {
        return ::exception::nullopt;
    }
    constexpr auto value_start = prefix_str.size() + 1;
    auto opt_value_end = ::pltxt2htm::details::try_parse_color_value<ndebug>(pltext, value_start);
    if (opt_value_end.has_value() == false) {
        return ::exception::nullopt;
    }
    return ::pltxt2htm::details::try_parse_equal_sign_tag_suffix<ndebug>(
        pltext, value_start, opt_value_end.template value<ndebug == ::pltxt2htm::Contracts::ignore>());
}

/**
 * @brief Return type of try_parse_size_tag: tag length and the parsed font-size value.
 */
struct TryParseSizeTagResult {
    ::std::size_t tag_len;
    ::pltxt2htm::FontSizeValue value;
};

/**
 * @brief Parse a `<size=N>` or `<size=N%>` opening tag.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_size_tag(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseSizeTagResult> {
    constexpr auto prefix_str = ::pltxt2htm::details::U8LiteralString{u8"ize"};
    if (::pltxt2htm::details::is_equal_sign_tag_prefix<ndebug, prefix_str>(pltext) == false) {
        return ::exception::nullopt;
    }
    constexpr auto value_start = prefix_str.size() + 1;
    auto opt_value = ::pltxt2htm::details::try_parse_size_t_decimal_value<ndebug>(pltext, value_start);
    if (opt_value.has_value() == false) {
        return ::exception::nullopt;
    }
    auto const value = opt_value.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    auto value_end = value.end;
    auto unit = ::pltxt2htm::SizeUnit::px;
    if (value_end < pltext.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, value_end) == u8'%') {
        unit = ::pltxt2htm::SizeUnit::percent;
        ++value_end;
    }
    auto opt_tag = ::pltxt2htm::details::try_parse_equal_sign_tag_suffix<ndebug>(pltext, value_start, value_end);
    if (opt_tag.has_value() == false) {
        return ::exception::nullopt;
    }
    return ::pltxt2htm::details::TryParseSizeTagResult{
        opt_tag.template value<ndebug == ::pltxt2htm::Contracts::ignore>().tag_len, {value.value, unit}};
}

/**
 * @brief Return type of try_parse_span_tag: tag length, color, and optional font-size/vertical-align.
 */
template<::pltxt2htm::Contracts ndebug>
struct TryParseSpanTagResult {
    ::std::size_t tag_len; ///< Length of the matched tag.
    ::fast_io::u8string color; ///< Extracted color value.
    ::exception::optional<::pltxt2htm::FontSizeValue> font_size; ///< Extracted font-size value+unit (if present).
    ::exception::optional<::pltxt2htm::VerticalAlignValue<ndebug>>
        vertical_align; ///< Extracted vertical-align value (if present).
};

/**
 * @brief Result of parsing a CSS font-size value.
 */
struct TryParseFontSizeValueResult {
    ::std::size_t end;
    ::pltxt2htm::FontSizeValue font_size;
};

/**
 * @brief Parse a non-zero integer optionally followed by lowercase `px` or `%`.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_font_size_value(::fast_io::u8string_view pltext, ::std::size_t const start) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseFontSizeValueResult> {
    auto opt_decimal = ::pltxt2htm::details::try_parse_size_t_decimal_value<ndebug>(pltext, start);
    if (opt_decimal.has_value() == false) {
        return ::exception::nullopt;
    }
    auto const decimal = opt_decimal.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    if (decimal.value == 0) {
        return ::exception::nullopt;
    }
    auto pos = decimal.end;
    auto unit = ::pltxt2htm::SizeUnit::px;
    if (pos < pltext.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'p') {
        ++pos;
        if (pos >= pltext.size() || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'x') {
            return ::exception::nullopt;
        }
        ++pos;
    }
    else if (pos < pltext.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'%') {
        unit = ::pltxt2htm::SizeUnit::percent;
        ++pos;
    }
    return ::pltxt2htm::details::TryParseFontSizeValueResult{pos, {decimal.value, unit}};
}

/**
 * @brief Result of parsing a CSS vertical-align value.
 */
template<::pltxt2htm::Contracts ndebug>
struct TryParseVerticalAlignValueResult {
    ::std::size_t end;
    ::pltxt2htm::VerticalAlignValue<ndebug> value;
};

/**
 * @brief Try to match a lowercase CSS vertical-align keyword at `pos`.
 * @details Rejects the keyword when the following character is an ASCII letter or
 *          digit, so that e.g. "superx" or "sub2" do not silently parse as keywords.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr bool try_parse_vertical_align_keyword(::fast_io::u8string_view pltext, ::std::size_t& pos,
                                                ::fast_io::u8string_view const keyword) noexcept {
    for (::std::size_t offset{0}; offset < keyword.size(); ++offset) {
        if (pos + offset >= pltext.size() || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos + offset) !=
                                                 ::pltxt2htm::details::u8string_view_index<ndebug>(keyword, offset)) {
            return false;
        }
    }
    if (pos + keyword.size() < pltext.size()) {
        auto const next = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos + keyword.size());
        if (::pltxt2htm::details::is_ascii_alpha(next) || ::pltxt2htm::details::is_ascii_digit(next)) {
            return false;
        }
    }
    pos += keyword.size();
    return true;
}

/**
 * @brief Parse a CSS vertical-align value: a lowercase keyword or a non-zero px/% length.
 * @param[in] pltext Input text starting at the value.
 * @param[in] start Position of the first value character (whitespace already skipped).
 * @return Parsed value with its end position on success; nullopt on failure.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_vertical_align_value(::fast_io::u8string_view pltext, ::std::size_t const start) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseVerticalAlignValueResult<ndebug>> {
    struct VerticalAlignKeywordEntry {
        ::pltxt2htm::VerticalAlignKeyword keyword;
        ::fast_io::u8string_view spelling;
    };

    static constexpr VerticalAlignKeywordEntry keywords[]{
        {::pltxt2htm::VerticalAlignKeyword::baseline, ::fast_io::u8string_view{u8"baseline"}},
        {::pltxt2htm::VerticalAlignKeyword::text_bottom, ::fast_io::u8string_view{u8"text-bottom"}},
        {::pltxt2htm::VerticalAlignKeyword::text_top, ::fast_io::u8string_view{u8"text-top"}},
        {::pltxt2htm::VerticalAlignKeyword::super, ::fast_io::u8string_view{u8"super"}},
        {::pltxt2htm::VerticalAlignKeyword::sub, ::fast_io::u8string_view{u8"sub"}},
        {::pltxt2htm::VerticalAlignKeyword::middle, ::fast_io::u8string_view{u8"middle"}},
        {::pltxt2htm::VerticalAlignKeyword::bottom, ::fast_io::u8string_view{u8"bottom"}},
        {::pltxt2htm::VerticalAlignKeyword::top, ::fast_io::u8string_view{u8"top"}},
    };
    auto pos = start;
    for (auto const& entry : keywords) {
        if (::pltxt2htm::details::try_parse_vertical_align_keyword<ndebug>(pltext, pos, entry.spelling)) {
            return TryParseVerticalAlignValueResult<ndebug>{
                .end = pos, .value = ::pltxt2htm::VerticalAlignValue<ndebug>{entry.keyword}};
        }
    }
    auto opt_length = ::pltxt2htm::details::try_parse_font_size_value<ndebug>(pltext, start);
    if (opt_length.has_value() == false) {
        return ::exception::nullopt;
    }
    auto const length = opt_length.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    return TryParseVerticalAlignValueResult<ndebug>{.end = length.end,
                                                    .value = ::pltxt2htm::VerticalAlignValue<ndebug>{length.font_size}};
}

/**
 * @brief Skip trailing whitespace and require a CSS property delimiter.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr bool try_parse_span_style_property_suffix(::fast_io::u8string_view pltext, ::std::size_t& pos,
                                                    char8_t const quote) noexcept {
    while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                   ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
        ++pos;
    }
    if (pos >= pltext.size()) {
        return false;
    }
    auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos);
    return chr == u8';' || chr == quote;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr bool try_parse_span_style(
    ::fast_io::u8string_view pltext, ::std::size_t& pos, char8_t const quote, ::fast_io::u8string& color,
    ::exception::optional<::pltxt2htm::FontSizeValue>& font_size,
    ::exception::optional<::pltxt2htm::VerticalAlignValue<ndebug>>& vertical_align) noexcept {
    while (pos < pltext.size()) {
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return false;
        }

        auto const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos)};
        if (chr == quote) {
            return true;
        }
        if (chr == u8';') {
            ++pos;
            continue;
        }

        auto const property_start{pos};
        auto property_end{pos};
        while (pos < pltext.size()) {
            auto const property_chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos)};
            if (property_chr == u8':') {
                break;
            }
            if (property_chr == u8';' || property_chr == quote) {
                return false;
            }
            if (property_chr != u8' ' && property_chr != u8'\t') {
                property_end = pos + 1;
            }
            ++pos;
        }
        if (pos >= pltext.size() || property_end == property_start) {
            return false;
        }
        auto const property{::fast_io::u8string_view{pltext.data() + property_start, property_end - property_start}};
        ++pos;

        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }

        if (property == ::fast_io::u8string_view{u8"color"}) {
            if (color.empty() == false) {
                return false;
            }
            auto const value_start = pos;
            auto opt_value_end = ::pltxt2htm::details::try_parse_color_value<ndebug>(pltext, value_start);
            if (opt_value_end.has_value() == false) {
                return false;
            }
            auto const value_end = opt_value_end.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            pos = value_end;
            if (::pltxt2htm::details::try_parse_span_style_property_suffix<ndebug>(pltext, pos, quote) == false) {
                return false;
            }
            color = ::fast_io::u8string{::fast_io::u8string_view{pltext.data() + value_start, value_end - value_start}};
        }
        else if (property == ::fast_io::u8string_view{u8"font-size"}) {
            if (font_size.has_value()) {
                return false;
            }
            auto opt_value = ::pltxt2htm::details::try_parse_font_size_value<ndebug>(pltext, pos);
            if (opt_value.has_value() == false) {
                return false;
            }
            auto const value = opt_value.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            pos = value.end;
            if (::pltxt2htm::details::try_parse_span_style_property_suffix<ndebug>(pltext, pos, quote) == false) {
                return false;
            }
            font_size = value.font_size;
        }
        else if (property == ::fast_io::u8string_view{u8"vertical-align"}) {
            if (vertical_align.has_value()) {
                return false;
            }
            auto opt_value = ::pltxt2htm::details::try_parse_vertical_align_value<ndebug>(pltext, pos);
            if (opt_value.has_value() == false) {
                return false;
            }
            auto const value = opt_value.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            pos = value.end;
            if (::pltxt2htm::details::try_parse_span_style_property_suffix<ndebug>(pltext, pos, quote) == false) {
                return false;
            }
            vertical_align = value.value;
        }
        else {
            return false;
        }

        if (pos >= pltext.size()) {
            return false;
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8';') {
            ++pos;
            continue;
        }
        return ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == quote;
    }
    return false;
}

/**
 * @brief Parse &lt;span style="color:V;font-size:S"&gt; and reject any other attributes or CSS properties.
 * @tparam ndebug When set to Contracts::ignore, runtime assertions are disabled for performance.
 * @param[in] pltext Input text starting at "pan ..." (after "<s").
 * @return Parsed tag result with color and/or font_size/vertical-align on success; nullopt on failure.
 * @note Only the lowercase "style" attribute is accepted. Within style, only lowercase "color",
 *       "font-size" and "vertical-align" CSS properties are accepted. Any other attribute or
 *       CSS property causes parse failure.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_span_tag(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<TryParseSpanTagResult<ndebug>> {
    // match "pan" prefix (case-insensitive)
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"pan"}>(pltext) ==
        false) {
        return ::exception::nullopt;
    }

    ::std::size_t pos{4}; // skip past "span" (the 's' was consumed by the trie dispatch)
    bool found_style{false};
    ::fast_io::u8string color{};
    ::exception::optional<::pltxt2htm::FontSizeValue> font_size{::exception::nullopt};
    ::exception::optional<::pltxt2htm::VerticalAlignValue<ndebug>> vertical_align{::exception::nullopt};

    while (pos < pltext.size()) {
        // skip whitespace
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'>') {
            break;
        }

        // parse attribute name
        ::std::size_t const attr_start = pos;
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
            return ::exception::nullopt;
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }

        // parse quoted attribute value
        char8_t const quote{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos)};
        if (quote != u8'"' && quote != u8'\'') {
            return ::exception::nullopt;
        }
        // only lowercase "style" attribute is allowed
        if (attr_name != ::fast_io::u8string_view{u8"style"}) {
            return ::exception::nullopt;
        }
        found_style = true;
        ++pos;
        if (::pltxt2htm::details::try_parse_span_style<ndebug>(pltext, pos, quote, color, font_size, vertical_align) ==
            false) {
            return ::exception::nullopt;
        }
        ++pos;
    }

    if (found_style == false || (color.empty() && !font_size.has_value() && !vertical_align.has_value())) {
        return ::exception::nullopt;
    }
    if (pos >= pltext.size() || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'>') {
        return ::exception::nullopt;
    }
    return TryParseSpanTagResult{.tag_len = pos + 1,
                                 .color = ::std::move(color),
                                 .font_size = ::std::move(font_size),
                                 .vertical_align = ::std::move(vertical_align)};
}

/**
 * @brief Result of parsing a <code class="language-..."> tag.
 */
struct TryParseCodeTagResult {
    ::std::size_t tag_len; ///< Length of the matched tag.
    ::exception::optional<::fast_io::u8string> language;
};

[[nodiscard]]
constexpr bool is_code_language_suffix_char(char8_t const chr) noexcept {
    return ::pltxt2htm::details::is_ascii_alpha(chr) || ::pltxt2htm::details::is_ascii_digit(chr) || chr == u8'+' ||
           chr == u8'#' || chr == u8'.' || chr == u8'_' || chr == u8'-';
}

/**
 * @brief Parse <code> or <code class="language-..."> with strict validation.
 * @tparam ndebug When set to Contracts::ignore, runtime assertions are disabled for performance.
 * @param[in] pltext Input text starting at "ode ..." (after "<c").
 * @return Parsed tag result on success; nullopt on any deviation.
 * @note Bare <code> (no attributes) is accepted. If "class" attribute is present,
 *       the value must start with "language-" and have at least one character after.
 *       Any other attribute, duplicate class, empty class, or non-"language-" prefix fails.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_code_tag(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<TryParseCodeTagResult> {
    // match "ode" prefix (case-insensitive)
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"ode"}>(pltext) ==
        false) {
        return ::exception::nullopt;
    }

    ::std::size_t pos{3}; // skip past "ode" (the 'c' was consumed by the trie dispatch)
    bool found_class{false};
    ::exception::optional<::fast_io::u8string> language{::exception::nullopt};

    while (pos < pltext.size()) {
        // skip whitespace
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'>') {
            break;
        }

        // parse attribute name
        ::std::size_t const attr_start = pos;
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
            return ::exception::nullopt;
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }

        // parse quoted attribute value
        char8_t const quote{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos)};
        if (quote != u8'"' && quote != u8'\'') {
            return ::exception::nullopt;
        }
        ++pos; // skip opening quote
        ::std::size_t const val_start = pos;
        bool language_suffix_is_safe{true};
        while (pos < pltext.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != quote) {
            if (attr_name == ::fast_io::u8string_view{u8"class"} && pos - val_start >= 9 &&
                ::pltxt2htm::details::is_code_language_suffix_char(
                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos)) == false) {
                language_suffix_is_safe = false;
            }
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }
        ::fast_io::u8string_view const attr_val{pltext.data() + val_start, pos - val_start};
        ++pos; // skip closing quote

        // only lowercase "class" attribute is allowed
        if (attr_name == ::fast_io::u8string_view{u8"class"}) {
            if (found_class) {
                return ::exception::nullopt; // duplicate class
            }
            if (attr_val.empty()) {
                return ::exception::nullopt;
            }
            // value must start with "language-" and have at least one char after
            if (attr_val.size() < 10 || ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, 0) != u8'l' ||
                ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, 1) != u8'a' ||
                ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, 2) != u8'n' ||
                ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, 3) != u8'g' ||
                ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, 4) != u8'u' ||
                ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, 5) != u8'a' ||
                ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, 6) != u8'g' ||
                ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, 7) != u8'e' ||
                ::pltxt2htm::details::u8string_view_index<ndebug>(attr_val, 8) != u8'-' ||
                language_suffix_is_safe == false) {
                return ::exception::nullopt;
            }
            language = ::fast_io::u8string{attr_val};
            found_class = true;
        }
        else {
            return ::exception::nullopt; // unknown attribute
        }
    }

    // bare tag with no attributes is accepted
    if (pos >= pltext.size() || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'>') {
        return ::exception::nullopt;
    }
    return TryParseCodeTagResult{.tag_len = pos + 1, .language = ::std::move(language)};
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
            return ::exception::nullopt;
        }
    }
    return ::exception::nullopt;
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
        return ::exception::nullopt;
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
            return ::exception::nullopt;
        }
    }
    return ::exception::nullopt;
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
    if (opt_tag_len.has_value() == false) {
        return ::exception::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::html_colgroup) {
        return ::exception::nullopt;
    }
    return opt_tag_len;
}

/**
 * @brief Parse &lt;input type=&quot;checkbox&quot; disabled [checked]&gt; tag.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting after `<i`.
 * @return Tag length and checked state if matched; otherwise nullopt.
 */
struct TryParseInputCheckboxTagResult {
    ::std::size_t tag_len;
    bool checked;
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_input_checkbox_tag(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<TryParseInputCheckboxTagResult> {
    // match "nput" (case-insensitive)
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"nput"}>(pltext) ==
        false) {
        return ::exception::nullopt;
    }
    ::std::size_t pos{4};
    bool found_type_checkbox{false};
    bool found_disabled{false};
    bool checked{false};

    while (pos < pltext.size()) {
        // skip whitespace
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }
        // end of tag
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'>') {
            break;
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'/' && pos + 1 < pltext.size() &&
            ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos + 1) == u8'>') {
            break;
        }

        // parse attribute name
        ::std::size_t const attr_start = pos;
        while (pos < pltext.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'=' &&
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'>' &&
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8' ' &&
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'\t' &&
               ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'/') {
            ++pos;
        }
        ::fast_io::u8string_view const attr_name{pltext.data() + attr_start, pos - attr_start};

        // boolean attribute without value: "disabled" or "checked"
        if (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t' ||
                                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'>' ||
                                    ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'/')) {
            if (attr_name == ::fast_io::u8string_view{u8"disabled"}) {
                found_disabled = true;
                continue;
            }
            if (attr_name == ::fast_io::u8string_view{u8"checked"}) {
                checked = true;
                continue;
            }
            return ::exception::nullopt;
        }

        // attribute with '=' value: "type=..."
        if (pos >= pltext.size() || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'=') {
            return ::exception::nullopt;
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }

        // parse quoted attribute value
        char8_t const quote{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos)};
        if (quote != u8'"' && quote != u8'\'') {
            return ::exception::nullopt;
        }
        ++pos; // skip opening quote
        ::std::size_t const val_start = pos;
        while (pos < pltext.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != quote) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }
        ::fast_io::u8string_view const attr_val{pltext.data() + val_start, pos - val_start};
        ++pos; // skip closing quote

        // only "type=checkbox" is allowed
        if (attr_name != ::fast_io::u8string_view{u8"type"}) {
            return ::exception::nullopt;
        }
        if (attr_val != ::fast_io::u8string_view{u8"checkbox"}) {
            return ::exception::nullopt;
        }
        found_type_checkbox = true;
    }

    if (found_type_checkbox == false || !found_disabled) {
        return ::exception::nullopt;
    }
    if (pos >= pltext.size()) {
        return ::exception::nullopt;
    }
    // skip '>' or '/>'
    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'/' && pos + 1 < pltext.size()) {
        ++pos;
    }
    if (pos >= pltext.size() || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'>') {
        return ::exception::nullopt;
    }
    return TryParseInputCheckboxTagResult{.tag_len = pos + 1, .checked = checked};
}

/**
 * @brief Result of parsing an `<img>` tag.
 */
struct TryParseImgTagResult {
    ::std::size_t tag_len;
    ::fast_io::u8string src;
    ::fast_io::u8string alt;
};

/**
 * @brief Parse `<img src="..." alt="...">` self-closing tag.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled.
 * @param[in] pltext Input text starting after `<i` (i.e. at "mg...").
 * @return Tag length, src and alt values if valid; otherwise nullopt.
 * @note `src` and `alt` are both required. Attribute order is flexible.
 *       Accepts both `>` and `/>` as closing.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_img_tag(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<TryParseImgTagResult> {
    // match "mg" (case-insensitive)
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"mg"}>(pltext) == false) {
        return ::exception::nullopt;
    }

    ::std::size_t pos{2};
    bool found_src{false};
    bool found_alt{false};
    ::fast_io::u8string src{};
    ::fast_io::u8string alt{};

    while (pos < pltext.size()) {
        // skip whitespace
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }
        // end of tag
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'>') {
            break;
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'/' && pos + 1 < pltext.size() &&
            ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos + 1) == u8'>') {
            break;
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
            return ::exception::nullopt;
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }

        // parse quoted attribute value
        char8_t const quote{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos)};
        if (quote != u8'"' && quote != u8'\'') {
            return ::exception::nullopt;
        }
        ++pos; // skip opening quote
        ::std::size_t const val_start{pos};
        while (pos < pltext.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != quote) {
            ++pos;
        }
        if (pos >= pltext.size()) {
            return ::exception::nullopt;
        }
        ::fast_io::u8string_view const attr_val{pltext.data() + val_start, pos - val_start};
        ++pos; // skip closing quote

        // recognize attribute
        if (attr_name == ::fast_io::u8string_view{u8"src"}) {
            if (found_src) {
                return ::exception::nullopt; // duplicate src
            }
            src = ::fast_io::u8string{attr_val};
            found_src = true;
        }
        else if (attr_name == ::fast_io::u8string_view{u8"alt"}) {
            if (found_alt) {
                return ::exception::nullopt; // duplicate alt
            }
            alt = ::fast_io::u8string{attr_val};
            found_alt = true;
        }
        else {
            return ::exception::nullopt; // unknown attribute
        }
    }

    if (found_src == false || !found_alt) {
        return ::exception::nullopt;
    }
    if (pos >= pltext.size()) {
        return ::exception::nullopt;
    }
    // skip '>' or '/>'
    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'/' && pos + 1 < pltext.size()) {
        ++pos;
    }
    if (pos >= pltext.size() || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'>') {
        return ::exception::nullopt;
    }
    return TryParseImgTagResult{.tag_len = pos + 1, .src = ::std::move(src), .alt = ::std::move(alt)};
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
        return ::exception::nullopt;
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
            return ::exception::nullopt;
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
                    .start_index = start_index,
                    .sublength = 0,
                    .advance_count = start_index,
                    .md_atx_heading_type = static_cast<::pltxt2htm::NodeKind>(md_atx_heading_type)};
            }
            return ::exception::nullopt;
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
        return ::exception::nullopt;
    }
    ++start_index;
    // spaces after the last #
    while (true) {
        if (start_index >= pltext_size) {
            // https://spec.commonmark.org/0.31.2/#example-79
            return ::pltxt2htm::details::TryParseMdAtxHeadingResult{
                .start_index = start_index,
                .sublength = 0,
                .advance_count = start_index,
                .md_atx_heading_type = static_cast<::pltxt2htm::NodeKind>(md_atx_heading_type)};
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

enum class ThematicBreakType : unsigned {
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
        return ::exception::nullopt;
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
                return ::exception::nullopt;
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
                return ::exception::nullopt;
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
                return ::exception::nullopt;
            }
            thematic_break_type = ::pltxt2htm::details::ThematicBreakType::underscore;
            ++thematic_break_count;
            continue;
        }
        if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::none) {
            return ::exception::nullopt;
        }
        auto opt_line_break = ::pltxt2htm::details::try_parse_pltext_line_break<ndebug>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(text, i));
        if (opt_line_break.has_value() == false) {
            return ::exception::nullopt;
        }
        if (thematic_break_count < 3) {
            return ::exception::nullopt;
        }
        return i + opt_line_break.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    }
    if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::none) {
        return ::exception::nullopt;
    }
    if (thematic_break_count < 3) {
        return ::exception::nullopt;
    }
    return i;
}

template<::pltxt2htm::Contracts ndebug>
struct SimplyParsePLtextResult {
    ::std::size_t advance_count; ///< Number of characters consumed.
    ::pltxt2htm::Ast<ndebug> ast; ///< Parsed AST.
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_entity_reference(::fast_io::u8string_view text) noexcept
    -> ::exception::optional<::std::size_t> {
    if (text.empty() || ::pltxt2htm::details::u8string_view_index<ndebug>(text, 0) != u8'&') {
        return ::exception::nullopt;
    }
    auto const max = text.size();
    auto index = ::std::size_t{1};
    if (index >= max) {
        return ::exception::nullopt;
    }
    if (::pltxt2htm::details::u8string_view_index<ndebug>(text, index) == u8'#') {
        ++index;
        if (index >= max) {
            return ::exception::nullopt;
        }
        bool hex{};
        auto const prefix = ::pltxt2htm::details::u8string_view_index<ndebug>(text, index);
        if (prefix == u8'x' || prefix == u8'X') {
            hex = true;
            ++index;
        }
        if (index >= max) {
            return ::exception::nullopt;
        }
        auto const begin = index;
        for (; index < max; ++index) {
            auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(text, index);
            if (chr == u8';') {
                break;
            }
            if (hex ? !::pltxt2htm::details::is_ascii_hexdigit(chr) : !::pltxt2htm::details::is_ascii_digit(chr)) {
                return ::exception::nullopt;
            }
        }
        if (index == begin || index >= max || ::pltxt2htm::details::u8string_view_index<ndebug>(text, index) != u8';') {
            return ::exception::nullopt;
        }
        return index + 1;
    }

    if (::pltxt2htm::details::is_ascii_alpha(::pltxt2htm::details::u8string_view_index<ndebug>(text, index)) == false) {
        return ::exception::nullopt;
    }
    ++index;
    for (; index < max; ++index) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(text, index);
        if (chr == u8';') {
            return index + 1;
        }
        if (::pltxt2htm::details::is_ascii_alpha(chr) == false && !::pltxt2htm::details::is_ascii_digit(chr)) {
            return ::exception::nullopt;
        }
    }
    return ::exception::nullopt;
}

/**
 * @brief Parse plain text content into an AST until a termination string is encountered.
 *
 * This function processes plain text content character by character, converting special
 * characters and escape sequences into appropriate AST nodes. It stops parsing when it
 * encounters the specified termination string.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @tparam end_string The exact string that marks the end of parsing. When empty, the whole
 *                    input view is parsed.
 * @param[in] pltext The input text to parse.
 * @return A structure containing the parsed AST and the index to continue parsing from.
 * @note Special characters such as newline, space, ampersand, quotes,
 *       greater-than, and tab are converted to specific AST nodes.
 * @note Backslash escape sequences are processed and converted to their escaped equivalents.
 * @note UTF-8 multi-byte characters are properly handled and converted to U8Char nodes.
 * @note When end_string is non-empty, the function consumes it and stops parsing immediately after.
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

        if constexpr (end_size != 0) {
            if (::pltxt2htm::details::is_prefix_match<ndebug, end_string>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index))) {
                current_index += end_size;
                break;
            }
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
            if (auto const opt_entity_len = ::pltxt2htm::details::try_parse_entity_reference<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
                opt_entity_len.has_value()) {
                auto const entity_len = opt_entity_len.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::EntityReference{::fast_io::u8string{
                    pltext.data() + current_index + 1, pltext.data() + current_index + entity_len - 1}}));
                current_index += entity_len;
                continue;
            }
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

[[nodiscard]]
constexpr bool is_allowed_in_language(char8_t const chr) noexcept {
    return (chr >= u8'a' && chr <= u8'z') || (chr >= u8'A' && chr <= u8'Z') || (chr >= u8'0' && chr <= u8'9') ||
           chr == u8'+' || chr == u8'#' || chr == u8'.' || chr == u8'_' || chr == u8'-';
}

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
        return ::exception::nullopt;
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
        return ::exception::nullopt;
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
                ::exception::optional<::fast_io::u8string> opt_lang{::exception::nullopt};
                if (lang.empty() == false) {
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
                return ::exception::nullopt;
            }
            while (current_index != pltext_size &&
                   ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8'\n') {
                ++current_index;
            }
            if (current_index == pltext_size) {
                // content after space/tab without newline: not a valid code fence
                return ::exception::nullopt;
            }
            ++current_index;
            break;
        }
        if constexpr (is_backtick) {
            if (::pltxt2htm::details::is_prefix_match<ndebug, fence>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index))) {
                return ::exception::nullopt;
            }
        }
        if (::pltxt2htm::details::is_allowed_in_language(chr) == false) {
            lang.clear();
            while (current_index != pltext_size &&
                   ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8'\n') {
                ++current_index;
            }
            if (current_index == pltext_size) {
                return ::exception::nullopt;
            }
            ++current_index;
            break;
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

    ::exception::optional<::fast_io::u8string> opt_lang{::exception::nullopt};
    if (lang.empty() == false) {
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
        return ::exception::nullopt;
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
    if (::pltxt2htm::details::is_prefix_match<ndebug, embraced_chars>(pltext) == false) {
        return ::exception::nullopt;
    }

    for (::std::size_t current_index{embraced_size}; current_index < pltext.size(); ++current_index) {
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8'\n') {
            return ::exception::nullopt;
        }
        if (::pltxt2htm::details::is_prefix_match<ndebug, embraced_chars>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index))) {
            ::std::size_t result{current_index - embraced_size};
            if (result == 0) {
                return ::exception::nullopt;
            }
            return result;
        }
    }
    return ::exception::nullopt;
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
                return ::exception::nullopt;
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
        return ::exception::nullopt;
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
    if (::pltxt2htm::details::is_prefix_match<ndebug, embraced_string>(pltext) == false) {
        return ::exception::nullopt;
    }

    auto&& [advance_count, ast] = ::pltxt2htm::details::simply_parse_pltext<ndebug, embraced_string>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, embraced_size));

    return ::pltxt2htm::details::TryParseMdCodeSpanResult<ndebug>{.advance_count = advance_count + embraced_size,
                                                                  .subast = ::std::move(ast)};
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
        return ::exception::nullopt;
    }

    auto body = ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 2);
    ::std::size_t const body_size{body.size()};
    ::pltxt2htm::Ast<ndebug> ast{};
    for (::std::size_t current_index{}; current_index < body_size;) {
        if (::pltxt2htm::details::is_prefix_match<ndebug, double_dollar>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(body, current_index))) {
            return ::pltxt2htm::details::TryParseMdLatexResult<ndebug>{.advance_count = current_index + 4,
                                                                       .subast = ::std::move(ast)};
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(body, current_index) == u8'\n') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{u8'\n'}));
            ++current_index;
        }
        else {
            current_index += ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(body, current_index), ast);
        }
    }
    return ::exception::nullopt;
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
        return ::exception::nullopt;
    }

    auto body = ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 1);
    ::std::size_t const body_size{body.size()};
    ::pltxt2htm::Ast<ndebug> ast{};
    for (::std::size_t current_index{}; current_index < body_size;) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(body, current_index);
        if (chr == u8'\n') {
            return ::exception::nullopt;
        }
        if (chr == u8'$') {
            if (current_index == 0) {
                return ::exception::nullopt;
            }
            return ::pltxt2htm::details::TryParseMdLatexResult<ndebug>{.advance_count = current_index + 2,
                                                                       .subast = ::std::move(ast)};
        }
        current_index += ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(body, current_index), ast);
    }
    return ::exception::nullopt;
}

/**
 * @brief Check whether a parsed domain ends in an accepted top-level domain.
 */
[[nodiscard]]
constexpr bool has_allowed_url_tld(::fast_io::u8string_view domain) noexcept {
    return domain.ends_with(u8".com") || domain.ends_with(u8".net") || domain.ends_with(u8".org") ||
           domain.ends_with(u8".cn") || domain.ends_with(u8".edu") || domain.ends_with(u8".gov") ||
           domain.ends_with(u8".io") || domain.ends_with(u8".ai") || domain.ends_with(u8".co") ||
           domain.ends_with(u8".me") || domain.ends_with(u8".cc") || domain.ends_with(u8".tv") ||
           domain.ends_with(u8".info") || domain.ends_with(u8".biz") || domain.ends_with(u8".us") ||
           domain.ends_with(u8".uk") || domain.ends_with(u8".jp") || domain.ends_with(u8".hk") ||
           domain.ends_with(u8".tw") || domain.ends_with(u8".xyz") || domain.ends_with(u8".top");
}

/**
 * @brief Parse and validate a URL domain in one pass.
 * @return The first index after the domain, or nullopt when a label or TLD is invalid.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_domain(::fast_io::u8string_view pltext, ::std::size_t domain_start) noexcept
    -> ::exception::optional<::std::size_t> {
    ::std::size_t current_index{domain_start};
    bool label_has_char{};
    bool label_ended_with_hyphen{};
    while (current_index < pltext.size()) {
        auto const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};
        if (::pltxt2htm::details::is_ascii_alpha(chr) || ::pltxt2htm::details::is_ascii_digit(chr)) {
            label_has_char = true;
            label_ended_with_hyphen = false;
        }
        else if (chr == u8'-') {
            if (label_has_char == false) {
                return ::exception::nullopt;
            }
            label_ended_with_hyphen = true;
        }
        else if (chr == u8'.') {
            if (label_has_char == false || label_ended_with_hyphen) {
                return ::exception::nullopt;
            }
            label_has_char = false;
            label_ended_with_hyphen = false;
        }
        else {
            break;
        }
        ++current_index;
    }

    if (label_has_char == false || label_ended_with_hyphen) {
        return ::exception::nullopt;
    }
    auto const domain =
        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, domain_start, current_index - domain_start);
    if (::pltxt2htm::details::has_allowed_url_tld(domain) == false) {
        return ::exception::nullopt;
    }
    return current_index;
}

/**
 * @brief Parse and validate a URL port and its following delimiter.
 * @return The first index after the port, or nullopt when the port is invalid.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_port(::fast_io::u8string_view pltext, ::std::size_t port_start) noexcept
    -> ::exception::optional<::std::size_t> {
    ::std::uint_least32_t port{};
    ::std::size_t current_index{port_start};
    ::std::size_t port_size{};
    while (current_index < pltext.size()) {
        auto const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};
        if (::pltxt2htm::details::is_ascii_digit(chr) == false) {
            break;
        }
        port = port * 10 + static_cast<::std::uint_least32_t>(chr - u8'0');
        ++current_index;
        ++port_size;
        if (port_size > 5) {
            return ::exception::nullopt;
        }
    }
    if (port_size == 0 || port > 65535) {
        return ::exception::nullopt;
    }
    if (current_index < pltext.size()) {
        auto const next_chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};
        if (next_chr != u8'/' && next_chr != u8'?' && next_chr != u8'#') {
            return ::exception::nullopt;
        }
    }
    return current_index;
}

/**
 * @brief Detect and return the end offset of `http://` or `https://` scheme.
 *
 * O(1) — does NOT scan for domains. Returns the scheme length (7 or 8) or nullopt.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text that may begin with a URL scheme.
 * @return 7 for `http://`, 8 for `https://`, or nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_scheme(::fast_io::u8string_view pltext) noexcept -> ::exception::optional<::std::size_t> {
    if (::pltxt2htm::details::is_prefix_match<ndebug, u8"http">(pltext) == false) {
        return ::exception::nullopt;
    }
    auto const after_http = ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 4);
    if (::pltxt2htm::details::is_prefix_match<ndebug, u8"://">(after_http)) {
        return ::std::size_t{7};
    }
    if (::pltxt2htm::details::is_prefix_match<ndebug, u8"s://">(after_http)) {
        return ::std::size_t{8};
    }
    return ::exception::nullopt;
}

/**
 * @brief Parse and validate the authority part (domain + port) of a URL.
 *
 * Does NOT detect the scheme — the caller must supply `domain_start` (use 0 when
 * no scheme is present). Supports domain validation and optional port.
 * Does NOT parse the path, query, or fragment — that is the caller's responsibility.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text that begins at a URL candidate.
 * @param[in] domain_start The offset within pltext where the domain starts (0 when no scheme).
 * @return The index after the port (or after the domain if no port), or nullopt when
 *         domain/port validation fails.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_authority(::fast_io::u8string_view pltext, ::std::size_t domain_start) noexcept
    -> ::exception::optional<::std::size_t> {
    auto const opt_domain_end{::pltxt2htm::details::try_parse_url_domain<ndebug>(pltext, domain_start)};
    if (opt_domain_end.has_value() == false) {
        return ::exception::nullopt;
    }
    auto const domain_end{opt_domain_end.template value<ndebug == ::pltxt2htm::Contracts::ignore>()};
    if (domain_end >= pltext.size() || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, domain_end) != u8':') {
        return domain_end;
    }
    return ::pltxt2htm::details::try_parse_url_port<ndebug>(pltext, domain_end + 1);
}

/**
 * @brief Build a URL AST from a raw URL string, decoding `&amp;` entities.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] parsed_url The raw URL string to convert into an AST.
 * @param[in] consumed_size The number of bytes consumed (may differ from parsed_url.size() due to trailing garbage).
 * @return A TryParseUrlResult containing the URL object and consumed size.
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
    ::fast_io::u8string url_str{};
    url_str.reserve(parsed_url.size());
    for (::std::size_t index{}; index < parsed_url.size(); ++index) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(parsed_url, index);
        if (chr == u8'&') {
            if (index + 5 <= parsed_url.size() &&
                ::pltxt2htm::details::u8string_view_subview<ndebug>(parsed_url, index, 5) == u8"&amp;") {
                chr = u8'&';
                index += 4;
            }
        }
        if (chr > u8'~') {
            // non-ASCII byte (e.g. UTF-8 CJK): percent-encode it so tag URLs keep the raw characters
            url_str.push_back(u8'%');
            auto const hi = static_cast<unsigned>(chr) >> 4;
            auto const lo = static_cast<unsigned>(chr) & 0x0F;
            url_str.push_back(hi < 10 ? u8'0' + hi : u8'A' + (hi - 10));
            url_str.push_back(lo < 10 ? u8'0' + lo : u8'A' + (lo - 10));
            continue;
        }
        switch (chr) {
        case u8'\'': {
            url_str.append(u8"%27");
            break;
        }
        case u8'\"': {
            url_str.append(u8"%22");
            break;
        }
        case u8'<': {
            url_str.append(u8"%3C");
            break;
        }
        case u8'>': {
            url_str.append(u8"%3E");
            break;
        }
        default:
            url_str.push_back(chr);
            break;
        }
    }
    return ::pltxt2htm::details::TryParseUrlResult{.consumed_size = consumed_size,
                                                   .url = ::pltxt2htm::Url{::std::move(url_str)}};
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
    if (start_index < pltext.size()) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index);
        if (chr != u8'/' && chr != u8'?' && chr != u8'#') {
            return start_index;
        }
    }
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
 * @brief Parse a URL path that may contain non-ASCII bytes (percent-encoded later).
 * @details Like try_parse_url_path_simple but also accepts bytes >= 0x7F so tag URLs
 *          (html_a / pl_external / pl_link) can carry UTF-8 characters (e.g. CJK);
 *          make_try_parse_url_result percent-encodes them. Auto-detected URLs stay ASCII-only.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The full input text view.
 * @param[in] start_index The index at which to start parsing the path (typically after authority).
 * @return The index at which the path ends.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_path_unicode(::fast_io::u8string_view pltext, ::std::size_t start_index) noexcept
    -> ::std::size_t {
    if (start_index < pltext.size()) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index);
        if (chr != u8'/' && chr != u8'?' && chr != u8'#') {
            return start_index;
        }
    }
    while (start_index < pltext.size()) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index);
        if (chr < u8'!' || chr == u8'<' || chr == u8'>' || chr == u8'\"') {
            break;
        }
        ++start_index;
    }
    return start_index;
}

/**
 * @brief Result of parsing a URL-bearing opening tag.
 * @details The three return states are encoded by the payload members `tag_len` and `url`:
 *          - `valid` — `url` is engaged (`tag_len` is the opening-tag length the caller
 *            skips to reach the tag content);
 *          - `invalid_url` — `url` is disengaged and `tag_len` != 0: the opening tag was
 *            recognized but its URL failed validation, so the caller consumes the first
 *            `tag_len` characters as one literal span;
 *          - `not_a_tag` — `tag_len` == 0: keep the char-by-char fallback.
 *          A recognized opening tag always has `tag_len` != 0, which keeps the three
 *          states distinguishable.
 */
struct TryParseHtmlATagResult {
    ::std::size_t tag_len; ///< Opening-tag length in the input view (valid for valid/invalid_url).
    ::exception::optional<::pltxt2htm::Url> url; ///< Extracted URL; engaged only when valid.
    bool internal;

    constexpr TryParseHtmlATagResult() noexcept
        : tag_len{},
          url{::exception::nullopt},
          internal{} {
    }

    constexpr TryParseHtmlATagResult(::std::size_t tag_len_) noexcept
        : tag_len{tag_len_},
          url{::exception::nullopt},
          internal{} {
    }

    constexpr TryParseHtmlATagResult(::std::size_t tag_len_, ::pltxt2htm::Url&& url_, bool internal_) noexcept
        : tag_len{tag_len_},
          url(::std::move(url_)),
          internal{internal_} {
    }

    /// State is derived from the payload: engaged url means valid; otherwise a non-zero
    /// tag_len means the URL failed validation; a zero tag_len means not a tag.
    [[nodiscard]]
    constexpr auto is_valid(this auto const& self) noexcept -> bool {
        return self.url.has_value();
    }

    [[nodiscard]]
    constexpr auto is_invalid_url(this auto const& self) noexcept -> bool {
        return self.url.has_value() == false && self.tag_len != 0;
    }
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_html_a_tag(::fast_io::u8string_view pltext) noexcept -> TryParseHtmlATagResult {
    ::std::size_t pos{};
    while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                   ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
        ++pos;
    }
    if (pos >= pltext.size()) {
        return {};
    }
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"href"}>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, pos)) == false) {
        return {};
    }
    pos += 4;
    while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                   ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
        ++pos;
    }
    if (pos >= pltext.size() || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'=') {
        return {};
    }
    ++pos;
    while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                   ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
        ++pos;
    }
    if (pos >= pltext.size()) {
        return {};
    }
    char8_t const quote{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos)};
    if (quote != u8'"' && quote != u8'\'') {
        return {};
    }
    ++pos;
    ::std::size_t const val_start{pos};
    while (pos < pltext.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != quote) {
        ++pos;
    }
    if (pos >= pltext.size()) {
        return {};
    }
    ::fast_io::u8string_view const attr_val{pltext.data() + val_start, pos - val_start};
    ++pos;
    while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                   ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
        ++pos;
    }
    bool internal{};
    if (pos < pltext.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'>') {
        // only the boolean attribute "internal" is accepted as an extra attribute
        constexpr auto internal_literal = ::pltxt2htm::details::U8LiteralString{u8"internal"};
        if (::pltxt2htm::details::is_prefix_match<ndebug, internal_literal>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, pos)) == false) {
            return {};
        }
        internal = true;
        pos += internal_literal.size();
        while (pos < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8' ' ||
                                       ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) == u8'\t')) {
            ++pos;
        }
    }
    if (pos >= pltext.size() || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, pos) != u8'>') {
        return {};
    }
    auto opt_auth_end = ::pltxt2htm::details::try_parse_url_authority<ndebug>(
        attr_val, ::pltxt2htm::details::try_parse_url_scheme<ndebug>(attr_val).value_or(::std::size_t{}));
    if (opt_auth_end.has_value() == false) {
        return TryParseHtmlATagResult{pos + 1};
    }
    auto auth_end = opt_auth_end.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    auto path_end = ::pltxt2htm::details::try_parse_url_path_unicode<ndebug>(attr_val, auth_end);
    if (path_end != attr_val.size()) {
        return TryParseHtmlATagResult{pos + 1};
    }
    auto opt_url_result = ::pltxt2htm::details::make_try_parse_url_result<ndebug>(attr_val, path_end);
    if (opt_url_result.has_value() == false) {
        return TryParseHtmlATagResult{pos + 1};
    }
    return TryParseHtmlATagResult{
        pos + 1, ::std::move(opt_url_result.template value<ndebug == ::pltxt2htm::Contracts::ignore>().url), internal};
}

/**
 * @brief Parse an auto-detected bare URL (http/https) with context guards.
 *
 * Detects a URL starting with `http://` or `https://` at `current_index`.
 *
 * Only `http://`/`https://` schemes are accepted.  Bare domains (e.g. `example.com`) are
 * intentionally not supported because the parser calls this function at every character
 * position — allowing domain scans without a scheme prefix would reintroduce O(n²)
 * behaviour on long lines without whitespace.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text view starting at the URL to parse.
 * @return Parsed URL payload on success; nullopt if no valid auto-link is found.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_auto_url(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseUrlResult> {
    auto opt_scheme_end = ::pltxt2htm::details::try_parse_url_scheme<ndebug>(pltext);
    if (opt_scheme_end.has_value() == false) {
        return ::exception::nullopt;
    }
    auto scheme_end = opt_scheme_end.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    auto opt_auth_end = ::pltxt2htm::details::try_parse_url_authority<ndebug>(pltext, scheme_end);
    if (opt_auth_end.has_value() == false) {
        return ::exception::nullopt;
    }
    auto auth_end = opt_auth_end.template value<ndebug == ::pltxt2htm::Contracts::ignore>();

    auto path_end = ::pltxt2htm::details::try_parse_url_path_simple<ndebug>(pltext, auth_end);
    return ::pltxt2htm::details::make_try_parse_url_result<ndebug>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 0, path_end), path_end);
}

/**
 * @brief Parse `<external=...>` tag and validate its URL payload.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text starting at the `external` tag payload.
 * @param[in] call_stack Active parser frames used to reject invalid nested contexts.
 * @return `valid` with tag length + URL on success; `invalid_url` with the tag length when the
 *         opening tag was recognized but its URL failed validation; `not_a_tag` otherwise.
 */
/**
 * @brief Result of parsing a URL-bearing opening tag.
 * @details The three return states are encoded by the payload members `tag_len` and `url`:
 *          - `valid` — `url` is engaged (`tag_len` is the opening-tag length the caller
 *            skips to reach the tag content);
 *          - `invalid_url` — `url` is disengaged and `tag_len` != 0: the opening tag was
 *            recognized but its URL failed validation, so the caller consumes the first
 *            `tag_len` characters as one literal span;
 *          - `not_a_tag` — `tag_len` == 0: keep the char-by-char fallback.
 *          A recognized opening tag always has `tag_len` != 0, which keeps the three
 *          states distinguishable.
 */
struct TryParseExternalTagResult {
    ::std::size_t tag_len; ///< Opening-tag length in the input view (valid for valid/invalid_url).
    ::exception::optional<::pltxt2htm::Url> url; ///< Extracted URL; engaged only when valid.

    constexpr TryParseExternalTagResult() noexcept
        : tag_len{},
          url{::exception::nullopt} {
    }

    constexpr TryParseExternalTagResult(::std::size_t tag_len_) noexcept
        : tag_len{tag_len_},
          url{::exception::nullopt} {
    }

    constexpr TryParseExternalTagResult(::std::size_t tag_len_, ::pltxt2htm::Url&& url_) noexcept
        : tag_len{tag_len_},
          url(::std::move(url_)) {
    }

    /// State is derived from the payload: engaged url means valid; otherwise a non-zero
    /// tag_len means the URL failed validation; a zero tag_len means not a tag.
    [[nodiscard]]
    constexpr auto is_valid(this auto const& self) noexcept -> bool {
        return self.url.has_value();
    }

    [[nodiscard]]
    constexpr auto is_invalid_url(this auto const& self) noexcept -> bool {
        return self.url.has_value() == false && self.tag_len != 0;
    }
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_external_tag(
    ::fast_io::u8string_view pltext,
    ::fast_io::stack<::pltxt2htm::details::ParserFrameContext<ndebug>> const& call_stack) noexcept
    -> TryParseExternalTagResult {
    auto result = ::pltxt2htm::details::try_parse_non_nestable_equal_sign_tag<ndebug, u8"xternal",
                                                                              ::pltxt2htm::details::is_url_value_char>(
        pltext, call_stack);
    if (result.has_value() == false) {
        return {};
    }
    auto const tag_len = result.template value<ndebug == ::pltxt2htm::Contracts::ignore>().tag_len;

    auto&& [_, url_str] = result.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    auto url_vw = ::fast_io::u8string_view{url_str.data(), url_str.size()};
    auto opt_auth_end = ::pltxt2htm::details::try_parse_url_authority<ndebug>(
        url_vw, ::pltxt2htm::details::try_parse_url_scheme<ndebug>(url_vw).value_or(::std::size_t{}));
    if (opt_auth_end.has_value() == false) {
        return TryParseExternalTagResult{tag_len};
    }
    auto auth_end = opt_auth_end.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    auto path_end = ::pltxt2htm::details::try_parse_url_path_unicode<ndebug>(url_vw, auth_end);
    if (path_end != url_vw.size()) {
        return TryParseExternalTagResult{tag_len};
    }
    auto opt_url = ::pltxt2htm::details::make_try_parse_url_result<ndebug>(url_vw, url_vw.size());
    if (opt_url.has_value() == false) {
        return TryParseExternalTagResult{tag_len};
    }

    return TryParseExternalTagResult{
        tag_len, ::std::move(opt_url.template value<ndebug == ::pltxt2htm::Contracts::ignore>().url)};
}

/**
 * @brief Parse `<link="...">` tag (Unity TextMeshPro rich text) and validate its URL payload.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text starting at the `link` tag payload.
 * @param[in] call_stack Active parser frames used to reject invalid nested contexts.
 * @return `valid` with tag length + URL on success; `invalid_url` with the tag length when the
 *         opening tag was recognized but its URL failed validation; `not_a_tag` otherwise.
 * @note The Unity TextMeshPro link tag uses a quoted value: &lt;link=&quot;url&quot;&gt;. A value
 *       without surrounding double quotes is rejected so that unquoted `<link=url>` stays plain text.
 */
/**
 * @brief Result of parsing a URL-bearing opening tag.
 * @details The three return states are encoded by the payload members `tag_len` and `url`:
 *          - `valid` — `url` is engaged (`tag_len` is the opening-tag length the caller
 *            skips to reach the tag content);
 *          - `invalid_url` — `url` is disengaged and `tag_len` != 0: the opening tag was
 *            recognized but its URL failed validation, so the caller consumes the first
 *            `tag_len` characters as one literal span;
 *          - `not_a_tag` — `tag_len` == 0: keep the char-by-char fallback.
 *          A recognized opening tag always has `tag_len` != 0, which keeps the three
 *          states distinguishable.
 */
struct TryParseLinkTagResult {
    ::std::size_t tag_len; ///< Opening-tag length in the input view (valid for valid/invalid_url).
    ::exception::optional<::pltxt2htm::Url> url; ///< Extracted URL; engaged only when valid.

    constexpr TryParseLinkTagResult() noexcept
        : tag_len{},
          url{::exception::nullopt} {
    }

    constexpr TryParseLinkTagResult(::std::size_t tag_len_) noexcept
        : tag_len{tag_len_},
          url{::exception::nullopt} {
    }

    constexpr TryParseLinkTagResult(::std::size_t tag_len_, ::pltxt2htm::Url&& url_) noexcept
        : tag_len{tag_len_},
          url(::std::move(url_)) {
    }

    /// State is derived from the payload: engaged url means valid; otherwise a non-zero
    /// tag_len means the URL failed validation; a zero tag_len means not a tag.
    [[nodiscard]]
    constexpr auto is_valid(this auto const& self) noexcept -> bool {
        return self.url.has_value();
    }

    [[nodiscard]]
    constexpr auto is_invalid_url(this auto const& self) noexcept -> bool {
        return self.url.has_value() == false && self.tag_len != 0;
    }
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_link_tag(
    ::fast_io::u8string_view pltext,
    ::fast_io::stack<::pltxt2htm::details::ParserFrameContext<ndebug>> const& call_stack) noexcept
    -> TryParseLinkTagResult {
    auto result = ::pltxt2htm::details::try_parse_non_nestable_equal_sign_tag<ndebug, u8"ink",
                                                                              ::pltxt2htm::details::is_url_value_char>(
        pltext, call_stack);
    if (result.has_value() == false) {
        return {};
    }
    auto const tag_len = result.template value<ndebug == ::pltxt2htm::Contracts::ignore>().tag_len;

    auto&& [_, raw_value] = result.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    if (raw_value.size() < 2 || ::pltxt2htm::details::u8string_view_index<ndebug>(raw_value, 0) != u8'"' ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(raw_value, raw_value.size() - 1) != u8'"') {
        // unquoted value is not a Unity TextMeshPro link tag: keep the char-by-char fallback
        return {};
    }
    auto url_vw = ::pltxt2htm::details::u8string_view_subview<ndebug>(raw_value, 1, raw_value.size() - 2);
    auto opt_auth_end = ::pltxt2htm::details::try_parse_url_authority<ndebug>(
        url_vw, ::pltxt2htm::details::try_parse_url_scheme<ndebug>(url_vw).value_or(::std::size_t{}));
    if (opt_auth_end.has_value() == false) {
        return TryParseLinkTagResult{tag_len};
    }
    auto auth_end = opt_auth_end.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    auto path_end = ::pltxt2htm::details::try_parse_url_path_unicode<ndebug>(url_vw, auth_end);
    if (path_end != url_vw.size()) {
        return TryParseLinkTagResult{tag_len};
    }
    auto opt_url = ::pltxt2htm::details::make_try_parse_url_result<ndebug>(url_vw, url_vw.size());
    if (opt_url.has_value() == false) {
        return TryParseLinkTagResult{tag_len};
    }

    return TryParseLinkTagResult{tag_len,
                                 ::std::move(opt_url.template value<ndebug == ::pltxt2htm::Contracts::ignore>().url)};
}

struct TryParseMdUrlResult {
    ::std::size_t consumed_size;
    ::pltxt2htm::Url url;
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_url(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseMdUrlResult> {
    // First attempt: authority + path with `)` as terminator, then verify `)` follows
    auto opt_auth_end = ::pltxt2htm::details::try_parse_url_authority<ndebug>(
        pltext, ::pltxt2htm::details::try_parse_url_scheme<ndebug>(pltext).value_or(::std::size_t{}));
    if (opt_auth_end.has_value()) {
        auto auth_end = opt_auth_end.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        bool const has_path_start =
            auth_end < pltext.size() && (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, auth_end) == u8'/' ||
                                         ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, auth_end) == u8'?' ||
                                         ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, auth_end) == u8'#');
        bool const ends_after_authority =
            auth_end < pltext.size() && ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, auth_end) == u8')';
        if (has_path_start || ends_after_authority) {
            ::std::size_t path_end{auth_end};
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
            if (path_end < pltext.size() &&
                ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, path_end) == u8')') {
                auto url_vw = ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, 0, path_end);
                auto opt_result = ::pltxt2htm::details::make_try_parse_url_result<ndebug>(url_vw, path_end);
                if (opt_result.has_value()) {
                    auto&& result = opt_result.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                    return ::pltxt2htm::details::TryParseMdUrlResult{.consumed_size = path_end,
                                                                     .url = ::std::move(result.url)};
                }
            }
        }
    }

    // Fallback: locate the closing parenthesis and encode the URL in one pass.
    ::fast_io::u8string encoded{};
    ::std::size_t raw_len{};
    for (; raw_len < pltext.size(); ++raw_len) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, raw_len);
        if (chr == u8')') {
            break;
        }
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
    auto const pltext_size = pltext.size();
    pltxt2htm_assert(raw_len <= pltext_size, u8"raw_len should not exceed pltext size");
    if (raw_len == pltext_size) {
        return ::exception::nullopt;
    }
    auto encoded_vw = ::fast_io::u8string_view{encoded.data(), encoded.size()};
    auto opt_retry_auth = ::pltxt2htm::details::try_parse_url_authority<ndebug>(
        encoded_vw, ::pltxt2htm::details::try_parse_url_scheme<ndebug>(encoded_vw).value_or(::std::size_t{}));
    if (opt_retry_auth.has_value() == false) {
        return ::exception::nullopt;
    }
    auto retry_auth_end = opt_retry_auth.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    auto retry_path_end = ::pltxt2htm::details::try_parse_url_path_simple<ndebug>(encoded_vw, retry_auth_end);
    if (retry_path_end != encoded_vw.size()) {
        return ::exception::nullopt;
    }
    auto opt_result = ::pltxt2htm::details::make_try_parse_url_result<ndebug>(encoded_vw, encoded_vw.size());
    if (opt_result.has_value() == false) {
        return ::exception::nullopt;
    }
    return ::pltxt2htm::details::TryParseMdUrlResult{
        .consumed_size = raw_len,
        .url = ::std::move(opt_result.template value<ndebug == ::pltxt2htm::Contracts::ignore>().url)};
}

struct TryParseMdLinkResult {
    ::std::size_t advance_count;
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
        return ::exception::nullopt;
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
            return ::exception::nullopt;
        }
        else if (chr == u8']') {
            break;
        }
        ++current_index;
    }

    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8']') {
        return ::exception::nullopt;
    }
    ::std::size_t const link_text_end{current_index};
    ++current_index;

    // Ensure the next character is '('
    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8'(') {
        return ::exception::nullopt;
    }
    ++current_index;
    auto opt_md_url = ::pltxt2htm::details::try_parse_md_url<ndebug>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
    if (opt_md_url.has_value() == false) {
        return ::exception::nullopt;
    }
    auto&& md_url_result = opt_md_url.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    current_index += md_url_result.consumed_size;
    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8')') {
        return ::exception::nullopt;
    }
    ++current_index;
    return ::pltxt2htm::details::TryParseMdLinkResult{.advance_count = current_index,
                                                      .link_text = pltext.subview(1, link_text_end - 1),
                                                      .link_url = ::std::move(md_url_result.url)};
}

template<::pltxt2htm::Contracts ndebug>
struct TryParseMdImageResult {
    ::std::size_t advance_count;
    ::pltxt2htm::Ast<ndebug> link_text;
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
    -> ::exception::optional<::pltxt2htm::details::TryParseMdImageResult<ndebug>> {
    if (pltext.size() < 5 || ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 0) != u8'!' ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1) != u8'[') {
        return ::exception::nullopt;
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
            return ::exception::nullopt;
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
        return ::exception::nullopt;
    }
    ++current_index;

    // Ensure the next character is '('
    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8'(') {
        return ::exception::nullopt;
    }
    ++current_index;
    auto opt_md_url = ::pltxt2htm::details::try_parse_md_url<ndebug>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
    if (opt_md_url.has_value() == false) {
        return ::exception::nullopt;
    }
    auto&& md_url_result = opt_md_url.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    current_index += md_url_result.consumed_size;
    if (current_index >= pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8')') {
        return ::exception::nullopt;
    }
    return ::pltxt2htm::details::TryParseMdImageResult<ndebug>{.advance_count = current_index + 1,
                                                               .link_text = ::std::move(link_text_ast),
                                                               .link_url = ::std::move(md_url_result.url)};
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
