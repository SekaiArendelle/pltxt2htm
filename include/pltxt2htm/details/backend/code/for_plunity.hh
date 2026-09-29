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

template<::pltxt2htm::Contracts ndebug>
constexpr void append_entity_reference_to_plunity_richtext(::pltxt2htm::container::U8StringView value,
                                                           ::pltxt2htm::container::U8String& out) noexcept;

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plunity_code_text(::pltxt2htm::container::U8StringView const text,
                                        ::pltxt2htm::container::U8String& result) noexcept {
    for (::std::size_t index{}; index != text.size(); ++index) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(text, index)};
        switch (chr) {
        case u8' ': {
            result.template append<ndebug>(u8"\u00A0");
            break;
        }
        case u8'<': {
            result.template append<ndebug>(u8"<size=20>\uff1c</size>");
            break;
        }
        case u8'>': {
            result.template append<ndebug>(u8"<size=20>\uff1e</size>");
            break;
        }
        default:
            result.template push_back<ndebug>(chr);
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

template<::pltxt2htm::Contracts ndebug>
constexpr auto append_plunity_rendered_code_style(::pltxt2htm::CodeRenderedStyle<ndebug> const& style,
                                                  ::pltxt2htm::container::U8String& result) noexcept
    -> PlUnityRenderedCodeStyleState {
    bool const has_color{style.get_color().is_empty() == false};
    bool const has_font_size{style.get_font_size().has_value()};
    bool has_vertical_align{};
    if (style.get_vertical_align().has_value()) {
        auto const& vertical_align{style.get_vertical_align().template value<ndebug>()};
        has_vertical_align = vertical_align.get_kind() == ::pltxt2htm::VerticalAlignKind::length &&
                             vertical_align.get_length().unit == ::pltxt2htm::Unit::px;
    }
    if (has_color) {
        result.template append<ndebug>(u8"<color=");
        result.template append<ndebug>(style.get_color());
        result.template push_back<ndebug>(u8'>');
    }
    if (has_font_size) {
        auto const& font_size{style.get_font_size().template value<ndebug>()};
        result.template append<ndebug>(u8"<size=");
        switch (font_size.unit) /* -Werror=switch */ {
        case ::pltxt2htm::Unit::percent: {
            result.template append<ndebug>(::pltxt2htm::details::double2str(font_size.value));
            result.template push_back<ndebug>(u8'%');
            break;
        }
        case ::pltxt2htm::Unit::em: {
            result.template append<ndebug>(::pltxt2htm::details::double2str(font_size.value));
            result.template append<ndebug>(u8"em");
            break;
        }
        case ::pltxt2htm::Unit::px: {
            result.template append<ndebug>(::pltxt2htm::details::double2str(font_size.value * 2));
            break;
        }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected rendered code font-size unit");
            }
#endif
        }
        result.template push_back<ndebug>(u8'>');
    }
    if (has_vertical_align) {
        result.template append<ndebug>(u8"<voffset=");
        result.template append<ndebug>(::pltxt2htm::details::ptrdiff_t2str(
            style.get_vertical_align().template value<ndebug>().get_length().value));
        result.template push_back<ndebug>(u8'>');
    }
    return {.has_color = has_color, .has_font_size = has_font_size, .has_vertical_align = has_vertical_align};
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plunity_rendered_code_style_end(PlUnityRenderedCodeStyleState const state,
                                                      ::pltxt2htm::container::U8String& result) noexcept {
    if (state.has_vertical_align) {
        result.template append<ndebug>(u8"</voffset>");
    }
    if (state.has_font_size) {
        result.template append<ndebug>(u8"</size>");
    }
    if (state.has_color) {
        result.template append<ndebug>(u8"</color>");
    }
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plunity_rendered_code_ast(::pltxt2htm::CodeAst<ndebug> const& ast,
                                                ::pltxt2htm::container::U8String& result) noexcept {
    ::pltxt2htm::container::Vector<::pltxt2htm::details::PlUnityRenderedCodeStyleState> style_stack{};
    for (auto const& node : ast.get_nodes()) {
        switch (ast.template get_node_kind<::pltxt2htm::CodeLanguage::rendered>(node)) /* -Werror=switch */ {
        case ::pltxt2htm::CodeRenderedNodeKind::text: {
            ::pltxt2htm::details::append_plunity_code_text<ndebug>(ast.get_text(node), result);
            break;
        }
        case ::pltxt2htm::CodeRenderedNodeKind::entity_reference: {
            ::pltxt2htm::details::append_entity_reference_to_plunity_richtext<ndebug>(ast.get_text(node), result);
            break;
        }
        case ::pltxt2htm::CodeRenderedNodeKind::style_begin: {
            style_stack.template push_back<ndebug>(
                ::pltxt2htm::details::append_plunity_rendered_code_style<ndebug>(ast.get_rendered_style(node), result));
            break;
        }
        case ::pltxt2htm::CodeRenderedNodeKind::style_end: {
            pltxt2htm_assert(style_stack.is_empty() == false, u8"unmatched rendered code style end node");
            auto const state{style_stack.template index<ndebug>(style_stack.size() - 1)};
            style_stack.template pop_back<ndebug>();
            ::pltxt2htm::details::append_plunity_rendered_code_style_end<ndebug>(state, result);
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

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plunity_language_code_ast(::pltxt2htm::CodeAst<ndebug> const& ast,
                                                ::pltxt2htm::container::U8String& result) noexcept {
    for (auto const& node : ast.get_nodes()) {
        CodeStyle const style{::pltxt2htm::details::code_style<ndebug>(ast, node)};
        if (style != CodeStyle::plain) {
            result.template append<ndebug>(u8"<color=");
            result.template append<ndebug>(::pltxt2htm::details::code_style_color<ndebug>(style));
            result.template push_back<ndebug>(u8'>');
        }
        ::pltxt2htm::details::append_plunity_code_text<ndebug>(ast.get_text(node), result);
        if (style != CodeStyle::plain) {
            result.template append<ndebug>(u8"</color>");
        }
    }
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plunity_code_ast(::pltxt2htm::CodeAst<ndebug> const& ast,
                                       ::pltxt2htm::container::U8String& result) noexcept {
    if (ast.get_language() == ::pltxt2htm::CodeLanguage::rendered) {
        ::pltxt2htm::details::append_plunity_rendered_code_ast<ndebug>(ast, result);
        return;
    }
    ::pltxt2htm::details::append_plunity_language_code_ast<ndebug>(ast, result);
}

} // namespace pltxt2htm::details

#include "../../pop_macro.hh"
