#pragma once

#if __cpp_expansion_statements >= 202506L
    #include <ranges>
#endif
#include <cstddef>
#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "utils.hh"
#include "heap_guard.hh"
#include "astnode/node_type.hh"
#include "astnode/basic.hh"
#include "astnode/html_node.hh"
#include "astnode/markdown_node.hh"
#include "astnode/physics_lab_node.hh"
#include "push_macro.hh"

namespace pltxt2htm {

namespace details {

/**
 * @brief Switch to a markdown punctuation character.
 */
constexpr ::exception::optional<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>> switch_escape_char(
    char8_t u8char) noexcept {
    switch (u8char) {
    case u8'\\': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeBackslash>{});
    }
    case u8'!': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeExclamation>{});
    }
    case u8'\"': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeDoubleQuote>{});
    }
    case u8'#': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeHash>{});
    }
    case u8'$': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeDollar>{});
    }
    case u8'%': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapePercent>{});
    }
    case u8'&': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeAmpersand>{});
    }
    case u8'\'': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeSingleQuote>{});
    }
    case u8'(': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeLeftParen>{});
    }
    case u8')': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeRightParen>{});
    }
    case u8'*': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeAsterisk>{});
    }
    case u8'+': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapePlus>{});
    }
    case u8',': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeComma>{});
    }
    case u8'-': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeHyphen>{});
    }
    case u8'.': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeDot>{});
    }
    case u8'/': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeSlash>{});
    }
    case u8':': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeColon>{});
    }
    case u8';': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeSemicolon>{});
    }
    case u8'<': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeLessThan>{});
    }
    case u8'=': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeEquals>{});
    }
    case u8'>': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeGreaterThan>{});
    }
    case u8'?': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeQuestion>{});
    }
    case u8'@': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeAt>{});
    }
    case u8'[': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeLeftBracket>{});
    }
    case u8']': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeRightBracket>{});
    }
    case u8'^': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeCaret>{});
    }
    case u8'_': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeUnderscore>{});
    }
    case u8'`': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeBacktick>{});
    }
    case u8'{': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeLeftBrace>{});
    }
    case u8'|': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapePipe>{});
    }
    case u8'}': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeRightBrace>{});
    }
    case u8'~': {
        return static_cast<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>(
            ::pltxt2htm::details::HeapGuard<::pltxt2htm::EscapeTilde>{});
    }
    default:
        return ::exception::nullopt_t{};
    }
}

/**
 * @brief Check whether the string is a prefix (without distinguishing
 *        between uppercase and lowercase) of the pl-text.
 *        This is a magic funciton that will generate if-expression in compile time.
 * @tparam prefix_str: The prefix string.
 * @param str: The string to be checked.
 * @return Whether the string is a prefix of the pl-text.
 */
template<bool ndebug, char8_t... prefix_str>
    requires ((prefix_str < 'A' || prefix_str > 'Z') && ...)
