/**
 * @file for_plunity.hh
 * @brief Render fenced-code ASTs as Unity TMP rich text.
 */

#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../../../ast/code/ast.hh"
#include "../../utils.hh"
#include "style.hh"
#include "../../push_macro.hh"

namespace pltxt2htm::details {

template<::pltxt2htm::Contracts ndebug>
constexpr void append_entity_reference_to_plunity_richtext(::fast_io::u8string_view value,
                                                           ::fast_io::u8string& out) noexcept;

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plunity_code_text(::fast_io::u8string_view const text, ::fast_io::u8string& result) noexcept {
    for (::std::size_t index{}; index != text.size(); ++index) {
        char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(text, index)};
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

template<::pltxt2htm::Contracts ndebug>
constexpr auto append_plunity_rendered_code_style(::pltxt2htm::CodeRenderedStyle<ndebug> const& style,
                                                  ::fast_io::u8string& result) noexcept
    -> PlUnityRenderedCodeStyleState {
    bool const has_color{style.get_color().empty() == false};
    bool const has_font_size{style.get_font_size().has_value()};
    bool has_vertical_align{};
    if (style.get_vertical_align().has_value()) {
        auto const& vertical_align{style.get_vertical_align().template value<ndebug>()};
        has_vertical_align = vertical_align.get_kind() == ::pltxt2htm::VerticalAlignKind::length &&
                             vertical_align.get_length().unit == ::pltxt2htm::Unit::px;
    }
    if (has_color) {
        result.append(u8"<color=");
        result.append(style.get_color());
        result.push_back(u8'>');
    }
    if (has_font_size) {
        auto const& font_size{style.get_font_size().template value<ndebug>()};
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
        result.append(::pltxt2htm::details::ptrdiff_t2str(
            style.get_vertical_align().template value<ndebug>().get_length().value));
        result.push_back(u8'>');
    }
    return {.has_color = has_color, .has_font_size = has_font_size, .has_vertical_align = has_vertical_align};
}

constexpr void append_plunity_rendered_code_style_end(PlUnityRenderedCodeStyleState const state,
                                                      ::fast_io::u8string& result) noexcept {
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

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plunity_rendered_code_ast(::pltxt2htm::CodeAst<ndebug> const& ast,
                                                ::fast_io::u8string& result) noexcept {
    ::fast_io::stack<::pltxt2htm::details::PlUnityRenderedCodeStyleState> style_stack{};
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
            style_stack.push(
                ::pltxt2htm::details::append_plunity_rendered_code_style<ndebug>(ast.get_rendered_style(node), result));
            break;
        }
        case ::pltxt2htm::CodeRenderedNodeKind::style_end: {
            pltxt2htm_assert(style_stack.empty() == false, u8"unmatched rendered code style end node");
            auto const state{::pltxt2htm::details::stack_top<ndebug>(style_stack)};
            style_stack.pop();
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
    pltxt2htm_assert(style_stack.empty(), u8"unclosed rendered code style node");
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plunity_language_code_ast(::pltxt2htm::CodeAst<ndebug> const& ast,
                                                ::fast_io::u8string& result) noexcept {
    for (auto const& node : ast.get_nodes()) {
        CodeStyle const style{::pltxt2htm::details::code_style<ndebug>(ast, node)};
        if (style != CodeStyle::plain) {
            result.append(u8"<color=");
            result.append(::pltxt2htm::details::code_style_color<ndebug>(style));
            result.push_back(u8'>');
        }
        ::pltxt2htm::details::append_plunity_code_text<ndebug>(ast.get_text(node), result);
        if (style != CodeStyle::plain) {
            result.append(u8"</color>");
        }
    }
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_plunity_code_ast(::pltxt2htm::CodeAst<ndebug> const& ast, ::fast_io::u8string& result) noexcept {
    if (ast.get_language() == ::pltxt2htm::CodeLanguage::rendered) {
        ::pltxt2htm::details::append_plunity_rendered_code_ast<ndebug>(ast, result);
        return;
    }
    ::pltxt2htm::details::append_plunity_language_code_ast<ndebug>(ast, result);
}

} // namespace pltxt2htm::details

#include "../../pop_macro.hh"
