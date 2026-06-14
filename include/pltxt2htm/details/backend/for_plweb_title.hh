#pragma once

#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/array.h>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "exception/exception.hh"
#include "frame_context.hh"
#include "../../contracts.hh"
#include "../../details/utils.hh"
#include "../../ast/ast.hh"

namespace pltxt2htm::details {

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto url_ast_to_string(::pltxt2htm::Ast<ndebug> const& ast) noexcept -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    for (auto&& node : ast) {
        switch (node.get_node_kind()) {
        case ::pltxt2htm::NodeKind::u8char: {
            result.push_back(node.get_u8char());
            continue;
        }
        case ::pltxt2htm::NodeKind::invalid_u8char: {
            result.append(u8"\uFFFD");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_backslash:
            result.push_back(u8'\\');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_exclamation:
            result.push_back(u8'!');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_double_quote:
            result.push_back(u8'"');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_hash:
            result.push_back(u8'#');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_dollar:
            result.push_back(u8'$');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_percent:
            result.push_back(u8'%');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_ampersand:
            result.push_back(u8'&');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_single_quote:
            result.push_back(u8'\'');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_left_paren:
            result.push_back(u8'(');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_right_paren:
            result.push_back(u8')');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_asterisk:
            result.push_back(u8'*');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_plus:
            result.push_back(u8'+');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_comma:
            result.push_back(u8',');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_hyphen:
            result.push_back(u8'-');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_dot:
            result.push_back(u8'.');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_slash:
            result.push_back(u8'/');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_colon:
            result.push_back(u8':');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_semicolon:
            result.push_back(u8';');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_less_than:
            result.push_back(u8'<');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_equals:
            result.push_back(u8'=');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_greater_than:
            result.push_back(u8'>');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_question:
            result.push_back(u8'?');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_at:
            result.push_back(u8'@');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_left_bracket:
            result.push_back(u8'[');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_right_bracket:
            result.push_back(u8']');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_caret:
            result.push_back(u8'^');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_underscore:
            result.push_back(u8'_');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_backtick:
            result.push_back(u8'`');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_left_brace:
            result.push_back(u8'{');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_pipe:
            result.push_back(u8'|');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_right_brace:
            result.push_back(u8'}');
            continue;
        case ::pltxt2htm::NodeKind::md_escape_tilde:
            result.push_back(u8'~');
            continue;
        case ::pltxt2htm::NodeKind::ampersand:
            result.push_back(u8'&');
            continue;
        case ::pltxt2htm::NodeKind::single_quote:
            result.push_back(u8'\'');
            continue;
        case ::pltxt2htm::NodeKind::double_quote:
            result.push_back(u8'"');
            continue;
        case ::pltxt2htm::NodeKind::less_than:
            result.push_back(u8'<');
            continue;
        case ::pltxt2htm::NodeKind::greater_than:
            result.push_back(u8'>');
            continue;
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }
    return result;
}

/**
 * @brief Translate pl-text's AST to common HTML (only supports color, b and i tags)
 * @details This backend generates simplified HTML output suitable for basic formatting.
 *          It only processes color tags, bold tags, and italic tags, ignoring
 *          more complex formatting like Markdown syntax or advanced HTML elements.
 *          This is typically used for rendering headers or simple text content.
 * @tparam ndebug Contract checking mode controlling assertion behavior.
 * @param[in] ast_init The AST to convert to HTML
 * @return A string containing the generated HTML
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto plweb_title_backend(::pltxt2htm::Ast<ndebug> const& ast_init) noexcept -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    ::fast_io::stack<::pltxt2htm::details::BackendFrameContext<ndebug>> call_stack{};
    call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(ast_init, ::pltxt2htm::NodeKind::text, 0));

entry:
    auto const& ast = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_ast();
    auto&& current_index = ::pltxt2htm::details::stack_top<ndebug>(call_stack).current_index;
    for (; current_index < ast.size(); ++current_index) {
        auto&& node = ::pltxt2htm::details::vector_index<ndebug>(ast, current_index);

        switch (node.get_node_kind()) /* -Werror=switch */ {
        case ::pltxt2htm::NodeKind::u8char: {
            result.push_back(node.get_u8char());
            continue;
        }
        case ::pltxt2htm::NodeKind::invalid_u8char: {
            result.append(u8"\uFFFD");
            continue;
        }
        case ::pltxt2htm::NodeKind::space: {
            result.append(u8"&nbsp;");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::ampersand: {
            result.append(u8"&amp;");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::single_quote: {
            result.append(u8"&apos;");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::double_quote: {
            result.append(u8"&quot;");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::less_than: {
            result.append(u8"&lt;");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::greater_than: {
            result.append(u8"&gt;");
            continue;
        }
        case ::pltxt2htm::NodeKind::tab: {
            result.append(u8"&nbsp;&nbsp;&nbsp;&nbsp;");
            continue;
        }
        case ::pltxt2htm::NodeKind::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_br: {
            continue;
        }
        case ::pltxt2htm::NodeKind::pl_color: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.get_subast(),
                                                                              ::pltxt2htm::NodeKind::pl_color, 0));
            ++current_index;
            result.append(u8"<span style=\"color:");
            result.append(node.get_equal_sign_tag_id());
            result.append(u8";\">");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_a: {
            call_stack.push(
                ::pltxt2htm::details::BackendFrameContext<ndebug>(node.get_subast(), ::pltxt2htm::NodeKind::pl_a, 0));
            ++current_index;
            constexpr auto open_tag = ::pltxt2htm::details::concat(
                ::pltxt2htm::details::U8LiteralString{u8"<span style=\"color:"},
                ::pltxt2htm::PlA<ndebug>::get_color_literal(), ::pltxt2htm::details::U8LiteralString{u8";\">"});
            result.append(::fast_io::u8string_view{open_tag.data(), open_tag.size()});
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_b:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_strong: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.get_subast(),
                                                                              ::pltxt2htm::NodeKind::html_strong, 0));
            ++current_index;
            result.append(u8"<strong>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_i:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_em: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.get_subast(),
                                                                              ::pltxt2htm::NodeKind::html_em, 0));
            ++current_index;
            result.append(u8"<em>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                node.get_subast(), ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk, 0));
            ++current_index;
            result.append(u8"<em><strong>");
            goto entry;
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
        case ::pltxt2htm::NodeKind::pl_macro_project: {
            result.append(u8"{Project}");
            continue;
        }
        case ::pltxt2htm::NodeKind::pl_macro_visitor: {
            result.append(u8"{Visitor}");
            continue;
        }
        case ::pltxt2htm::NodeKind::pl_macro_author: {
            result.append(u8"{Author}");
            continue;
        }
        case ::pltxt2htm::NodeKind::pl_macro_coauthors: {
            result.append(u8"{CoAuthors}");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_note:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_col: {
            continue;
        }
        case ::pltxt2htm::NodeKind::text:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_discussion:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_user:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_size:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_p:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_li_checkbox:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_code:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_inline:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_latex_block:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_atx_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_pre:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_blockquote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_table:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_td:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_table:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tr:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_td:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_th:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_thead:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tbody:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_tfoot:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_caption:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_colgroup:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_block_quotes:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_fence_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_fence_tilde:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_1_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_2_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
            call_stack.push(
                ::pltxt2htm::details::BackendFrameContext<ndebug>(node.get_subast(), ::pltxt2htm::NodeKind::text, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_external: {
            result.append(u8"<a href=\"");
            result.append(::pltxt2htm::details::url_ast_to_string<ndebug>(node.get_external_tag_url().get_url_ast()));
            result.append(u8"\">");
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.get_subast(),
                                                                              ::pltxt2htm::NodeKind::pl_external, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_link: {
            result.append(u8"<a href=\"");
            result.append(::pltxt2htm::details::url_ast_to_string<ndebug>(node.get_md_link_url().get_url_ast()));
            result.append(u8"\">");
            call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.get_subast(),
                                                                              ::pltxt2htm::NodeKind::md_link, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::url: {
            result.append(u8"<a href=\"");
            result.append(::pltxt2htm::details::url_ast_to_string<ndebug>(node.get_url_node().get_url_ast()));
            result.append(u8"\">");
            call_stack.push(
                ::pltxt2htm::details::BackendFrameContext<ndebug>(node.get_subast(), ::pltxt2htm::NodeKind::url, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_image: {
            ++current_index;
            goto entry;
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
        case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_strong:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_b: {
            result.append(u8"</strong>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_i:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::html_em: {
            result.append(u8"</em>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
            result.append(u8"</strong></em>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_a:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::pl_color: {
            result.append(u8"</span>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::pl_external:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::md_link:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::url: {
            result.append(u8"</a>");
            goto entry;
        }
        case ::pltxt2htm::NodeKind::text: {
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
