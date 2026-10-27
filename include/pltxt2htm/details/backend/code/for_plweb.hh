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

constexpr void append_plweb_code_text(::pltxt2htm::container::U8StringView const text,
                                      ::pltxt2htm::container::U8String& result) noexcept {
    for (::std::size_t index{}; index != text.size(); ++index) {
        char8_t const chr{text[index]};
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

constexpr void append_plweb_code_unit(::pltxt2htm::Unit const unit, ::pltxt2htm::container::U8String& result) noexcept {
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

constexpr void append_plweb_rendered_style(::pltxt2htm::RenderedCodeStyle const& style,
                                           ::pltxt2htm::container::U8String& result) noexcept {
    result.append(u8"<span style=\"");
    if (style.get_color().is_empty() == false) {
        result.append(u8"color:");
        result.append(style.get_color());
        result.push_back(u8';');
    }
    if (style.get_font_size().has_value()) {
        auto const& font_size{style.get_font_size().value()};
        result.append(u8"font-size:");
        result.append(::pltxt2htm::details::double2str(font_size.value));
        ::pltxt2htm::details::append_plweb_code_unit(font_size.unit, result);
        result.push_back(u8';');
    }
    if (style.get_vertical_align().has_value()) {
        auto const& vertical_align{style.get_vertical_align().value()};
        result.append(u8"vertical-align:");
        if (vertical_align.get_kind() == ::pltxt2htm::VerticalAlignKind::keyword) {
            result.append(::pltxt2htm::details::vertical_align_keyword_string(vertical_align.get_keyword()));
        }
        else {
            result.append(::pltxt2htm::details::ptrdiff_t2str(vertical_align.get_length().value));
            ::pltxt2htm::details::append_plweb_code_unit(vertical_align.get_length().unit, result);
        }
        result.push_back(u8';');
    }
    result.append(u8"\">");
}

constexpr void append_plweb_highlighted_code_ast(::pltxt2htm::HighlightedCodeAst const& ast,
                                                 ::pltxt2htm::container::U8String& result) noexcept {
    for (auto const& node : ast.get_nodes()) {
        ::pltxt2htm::CodeHighlightKind const kind{node.get_kind()};
        if (kind != ::pltxt2htm::CodeHighlightKind::plain) {
            result.append(u8"<span style=\"color:");
            result.append(::pltxt2htm::details::code_style_color(kind));
            result.append(u8";\">");
        }
        ::pltxt2htm::details::append_plweb_code_text(ast.get_text(node), result);
        if (kind != ::pltxt2htm::CodeHighlightKind::plain) {
            result.append(u8"</span>");
        }
    }
}

constexpr void append_plweb_rendered_code_ast(::pltxt2htm::RenderedCodeAst const& ast,
                                              ::pltxt2htm::container::U8String& result) noexcept {
    for (auto const& node : ast.get_nodes()) {
        switch (node.get_kind()) /* -Werror=switch */ {
        case ::pltxt2htm::RenderedCodeNodeKind::text: {
            ::pltxt2htm::details::append_plweb_code_text(ast.get_text(node), result);
            break;
        }
        case ::pltxt2htm::RenderedCodeNodeKind::entity_reference: {
            result.push_back(u8'&');
            result.append(ast.get_text(node));
            result.push_back(u8';');
            break;
        }
        case ::pltxt2htm::RenderedCodeNodeKind::style_begin: {
            ::pltxt2htm::details::append_plweb_rendered_style(ast.get_style(node), result);
            break;
        }
        case ::pltxt2htm::RenderedCodeNodeKind::style_end: {
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

constexpr void append_plweb_code_ast(::pltxt2htm::HighlightedCodeAst const& ast,
                                     ::pltxt2htm::container::U8String& result) noexcept {
    ::pltxt2htm::details::append_plweb_highlighted_code_ast(ast, result);
}

constexpr void append_plweb_code_ast(::pltxt2htm::RenderedCodeAst const& ast,
                                     ::pltxt2htm::container::U8String& result) noexcept {
    ::pltxt2htm::details::append_plweb_rendered_code_ast(ast, result);
}

} // namespace pltxt2htm::details

#include "../../pop_macro.hh"
