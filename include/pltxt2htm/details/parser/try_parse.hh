/**
 * @file try_parse.hh
 * @brief Low-level parser helpers for probing specific Physics-Lab / Markdown token patterns.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>
#include "../../container/expected.hh"
#include <fast_io/fast_io_dsal/array.h>
#include "../../container/string_view.hh"
#include "../utils.hh"
#include "../../contracts.hh"
#include "../../ast/ast.hh"
#include "../../ast/value_unit.hh"
#include "../../ast/vertical_align_value.hh"
#include "character_processing.hh"
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
constexpr auto try_parse_md_escape(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMdEscapeResult<ndebug>> {
    if (pltext.empty()) {
        return ::pltxt2htm::container::nullopt;
    }
    if (pltext.template index<ndebug>(0) != u8'\\') {
        return ::pltxt2htm::container::nullopt;
    }
    if (pltext.size() == 1) {
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{u8'\\'}), 1};
    }
    switch (pltext.template index<ndebug>(1)) {
    case u8'\\':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeBackslash{}), 2};
    case u8'!':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeExclamation{}), 2};
    case u8'\"':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeDoubleQuote{}), 2};
    case u8'#':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeHash{}), 2};
    case u8'$':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeDollar{}), 2};
    case u8'%':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapePercent{}), 2};
    case u8'&':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeAmpersand{}), 2};
    case u8'\'':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeSingleQuote{}), 2};
    case u8'(':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeLeftParen{}), 2};
    case u8')':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeRightParen{}), 2};
    case u8'*':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeAsterisk{}), 2};
    case u8'+':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapePlus{}), 2};
    case u8',':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeComma{}), 2};
    case u8'-':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeHyphen{}), 2};
    case u8'.':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeDot{}), 2};
    case u8'/':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeSlash{}), 2};
    case u8':':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeColon{}), 2};
    case u8';':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeSemicolon{}), 2};
    case u8'<':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeLessThan{}), 2};
    case u8'=':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeEquals{}), 2};
    case u8'>':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeGreaterThan{}), 2};
    case u8'?':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeQuestion{}), 2};
    case u8'@':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeAt{}), 2};
    case u8'[':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeLeftBracket{}), 2};
    case u8']':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeRightBracket{}), 2};
    case u8'^':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeCaret{}), 2};
    case u8'_':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeUnderscore{}), 2};
    case u8'`':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeBacktick{}), 2};
    case u8'{':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeLeftBrace{}), 2};
    case u8'|':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapePipe{}), 2};
    case u8'}':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeRightBrace{}), 2};
    case u8'~':
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::MdEscapeTilde{}), 2};
    default:
        return TryParseMdEscapeResult<ndebug>{::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{u8'\\'}), 1};
    }
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_character_reference_to_ast(TryDecodeCharacterReferenceResult const& reference,
                                                 ::pltxt2htm::Ast<ndebug>& result) noexcept {
    ::pltxt2htm::details::append_code_point_to_ast<ndebug>(reference.first_code_point, result);
    if (reference.code_point_count == 2) {
        ::pltxt2htm::details::append_code_point_to_ast<ndebug>(reference.second_code_point, result);
    }
}

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
[[nodiscard]] constexpr auto try_parse_bare_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    constexpr ::std::size_t tag_name_size{tag_name.size()};
    if (::pltxt2htm::details::is_prefix_match<ndebug, tag_name>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }

    ::std::size_t const pltext_size{pltext.size()};
    for (::std::size_t i{tag_name_size}; i < pltext_size; ++i) {
        auto const forward_chr = pltext.template index<ndebug>(i);
        if (forward_chr == u8'>') {
            return i;
        }
        if (forward_chr != u8' ' && forward_chr != u8'\t') {
            return ::pltxt2htm::container::nullopt;
        }
    }
    return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_caption_tag(::pltxt2htm::container::U8StringView pltext,
                                     ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_bare_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"aption"}>(pltext);
    if (opt_tag_len.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::table) {
        return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_colgroup_tag(::pltxt2htm::container::U8StringView pltext,
                                      ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_bare_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"olgroup"}>(pltext);
    if (opt_tag_len.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::table) {
        return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_thead_tag(::pltxt2htm::container::U8StringView pltext,
                                   ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_bare_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"head"}>(pltext);
    if (opt_tag_len.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::table) {
        return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_tbody_tag(::pltxt2htm::container::U8StringView pltext,
                                   ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_bare_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"body"}>(pltext);
    if (opt_tag_len.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::table) {
        return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_tfoot_tag(::pltxt2htm::container::U8StringView pltext,
                                   ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_bare_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"foot"}>(pltext);
    if (opt_tag_len.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::table) {
        return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_tr_tag(::pltxt2htm::container::U8StringView pltext,
                                ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_bare_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"r"}>(pltext);
    if (opt_tag_len.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::table && nested_tag_type != ::pltxt2htm::NodeKind::table_thead &&
        nested_tag_type != ::pltxt2htm::NodeKind::table_tbody &&
        nested_tag_type != ::pltxt2htm::NodeKind::table_tfoot) {
        return ::pltxt2htm::container::nullopt;
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
constexpr auto parse_text_align_value(::pltxt2htm::container::U8StringView value) noexcept
    -> ::pltxt2htm::container::Optional<::pltxt2htm::TableAlign> {
    // only exact lowercase values accepted
    if (value == ::pltxt2htm::container::U8StringView{u8"left"}) {
        return ::pltxt2htm::TableAlign::left;
    }
    if (value == ::pltxt2htm::container::U8StringView{u8"center"}) {
        return ::pltxt2htm::TableAlign::center;
    }
    if (value == ::pltxt2htm::container::U8StringView{u8"right"}) {
        return ::pltxt2htm::TableAlign::right;
    }
    return ::pltxt2htm::container::nullopt;
}

/**
 * @brief Result of parsing a td text-align style.
 */
struct TryParseTdStyleResult {
    ::std::size_t end; ///< Byte offset just past the closing quote, relative to the input subview.
    ::pltxt2htm::TableAlign align; ///< Last valid text-align value in the style.
};

/**
 * @brief Parse a td text-align style directly from the original tag input.
 * @param[in] pltext The quoted style value starting right after the opening quote.
 * @param[in] quote Attribute quote character.
 * @return Parsed end position (relative to the subview) and alignment on success; otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_td_style(::pltxt2htm::container::U8StringView pltext, char8_t const quote) noexcept
    -> ::pltxt2htm::container::Optional<TryParseTdStyleResult> {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t p{};
    ::pltxt2htm::TableAlign align{::pltxt2htm::TableAlign::left};
    while (p < pltext_size) {
        while (p < pltext_size &&
               (pltext.template index<ndebug>(p) == u8' ' || pltext.template index<ndebug>(p) == u8'\t')) {
            ++p;
        }
        if (p >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }

        auto const chr = pltext.template index<ndebug>(p);
        if (chr == quote) {
            return TryParseTdStyleResult{.end = p + 1, .align = align};
        }
        if (chr == u8';') {
            ++p;
            continue;
        }

        auto const segment_start{p};
        auto property_end{segment_start};
        ::std::size_t colon{};
        bool has_colon{};
        ::std::size_t value_start{};
        ::std::size_t value_end{};
        bool value_started{};
        while (p < pltext_size) {
            auto const segment_chr = pltext.template index<ndebug>(p);
            if (segment_chr == u8';' || segment_chr == quote) {
                break;
            }
            bool const is_whitespace{segment_chr == u8' ' || segment_chr == u8'\t'};
            if (has_colon == false) {
                if (segment_chr == u8':') {
                    colon = p;
                    has_colon = true;
                    value_start = p + 1;
                    value_end = value_start;
                }
                else if (is_whitespace == false) {
                    property_end = p + 1;
                }
            }
            else if (is_whitespace == false) {
                if (value_started == false) {
                    value_start = p;
                    value_started = true;
                }
                value_end = p + 1;
            }
            ++p;
        }

        // Preserve the existing behavior of ignoring declarations without a usable separator.
        if (has_colon && colon != segment_start) {
            auto const property =
                ::pltxt2htm::container::U8StringView{pltext.data() + segment_start, property_end - segment_start};
            if (property != ::pltxt2htm::container::U8StringView{u8"text-align"}) {
                return ::pltxt2htm::container::nullopt;
            }
            auto const value =
                ::pltxt2htm::container::U8StringView{pltext.data() + value_start, value_end - value_start};
            auto const opt_align = ::pltxt2htm::details::parse_text_align_value(value);
            if (opt_align.has_value() == false) {
                return ::pltxt2htm::container::nullopt;
            }
            align = opt_align.template value<ndebug>();
        }

        if (p >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        if (pltext.template index<ndebug>(p) == u8';') {
            ++p;
            continue;
        }
        if (pltext.template index<ndebug>(p) == quote) {
            return TryParseTdStyleResult{.end = p + 1, .align = align};
        }
        return ::pltxt2htm::container::nullopt;
    }
    return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_th_tag(::pltxt2htm::container::U8StringView pltext,
                                ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::pltxt2htm::container::Optional<TryParseTdTagResult> {
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"h"}>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::table_tr) {
        return ::pltxt2htm::container::nullopt;
    }

    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t pos{1}; // skip past "h"
    ::pltxt2htm::TableAlign align{::pltxt2htm::TableAlign::left};

    while (pos < pltext_size) {
        // skip whitespace
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        if (pltext.template index<ndebug>(pos) == u8'>') {
            return TryParseTdTagResult{.tag_len = pos, .align = align};
        }

        // parse attribute name
        ::std::size_t const attr_start{pos};
        while (pos < pltext_size && pltext.template index<ndebug>(pos) != u8'=' &&
               pltext.template index<ndebug>(pos) != u8'>' && pltext.template index<ndebug>(pos) != u8' ' &&
               pltext.template index<ndebug>(pos) != u8'\t' && pltext.template index<ndebug>(pos) != u8'/') {
            ++pos;
        }
        ::pltxt2htm::container::U8StringView const attr_name{pltext.data() + attr_start, pos - attr_start};

        // skip whitespace before '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'=') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }

        // parse quoted attribute value
        char8_t const quote{pltext.template index<ndebug>(pos)};
        if (quote != u8'\"' && quote != u8'\'') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos; // skip opening quote
        ::std::size_t const val_start{pos};
        while (pos < pltext_size && pltext.template index<ndebug>(pos) != quote) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        ::pltxt2htm::container::U8StringView const attr_val{pltext.data() + val_start, pos - val_start};
        ::std::size_t const attr_val_size{attr_val.size()};
        ++pos; // skip closing quote

        // only lowercase "style" attribute is checked for text-align
        if (attr_name != u8"style") {
            return ::pltxt2htm::container::nullopt;
        }
        // parse CSS property:value pairs from the style value
        ::std::size_t css_pos{};
        while (css_pos < attr_val_size) {
            // skip leading whitespace
            while (css_pos < attr_val_size && (attr_val.template index<ndebug>(css_pos) == u8' ' ||
                                               attr_val.template index<ndebug>(css_pos) == u8'\t')) {
                ++css_pos;
            }
            if (css_pos >= attr_val_size) {
                break;
            }

            // parse CSS property name (text-align)
            ::std::size_t const css_prop_start{css_pos};
            while (css_pos < attr_val_size && attr_val.template index<ndebug>(css_pos) != u8':') {
                ++css_pos;
            }
            if (css_pos >= attr_val_size) {
                break;
            }
            ::pltxt2htm::container::U8StringView const css_prop{attr_val.data() + css_prop_start,
                                                                css_pos - css_prop_start};
            ++css_pos; // skip ':'

            // skip whitespace before value
            while (css_pos < attr_val_size && (attr_val.template index<ndebug>(css_pos) == u8' ' ||
                                               attr_val.template index<ndebug>(css_pos) == u8'\t')) {
                ++css_pos;
            }

            // parse CSS value
            ::std::size_t const css_val_start{css_pos};
            while (css_pos < attr_val_size && attr_val.template index<ndebug>(css_pos) != u8';') {
                ++css_pos;
            }
            ::pltxt2htm::container::U8StringView const css_val{attr_val.data() + css_val_start,
                                                               css_pos - css_val_start};
            if (css_pos < attr_val_size) {
                ++css_pos; // skip ';'
            }

            // check text-align property
            if (css_prop != u8"text-align") {
                return ::pltxt2htm::container::nullopt;
            }
            auto opt_align = ::pltxt2htm::details::parse_text_align_value(css_val);
            if (opt_align.has_value() == false) {
                return ::pltxt2htm::container::nullopt;
            }
            align = opt_align.template value<ndebug>();
        }
    }

    return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_td_tag(::pltxt2htm::container::U8StringView pltext,
                                ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::pltxt2htm::container::Optional<TryParseTdTagResult> {
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"d"}>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::table_tr) {
        return ::pltxt2htm::container::nullopt;
    }

    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t pos{1}; // skip past "d"
    ::pltxt2htm::TableAlign align{::pltxt2htm::TableAlign::left};

    while (pos < pltext_size) {
        // skip whitespace
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        if (pltext.template index<ndebug>(pos) == u8'>') {
            return TryParseTdTagResult{.tag_len = pos, .align = align};
        }

        // parse attribute name
        ::std::size_t const attr_start{pos};
        while (pos < pltext_size && pltext.template index<ndebug>(pos) != u8'=' &&
               pltext.template index<ndebug>(pos) != u8'>' && pltext.template index<ndebug>(pos) != u8' ' &&
               pltext.template index<ndebug>(pos) != u8'\t' && pltext.template index<ndebug>(pos) != u8'/') {
            ++pos;
        }
        ::pltxt2htm::container::U8StringView const attr_name{pltext.data() + attr_start, pos - attr_start};

        // skip whitespace before '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'=') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }

        // parse quoted attribute value
        char8_t const quote{pltext.template index<ndebug>(pos)};
        if (quote != u8'"' && quote != u8'\'') {
            return ::pltxt2htm::container::nullopt;
        }
        if (attr_name != u8"style") {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos;
        auto opt_style = ::pltxt2htm::details::try_parse_td_style<ndebug>(pltext.template subview<ndebug>(pos), quote);
        if (opt_style.has_value() == false) {
            return ::pltxt2htm::container::nullopt;
        }
        auto const& style = opt_style.template value<ndebug>();
        align = style.align;
        pos += style.end;
    }
    return ::pltxt2htm::container::nullopt;
}

/**
 * @brief Return type of try_parse_p_tag: tag length and alignment.
 */
struct TryParsePTagResult {
    ::std::size_t tag_len; ///< Length of the matched tag up to the closing `>`.
    ::pltxt2htm::TextAlign align; ///< Alignment parsed from `style="text-align:..."`.
};

/**
 * @brief Parse &lt;p&gt;, optionally with `style="text-align:..."`.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting after `<p`.
 * @return Parsed tag length and alignment when valid; otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_p_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParsePTagResult> {
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"<p"}>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t pos{2}; // skip past "<p"
    ::pltxt2htm::TextAlign align{::pltxt2htm::TextAlign::left};

    while (pos < pltext_size) {
        // skip whitespace
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        if (pltext.template index<ndebug>(pos) == u8'>') {
            return TryParsePTagResult{.tag_len = pos, .align = align};
        }

        // parse attribute name
        ::std::size_t const attr_start{pos};
        while (pos < pltext_size && pltext.template index<ndebug>(pos) != u8'=' &&
               pltext.template index<ndebug>(pos) != u8'>' && pltext.template index<ndebug>(pos) != u8' ' &&
               pltext.template index<ndebug>(pos) != u8'\t' && pltext.template index<ndebug>(pos) != u8'/') {
            ++pos;
        }
        ::pltxt2htm::container::U8StringView const attr_name{pltext.data() + attr_start, pos - attr_start};
        if (attr_name != ::pltxt2htm::container::U8StringView{u8"style"}) {
            return ::pltxt2htm::container::nullopt;
        }

        // skip whitespace before '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'=') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }

        // parse quoted attribute value
        char8_t const quote{pltext.template index<ndebug>(pos)};
        if (quote != u8'\"' && quote != u8'\'') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos; // skip opening quote
        ::std::size_t const val_start{pos};
        while (pos < pltext_size && pltext.template index<ndebug>(pos) != quote) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        ::pltxt2htm::container::U8StringView const attr_val{pltext.data() + val_start, pos - val_start};
        ::std::size_t const attr_val_size{attr_val.size()};
        ++pos; // skip closing quote

        // parse CSS property:value pairs from the style value
        ::std::size_t css_pos{};
        while (css_pos < attr_val_size) {
            // skip leading whitespace
            while (css_pos < attr_val_size && (attr_val.template index<ndebug>(css_pos) == u8' ' ||
                                               attr_val.template index<ndebug>(css_pos) == u8'\t')) {
                ++css_pos;
            }
            if (css_pos >= attr_val_size) {
                break;
            }

            // parse CSS property name (text-align)
            ::std::size_t const css_prop_start{css_pos};
            while (css_pos < attr_val_size && attr_val.template index<ndebug>(css_pos) != u8':') {
                ++css_pos;
            }
            if (css_pos >= attr_val_size) {
                break;
            }
            ::pltxt2htm::container::U8StringView const css_prop{attr_val.data() + css_prop_start,
                                                                css_pos - css_prop_start};
            ++css_pos; // skip ':'

            // skip whitespace before value
            while (css_pos < attr_val_size && (attr_val.template index<ndebug>(css_pos) == u8' ' ||
                                               attr_val.template index<ndebug>(css_pos) == u8'\t')) {
                ++css_pos;
            }

            // parse CSS value
            ::std::size_t const css_val_start{css_pos};
            while (css_pos < attr_val_size && attr_val.template index<ndebug>(css_pos) != u8';') {
                ++css_pos;
            }
            ::pltxt2htm::container::U8StringView const css_val{attr_val.data() + css_val_start,
                                                               css_pos - css_val_start};
            if (css_pos < attr_val_size) {
                ++css_pos; // skip ';'
            }

            // check text-align property
            if (css_prop != ::pltxt2htm::container::U8StringView{u8"text-align"}) {
                return ::pltxt2htm::container::nullopt;
            }
            // only exact lowercase values accepted
            if (css_val == ::pltxt2htm::container::U8StringView{u8"left"}) {
                align = ::pltxt2htm::TextAlign::left;
            }
            else if (css_val == ::pltxt2htm::container::U8StringView{u8"center"}) {
                align = ::pltxt2htm::TextAlign::center;
            }
            else if (css_val == ::pltxt2htm::container::U8StringView{u8"right"}) {
                align = ::pltxt2htm::TextAlign::right;
            }
            else if (css_val == ::pltxt2htm::container::U8StringView{u8"justify"}) {
                align = ::pltxt2htm::TextAlign::justify;
            }
            else {
                return ::pltxt2htm::container::nullopt;
            }
        }
    }

    return ::pltxt2htm::container::nullopt;
}

/**
 * @brief Return type of try_parse_equal_sign_tag: tag length and extracted value.
 */