#if __has_cpp_attribute(__gnu__::always_inline)
[[__gnu__::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
[[nodiscard]]
constexpr bool is_prefix_match(::fast_io::u8string_view str) /* throws */ {
    // Check whether the index is out of bound.
    if (sizeof...(prefix_str) > str.size()) [[unlikely]] {
        return false;
    }

    // Return whether the `prefix_str` is a prefix of the str.
#if __cpp_expansion_statements >= 202506L
    template for (constexpr ::std::size_t I : ::std::ranges::views::iota(::std::size_t{}, sizeof...(prefix_str))) {
#else
    return [str]<::std::size_t... Is>(::std::index_sequence<Is...>) {
        return ([str]<::std::size_t I>() {
#endif
        constexpr auto expect = ::pltxt2htm::details::pack_indexing_char8_t<I, prefix_str...>();
        if constexpr ('a' <= expect && expect <= 'z') {
            // ASCII between lowercase and uppercase is 32 (e.g. 'a' - 'A' == 32)
            constexpr ::std::uint8_t diff{32};
            // (expect != str[I] && expect != str[I] + diff) <=> (expect != (str[I] | diff))
            if (expect != (::pltxt2htm::details::u8string_view_index<ndebug>(str, I) | diff)) {
                return false;
            } else {
                return true;
            }
        } else {
            if (expect != ::pltxt2htm::details::u8string_view_index<ndebug>(str, I)) {
                return false;
            } else {
                return true;
            }
        }
#if __cpp_expansion_statements >= 202506L
    }
#else
        }.template operator()<Is>() &&
                ...);
    }(::std::make_index_sequence<sizeof...(prefix_str)>{});
#endif
}

struct ParseUtf8CodePointResult {
    ::pltxt2htm::Ast result;
    ::std::size_t forward_index;
};

/**
 * @brief Parse a single UTF-8 code point and append the corresponding AST node(s).
 *
 * @tparam ndebug  When `true`, runtime assertions are disabled.
 * @param pltext   The complete input text being parsed.
 */
template<bool ndebug>
constexpr ::pltxt2htm::details::ParseUtf8CodePointResult parse_utf8_code_point(
    ::fast_io::u8string_view const& pltext) /* throws */ {
    ::pltxt2htm::Ast result{};
    ::std::size_t const pltext_size{pltext.size()};
    char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 0)};

    if (chr <= 0x1f || (0x7f <= chr && chr <= 0x9f)) {
        return {.result = ::std::move(result), .forward_index = 0};
    }
    if ((chr & 0x80) == 0) {
        // normal utf-8 characters
        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{chr});
        return {.result = ::std::move(result), .forward_index = 0};
    } else if ((chr & 0xE0) == 0xC0) {
        if (1 >= pltext_size) {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return {.result = ::std::move(result), .forward_index = 0};
        }
        auto next_char = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1);
        if ((next_char & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return {.result = ::std::move(result), .forward_index = 0};
        }
        char32_t combine{static_cast<char32_t>(chr & 0x1F) << 6 | static_cast<char32_t>(next_char & 0x3F)};
        if (combine < 0x80 || combine > 0x7FF) {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return {.result = ::std::move(result), .forward_index = 0};
        }

        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{chr});
        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{next_char});
        return {.result = ::std::move(result), .forward_index = 1};
    } else if ((chr & 0xF0) == 0xE0) {
        if (2 >= pltext_size) {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return {.result = ::std::move(result), .forward_index = 0};
        }
        auto next_char = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1);
        if ((next_char & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return {.result = ::std::move(result), .forward_index = 0};
        }
        auto next_char2 = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 2);
        if ((next_char2 & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return {.result = ::std::move(result), .forward_index = 0};
        }
        char32_t combine{static_cast<char32_t>(chr & 0x0f) << 12 | static_cast<char32_t>(next_char & 0x3f) << 6 |
                         static_cast<char32_t>(next_char2 & 0x3f)};
        if (combine < 0x800 || combine > 0xffff) {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return {.result = ::std::move(result), .forward_index = 0};
        }
        if (0xd800 <= combine && combine <= 0xdfff) {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return {.result = ::std::move(result), .forward_index = 0};
        }

        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{chr});
        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{next_char});
        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{next_char2});
        return {.result = ::std::move(result), .forward_index = 2};
    } else if ((chr & 0xF8) == 0xF0) {
        if (3 >= pltext_size) {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return {.result = ::std::move(result), .forward_index = 0};
        }
        auto next_char = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1);
        if ((next_char & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return {.result = ::std::move(result), .forward_index = 0};
        }
        auto next_char2 = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 2);
        if ((next_char & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return {.result = ::std::move(result), .forward_index = 0};
        }
        auto next_char3 = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 3);
        if ((next_char3 & 0xC0) != 0x80) {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return {.result = ::std::move(result), .forward_index = 0};
        }
        char32_t combine{static_cast<char32_t>(chr & 0x07) << 18 | static_cast<char32_t>(next_char & 0x3F) << 12 |
                         static_cast<char32_t>(next_char2 & 0x3F) << 6 | static_cast<char32_t>(next_char3 & 0x3F)};
        if (combine < 0x10000 || combine > 0x10FFFF) {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return {.result = ::std::move(result), .forward_index = 0};
        }
        if (0xd800 <= combine && combine <= 0xdfff) {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::InvalidU8Char>{});
            return {.result = ::std::move(result), .forward_index = 0};
        }

        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{chr});
        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{next_char});
        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{next_char2});
        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{next_char3});
        return {.result = ::std::move(result), .forward_index = 3};
    } else {
        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::InvalidU8Char>{});
        return {.result = ::std::move(result), .forward_index = 0};
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
constexpr auto try_parse_bare_tag(
    ::fast_io::u8string_view pltext) /* throws */ -> ::exception::optional<::std::size_t> {
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
constexpr auto try_parse_equal_sign_tag(::fast_io::u8string_view pltext,
                                        Func&& func) /* throws */ -> ::exception::optional<TryParseEqualSignTagResult> {
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

enum class MdAtxHeadingEndingType : ::std::uint_least32_t {
    no_ending = 0,
    newline,
    br_tag
};

struct MdAtxEndingType {
    ::pltxt2htm::details::MdAtxHeadingEndingType ending_type;
    ::std::size_t br_len;
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
constexpr bool try_parse_md_atx_heading(::fast_io::u8string_view pltext, ::std::size_t& start_index_,
                                        ::std::size_t& sublength_, ::pltxt2htm::NodeType& md_atx_heading_type_,
                                        ::pltxt2htm::details::MdAtxEndingType& ending_type) /* throws */ {
    ::std::size_t const pltext_size{pltext.size()};
    ::std::size_t start_index{};
    // spaces before the first #
    while (true) {
        if (start_index >= pltext_size) {
            return false;
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
                start_index_ = start_index;
                sublength_ = 0;
                md_atx_heading_type_ = static_cast<::pltxt2htm::NodeType>(md_atx_heading_type);
                ending_type.ending_type = ::pltxt2htm::details::MdAtxHeadingEndingType::no_ending;
                return true;
            } else {
                return false;
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
        return false;
    }
    ++start_index;
    // spaces after the last #
    while (true) {
        if (start_index >= pltext_size) {
            // https://spec.commonmark.org/0.31.2/#example-79
            start_index_ = start_index;
            sublength_ = 0;
            md_atx_heading_type_ = static_cast<::pltxt2htm::NodeType>(md_atx_heading_type);
            ending_type.ending_type = ::pltxt2htm::details::MdAtxHeadingEndingType::no_ending;
            return true;
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, start_index) != u8' ') {
            break;
        }
        ++start_index;
    }
    // end of the atx header
    ::std::size_t sublength{start_index};
    for (; sublength < pltext_size; ++sublength) {
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, sublength) == u8'\n') {
            ending_type.ending_type = ::pltxt2htm::details::MdAtxHeadingEndingType::newline;
            break;
        } else if (auto opt = ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, u8'<', u8'b', u8'r'>(
                       ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, sublength));
                   opt.has_value()) {
            ending_type.ending_type = ::pltxt2htm::details::MdAtxHeadingEndingType::br_tag;
            ending_type.br_len = opt.template value<ndebug>();
            break;
        }
    }
    start_index_ = start_index;
    sublength_ = sublength - start_index;
    md_atx_heading_type_ = static_cast<::pltxt2htm::NodeType>(md_atx_heading_type);
    return true;
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

enum class EndType : ::std::uint_least32_t {
    end_of_string = 0,
    line_break,
    br_tag,
};

struct TryParseMdThematicBreakResult {
    ::pltxt2htm::Ast subast;
    ::std::size_t forward_index;
};

/**
 * @brief Parsing markdown thematic breaks (e.g. `---`, `___`, `***`)
 * @param text The text to parse
 * @return length of the parsed markdown thematic breaks
 */
template<bool ndebug>
constexpr auto try_parse_md_thematic_break(
    ::fast_io::u8string_view text) /* throws */ -> ::exception::optional<TryParseMdThematicBreakResult> {
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
                return ::pltxt2htm::details::TryParseMdThematicBreakResult{
                    ::pltxt2htm::Ast{::pltxt2htm::details::HeapGuard<::pltxt2htm::MdHr>{},
                                     ::pltxt2htm::details::HeapGuard<::pltxt2htm::LineBreak>{}},
                    i + 1};
            } else if (auto opt_tag_len = ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, u8'<', u8'b', u8'r'>(
                           ::pltxt2htm::details::u8string_view_subview<ndebug>(text, i));
                       opt_tag_len.has_value()) {
                return ::pltxt2htm::details::TryParseMdThematicBreakResult{
                    ::pltxt2htm::Ast{::pltxt2htm::details::HeapGuard<::pltxt2htm::MdHr>{},
                                     ::pltxt2htm::details::HeapGuard<::pltxt2htm::Br>{}},
                    i + opt_tag_len.template value<ndebug>() + 1};
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
        return ::pltxt2htm::details::TryParseMdThematicBreakResult{
            ::pltxt2htm::Ast{::pltxt2htm::details::HeapGuard<::pltxt2htm::MdHr>{}}, i};
    }
}

