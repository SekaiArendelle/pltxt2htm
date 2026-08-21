/**
 * @file syntax_highlight_parser.hh
 * @brief Language-specific fenced-code parsers that emit highlighted AST nodes.
 * @note This internal header is included by try_parse.hh after parse_simple_pltext_node is defined.
 */

#pragma once

#include <cstddef>
#include <utility>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../../ast/ast.hh"
#include "../../contracts.hh"
#include "syntax_highlight.hh"
#include "../utils.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

template<::pltxt2htm::Contracts ndebug>
constexpr void append_code_syntax_ast(::pltxt2htm::Ast<ndebug>& source,
                                      ::pltxt2htm::Ast<ndebug>& destination) noexcept {
    for (auto& node : source) {
        destination.push_back(::std::move(node));
    }
    source.clear();
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_colored_code_syntax_ast(::pltxt2htm::Ast<ndebug>& token_ast, SyntaxTokenKind const kind,
                                              ::pltxt2htm::Ast<ndebug>& destination) noexcept {
    ::std::size_t current_index{};
    ::std::size_t const token_size{token_ast.size()};
    while (current_index != token_size) {
        ::std::size_t colored_end{current_index};
        while (colored_end != token_size &&
               ::pltxt2htm::details::vector_index<ndebug>(token_ast, colored_end).get_node_kind() !=
                   ::pltxt2htm::NodeKind::line_break) {
            ++colored_end;
        }
        if (current_index != colored_end) {
            ::pltxt2htm::Ast<ndebug> colored_ast{};
            colored_ast.reserve(colored_end - current_index);
            while (current_index != colored_end) {
                colored_ast.push_back(
                    ::std::move(::pltxt2htm::details::vector_index<ndebug>(token_ast, current_index++)));
            }
            destination.push_back(::pltxt2htm::PlTxtNode<ndebug>(::pltxt2htm::PlColor<ndebug>{
                ::std::move(colored_ast),
                ::fast_io::u8string{::pltxt2htm::details::syntax_token_color<ndebug>(kind)}}));
        }
        if (current_index != token_size) {
            destination.push_back(::std::move(::pltxt2htm::details::vector_index<ndebug>(token_ast, current_index++)));
        }
    }
    token_ast.clear();
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_plain_code_syntax(::fast_io::u8string_view const content) noexcept -> ::pltxt2htm::Ast<ndebug> {
    ::pltxt2htm::Ast<ndebug> ast{};
    ast.reserve(content.size());
    ::std::size_t current_index{};
    while (current_index != content.size()) {
        current_index += ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(
                             ::pltxt2htm::details::u8string_view_subview<ndebug>(content, current_index), ast)
                             .advance_count;
    }
    return ast;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_c_style_code_syntax(::fast_io::u8string_view const content, SyntaxLanguage const language) noexcept
    -> ::pltxt2htm::Ast<ndebug> {
    ::fast_io::u8string_view remaining{content};
    ::pltxt2htm::Ast<ndebug> lookahead_ast{};
    char8_t lookahead_ascii{};
    ::pltxt2htm::Ast<ndebug> ast{};
    ast.reserve(content.size());

    while (lookahead_ast.empty() == false || remaining.empty() == false) {
        if (lookahead_ast.empty()) {
            auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
            remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
            lookahead_ascii = parsed.ascii;
        }
        char8_t const chr{lookahead_ascii};

        if (chr == u8'/') {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            if (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                lookahead_ascii = parsed.ascii;
            }
            if (lookahead_ast.empty()) {
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(token_ast, ast);
                continue;
            }
            if (lookahead_ascii == u8'/') {
                while (lookahead_ast.empty() == false && lookahead_ascii != u8'\n') {
                    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                    if (remaining.empty() == false) {
                        auto const parsed =
                            ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                        remaining =
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                        lookahead_ascii = parsed.ascii;
                    }
                }
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::comment, ast);
                continue;
            }
            if (lookahead_ascii == u8'*') {
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                if (remaining.empty() == false) {
                    auto const parsed =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                    lookahead_ascii = parsed.ascii;
                }
                while (lookahead_ast.empty() == false) {
                    char8_t const current{lookahead_ascii};
                    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                    if (remaining.empty() == false) {
                        auto const parsed =
                            ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                        remaining =
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                        lookahead_ascii = parsed.ascii;
                    }
                    if (current != u8'*' || lookahead_ast.empty() || lookahead_ascii != u8'/') {
                        continue;
                    }
                    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                    if (remaining.empty() == false) {
                        auto const parsed =
                            ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                        remaining =
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                        lookahead_ascii = parsed.ascii;
                    }
                    break;
                }
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::comment, ast);
                continue;
            }
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(token_ast, ast);
            continue;
        }

        if (chr == u8'\"' || chr == u8'\'' ||
            ((language == SyntaxLanguage::go || language == SyntaxLanguage::javascript ||
              language == SyntaxLanguage::typescript) &&
             chr == u8'`')) {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            char8_t const quote{chr};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            while (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                char8_t const current{parsed.ascii};
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                if (current == u8'\\' && remaining.empty() == false &&
                    (language != SyntaxLanguage::go || quote != u8'`')) {
                    auto const escaped =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, escaped.advance_count);
                    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                    continue;
                }
                if (current == quote || (current == u8'\n' && quote == u8'\'')) {
                    break;
                }
            }
            ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::string, ast);
            continue;
        }

        if (::pltxt2htm::details::is_ascii_digit(chr)) {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            while (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                lookahead_ascii = parsed.ascii;
                if (::pltxt2htm::details::syntax_is_identifier_continue(lookahead_ascii) == false &&
                    lookahead_ascii != u8'.' && lookahead_ascii != u8'\'') {
                    break;
                }
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            }
            ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::number, ast);
            continue;
        }

        if ((language == SyntaxLanguage::c || language == SyntaxLanguage::cpp) && chr == u8'#') {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            while (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                lookahead_ascii = parsed.ascii;
                if (lookahead_ascii != u8' ' && lookahead_ascii != u8'\t') {
                    break;
                }
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            }
            while (lookahead_ast.empty() == false || remaining.empty() == false) {
                if (lookahead_ast.empty()) {
                    auto const parsed =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                    lookahead_ascii = parsed.ascii;
                }
                if (::pltxt2htm::details::syntax_is_identifier_continue(lookahead_ascii) == false) {
                    break;
                }
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            }
            ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::preprocessor, ast);
            continue;
        }

        if (::pltxt2htm::details::syntax_is_identifier_start(chr)) {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::fast_io::u8string identifier{};
            identifier.push_back(chr);
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            while (lookahead_ast.empty() == false || remaining.empty() == false) {
                if (lookahead_ast.empty()) {
                    auto const parsed =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                    lookahead_ascii = parsed.ascii;
                }
                if (::pltxt2htm::details::syntax_is_identifier_continue(lookahead_ascii) == false) {
                    break;
                }
                identifier.push_back(lookahead_ascii);
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            }
            ::pltxt2htm::Ast<ndebug> whitespace_ast{};
            while (lookahead_ast.empty() == false || remaining.empty() == false) {
                if (lookahead_ast.empty()) {
                    auto const parsed =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                    lookahead_ascii = parsed.ascii;
                }
                if (lookahead_ascii != u8' ' && lookahead_ascii != u8'\t') {
                    break;
                }
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, whitespace_ast);
            }
            ::fast_io::u8string_view const identifier_view{identifier.data(), identifier.size()};
            bool is_keyword{};
            switch (language) {
            case SyntaxLanguage::c:
                is_keyword =
                    ::pltxt2htm::details::syntax_is_keyword(identifier_view, ::pltxt2htm::details::c23_keywords);
                break;
            case SyntaxLanguage::cpp:
                is_keyword =
                    ::pltxt2htm::details::syntax_is_keyword(identifier_view, ::pltxt2htm::details::cpp_keywords);
                break;
            case SyntaxLanguage::go:
                is_keyword =
                    ::pltxt2htm::details::syntax_is_keyword(identifier_view, ::pltxt2htm::details::go_keywords);
                break;
            case SyntaxLanguage::java:
                is_keyword =
                    ::pltxt2htm::details::syntax_is_keyword(identifier_view, ::pltxt2htm::details::java_keywords);
                break;
            case SyntaxLanguage::javascript:
                is_keyword =
                    ::pltxt2htm::details::syntax_is_keyword(identifier_view, ::pltxt2htm::details::javascript_keywords);
                break;
            case SyntaxLanguage::typescript:
                is_keyword =
                    ::pltxt2htm::details::syntax_is_keyword(identifier_view, ::pltxt2htm::details::typescript_keywords);
                break;
            default:
                pltxt2htm_unreachable(u8"Unexpected C-style syntax language");
            }
            if (is_keyword) {
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::keyword, ast);
            }
            else if (lookahead_ast.empty() == false && lookahead_ascii == u8'(') {
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::function, ast);
            }
            else {
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(token_ast, ast);
            }
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(whitespace_ast, ast);
            continue;
        }

        ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, ast);
    }
    return ast;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_data_script_code_syntax(::fast_io::u8string_view const content,
                                             SyntaxLanguage const language) noexcept -> ::pltxt2htm::Ast<ndebug> {
    bool const has_hash_comment{language == SyntaxLanguage::bash || language == SyntaxLanguage::python ||
                                language == SyntaxLanguage::yaml};
    bool const has_dash_comment{language == SyntaxLanguage::sql};
    bool const has_block_comment{language == SyntaxLanguage::css || language == SyntaxLanguage::sql};
    bool const only_double_quote{language == SyntaxLanguage::json};
    ::fast_io::u8string_view remaining{content};
    ::pltxt2htm::Ast<ndebug> lookahead_ast{};
    char8_t lookahead_ascii{};
    ::pltxt2htm::Ast<ndebug> ast{};
    ast.reserve(content.size());

    while (lookahead_ast.empty() == false || remaining.empty() == false) {
        if (lookahead_ast.empty()) {
            auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
            remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
            lookahead_ascii = parsed.ascii;
        }
        char8_t const chr{lookahead_ascii};

        if (has_hash_comment && chr == u8'#') {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            while (lookahead_ast.empty() == false && lookahead_ascii != u8'\n') {
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                if (remaining.empty() == false) {
                    auto const parsed =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                    lookahead_ascii = parsed.ascii;
                }
            }
            ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::comment, ast);
            continue;
        }

        if (has_dash_comment && chr == u8'-') {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            if (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                lookahead_ascii = parsed.ascii;
            }
            if (lookahead_ast.empty() == false && lookahead_ascii == u8'-') {
                while (lookahead_ast.empty() == false && lookahead_ascii != u8'\n') {
                    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                    if (remaining.empty() == false) {
                        auto const parsed =
                            ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                        remaining =
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                        lookahead_ascii = parsed.ascii;
                    }
                }
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::comment, ast);
                continue;
            }
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(token_ast, ast);
            continue;
        }

        if (has_block_comment && chr == u8'/') {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            if (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                lookahead_ascii = parsed.ascii;
            }
            if (lookahead_ast.empty() == false && lookahead_ascii == u8'*') {
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                if (remaining.empty() == false) {
                    auto const parsed =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                    lookahead_ascii = parsed.ascii;
                }
                while (lookahead_ast.empty() == false) {
                    char8_t const current{lookahead_ascii};
                    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                    if (remaining.empty() == false) {
                        auto const parsed =
                            ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                        remaining =
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                        lookahead_ascii = parsed.ascii;
                    }
                    if (current != u8'*' || lookahead_ast.empty() || lookahead_ascii != u8'/') {
                        continue;
                    }
                    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                    if (remaining.empty() == false) {
                        auto const parsed =
                            ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                        remaining =
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                        lookahead_ascii = parsed.ascii;
                    }
                    break;
                }
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::comment, ast);
                continue;
            }
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(token_ast, ast);
            continue;
        }

        if (chr == u8'\"' || (only_double_quote == false && chr == u8'\'')) {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            char8_t const quote{chr};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            while (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                char8_t const current{parsed.ascii};
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                if (current == u8'\\' && remaining.empty() == false &&
                    !(language == SyntaxLanguage::sql && quote == u8'\'')) {
                    auto const escaped =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, escaped.advance_count);
                    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                    continue;
                }
                if (current == quote) {
                    if (language == SyntaxLanguage::sql && quote == u8'\'' && remaining.empty() == false) {
                        auto const next = ::pltxt2htm::details::u8string_view_index<ndebug>(remaining, 0);
                        if (next == quote) {
                            auto const escaped =
                                ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                            remaining =
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, escaped.advance_count);
                            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                            continue;
                        }
                    }
                    break;
                }
            }
            ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::string, ast);
            continue;
        }

        if (::pltxt2htm::details::is_ascii_digit(chr)) {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            while (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                lookahead_ascii = parsed.ascii;
                if (::pltxt2htm::details::syntax_is_identifier_continue(lookahead_ascii) == false &&
                    lookahead_ascii != u8'.' && lookahead_ascii != u8'_' && lookahead_ascii != u8'-') {
                    break;
                }
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            }
            ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::number, ast);
            continue;
        }

        if (language == SyntaxLanguage::css && chr == u8'@') {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            while (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                lookahead_ascii = parsed.ascii;
                if (::pltxt2htm::details::syntax_is_identifier_continue(lookahead_ascii) == false &&
                    lookahead_ascii != u8'-') {
                    break;
                }
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            }
            ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::keyword, ast);
            continue;
        }

        if (::pltxt2htm::details::syntax_is_identifier_start(chr)) {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::fast_io::u8string identifier{};
            identifier.push_back(chr);
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            while (lookahead_ast.empty() == false || remaining.empty() == false) {
                if (lookahead_ast.empty()) {
                    auto const parsed =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                    lookahead_ascii = parsed.ascii;
                }
                if (::pltxt2htm::details::syntax_is_identifier_continue(lookahead_ascii) == false) {
                    break;
                }
                identifier.push_back(lookahead_ascii);
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            }
            ::pltxt2htm::Ast<ndebug> whitespace_ast{};
            while (lookahead_ast.empty() == false || remaining.empty() == false) {
                if (lookahead_ast.empty()) {
                    auto const parsed =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                    lookahead_ascii = parsed.ascii;
                }
                if (lookahead_ascii != u8' ' && lookahead_ascii != u8'\t') {
                    break;
                }
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, whitespace_ast);
            }
            ::fast_io::u8string_view const identifier_view{identifier.data(), identifier.size()};
            bool is_keyword{};
            switch (language) {
            case SyntaxLanguage::bash:
                is_keyword =
                    ::pltxt2htm::details::syntax_is_keyword(identifier_view, ::pltxt2htm::details::bash_keywords);
                break;
            case SyntaxLanguage::css:
                is_keyword =
                    ::pltxt2htm::details::syntax_is_keyword(identifier_view, ::pltxt2htm::details::css_keywords);
                break;
            case SyntaxLanguage::json:
                is_keyword =
                    ::pltxt2htm::details::syntax_is_keyword(identifier_view, ::pltxt2htm::details::json_keywords);
                break;
            case SyntaxLanguage::python:
                is_keyword =
                    ::pltxt2htm::details::syntax_is_keyword(identifier_view, ::pltxt2htm::details::python_keywords);
                break;
            case SyntaxLanguage::sql:
                is_keyword = ::pltxt2htm::details::syntax_is_keyword_case_insensitive(
                    identifier_view, ::pltxt2htm::details::sql_keywords);
                break;
            case SyntaxLanguage::yaml:
                is_keyword =
                    ::pltxt2htm::details::syntax_is_keyword(identifier_view, ::pltxt2htm::details::yaml_keywords);
                break;
            default:
                pltxt2htm_unreachable(u8"Unexpected data or script syntax language");
            }
            if (is_keyword) {
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::keyword, ast);
            }
            else if (language != SyntaxLanguage::json && language != SyntaxLanguage::yaml &&
                     lookahead_ast.empty() == false && lookahead_ascii == u8'(') {
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::function, ast);
            }
            else {
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(token_ast, ast);
            }
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(whitespace_ast, ast);
            continue;
        }

        ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, ast);
    }
    return ast;
}

