/**
 * @file style.hh
 * @brief Backend presentation roles for language-specific code nodes.
 */

#pragma once

#include <fast_io/fast_io_dsal/string_view.h>
#include "../../../ast/code/ast.hh"
#include "../../push_macro.hh"

namespace pltxt2htm::details {

enum class CodeStyle : unsigned {
    plain = 0,
    keyword,
    string,
    number,
    comment,
    function,
    macro,
    preprocessor,
};

template<::pltxt2htm::CodeLanguage language>
[[nodiscard]]
constexpr auto code_style(::pltxt2htm::CodeNodeKind<language> const kind) noexcept -> CodeStyle {
    static_assert(language != ::pltxt2htm::CodeLanguage::rendered);
    return static_cast<::pltxt2htm::details::CodeStyle>(static_cast<unsigned>(kind));
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto code_style(::pltxt2htm::CodeAst<ndebug> const& ast, ::pltxt2htm::CodeNode const& node) noexcept
    -> CodeStyle {
    switch (ast.get_language()) /* -Werror=switch */ {
    case ::pltxt2htm::CodeLanguage::plain: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::plain>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::plain>(node));
    }
    case ::pltxt2htm::CodeLanguage::bash: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::bash>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::bash>(node));
    }
    case ::pltxt2htm::CodeLanguage::c: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::c>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::c>(node));
    }
    case ::pltxt2htm::CodeLanguage::cpp: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::cpp>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::cpp>(node));
    }
    case ::pltxt2htm::CodeLanguage::csharp: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::csharp>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::csharp>(node));
    }
    case ::pltxt2htm::CodeLanguage::css: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::css>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::css>(node));
    }
    case ::pltxt2htm::CodeLanguage::go: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::go>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::go>(node));
    }
    case ::pltxt2htm::CodeLanguage::html: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::html>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::html>(node));
    }
    case ::pltxt2htm::CodeLanguage::java: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::java>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::java>(node));
    }
    case ::pltxt2htm::CodeLanguage::javascript: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::javascript>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::javascript>(node));
    }
    case ::pltxt2htm::CodeLanguage::json: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::json>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::json>(node));
    }
    case ::pltxt2htm::CodeLanguage::kotlin: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::kotlin>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::kotlin>(node));
    }
    case ::pltxt2htm::CodeLanguage::lua: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::lua>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::lua>(node));
    }
    case ::pltxt2htm::CodeLanguage::python: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::python>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::python>(node));
    }
    case ::pltxt2htm::CodeLanguage::rust: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::rust>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::rust>(node));
    }
    case ::pltxt2htm::CodeLanguage::sql: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::sql>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::sql>(node));
    }
    case ::pltxt2htm::CodeLanguage::toml: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::toml>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::toml>(node));
    }
    case ::pltxt2htm::CodeLanguage::typescript: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::typescript>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::typescript>(node));
    }
    case ::pltxt2htm::CodeLanguage::xml: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::xml>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::xml>(node));
    }
    case ::pltxt2htm::CodeLanguage::yaml: {
        return ::pltxt2htm::details::code_style<::pltxt2htm::CodeLanguage::yaml>(
            ast.template get_node_kind<::pltxt2htm::CodeLanguage::yaml>(node));
    }
    case ::pltxt2htm::CodeLanguage::rendered: {
        [[unlikely]] { pltxt2htm_unreachable(u8"Rendered code does not have a syntax style"); }
    }
    }
    pltxt2htm_unreachable(u8"Unreachable code after exhaustive switch on code language");
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto code_style_color(CodeStyle const style) noexcept -> ::fast_io::u8string_view {
    switch (style) /* -Werror=switch */ {
    case CodeStyle::plain: {
        return u8"";
    }
    case CodeStyle::keyword: {
        return u8"#cf222e";
    }
    case CodeStyle::string: {
        return u8"#0a3069";
    }
    case CodeStyle::number: {
        return u8"#0550ae";
    }
    case CodeStyle::comment: {
        return u8"#6e7781";
    }
    case CodeStyle::function: {
        return u8"#8250df";
    }
    case CodeStyle::macro: {
        return u8"#cf222e";
    }
    case CodeStyle::preprocessor: {
        return u8"#0550ae";
    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
    default:
        [[unlikely]] {
            pltxt2htm_unreachable(u8"Unexpected code style");
        }
#endif
    }
    pltxt2htm_unreachable(u8"Unreachable code after exhaustive switch on code style");
}

} // namespace pltxt2htm::details

#include "../../pop_macro.hh"
