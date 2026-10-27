/**
 * @file for_plunity.hh
 * @brief Render fenced-code ASTs as Unity TMP rich text.
 */

#pragma once

#include <cstddef>
#include "../../../ast/code/ast.hh"
#include "../../../container/string.hh"
#include "../../../container/string_view.hh"
#include "../../../container/vector.hh"
#include "../../utils.hh"
#include "style.hh"
#include "../../push_macro.hh"

namespace pltxt2htm::details {

constexpr void append_entity_reference_to_plunity_richtext(::pltxt2htm::container::U8StringView value,
                                                           ::pltxt2htm::container::U8String& out) noexcept;

constexpr void append_plunity_code_text(::pltxt2htm::container::U8StringView const text,
                                        ::pltxt2htm::container::U8String& result) noexcept {
    for (::std::size_t index{}; index != text.size(); ++index) {
        char8_t const chr{text[index]};
        switch (chr) {
        case u8' ': {
            result.append(u8"\u00A0");
            break;
        }
        case u8'<': {
            result.append(u8"<size=20>\uff1c</size>");
            break;
        }
        case u8'>': {
            result.append(u8"<size=20>\uff1e</size>");
            break;
        }
        default:
            result.push_back(chr);
            break;
        }
    }
}

class PlUnityRenderedCodeStyleState {
public:
    bool has_color{};
    bool has_font_size{};
    bool has_vertical_align{};
};

constexpr auto append_plunity_rendered_code_style(::pltxt2htm::RenderedCodeStyle const& style,
                                                  ::pltxt2htm::container::U8String& result) noexcept
    -> PlUnityRenderedCodeStyleState {
    bool const has_color{style.get_color().is_empty() == false};
    bool const has_font_size{style.get_font_size().has_value()};
    bool has_vertical_align{};
    if (style.get_vertical_align().has_value()) {
        auto const& vertical_align{style.get_vertical_align().value()};
        has_vertical_align = vertical_align.get_kind() == ::pltxt2htm::VerticalAlignKind::length &&
                             vertical_align.get_length().unit == ::pltxt2htm::Unit::px;
    }
    if (has_color) {
        result.append(u8"<color=");
        result.append(style.get_color());
        result.push_back(u8'>');
    }
    if (has_font_size) {
        auto const& font_size{style.get_font_size().value()};
        result.append(u8"<size=");
        switch (font_size.unit) /* -Werror=switch */ {
        case ::pltxt2htm::Unit::percent: {
            result.append(::pltxt2htm::details::double2str(font_size.value));
            result.push_back(u8'%');
            break;
        }
        case ::pltxt2htm::Unit::em: {
            result.append(::pltxt2htm::details::double2str(font_size.value));
            result.append(u8"em");
            break;
        }
        case ::pltxt2htm::Unit::px: {
            result.append(::pltxt2htm::details::double2str(font_size.value * 2));
            break;
        }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected rendered code font-size unit");
            }
#endif
        }
        result.push_back(u8'>');
    }
    if (has_vertical_align) {
        result.append(u8"<voffset=");
        result.append(::pltxt2htm::details::ptrdiff_t2str(style.get_vertical_align().value().get_length().value));
        result.push_back(u8'>');
    }
    return {.has_color = has_color, .has_font_size = has_font_size, .has_vertical_align = has_vertical_align};
}

constexpr void append_plunity_rendered_code_style_end(PlUnityRenderedCodeStyleState const state,
                                                      ::pltxt2htm::container::U8String& result) noexcept {
    if (state.has_vertical_align) {
        result.append(u8"</voffset>");
    }
    if (state.has_font_size) {
        result.append(u8"</size>");
    }
    if (state.has_color) {
        result.append(u8"</color>");
    }
}

constexpr void append_plunity_rendered_code_ast(::pltxt2htm::RenderedCodeAst const& ast,
                                                ::pltxt2htm::container::U8String& result) noexcept {
    ::pltxt2htm::container::Vector<::pltxt2htm::details::PlUnityRenderedCodeStyleState> style_stack{};
    for (auto const& node : ast.get_nodes()) {
        switch (node.get_kind()) /* -Werror=switch */ {
        case ::pltxt2htm::RenderedCodeNodeKind::text: {
            ::pltxt2htm::details::append_plunity_code_text(ast.get_text(node), result);
            break;
        }
        case ::pltxt2htm::RenderedCodeNodeKind::entity_reference: {
            ::pltxt2htm::details::append_entity_reference_to_plunity_richtext(ast.get_text(node), result);
            break;
        }
        case ::pltxt2htm::RenderedCodeNodeKind::style_begin: {
            style_stack.push_back(
                ::pltxt2htm::details::append_plunity_rendered_code_style(ast.get_style(node), result));
            break;
        }
        case ::pltxt2htm::RenderedCodeNodeKind::style_end: {
            pltxt2htm_assert(style_stack.is_empty() == false, u8"unmatched rendered code style end node");
            auto const state{style_stack[style_stack.size() - 1]};
            style_stack.pop_back();
            ::pltxt2htm::details::append_plunity_rendered_code_style_end(state, result);
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
    pltxt2htm_assert(style_stack.is_empty(), u8"unclosed rendered code style node");
}

constexpr void append_plunity_highlighted_code_ast(::pltxt2htm::HighlightedCodeAst const& ast,
                                                   ::pltxt2htm::container::U8String& result) noexcept {
    for (auto const& node : ast.get_nodes()) {
        ::pltxt2htm::CodeHighlightKind const kind{node.get_kind()};
        if (kind != ::pltxt2htm::CodeHighlightKind::plain) {
            result.append(u8"<color=");
            result.append(::pltxt2htm::details::code_style_color(kind));
            result.push_back(u8'>');
        }
        ::pltxt2htm::details::append_plunity_code_text(ast.get_text(node), result);
        if (kind != ::pltxt2htm::CodeHighlightKind::plain) {
            result.append(u8"</color>");
        }
    }
}

constexpr void append_plunity_code_ast(::pltxt2htm::HighlightedCodeAst const& ast,
                                       ::pltxt2htm::container::U8String& result) noexcept {
    ::pltxt2htm::details::append_plunity_highlighted_code_ast(ast, result);
}

constexpr void append_plunity_code_ast(::pltxt2htm::RenderedCodeAst const& ast,
                                       ::pltxt2htm::container::U8String& result) noexcept {
    ::pltxt2htm::details::append_plunity_rendered_code_ast(ast, result);
}

} // namespace pltxt2htm::details

#include "../../pop_macro.hh"
