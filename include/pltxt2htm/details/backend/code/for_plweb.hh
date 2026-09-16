/**
 * @file for_plweb.hh
 * @brief Render fenced-code ASTs as HTML.
 */

#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../../../ast/code/ast.hh"
#include "../../utils.hh"
#include "style.hh"
#include "../../push_macro.hh"

namespace pltxt2htm::details {

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plweb_code_text(::fast_io::u8string_view const text, ::fast_io::u8string& result) noexcept {
    for (::std::size_t index{}; index != text.size(); ++index) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(text, index)};
        switch (chr) {
        case u8'&': {
            result.append(u8"&amp;");
            break;
        }
        case u8'\'': {
            result.append(u8"&apos;");
            break;
        }
        case u8'\"': {
            result.append(u8"&quot;");
            break;
        }
        case u8'<': {
            result.append(u8"&lt;");
            break;
        }
        case u8'>': {
            result.append(u8"&gt;");
            break;
        }
        case u8' ': {
            result.append(u8"&nbsp;");
            break;
        }
        case u8'\t': {
            result.append(u8"&nbsp;&nbsp;&nbsp;&nbsp;");
            break;
        }
        default:
            result.push_back(chr);
            break;
        }
    }
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plweb_code_unit(::pltxt2htm::Unit const unit, ::fast_io::u8string& result) noexcept {
    switch (unit) /* -Werror=switch */ {
    case ::pltxt2htm::Unit::percent: {
        result.push_back(u8'%');
        return;
    }
    case ::pltxt2htm::Unit::em: {
        result.append(u8"em");
        return;
    }
    case ::pltxt2htm::Unit::px: {
        result.append(u8"px");
        return;
    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
    default:
        [[unlikely]] {
            pltxt2htm_unreachable(u8"Unexpected rendered code style unit");
        }
#endif
    }
    pltxt2htm_unreachable(u8"Unreachable code after exhaustive switch on rendered code style unit");
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plweb_rendered_style(::pltxt2htm::CodeRenderedStyle<ndebug> const& style,
                                           ::fast_io::u8string& result) noexcept {
    result.append(u8"<span style=\"");
    if (style.get_color().empty() == false) {
        result.append(u8"color:");
        result.append(style.get_color());
        result.push_back(u8';');
    }
    if (style.get_font_size().has_value()) {
        auto const& font_size{style.get_font_size().template value<ndebug>()};
        result.append(u8"font-size:");
        result.append(::pltxt2htm::details::double2str(font_size.value));
        ::pltxt2htm::details::append_plweb_code_unit<ndebug>(font_size.unit, result);
        result.push_back(u8';');
    }
    if (style.get_vertical_align().has_value()) {
        auto const& vertical_align{style.get_vertical_align().template value<ndebug>()};
        result.append(u8"vertical-align:");
        if (vertical_align.get_kind() == ::pltxt2htm::VerticalAlignKind::keyword) {
            result.append(::pltxt2htm::details::vertical_align_keyword_string<ndebug>(vertical_align.get_keyword()));
        }
        else {
            result.append(::pltxt2htm::details::ptrdiff_t2str(vertical_align.get_length().value));
            ::pltxt2htm::details::append_plweb_code_unit<ndebug>(vertical_align.get_length().unit, result);
        }
        result.push_back(u8';');
    }
    result.append(u8"\">");
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plweb_language_code_ast(::pltxt2htm::CodeAst<ndebug> const& ast,
                                              ::fast_io::u8string& result) noexcept {
    for (auto const& node : ast.get_nodes()) {
        CodeStyle const style{::pltxt2htm::details::code_style<ndebug>(ast, node)};
        if (style != CodeStyle::plain) {
            result.append(u8"<span style=\"color:");
            result.append(::pltxt2htm::details::code_style_color<ndebug>(style));
            result.append(u8";\">");
        }
        ::pltxt2htm::details::append_plweb_code_text<ndebug>(ast.get_text(node), result);
        if (style != CodeStyle::plain) {
            result.append(u8"</span>");
        }
    }
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plweb_rendered_code_ast(::pltxt2htm::CodeAst<ndebug> const& ast,
                                              ::fast_io::u8string& result) noexcept {
    for (auto const& node : ast.get_nodes()) {
        switch (ast.template get_node_kind<::pltxt2htm::CodeLanguage::rendered>(node)) /* -Werror=switch */ {
        case ::pltxt2htm::CodeRenderedNodeKind::text: {
            ::pltxt2htm::details::append_plweb_code_text<ndebug>(ast.get_text(node), result);
            break;
        }
        case ::pltxt2htm::CodeRenderedNodeKind::entity_reference: {
            result.push_back(u8'&');
            result.append(ast.get_text(node));
            result.push_back(u8';');
            break;
        }
        case ::pltxt2htm::CodeRenderedNodeKind::style_begin: {
            ::pltxt2htm::details::append_plweb_rendered_style<ndebug>(ast.get_rendered_style(node), result);
            break;
        }
        case ::pltxt2htm::CodeRenderedNodeKind::style_end: {
            result.append(u8"</span>");
            break;
        }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected rendered code node kind");
            }
#endif
        }
    }
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plweb_code_ast(::pltxt2htm::CodeAst<ndebug> const& ast, ::fast_io::u8string& result) noexcept {
    if (ast.get_language() == ::pltxt2htm::CodeLanguage::rendered) {
        ::pltxt2htm::details::append_plweb_rendered_code_ast<ndebug>(ast, result);
        return;
    }
    ::pltxt2htm::details::append_plweb_language_code_ast<ndebug>(ast, result);
}

} // namespace pltxt2htm::details

#include "../../pop_macro.hh"