struct TryParseMdCodeFenceResult {
    ::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode> node;
    ::std::size_t forward_index;
};

template<bool ndebug, bool is_backtick>
constexpr auto try_parse_md_code_fence_(::fast_io::u8string_view pltext) /* throws */ -> ::exception::optional<
    ::pltxt2htm::details::TryParseMdCodeFenceResult> {
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

    // TODO rename to ast
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
            ast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LineBreak>{});
        } else if (chr == u8' ') {
            ast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Space>{});
            continue;
        } else if (chr == u8'&') {
            ast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Ampersand>{});
            continue;
        } else if (chr == u8'\'') {
            ast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::SingleQuotationMark>{});
            continue;
        } else if (chr == u8'\"') {
            ast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::DoubleQuotationMark>{});
            continue;
        } else if (chr == u8'>') {
            ast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::GreaterThan>{});
            continue;
        } else if (chr == u8'\t') {
            ast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Tab>{});
            continue;
        } else if (chr == u8'\\') {
            if (current_index + 1 == pltext_size) {
                ast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{u8'\\'});
                continue;
            }
            auto escape_node = ::pltxt2htm::details::switch_escape_char(
                ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 1));
            if (escape_node.has_value()) {
                ast.push_back(::std::move(escape_node.template value<ndebug>()));
                ++current_index;
            } else {
                ast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{u8'\\'});
            }
            continue;
        } else if (chr == u8'<') {
            ast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
        } else {
            auto&& [subast, forward_index] = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            current_index += forward_index;
            for (auto&& node : subast) {
                ast.push_back(::std::move(node));
            }
            continue;
        }
    }

    ::exception::optional<::fast_io::u8string> opt_lang{::exception::nullopt_t{}};
    if (!lang.empty()) {
        opt_lang = ::std::move(lang);
    }
    if constexpr (is_backtick) {
        return ::pltxt2htm::details::TryParseMdCodeFenceResult{
            .node = ::pltxt2htm::details::HeapGuard<::pltxt2htm::MdCodeFenceBacktick>{::std::move(ast),
                                                                                      ::std::move(opt_lang)},
            .forward_index = current_index + 4};
    } else {
        return ::pltxt2htm::details::TryParseMdCodeFenceResult{
            .node =
                ::pltxt2htm::details::HeapGuard<::pltxt2htm::MdCodeFenceTilde>{::std::move(ast), ::std::move(opt_lang)},
            .forward_index = current_index + 4};
    }
}

template<bool ndebug>
constexpr auto try_parse_md_code_fence(::fast_io::u8string_view pltext) /* throws */ -> ::exception::optional<
    ::pltxt2htm::details::TryParseMdCodeFenceResult> {
    if (auto opt_code_fence_backtick = ::pltxt2htm::details::try_parse_md_code_fence_<ndebug, true>(pltext);
        opt_code_fence_backtick.has_value()) {
        return opt_code_fence_backtick;
    } else if (auto opt_code_fence_tilde = ::pltxt2htm::details::try_parse_md_code_fence_<ndebug, false>(pltext);
               opt_code_fence_tilde.has_value()) {
        return opt_code_fence_tilde;
    } else {
        return ::exception::nullopt_t{};
    }
}

/**
 * @brief Switch to a markdown atx header.
 * @param[in] header_level: The header level.
 * @param[in] subast: The sub ast.
 * @return True if successful.
 */
template<bool ndebug>
[[nodiscard]]
constexpr auto switch_md_atx_header(::pltxt2htm::NodeType md_atx_heading_type, ::pltxt2htm::Ast&& subast) noexcept
    -> ::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode> {
    switch (md_atx_heading_type) {
    case ::pltxt2htm::NodeType::md_atx_h1: {
        return ::pltxt2htm::details::HeapGuard<::pltxt2htm::MdAtxH1>(::std::move(subast));
    }
    case ::pltxt2htm::NodeType::md_atx_h2: {
        return ::pltxt2htm::details::HeapGuard<::pltxt2htm::MdAtxH2>(::std::move(subast));
    }
    case ::pltxt2htm::NodeType::md_atx_h3: {
        return ::pltxt2htm::details::HeapGuard<::pltxt2htm::MdAtxH3>(::std::move(subast));
    }
    case ::pltxt2htm::NodeType::md_atx_h4: {
        return ::pltxt2htm::details::HeapGuard<::pltxt2htm::MdAtxH4>(::std::move(subast));
    }
    case ::pltxt2htm::NodeType::md_atx_h5: {
        return ::pltxt2htm::details::HeapGuard<::pltxt2htm::MdAtxH5>(::std::move(subast));
    }
    case ::pltxt2htm::NodeType::md_atx_h6: {
        return ::pltxt2htm::details::HeapGuard<::pltxt2htm::MdAtxH6>(::std::move(subast));
    }
    default:
        [[unlikely]] {
            ::exception::unreachable<ndebug>();
        }
    }
}