/**
 * @brief Parse a Lua long-bracket opener and its content.
 * @details The current lookahead must be the first `[`. Tentatively consumed opener
 *          characters remain in token_ast when the opener is invalid.
 * @return true once a valid `[=*[` opener is found, including when it runs to EOF.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr bool parse_lua_long_bracket(::fast_io::u8string_view& remaining, ::pltxt2htm::Ast<ndebug>& lookahead_ast,
                                      char8_t& lookahead_ascii, ::pltxt2htm::Ast<ndebug>& token_ast) noexcept {
    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
    ::std::size_t equals_count{};
    while (remaining.empty() == false) {
        auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
        remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
        lookahead_ascii = parsed.ascii;
        if (lookahead_ascii != u8'=') {
            break;
        }
        ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
        ++equals_count;
    }
    if (lookahead_ast.empty() || lookahead_ascii != u8'[') {
        return false;
    }
    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);

    while (lookahead_ast.empty() == false || remaining.empty() == false) {
        if (lookahead_ast.empty()) {
            auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
            remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
            lookahead_ascii = parsed.ascii;
        }
        char8_t const current{lookahead_ascii};
        ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
        if (current != u8']') {
            continue;
        }

        ::std::size_t closing_equals{};
        while (closing_equals != equals_count && remaining.empty() == false) {
            auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
            remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
            lookahead_ascii = parsed.ascii;
            if (lookahead_ascii != u8'=') {
                break;
            }
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            ++closing_equals;
        }
        if (closing_equals != equals_count) {
            continue;
        }
        if (lookahead_ast.empty()) {
            if (remaining.empty()) {
                break;
            }
            auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
            remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
            lookahead_ascii = parsed.ascii;
        }
        if (lookahead_ascii != u8']') {
            continue;
        }
        ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
        return true;
    }
    return true;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_lua_code_syntax(::fast_io::u8string_view const content) noexcept -> ::pltxt2htm::Ast<ndebug> {
    ::fast_io::u8string_view remaining{content};
    ::pltxt2htm::Ast<ndebug> lookahead_ast{};
    char8_t lookahead_ascii{};
    ::pltxt2htm::Ast<ndebug> ast{};
    ast.reserve(content.size());

    while (lookahead_ast.empty() == false || remaining.empty() == false) {
        if (lookahead_ast.empty()) {
            auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
            remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
            lookahead_ascii = parsed.ascii;
        }
        char8_t const chr{lookahead_ascii};

        if (chr == u8'-') {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            if (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                lookahead_ascii = parsed.ascii;
            }
            if (lookahead_ast.empty() || lookahead_ascii != u8'-') {
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(token_ast, ast);
                continue;
            }
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            if (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                lookahead_ascii = parsed.ascii;
            }
            if (lookahead_ast.empty() == false && lookahead_ascii == u8'[' &&
                ::pltxt2htm::details::parse_lua_long_bracket<ndebug>(remaining, lookahead_ast, lookahead_ascii,
                                                                     token_ast)) {
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::comment, ast);
                continue;
            }
            while (lookahead_ast.empty() == false && lookahead_ascii != u8'\n') {
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                if (remaining.empty() == false) {
                    auto const parsed =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                    lookahead_ascii = parsed.ascii;
                }
            }
            ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::comment, ast);
            continue;
        }

        if (chr == u8'\"' || chr == u8'\'') {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            char8_t const quote{chr};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            while (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                char8_t const current{parsed.ascii};
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                if (current == u8'\\' && remaining.empty() == false) {
                    auto const escaped =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, escaped.advance_count);
                    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                    continue;
                }
                if (current == quote || current == u8'\n') {
                    break;
                }
            }
            ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::string, ast);
            continue;
        }

        if (chr == u8'[') {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            if (::pltxt2htm::details::parse_lua_long_bracket<ndebug>(remaining, lookahead_ast, lookahead_ascii,
                                                                     token_ast)) {
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::string, ast);
            }
            else {
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(token_ast, ast);
            }
            continue;
        }

        if (::pltxt2htm::details::is_ascii_digit(chr)) {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            while (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                lookahead_ascii = parsed.ascii;
                if (::pltxt2htm::details::syntax_is_identifier_continue(lookahead_ascii) == false &&
                    lookahead_ascii != u8'.') {
                    break;
                }
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            }
            ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::number, ast);
            continue;
        }

        if (::pltxt2htm::details::syntax_is_identifier_start(chr)) {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::fast_io::u8string identifier{};
            identifier.push_back(chr);
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            while (lookahead_ast.empty() == false || remaining.empty() == false) {
                if (lookahead_ast.empty()) {
                    auto const parsed =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                    lookahead_ascii = parsed.ascii;
                }
                if (::pltxt2htm::details::syntax_is_identifier_continue(lookahead_ascii) == false) {
                    break;
                }
                identifier.push_back(lookahead_ascii);
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            }
            ::pltxt2htm::Ast<ndebug> whitespace_ast{};
            while (lookahead_ast.empty() == false || remaining.empty() == false) {
                if (lookahead_ast.empty()) {
                    auto const parsed =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                    lookahead_ascii = parsed.ascii;
                }
                if (lookahead_ascii != u8' ' && lookahead_ascii != u8'\t') {
                    break;
                }
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, whitespace_ast);
            }
            ::fast_io::u8string_view const identifier_view{identifier.data(), identifier.size()};
            if (::pltxt2htm::details::syntax_is_keyword(identifier_view, ::pltxt2htm::details::lua_keywords)) {
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::keyword, ast);
            }
            else if (lookahead_ast.empty() == false && lookahead_ascii == u8'(') {
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::function, ast);
            }
            else {
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(token_ast, ast);
            }
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(whitespace_ast, ast);
            continue;
        }

        ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, ast);
    }
    return ast;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_rust_code_syntax(::fast_io::u8string_view const content) noexcept -> ::pltxt2htm::Ast<ndebug> {
    ::fast_io::u8string_view remaining{content};
    ::pltxt2htm::Ast<ndebug> lookahead_ast{};
    char8_t lookahead_ascii{};
    ::pltxt2htm::Ast<ndebug> ast{};
    ast.reserve(content.size());

    while (lookahead_ast.empty() == false || remaining.empty() == false) {
        if (lookahead_ast.empty()) {
            auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
            remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
            lookahead_ascii = parsed.ascii;
        }
        char8_t const chr{lookahead_ascii};

        if (chr == u8'/') {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            if (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                lookahead_ascii = parsed.ascii;
            }
            if (lookahead_ast.empty()) {
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(token_ast, ast);
                continue;
            }
            if (lookahead_ascii == u8'/') {
                while (lookahead_ast.empty() == false && lookahead_ascii != u8'\n') {
                    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                    if (remaining.empty() == false) {
                        auto const parsed =
                            ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                        remaining =
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                        lookahead_ascii = parsed.ascii;
                    }
                }
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::comment, ast);
                continue;
            }
            if (lookahead_ascii == u8'*') {
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                if (remaining.empty() == false) {
                    auto const parsed =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                    lookahead_ascii = parsed.ascii;
                }
                ::std::size_t depth{1};
                while (lookahead_ast.empty() == false && depth != 0) {
                    char8_t const current{lookahead_ascii};
                    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                    if (remaining.empty() == false) {
                        auto const parsed =
                            ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                        remaining =
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                        lookahead_ascii = parsed.ascii;
                    }
                    if (lookahead_ast.empty()) {
                        continue;
                    }
                    char8_t const after_current{lookahead_ascii};
                    if (current == u8'/' && after_current == u8'*') {
                        ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                        ++depth;
                    }
                    else if (current == u8'*' && after_current == u8'/') {
                        ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                        --depth;
                    }
                    else {
                        continue;
                    }
                    if (remaining.empty() == false) {
                        auto const parsed =
                            ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                        remaining =
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                        lookahead_ascii = parsed.ascii;
                    }
                }
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::comment, ast);
                continue;
            }
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(token_ast, ast);
            continue;
        }

        if (chr == u8'\"') {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            while (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                char8_t const current{parsed.ascii};
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                if (current == u8'\\' && remaining.empty() == false) {
                    auto const escaped =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, escaped.advance_count);
                    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                    continue;
                }
                if (current == u8'\"') {
                    break;
                }
            }
            ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::string, ast);
            continue;
        }

        if (chr == u8'\'') {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            bool has_identifier{};
            while (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                lookahead_ascii = parsed.ascii;
                if (::pltxt2htm::details::syntax_is_identifier_continue(lookahead_ascii) == false) {
                    break;
                }
                has_identifier = true;
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            }
            if (has_identifier && lookahead_ast.empty() == false && lookahead_ascii == u8'\'') {
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::string, ast);
                continue;
            }
            if (has_identifier) {
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::keyword, ast);
                continue;
            }
            while (lookahead_ast.empty() == false || remaining.empty() == false) {
                if (lookahead_ast.empty()) {
                    auto const parsed =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                    lookahead_ascii = parsed.ascii;
                }
                char8_t const current{lookahead_ascii};
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                if (current == u8'\\' && remaining.empty() == false) {
                    auto const escaped =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, escaped.advance_count);
                    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                    continue;
                }
                if (current == u8'\'' || current == u8'\n') {
                    break;
                }
            }
            ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::string, ast);
            continue;
        }

        if (::pltxt2htm::details::is_ascii_digit(chr)) {
            ::pltxt2htm::Ast<ndebug> token_ast{};
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            while (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                lookahead_ascii = parsed.ascii;
                if (::pltxt2htm::details::syntax_is_identifier_continue(lookahead_ascii) == false &&
                    lookahead_ascii != u8'.' && lookahead_ascii != u8'\'') {
                    break;
                }
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            }
            ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::number, ast);
            continue;
        }

        ::pltxt2htm::Ast<ndebug> token_ast{};
        ::fast_io::u8string identifier{};
        bool identifier_started{};
        if (chr == u8'r') {
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            identifier.push_back(u8'r');
            identifier_started = true;
            if (remaining.empty() == false) {
                auto const parsed = ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                lookahead_ascii = parsed.ascii;
            }
            if (lookahead_ast.empty() == false && (lookahead_ascii == u8'#' || lookahead_ascii == u8'\"')) {
                ::std::size_t hash_count{};
                while (lookahead_ast.empty() == false && lookahead_ascii == u8'#') {
                    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                    ++hash_count;
                    if (remaining.empty() == false) {
                        auto const parsed =
                            ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                        remaining =
                            ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                        lookahead_ascii = parsed.ascii;
                    }
                }
                if (lookahead_ast.empty() == false && lookahead_ascii == u8'\"') {
                    ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                    while (lookahead_ast.empty() == false || remaining.empty() == false) {
                        if (lookahead_ast.empty()) {
                            auto const parsed =
                                ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                            remaining =
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                            lookahead_ascii = parsed.ascii;
                        }
                        char8_t const current{lookahead_ascii};
                        ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                        if (current != u8'\"') {
                            continue;
                        }
                        ::std::size_t closing_hashes{};
                        while (closing_hashes != hash_count && remaining.empty() == false) {
                            auto const hash =
                                ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                            remaining =
                                ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, hash.advance_count);
                            lookahead_ascii = hash.ascii;
                            if (lookahead_ascii != u8'#') {
                                break;
                            }
                            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
                            ++closing_hashes;
                        }
                        if (closing_hashes == hash_count) {
                            break;
                        }
                    }
                    ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::string,
                                                                                 ast);
                    continue;
                }
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(token_ast, ast);
                continue;
            }
        }

        if (identifier_started || ::pltxt2htm::details::syntax_is_identifier_start(chr)) {
            if (identifier_started == false) {
                identifier.push_back(chr);
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            }
            while (lookahead_ast.empty() == false || remaining.empty() == false) {
                if (lookahead_ast.empty()) {
                    auto const parsed =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                    lookahead_ascii = parsed.ascii;
                }
                if (::pltxt2htm::details::syntax_is_identifier_continue(lookahead_ascii) == false) {
                    break;
                }
                identifier.push_back(lookahead_ascii);
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, token_ast);
            }
            ::pltxt2htm::Ast<ndebug> whitespace_ast{};
            while (lookahead_ast.empty() == false || remaining.empty() == false) {
                if (lookahead_ast.empty()) {
                    auto const parsed =
                        ::pltxt2htm::details::parse_simple_pltext_node<ndebug>(remaining, lookahead_ast);
                    remaining = ::pltxt2htm::details::u8string_view_subview<ndebug>(remaining, parsed.advance_count);
                    lookahead_ascii = parsed.ascii;
                }
                if (lookahead_ascii != u8' ' && lookahead_ascii != u8'\t') {
                    break;
                }
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, whitespace_ast);
            }
            ::fast_io::u8string_view const identifier_view{identifier.data(), identifier.size()};
            if (::pltxt2htm::details::syntax_is_keyword(identifier_view, ::pltxt2htm::details::rust_keywords)) {
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::keyword, ast);
            }
            else if (lookahead_ast.empty() == false && lookahead_ascii == u8'!') {
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::macro, ast);
            }
            else if (lookahead_ast.empty() == false && lookahead_ascii == u8'(') {
                ::pltxt2htm::details::append_colored_code_syntax_ast<ndebug>(token_ast, SyntaxTokenKind::function, ast);
            }
            else {
                ::pltxt2htm::details::append_code_syntax_ast<ndebug>(token_ast, ast);
            }
            ::pltxt2htm::details::append_code_syntax_ast<ndebug>(whitespace_ast, ast);
            continue;
        }

        ::pltxt2htm::details::append_code_syntax_ast<ndebug>(lookahead_ast, ast);
    }
    return ast;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto parse_code_fence_syntax(::fast_io::u8string_view const content, SyntaxLanguage const language) noexcept
    -> ::pltxt2htm::Ast<ndebug> {
    switch (language) {
    case SyntaxLanguage::plain:
        return ::pltxt2htm::details::parse_plain_code_syntax<ndebug>(content);
    case SyntaxLanguage::bash:
        return ::pltxt2htm::details::parse_data_script_code_syntax<ndebug>(content, language);
    case SyntaxLanguage::c:
        return ::pltxt2htm::details::parse_c_style_code_syntax<ndebug>(content, language);
    case SyntaxLanguage::cpp:
        return ::pltxt2htm::details::parse_c_style_code_syntax<ndebug>(content, language);
    case SyntaxLanguage::css:
        return ::pltxt2htm::details::parse_data_script_code_syntax<ndebug>(content, language);
    case SyntaxLanguage::go:
        return ::pltxt2htm::details::parse_c_style_code_syntax<ndebug>(content, language);
    case SyntaxLanguage::java:
        return ::pltxt2htm::details::parse_c_style_code_syntax<ndebug>(content, language);
    case SyntaxLanguage::javascript:
        return ::pltxt2htm::details::parse_c_style_code_syntax<ndebug>(content, language);
    case SyntaxLanguage::json:
        return ::pltxt2htm::details::parse_data_script_code_syntax<ndebug>(content, language);
    case SyntaxLanguage::lua:
        return ::pltxt2htm::details::parse_lua_code_syntax<ndebug>(content);
    case SyntaxLanguage::python:
        return ::pltxt2htm::details::parse_data_script_code_syntax<ndebug>(content, language);
    case SyntaxLanguage::rust:
        return ::pltxt2htm::details::parse_rust_code_syntax<ndebug>(content);
    case SyntaxLanguage::sql:
        return ::pltxt2htm::details::parse_data_script_code_syntax<ndebug>(content, language);
    case SyntaxLanguage::typescript:
        return ::pltxt2htm::details::parse_c_style_code_syntax<ndebug>(content, language);
    case SyntaxLanguage::yaml:
        return ::pltxt2htm::details::parse_data_script_code_syntax<ndebug>(content, language);
    }
    pltxt2htm_unreachable(u8"Unexpected syntax language");
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
