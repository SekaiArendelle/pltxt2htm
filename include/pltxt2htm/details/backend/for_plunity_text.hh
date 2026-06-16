/**
 * @file for_plunity_text.hh
 * @brief Advanced HTML backend for pltxt2htm
 * @details Generates full-featured HTML output with comprehensive support for
 *          Physics-Lab tags, Markdown syntax, and HTML elements.
 */

#pragma once

#include <cstddef>
#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/array.h>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "frame_context.hh"
#include "../../contracts.hh"
#include "../../details/utils.hh"
#include "../../ast/ast.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto convert_simple_pltxt_ast_to_plunity_richtext(::pltxt2htm::Ast<ndebug> const& url) noexcept
    -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    for (auto&& node : url) {
        switch (node.get_node_kind()) {
        case ::pltxt2htm::NodeKind::u8char: {
            result.push_back(node.as_u8char().chr);
            continue;
        }
        case ::pltxt2htm::NodeKind::invalid_u8char: {
            result.append(u8"\uFFFD");
            continue;
        }
        case ::pltxt2htm::NodeKind::space: {
            result.push_back(u8' ');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::ampersand: {
            result.push_back(u8'&');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::single_quote: {
            result.push_back(u8'\'');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::double_quote: {
            result.push_back(u8'\"');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::less_than: {
            result.push_back(u8'<');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::greater_than: {
            result.push_back(u8'>');
            continue;
        }
        case ::pltxt2htm::NodeKind::tab: {
            result.push_back(u8'\t');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_backslash: {
            result.push_back(u8'\\');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_exclamation: {
            result.push_back(u8'!');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_hash: {
            result.push_back(u8'#');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_dollar: {
            result.push_back(u8'$');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_percent: {
            result.push_back(u8'%');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_paren: {
            result.push_back(u8'(');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_paren: {
            result.push_back(u8')');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_asterisk: {
            result.push_back(u8'*');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_plus: {
            result.push_back(u8'+');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_comma: {
            result.push_back(u8',');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_hyphen: {
            result.push_back(u8'-');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_dot: {
            result.push_back(u8'.');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_slash: {
            result.push_back(u8'/');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_colon: {
            result.push_back(u8':');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_semicolon: {
            result.push_back(u8';');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_equals: {
            result.push_back(u8'=');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_question: {
            result.push_back(u8'?');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_at: {
            result.push_back(u8'@');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_bracket: {
            result.push_back(u8'[');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_bracket: {
            result.push_back(u8']');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_caret: {
            result.push_back(u8'^');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_underscore: {
            result.push_back(u8'_');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_backtick: {
            result.push_back(u8'`');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_brace: {
            result.push_back(u8'{');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_pipe: {
            result.push_back(u8'|');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_brace: {
            result.push_back(u8'}');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_tilde: {
            result.push_back(u8'~');
            continue;
        }
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }

    return result;
}

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto plunity_text_backend(::pltxt2htm::Ast<ndebug> const& ast_init, ::fast_io::u8string_view project,
                                    ::fast_io::u8string_view visitor, ::fast_io::u8string_view author,
                                    ::fast_io::u8string_view coauthors) noexcept -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    ::fast_io::stack<::pltxt2htm::details::BackendFrameContext<ndebug>> call_stack{};
    call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(ast_init, ::pltxt2htm::NodeKind::text, 0));
    ::std::size_t list_nesting_depth{};

entry:
    auto const& ast = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_ast();
    auto&& current_index = ::pltxt2htm::details::stack_top<ndebug>(call_stack).current_index;
    for (; current_index < ast.size(); ++current_index) {
        auto&& node = ::pltxt2htm::details::vector_index<ndebug>(ast, current_index);

        switch (node.get_node_kind()) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::u8char: {
            result.push_back(node.as_u8char().chr);
            continue;
        }
        case ::pltxt2htm::NodeKind::invalid_u8char: {
            result.append(u8"\uFFFD");
            continue;
        }
        case ::pltxt2htm::NodeKind::text: {
            call_stack.push(
                ::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_text().get_subast(), ::pltxt2htm::NodeKind::text, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::space: {
            result.push_back(u8' ');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::ampersand: {
            result.push_back(u8'&');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::single_quote: {
            result.push_back(u8'\'');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::double_quote: {
            result.push_back(u8'\"');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::less_than: {
            result.append(u8"<size=20>\uff1c</size>");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::greater_than: {
            result.append(u8"<size=20>\uff1e</size>");
            continue;
        }
        case ::pltxt2htm::NodeKind::tab: {
            result.push_back(u8'\t');
            continue;
        }
        case ::pltxt2htm::NodeKind::pl_color: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_color().get_subast(),
                                                                               ::pltxt2htm::NodeKind::pl_color, 0));
            ++current_index;
            result.append(u8"<color=");
            result.append(node.as_pl_color().get_color());
            result.push_back(u8'>');
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_a: {
            call_stack.push(
                ::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_a().get_subast(), ::pltxt2htm::NodeKind::pl_a, 0));
            ++current_index;
            constexpr auto open_tag = ::pltxt2htm::details::concat(::pltxt2htm::details::U8LiteralString{u8"<color="},
                                                                   ::pltxt2htm::PlA<ndebug>::get_color_literal(),
                                                                   ::pltxt2htm::details::U8LiteralString{u8">"});
            result.append(::fast_io::u8string_view{open_tag.data(), open_tag.size()});
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_experiment: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_experiment().get_subast(),
                                                                               ::pltxt2htm::NodeKind::pl_experiment, 0));
            ++current_index;
            result.append(u8"<experiment=");
            result.append(node.as_pl_experiment().get_id());
            result.append(u8">");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_discussion: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_discussion().get_subast(),
                                                                               ::pltxt2htm::NodeKind::pl_discussion, 0));
            ++current_index;
            result.append(u8"<discussion=");
            result.append(node.as_pl_discussion().get_id());
            result.append(u8">");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_external().get_subast(),
                                                                               ::pltxt2htm::NodeKind::pl_external, 0));
            ++current_index;
            result.append(u8"<external=");
            result.append(::pltxt2htm::details::convert_simple_pltxt_ast_to_plunity_richtext<ndebug>(
                node.as_pl_external().get_url().get_url_ast()));
            result.append(u8">");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_user: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_user().get_subast(),
                                                                               ::pltxt2htm::NodeKind::pl_user, 0));
            ++current_index;
            result.append(u8"<user=");
            result.append(node.as_pl_user().get_id());
            result.push_back(u8'>');
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_size().get_subast(),
                                                                               ::pltxt2htm::NodeKind::pl_size, 0));
            ++current_index;
            result.append(u8"<size=");
            result.append(::pltxt2htm::details::size_t2str(node.as_pl_size().get_size()));
            result.push_back(u8'>');
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_double_emphasis_underscore().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_strong, 0));
            ++current_index;
            result.append(u8"<b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_double_emphasis_asterisk().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_strong, 0));
            ++current_index;
            result.append(u8"<b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_b: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_b().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_strong, 0));
            ++current_index;
            result.append(u8"<b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_strong: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_strong().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_strong, 0));
            ++current_index;
            result.append(u8"<b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_p: {
            call_stack.push(
                ::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_p().get_subast(), ::pltxt2htm::NodeKind::html_p, 0));
            ++current_index;
            result.append(u8"<p>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_br:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::line_break: {
            result.push_back('\n');
            continue;
        }
        case ::pltxt2htm::NodeKind::html_h1: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h1().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_h1, 0));
            ++current_index;
            result.append(u8"<size=38><b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h1: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h1().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_h1, 0));
            ++current_index;
            result.append(u8"<size=38><b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h2: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h2().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_h2, 0));
            ++current_index;
            result.append(u8"<size=37><b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h2: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h2().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_h2, 0));
            ++current_index;
            result.append(u8"<size=37><b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h3: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h3().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_h3, 0));
            ++current_index;
            result.append(u8"<size=36><b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h3: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h3().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_h3, 0));
            ++current_index;
            result.append(u8"<size=36><b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h4: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h4().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_h4, 0));
            ++current_index;
            result.append(u8"<size=35><b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h4: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h4().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_h4, 0));
            ++current_index;
            result.append(u8"<size=35><b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h5: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h5().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_h5, 0));
            ++current_index;
            result.append(u8"<b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h5: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h5().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_h5, 0));
            ++current_index;
            result.append(u8"<b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h6: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h6().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_h6, 0));
            ++current_index;
            result.append(u8"<b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_atx_h6: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h6().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_h6, 0));
            ++current_index;
            result.append(u8"<b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_del: {
            call_stack.push(
                ::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_del().get_subast(), ::pltxt2htm::NodeKind::md_del, 0));
            ++current_index;
            result.append(u8"~~");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_del: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_del().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_del, 0));
            ++current_index;
            result.append(u8"<del>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_single_emphasis_underscore().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_em, 0));
            ++current_index;
            result.append(u8"<i>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_single_emphasis_asterisk().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_em, 0));
            ++current_index;
            result.append(u8"<i>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_i: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_i().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_em, 0));
            ++current_index;
            result.append(u8"<i>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_em: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_em().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_em, 0));
            ++current_index;
            result.append(u8"<i>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_hr: {
            result.append(u8" ---\n");
            continue;
        }
        case ::pltxt2htm::NodeKind::html_hr: {
            result.append(u8"<hr>");
            continue;
        }
        case ::pltxt2htm::NodeKind::html_note: {
            continue;
        }
        case ::pltxt2htm::NodeKind::md_ul: {
            {
                auto const parent_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
                if (parent_tag_type == ::pltxt2htm::NodeKind::html_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li_checkbox) {
                    result.push_back(u8'\n');
                }
            }
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_ul().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_ul, 0));
            ++current_index;
            ++list_nesting_depth;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_ul: {
            {
                auto const parent_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
                if (parent_tag_type == ::pltxt2htm::NodeKind::html_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li_checkbox) {
                    result.push_back(u8'\n');
                }
            }
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_ul().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_ul, 0));
            ++current_index;
            ++list_nesting_depth;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_ol: {
            {
                auto const parent_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
                if (parent_tag_type == ::pltxt2htm::NodeKind::html_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li_checkbox) {
                    result.push_back(u8'\n');
                }
            }
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                node.as_md_ol().get_subast(), 0, ::pltxt2htm::details::BackendContextWithOlInfo{}));
            ++current_index;
            ++list_nesting_depth;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_ol: {
            {
                auto const parent_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
                if (parent_tag_type == ::pltxt2htm::NodeKind::html_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li_checkbox) {
                    result.push_back(u8'\n');
                }
            }
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                node.as_html_ol().get_subast(), 0, ::pltxt2htm::details::BackendContextWithOlInfo{}));
            ++current_index;
            ++list_nesting_depth;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_li: {
            auto const nested_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
            pltxt2htm_assert(
                nested_tag_type == ::pltxt2htm::NodeKind::html_ol || nested_tag_type == ::pltxt2htm::NodeKind::html_ul,
                u8"Invalid tag type");
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_li().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_li, 0));
            ++current_index;
            auto const indent_level = list_nesting_depth;
            for (::std::size_t i = 1; i < indent_level; ++i) {
                result.append(u8"  ");
            }
            auto reverse_iter = call_stack.container.rbegin();
            ::pltxt2htm::details::BackendFrameContext<ndebug>& the_second_to_last_frame{*(++reverse_iter)};
            auto const second_nested_tag_type = the_second_to_last_frame.get_nested_tag_type();
            if (second_nested_tag_type == ::pltxt2htm::NodeKind::html_ol || second_nested_tag_type == ::pltxt2htm::NodeKind::md_ol) {
                ::std::size_t& ol_li_count = the_second_to_last_frame.get_ol_li_count();
                result.append(::pltxt2htm::details::size_t2str(ol_li_count));
                result.append(u8". ");
                ++ol_li_count;
            }
            else if (nested_tag_type == ::pltxt2htm::NodeKind::html_ul ||
                     nested_tag_type == ::pltxt2htm::NodeKind::md_ul) {
                if (node.get_node_kind() == ::pltxt2htm::NodeKind::md_li_checkbox) {
                    if (node.as_md_li_checkbox().is_checked()) {
                        result.append(u8"[x] ");
                    }
                    else {
                        result.append(u8"[ ] ");
                    }
                }
                else if (indent_level % 3 == 1) {
                    result.append(u8"\u2022 ");
                }
                else if (indent_level % 3 == 2) {
                    result.append(u8"\u2218 ");
                }
                else if (indent_level % 3 == 0) {
                    result.append(u8"\u25ab ");
                }
                else {
                    ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
                }
            }
            else {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_li_checkbox: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_li_checkbox().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_li, 0));
            ++current_index;
            auto const indent_level = list_nesting_depth;
            for (::std::size_t i = 1; i < indent_level; ++i) {
                result.append(u8"  ");
            }
            auto reverse_iter = call_stack.container.rbegin();
            ::pltxt2htm::details::BackendFrameContext<ndebug>& the_second_to_last_frame{*(++reverse_iter)};
            auto const nested_tag_type = the_second_to_last_frame.get_nested_tag_type();
            if (nested_tag_type == ::pltxt2htm::NodeKind::html_ol || nested_tag_type == ::pltxt2htm::NodeKind::md_ol) {
                ::std::size_t& ol_li_count = the_second_to_last_frame.get_ol_li_count();
                result.append(::pltxt2htm::details::size_t2str(ol_li_count));
                result.append(u8". ");
                ++ol_li_count;
            }
            else if (nested_tag_type == ::pltxt2htm::NodeKind::html_ul ||
                     nested_tag_type == ::pltxt2htm::NodeKind::md_ul) {
                if (node.get_node_kind() == ::pltxt2htm::NodeKind::md_li_checkbox) {
                    if (node.as_md_li_checkbox().is_checked()) {
                        result.append(u8"[x] ");
                    }
                    else {
                        result.append(u8"[ ] ");
                    }
                }
                else if (indent_level % 3 == 1) {
                    result.append(u8"\u2022 ");
                }
                else if (indent_level % 3 == 2) {
                    result.append(u8"\u2218 ");
                }
                else if (indent_level % 3 == 0) {
                    result.append(u8"\u25ab ");
                }
                else {
                    ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
                }
            }
            else {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_li: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_li().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_li, 0));
            ++current_index;
            auto const indent_level = list_nesting_depth;
            for (::std::size_t i = 1; i < indent_level; ++i) {
                result.append(u8"  ");
            }
            auto reverse_iter = call_stack.container.rbegin();
            ::pltxt2htm::details::BackendFrameContext<ndebug>& the_second_to_last_frame{*(++reverse_iter)};
            auto const nested_tag_type = the_second_to_last_frame.get_nested_tag_type();
            if (nested_tag_type == ::pltxt2htm::NodeKind::html_ol || nested_tag_type == ::pltxt2htm::NodeKind::md_ol) {
                ::std::size_t& ol_li_count = the_second_to_last_frame.get_ol_li_count();
                result.append(::pltxt2htm::details::size_t2str(ol_li_count));
                result.append(u8". ");
                ++ol_li_count;
            }
            else if (nested_tag_type == ::pltxt2htm::NodeKind::html_ul ||
                     nested_tag_type == ::pltxt2htm::NodeKind::md_ul) {
                if (indent_level % 3 == 1) {
                    result.append(u8"\u2022 ");
                }
                else if (indent_level % 3 == 2) {
                    result.append(u8"\u2218 ");
                }
                else if (indent_level % 3 == 0) {
                    result.append(u8"\u25ab ");
                }
                else {
                    ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
                }
            }
            else {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                node.as_md_code_span_1_backtick().get_subast(), ::pltxt2htm::NodeKind::md_code_span_1_backtick, 0));
            ++current_index;
            result.push_back(u8' ');
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                node.as_md_code_span_2_backtick().get_subast(), ::pltxt2htm::NodeKind::md_code_span_1_backtick, 0));
            ++current_index;
            result.push_back(u8' ');
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                node.as_md_code_span_3_backtick().get_subast(), ::pltxt2htm::NodeKind::md_code_span_1_backtick, 0));
            ++current_index;
            result.push_back(u8' ');
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_code: {
            // Note: Despite `<code></code>` is empty, we still need to handle it
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_code().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_code, 0));
            ++current_index;
            result.append(u8"<code>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_latex_inline: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                node.as_md_latex_inline().get_subast(), ::pltxt2htm::NodeKind::md_latex_inline, 0));
            ++current_index;
            result.append(u8"$");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_latex_block: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                node.as_md_latex_block().get_subast(), ::pltxt2htm::NodeKind::md_latex_block, 0));
            ++current_index;
            result.append(u8"$$");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_pre: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_pre().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_pre, 0));
            ++current_index;
            result.append(u8"<pre>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                node.as_md_block_quotes().get_subast(), ::pltxt2htm::NodeKind::md_block_quotes, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_blockquote: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                node.as_html_blockquote().get_subast(), ::pltxt2htm::NodeKind::html_blockquote, 0));
            ++current_index;
            result.append(u8"<blockquote>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_table: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_table().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_table, 0));
            ++current_index;
            result.append(u8"<table>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_table: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_table().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_table, 0));
            ++current_index;
            result.append(u8"<table>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_tr: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_tr().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_tr, 0));
            ++current_index;
            result.append(u8"<tr>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_tr: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_tr().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_tr, 0));
            ++current_index;
            result.append(u8"<tr>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_td: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_td().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_td, 0));
            ++current_index;
            result.append(u8"<td");
            auto const align = node.as_md_td().get_align();
            if (align == ::pltxt2htm::MdTableAlign::center) {
                result.append(u8" style=\"text-align:center\"");
            }
            else if (align == ::pltxt2htm::MdTableAlign::right) {
                result.append(u8" style=\"text-align:right\"");
            }
            result.push_back(u8'>');
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_td: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_td().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_td, 0));
            ++current_index;
            result.append(u8"<td>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_th: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_th().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_th, 0));
            ++current_index;
            result.append(u8"<th");
            auto const align = node.as_md_th().get_align();
            if (align == ::pltxt2htm::MdTableAlign::center) {
                result.append(u8" style=\"text-align:center\"");
            }
            else if (align == ::pltxt2htm::MdTableAlign::right) {
                result.append(u8" style=\"text-align:right\"");
            }
            result.push_back(u8'>');
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_th: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_th().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_th, 0));
            ++current_index;
            result.append(u8"<th>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_thead: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_thead().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_thead, 0));
            ++current_index;
            result.append(u8"<thead>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_thead: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_thead().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_thead, 0));
            ++current_index;
            result.append(u8"<thead>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_tbody: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_tbody().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_tbody, 0));
            ++current_index;
            result.append(u8"<tbody>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_tbody: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_tbody().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_tbody, 0));
            ++current_index;
            result.append(u8"<tbody>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_tfoot: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_tfoot().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_tfoot, 0));
            ++current_index;
            result.append(u8"<tfoot>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_caption: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_caption().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_caption, 0));
            ++current_index;
            result.append(u8"<caption>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_colgroup: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_colgroup().get_subast(),
                                                                               ::pltxt2htm::NodeKind::html_colgroup, 0));
            ++current_index;
            result.append(u8"<colgroup>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_col: {
            result.append(u8"<col>");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                node.as_md_triple_emphasis_underscore().get_subast(), ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk, 0));
            ++current_index;
            result.append(u8"<b><i>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                node.as_md_triple_emphasis_asterisk().get_subast(), ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk, 0));
            ++current_index;
            result.append(u8"<b><i>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::url: {
            call_stack.push(
                ::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_url().get_url_ast(), ::pltxt2htm::NodeKind::url, 0));
            ++current_index;
            result.append(u8"<external=");
            result.append(::pltxt2htm::details::convert_simple_pltxt_ast_to_plunity_richtext<ndebug>(
                node.as_url().get_url_ast()));
            result.push_back(u8'>');
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_link: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_link().get_subast(),
                                                                               ::pltxt2htm::NodeKind::md_link, 0));
            ++current_index;
            result.append(u8"<external=");
            result.append(::pltxt2htm::details::convert_simple_pltxt_ast_to_plunity_richtext<ndebug>(
                node.as_md_link().get_url().get_url_ast()));
            result.push_back(u8'>');
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_image: {
            result.append(u8"![");
            result.append(
                ::pltxt2htm::details::convert_simple_pltxt_ast_to_plunity_richtext<ndebug>(node.as_md_image().get_subast()));
            result.append(u8"](");
            result.append(::pltxt2htm::details::convert_simple_pltxt_ast_to_plunity_richtext<ndebug>(
                node.as_md_image().get_url().get_url_ast()));
            result.push_back(u8')');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_backslash: {
            result.push_back(u8'\\');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_exclamation: {
            result.push_back(u8'!');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_hash: {
            result.push_back(u8'#');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_dollar: {
            result.push_back(u8'$');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_percent: {
            result.push_back(u8'%');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_paren: {
            result.push_back(u8'(');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_paren: {
            result.push_back(u8')');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_asterisk: {
            result.push_back(u8'*');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_plus: {
            result.push_back(u8'+');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_comma: {
            result.push_back(u8',');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_hyphen: {
            result.push_back(u8'-');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_dot: {
            result.push_back(u8'.');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_slash: {
            result.push_back(u8'/');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_colon: {
            result.push_back(u8':');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_semicolon: {
            result.push_back(u8';');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_equals: {
            result.push_back(u8'=');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_question: {
            result.push_back(u8'?');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_at: {
            result.push_back(u8'@');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_bracket: {
            result.push_back(u8'[');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_bracket: {
            result.push_back(u8']');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_caret: {
            result.push_back(u8'^');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_underscore: {
            result.push_back(u8'_');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_backtick: {
            result.push_back(u8'`');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_brace: {
            result.push_back(u8'{');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_pipe: {
            result.push_back(u8'|');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_brace: {
            result.push_back(u8'}');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_tilde: {
            result.push_back(u8'~');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_code_fence_backtick: {
            result.append(u8"```");
            auto const& opt_language = node.as_md_code_fence_backtick().get_language();
            if (opt_language.has_value()) {
                result.append(opt_language.template value<ndebug == ::pltxt2htm::Contracts::ignore>());
            }
            result.push_back(u8'\n');
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                node.as_md_code_fence_backtick().get_subast(), ::pltxt2htm::NodeKind::md_code_fence_backtick, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_code_fence_tilde: {
            result.append(u8"```");
            auto const& opt_language = node.as_md_code_fence_tilde().get_language();
            if (opt_language.has_value()) {
                result.append(opt_language.template value<ndebug == ::pltxt2htm::Contracts::ignore>());
            }
            result.push_back(u8'\n');
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                node.as_md_code_fence_tilde().get_subast(), ::pltxt2htm::NodeKind::md_code_fence_backtick, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_macro_project: {
            result.append(project);
            continue;
        }
        case ::pltxt2htm::NodeKind::pl_macro_visitor: {
            result.append(visitor);
            continue;
        }
        case ::pltxt2htm::NodeKind::pl_macro_author: {
            result.append(author);
            continue;
        }
        case ::pltxt2htm::NodeKind::pl_macro_coauthors: {
            result.append(coauthors);
            continue;
        }
// All enum values are covered above; default is unreachable.
// #if 0 documents intent and keeps the formatter output clean.
#if 0
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
#endif
        }
    }

    {
        auto top_frame = ::std::move(::pltxt2htm::details::stack_top<ndebug>(call_stack));
        call_stack.pop();
        if (call_stack.empty()) {
            return result;
        }
        switch (top_frame.get_nested_tag_type()) {
        case ::pltxt2htm::NodeKind::text: {
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_a:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_color: {
            result.append(u8"</color>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_experiment: {
            result.append(u8"</experiment>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_discussion: {
            result.append(u8"</discussion>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            result.append(u8"</external>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_user: {
            result.append(u8"</user>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_size: {
            result.append(u8"</size>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_b:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_strong: {
            result.append(u8"</b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_i:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_em: {
            result.append(u8"</i>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
            result.append(u8"</i></b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_p: {
            result.append(u8"</p>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h1: {
            result.append(u8"</b></size>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h2: {
            result.append(u8"</b></size>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h3: {
            result.append(u8"</b></size>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h4: {
            result.append(u8"</b></size>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h5: {
            result.append(u8"</b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_h6: {
            result.append(u8"</b>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_del: {
            result.append(u8"~~");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_del: {
            result.append(u8"</del>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_ol: {
            --list_nesting_depth;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_li_checkbox:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_li: {
            result.push_back(u8'\n');
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
            result.push_back(u8' ');
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_code: {
            result.append(u8"</code>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_latex_inline: {
            result.append(u8"$");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_latex_block: {
            result.append(u8"$$");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_pre: {
            result.append(u8"</pre>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_block_quotes: {
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_table:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_table: {
            result.append(u8"</table>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tr: {
            result.append(u8"</tr>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_td:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_td: {
            result.append(u8"</td>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_th: {
            result.append(u8"</th>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_thead: {
            result.append(u8"</thead>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tbody: {
            result.append(u8"</tbody>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_tfoot: {
            result.append(u8"</tfoot>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_caption: {
            result.append(u8"</caption>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_colgroup: {
            result.append(u8"</colgroup>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::html_blockquote: {
            result.append(u8"</blockquote>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_code_fence_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_fence_tilde: {
            result.append(u8"\n```");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_link:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::url: {
            result.append(u8"</external>");
            goto entry;
        }
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