class BasicFrameContext {
public:
    ::fast_io::u8string_view pltext;
    ::pltxt2htm::NodeType const nested_tag_type;
    ::std::size_t current_index{};
    ::pltxt2htm::Ast subast{};

    constexpr ~BasicFrameContext() noexcept = default;

protected:
    constexpr BasicFrameContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_) noexcept
        : pltext(pltext_),
          nested_tag_type{nested_tag_type_} {
    }
};

class BareTagContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    constexpr BareTagContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_) noexcept
        : ::pltxt2htm::details::BasicFrameContext(pltext_, nested_tag_type_) {
    }

    constexpr ~BareTagContext() noexcept = default;
};

class MdAtxHeadingContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::pltxt2htm::details::MdAtxEndingType ending_type;

    constexpr MdAtxHeadingContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                  ::pltxt2htm::details::MdAtxEndingType ending_type_) noexcept
        : ::pltxt2htm::details::BasicFrameContext(pltext_, nested_tag_type_),
          ending_type{ending_type_} {
    }

    constexpr ~MdAtxHeadingContext() noexcept = default;
};

class EqualSignTagContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::fast_io::u8string id;

    constexpr EqualSignTagContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                                  ::fast_io::u8string&& id_) noexcept
        : ::pltxt2htm::details::BasicFrameContext{pltext_, nested_tag_type_},
          id(::std::move(id_)) {
    }

    constexpr ~EqualSignTagContext() noexcept = default;
};

class PlSizeTagContext : public ::pltxt2htm::details::BasicFrameContext {
public:
    ::std::size_t id;

    constexpr PlSizeTagContext(::fast_io::u8string_view pltext_, ::pltxt2htm::NodeType const nested_tag_type_,
                               ::std::size_t id_) noexcept
        : ::pltxt2htm::details::BasicFrameContext{pltext_, nested_tag_type_},
          id(::std::move(id_)) {
    }

    constexpr ~PlSizeTagContext() noexcept = default;
};

/**
 * @brief Parse pl-text to nodes.
 * @tparam ndebug: Whether disables all debug checks.
 * @param call_stack: use `call_stack` + `goto restart` to avoid stack overflow.
 * @return Quantum-Physics text's ast.
 */
template<bool ndebug>
[[nodiscard]]
constexpr auto parse_pltxt(
    ::fast_io::stack<::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BasicFrameContext>,
                     ::fast_io::list<::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BasicFrameContext>>>&
        call_stack) /* throws */ -> ::pltxt2htm::Ast {
restart:
    auto&& current_index = call_stack.top()->current_index;
    auto&& pltext = call_stack.top()->pltext;
    auto&& result = call_stack.top()->subast;
    ::std::size_t const pltext_size{pltext.size()};

    for (; current_index < pltext_size; ++current_index) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};

        if (chr == u8'\n') {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LineBreak>{});

#if __has_cpp_attribute(indeterminate)
            ::std::size_t start_index [[indeterminate]];
            ::std::size_t end_index [[indeterminate]];
            ::pltxt2htm::NodeType md_atx_heading_type [[indeterminate]];
            ::pltxt2htm::details::MdAtxEndingType ending_type [[indeterminate]];
#else
            ::std::size_t start_index;
            ::std::size_t sublength;
            ::pltxt2htm::NodeType md_atx_heading_type;
            ::pltxt2htm::details::MdAtxEndingType ending_type;
#endif
            // try parsing markdown atx header
            if (current_index + 1 < pltext_size &&
                ::pltxt2htm::details::try_parse_md_atx_heading<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1), start_index,
                    sublength, md_atx_heading_type, ending_type)) {
                ::pltxt2htm::Ast subast{};
                current_index += start_index + 1;
                if (current_index < pltext_size) {
                    auto subtext =
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index, sublength);
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::MdAtxHeadingContext>(
                        subtext, md_atx_heading_type, ending_type));
                } else {
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::MdAtxHeadingContext>(
                        ::fast_io::u8string_view{}, md_atx_heading_type, ending_type));
                }
                goto restart;
            } else if (auto opt_len = ::pltxt2htm::details::try_parse_md_thematic_break<ndebug>(
                           ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1));
                       opt_len.has_value()) {
                auto&& [subast, forward_index] = opt_len.template value<ndebug>();
                for (auto&& node : subast) {
                    result.push_back(::std::move(node));
                }
                current_index += forward_index;
                continue;
            } else if (auto opt_code_fence = ::pltxt2htm::details::try_parse_md_code_fence<ndebug>(
                           ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1));
                       opt_code_fence.has_value()) {
                auto&& [node, forward_index] = opt_code_fence.template value<ndebug>();
                result.push_back(::std::move(node));
                current_index += forward_index;
            }
            continue;
        } else if (chr == u8' ') {
            // TODO should we delete tail space?
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Space>{});
            continue;
        } else if (chr == u8'&') {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Ampersand>{});
            continue;
        } else if (chr == u8'\'') {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::SingleQuotationMark>{});
            continue;
        } else if (chr == u8'\"') {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::DoubleQuotationMark>{});
            continue;
        } else if (chr == u8'>') {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::GreaterThan>{});
            continue;
        } else if (chr == u8'\t') {
            result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Tab>{});
            continue;
        } else if (chr == u8'\\') {
            if (current_index + 1 == pltext_size) {
                result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{u8'\\'});
                continue;
            }
            auto escape_node = ::pltxt2htm::details::switch_escape_char(
                ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 1));
            if (escape_node.has_value()) {
                result.push_back(::std::move(escape_node.template value<ndebug>()));
                ++current_index;
            } else {
                result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>{u8'\\'});
            }
            continue;
        } else if (chr == u8'<') {
            // if i is a valid value, i always less than pltext_size
            pltxt2htm_assert(current_index < pltext_size, u8"Index of parser out of bound");

            if (current_index + 1 == pltext_size) {
                result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                continue;
            }

            // a trie for tags
            switch (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 1)) {
            case u8'a':
                [[fallthrough]];
            case u8'A': {
                // parsing pl <a>$1</a> tag (not html <a> tag)
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_a));
                    goto restart;
                } else {
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'b':
                [[fallthrough]];
            case u8'B': {
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    // parsing pl&html <b> tag
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_b));
                    goto restart;
                } else if (auto opt_br_tag_len = ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, u8'r'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_br_tag_len.has_value()) {
                    current_index += opt_br_tag_len.template value<ndebug>() + 2;
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Br>(::pltxt2htm::Br()));

