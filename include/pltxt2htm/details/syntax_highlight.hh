/**
 * @file syntax_highlight.hh
 * @brief Built-in syntax highlighting for fenced C++ and Rust code.
 */

#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <fast_io/fast_io_dsal/vector.h>
#include "../ast/ast.hh"
#include "../contracts.hh"
#include "utils.hh"
#include "push_macro.hh"

namespace pltxt2htm::details {

enum class SyntaxLanguage : unsigned {
    plain = 0,
    cpp,
    rust,
};

enum class SyntaxTokenKind : unsigned {
    keyword = 0,
    string,
    number,
    comment,
    function,
    macro,
    preprocessor,
};

class SyntaxTokenSpan {
public:
    ::std::size_t begin{};
    ::std::size_t end{};
    SyntaxTokenKind kind{};
};

[[nodiscard]]
constexpr auto syntax_ascii_lower(char8_t const chr) noexcept -> char8_t {
    if (u8'A' <= chr && chr <= u8'Z') {
        return static_cast<char8_t>(chr + (u8'a' - u8'A'));
    }
    return chr;
}

[[nodiscard]]
constexpr auto syntax_language_name_equals(::fast_io::u8string_view const language,
                                           ::fast_io::u8string_view const expected) noexcept -> bool {
    if (language.size() != expected.size()) {
        return false;
    }
    for (::std::size_t index{}; index < language.size(); ++index) {
        if (::pltxt2htm::details::syntax_ascii_lower(language[index]) != expected[index]) {
            return false;
        }
    }
    return true;
}

[[nodiscard]]
constexpr auto resolve_syntax_language(::fast_io::u8string_view const language) noexcept -> SyntaxLanguage {
    if (::pltxt2htm::details::syntax_language_name_equals(language, u8"cpp") ||
        ::pltxt2htm::details::syntax_language_name_equals(language, u8"c++") ||
        ::pltxt2htm::details::syntax_language_name_equals(language, u8"cc") ||
        ::pltxt2htm::details::syntax_language_name_equals(language, u8"cxx") ||
        ::pltxt2htm::details::syntax_language_name_equals(language, u8"hpp") ||
        ::pltxt2htm::details::syntax_language_name_equals(language, u8"h++")) {
        return SyntaxLanguage::cpp;
    }
    if (::pltxt2htm::details::syntax_language_name_equals(language, u8"rust") ||
        ::pltxt2htm::details::syntax_language_name_equals(language, u8"rs")) {
        return SyntaxLanguage::rust;
    }
    return SyntaxLanguage::plain;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto syntax_token_color(SyntaxTokenKind const kind) noexcept -> ::fast_io::u8string_view {
    switch (kind) {
    case SyntaxTokenKind::keyword:
        return u8"#cf222e";
    case SyntaxTokenKind::string:
        return u8"#0a3069";
    case SyntaxTokenKind::number:
        return u8"#0550ae";
    case SyntaxTokenKind::comment:
        return u8"#6e7781";
    case SyntaxTokenKind::function:
        return u8"#8250df";
    case SyntaxTokenKind::macro:
        return u8"#cf222e";
    case SyntaxTokenKind::preprocessor:
        return u8"#0550ae";
    }
    pltxt2htm_unreachable(u8"Unexpected syntax token kind");
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto syntax_node_ascii(::pltxt2htm::PlTxtNode<ndebug> const& node) noexcept -> char8_t {
    switch (node.get_node_kind()) {
    case ::pltxt2htm::NodeKind::u8char:
        return node.as_u8char().chr;
    case ::pltxt2htm::NodeKind::line_break:
        return u8'\n';
    case ::pltxt2htm::NodeKind::space:
        return u8' ';
    case ::pltxt2htm::NodeKind::ampersand:
    case ::pltxt2htm::NodeKind::md_escape_ampersand:
        return u8'&';
    case ::pltxt2htm::NodeKind::double_quote:
    case ::pltxt2htm::NodeKind::md_escape_double_quote:
        return u8'"';
    case ::pltxt2htm::NodeKind::single_quote:
    case ::pltxt2htm::NodeKind::md_escape_single_quote:
        return u8'\'';
    case ::pltxt2htm::NodeKind::less_than:
    case ::pltxt2htm::NodeKind::md_escape_less_than:
        return u8'<';
    case ::pltxt2htm::NodeKind::greater_than:
    case ::pltxt2htm::NodeKind::md_escape_greater_than:
        return u8'>';
    case ::pltxt2htm::NodeKind::tab:
        return u8'\t';
    case ::pltxt2htm::NodeKind::md_escape_backslash:
        return u8'\\';
    case ::pltxt2htm::NodeKind::md_escape_exclamation:
        return u8'!';
    case ::pltxt2htm::NodeKind::md_escape_hash:
        return u8'#';
    case ::pltxt2htm::NodeKind::md_escape_dollar:
        return u8'$';
    case ::pltxt2htm::NodeKind::md_escape_percent:
        return u8'%';
    case ::pltxt2htm::NodeKind::md_escape_left_paren:
        return u8'(';
    case ::pltxt2htm::NodeKind::md_escape_right_paren:
        return u8')';
    case ::pltxt2htm::NodeKind::md_escape_asterisk:
        return u8'*';
    case ::pltxt2htm::NodeKind::md_escape_plus:
        return u8'+';
    case ::pltxt2htm::NodeKind::md_escape_comma:
        return u8',';
    case ::pltxt2htm::NodeKind::md_escape_hyphen:
        return u8'-';
    case ::pltxt2htm::NodeKind::md_escape_dot:
        return u8'.';
    case ::pltxt2htm::NodeKind::md_escape_slash:
        return u8'/';
    case ::pltxt2htm::NodeKind::md_escape_colon:
        return u8':';
    case ::pltxt2htm::NodeKind::md_escape_semicolon:
        return u8';';
    case ::pltxt2htm::NodeKind::md_escape_equals:
        return u8'=';
    case ::pltxt2htm::NodeKind::md_escape_question:
        return u8'?';
    case ::pltxt2htm::NodeKind::md_escape_at:
        return u8'@';
    case ::pltxt2htm::NodeKind::md_escape_left_bracket:
        return u8'[';
    case ::pltxt2htm::NodeKind::md_escape_right_bracket:
        return u8']';
    case ::pltxt2htm::NodeKind::md_escape_caret:
        return u8'^';
    case ::pltxt2htm::NodeKind::md_escape_underscore:
        return u8'_';
    case ::pltxt2htm::NodeKind::md_escape_backtick:
        return u8'`';
    case ::pltxt2htm::NodeKind::md_escape_left_brace:
        return u8'{';
    case ::pltxt2htm::NodeKind::md_escape_pipe:
        return u8'|';
    case ::pltxt2htm::NodeKind::md_escape_right_brace:
        return u8'}';
    case ::pltxt2htm::NodeKind::md_escape_tilde:
        return u8'~';
    default:
        return char8_t{};
    }
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto syntax_ast_ascii(::pltxt2htm::Ast<ndebug> const& ast, ::std::size_t const index) noexcept -> char8_t {
    return ::pltxt2htm::details::syntax_node_ascii<ndebug>(::pltxt2htm::details::vector_index<ndebug>(ast, index));
}

[[nodiscard]]
constexpr bool syntax_is_identifier_start(char8_t const chr) noexcept {
    return ::pltxt2htm::details::is_ascii_alpha(chr) || chr == u8'_';
}

[[nodiscard]]
constexpr bool syntax_is_identifier_continue(char8_t const chr) noexcept {
    return ::pltxt2htm::details::syntax_is_identifier_start(chr) || ::pltxt2htm::details::is_ascii_digit(chr);
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto syntax_range_equals(::pltxt2htm::Ast<ndebug> const& ast, ::std::size_t const begin,
                                   ::std::size_t const end, ::fast_io::u8string_view const expected) noexcept -> bool {
    if (end - begin != expected.size()) {
        return false;
    }
    for (::std::size_t index{}; index < expected.size(); ++index) {
        if (::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, begin + index) != expected[index]) {
            return false;
        }
    }
    return true;
}

constexpr ::fast_io::u8string_view cpp_keywords[]{
    u8"alignas",
    u8"alignof",
    u8"and",
    u8"and_eq",
    u8"asm",
    u8"auto",
    u8"bitand",
    u8"bitor",
    u8"bool",
    u8"break",
    u8"case",
    u8"catch",
    u8"char",
    u8"char8_t",
    u8"char16_t",
    u8"char32_t",
    u8"class",
    u8"compl",
    u8"concept",
    u8"const",
    u8"consteval",
    u8"constexpr",
    u8"constinit",
    u8"const_cast",
    u8"continue",
    u8"co_await",
    u8"co_return",
    u8"co_yield",
    u8"decltype",
    u8"default",
    u8"delete",
    u8"do",
    u8"double",
    u8"dynamic_cast",
    u8"else",
    u8"enum",
    u8"explicit",
    u8"export",
    u8"extern",
    u8"false",
    u8"float",
    u8"for",
    u8"friend",
    u8"goto",
    u8"if",
    u8"import",
    u8"inline",
    u8"int",
    u8"long",
    u8"module",
    u8"mutable",
    u8"namespace",
    u8"new",
    u8"noexcept",
    u8"not",
    u8"not_eq",
    u8"nullptr",
    u8"operator",
    u8"or",
    u8"or_eq",
    u8"private",
    u8"protected",
    u8"public",
    u8"register",
    u8"reinterpret_cast",
    u8"requires",
    u8"return",
    u8"short",
    u8"signed",
    u8"sizeof",
    u8"static",
    u8"static_assert",
    u8"static_cast",
    u8"struct",
    u8"switch",
    u8"template",
    u8"this",
    u8"thread_local",
    u8"throw",
    u8"true",
    u8"try",
    u8"typedef",
    u8"typeid",
    u8"typename",
    u8"union",
    u8"unsigned",
    u8"using",
    u8"virtual",
    u8"void",
    u8"volatile",
    u8"wchar_t",
    u8"while",
    u8"xor",
    u8"xor_eq",
};

constexpr ::fast_io::u8string_view rust_keywords[]{
    u8"Self",     u8"abstract", u8"as",      u8"async", u8"await",    u8"become", u8"box",    u8"break", u8"const",
    u8"continue", u8"crate",    u8"do",      u8"dyn",   u8"else",     u8"enum",   u8"extern", u8"false", u8"final",
    u8"fn",       u8"for",      u8"gen",     u8"if",    u8"impl",     u8"in",     u8"let",    u8"loop",  u8"macro",
    u8"match",    u8"mod",      u8"move",    u8"mut",   u8"override", u8"priv",   u8"pub",    u8"ref",   u8"return",
    u8"self",     u8"static",   u8"struct",  u8"super", u8"trait",    u8"true",   u8"try",    u8"type",  u8"typeof",
    u8"union",    u8"unsafe",   u8"unsized", u8"use",   u8"virtual",  u8"where",  u8"while",  u8"yield", u8"bool",
    u8"char",     u8"f32",      u8"f64",     u8"i8",    u8"i16",      u8"i32",    u8"i64",    u8"i128",  u8"isize",
    u8"str",      u8"u8",       u8"u16",     u8"u32",   u8"u64",      u8"u128",   u8"usize",
};

template<::pltxt2htm::Contracts ndebug, ::std::size_t keyword_count>
[[nodiscard]]
constexpr auto syntax_is_keyword(::pltxt2htm::Ast<ndebug> const& ast, ::std::size_t const begin,
                                 ::std::size_t const end,
                                 ::fast_io::u8string_view const (&keywords)[keyword_count]) noexcept -> bool {
    for (auto const keyword : keywords) {
        if (::pltxt2htm::details::syntax_range_equals<ndebug>(ast, begin, end, keyword)) {
            return true;
        }
    }
    return false;
}

constexpr void append_syntax_span(::fast_io::vector<SyntaxTokenSpan>& spans, ::std::size_t const begin,
                                  ::std::size_t const end, SyntaxTokenKind const kind) noexcept {
    spans.push_back(SyntaxTokenSpan{.begin = begin, .end = end, .kind = kind});
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto syntax_skip_horizontal_space(::pltxt2htm::Ast<ndebug> const& ast, ::std::size_t index) noexcept
    -> ::std::size_t {
    ::std::size_t const size{ast.size()};
    while (index < size) {
        char8_t const chr{::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index)};
        if (chr != u8' ' && chr != u8'\t') {
            break;
        }
        ++index;
    }
    return index;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto syntax_consume_quoted(::pltxt2htm::Ast<ndebug> const& ast, ::std::size_t index,
                                     char8_t const quote) noexcept -> ::std::size_t {
    ::std::size_t const size{ast.size()};
    ++index;
    while (index < size) {
        char8_t const chr{::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index)};
        if (chr == u8'\\' && index + 1 < size) {
            index += 2;
            continue;
        }
        ++index;
        if (chr == quote) {
            break;
        }
        if (chr == u8'\n' && quote == u8'\'') {
            break;
        }
    }
    return index;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto syntax_consume_number(::pltxt2htm::Ast<ndebug> const& ast, ::std::size_t index) noexcept
    -> ::std::size_t {
    ::std::size_t const size{ast.size()};
    while (index < size) {
        char8_t const chr{::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index)};
        if (::pltxt2htm::details::syntax_is_identifier_continue(chr) || chr == u8'.' || chr == u8'\'') {
            ++index;
            continue;
        }
        break;
    }
    return index;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto highlight_cpp_syntax(::pltxt2htm::Ast<ndebug> const& ast) noexcept
    -> ::fast_io::vector<SyntaxTokenSpan> {
    ::fast_io::vector<SyntaxTokenSpan> spans{};
    ::std::size_t const size{ast.size()};
    for (::std::size_t index{}; index < size;) {
        char8_t const chr{::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index)};
        if (chr == u8'/' && index + 1 < size) {
            char8_t const next{::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index + 1)};
            if (next == u8'/') {
                ::std::size_t const begin{index};
                index += 2;
                while (index < size && ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index) != u8'\n') {
                    ++index;
                }
                ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::comment);
                continue;
            }
            if (next == u8'*') {
                ::std::size_t const begin{index};
                index += 2;
                while (index < size) {
                    if (index + 1 < size && ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index) == u8'*' &&
                        ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index + 1) == u8'/') {
                        index += 2;
                        break;
                    }
                    ++index;
                }
                ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::comment);
                continue;
            }
        }
        if (chr == u8'"' || chr == u8'\'') {
            ::std::size_t const begin{index};
            index = ::pltxt2htm::details::syntax_consume_quoted<ndebug>(ast, index, chr);
            ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::string);
            continue;
        }
        if (::pltxt2htm::details::is_ascii_digit(chr)) {
            ::std::size_t const begin{index};
            index = ::pltxt2htm::details::syntax_consume_number<ndebug>(ast, index);
            ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::number);
            continue;
        }
        if (chr == u8'#') {
            ::std::size_t const begin{index};
            index = ::pltxt2htm::details::syntax_skip_horizontal_space<ndebug>(ast, index + 1);
            while (index < size && ::pltxt2htm::details::syntax_is_identifier_continue(
                                       ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index))) {
                ++index;
            }
            ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::preprocessor);
            continue;
        }
        if (::pltxt2htm::details::syntax_is_identifier_start(chr)) {
            ::std::size_t const begin{index};
            do {
                ++index;
            } while (index < size && ::pltxt2htm::details::syntax_is_identifier_continue(
                                         ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index)));
            if (::pltxt2htm::details::syntax_is_keyword<ndebug>(ast, begin, index,
                                                                ::pltxt2htm::details::cpp_keywords)) {
                ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::keyword);
                continue;
            }
            ::std::size_t const next{::pltxt2htm::details::syntax_skip_horizontal_space<ndebug>(ast, index)};
            if (next < size && ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, next) == u8'(') {
                ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::function);
            }
            continue;
        }
        ++index;
    }
    return spans;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto syntax_consume_rust_raw_string(::pltxt2htm::Ast<ndebug> const& ast, ::std::size_t const begin,
                                              ::std::size_t quote_index) noexcept -> ::std::size_t {
    ::std::size_t const size{ast.size()};
    ::std::size_t const hash_count{quote_index - begin - 1};
    ++quote_index;
    while (quote_index < size) {
        if (::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, quote_index) != u8'"') {
            ++quote_index;
            continue;
        }
        ::std::size_t index{quote_index + 1};
        ::std::size_t hashes{};
        while (hashes < hash_count && index < size &&
               ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index) == u8'#') {
            ++hashes;
            ++index;
        }
        if (hashes == hash_count) {
            return index;
        }
        ++quote_index;
    }
    return size;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto highlight_rust_syntax(::pltxt2htm::Ast<ndebug> const& ast) noexcept
    -> ::fast_io::vector<SyntaxTokenSpan> {
    ::fast_io::vector<SyntaxTokenSpan> spans{};
    ::std::size_t const size{ast.size()};
    for (::std::size_t index{}; index < size;) {
        char8_t const chr{::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index)};
        if (chr == u8'/' && index + 1 < size) {
            char8_t const next{::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index + 1)};
            if (next == u8'/') {
                ::std::size_t const begin{index};
                index += 2;
                while (index < size && ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index) != u8'\n') {
                    ++index;
                }
                ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::comment);
                continue;
            }
            if (next == u8'*') {
                ::std::size_t const begin{index};
                index += 2;
                ::std::size_t depth{1};
                while (index < size && depth != 0) {
                    if (index + 1 < size && ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index) == u8'/' &&
                        ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index + 1) == u8'*') {
                        ++depth;
                        index += 2;
                    }
                    else if (index + 1 < size && ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index) == u8'*' &&
                             ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index + 1) == u8'/') {
                        --depth;
                        index += 2;
                    }
                    else {
                        ++index;
                    }
                }
                ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::comment);
                continue;
            }
        }
        if (chr == u8'r') {
            ::std::size_t quote_index{index + 1};
            while (quote_index < size && ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, quote_index) == u8'#') {
                ++quote_index;
            }
            if (quote_index < size && ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, quote_index) == u8'"') {
                ::std::size_t const begin{index};
                index = ::pltxt2htm::details::syntax_consume_rust_raw_string<ndebug>(ast, begin, quote_index);
                ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::string);
                continue;
            }
        }
        if (chr == u8'"') {
            ::std::size_t const begin{index};
            index = ::pltxt2htm::details::syntax_consume_quoted<ndebug>(ast, index, chr);
            ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::string);
            continue;
        }
        if (chr == u8'\'') {
            ::std::size_t const begin{index};
            ::std::size_t identifier_end{index + 1};
            while (identifier_end < size && ::pltxt2htm::details::syntax_is_identifier_continue(
                                                ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, identifier_end))) {
                ++identifier_end;
            }
            if (identifier_end < size &&
                ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, identifier_end) == u8'\'') {
                index = identifier_end + 1;
                ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::string);
            }
            else if (identifier_end != index + 1) {
                index = identifier_end;
                ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::keyword);
            }
            else {
                index = ::pltxt2htm::details::syntax_consume_quoted<ndebug>(ast, index, chr);
                ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::string);
            }
            continue;
        }
        if (::pltxt2htm::details::is_ascii_digit(chr)) {
            ::std::size_t const begin{index};
            index = ::pltxt2htm::details::syntax_consume_number<ndebug>(ast, index);
            ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::number);
            continue;
        }
        if (::pltxt2htm::details::syntax_is_identifier_start(chr)) {
            ::std::size_t const begin{index};
            do {
                ++index;
            } while (index < size && ::pltxt2htm::details::syntax_is_identifier_continue(
                                         ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, index)));
            if (::pltxt2htm::details::syntax_is_keyword<ndebug>(ast, begin, index,
                                                                ::pltxt2htm::details::rust_keywords)) {
                ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::keyword);
                continue;
            }
            ::std::size_t const next{::pltxt2htm::details::syntax_skip_horizontal_space<ndebug>(ast, index)};
            if (next < size && ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, next) == u8'!') {
                ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::macro);
            }
            else if (next < size && ::pltxt2htm::details::syntax_ast_ascii<ndebug>(ast, next) == u8'(') {
                ::pltxt2htm::details::append_syntax_span(spans, begin, index, SyntaxTokenKind::function);
            }
            continue;
        }
        ++index;
    }
    return spans;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto highlight_syntax(::pltxt2htm::Ast<ndebug> const& ast, SyntaxLanguage const language) noexcept
    -> ::fast_io::vector<SyntaxTokenSpan> {
    switch (language) {
    case SyntaxLanguage::plain:
        return {};
    case SyntaxLanguage::cpp:
        return ::pltxt2htm::details::highlight_cpp_syntax<ndebug>(ast);
    case SyntaxLanguage::rust:
        return ::pltxt2htm::details::highlight_rust_syntax<ndebug>(ast);
    }
    pltxt2htm_unreachable(u8"Unexpected syntax language");
}

} // namespace pltxt2htm::details

#include "pop_macro.hh"