struct TryParseEqualSignTagResult {
    ::std::size_t tag_len; ///< Length of the tag.
    ::pltxt2htm::container::U8StringView substr; ///< View of the value extracted from the tag.
};

/**
 * @brief Find the end of a value whose characters are accepted by a named predicate.
 * @details `pltext` must already be subviewed so that it starts at the value, and
 *          the returned value is the relative end (i.e. the run length). Callers
 *          re-add the value-start offset when they need absolute coordinates.
 */
template<::pltxt2htm::Contracts ndebug, auto value_char_predicate>
    requires requires(char8_t chr) {
        { value_char_predicate(chr) } -> ::std::same_as<bool>;
    }
[[nodiscard]]
constexpr auto find_value_end(::pltxt2htm::container::U8StringView pltext) noexcept -> ::std::size_t {
    ::std::size_t const pltext_size{pltext.size()};
    auto pos = ::std::size_t{0};
    while (pos < pltext_size) {
        auto const chr = pltext.template index<ndebug>(pos);
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
constexpr bool is_equal_sign_tag_prefix(::pltxt2htm::container::U8StringView pltext) noexcept {
    constexpr auto prefix_with_equal =
        ::pltxt2htm::details::concat(prefix_str, ::pltxt2htm::details::U8LiteralString{u8"="});
    return ::pltxt2htm::details::is_prefix_match<ndebug, prefix_with_equal>(pltext);
}

/**
 * @brief Parse trailing whitespace and the closing bracket of an equals-sign tag.
 * @details `pltext` must already be subviewed so that it starts at the end of the
 *          tag value. The returned value is the index of the closing `>` relative
 *          to that subview; callers re-add the value-end offset when they need
 *          absolute coordinates. The caller is responsible for the value substring.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_equal_sign_tag_suffix(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    ::std::size_t const pltext_size{pltext.size()};
    auto pos = ::std::size_t{0};
    while (pos < pltext_size &&
           (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
        ++pos;
    }
    if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'>') {
        return ::pltxt2htm::container::nullopt;
    }
    return pos;
}

/**
 * @brief Parse an equals-sign tag whose value uses a stateless character grammar.
 */
template<::pltxt2htm::Contracts ndebug, ::pltxt2htm::details::U8LiteralString prefix_str, auto value_char_predicate>
[[nodiscard]]
constexpr auto try_parse_equal_sign_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseEqualSignTagResult> {
    if (::pltxt2htm::details::is_equal_sign_tag_prefix<ndebug, prefix_str>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    constexpr auto value_start = prefix_str.size() + 1;
    auto const value_end_rel = ::pltxt2htm::details::find_value_end<ndebug, value_char_predicate>(
        pltext.template subview<ndebug>(value_start));
    if (value_end_rel == 0) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const value_end = value_start + value_end_rel;
    auto opt_close =
        ::pltxt2htm::details::try_parse_equal_sign_tag_suffix<ndebug>(pltext.template subview<ndebug>(value_end));
    if (opt_close.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const close_rel = opt_close.template value<ndebug>();
    return TryParseEqualSignTagResult{.tag_len = value_end + close_rel,
                                      .substr = pltext.template subview<ndebug>(value_start, value_end_rel)};
}

/**
 * @brief Parse a color value and return the relative end within `pltext`.
 * @details `pltext` must start at the value (the caller subviews it); the returned end is
 *          relative to `pltext`, so the caller re-adds its absolute offset.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_color_value(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    if (pltext.empty()) {
        return ::pltxt2htm::container::nullopt;
    }
    if (pltext.template index<ndebug>(0) != u8'#') {
        auto const end = ::pltxt2htm::details::find_value_end<ndebug, ::pltxt2htm::details::is_ascii_alpha>(pltext);
        if (end == 0) {
            return ::pltxt2htm::container::nullopt;
        }
        return end;
    }

    auto const hex_size = ::pltxt2htm::details::find_value_end<ndebug, ::pltxt2htm::details::is_ascii_hexdigit>(
        pltext.template subview<ndebug>(1));
    if (hex_size != 3 && hex_size != 4 && hex_size != 6 && hex_size != 8) {
        return ::pltxt2htm::container::nullopt;
    }
    return hex_size + 1;
}

/**
 * @brief Parse `<color=...>` and validate value against a strict color grammar.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext Input text starting at `olor=...`.
 * @return Parsed tag result when valid; otherwise nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_color_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseEqualSignTagResult> {
    constexpr auto prefix_str = ::pltxt2htm::details::U8LiteralString{u8"olor"};
    if (::pltxt2htm::details::is_equal_sign_tag_prefix<ndebug, prefix_str>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    constexpr auto value_start = prefix_str.size() + 1;
    auto opt_value_end =
        ::pltxt2htm::details::try_parse_color_value<ndebug>(pltext.template subview<ndebug>(value_start));
    if (opt_value_end.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const value_end = opt_value_end.template value<ndebug>() + value_start;
    auto opt_close =
        ::pltxt2htm::details::try_parse_equal_sign_tag_suffix<ndebug>(pltext.template subview<ndebug>(value_end));
    if (opt_close.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const close_rel = opt_close.template value<ndebug>();
    return TryParseEqualSignTagResult{.tag_len = value_end + close_rel,
                                      .substr = pltext.template subview<ndebug>(value_start, value_end - value_start)};
}

/**
 * @brief Return type of try_parse_size_tag: tag length and the parsed font-size value.
 */
struct TryParseSizeTagResult {
    ::std::size_t tag_len;
    ::pltxt2htm::ValueWithUnit<double> value;
};

/**
 * @brief Parse a `<size=N>`, `<size=N%>` or `<size=Nem>` opening tag.
 * @details N may be an integer or a fractional decimal such as `12.5`.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_size_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseSizeTagResult> {
    constexpr auto prefix_str = ::pltxt2htm::details::U8LiteralString{u8"ize"};
    if (::pltxt2htm::details::is_equal_sign_tag_prefix<ndebug, prefix_str>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    constexpr auto value_start = prefix_str.size() + 1;
    auto opt_value =
        ::pltxt2htm::details::try_parse_double_decimal_value<ndebug>(pltext.template subview<ndebug>(value_start));
    if (opt_value.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const value = opt_value.template value<ndebug>();
    auto value_end = value_start + value.end;
    auto unit = ::pltxt2htm::Unit::px;
    if (value_end < pltext.size() && pltext.template index<ndebug>(value_end) == u8'%') {
        unit = ::pltxt2htm::Unit::percent;
        ++value_end;
    }
    else if (value_end + 1 < pltext.size() && pltext.template index<ndebug>(value_end) == u8'e' &&
             pltext.template index<ndebug>(value_end + 1) == u8'm') {
        unit = ::pltxt2htm::Unit::em;
        value_end += 2;
    }
    auto opt_close =
        ::pltxt2htm::details::try_parse_equal_sign_tag_suffix<ndebug>(pltext.template subview<ndebug>(value_end));
    if (opt_close.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const tag_len = value_end + opt_close.template value<ndebug>();
    return TryParseSizeTagResult{tag_len, {value.value, unit}};
}

/**
 * @brief Return type of try_parse_span_tag: tag length, color, and optional font-size/vertical-align.
 */
template<::pltxt2htm::Contracts ndebug>
struct TryParseSpanTagResult {
    ::std::size_t tag_len; ///< Length of the matched tag.
    ::fast_io::u8string color; ///< Extracted color value.
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>>
        font_size; ///< Extracted font-size value+unit (if present).
    ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>>
        vertical_align; ///< Extracted vertical-align value (if present).
};

/**
 * @brief Result of parsing a CSS font-size value.
 */
struct TryParseFontSizeValueResult {
    ::std::size_t end;
    ::pltxt2htm::ValueWithUnit<double> value;
};

/**
 * @brief Parse a non-zero number optionally followed by lowercase `px`, `em` or `%`.
 * @details `pltext` must already be subviewed so that it starts at the value, and
 *          the returned `end` is relative to that subview.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_font_size_value(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseFontSizeValueResult> {
    auto opt_decimal = ::pltxt2htm::details::try_parse_double_decimal_value<ndebug>(pltext);
    if (opt_decimal.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const decimal = opt_decimal.template value<ndebug>();
    if (decimal.value == 0) {
        return ::pltxt2htm::container::nullopt;
    }
    auto pos = decimal.end;
    auto unit = ::pltxt2htm::Unit::px;
    if (pos < pltext.size() && pltext.template index<ndebug>(pos) == u8'p') {
        ++pos;
        if (pos >= pltext.size() || pltext.template index<ndebug>(pos) != u8'x') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos;
    }
    else if (pos < pltext.size() && pltext.template index<ndebug>(pos) == u8'%') {
        unit = ::pltxt2htm::Unit::percent;
        ++pos;
    }
    else if (pos + 1 < pltext.size() && pltext.template index<ndebug>(pos) == u8'e' &&
             pltext.template index<ndebug>(pos + 1) == u8'm') {
        unit = ::pltxt2htm::Unit::em;
        pos += 2;
    }
    return TryParseFontSizeValueResult{pos, {decimal.value, unit}};
}

/**
 * @brief Result of parsing a signed CSS length value.
 */
struct TryParseSignedLengthValueResult {
    ::std::size_t end;
    ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t> value;
};

/**
 * @brief Parse a non-zero signed integer optionally followed by lowercase `px`, `em` or `%`.
 * @details Accepts an optional leading U+002D '-' sign, so negative lengths such as
 *          `-5px` (used by `vertical-align`) are representable. `pltext` must already
 *          be subviewed so that it starts at the value, and the returned `end` is
 *          relative to that subview.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_signed_length_value(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseSignedLengthValueResult> {
    auto opt_decimal = ::pltxt2htm::details::try_parse_ptrdiff_t_decimal_value<ndebug>(pltext);
    if (opt_decimal.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const decimal = opt_decimal.template value<ndebug>();
    if (decimal.value == 0) {
        return ::pltxt2htm::container::nullopt;
    }
    auto pos = decimal.end;
    auto unit = ::pltxt2htm::Unit::px;
    if (pos < pltext.size() && pltext.template index<ndebug>(pos) == u8'p') {
        ++pos;
        if (pos >= pltext.size() || pltext.template index<ndebug>(pos) != u8'x') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos;
    }
    else if (pos < pltext.size() && pltext.template index<ndebug>(pos) == u8'%') {
        unit = ::pltxt2htm::Unit::percent;
        ++pos;
    }
    else if (pos + 1 < pltext.size() && pltext.template index<ndebug>(pos) == u8'e' &&
             pltext.template index<ndebug>(pos + 1) == u8'm') {
        unit = ::pltxt2htm::Unit::em;
        pos += 2;
    }
    return TryParseSignedLengthValueResult{pos, {decimal.value, unit}};
}

/**
 * @brief Return type of try_parse_voffset_tag: tag length and the parsed offset value+unit.
 */
struct TryParseVoffsetTagResult {
    ::std::size_t tag_len;
    ::pltxt2htm::ValueWithUnit<::std::ptrdiff_t> value;
};

/**
 * @brief Parse a `<voffset=N>` opening tag.
 * @details N is a signed non-zero length with an optional lowercase `px` or `em` unit
 *          (Unity TextMeshPro rich text); the unit defaults to `px` when omitted. A zero
 *          value is rejected so `<voffset=0>` renders as literal text. A percent unit is
 *          also rejected so `<voffset=50%>` renders as literal text (the backend asserts
 *          the no-percent invariant as a safety net).
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_voffset_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseVoffsetTagResult> {
    constexpr auto prefix_str = ::pltxt2htm::details::U8LiteralString{u8"offset"};
    if (::pltxt2htm::details::is_equal_sign_tag_prefix<ndebug, prefix_str>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    constexpr auto value_start = prefix_str.size() + 1;
    auto opt_value =
        ::pltxt2htm::details::try_parse_signed_length_value<ndebug>(pltext.template subview<ndebug>(value_start));
    if (opt_value.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const value = opt_value.template value<ndebug>();
    if (value.value.unit == ::pltxt2htm::Unit::percent) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const value_end = value_start + value.end;
    auto opt_close =
        ::pltxt2htm::details::try_parse_equal_sign_tag_suffix<ndebug>(pltext.template subview<ndebug>(value_end));
    if (opt_close.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const tag_len = value_end + opt_close.template value<ndebug>();
    return TryParseVoffsetTagResult{tag_len, value.value};
}

/**
 * @brief Return type of try_parse_align_tag: tag length and the parsed alignment keyword.
 */
struct TryParseAlignTagResult {
    ::std::size_t tag_len;
    ::pltxt2htm::TextAlign align;
};

/**
 * @brief Parse a `<align=value>` opening tag (Unity TextMeshPro rich text).
 * @details The whole `<align=...>` tag is consumed, matching the `lign` name (case
 *          insensitive) after the leading `<`. The value is a text-alignment keyword,
 *          optionally wrapped in double quotes: `left`, `center`, `right`, `justified`
 *          (or `"left"`, `"center"`, etc.). Any other value makes the tag render as
 *          literal text.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_align_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseAlignTagResult> {
    constexpr auto prefix_str = ::pltxt2htm::details::U8LiteralString{u8"<align"};
    if (::pltxt2htm::details::is_equal_sign_tag_prefix<ndebug, prefix_str>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    constexpr auto value_start = prefix_str.size() + 1;
    auto value_view = pltext.template subview<ndebug>(value_start);
    bool const quoted = value_view.size() != 0 && value_view.template index<ndebug>(0) == u8'"';
    ::std::size_t const quote_offset = quoted ? 1 : 0;
    if (quoted) {
        value_view = value_view.template subview<ndebug>(1);
    }
    ::std::size_t const value_view_size{value_view.size()};
    ::pltxt2htm::TextAlign align{};
    ::std::size_t value_end{0};

    struct AlignCandidate {
        ::pltxt2htm::container::U8StringView keyword;
        ::pltxt2htm::TextAlign align;
    };

    constexpr AlignCandidate candidates[]{
        {u8"justified", ::pltxt2htm::TextAlign::justify},
        {u8"left", ::pltxt2htm::TextAlign::left},
        {u8"center", ::pltxt2htm::TextAlign::center},
        {u8"right", ::pltxt2htm::TextAlign::right},
    };
    bool matched{false};
    for (auto const& candidate : candidates) {
        auto const keyword = candidate.keyword;
        ::std::size_t const keyword_size{keyword.size()};
        auto const candidate_align = candidate.align;
        if (value_view_size < keyword_size) {
            continue;
        }
        if (::pltxt2htm::container::U8StringView{value_view.data(), keyword_size} != keyword) {
            continue;
        }
        if (value_view_size > keyword_size) {
            auto const next = value_view.template index<ndebug>(keyword_size);
            if (::pltxt2htm::details::is_ascii_alpha(next) || ::pltxt2htm::details::is_ascii_digit(next)) {
                continue;
            }
        }
        align = candidate_align;
        value_end = keyword_size;
        matched = true;
        break;
    }
    if (matched == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto after_value = value_view.template subview<ndebug>(value_end);
    ::std::size_t quote_close_offset{0};
    if (quoted) {
        ::std::size_t const after_value_size{after_value.size()};
        while (quote_close_offset < after_value_size &&
               (after_value.template index<ndebug>(quote_close_offset) == u8' ' ||
                after_value.template index<ndebug>(quote_close_offset) == u8'\t')) {
            ++quote_close_offset;
        }
        if (quote_close_offset >= after_value_size || after_value.template index<ndebug>(quote_close_offset) != u8'"') {
            return ::pltxt2htm::container::nullopt;
        }
        ++quote_close_offset;
        after_value = after_value.template subview<ndebug>(quote_close_offset);
    }
    auto opt_close = ::pltxt2htm::details::try_parse_equal_sign_tag_suffix<ndebug>(after_value);
    if (opt_close.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const tag_len =
        value_start + quote_offset + value_end + quote_close_offset + opt_close.template value<ndebug>();
    return TryParseAlignTagResult{tag_len, align};
}

/**
 * @brief Result of parsing a single margin value with optional unit.
 */
struct TryParseMarginValueResult {
    ::std::size_t end;
    ::pltxt2htm::ValueWithUnit<::std::size_t> value;
};

/**
 * @brief Parse an unsigned decimal margin value followed by an optional `px`/`em`/`%`
 *        unit at the start of `pltext`.
 * @details The unit defaults to `px`; `0` is allowed. Returns the length of the consumed
 *          value plus unit together with the parsed value.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_margin_value(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMarginValueResult> {
    auto opt_value = ::pltxt2htm::details::try_parse_size_t_decimal_value<ndebug>(pltext);
    if (opt_value.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const parsed = opt_value.template value<ndebug>();
    auto value_end = parsed.end;
    auto unit = ::pltxt2htm::Unit::px;
    if (value_end < pltext.size() && pltext.template index<ndebug>(value_end) == u8'p') {
        ++value_end;
        if (value_end >= pltext.size() || pltext.template index<ndebug>(value_end) != u8'x') {
            return ::pltxt2htm::container::nullopt;
        }
        ++value_end;
    }
    else if (value_end < pltext.size() && pltext.template index<ndebug>(value_end) == u8'%') {
        unit = ::pltxt2htm::Unit::percent;
        ++value_end;
    }
    else if (value_end + 1 < pltext.size() && pltext.template index<ndebug>(value_end) == u8'e' &&
             pltext.template index<ndebug>(value_end + 1) == u8'm') {
        unit = ::pltxt2htm::Unit::em;
        value_end += 2;
    }
    return TryParseMarginValueResult{value_end, {parsed.value, unit}};
}

/**
 * @brief Result of parsing a `<margin=V>`-style single-value tag.
 */
struct TryParseMarginSingleResult {
    ::std::size_t tag_len;
    ::pltxt2htm::ValueWithUnit<::std::size_t> value;
};

/**
 * @brief Parse the value and closing bracket of a single-value margin tag.
 * @details `pltext` must start at the value (right after the `=` sign; the caller
 *          subviews it). The returned `tag_len` is relative to `pltext`, so the
 *          caller re-adds its absolute offset. The tag spans through the closing `>`.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_margin_single(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMarginSingleResult> {
    auto opt_value = ::pltxt2htm::details::try_parse_margin_value<ndebug>(pltext);
    if (opt_value.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const value = opt_value.template value<ndebug>();
    auto opt_close =
        ::pltxt2htm::details::try_parse_equal_sign_tag_suffix<ndebug>(pltext.template subview<ndebug>(value.end));
    if (opt_close.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const tag_len = value.end + opt_close.template value<ndebug>();
    return TryParseMarginSingleResult{tag_len, value.value};
}

/**
 * @brief Return type of try_parse_margin_tag: tag length plus optional left/right margins.
 */
struct TryParseMarginTagResult {
    ::std::size_t tag_len;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right;
};

/**
 * @brief Parse the space-separated attribute form `<margin left=V right=W>`.
 * @details `pltext` must start right after the `<margin` name (the caller subviews
 *          it). The returned `tag_len` is relative to `pltext`, so the caller
 *          re-adds its absolute offset. Attributes are `left=`/`right=`
 *          (case-insensitive) followed by a value with optional unit, separated by
 *          spaces or tabs. At least one recognized attribute is required; unknown
 *          names, missing `=`, malformed values, or a repeated side reject the tag.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_margin_attributes(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMarginTagResult> {
    ::std::size_t const pltext_size{pltext.size()};
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left = ::pltxt2htm::container::nullopt;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right = ::pltxt2htm::container::nullopt;
    auto pos = ::std::size_t{0};
    bool saw_attribute = false;
    while (true) {
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        if (pltext.template index<ndebug>(pos) == u8'>') {
            if (saw_attribute == false) {
                return ::pltxt2htm::container::nullopt;
            }
            return TryParseMarginTagResult{.tag_len = pos, .left = left, .right = right};
        }
        bool const is_left =
            ::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"left"}>(
                pltext.template subview<ndebug>(pos));
        bool const is_right =
            ::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"right"}>(
                pltext.template subview<ndebug>(pos));
        if (is_left == false && is_right == false) {
            return ::pltxt2htm::container::nullopt;
        }
        auto const name_len = is_left ? ::std::size_t{4} : ::std::size_t{5};
        pos += name_len;
        if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'=') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos;
        auto opt_value = ::pltxt2htm::details::try_parse_margin_value<ndebug>(pltext.template subview<ndebug>(pos));
        if (opt_value.has_value() == false) {
            return ::pltxt2htm::container::nullopt;
        }
        auto const value = opt_value.template value<ndebug>();
        if (is_left) {
            if (left.has_value()) {
                return ::pltxt2htm::container::nullopt;
            }
            left = value.value;
        }
        else {
            if (right.has_value()) {
                return ::pltxt2htm::container::nullopt;
            }
            right = value.value;
        }
        pos += value.end;
        saw_attribute = true;
    }
}

/**
 * @brief Parse a `<margin-left=value>`, `<margin-right=value>`, `<margin=value>` or
 *        `<margin left=value right=value>` opening tag (Unity TextMeshPro rich text).
 * @details The whole `<margin...>` tag is consumed, matching the tag name (case
 *          insensitive) after the leading `<`. `<margin=V>` sets both sides to the same
 *          value; the attribute form accepts `left=`/`right=` names (case-insensitive)
 *          separated by whitespace, and only the specified sides are set. Values are
 *          unsigned integers optionally followed by `px`, `em` or `%` (the unit defaults
 *          to px); `0` is allowed. Any malformed tag renders as literal text.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_margin_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMarginTagResult> {
    ::std::size_t const pltext_size{pltext.size()};
    constexpr auto prefix_str = ::pltxt2htm::details::U8LiteralString{u8"<margin"};
    constexpr auto left_prefix_str = ::pltxt2htm::details::U8LiteralString{u8"<margin-left"};
    constexpr auto right_prefix_str = ::pltxt2htm::details::U8LiteralString{u8"<margin-right"};
    if (::pltxt2htm::details::is_equal_sign_tag_prefix<ndebug, left_prefix_str>(pltext)) {
        constexpr auto value_start = left_prefix_str.size() + 1;
        auto opt_single =
            ::pltxt2htm::details::try_parse_margin_single<ndebug>(pltext.template subview<ndebug>(value_start));
        if (opt_single.has_value() == false) {
            return ::pltxt2htm::container::nullopt;
        }
        auto const single = opt_single.template value<ndebug>();
        return TryParseMarginTagResult{value_start + single.tag_len, single.value, ::pltxt2htm::container::nullopt};
    }
    if (::pltxt2htm::details::is_equal_sign_tag_prefix<ndebug, right_prefix_str>(pltext)) {
        constexpr auto value_start = right_prefix_str.size() + 1;
        auto opt_single =
            ::pltxt2htm::details::try_parse_margin_single<ndebug>(pltext.template subview<ndebug>(value_start));
        if (opt_single.has_value() == false) {
            return ::pltxt2htm::container::nullopt;
        }
        auto const single = opt_single.template value<ndebug>();
        return TryParseMarginTagResult{value_start + single.tag_len, ::pltxt2htm::container::nullopt, single.value};
    }
    if (::pltxt2htm::details::is_equal_sign_tag_prefix<ndebug, prefix_str>(pltext)) {
        constexpr auto value_start = prefix_str.size() + 1;
        auto opt_single =
            ::pltxt2htm::details::try_parse_margin_single<ndebug>(pltext.template subview<ndebug>(value_start));
        if (opt_single.has_value() == false) {
            return ::pltxt2htm::container::nullopt;
        }
        auto const single = opt_single.template value<ndebug>();
        return TryParseMarginTagResult{value_start + single.tag_len, single.value, single.value};
    }
    if (::pltxt2htm::details::is_prefix_match<ndebug, prefix_str>(pltext)) {
        if (pltext_size > prefix_str.size()) {
            auto const next = pltext.template index<ndebug>(prefix_str.size());
            if (next == u8' ' || next == u8'\t') {
                constexpr auto start = prefix_str.size();
                auto opt_attributes =
                    ::pltxt2htm::details::try_parse_margin_attributes<ndebug>(pltext.template subview<ndebug>(start));
                if (opt_attributes.has_value() == false) {
                    return ::pltxt2htm::container::nullopt;
                }
                auto const attributes = opt_attributes.template value<ndebug>();
                return TryParseMarginTagResult{start + attributes.tag_len, attributes.left, attributes.right};
            }
        }
    }
    return ::pltxt2htm::container::nullopt;
}

/**
 * @brief Return type of try_parse_html_div_tag: tag length plus optional left/right margins.
 */
struct TryParseHtmlDivTagResult {
    ::std::size_t tag_len;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left;
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right;
};

/**
 * @brief Parse an HTML &lt;div style="margin-left:V;margin-right:W"&gt; opening tag.
 * @details `pltext` must start at the `<` of the tag (the caller subviews it). Only the
 *          `style` attribute is accepted, and inside it only `margin-left` and/or
 *          `margin-right` declarations (each with an unsigned px/em/% value, parsed via
 *          ::pltxt2htm::details::try_parse_margin_value) are allowed. Any other attribute
 *          or CSS declaration makes the whole tag invalid so the caller falls back to
 *          treating it as literal text, keeping the emitted HTML XSS-safe.
 * @param[in] pltext The input text view starting at the candidate tag.
 * @return The consumed tag length (the index of the closing `>` relative to `pltext`,
 *          i.e. excluding the `>` itself) plus the optional left/right margins.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_html_div_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseHtmlDivTagResult> {
    ::std::size_t const pltext_size{pltext.size()};
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"<div"}>(pltext) ==
        false) {
        return ::pltxt2htm::container::nullopt;
    }
    ::std::size_t pos{4}; // skip past "<div"

    // match the single allowed attribute name "style" (case-sensitive, like the span parser)
    while (pos < pltext_size &&
           (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
        ++pos;
    }
    constexpr auto style_str = ::pltxt2htm::details::U8LiteralString{u8"style"};
    if (pltext_size - pos < style_str.size() ||
        ::pltxt2htm::details::is_prefix_match<ndebug, style_str>(pltext.template subview<ndebug>(pos)) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    pos += style_str.size();

    while (pos < pltext_size &&
           (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
        ++pos;
    }
    if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'=') {
        return ::pltxt2htm::container::nullopt;
    }
    ++pos;
    while (pos < pltext_size &&
           (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
        ++pos;
    }
    if (pos >= pltext_size) {
        return ::pltxt2htm::container::nullopt;
    }
    char8_t const quote{pltext.template index<ndebug>(pos)};
    if (quote != u8'"' && quote != u8'\'') {
        return ::pltxt2htm::container::nullopt;
    }
    ++pos;

    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> left{::pltxt2htm::container::nullopt};
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<::std::size_t>> right{::pltxt2htm::container::nullopt};

    while (true) {
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        if (pltext.template index<ndebug>(pos) == quote) {
            ++pos;
            break;
        }

        // property name: only margin-left / margin-right are allowed
        constexpr auto left_str = ::pltxt2htm::details::U8LiteralString{u8"margin-left"};
        constexpr auto right_str = ::pltxt2htm::details::U8LiteralString{u8"margin-right"};
        bool const is_left =
            pltext_size - pos >= left_str.size() &&
            ::pltxt2htm::details::is_prefix_match<ndebug, left_str>(pltext.template subview<ndebug>(pos));
        bool const is_right =
            is_left == false && pltext_size - pos >= right_str.size() &&
            ::pltxt2htm::details::is_prefix_match<ndebug, right_str>(pltext.template subview<ndebug>(pos));
        if (is_left == false && is_right == false) {
            return ::pltxt2htm::container::nullopt;
        }
        pos += is_left ? left_str.size() : right_str.size();

        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8':') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos;
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }

        auto opt_value = ::pltxt2htm::details::try_parse_margin_value<ndebug>(pltext.template subview<ndebug>(pos));
        if (opt_value.has_value() == false) {
            return ::pltxt2htm::container::nullopt;
        }
        auto const value = opt_value.template value<ndebug>();
        pos += value.end;

        if (is_left) {
            if (left.has_value()) {
                return ::pltxt2htm::container::nullopt; // duplicate margin-left declaration
            }
            left = value.value;
        }
        else {
            if (right.has_value()) {
                return ::pltxt2htm::container::nullopt; // duplicate margin-right declaration
            }
            right = value.value;
        }

        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        if (pltext.template index<ndebug>(pos) == u8';') {
            ++pos;
            continue;
        }
        if (pltext.template index<ndebug>(pos) == quote) {
            ++pos;
            break;
        }
        return ::pltxt2htm::container::nullopt;
    }

    // a div must declare at least one margin (style must contain margin-left/right only)
    if (left.has_value() == false && right.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }

    while (pos < pltext_size &&
           (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
        ++pos;
    }
    if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'>') {
        return ::pltxt2htm::container::nullopt;
    }

    return TryParseHtmlDivTagResult{.tag_len = pos, .left = left, .right = right};
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
 * @brief Try to match a lowercase CSS vertical-align keyword at the start of `pltext`.
 * @details `pltext` must be a view starting at the candidate keyword (the caller subviews
 *          it). Rejects the keyword when the following character is an ASCII letter or
 *          digit, so that e.g. "superx" or "sub2" do not silently parse as keywords.
 * @param[in] pltext The input text view starting at the candidate keyword.
 * @param[in] keyword The keyword spelling to match.
 * @return The consumed length (`keyword.size()`) on success; nullopt on mismatch.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_vertical_align_keyword(::pltxt2htm::container::U8StringView pltext,
                                                ::pltxt2htm::container::U8StringView const keyword) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    if (pltext.size() < keyword.size()) {
        return ::pltxt2htm::container::nullopt;
    }
    if (::pltxt2htm::container::U8StringView{pltext.data(), keyword.size()} != keyword) {
        return ::pltxt2htm::container::nullopt;
    }
    if (pltext.size() > keyword.size()) {
        auto const next = pltext.template index<ndebug>(keyword.size());
        if (::pltxt2htm::details::is_ascii_alpha(next) || ::pltxt2htm::details::is_ascii_digit(next)) {
            return ::pltxt2htm::container::nullopt;
        }
    }
    return keyword.size();
}

/**
 * @brief Parse a CSS vertical-align value: a lowercase keyword or a non-zero px/em/% length.
 * @details `pltext` must start at the value (the caller subviews past whitespace); the returned
 *          `end` is relative to `pltext`, so the caller re-adds its absolute offset.
 * @param[in] pltext Input text starting at the value.
 * @return Parsed value with its relative end on success; nullopt on failure.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_vertical_align_value(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseVerticalAlignValueResult<ndebug>> {
    struct VerticalAlignKeywordEntry {
        ::pltxt2htm::VerticalAlignKeyword keyword;
        ::pltxt2htm::container::U8StringView spelling;
    };

    static constexpr auto keywords = ::fast_io::array{
        VerticalAlignKeywordEntry{::pltxt2htm::VerticalAlignKeyword::baseline,
                                  ::pltxt2htm::container::U8StringView{u8"baseline"}},
        VerticalAlignKeywordEntry{::pltxt2htm::VerticalAlignKeyword::text_bottom,
                                  ::pltxt2htm::container::U8StringView{u8"text-bottom"}},
        VerticalAlignKeywordEntry{::pltxt2htm::VerticalAlignKeyword::text_top,
                                  ::pltxt2htm::container::U8StringView{u8"text-top"}},
        VerticalAlignKeywordEntry{::pltxt2htm::VerticalAlignKeyword::super,
                                  ::pltxt2htm::container::U8StringView{u8"super"}},
        VerticalAlignKeywordEntry{::pltxt2htm::VerticalAlignKeyword::sub,
                                  ::pltxt2htm::container::U8StringView{u8"sub"}},
        VerticalAlignKeywordEntry{::pltxt2htm::VerticalAlignKeyword::middle,
                                  ::pltxt2htm::container::U8StringView{u8"middle"}},
        VerticalAlignKeywordEntry{::pltxt2htm::VerticalAlignKeyword::bottom,
                                  ::pltxt2htm::container::U8StringView{u8"bottom"}},
        VerticalAlignKeywordEntry{::pltxt2htm::VerticalAlignKeyword::top,
                                  ::pltxt2htm::container::U8StringView{u8"top"}},
    };
    for (auto const& entry : keywords) {
        auto opt_len = ::pltxt2htm::details::try_parse_vertical_align_keyword<ndebug>(pltext, entry.spelling);
        if (opt_len.has_value()) {
            auto const len = opt_len.template value<ndebug>();
            return TryParseVerticalAlignValueResult<ndebug>{
                .end = len, .value = ::pltxt2htm::VerticalAlignValue<ndebug>{entry.keyword}};
        }
    }
    auto opt_length = ::pltxt2htm::details::try_parse_signed_length_value<ndebug>(pltext);
    if (opt_length.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const length = opt_length.template value<ndebug>();
    return TryParseVerticalAlignValueResult<ndebug>{.end = length.end,
                                                    .value = ::pltxt2htm::VerticalAlignValue<ndebug>{length.value}};
}

/**
 * @brief Skip trailing whitespace and require a CSS property delimiter.
 * @details `pltext` must start right after a property value (the caller subviews it); the
 *          returned offset is relative to `pltext`.
 * @param[in] pltext The input text view starting after a property value.
 * @param[in] quote The quote character that terminates the style value.
 * @return The offset of the `;`/quote delimiter after skipped whitespace; nullopt when
 *         no delimiter follows.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_span_style_property_suffix(::pltxt2htm::container::U8StringView pltext,
                                                    char8_t const quote) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t pos{};
    while (pos < pltext_size &&
           (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
        ++pos;
    }
    if (pos >= pltext_size) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const chr = pltext.template index<ndebug>(pos);
    if (chr != u8';' && chr != quote) {
        return ::pltxt2htm::container::nullopt;
    }
    return pos;
}

/**
 * @brief Result of parsing the CSS declarations inside a span style attribute value.
 */
template<::pltxt2htm::Contracts ndebug>
struct TryParseSpanStyleResult {
    ::std::size_t end; ///< Byte offset just past the closing quote, relative to the input subview.
    ::fast_io::u8string color; ///< Extracted color value, empty if none was present.
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>>
        font_size; ///< Extracted font-size value+unit, if present.
    ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>>
        vertical_align; ///< Extracted vertical-align value, if present.
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_span_style(::pltxt2htm::container::U8StringView pltext, char8_t const quote) noexcept
    -> ::pltxt2htm::container::Optional<TryParseSpanStyleResult<ndebug>> {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t p{};
    ::fast_io::u8string color{};
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> font_size{::pltxt2htm::container::nullopt};
    ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> vertical_align{
        ::pltxt2htm::container::nullopt};

    while (p < pltext_size) {
        while (p < pltext_size &&
               (pltext.template index<ndebug>(p) == u8' ' || pltext.template index<ndebug>(p) == u8'\t')) {
            ++p;
        }
        if (p >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }

        auto const chr{pltext.template index<ndebug>(p)};
        if (chr == quote) {
            return TryParseSpanStyleResult<ndebug>{.end = p + 1,
                                                   .color = ::std::move(color),
                                                   .font_size = ::std::move(font_size),
                                                   .vertical_align = ::std::move(vertical_align)};
        }
        if (chr == u8';') {
            ++p;
            continue;
        }

        auto const property_start{p};
        auto property_end{p};
        while (p < pltext_size) {
            auto const property_chr{pltext.template index<ndebug>(p)};
            if (property_chr == u8':') {
                break;
            }
            if (property_chr == u8';' || property_chr == quote) {
                return ::pltxt2htm::container::nullopt;
            }
            if (property_chr != u8' ' && property_chr != u8'\t') {
                property_end = p + 1;
            }
            ++p;
        }
        if (p >= pltext_size || property_end == property_start) {
            return ::pltxt2htm::container::nullopt;
        }
        auto const property{
            ::pltxt2htm::container::U8StringView{pltext.data() + property_start, property_end - property_start}};
        ++p;

        while (p < pltext_size &&
               (pltext.template index<ndebug>(p) == u8' ' || pltext.template index<ndebug>(p) == u8'\t')) {
            ++p;
        }

        if (property == ::pltxt2htm::container::U8StringView{u8"color"}) {
            if (color.empty() == false) {
                return ::pltxt2htm::container::nullopt;
            }
            auto const value_start = p;
            auto opt_value_end =
                ::pltxt2htm::details::try_parse_color_value<ndebug>(pltext.template subview<ndebug>(value_start));
            if (opt_value_end.has_value() == false) {
                return ::pltxt2htm::container::nullopt;
            }
            auto const value_end = opt_value_end.template value<ndebug>() + value_start;
            p = value_end;
            auto opt_delimiter_pos = ::pltxt2htm::details::try_parse_span_style_property_suffix<ndebug>(
                pltext.template subview<ndebug>(p), quote);
            if (opt_delimiter_pos.has_value() == false) {
                return ::pltxt2htm::container::nullopt;
            }
            p += opt_delimiter_pos.template value<ndebug>();
            color = ::fast_io::u8string{
                ::pltxt2htm::container::U8StringView{pltext.data() + value_start, value_end - value_start}};
        }
        else if (property == ::pltxt2htm::container::U8StringView{u8"font-size"}) {
            if (font_size.has_value()) {
                return ::pltxt2htm::container::nullopt;
            }
            auto opt_value =
                ::pltxt2htm::details::try_parse_font_size_value<ndebug>(pltext.template subview<ndebug>(p));
            if (opt_value.has_value() == false) {
                return ::pltxt2htm::container::nullopt;
            }
            auto const value = opt_value.template value<ndebug>();
            p += value.end;
            auto opt_delimiter_pos = ::pltxt2htm::details::try_parse_span_style_property_suffix<ndebug>(
                pltext.template subview<ndebug>(p), quote);
            if (opt_delimiter_pos.has_value() == false) {
                return ::pltxt2htm::container::nullopt;
            }
            p += opt_delimiter_pos.template value<ndebug>();
            font_size = value.value;
        }
        else if (property == ::pltxt2htm::container::U8StringView{u8"vertical-align"}) {
            if (vertical_align.has_value()) {
                return ::pltxt2htm::container::nullopt;
            }
            auto opt_value =
                ::pltxt2htm::details::try_parse_vertical_align_value<ndebug>(pltext.template subview<ndebug>(p));
            if (opt_value.has_value() == false) {
                return ::pltxt2htm::container::nullopt;
            }
            auto const value = opt_value.template value<ndebug>();
            p += value.end;
            auto opt_delimiter_pos = ::pltxt2htm::details::try_parse_span_style_property_suffix<ndebug>(
                pltext.template subview<ndebug>(p), quote);
            if (opt_delimiter_pos.has_value() == false) {
                return ::pltxt2htm::container::nullopt;
            }
            p += opt_delimiter_pos.template value<ndebug>();
            vertical_align = value.value;
        }
        else {
            return ::pltxt2htm::container::nullopt;
        }

        if (p >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        if (pltext.template index<ndebug>(p) == u8';') {
            ++p;
            continue;
        }
        if (pltext.template index<ndebug>(p) == quote) {
            return TryParseSpanStyleResult<ndebug>{.end = p + 1,
                                                   .color = ::std::move(color),
                                                   .font_size = ::std::move(font_size),
                                                   .vertical_align = ::std::move(vertical_align)};
        }
        return ::pltxt2htm::container::nullopt;
    }
    return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_span_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseSpanTagResult<ndebug>> {
    ::std::size_t const pltext_size{pltext.size()};
    // match "pan" prefix (case-insensitive)
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"pan"}>(pltext) ==
        false) {
        return ::pltxt2htm::container::nullopt;
    }

    ::std::size_t pos{4}; // skip past "span" (the 's' was consumed by the trie dispatch)
    bool found_style{false};
    ::fast_io::u8string color{};
    ::pltxt2htm::container::Optional<::pltxt2htm::ValueWithUnit<double>> font_size{::pltxt2htm::container::nullopt};
    ::pltxt2htm::container::Optional<::pltxt2htm::VerticalAlignValue<ndebug>> vertical_align{
        ::pltxt2htm::container::nullopt};

    while (pos < pltext_size) {
        // skip whitespace
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        if (pltext.template index<ndebug>(pos) == u8'>') {
            break;
        }

        // parse attribute name
        ::std::size_t const attr_start = pos;
        while (pos < pltext_size && pltext.template index<ndebug>(pos) != u8'=' &&
               pltext.template index<ndebug>(pos) != u8'>' && pltext.template index<ndebug>(pos) != u8' ' &&
               pltext.template index<ndebug>(pos) != u8'\t' && pltext.template index<ndebug>(pos) != u8'/') {
            ++pos;
        }
        ::pltxt2htm::container::U8StringView const attr_name{pltext.data() + attr_start, pos - attr_start};

        // skip whitespace before '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'=') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }

        // parse quoted attribute value
        char8_t const quote{pltext.template index<ndebug>(pos)};
        if (quote != u8'"' && quote != u8'\'') {
            return ::pltxt2htm::container::nullopt;
        }
        // only lowercase "style" attribute is allowed
        if (attr_name != ::pltxt2htm::container::U8StringView{u8"style"}) {
            return ::pltxt2htm::container::nullopt;
        }
        found_style = true;
        ++pos;
        auto opt_style =
            ::pltxt2htm::details::try_parse_span_style<ndebug>(pltext.template subview<ndebug>(pos), quote);
        if (opt_style.has_value() == false) {
            return ::pltxt2htm::container::nullopt;
        }
        auto& style = opt_style.template value<ndebug>();
        color = ::std::move(style.color);
        font_size = ::std::move(style.font_size);
        vertical_align = ::std::move(style.vertical_align);
        pos += style.end;
    }

    if (found_style == false || (color.empty() && !font_size.has_value() && !vertical_align.has_value())) {
        return ::pltxt2htm::container::nullopt;
    }
    if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'>') {
        return ::pltxt2htm::container::nullopt;
    }
    return TryParseSpanTagResult{.tag_len = pos + 1,
                                 .color = ::std::move(color),
                                 .font_size = ::std::move(font_size),
                                 .vertical_align = ::std::move(vertical_align)};
}

/**
 * @brief Result of parsing the CSS declarations inside a mark style attribute value.
 */
template<::pltxt2htm::Contracts ndebug>
struct TryParseMarkStyleResult {
    ::std::size_t end; ///< Byte offset just past the closing quote, relative to the input subview.
    ::fast_io::u8string background_color; ///< Extracted background-color value.
};

/**
 * @brief Parse the CSS declarations inside a mark style attribute value.
 * @details Only the lowercase "background-color" CSS property is accepted; its value must
 *          match the strict color grammar of try_parse_color_value. Any other property,
 *          a duplicate "background-color", or an empty value causes parse failure.
 * @param[in] pltext The input text view starting at the first style declaration character.
 * @param[in] quote The quote character that terminates the style value.
 * @return The byte offset just past the closing quote and the extracted color on success;
 *         nullopt on failure.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_mark_style(::pltxt2htm::container::U8StringView pltext, char8_t const quote) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMarkStyleResult<ndebug>> {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t p{};
    ::fast_io::u8string background_color{};

    while (p < pltext_size) {
        while (p < pltext_size &&
               (pltext.template index<ndebug>(p) == u8' ' || pltext.template index<ndebug>(p) == u8'\t')) {
            ++p;
        }
        if (p >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        auto const chr{pltext.template index<ndebug>(p)};
        if (chr == quote) {
            if (background_color.empty()) {
                return ::pltxt2htm::container::nullopt;
            }
            return TryParseMarkStyleResult<ndebug>{.end = p + 1, .background_color = ::std::move(background_color)};
        }
        if (chr == u8';') {
            ++p;
            continue;
        }

        // parse the property name up to ':'
        auto const property_start{p};
        auto property_end{p};
        while (p < pltext_size) {
            auto const property_chr{pltext.template index<ndebug>(p)};
            if (property_chr == u8':') {
                break;
            }
            if (property_chr == u8';' || property_chr == quote) {
                return ::pltxt2htm::container::nullopt;
            }
            if (property_chr != u8' ' && property_chr != u8'\t') {
                property_end = p + 1;
            }
            ++p;
        }
        if (p >= pltext_size || property_end == property_start) {
            return ::pltxt2htm::container::nullopt;
        }
        auto const property{
            ::pltxt2htm::container::U8StringView{pltext.data() + property_start, property_end - property_start}};
        ++p;
        while (p < pltext_size &&
               (pltext.template index<ndebug>(p) == u8' ' || pltext.template index<ndebug>(p) == u8'\t')) {
            ++p;
        }

        if (property != ::pltxt2htm::container::U8StringView{u8"background-color"}) {
            return ::pltxt2htm::container::nullopt;
        }
        if (background_color.empty() == false) {
            return ::pltxt2htm::container::nullopt;
        }
        auto const value_start{p};
        auto opt_value_end =
            ::pltxt2htm::details::try_parse_color_value<ndebug>(pltext.template subview<ndebug>(value_start));
        if (opt_value_end.has_value() == false) {
            return ::pltxt2htm::container::nullopt;
        }
        auto const value_end = opt_value_end.template value<ndebug>() + value_start;
        p = value_end;
        auto opt_delimiter_pos = ::pltxt2htm::details::try_parse_span_style_property_suffix<ndebug>(
            pltext.template subview<ndebug>(p), quote);
        if (opt_delimiter_pos.has_value() == false) {
            return ::pltxt2htm::container::nullopt;
        }
        p += opt_delimiter_pos.template value<ndebug>();
        background_color = ::fast_io::u8string{
            ::pltxt2htm::container::U8StringView{pltext.data() + value_start, value_end - value_start}};

        if (p >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        if (pltext.template index<ndebug>(p) == u8';') {
            ++p;
            continue;
        }
        if (pltext.template index<ndebug>(p) == quote) {
            return TryParseMarkStyleResult<ndebug>{.end = p + 1, .background_color = ::std::move(background_color)};
        }
        return ::pltxt2htm::container::nullopt;
    }
    return ::pltxt2htm::container::nullopt;
}

/**
 * @brief Return type of try_parse_mark_tag: tag length and effective background-color.
 */
struct TryParseMarkTagResult {
    ::std::size_t tag_len; ///< Length of the matched tag.
    ::fast_io::u8string background_color; ///< Effective background-color; the standard highlight color
                                          ///< when the tag has no style attribute.
};

/**
 * @brief Parse &lt;mark&gt; or &lt;mark style="background-color:V"&gt; and reject any other attributes.
 * @tparam ndebug When set to Contracts::ignore, runtime assertions are disabled for performance.
 * @param[in] pltext Input text starting at "ark ..." (after "<m").
 * @return Parsed tag result with the effective background-color on success; nullopt on failure.
 * @note The bare &lt;mark&gt; form is accepted and carries the default background-color.
 *       If a "style" attribute is present, only the
 *       lowercase "background-color" CSS property is accepted; any other attribute or CSS
 *       property causes parse failure.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_mark_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMarkTagResult> {
    ::std::size_t const pltext_size{pltext.size()};
    // match "ark" prefix (case-insensitive)
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"ark"}>(pltext) ==
        false) {
        return ::pltxt2htm::container::nullopt;
    }

    ::std::size_t pos{3}; // skip past "ark" (the 'm' was consumed by the trie dispatch)
    bool found_style{false};
    ::fast_io::u8string background_color{};

    while (pos < pltext_size) {
        // skip whitespace
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        if (pltext.template index<ndebug>(pos) == u8'>') {
            if (found_style == false) {
                return TryParseMarkTagResult{
                    .tag_len = pos + 1,
                    .background_color = ::fast_io::u8string{::pltxt2htm::HtmlMark<ndebug>::default_background_color}};
            }
            break;
        }
        if (found_style) {
            return ::pltxt2htm::container::nullopt;
        }

        // parse attribute name
        ::std::size_t const attr_start{pos};
        while (pos < pltext_size && pltext.template index<ndebug>(pos) != u8'=' &&
               pltext.template index<ndebug>(pos) != u8'>' && pltext.template index<ndebug>(pos) != u8' ' &&
               pltext.template index<ndebug>(pos) != u8'\t' && pltext.template index<ndebug>(pos) != u8'/') {
            ++pos;
        }
        ::pltxt2htm::container::U8StringView const attr_name{pltext.data() + attr_start, pos - attr_start};

        // skip whitespace before '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'=') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }

        // parse quoted attribute value
        char8_t const quote{pltext.template index<ndebug>(pos)};
        if (quote != u8'"' && quote != u8'\'') {
            return ::pltxt2htm::container::nullopt;
        }
        // only lowercase "style" attribute is allowed
        if (attr_name != ::pltxt2htm::container::U8StringView{u8"style"}) {
            return ::pltxt2htm::container::nullopt;
        }
        found_style = true;
        ++pos;
        auto opt_style =
            ::pltxt2htm::details::try_parse_mark_style<ndebug>(pltext.template subview<ndebug>(pos), quote);
        if (opt_style.has_value() == false) {
            return ::pltxt2htm::container::nullopt;
        }
        auto& style = opt_style.template value<ndebug>();
        background_color = ::std::move(style.background_color);
        pos += style.end;
    }

    if (found_style == false || background_color.empty()) {
        return ::pltxt2htm::container::nullopt;
    }
    if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'>') {
        return ::pltxt2htm::container::nullopt;
    }
    return TryParseMarkTagResult{.tag_len = pos + 1, .background_color = ::std::move(background_color)};
}

/**
 * @brief Parse &lt;mark=Xxx&gt; (TMP rich text) with a strict color value grammar.
 * @tparam ndebug When set to Contracts::ignore, runtime assertions are disabled for performance.
 * @param[in] pltext Input text starting at "ark=Xxx>..." (after "<m").
 * @return Parsed tag result with the background color on success; nullopt on failure.
 * @note The value is validated with the same grammar as &lt;color=Xxx&gt; (ascii-alpha
 *       named colors or #hex 3/4/6/8). Any other value, trailing attribute, missing
 *       closing bracket, or malformed form causes parse failure so the whole &lt;...&gt;
 *       degrades to escaped literal text.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_mark_equal_sign_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMarkTagResult> {
    // match "ark" prefix (case-insensitive)
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"ark"}>(pltext) ==
        false) {
        return ::pltxt2htm::container::nullopt;
    }
    if (pltext.size() <= 3 || pltext.template index<ndebug>(3) != u8'=') {
        return ::pltxt2htm::container::nullopt;
    }
    constexpr ::std::size_t value_start{4};
    auto opt_value_end =
        ::pltxt2htm::details::try_parse_color_value<ndebug>(pltext.template subview<ndebug>(value_start));
    if (opt_value_end.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const value_end = opt_value_end.template value<ndebug>() + value_start;
    auto opt_close =
        ::pltxt2htm::details::try_parse_equal_sign_tag_suffix<ndebug>(pltext.template subview<ndebug>(value_end));
    if (opt_close.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const close_rel = opt_close.template value<ndebug>();
    return TryParseMarkTagResult{
        .tag_len = value_end + close_rel + 1,
        .background_color = ::fast_io::u8string{pltext.template subview<ndebug>(value_start, value_end - value_start)}};
}

/**
 * @brief Result of parsing a <code class="language-..."> tag.
 */
struct TryParseCodeTagResult {
    ::std::size_t tag_len; ///< Length of the matched tag.
    ::pltxt2htm::container::Optional<::fast_io::u8string> language;
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
constexpr auto try_parse_code_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseCodeTagResult> {
    ::std::size_t const pltext_size{pltext.size()};
    // match "ode" prefix (case-insensitive)
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"ode"}>(pltext) ==
        false) {
        return ::pltxt2htm::container::nullopt;
    }

    ::std::size_t pos{3}; // skip past "ode" (the 'c' was consumed by the trie dispatch)
    bool found_class{false};
    ::pltxt2htm::container::Optional<::fast_io::u8string> language{::pltxt2htm::container::nullopt};

    while (pos < pltext_size) {
        // skip whitespace
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        if (pltext.template index<ndebug>(pos) == u8'>') {
            break;
        }

        // parse attribute name
        ::std::size_t const attr_start = pos;
        while (pos < pltext_size && pltext.template index<ndebug>(pos) != u8'=' &&
               pltext.template index<ndebug>(pos) != u8'>' && pltext.template index<ndebug>(pos) != u8' ' &&
               pltext.template index<ndebug>(pos) != u8'\t' && pltext.template index<ndebug>(pos) != u8'/') {
            ++pos;
        }
        ::pltxt2htm::container::U8StringView const attr_name{pltext.data() + attr_start, pos - attr_start};

        // skip whitespace before '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'=') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }

        // parse quoted attribute value
        char8_t const quote{pltext.template index<ndebug>(pos)};
        if (quote != u8'"' && quote != u8'\'') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos; // skip opening quote
        ::std::size_t const val_start = pos;
        bool language_suffix_is_safe{true};
        while (pos < pltext_size && pltext.template index<ndebug>(pos) != quote) {
            if (attr_name == ::pltxt2htm::container::U8StringView{u8"class"} && pos - val_start >= 9 &&
                ::pltxt2htm::details::is_code_language_suffix_char(pltext.template index<ndebug>(pos)) == false) {
                language_suffix_is_safe = false;
            }
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        ::pltxt2htm::container::U8StringView const attr_val{pltext.data() + val_start, pos - val_start};
        ++pos; // skip closing quote

        // only lowercase "class" attribute is allowed
        if (attr_name == ::pltxt2htm::container::U8StringView{u8"class"}) {
            if (found_class) {
                return ::pltxt2htm::container::nullopt; // duplicate class
            }
            if (attr_val.empty()) {
                return ::pltxt2htm::container::nullopt;
            }
            // value must start with "language-" and have at least one char after
            if (attr_val.size() < 10 || attr_val.template index<ndebug>(0) != u8'l' ||
                attr_val.template index<ndebug>(1) != u8'a' || attr_val.template index<ndebug>(2) != u8'n' ||
                attr_val.template index<ndebug>(3) != u8'g' || attr_val.template index<ndebug>(4) != u8'u' ||
                attr_val.template index<ndebug>(5) != u8'a' || attr_val.template index<ndebug>(6) != u8'g' ||
                attr_val.template index<ndebug>(7) != u8'e' || attr_val.template index<ndebug>(8) != u8'-' ||
                language_suffix_is_safe == false) {
                return ::pltxt2htm::container::nullopt;
            }
            language = ::fast_io::u8string{attr_val};
            found_class = true;
        }
        else {
            return ::pltxt2htm::container::nullopt; // unknown attribute
        }
    }

    // bare tag with no attributes is accepted
    if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'>') {
        return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_self_closing_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    ::std::size_t const pltext_size{pltext.size()};
    for (::std::size_t forward_index{}; forward_index < pltext_size; ++forward_index) {
        char8_t const forward_chr{pltext.template index<ndebug>(forward_index)};
        if (forward_chr == u8'>') {
            return forward_index + 1;
        }
        if (forward_chr == u8'/' && forward_index + 1 < pltext_size &&
            pltext.template index<ndebug>(forward_index + 1) == u8'>') {
            return forward_index + 2;
        }
        if (forward_chr != u8' ' && forward_chr != u8'\t') {
            return ::pltxt2htm::container::nullopt;
        }
    }
    return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_self_closing_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    ::std::size_t const pltext_size{pltext.size()};
    constexpr ::std::size_t tag_name_size{tag_name.size()};
    if (::pltxt2htm::details::is_prefix_match<ndebug, tag_name>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }

    for (::std::size_t forward_index{tag_name_size}; forward_index < pltext_size; ++forward_index) {
        char8_t const forward_chr{pltext.template index<ndebug>(forward_index)};
        if (forward_chr == u8'>') {
            return forward_index + 1;
        }
        if (forward_chr == u8'/' && forward_index + 1 < pltext_size &&
            pltext.template index<ndebug>(forward_index + 1) == u8'>') {
            return forward_index + 2;
        }
        if (forward_chr != u8' ' && forward_chr != u8'\t') {
            return ::pltxt2htm::container::nullopt;
        }
    }
    return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_col_tag(::pltxt2htm::container::U8StringView pltext,
                                 ::pltxt2htm::NodeKind const nested_tag_type) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    auto opt_tag_len =
        ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, ::pltxt2htm::details::U8LiteralString{u8"ol"}>(pltext);
    if (opt_tag_len.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    if (nested_tag_type != ::pltxt2htm::NodeKind::table_colgroup) {
        return ::pltxt2htm::container::nullopt;
    }
    return opt_tag_len;
}

/**
 * @brief Parse &lt;input type=&quot;checkbox&quot; disabled [checked]&gt; tag.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text to parse, starting at the opening `<`.
 * @return Total tag length (from the `<`) and checked state if matched; otherwise nullopt.
 */
struct TryParseInputCheckboxTagResult {
    ::std::size_t tag_len;
    bool checked;
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_input_checkbox_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseInputCheckboxTagResult> {
    ::std::size_t const pltext_size{pltext.size()};
    // match "<input" (case-insensitive)
    constexpr auto input_tag_prefix = ::pltxt2htm::details::U8LiteralString{u8"<input"};
    if (::pltxt2htm::details::is_prefix_match<ndebug, input_tag_prefix>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    ::std::size_t pos{input_tag_prefix.size()};
    bool found_type_checkbox{false};
    bool found_disabled{false};
    bool checked{false};

    while (pos < pltext_size) {
        // skip whitespace
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        // end of tag
        if (pltext.template index<ndebug>(pos) == u8'>') {
            break;
        }
        if (pltext.template index<ndebug>(pos) == u8'/' && pos + 1 < pltext_size &&
            pltext.template index<ndebug>(pos + 1) == u8'>') {
            break;
        }

        // parse attribute name
        ::std::size_t const attr_start = pos;
        while (pos < pltext_size && pltext.template index<ndebug>(pos) != u8'=' &&
               pltext.template index<ndebug>(pos) != u8'>' && pltext.template index<ndebug>(pos) != u8' ' &&
               pltext.template index<ndebug>(pos) != u8'\t' && pltext.template index<ndebug>(pos) != u8'/') {
            ++pos;
        }
        ::pltxt2htm::container::U8StringView const attr_name{pltext.data() + attr_start, pos - attr_start};

        // boolean attribute without value: "disabled" or "checked"
        if (pos < pltext_size &&
            (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t' ||
             pltext.template index<ndebug>(pos) == u8'>' || pltext.template index<ndebug>(pos) == u8'/')) {
            if (attr_name == ::pltxt2htm::container::U8StringView{u8"disabled"}) {
                found_disabled = true;
                continue;
            }
            if (attr_name == ::pltxt2htm::container::U8StringView{u8"checked"}) {
                checked = true;
                continue;
            }
            return ::pltxt2htm::container::nullopt;
        }

        // attribute with '=' value: "type=..."
        if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'=') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }

        // parse quoted attribute value
        char8_t const quote{pltext.template index<ndebug>(pos)};
        if (quote != u8'"' && quote != u8'\'') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos; // skip opening quote
        ::std::size_t const val_start = pos;
        while (pos < pltext_size && pltext.template index<ndebug>(pos) != quote) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        ::pltxt2htm::container::U8StringView const attr_val{pltext.data() + val_start, pos - val_start};
        ++pos; // skip closing quote

        // only "type=checkbox" is allowed
        if (attr_name != ::pltxt2htm::container::U8StringView{u8"type"}) {
            return ::pltxt2htm::container::nullopt;
        }
        if (attr_val != ::pltxt2htm::container::U8StringView{u8"checkbox"}) {
            return ::pltxt2htm::container::nullopt;
        }
        found_type_checkbox = true;
    }

    if (found_type_checkbox == false || !found_disabled) {
        return ::pltxt2htm::container::nullopt;
    }
    if (pos >= pltext_size) {
        return ::pltxt2htm::container::nullopt;
    }
    // skip '>' or '/>'
    if (pltext.template index<ndebug>(pos) == u8'/' && pos + 1 < pltext_size) {
        ++pos;
    }
    if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'>') {
        return ::pltxt2htm::container::nullopt;
    }
    return TryParseInputCheckboxTagResult{.tag_len = pos + 1, .checked = checked};
}

/**
 * @brief Result of parsing an `<ol>` tag.
 */
struct TryParseOlTagResult {
    ::std::size_t tag_len;
    ::std::size_t start;
};

/**
 * @brief Parse `<ol [start="N"]>` tag.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext Input text starting at the opening `<`.
 * @return Tag length (from the `<`) and the start value (default 1) if matched; otherwise nullopt.
 * @note Only the lowercase `start` attribute is supported; any other attribute makes the
 *       whole tag invalid so it falls back to literal text.  Attribute order is flexible,
 *       and both `>` and `/>` are accepted as the closing delimiter.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_ol_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseOlTagResult> {
    ::std::size_t const pltext_size{pltext.size()};
    // match "<ol" (case-insensitive)
    constexpr auto ol_tag_prefix = ::pltxt2htm::details::U8LiteralString{u8"<ol"};
    if (::pltxt2htm::details::is_prefix_match<ndebug, ol_tag_prefix>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    ::std::size_t pos{ol_tag_prefix.size()};
    ::std::size_t start{1};

    while (pos < pltext_size) {
        // skip whitespace
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        // end of tag
        if (pltext.template index<ndebug>(pos) == u8'>') {
            return TryParseOlTagResult{.tag_len = pos + 1, .start = start};
        }
        if (pltext.template index<ndebug>(pos) == u8'/' && pos + 1 < pltext_size &&
            pltext.template index<ndebug>(pos + 1) == u8'>') {
            return TryParseOlTagResult{.tag_len = pos + 2, .start = start};
        }

        // parse attribute name
        ::std::size_t const attr_start = pos;
        while (pos < pltext_size && pltext.template index<ndebug>(pos) != u8'=' &&
               pltext.template index<ndebug>(pos) != u8'>' && pltext.template index<ndebug>(pos) != u8' ' &&
               pltext.template index<ndebug>(pos) != u8'\t' && pltext.template index<ndebug>(pos) != u8'/') {
            ++pos;
        }
        ::pltxt2htm::container::U8StringView const attr_name{pltext.data() + attr_start, pos - attr_start};

        // only the `start` attribute is supported (lowercase), and it must have a value
        if (attr_name != ::pltxt2htm::container::U8StringView{u8"start"}) {
            return ::pltxt2htm::container::nullopt;
        }
        if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'=') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }

        // parse quoted attribute value
        char8_t const quote{pltext.template index<ndebug>(pos)};
        if (quote != u8'"' && quote != u8'\'') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos; // skip opening quote
        ::std::size_t const val_start = pos;
        while (pos < pltext_size && pltext.template index<ndebug>(pos) != quote) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        ::pltxt2htm::container::U8StringView const attr_val{pltext.data() + val_start, pos - val_start};
        ++pos; // skip closing quote

        auto opt_value = ::pltxt2htm::details::try_parse_size_t_decimal_value<ndebug>(attr_val);
        if (opt_value.has_value() == false) {
            return ::pltxt2htm::container::nullopt;
        }
        auto const [value_end, value] = opt_value.template value<ndebug>();
        if (value_end != attr_val.size()) {
            return ::pltxt2htm::container::nullopt;
        }
        start = value;
    }
    return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_img_tag(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseImgTagResult> {
    ::std::size_t const pltext_size{pltext.size()};
    // match "mg" (case-insensitive)
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"mg"}>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }

    ::std::size_t pos{2};
    bool found_src{false};
    bool found_alt{false};
    ::fast_io::u8string src{};
    ::fast_io::u8string alt{};

    while (pos < pltext_size) {
        // skip whitespace
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        // end of tag
        if (pltext.template index<ndebug>(pos) == u8'>') {
            break;
        }
        if (pltext.template index<ndebug>(pos) == u8'/' && pos + 1 < pltext_size &&
            pltext.template index<ndebug>(pos + 1) == u8'>') {
            break;
        }

        // parse attribute name
        ::std::size_t const attr_start{pos};
        while (pos < pltext_size && pltext.template index<ndebug>(pos) != u8'=' &&
               pltext.template index<ndebug>(pos) != u8'>' && pltext.template index<ndebug>(pos) != u8' ' &&
               pltext.template index<ndebug>(pos) != u8'\t' && pltext.template index<ndebug>(pos) != u8'/') {
            ++pos;
        }
        ::pltxt2htm::container::U8StringView const attr_name{pltext.data() + attr_start, pos - attr_start};

        // skip whitespace before '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'=') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos; // skip '='

        // skip whitespace after '='
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }

        // parse quoted attribute value
        char8_t const quote{pltext.template index<ndebug>(pos)};
        if (quote != u8'"' && quote != u8'\'') {
            return ::pltxt2htm::container::nullopt;
        }
        ++pos; // skip opening quote
        ::std::size_t const val_start{pos};
        while (pos < pltext_size && pltext.template index<ndebug>(pos) != quote) {
            ++pos;
        }
        if (pos >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        ::pltxt2htm::container::U8StringView const attr_val{pltext.data() + val_start, pos - val_start};
        ++pos; // skip closing quote

        // recognize attribute
        if (attr_name == ::pltxt2htm::container::U8StringView{u8"src"}) {
            if (found_src) {
                return ::pltxt2htm::container::nullopt; // duplicate src
            }
            src = ::pltxt2htm::details::decode_character_references<ndebug>(attr_val);
            found_src = true;
        }
        else if (attr_name == ::pltxt2htm::container::U8StringView{u8"alt"}) {
            if (found_alt) {
                return ::pltxt2htm::container::nullopt; // duplicate alt
            }
            alt = ::pltxt2htm::details::decode_character_references<ndebug>(attr_val);
            found_alt = true;
        }
        else {
            return ::pltxt2htm::container::nullopt; // unknown attribute
        }
    }

    if (found_src == false || !found_alt) {
        return ::pltxt2htm::container::nullopt;
    }
    if (pos >= pltext_size) {
        return ::pltxt2htm::container::nullopt;
    }
    // skip '>' or '/>'
    if (pltext.template index<ndebug>(pos) == u8'/' && pos + 1 < pltext_size) {
        ++pos;
    }
    if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'>') {
        return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_pltext_line_break(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    if (pltext.empty()) {
        return ::pltxt2htm::container::nullopt;
    }
    if (pltext.template index<ndebug>(0) == u8'\n') {
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
constexpr auto try_parse_md_atx_heading(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMdAtxHeadingResult> {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t start_index{};
    // spaces before the first #
    while (true) {
        if (start_index >= pltext_size) {
            return ::pltxt2htm::container::nullopt;
        }
        if (pltext.template index<ndebug>(start_index) != u8' ' &&
            pltext.template index<ndebug>(start_index) != u8'\t') {
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
                return TryParseMdAtxHeadingResult{
                    .start_index = start_index,
                    .sublength = 0,
                    .advance_count = start_index,
                    .md_atx_heading_type = static_cast<::pltxt2htm::NodeKind>(md_atx_heading_type)};
            }
            return ::pltxt2htm::container::nullopt;
        }
        if (pltext.template index<ndebug>(start_index) != u8'#') {
            break;
        }
        ++start_index;
        ++md_atx_heading_type;
    }
    if (md_atx_heading_type < static_cast<::std::size_t>(::pltxt2htm::NodeKind::md_atx_h1) ||
        static_cast<::std::size_t>(::pltxt2htm::NodeKind::md_atx_h6) < md_atx_heading_type ||
        (pltext.template index<ndebug>(start_index) != u8' ' && pltext.template index<ndebug>(start_index) != u8'\t')) {
        // invalid atx header
        return ::pltxt2htm::container::nullopt;
    }
    ++start_index;
    // spaces after the last #
    while (true) {
        if (start_index >= pltext_size) {
            // https://spec.commonmark.org/0.31.2/#example-79
            return TryParseMdAtxHeadingResult{
                .start_index = start_index,
                .sublength = 0,
                .advance_count = start_index,
                .md_atx_heading_type = static_cast<::pltxt2htm::NodeKind>(md_atx_heading_type)};
        }
        if (pltext.template index<ndebug>(start_index) != u8' ' &&
            pltext.template index<ndebug>(start_index) != u8'\t') {
            break;
        }
        ++start_index;
    }
    // end of the atx header
    ::std::size_t end_index{start_index};
    ::std::size_t extra_length{};
    for (; end_index < pltext_size; ++end_index) {
        if (auto opt_line_break =
                ::pltxt2htm::details::try_parse_pltext_line_break<ndebug>(pltext.template subview<ndebug>(end_index));
            opt_line_break.has_value()) {
            extra_length = opt_line_break.template value<ndebug>();
            break;
        }
    }
    return TryParseMdAtxHeadingResult{.start_index = start_index,
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
constexpr auto try_parse_md_thematic_break(::pltxt2htm::container::U8StringView text) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    ::std::size_t const text_size{text.size()};
    if (text_size < 3) {
        return ::pltxt2htm::container::nullopt;
    }
    ::pltxt2htm::details::ThematicBreakType thematic_break_type{::pltxt2htm::details::ThematicBreakType::none};
    ::std::size_t thematic_break_count{};
    ::std::size_t i{};
    for (; i < text_size; ++i) {
        char8_t const chr{text.template index<ndebug>(i)};
        if (chr == u8' ' || chr == u8'\t') {
            continue;
        }
        if (chr == u8'*') {
            if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::asterisk) {
                ++thematic_break_count;
                continue;
            }
            if (thematic_break_type != ::pltxt2htm::details::ThematicBreakType::none) {
                return ::pltxt2htm::container::nullopt;
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
                return ::pltxt2htm::container::nullopt;
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
                return ::pltxt2htm::container::nullopt;
            }
            thematic_break_type = ::pltxt2htm::details::ThematicBreakType::underscore;
            ++thematic_break_count;
            continue;
        }
        if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::none) {
            return ::pltxt2htm::container::nullopt;
        }
        auto opt_line_break =
            ::pltxt2htm::details::try_parse_pltext_line_break<ndebug>(text.template subview<ndebug>(i));
        if (opt_line_break.has_value() == false) {
            return ::pltxt2htm::container::nullopt;
        }
        if (thematic_break_count < 3) {
            return ::pltxt2htm::container::nullopt;
        }
        return i + opt_line_break.template value<ndebug>();
    }
    if (thematic_break_type == ::pltxt2htm::details::ThematicBreakType::none) {
        return ::pltxt2htm::container::nullopt;
    }
    if (thematic_break_count < 3) {
        return ::pltxt2htm::container::nullopt;
    }
    return i;
}

template<::pltxt2htm::Contracts ndebug>
struct SimplyParsePLtextResult {
    ::std::size_t advance_count; ///< Number of characters consumed.
    ::pltxt2htm::Ast<ndebug> ast; ///< Parsed AST.
    bool found_end; ///< Whether `end_string` was actually matched (only meaningful when end_string is non-empty).
};

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
template<::pltxt2htm::Contracts ndebug, ::pltxt2htm::details::U8LiteralString end_string, bool process_md_escape = true>
[[nodiscard]]
constexpr auto simply_parse_pltext(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::details::SimplyParsePLtextResult<ndebug> {
    ::std::size_t const pltext_size{pltext.size()};
    ::pltxt2htm::Ast<ndebug> ast{};
    ::std::size_t current_index{};
    constexpr ::std::size_t end_size{end_string.size()};
    ::std::conditional_t<end_size == 0, bool const, bool> found_end{};

    while (current_index < pltext_size) {
        char8_t const chr{pltext.template index<ndebug>(current_index)};

        if constexpr (end_size != 0) {
            if (::pltxt2htm::details::is_prefix_match<ndebug, end_string>(
                    pltext.template subview<ndebug>(current_index))) {
                current_index += end_size;
                found_end = true;
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
            if (auto const opt_entity_len = ::pltxt2htm::details::try_append_character_reference<ndebug>(
                    pltext.template subview<ndebug>(current_index), ast);
                opt_entity_len.has_value()) {
                current_index += opt_entity_len.template value<ndebug>();
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
        if constexpr (process_md_escape) {
            if (auto opt_escape =
                    ::pltxt2htm::details::try_parse_md_escape<ndebug>(pltext.template subview<ndebug>(current_index));
                opt_escape.has_value()) {
                auto&& [node, advance_count] = opt_escape.template value<ndebug>();
                ast.push_back(::std::move(node));
                current_index += advance_count;
                continue;
            }
        }
        if (chr == u8'<') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
            ++current_index;
            continue;
        }
        auto const advance_count =
            ::pltxt2htm::details::parse_utf8_code_point<ndebug>(pltext.template subview<ndebug>(current_index), ast);
        current_index += advance_count;
        continue;
    }
    return {.advance_count = current_index, .ast = ::std::move(ast), .found_end = found_end};
}

template<::pltxt2htm::Contracts ndebug>
struct TryParseMdCodeFenceResult {
    ::pltxt2htm::PlTxtNode<ndebug> node; ///< Parsed code fence node.
    ::std::size_t advance_count; ///< Number of characters consumed.
};

/**
 * @brief Parse a block-level HTML <pre><code>...</code></pre> code block into a CodeFence node.
 *
 * This function attempts to parse the full `<pre><code>` block: a bare `<pre>` tag,
 * optionally followed by spaces/tabs and a `<code>` tag (bare or with a
 * `class="language-..."` attribute). The content up to the matching `</code></pre>`
 * is parsed as plain text and stored in a ::pltxt2htm::CodeFence node.
 *
 * @tparam ndebug When set to Contracts::ignore, runtime assertions are disabled for performance.
 * @param[in] pltext Input text starting at "<pre>".
 * @return The parsed CodeFence node and continuation index on success; nullopt on any deviation.
 * @note The `<pre>` tag must be bare (no attributes) and `<code>` must immediately
 *       follow it (allowing only spaces/tabs in between).
 */
template<::pltxt2htm::Contracts ndebug, bool process_md_escape = true>
[[nodiscard]]
constexpr auto try_parse_html_pre_code_block(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMdCodeFenceResult<ndebug>> {
    ::std::size_t const pltext_size{pltext.size()};
    // <pre> must be a bare tag (no attributes).
    auto opt_pre_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8"<pre">(pltext);
    if (opt_pre_tag_len.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    ::std::size_t pos{opt_pre_tag_len.template value<ndebug>() + 1};

    // allow spaces/tabs between <pre> and <code>
    while (pos < pltext_size &&
           (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
        ++pos;
    }
    if (pos + 1 >= pltext_size) {
        return ::pltxt2htm::container::nullopt;
    }
    if (pltext.template index<ndebug>(pos) != u8'<') {
        return ::pltxt2htm::container::nullopt;
    }
    char8_t const code_first_chr{pltext.template index<ndebug>(pos + 1)};
    if (code_first_chr != u8'c' && code_first_chr != u8'C') {
        return ::pltxt2htm::container::nullopt;
    }
    auto opt_code_tag = ::pltxt2htm::details::try_parse_code_tag<ndebug>(pltext.template subview<ndebug>(pos + 2));
    if (opt_code_tag.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto&& [code_tag_len, language] = opt_code_tag.template value<ndebug>();
    pos += 2 + code_tag_len;

    // parse content until the closing </code></pre>
    constexpr auto end_string = ::pltxt2htm::details::U8LiteralString{u8"</code></pre>"};
    auto&& [advance_count, ast, found_end] =
        ::pltxt2htm::details::simply_parse_pltext<ndebug, end_string, process_md_escape>(
            pltext.template subview<ndebug>(pos));
    if (found_end == false) {
        // No closing </code></pre> in the input: treat the whole construct as literal
        // text instead of an unterminated code block.
        return ::pltxt2htm::container::nullopt;
    }
    pos += advance_count;

    // <code class="language-..."> stores the full class value; CodeFence stores only the suffix
    // after the "language-" prefix (the backends prepend it again when rendering).
    ::pltxt2htm::container::Optional<::fast_io::u8string> opt_lang{::pltxt2htm::container::nullopt};
    if (language.has_value()) {
        auto const& full_language = language.template value<ndebug>();
        opt_lang = ::fast_io::u8string{full_language.data() + 9, full_language.data() + full_language.size()};
    }
    return TryParseMdCodeFenceResult<ndebug>{
        .node = ::pltxt2htm::CodeFence<ndebug>{::std::move(ast), ::std::move(opt_lang)}, .advance_count = pos};
}

[[nodiscard]]
constexpr bool is_allowed_in_language(char8_t const chr) noexcept {
    return (chr >= u8'a' && chr <= u8'z') || (chr >= u8'A' && chr <= u8'Z') || (chr >= u8'0' && chr <= u8'9') ||
           chr == u8'+' || chr == u8'#' || chr == u8'.' || chr == u8'_' || chr == u8'-';
}

/**
 * @brief Result of locating the closing fence line of a fenced code block.
 */
struct MdCodeFenceEndResult {
    ::std::size_t content_end; ///< Offset (relative to content) at which the code content ends.
    ::std::size_t consumed; ///< Total content bytes to consume, including the closing fence line.
};

/**
 * @brief Find the first valid closing fence line in fenced code block content.
 *
 * @details A valid closing fence is a line that starts with exactly three
 *          delimiter characters of the same kind as the opening fence, followed
 *          only by spaces/tabs and then a newline or the end of the content. The
 *          fence must be on its own line, i.e. it must be at the very start of
 *          the content (the opening fence's line ending was already consumed) or
 *          immediately after a newline. This project only supports fixed
 *          3-delimiter fences, so a longer delimiter run (4+) is NOT a valid
 *          closing fence.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @tparam is_backtick When true, uses backtick delimiters; otherwise uses tilde delimiters.
 * @param[in] content The code block content, starting right after the opening fence line.
 * @return A ::pltxt2htm::details::MdCodeFenceEndResult with `content_end` set to the offset
 *         (relative to `content`) at which the content ends (the leading `\n` of the closing
 *         fence line, or `0` when the closing fence is at the very start of `content`) and
 *         `consumed` set to the total number of bytes to consume, including the closing fence
 *         line (leading `\n` if any, the three delimiters and any trailing spaces/tabs, but not
 *         the trailing `\n`). Returns `::pltxt2htm::container::nullopt` if no valid closing fence exists.
 * @note The closing fence must be exactly three delimiters long and use the same
 *       delimiter character as the opening fence.
 * @note A line like "```text" (content after the fence on the same line) or "````"
 *       (more than three delimiters) is NOT a closing fence and is treated as code content.
 * @see https://spec.commonmark.org/0.31.2/#fenced-code-blocks
 */
template<::pltxt2htm::Contracts ndebug, bool is_backtick>
[[nodiscard]]
constexpr auto find_md_code_fence_end(::pltxt2htm::container::U8StringView content) noexcept
    -> ::pltxt2htm::container::Optional<::pltxt2htm::details::MdCodeFenceEndResult> {
    constexpr char8_t fence_char{is_backtick ? u8'`' : u8'~'};
    ::std::size_t const size{content.size()};
    ::std::size_t line_start{0};
    while (line_start < size) {
        // The closing fence must be exactly three delimiter characters: three fence
        // characters immediately followed by a non-delimiter (or the end of content).
        if (line_start + 3 <= size && content.template index<ndebug>(line_start) == fence_char &&
            content.template index<ndebug>(line_start + 1) == fence_char &&
            content.template index<ndebug>(line_start + 2) == fence_char &&
            (line_start + 3 == size || content.template index<ndebug>(line_start + 3) != fence_char)) {
            // The fence must be followed only by spaces/tabs and then a newline or EOF.
            ::std::size_t j{line_start + 3};
            while (j < size &&
                   (content.template index<ndebug>(j) == u8' ' || content.template index<ndebug>(j) == u8'\t')) {
                ++j;
            }
            if (j == size || content.template index<ndebug>(j) == u8'\n') {
                // Valid closing fence line. Content ends at the leading '\n' (or 0 when the
                // closing fence is at the very start of the content), and all bytes up to `j`
                // (the fence run and any trailing spaces/tabs) are consumed.
                return ::pltxt2htm::details::MdCodeFenceEndResult{.content_end = line_start == 0 ? 0 : line_start - 1,
                                                                  .consumed = j};
            }
        }
        // Move to the next line start.
        ::std::size_t next{line_start};
        while (next < size && content.template index<ndebug>(next) != u8'\n') {
            ++next;
        }
        if (next == size) {
            break;
        }
        line_start = next + 1;
    }
    return ::pltxt2htm::container::nullopt;
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
 * @note The content ends at the first occurrence of a valid closing fence on its own line
 *       (the fence may be followed only by spaces/tabs and then a newline or end of input).
 * @note Empty language identifiers are allowed and result in no language specification.
 * @see https://spec.commonmark.org/0.31.2/#fenced-code-blocks
 */
template<::pltxt2htm::Contracts ndebug, bool is_backtick>
[[nodiscard]]
constexpr auto try_parse_md_code_fence_(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMdCodeFenceResult<ndebug>> {
    if (pltext.size() < 7) {
        return ::pltxt2htm::container::nullopt;
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
        return ::pltxt2htm::container::nullopt;
    }

    ::fast_io::u8string lang{};
    ::std::size_t current_index{fence_size};
    ::std::size_t const pltext_size{pltext.size()};

    // Parsing language string

    // Skipping spaces before language string
    while (current_index != pltext_size && (pltext.template index<ndebug>(current_index) == u8' ' ||
                                            pltext.template index<ndebug>(current_index) == u8'\t')) {
        ++current_index;
    }
    // parsing language string
    for (; current_index != pltext_size; ++current_index) {
        auto const chr = pltext.template index<ndebug>(current_index);
        if (chr == u8'\n') {
            ++current_index;
            break;
        }
        if (chr == u8' ' || chr == u8'\t') {
            ++current_index;
            if (current_index == pltext_size) {
                // space/tab at end of input after language: not a valid code fence
                return ::pltxt2htm::container::nullopt;
            }
            while (current_index != pltext_size && pltext.template index<ndebug>(current_index) != u8'\n') {
                ++current_index;
            }
            if (current_index == pltext_size) {
                // content after space/tab without newline: not a valid code fence
                return ::pltxt2htm::container::nullopt;
            }
            ++current_index;
            break;
        }
        if constexpr (is_backtick) {
            if (::pltxt2htm::details::is_prefix_match<ndebug, fence>(pltext.template subview<ndebug>(current_index))) {
                return ::pltxt2htm::container::nullopt;
            }
        }
        if (::pltxt2htm::details::is_allowed_in_language(chr) == false) {
            lang.clear();
            while (current_index != pltext_size && pltext.template index<ndebug>(current_index) != u8'\n') {
                ++current_index;
            }
            if (current_index == pltext_size) {
                return ::pltxt2htm::container::nullopt;
            }
            ++current_index;
            break;
        }
        lang.push_back(chr);
    }

    // parsing context of code fence
    ::pltxt2htm::Ast<ndebug> ast{};
    ::pltxt2htm::container::U8StringView const content{pltext.template subview<ndebug>(current_index)};
    if (auto opt_fence_end = ::pltxt2htm::details::find_md_code_fence_end<ndebug, is_backtick>(content);
        opt_fence_end.has_value()) {
        auto&& [content_end, consumed] = opt_fence_end.template value<ndebug>();
        auto&& [_, ast_, found_end_] =
            ::pltxt2htm::details::simply_parse_pltext<ndebug, ::pltxt2htm::details::U8LiteralString<0>{}>(
                content.template subview<ndebug>(0, content_end));
        ast = ::std::move(ast_);
        current_index += consumed;
    }
    else {
        // No valid closing fence: content runs to the end of the input.
        auto&& [advance_count, ast_, found_end_] =
            ::pltxt2htm::details::simply_parse_pltext<ndebug, ::pltxt2htm::details::U8LiteralString<0>{}>(content);
        ast = ::std::move(ast_);
        current_index += advance_count;
    }

    ::pltxt2htm::container::Optional<::fast_io::u8string> opt_lang{::pltxt2htm::container::nullopt};
    if (lang.empty() == false) {
        opt_lang = ::std::move(lang);
    }
    return TryParseMdCodeFenceResult<ndebug>{
        .node = ::pltxt2htm::CodeFence<ndebug>{::std::move(ast), ::std::move(opt_lang)},
        .advance_count = current_index};
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
constexpr auto try_parse_md_code_fence(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMdCodeFenceResult<ndebug>> {
    if (auto opt_code_fence_backtick = ::pltxt2htm::details::try_parse_md_code_fence_<ndebug, true>(pltext);
        opt_code_fence_backtick.has_value()) {
        return opt_code_fence_backtick;
    }
#if 1
    return ::pltxt2htm::details::try_parse_md_code_fence_<ndebug, false>(pltext);
#else
    // Above code equals to below code
    if (auto opt_code_fence_tilde = ::pltxt2htm::details::try_parse_md_code_fence_<ndebug, false>(pltext);
        opt_code_fence_tilde.has_value()) {
        return opt_code_fence_tilde;
    }
    return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_md_inlines(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    ::std::size_t const pltext_size{pltext.size()};
    constexpr ::std::size_t embraced_size{embraced_chars.size()};
    if (::pltxt2htm::details::is_prefix_match<ndebug, embraced_chars>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }

    for (::std::size_t current_index{embraced_size}; current_index < pltext_size; ++current_index) {
        if (pltext.template index<ndebug>(current_index) == u8'\n') {
            return ::pltxt2htm::container::nullopt;
        }
        if (::pltxt2htm::details::is_prefix_match<ndebug, embraced_chars>(
                pltext.template subview<ndebug>(current_index))) {
            ::std::size_t result{current_index - embraced_size};
            if (result == 0) {
                return ::pltxt2htm::container::nullopt;
            }
            return result;
        }
    }
    return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_md_block_quotes(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMdBlockQuotesResult> {
    ::fast_io::u8string subpltext{};

    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t current_index{};
    for (; current_index < pltext_size; ++current_index) {
        ::std::size_t temp_index{current_index};
        while (pltext.template index<ndebug>(temp_index) == u8' ' ||
               pltext.template index<ndebug>(temp_index) == u8'\t') {
            ++temp_index;
            pltxt2htm_assert(temp_index <= pltext_size, u8"temp_index out of range");
            if (temp_index == pltext_size) { // equals to `temp_index >= pltext_size`
                return ::pltxt2htm::container::nullopt;
            }
        }
        if (pltext.template index<ndebug>(temp_index) != u8'>') {
            break;
        }
        current_index = temp_index + 1;
        while (current_index < pltext_size && (pltext.template index<ndebug>(current_index) == u8' ' ||
                                               pltext.template index<ndebug>(current_index) == u8'\t')) {
            ++current_index;
        }
        if (current_index == pltext_size) {
            break;
        }
        while (true) {
            subpltext.push_back(pltext.template index<ndebug>(current_index));
            ++current_index;
            pltxt2htm_assert(current_index <= pltext_size, u8"current_index exceeds pltext_size");
            if (current_index == pltext_size) {
                break;
            }
            if (pltext.template index<ndebug>(current_index) == u8'\n') {
                break;
            }
        }
        if (current_index == pltext_size) {
            break;
        }
        subpltext.push_back(u8'\n');
    }

    if (subpltext.empty()) {
        return ::pltxt2htm::container::nullopt;
    }
    if (subpltext.back_unchecked() == u8'\n') {
        subpltext.pop_back();
    }
    return TryParseMdBlockQuotesResult{.advance_count = current_index, .subpltext = ::std::move(subpltext)};
}

template<::pltxt2htm::Contracts ndebug>
struct TryParseMdCodeSpanResult {
    ::std::size_t content_size; ///< Length of the content (excluding both delimiters).
    ::pltxt2htm::Ast<ndebug> subast; ///< Parsed AST for the code span content.
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
 * @return The parsed result containing the content length and the code content AST, or nullopt if parsing fails.
 * @note The delimiter length determines the minimum number of consecutive backticks that can appear
 *       in the code content without prematurely ending the span.
 * @note The content is parsed as plain text and converted to appropriate AST nodes.
 * @note Code spans cannot contain newline characters - they must be single-line.
 * @note Empty code spans are NOT code spans; the function returns nullopt so the input stays literal text.
 * @note An opening delimiter without a matching closing delimiter is NOT a code span;
 *       the function returns nullopt so the input stays literal text.
 * @see https://spec.commonmark.org/0.31.2/#code-spans
 */
template<::pltxt2htm::Contracts ndebug, ::pltxt2htm::details::U8LiteralString embraced_string>
[[nodiscard]]
constexpr auto try_parse_md_code_span(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMdCodeSpanResult<ndebug>> {
    constexpr ::std::size_t embraced_size{embraced_string.size()};
    if (::pltxt2htm::details::is_prefix_match<ndebug, embraced_string>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }

    auto&& [advance_count, ast, found_end] = ::pltxt2htm::details::simply_parse_pltext<ndebug, embraced_string>(
        pltext.template subview<ndebug>(embraced_size));
    if (found_end == false) {
        // The closing delimiter is missing: `advance_count` only reaches the end of the
        // input, so treating this as a code span would silently drop the unclosed backticks.
        return ::pltxt2htm::container::nullopt;
    }
    ::std::size_t const content_size{advance_count - embraced_size};
    if (content_size == 0) {
        // Empty content: an empty code span is not meaningful, so it stays literal text.
        return ::pltxt2htm::container::nullopt;
    }

    return TryParseMdCodeSpanResult<ndebug>{.content_size = content_size, .subast = ::std::move(ast)};
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
constexpr auto try_parse_md_latex_block_dollar(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMdLatexResult<ndebug>> {
    constexpr auto double_dollar = ::pltxt2htm::details::U8LiteralString{u8"$$"};
    if (pltext.size() < 4 || ::pltxt2htm::details::is_prefix_match<ndebug, double_dollar>(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }

    auto const body = pltext.template subview<ndebug>(2);
    ::std::size_t const body_size{body.size()};
    ::pltxt2htm::Ast<ndebug> ast{};
    for (::std::size_t current_index{}; current_index < body_size;) {
        if (::pltxt2htm::details::is_prefix_match<ndebug, double_dollar>(
                body.template subview<ndebug>(current_index))) {
            return TryParseMdLatexResult<ndebug>{.advance_count = current_index + 4, .subast = ::std::move(ast)};
        }
        if (body.template index<ndebug>(current_index) == u8'\n') {
            ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::U8Char{u8'\n'}));
            ++current_index;
        }
        else {
            current_index +=
                ::pltxt2htm::details::parse_utf8_code_point<ndebug>(body.template subview<ndebug>(current_index), ast);
        }
    }
    return ::pltxt2htm::container::nullopt;
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
constexpr auto try_parse_md_latex_inline(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMdLatexResult<ndebug>> {
    if (pltext.size() < 3 || pltext.template index<ndebug>(0) != u8'$') {
        return ::pltxt2htm::container::nullopt;
    }

    auto const body = pltext.template subview<ndebug>(1);
    ::std::size_t const body_size{body.size()};
    ::pltxt2htm::Ast<ndebug> ast{};
    for (::std::size_t current_index{}; current_index < body_size;) {
        auto const chr = body.template index<ndebug>(current_index);
        if (chr == u8'\n') {
            return ::pltxt2htm::container::nullopt;
        }
        if (chr == u8'$') {
            if (current_index == 0) {
                return ::pltxt2htm::container::nullopt;
            }
            return TryParseMdLatexResult<ndebug>{.advance_count = current_index + 2, .subast = ::std::move(ast)};
        }
        current_index +=
            ::pltxt2htm::details::parse_utf8_code_point<ndebug>(body.template subview<ndebug>(current_index), ast);
    }
    return ::pltxt2htm::container::nullopt;
}

/**
 * @brief Check whether a parsed domain ends in an accepted top-level domain.
 */
[[nodiscard]]
constexpr bool has_allowed_url_tld(::pltxt2htm::container::U8StringView domain) noexcept {
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
 * @details `pltext` must start at the domain (the caller subviews past the scheme, if any);
 *          the returned index is relative to `pltext`.
 * @param[in] pltext Input text starting at the domain.
 * @return The relative index after the domain, or nullopt when a label or TLD is invalid.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_domain(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t current_index{};
    bool label_has_char{};
    bool label_ended_with_hyphen{};
    while (current_index < pltext_size) {
        auto const chr{pltext.template index<ndebug>(current_index)};
        if (::pltxt2htm::details::is_ascii_alpha(chr) || ::pltxt2htm::details::is_ascii_digit(chr)) {
            label_has_char = true;
            label_ended_with_hyphen = false;
        }
        else if (chr == u8'-') {
            if (label_has_char == false) {
                return ::pltxt2htm::container::nullopt;
            }
            label_ended_with_hyphen = true;
        }
        else if (chr == u8'.') {
            if (label_has_char == false || label_ended_with_hyphen) {
                return ::pltxt2htm::container::nullopt;
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
        return ::pltxt2htm::container::nullopt;
    }
    auto const domain = pltext.template subview<ndebug>(0, current_index);
    if (::pltxt2htm::details::has_allowed_url_tld(domain) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    return current_index;
}

/**
 * @brief Parse and validate a URL port and its following delimiter.
 * @details `pltext` must start at the port digits (the caller subviews past the `:`); the
 *          returned index is relative to `pltext`.
 * @param[in] pltext Input text starting at the port.
 * @return The relative index after the port, or nullopt when the port is invalid.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_port(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::uint_least32_t port{};
    ::std::size_t current_index{};
    ::std::size_t port_size{};
    while (current_index < pltext_size) {
        auto const chr{pltext.template index<ndebug>(current_index)};
        if (::pltxt2htm::details::is_ascii_digit(chr) == false) {
            break;
        }
        port = port * 10 + static_cast<::std::uint_least32_t>(chr - u8'0');
        ++current_index;
        ++port_size;
        if (port_size > 5) {
            return ::pltxt2htm::container::nullopt;
        }
    }
    if (port_size == 0 || port > 65535) {
        return ::pltxt2htm::container::nullopt;
    }
    if (current_index < pltext_size) {
        auto const next_chr{pltext.template index<ndebug>(current_index)};
        if (next_chr != u8'/' && next_chr != u8'?' && next_chr != u8'#') {
            return ::pltxt2htm::container::nullopt;
        }
    }
    return current_index;
}

/**
 * @brief Detect and return the end offset of `http://` or `https://` scheme.
 *
 * O(1) - does NOT scan for domains. Returns the scheme length (7 or 8) or nullopt.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text that may begin with a URL scheme.
 * @return 7 for `http://`, 8 for `https://`, or nullopt.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_scheme(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    if (::pltxt2htm::details::is_prefix_match<ndebug, u8"http">(pltext) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const after_http = pltext.template subview<ndebug>(4);
    if (::pltxt2htm::details::is_prefix_match<ndebug, u8"://">(after_http)) {
        return ::std::size_t{7};
    }
    if (::pltxt2htm::details::is_prefix_match<ndebug, u8"s://">(after_http)) {
        return ::std::size_t{8};
    }
    return ::pltxt2htm::container::nullopt;
}

/**
 * @brief Parse and validate the authority part (domain + port) of a URL.
 *
 * Does NOT detect the scheme - the caller must pass a view starting at the domain (e.g. a
 * subview past the scheme, or the whole candidate when no scheme is present). Supports
 * domain validation and optional port. Does NOT parse the path, query, or fragment - that
 * is the caller's responsibility.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text starting at the domain.
 * @return The relative index after the port (or after the domain if no port), or nullopt when
 *         domain/port validation fails.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_authority(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<::std::size_t> {
    auto const opt_domain_end{::pltxt2htm::details::try_parse_url_domain<ndebug>(pltext)};
    if (opt_domain_end.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const domain_end{opt_domain_end.template value<ndebug>()};
    if (domain_end >= pltext.size() || pltext.template index<ndebug>(domain_end) != u8':') {
        return domain_end;
    }
    auto const opt_port_end{
        ::pltxt2htm::details::try_parse_url_port<ndebug>(pltext.template subview<ndebug>(domain_end + 1))};
    if (opt_port_end.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    return domain_end + 1 + opt_port_end.template value<ndebug>();
}

constexpr void append_percent_encoded_url_byte(::fast_io::u8string& result, char8_t byte) noexcept {
    result.push_back(u8'%');
    auto const hi{static_cast<unsigned>(byte) >> 4};
    auto const lo{static_cast<unsigned>(byte) & 0x0F};
    result.push_back(static_cast<char8_t>(hi < 10 ? u8'0' + hi : u8'A' + (hi - 10)));
    result.push_back(static_cast<char8_t>(lo < 10 ? u8'0' + lo : u8'A' + (lo - 10)));
}

constexpr void append_code_point_to_url(::fast_io::u8string& result, char32_t code_point) noexcept {
    if (code_point < char32_t{0x80}) {
        auto const chr{static_cast<char8_t>(code_point)};
        if (chr < u8'!' || chr > u8'~' || chr == u8'\'' || chr == u8'<' || chr == u8'>' || chr == u8'"') {
            ::pltxt2htm::details::append_percent_encoded_url_byte(result, chr);
        }
        else {
            result.push_back(chr);
        }
        return;
    }

    auto const encoded = ::pltxt2htm::details::encode_utf8_code_point(code_point);
    for (::std::size_t index{}; index < encoded.size; ++index) {
        ::pltxt2htm::details::append_percent_encoded_url_byte(result, encoded.code_units[index]);
    }
}

/**
 * @brief Build a URL AST from a raw URL string, decoding HTML character references.
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
constexpr auto make_try_parse_url_result(::pltxt2htm::container::U8StringView const parsed_url,
                                         ::std::size_t consumed_size) noexcept
    -> ::pltxt2htm::container::Optional<TryParseUrlResult> {
    ::std::size_t const parsed_url_size{parsed_url.size()};
    ::fast_io::u8string url_str{};
    url_str.reserve(parsed_url_size);
    for (::std::size_t index{}; index < parsed_url_size; ++index) {
        auto const chr = parsed_url.template index<ndebug>(index);
        if (chr == u8'&') {
            auto const reference = ::pltxt2htm::details::try_decode_character_reference<ndebug>(
                parsed_url.template subview<ndebug>(index));
            if (reference.has_value()) {
                auto const& decoded = reference.template value<ndebug>();
                ::pltxt2htm::details::append_code_point_to_url(url_str, decoded.first_code_point);
                if (decoded.code_point_count == 2) {
                    ::pltxt2htm::details::append_code_point_to_url(url_str, decoded.second_code_point);
                }
                index += decoded.consumed_size - 1;
                continue;
            }
        }
        if (chr > u8'~') {
            // non-ASCII byte (e.g. UTF-8 CJK): percent-encode it so tag URLs keep the raw characters
            ::pltxt2htm::details::append_percent_encoded_url_byte(url_str, chr);
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
    return TryParseUrlResult{.consumed_size = consumed_size, .url = ::pltxt2htm::Url{::std::move(url_str)}};
}

/**
 * @brief Parse the simple URL path: printable ASCII, stops at `<` `>` `"` or non-printable characters.
 * @details `pltext` must start at the path (the caller subviews past the authority); the
 *          returned index is relative to `pltext`.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text view starting at the path.
 * @return The relative index at which the path ends.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_path_simple(::pltxt2htm::container::U8StringView pltext) noexcept -> ::std::size_t {
    ::std::size_t const pltext_size{pltext.size()};
    if (pltext.empty() == false) {
        auto const chr = pltext.template index<ndebug>(0);
        if (chr != u8'/' && chr != u8'?' && chr != u8'#') {
            return 0;
        }
    }
    ::std::size_t current_index{};
    while (current_index < pltext_size) {
        auto const chr = pltext.template index<ndebug>(current_index);
        if (chr < u8'!' || chr > u8'~' || chr == u8'<' || chr == u8'>' || chr == u8'\"') {
            break;
        }
        ++current_index;
    }
    return current_index;
}

/**
 * @brief Parse a URL path that may contain non-ASCII bytes (percent-encoded later).
 * @details Like try_parse_url_path_simple but also accepts bytes >= 0x7F so tag URLs
 *          (html_a / pl_external / pl_link) can carry UTF-8 characters (e.g. CJK);
 *          make_try_parse_url_result percent-encodes them. Auto-detected URLs stay ASCII-only.
 *          `pltext` must start at the path (the caller subviews past the authority); the
 *          returned index is relative to `pltext`.
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text view starting at the path.
 * @return The relative index at which the path ends.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_url_path_unicode(::pltxt2htm::container::U8StringView pltext) noexcept -> ::std::size_t {
    ::std::size_t const pltext_size{pltext.size()};
    if (pltext.empty() == false) {
        auto const chr = pltext.template index<ndebug>(0);
        if (chr != u8'/' && chr != u8'?' && chr != u8'#') {
            return 0;
        }
    }
    ::std::size_t current_index{};
    while (current_index < pltext_size) {
        auto const chr = pltext.template index<ndebug>(current_index);
        if (chr < u8'!' || chr == u8'<' || chr == u8'>' || chr == u8'\"') {
            break;
        }
        ++current_index;
    }
    return current_index;
}

/**
 * @brief Result of parsing a URL-bearing opening tag.
 * @details The three return states are encoded by the payload members `tag_len` and `url`:
 *          - `valid` - `url` is engaged (`tag_len` is the opening-tag length the caller
 *            skips to reach the tag content);
 *          - `invalid_url` - `url` is disengaged and `tag_len` != 0: the opening tag was
 *            recognized but its URL failed validation, so the caller consumes the first
 *            `tag_len` characters as one literal span;
 *          - `not_a_tag` - `tag_len` == 0: keep the char-by-char fallback.
 *          A recognized opening tag always has `tag_len` != 0, which keeps the three
 *          states distinguishable.
 */
struct TryParseHtmlATagResult {
    ::std::size_t tag_len; ///< Opening-tag length in the input view (valid for valid/invalid_url).
    ::pltxt2htm::container::Optional<::pltxt2htm::Url> url; ///< Extracted URL; engaged only when valid.
    bool internal;

    constexpr TryParseHtmlATagResult() noexcept
        : tag_len{},
          url{::pltxt2htm::container::nullopt},
          internal{} {
    }

    constexpr TryParseHtmlATagResult(::std::size_t tag_len_) noexcept
        : tag_len{tag_len_},
          url{::pltxt2htm::container::nullopt},
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
constexpr auto try_parse_html_a_tag(::pltxt2htm::container::U8StringView pltext) noexcept -> TryParseHtmlATagResult {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t pos{};
    while (pos < pltext_size &&
           (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
        ++pos;
    }
    if (pos >= pltext_size) {
        return {};
    }
    if (::pltxt2htm::details::is_prefix_match<ndebug, ::pltxt2htm::details::U8LiteralString{u8"href"}>(
            pltext.template subview<ndebug>(pos)) == false) {
        return {};
    }
    pos += 4;
    while (pos < pltext_size &&
           (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
        ++pos;
    }
    if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'=') {
        return {};
    }
    ++pos;
    while (pos < pltext_size &&
           (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
        ++pos;
    }
    if (pos >= pltext_size) {
        return {};
    }
    char8_t const quote{pltext.template index<ndebug>(pos)};
    if (quote != u8'"' && quote != u8'\'') {
        return {};
    }
    ++pos;
    ::std::size_t const val_start{pos};
    while (pos < pltext_size && pltext.template index<ndebug>(pos) != quote) {
        ++pos;
    }
    if (pos >= pltext_size) {
        return {};
    }
    ::pltxt2htm::container::U8StringView const attr_val{pltext.data() + val_start, pos - val_start};
    ++pos;
    while (pos < pltext_size &&
           (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
        ++pos;
    }
    bool internal{};
    if (pos < pltext_size && pltext.template index<ndebug>(pos) != u8'>') {
        // only the boolean attribute "internal" is accepted as an extra attribute
        constexpr auto internal_literal = ::pltxt2htm::details::U8LiteralString{u8"internal"};
        if (::pltxt2htm::details::is_prefix_match<ndebug, internal_literal>(pltext.template subview<ndebug>(pos)) ==
            false) {
            return {};
        }
        internal = true;
        pos += internal_literal.size();
        while (pos < pltext_size &&
               (pltext.template index<ndebug>(pos) == u8' ' || pltext.template index<ndebug>(pos) == u8'\t')) {
            ++pos;
        }
    }
    if (pos >= pltext_size || pltext.template index<ndebug>(pos) != u8'>') {
        return {};
    }
    auto const scheme_end = ::pltxt2htm::details::try_parse_url_scheme<ndebug>(attr_val).value_or(::std::size_t{});
    auto opt_auth_end =
        ::pltxt2htm::details::try_parse_url_authority<ndebug>(attr_val.template subview<ndebug>(scheme_end));
    if (opt_auth_end.has_value() == false) {
        return TryParseHtmlATagResult{pos + 1};
    }
    auto const auth_end = opt_auth_end.template value<ndebug>() + scheme_end;
    auto const path_end =
        ::pltxt2htm::details::try_parse_url_path_unicode<ndebug>(attr_val.template subview<ndebug>(auth_end)) +
        auth_end;
    if (path_end != attr_val.size()) {
        return TryParseHtmlATagResult{pos + 1};
    }
    auto opt_url_result = ::pltxt2htm::details::make_try_parse_url_result<ndebug>(attr_val, path_end);
    if (opt_url_result.has_value() == false) {
        return TryParseHtmlATagResult{pos + 1};
    }
    return TryParseHtmlATagResult{pos + 1, ::std::move(opt_url_result.template value<ndebug>().url), internal};
}

/**
 * @brief Parse an auto-detected bare URL (http/https) with context guards.
 *
 * Detects a URL starting with `http://` or `https://` at `current_index`.
 *
 * Only `http://`/`https://` schemes are accepted.  Bare domains (e.g. `example.com`) are
 * intentionally not supported because the parser calls this function at every character
 * position - allowing domain scans without a scheme prefix would reintroduce O(n^2)
 * behaviour on long lines without whitespace.
 *
 * @tparam ndebug When set to `::pltxt2htm::Contracts::ignore`, runtime assertions are disabled for performance.
 * @param[in] pltext The input text view starting at the URL to parse.
 * @return Parsed URL payload on success; nullopt if no valid auto-link is found.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_auto_url(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseUrlResult> {
    auto opt_scheme_end = ::pltxt2htm::details::try_parse_url_scheme<ndebug>(pltext);
    if (opt_scheme_end.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const scheme_end = opt_scheme_end.template value<ndebug>();
    auto opt_auth_end =
        ::pltxt2htm::details::try_parse_url_authority<ndebug>(pltext.template subview<ndebug>(scheme_end));
    if (opt_auth_end.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const auth_end = opt_auth_end.template value<ndebug>() + scheme_end;

    auto const path_end =
        ::pltxt2htm::details::try_parse_url_path_simple<ndebug>(pltext.template subview<ndebug>(auth_end)) + auth_end;
    return ::pltxt2htm::details::make_try_parse_url_result<ndebug>(pltext.template subview<ndebug>(0, path_end),
                                                                   path_end);
}

struct TryParseMdUrlResult {
    ::std::size_t consumed_size;
    ::pltxt2htm::Url url;
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_md_url(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMdUrlResult> {
    ::std::size_t const pltext_size{pltext.size()};
    // First attempt: authority + path with `)` as terminator, then verify `)` follows
    auto const scheme_end = ::pltxt2htm::details::try_parse_url_scheme<ndebug>(pltext).value_or(::std::size_t{});
    auto opt_auth_end =
        ::pltxt2htm::details::try_parse_url_authority<ndebug>(pltext.template subview<ndebug>(scheme_end));
    if (opt_auth_end.has_value()) {
        auto const auth_end = opt_auth_end.template value<ndebug>() + scheme_end;
        bool const has_path_start = auth_end < pltext_size && (pltext.template index<ndebug>(auth_end) == u8'/' ||
                                                               pltext.template index<ndebug>(auth_end) == u8'?' ||
                                                               pltext.template index<ndebug>(auth_end) == u8'#');
        bool const ends_after_authority = auth_end < pltext_size && pltext.template index<ndebug>(auth_end) == u8')';
        if (has_path_start || ends_after_authority) {
            ::std::size_t path_end{auth_end};
            while (path_end < pltext_size) {
                auto const chr = pltext.template index<ndebug>(path_end);
                if (chr == u8')') {
                    break;
                }
                if (chr < u8'!' || chr > u8'~' || chr == u8'<' || chr == u8'>' || chr == u8'\"') {
                    break;
                }
                ++path_end;
            }
            if (path_end < pltext_size && pltext.template index<ndebug>(path_end) == u8')') {
                auto const url_vw = pltext.template subview<ndebug>(0, path_end);
                auto opt_result = ::pltxt2htm::details::make_try_parse_url_result<ndebug>(url_vw, path_end);
                if (opt_result.has_value()) {
                    auto&& result = opt_result.template value<ndebug>();
                    return TryParseMdUrlResult{.consumed_size = path_end, .url = ::std::move(result.url)};
                }
            }
        }
    }

    // Fallback: locate the closing parenthesis and encode the URL in one pass.
    ::fast_io::u8string encoded{};
    ::std::size_t raw_len{};
    for (; raw_len < pltext_size; ++raw_len) {
        auto const chr = pltext.template index<ndebug>(raw_len);
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
    pltxt2htm_assert(raw_len <= pltext_size, u8"raw_len should not exceed pltext size");
    if (raw_len == pltext_size) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const encoded_vw = ::pltxt2htm::container::U8StringView{encoded};
    auto const retry_scheme_end =
        ::pltxt2htm::details::try_parse_url_scheme<ndebug>(encoded_vw).value_or(::std::size_t{});
    auto opt_retry_auth =
        ::pltxt2htm::details::try_parse_url_authority<ndebug>(encoded_vw.template subview<ndebug>(retry_scheme_end));
    if (opt_retry_auth.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto const retry_auth_end = opt_retry_auth.template value<ndebug>() + retry_scheme_end;
    auto const retry_path_end =
        ::pltxt2htm::details::try_parse_url_path_simple<ndebug>(encoded_vw.template subview<ndebug>(retry_auth_end)) +
        retry_auth_end;
    if (retry_path_end != encoded_vw.size()) {
        return ::pltxt2htm::container::nullopt;
    }
    auto opt_result = ::pltxt2htm::details::make_try_parse_url_result<ndebug>(encoded_vw, encoded_vw.size());
    if (opt_result.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    return TryParseMdUrlResult{.consumed_size = raw_len, .url = ::std::move(opt_result.template value<ndebug>().url)};
}

struct TryParseMdLinkResult {
    ::std::size_t advance_count;
    ::pltxt2htm::container::U8StringView link_text;
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
constexpr auto try_parse_md_link(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMdLinkResult> {
    ::std::size_t const pltext_size{pltext.size()};
    if (pltext_size < 4 || pltext.template index<ndebug>(0) != u8'[') {
        return ::pltxt2htm::container::nullopt;
    }

    ::std::size_t current_index{1};

    // Parse link text
    while (current_index < pltext_size) {
        char8_t const chr{pltext.template index<ndebug>(current_index)};
        if (chr == u8'\\') {
            // No need to handle escape here
            // Because the result of `link_text` is string_view
            // `::pltxt2htm::details::parse_pltxt` will handle the escape when converting to ast
            ++current_index;
        }
        else if (chr == u8'\n') {
            return ::pltxt2htm::container::nullopt;
        }
        else if (chr == u8']') {
            break;
        }
        ++current_index;
    }

    if (current_index >= pltext_size || pltext.template index<ndebug>(current_index) != u8']') {
        return ::pltxt2htm::container::nullopt;
    }
    ::std::size_t const link_text_end{current_index};
    ++current_index;

    // Ensure the next character is '('
    if (current_index >= pltext_size || pltext.template index<ndebug>(current_index) != u8'(') {
        return ::pltxt2htm::container::nullopt;
    }
    ++current_index;
    auto opt_md_url = ::pltxt2htm::details::try_parse_md_url<ndebug>(pltext.template subview<ndebug>(current_index));
    if (opt_md_url.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto&& md_url_result = opt_md_url.template value<ndebug>();
    current_index += md_url_result.consumed_size;
    if (current_index >= pltext_size || pltext.template index<ndebug>(current_index) != u8')') {
        return ::pltxt2htm::container::nullopt;
    }
    ++current_index;
    return TryParseMdLinkResult{.advance_count = current_index,
                                .link_text = pltext.template subview<ndebug>(1, link_text_end - 1),
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
constexpr auto try_parse_md_image(::pltxt2htm::container::U8StringView pltext) noexcept
    -> ::pltxt2htm::container::Optional<TryParseMdImageResult<ndebug>> {
    ::std::size_t const pltext_size{pltext.size()};
    if (pltext_size < 5 || pltext.template index<ndebug>(0) != u8'!' || pltext.template index<ndebug>(1) != u8'[') {
        return ::pltxt2htm::container::nullopt;
    }

    ::std::size_t current_index{2};

    // Parse link text
    ::pltxt2htm::Ast<ndebug> link_text_ast{};
    while (current_index < pltext_size) {
        char8_t const chr{pltext.template index<ndebug>(current_index)};

        if (chr == u8']') {
            break;
        }

        if (chr == u8'\n') {
            return ::pltxt2htm::container::nullopt;
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
        if (auto opt_escape =
                ::pltxt2htm::details::try_parse_md_escape<ndebug>(pltext.template subview<ndebug>(current_index));
            opt_escape.has_value()) {
            auto&& [node, advance_count] = opt_escape.template value<ndebug>();
            link_text_ast.push_back(::std::move(node));
            current_index += advance_count;
            continue;
        }
        if (chr == u8'<') {
            link_text_ast.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::LessThan{}));
            ++current_index;
            continue;
        }
        auto const advance_count = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
            pltext.template subview<ndebug>(current_index), link_text_ast);
        current_index += advance_count;
        continue;
    }

    if (current_index >= pltext_size || pltext.template index<ndebug>(current_index) != u8']') {
        return ::pltxt2htm::container::nullopt;
    }
    ++current_index;

    // Ensure the next character is '('
    if (current_index >= pltext_size || pltext.template index<ndebug>(current_index) != u8'(') {
        return ::pltxt2htm::container::nullopt;
    }
    ++current_index;
    auto opt_md_url = ::pltxt2htm::details::try_parse_md_url<ndebug>(pltext.template subview<ndebug>(current_index));
    if (opt_md_url.has_value() == false) {
        return ::pltxt2htm::container::nullopt;
    }
    auto&& md_url_result = opt_md_url.template value<ndebug>();
    current_index += md_url_result.consumed_size;
    if (current_index >= pltext_size || pltext.template index<ndebug>(current_index) != u8')') {
        return ::pltxt2htm::container::nullopt;
    }
    return TryParseMdImageResult<ndebug>{.advance_count = current_index + 1,
                                         .link_text = ::std::move(link_text_ast),
                                         .link_url = ::std::move(md_url_result.url)};
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