#if __has_cpp_attribute(indeterminate)
                    ::std::size_t start_index [[indeterminate]];
                    ::std::size_t end_index [[indeterminate]];
                    ::pltxt2htm::NodeType md_atx_heading_type [[indeterminate]];
                    ::pltxt2htm::details::MdAtxEndingType ending_type [[indeterminate]];
#else
                    ::std::size_t start_index;
                    ::std::size_t sublength;
                    ::pltxt2htm::NodeType md_atx_heading_type;
                    ::pltxt2htm::details::MdAtxEndingType ending_type;
#endif
                    // try parsing markdown atx header
                    if (current_index + 1 < pltext_size &&
                        ::pltxt2htm::details::try_parse_md_atx_heading<ndebug>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1), start_index,
                            sublength, md_atx_heading_type, ending_type)) {
                        ::pltxt2htm::Ast subast{};
                        current_index += start_index + 1;
                        if (current_index < pltext_size) {
                            auto subtext =
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index, sublength);
                            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::MdAtxHeadingContext>(
                                subtext, md_atx_heading_type, ending_type));
                        } else {
                            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::MdAtxHeadingContext>(
                                ::fast_io::u8string_view{}, md_atx_heading_type, ending_type));
                        }
                        goto restart;
                    } else if (auto opt_len = ::pltxt2htm::details::try_parse_md_thematic_break<ndebug>(
                                   ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1));
                               opt_len.has_value()) {
                        auto&& [subast, forward_index] = opt_len.template value<ndebug>();
                        for (auto&& node : subast) {
                            result.push_back(::std::move(node));
                        }
                        current_index += forward_index;
                        continue;
                    } else if (auto opt_code_fence = ::pltxt2htm::details::try_parse_md_code_fence<ndebug>(
                                   ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 1));
                               opt_code_fence.has_value()) {
                        auto&& [node, forward_index] = opt_code_fence.template value<ndebug>();
                        result.push_back(::std::move(node));
                        current_index += forward_index;
                    }
                    continue;
                } else {
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'c':
                [[fallthrough]];
            case u8'C': {
                // parsing: <color=$1>$2</color>
                if (auto opt_color_tag =
                        ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, u8'o', u8'l', u8'o', u8'r'>(
                            u8string_view_subview<ndebug>(pltext, current_index + 2),
                            [](char8_t u8chr) static constexpr noexcept {
                                return (u8'0' <= u8chr && u8chr <= u8'9') || (u8'a' <= u8chr && u8chr <= u8'z') ||
                                       (u8'A' <= u8chr && u8chr <= u8'Z') || u8chr == u8'#';
                            });
                    opt_color_tag.has_value()) {
                    auto&& [tag_len, color] = opt_color_tag.template value<ndebug>();
                    current_index += tag_len + 3;
                    // parsing start tag <color> successed
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::EqualSignTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_color, ::std::move(color)));
                    goto restart;
                } else if (auto opt_code_tag = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'o', u8'd', u8'e'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_code_tag.has_value()) {
                    // parsing html <code> tag
                    current_index += opt_code_tag.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_code));
                    goto restart;
                } else {
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'd':
                [[fallthrough]];
            case u8'D': {
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'e', u8'l'>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    // parsing <del>$1</del>
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_del));
                    goto restart;
                }
                // parsing: <discussion=$1>$2</discussion>
                if (auto opt_discussion_tag =
                        ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, u8'i', u8's', u8'c', u8'u', u8's', u8's',
                                                                       u8'i', u8'o', u8'n'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            [](char8_t u8chr) static constexpr noexcept {
                                return (u8'a' <= u8chr && u8chr <= u8'z') || (u8'0' <= u8chr && u8chr <= u8'9');
                            });
                    opt_discussion_tag.has_value()) {
                    auto&& [tag_len, id] = opt_discussion_tag.template value<ndebug>();
                    current_index += tag_len + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::EqualSignTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_discussion, ::std::move(id)));
                    goto restart;
                } else {
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'e':
                [[fallthrough]];
            case u8'E': {
                // parsing: <experiment=$1>$2</experiment>
                if (auto opt_experiment_tag =
                        ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, u8'x', u8'p', u8'e', u8'r', u8'i', u8'm',
                                                                       u8'e', u8'n', u8't'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                            [](char8_t u8chr) static constexpr noexcept {
                                return (u8'a' <= u8chr && u8chr <= u8'z') || (u8'0' <= u8chr && u8chr <= u8'9');
                            });
                    opt_experiment_tag.has_value()) {
                    auto&& [tag_len, id] = opt_experiment_tag.template value<ndebug>();
                    current_index += tag_len + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::EqualSignTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_experiment, ::std::move(id)));
                    goto restart;
                } else if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'm'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_em));
                    goto restart;
                } else {
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'h':
                [[fallthrough]];
            case u8'H': {
                if (auto opt_h1_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'1'>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_h1_tag_len.has_value()) {
                    // parsing html <h1> tag
                    current_index += opt_h1_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_h1));
                    goto restart;
                } else if (auto opt_h2_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'2'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_h2_tag_len.has_value()) {
                    // parsing html <h2> tag
                    current_index += opt_h2_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_h2));
                    goto restart;
                } else if (auto opt_h3_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'3'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_h3_tag_len.has_value()) {
                    // parsing html <h3> tag
                    current_index += opt_h3_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_h3));
                    goto restart;
                } else if (auto opt_h4_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'4'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_h4_tag_len.has_value()) {
                    // parsing html <h4> tag
                    current_index += opt_h4_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_h4));
                    goto restart;
                } else if (auto opt_h5_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'5'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_h5_tag_len.has_value()) {
                    // parsing html <h5> tag
                    current_index += opt_h5_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_h5));
                    goto restart;
                } else if (auto opt_h6_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'6'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_h6_tag_len.has_value()) {
                    // parsing html <h6> tag
                    current_index += opt_h6_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_h6));
                    goto restart;
                } else if (auto opt_tag_len = ::pltxt2htm::details::try_parse_self_closing_tag<ndebug, u8'r'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug>() + 2;
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Hr>());
                    continue;
                } else {
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'i':
                [[fallthrough]];
            case u8'I': {
                // parsing pl&html <i>$1</i> tag
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_i));
                    goto restart;
                } else {
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'l':
                [[fallthrough]];
            case u8'L': {
                if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'i'>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_li));
                    goto restart;
                } else {
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'p':
                [[fallthrough]];
            case u8'P': {
                // parsing html <p></p> tag
                if (auto opt_p_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                    opt_p_tag_len.has_value()) {
                    current_index += opt_p_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_p));
                    goto restart;
                } else if (auto opt_pre_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'r', u8'e'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_pre_tag_len.has_value()) {
                    current_index += opt_pre_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_pre));
                    goto restart;
                } else {
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8's':
                [[fallthrough]];
            case u8'S': {
                // parsing pl <size=$1>$2</size> tag
                if (auto opt_size_tag = ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, u8'i', u8'z', u8'e'>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        [](char8_t u8chr) static constexpr noexcept { return u8'0' <= u8chr && u8chr <= u8'9'; });
                    opt_size_tag.has_value()) {
                    auto&& [tag_len, id_] = opt_size_tag.template value<ndebug>();
                    ::exception::optional<::std::size_t> id{
                        ::pltxt2htm::details::u8str2size_t(::fast_io::mnp::os_c_str(id_))};
                    if (!id.has_value()) [[unlikely]] {
                        ::exception::unreachable<ndebug>();
                    }

                    current_index += tag_len + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::PlSizeTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_size, id.template value<ndebug>()));
                    goto restart;
                } else if (auto opt_tag_len =
                               ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8't', u8'r', u8'o', u8'n', u8'g'>(
                                   ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_tag_len.has_value()) {
                    // HTML <strong> tag
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_strong));
                    goto restart;
                } else {
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'u':
                [[fallthrough]];
            case u8'U': {
                // parsing pl <user=$1>$2</user> tag
                if (auto opt_user_tag = ::pltxt2htm::details::try_parse_equal_sign_tag<ndebug, u8's', u8'e', u8'r'>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2),
                        [](char8_t u8chr) static constexpr noexcept {
                            return (u8'a' <= u8chr && u8chr <= u8'z') || (u8'0' <= u8chr && u8chr <= u8'9');
                        });
                    opt_user_tag.has_value()) {
                    auto&& [tag_len, id] = opt_user_tag.template value<ndebug>();
                    current_index += tag_len + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::EqualSignTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::pl_user, ::std::move(id)));
                    goto restart;
                } else if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'l'>(
                               ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                           opt_tag_len.has_value()) {
                    current_index += opt_tag_len.template value<ndebug>() + 3;
                    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index),
                        ::pltxt2htm::NodeType::html_ul));
                    goto restart;
                } else {
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'!': {
                // parsing: <!--$1-->
                if (::pltxt2htm::details::is_prefix_match<ndebug, u8'-', u8'-'>(
                        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2))) {
                    // Find the closing -->
                    ::std::size_t comment_end{current_index + 4}; // Position after <!--
                    ::pltxt2htm::Ast subast{};

                    for (; comment_end < pltext_size; ++comment_end) {
                        if (::pltxt2htm::details::is_prefix_match<ndebug, u8'-', u8'-', u8'>'>(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, comment_end))) {
                            break;
                        }
                        subast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::U8Char>(
                            ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, comment_end)));
                    }

                    current_index = comment_end + 2; // Point to '>'
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Note>(::std::move(subast)));
                    continue;
                } else {
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
            }

            case u8'/': {
                switch (call_stack.top()->nested_tag_type) {
                case ::pltxt2htm::NodeType::pl_color: {
                    if (auto opt_tag_len =
                            ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'c', u8'o', u8'l', u8'o', u8'r'>(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </color> successed
                        auto frame =
                            static_cast<::pltxt2htm::details::EqualSignTagContext*>(call_stack.top().get_unsafe());
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Color staged_node(::std::move(result), ::std::move(frame->id));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::Color>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::pl_a: {
                    // parsing </a>
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'a'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </a> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::A staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::A>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::pl_experiment: {
                    // parsing </experiment>
                    if (auto opt_tag_len =
                            ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'e', u8'x', u8'p', u8'e', u8'r', u8'i',
                                                                     u8'm', u8'e', u8'n', u8't'>(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // Whether or not extern_index is out of range, extern for loop will handle it correctly.
                        auto frame =
                            static_cast<::pltxt2htm::details::EqualSignTagContext*>(call_stack.top().get_unsafe());
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Experiment staged_node(::std::move(result), ::std::move(frame->id));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::Experiment>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::pl_discussion: {
                    // parsing </discussion>
                    if (auto opt_tag_len =
                            ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'd', u8'i', u8's', u8'c', u8'u', u8's',
                                                                     u8's', u8'i', u8'o', u8'n'>(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // Whether or not extern_index is out of range, extern for loop will handle it correctly.
                        auto frame =
                            static_cast<::pltxt2htm::details::EqualSignTagContext*>(call_stack.top().get_unsafe());
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Discussion staged_node(::std::move(result), ::std::move(frame->id));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::Discussion>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::pl_user: {
                    // parsing </user>
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'u', u8's', u8'e', u8'r'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        auto frame =
                            static_cast<::pltxt2htm::details::EqualSignTagContext*>(call_stack.top().get_unsafe());
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::User staged_node(::std::move(result), ::std::move(frame->id));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::User>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::pl_size: {
                    // parsing </size>
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8's', u8'i', u8'z', u8'e'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        auto frame =
                            static_cast<::pltxt2htm::details::PlSizeTagContext const*>(call_stack.top().release_imul());
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Size staged_node(::std::move(result), frame->id);
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::Size>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::pl_b: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'b'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </b> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::B staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::B>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::pl_i: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'i'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </a> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::I staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::I>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_p: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'p'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </p> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::P staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::P>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_h1: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'h', u8'1'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </h1> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::H1 staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::H1>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_h2: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'h', u8'2'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </h2> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::H2 staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::H2>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_h3: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'h', u8'3'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </h3> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::H3 staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::H3>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_h4: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'h', u8'4'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </h4> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::H4 staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::H4>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_h5: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'h', u8'5'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </h5> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::H5 staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::H5>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_h6: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'h', u8'6'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </h6> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::H6 staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::H6>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_del: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'd', u8'e', u8'l'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </del> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Del staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::Del>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_note: {
                    ::exception::unreachable<ndebug>();
                }
                case ::pltxt2htm::NodeType::html_em: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'e', u8'm'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </em> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Em staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::Em>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_strong: {
                    if (auto opt_tag_len =
                            ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8's', u8't', u8'r', u8'o', u8'n', u8'g'>(
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </strong> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Strong staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::Strong>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_ul: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'u', u8'l'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </ul> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Ul staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::Ul>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_li: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'l', u8'i'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </li> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Li staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::Li>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_code: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'c', u8'o', u8'd', u8'e'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </code> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Code staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::Code>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                case ::pltxt2htm::NodeType::html_pre: {
                    if (auto opt_tag_len = ::pltxt2htm::details::try_parse_bare_tag<ndebug, u8'p', u8'r', u8'e'>(
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index + 2));
                        opt_tag_len.has_value()) {
                        // parsing end tag </pre> successed
                        ::std::size_t const staged_index{current_index};
                        ::pltxt2htm::Pre staged_node(::std::move(result));
                        call_stack.pop();
                        call_stack.top()->subast.push_back(
                            ::pltxt2htm::details::HeapGuard<::pltxt2htm::Pre>(::std::move(staged_node)));
                        call_stack.top()->current_index += staged_index + opt_tag_len.template value<ndebug>() + 3;
                        goto restart;
                    } else {
                        result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                        continue;
                    }
                }
                default:
                    result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                    continue;
                }
                ::exception::unreachable<ndebug>();
            }

            default: {
                result.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LessThan>{});
                continue;
            }
            }

            ::exception::unreachable<ndebug>();
        } else {
            auto&& [subast, forward_index] = ::pltxt2htm::details::parse_utf8_code_point<ndebug>(
                ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
            current_index += forward_index;
            for (auto&& node : subast) {
                result.push_back(::std::move(node));
            }
            continue;
        }
    }

    {
        ::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BasicFrameContext> frame(::std::move(call_stack.top()));
        ::std::size_t const staged_index = pltext_size;
        call_stack.pop();
        if (call_stack.empty()) {
            // Considering the following markdown:
            // ```md
            // <b>e</b>xample
            // ```
            // Text without any tag in the end will hit this branch.
            return ::std::move(frame->subast);
        } else {
            // Considering the following markdown:
            // ```md
            // <b>example
            // ```
            // Any tag without a closing tag will hit this branch.
            auto&& subast = frame->subast;
            auto&& superast = call_stack.top()->subast;
            auto&& super_index = call_stack.top()->current_index;
            switch (frame->nested_tag_type) {
            case ::pltxt2htm::NodeType::pl_color: {
                auto&& id = static_cast<::pltxt2htm::details::EqualSignTagContext*>(frame.get_unsafe())->id;
                superast.push_back(
                    ::pltxt2htm::details::HeapGuard<::pltxt2htm::Color>(::std::move(subast), ::std::move(id)));
                break;
            }
            case ::pltxt2htm::NodeType::pl_a: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::A>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::pl_experiment: {
                auto&& id = static_cast<::pltxt2htm::details::EqualSignTagContext*>(frame.get_unsafe())->id;
                superast.push_back(
                    ::pltxt2htm::details::HeapGuard<::pltxt2htm::Experiment>(::std::move(subast), ::std::move(id)));
                break;
            }
            case ::pltxt2htm::NodeType::pl_discussion: {
                auto&& id = static_cast<::pltxt2htm::details::EqualSignTagContext*>(frame.get_unsafe())->id;
                superast.push_back(
                    ::pltxt2htm::details::HeapGuard<::pltxt2htm::Discussion>(::std::move(subast), ::std::move(id)));
                break;
            }
            case ::pltxt2htm::NodeType::pl_user: {
                auto&& id = static_cast<::pltxt2htm::details::EqualSignTagContext*>(frame.get_unsafe())->id;
                superast.push_back(
                    ::pltxt2htm::details::HeapGuard<::pltxt2htm::User>(::std::move(subast), ::std::move(id)));
                break;
            }
            case ::pltxt2htm::NodeType::pl_size: {
                auto&& id = static_cast<::pltxt2htm::details::PlSizeTagContext const*>(frame.release_imul())->id;
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Size>(::std::move(subast), id));
                break;
            }
            case ::pltxt2htm::NodeType::html_strong:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_b: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::B>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::pl_i: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::I>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::html_p: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::P>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::html_h1: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::H1>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::html_note: {
                ::exception::unreachable<ndebug>();
            }
            case ::pltxt2htm::NodeType::html_h2: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::H2>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::html_h3: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::H3>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::html_h4: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::H4>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::html_h5: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::H5>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::html_h6: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::H6>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::html_del: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Del>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::html_em: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Em>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::html_ul: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Ul>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::html_li: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Li>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::html_code: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Code>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::html_pre: {
                superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Pre>(::std::move(subast)));
                break;
            }
            case ::pltxt2htm::NodeType::md_atx_h1:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_atx_h2:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_atx_h3:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_atx_h4:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_atx_h5:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_atx_h6: {
                switch (frame->nested_tag_type) {
                case ::pltxt2htm::NodeType::md_atx_h1:
                    superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::MdAtxH1>(::std::move(subast)));
                    break;
                case ::pltxt2htm::NodeType::md_atx_h2:
                    superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::MdAtxH2>(::std::move(subast)));
                    break;
                case ::pltxt2htm::NodeType::md_atx_h3:
                    superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::MdAtxH3>(::std::move(subast)));
                    break;
                case ::pltxt2htm::NodeType::md_atx_h4:
                    superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::MdAtxH4>(::std::move(subast)));
                    break;
                case ::pltxt2htm::NodeType::md_atx_h5:
                    superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::MdAtxH5>(::std::move(subast)));
                    break;
                case ::pltxt2htm::NodeType::md_atx_h6:
                    superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::MdAtxH6>(::std::move(subast)));
                    break;
                default:
                    [[unlikely]] {
                        ::exception::unreachable<ndebug>();
                    }
                }
                super_index += frame->subast.size();
                // Handle the ending type
                auto&& ending_type =
                    static_cast<::pltxt2htm::details::MdAtxHeadingContext*>(frame.get_unsafe())->ending_type;
                if (ending_type.ending_type == ::pltxt2htm::details::MdAtxHeadingEndingType::newline) {
                    superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::LineBreak>{});
                    super_index += 1;
                } else if (ending_type.ending_type == ::pltxt2htm::details::MdAtxHeadingEndingType::br_tag) {
                    superast.push_back(::pltxt2htm::details::HeapGuard<::pltxt2htm::Br>{});
                    super_index += ending_type.br_len + 1;
                }
                break;
            }
            default:
                [[unlikely]] {
                    ::exception::unreachable<ndebug>();
                }
            }
            super_index += staged_index;
            goto restart;
        }
    }
}

} // namespace details

