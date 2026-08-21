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
    bash,
    c,
    cpp,
    csharp,
    css,
    go,
    html,
    java,
    javascript,
    json,
    kotlin,
    lua,
    python,
    rust,
    sql,
    toml,
    typescript,
    xml,
    yaml,
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
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"bash">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"sh">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"shell">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"zsh">(language)) {
        return SyntaxLanguage::bash;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"c">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"h">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"c23">(language)) {
        return SyntaxLanguage::c;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"cpp">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"c++">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"cc">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"cxx">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"hpp">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"h++">(language)) {
        return SyntaxLanguage::cpp;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"cs">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"csharp">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"c#">(language)) {
        return SyntaxLanguage::csharp;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"css">(language)) {
        return SyntaxLanguage::css;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"go">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"golang">(language)) {
        return SyntaxLanguage::go;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"html">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"htm">(language)) {
        return SyntaxLanguage::html;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"java">(language)) {
        return SyntaxLanguage::java;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"js">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"javascript">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"mjs">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"cjs">(language)) {
        return SyntaxLanguage::javascript;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"json">(language)) {
        return SyntaxLanguage::json;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"kotlin">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"kt">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"kts">(language)) {
        return SyntaxLanguage::kotlin;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"rust">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"rs">(language)) {
        return SyntaxLanguage::rust;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"lua">(language)) {
        return SyntaxLanguage::lua;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"py">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"python">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"python3">(language)) {
        return SyntaxLanguage::python;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"sql">(language)) {
        return SyntaxLanguage::sql;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"toml">(language)) {
        return SyntaxLanguage::toml;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"ts">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"typescript">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"mts">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"cts">(language)) {
        return SyntaxLanguage::typescript;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"yaml">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"yml">(language)) {
        return SyntaxLanguage::yaml;
    }
    if (::pltxt2htm::details::is_exact_match<ndebug, u8"xml">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"xhtml">(language) ||
        ::pltxt2htm::details::is_exact_match<ndebug, u8"svg">(language)) {
        return SyntaxLanguage::xml;
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

constexpr ::fast_io::u8string_view c23_keywords[]{
    u8"alignas",
    u8"alignof",
    u8"auto",
    u8"bool",
    u8"break",
    u8"case",
    u8"char",
    u8"const",
    u8"constexpr",
    u8"continue",
    u8"default",
    u8"do",
    u8"double",
    u8"else",
    u8"enum",
    u8"extern",
    u8"false",
    u8"float",
    u8"for",
    u8"goto",
    u8"if",
    u8"inline",
    u8"int",
    u8"long",
    u8"nullptr",
    u8"register",
    u8"restrict",
    u8"return",
    u8"short",
    u8"signed",
    u8"sizeof",
    u8"static",
    u8"static_assert",
    u8"struct",
    u8"switch",
    u8"thread_local",
    u8"true",
    u8"typedef",
    u8"typeof",
    u8"typeof_unqual",
    u8"union",
    u8"unsigned",
    u8"void",
    u8"volatile",
    u8"while",
    u8"_Alignas",
    u8"_Alignof",
    u8"_Atomic",
    u8"_BitInt",
    u8"_Bool",
    u8"_Complex",
    u8"_Decimal128",
    u8"_Decimal32",
    u8"_Decimal64",
    u8"_Generic",
    u8"_Imaginary",
    u8"_Noreturn",
    u8"_Static_assert",
    u8"_Thread_local",
};

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

constexpr ::fast_io::u8string_view csharp_keywords[]{
    u8"abstract",  u8"add",        u8"alias",      u8"and",      u8"as",        u8"ascending", u8"async",
    u8"await",     u8"base",       u8"bool",       u8"break",    u8"by",        u8"byte",      u8"case",
    u8"catch",     u8"char",       u8"checked",    u8"class",    u8"const",     u8"continue",  u8"decimal",
    u8"default",   u8"delegate",   u8"descending", u8"do",       u8"double",    u8"dynamic",   u8"else",
    u8"enum",      u8"equals",     u8"event",      u8"explicit", u8"extern",    u8"false",     u8"file",
    u8"finally",   u8"fixed",      u8"float",      u8"for",      u8"foreach",   u8"from",      u8"get",
    u8"global",    u8"goto",       u8"group",      u8"if",       u8"implicit",  u8"in",        u8"init",
    u8"int",       u8"interface",  u8"internal",   u8"into",     u8"is",        u8"join",      u8"let",
    u8"lock",      u8"long",       u8"managed",    u8"nameof",   u8"namespace", u8"new",       u8"nint",
    u8"not",       u8"notnull",    u8"nuint",      u8"null",     u8"object",    u8"on",        u8"operator",
    u8"or",        u8"orderby",    u8"out",        u8"override", u8"params",    u8"partial",   u8"private",
    u8"protected", u8"public",     u8"readonly",   u8"record",   u8"ref",       u8"remove",    u8"required",
    u8"return",    u8"sbyte",      u8"scoped",     u8"sealed",   u8"select",    u8"set",       u8"short",
    u8"sizeof",    u8"stackalloc", u8"static",     u8"string",   u8"struct",    u8"switch",    u8"this",
    u8"throw",     u8"true",       u8"try",        u8"typeof",   u8"uint",      u8"ulong",     u8"unchecked",
    u8"unmanaged", u8"unsafe",     u8"ushort",     u8"using",    u8"value",     u8"var",       u8"virtual",
    u8"void",      u8"volatile",   u8"when",       u8"where",    u8"while",     u8"with",      u8"yield",
};

constexpr ::fast_io::u8string_view kotlin_keywords[]{
    u8"as",          u8"break",    u8"by",          u8"catch",    u8"class",    u8"companion", u8"const",
    u8"constructor", u8"continue", u8"crossinline", u8"data",     u8"delegate", u8"do",        u8"dynamic",
    u8"else",        u8"enum",     u8"expect",      u8"external", u8"false",    u8"field",     u8"file",
    u8"final",       u8"finally",  u8"for",         u8"fun",      u8"get",      u8"if",        u8"import",
    u8"in",          u8"infix",    u8"init",        u8"inline",   u8"inner",    u8"interface", u8"internal",
    u8"is",          u8"lateinit", u8"noinline",    u8"null",     u8"object",   u8"open",      u8"operator",
    u8"out",         u8"override", u8"package",     u8"param",    u8"private",  u8"property",  u8"protected",
    u8"public",      u8"receiver", u8"reified",     u8"return",   u8"sealed",   u8"set",       u8"setparam",
    u8"super",       u8"suspend",  u8"tailrec",     u8"this",     u8"throw",    u8"true",      u8"try",
    u8"typealias",   u8"typeof",   u8"val",         u8"value",    u8"var",      u8"vararg",    u8"when",
    u8"where",       u8"while",
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

constexpr ::fast_io::u8string_view lua_keywords[]{
    u8"and",      u8"break",  u8"do",   u8"else", u8"elseif", u8"end",   u8"false", u8"for",
    u8"function", u8"goto",   u8"if",   u8"in",   u8"local",  u8"nil",   u8"not",   u8"or",
    u8"repeat",   u8"return", u8"then", u8"true", u8"until",  u8"while",
};

constexpr ::fast_io::u8string_view bash_keywords[]{
    u8"case",     u8"coproc", u8"do", u8"done",   u8"elif", u8"else", u8"esac",  u8"fi",    u8"for",
    u8"function", u8"if",     u8"in", u8"select", u8"then", u8"time", u8"until", u8"while",
};

constexpr ::fast_io::u8string_view go_keywords[]{
    u8"break",  u8"case",        u8"chan", u8"const",   u8"continue", u8"default", u8"defer",
    u8"else",   u8"fallthrough", u8"for",  u8"func",    u8"go",       u8"goto",    u8"if",
    u8"import", u8"interface",   u8"map",  u8"package", u8"range",    u8"return",  u8"select",
    u8"struct", u8"switch",      u8"type", u8"var",     u8"true",     u8"false",   u8"nil",
};

constexpr ::fast_io::u8string_view java_keywords[]{
    u8"abstract", u8"assert",    u8"boolean",   u8"break",    u8"byte",         u8"case",    u8"catch",
    u8"char",     u8"class",     u8"const",     u8"continue", u8"default",      u8"do",      u8"double",
    u8"else",     u8"enum",      u8"exports",   u8"extends",  u8"false",        u8"final",   u8"finally",
    u8"float",    u8"for",       u8"goto",      u8"if",       u8"implements",   u8"import",  u8"instanceof",
    u8"int",      u8"interface", u8"long",      u8"module",   u8"native",       u8"new",     u8"non-sealed",
    u8"null",     u8"open",      u8"opens",     u8"package",  u8"permits",      u8"private", u8"protected",
    u8"provides", u8"public",    u8"record",    u8"requires", u8"return",       u8"sealed",  u8"short",
    u8"static",   u8"strictfp",  u8"super",     u8"switch",   u8"synchronized", u8"this",    u8"throw",
    u8"throws",   u8"to",        u8"transient", u8"true",     u8"transitive",   u8"try",     u8"uses",
    u8"var",      u8"void",      u8"volatile",  u8"while",    u8"with",         u8"yield",
};

constexpr ::fast_io::u8string_view javascript_keywords[]{
    u8"async",      u8"await",   u8"break",  u8"case",     u8"catch", u8"class",  u8"const",   u8"continue",
    u8"debugger",   u8"default", u8"delete", u8"do",       u8"else",  u8"export", u8"extends", u8"false",
    u8"finally",    u8"for",     u8"from",   u8"function", u8"get",   u8"if",     u8"import",  u8"in",
    u8"instanceof", u8"let",     u8"new",    u8"null",     u8"of",    u8"return", u8"set",     u8"static",
    u8"super",      u8"switch",  u8"this",   u8"throw",    u8"true",  u8"try",    u8"typeof",  u8"undefined",
    u8"var",        u8"void",    u8"while",  u8"with",     u8"yield",
};

constexpr ::fast_io::u8string_view typescript_keywords[]{
    u8"abstract",  u8"any",     u8"as",         u8"asserts", u8"async",     u8"await",     u8"bigint",
    u8"boolean",   u8"break",   u8"case",       u8"catch",   u8"class",     u8"const",     u8"constructor",
    u8"continue",  u8"declare", u8"default",    u8"delete",  u8"do",        u8"else",      u8"enum",
    u8"export",    u8"extends", u8"false",      u8"finally", u8"for",       u8"from",      u8"function",
    u8"get",       u8"if",      u8"implements", u8"import",  u8"in",        u8"infer",     u8"instanceof",
    u8"interface", u8"is",      u8"keyof",      u8"let",     u8"module",    u8"namespace", u8"never",
    u8"new",       u8"null",    u8"number",     u8"object",  u8"of",        u8"override",  u8"private",
    u8"protected", u8"public",  u8"readonly",   u8"require", u8"return",    u8"satisfies", u8"set",
    u8"static",    u8"string",  u8"super",      u8"switch",  u8"symbol",    u8"this",      u8"throw",
    u8"true",      u8"try",     u8"type",       u8"typeof",  u8"undefined", u8"unique",    u8"unknown",
    u8"var",       u8"void",    u8"while",      u8"with",    u8"yield",
};

constexpr ::fast_io::u8string_view python_keywords[]{
    u8"False",  u8"None",     u8"True",   u8"and", u8"as",    u8"assert", u8"async",  u8"await",    u8"break", u8"case",
    u8"class",  u8"continue", u8"def",    u8"del", u8"elif",  u8"else",   u8"except", u8"finally",  u8"for",   u8"from",
    u8"global", u8"if",       u8"import", u8"in",  u8"is",    u8"lambda", u8"match",  u8"nonlocal", u8"not",   u8"or",
    u8"pass",   u8"raise",    u8"return", u8"try", u8"while", u8"with",   u8"yield",
};

constexpr ::fast_io::u8string_view json_keywords[]{u8"false", u8"null", u8"true"};

constexpr ::fast_io::u8string_view toml_keywords[]{u8"false", u8"inf", u8"nan", u8"true"};

constexpr ::fast_io::u8string_view yaml_keywords[]{
    u8"false", u8"False", u8"FALSE", u8"null", u8"Null", u8"NULL", u8"true", u8"True",
    u8"TRUE",  u8"yes",   u8"Yes",   u8"YES",  u8"no",   u8"No",   u8"NO",
};

constexpr ::fast_io::u8string_view sql_keywords[]{
    u8"all",   u8"alter",    u8"and",    u8"any",     u8"as",         u8"asc",    u8"begin",    u8"between", u8"by",
    u8"case",  u8"check",    u8"column", u8"commit",  u8"constraint", u8"create", u8"database", u8"default", u8"delete",
    u8"desc",  u8"distinct", u8"drop",   u8"else",    u8"end",        u8"exists", u8"false",    u8"foreign", u8"from",
    u8"full",  u8"group",    u8"having", u8"in",      u8"index",      u8"inner",  u8"insert",   u8"into",    u8"is",
    u8"join",  u8"key",      u8"left",   u8"like",    u8"limit",      u8"not",    u8"null",     u8"offset",  u8"on",
    u8"or",    u8"order",    u8"outer",  u8"primary", u8"references", u8"right",  u8"rollback", u8"select",  u8"set",
    u8"table", u8"then",     u8"true",   u8"union",   u8"unique",     u8"update", u8"values",   u8"view",    u8"when",
    u8"where", u8"with",
};

constexpr ::fast_io::u8string_view css_keywords[]{u8"important"};

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

template<::std::size_t keyword_count>
[[nodiscard]]
constexpr auto syntax_is_keyword_case_insensitive(::fast_io::u8string_view const identifier,
                                                  ::fast_io::u8string_view const (&keywords)[keyword_count]) noexcept
    -> bool {
    for (auto const keyword : keywords) {
        if (identifier.size() != keyword.size()) {
            continue;
        }
        bool equal{true};
        for (::std::size_t index{}; index != identifier.size(); ++index) {
            char8_t const chr{
                ::pltxt2htm::details::u8string_view_index<::pltxt2htm::Contracts::ignore>(identifier, index)};
            char8_t const folded{chr >= u8'A' && chr <= u8'Z' ? static_cast<char8_t>(chr + (u8'a' - u8'A')) : chr};
            if (folded != ::pltxt2htm::details::u8string_view_index<::pltxt2htm::Contracts::ignore>(keyword, index)) {
                equal = false;
                break;
            }
        }
        if (equal) {
            return true;
        }
    }
    return false;
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
