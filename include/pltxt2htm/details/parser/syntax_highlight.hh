/**
 * @file syntax_highlight.hh
 * @brief Shared language and token definitions for fenced-code parsing.
 */

#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../../contracts.hh"
#include "../utils.hh"
#include "../push_macro.hh"

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

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto resolve_syntax_language(::fast_io::u8string_view const language) noexcept -> SyntaxLanguage {
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"cpp">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"c++">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"cc">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"cxx">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"hpp">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"h++">(language)) {
        return SyntaxLanguage::cpp;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"rust">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"rs">(language)) {
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

[[nodiscard]]
constexpr bool syntax_is_identifier_start(char8_t const chr) noexcept {
    return ::pltxt2htm::details::is_ascii_alpha(chr) || chr == u8'_';
}

[[nodiscard]]
constexpr bool syntax_is_identifier_continue(char8_t const chr) noexcept {
    return ::pltxt2htm::details::syntax_is_identifier_start(chr) || ::pltxt2htm::details::is_ascii_digit(chr);
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

template<::std::size_t keyword_count>
[[nodiscard]]
constexpr auto syntax_is_keyword(::fast_io::u8string_view const identifier,
                                 ::fast_io::u8string_view const (&keywords)[keyword_count]) noexcept -> bool {
    for (auto const keyword : keywords) {
        if (identifier == keyword) {
            return true;
        }
    }
    return false;
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