/**
 * @brief Impl of parse pl-text to nodes.
 * @tparam ndebug: Whether or not to disable debugging checks (like NDEBUG macro).
 * @param pltext: The text readed from Quantum-Physics.
 */
template<bool ndebug>
[[nodiscard]]
constexpr auto parse_pltxt(::fast_io::u8string_view pltext) /* throws */ -> ::pltxt2htm::Ast {
    // fast_io::deque contains bug about RAII, use fast_io::list instead
    ::fast_io::stack<::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BasicFrameContext>,
                     ::fast_io::list<::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BasicFrameContext>>>
        call_stack{};
    ::pltxt2htm::Ast result{};

    ::std::size_t start_index{};
#if __has_cpp_attribute(indeterminate)
    ::std::size_t end_index [[indeterminate]];
    ::pltxt2htm::NodeType md_atx_heading_type [[indeterminate]];
    ::pltxt2htm::details::MdAtxEndingType ending_type [[indeterminate]];
#else
    ::std::size_t sublength;
    ::pltxt2htm::NodeType md_atx_heading_type;
    ::pltxt2htm::details::MdAtxEndingType ending_type;
#endif
    // try parsing markdown atx header
    if (::pltxt2htm::details::try_parse_md_atx_heading<ndebug>(pltext, start_index, sublength, md_atx_heading_type,
                                                               ending_type)) {
        // Consider the following markdown
        // ```md
        // ## test
        // ```
        // Here, the first line is a markdown atx heading, will hit this case
        ::pltxt2htm::Ast subast{};
        if (start_index < pltext.size()) {
            auto subtext = ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, start_index, sublength);
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::MdAtxHeadingContext>(
                subtext, md_atx_heading_type, ending_type));
            subast = ::pltxt2htm::details::parse_pltxt<ndebug>(call_stack);
        }
        result.push_back(::pltxt2htm::details::switch_md_atx_header<ndebug>(md_atx_heading_type, ::std::move(subast)));
        // rectify the start index to the start of next text (aka. below common cases)
        start_index += sublength;
    } else if (auto opt_thematic_break = ::pltxt2htm::details::try_parse_md_thematic_break<ndebug>(pltext);
               opt_thematic_break.has_value()) {
        // considering following markdown
        // ```md
        // ---
        // ```
        // above example will hit this branch
        auto&& [subast, forward_index] = opt_thematic_break.template value<ndebug>();
        for (auto&& node : subast) {
            result.push_back(::std::move(node));
        }
        start_index += forward_index;
    } else if (auto opt_code_fence = ::pltxt2htm::details::try_parse_md_code_fence<ndebug>(pltext);
               opt_code_fence.has_value()) {
        auto&& [node, forward_index] = opt_code_fence.template value<ndebug>();
        result.push_back(::std::move(node));
        start_index += forward_index;
    }

    // other common cases
    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BareTagContext>(
        ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, start_index), ::pltxt2htm::NodeType::base));
    auto subast = ::pltxt2htm::details::parse_pltxt<ndebug>(call_stack);
    for (auto&& node : subast) {
        result.push_back(::std::move(node));
    }

    pltxt2htm_assert(call_stack.empty(), u8"call_stack is not empty");

    return result;
}

} // namespace pltxt2htm

#include "pop_macro.hh"
