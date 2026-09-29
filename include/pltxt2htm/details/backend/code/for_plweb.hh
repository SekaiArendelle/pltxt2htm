/**
 * @file for_plweb.hh
 * @brief Render fenced-code ASTs as HTML.
 */

#pragma once

#include <cstddef>
#include "../../../ast/code/ast.hh"
#include "../../../container/string.hh"
#include "../../../container/string_view.hh"
#include "../../utils.hh"
#include "style.hh"
#include "../../push_macro.hh"

namespace pltxt2htm::details {

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plweb_code_text(::pltxt2htm::container::U8StringView const text,
                                      ::pltxt2htm::container::U8String& result) noexcept {
    for (::std::size_t index{}; index != text.size(); ++index) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(text, index)};
        switch (chr) {
        case u8'&': {
            result.template append<ndebug>(u8"&amp;");
            break;
        }
        case u8'\'': {
            result.template append<ndebug>(u8"&apos;");
            break;
        }
        case u8'\"': {
            result.template append<ndebug>(u8"&quot;");
            break;
        }
        case u8'<': {
            result.template append<ndebug>(u8"&lt;");
            break;
        }
        case u8'>': {
            result.template append<ndebug>(u8"&gt;");
            break;
        }
        case u8' ': {
            result.template append<ndebug>(u8"&nbsp;");
            break;
        }
        case u8'\t': {
            result.template append<ndebug>(u8"&nbsp;&nbsp;&nbsp;&nbsp;");
            break;
        }
        default:
            result.template push_back<ndebug>(chr);
            break;
        }
    }
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plweb_code_unit(::pltxt2htm::Unit const unit,
                                      ::pltxt2htm::container::U8String& result) noexcept {
    switch (unit) /* -Werror=switch */ {
    case ::pltxt2htm::Unit::percent: {
        result.template push_back<ndebug>(u8'%');
        return;
    }
    case ::pltxt2htm::Unit::em: {
        result.template append<ndebug>(u8"em");
        return;
    }
    case ::pltxt2htm::Unit::px: {
        result.template append<ndebug>(u8"px");
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
                                           ::pltxt2htm::container::U8String& result) noexcept {
    result.template append<ndebug>(u8"<span style=\"");
    if (style.get_color().is_empty() == false) {
        result.template append<ndebug>(u8"color:");
        result.template append<ndebug>(style.get_color());
        result.template push_back<ndebug>(u8';');
    }
    if (style.get_font_size().has_value()) {
        auto const& font_size{style.get_font_size().template value<ndebug>()};
        result.template append<ndebug>(u8"font-size:");
        result.template append<ndebug>(::pltxt2htm::details::double2str(font_size.value));
        ::pltxt2htm::details::append_plweb_code_unit<ndebug>(font_size.unit, result);
        result.template push_back<ndebug>(u8';');
    }
    if (style.get_vertical_align().has_value()) {
        auto const& vertical_align{style.get_vertical_align().template value<ndebug>()};
        result.template append<ndebug>(u8"vertical-align:");
        if (vertical_align.get_kind() == ::pltxt2htm::VerticalAlignKind::keyword) {
            result.template append<ndebug>(
                ::pltxt2htm::details::vertical_align_keyword_string<ndebug>(vertical_align.get_keyword()));
        }
        else {
            result.template append<ndebug>(
                ::pltxt2htm::details::ptrdiff_t2str(vertical_align.get_length().value));
            ::pltxt2htm::details::append_plweb_code_unit<ndebug>(vertical_align.get_length().unit, result);
        }
        result.template push_back<ndebug>(u8';');
    }
    result.template append<ndebug>(u8"\">");
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plweb_language_code_ast(::pltxt2htm::CodeAst<ndebug> const& ast,
                                              ::pltxt2htm::container::U8String& result) noexcept {
    for (auto const& node : ast.get_nodes()) {
        CodeStyle const style{::pltxt2htm::details::code_style<ndebug>(ast, node)};
        if (style != CodeStyle::plain) {
            result.template append<ndebug>(u8"<span style=\"color:");
            result.template append<ndebug>(::pltxt2htm::details::code_style_color<ndebug>(style));
            result.template append<ndebug>(u8";\">");
        }
        ::pltxt2htm::details::append_plweb_code_text<ndebug>(ast.get_text(node), result);
        if (style != CodeStyle::plain) {
            result.template append<ndebug>(u8"</span>");
        }
    }
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plweb_rendered_code_ast(::pltxt2htm::CodeAst<ndebug> const& ast,
                                              ::pltxt2htm::container::U8String& result) noexcept {
    for (auto const& node : ast.get_nodes()) {
        switch (ast.template get_node_kind<::pltxt2htm::CodeLanguage::rendered>(node)) /* -Werror=switch */ {
        case ::pltxt2htm::CodeRenderedNodeKind::text: {
            ::pltxt2htm::details::append_plweb_code_text<ndebug>(ast.get_text(node), result);
            break;
        }
        case ::pltxt2htm::CodeRenderedNodeKind::entity_reference: {
            result.template push_back<ndebug>(u8'&');
            result.template append<ndebug>(ast.get_text(node));
            result.template push_back<ndebug>(u8';');
            break;
        }
        case ::pltxt2htm::CodeRenderedNodeKind::style_begin: {
            ::pltxt2htm::details::append_plweb_rendered_style<ndebug>(ast.get_rendered_style(node), result);
            break;
        }
        case ::pltxt2htm::CodeRenderedNodeKind::style_end: {
            result.template append<ndebug>(u8"</span>");
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
constexpr void append_plweb_code_ast(::pltxt2htm::CodeAst<ndebug> const& ast,
                                     ::pltxt2htm::container::U8String& result) noexcept {
    if (ast.get_language() == ::pltxt2htm::CodeLanguage::rendered) {
        ::pltxt2htm::details::append_plweb_rendered_code_ast<ndebug>(ast, result);
        return;
    }
    ::pltxt2htm::details::append_plweb_language_code_ast<ndebug>(ast, result);
}

} // namespace pltxt2htm::details

#include "../../pop_macro.hh"
