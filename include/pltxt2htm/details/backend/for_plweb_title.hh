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
#include "../push_macro.hh"

namespace pltxt2htm::details {

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto url_ast_to_string(::pltxt2htm::Ast<ndebug> const& ast) noexcept -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    for (auto&& node : ast) {
        switch (node.get_node_kind()) {
        case ::pltxt2htm::NodeKind::u8char: {
            result.push_back(node.as_u8char().chr);
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
                pltxt2htm_unreachable(u8"Unexpected node kind in title character rendering");
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
    while (true) {
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
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_color().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_color, 0));
                ++current_index;
                result.append(u8"<span style=\"color:");
                result.append(node.as_pl_color().get_color());
                result.append(u8";\">");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_a: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_a().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_a, 0));
                ++current_index;
                constexpr auto open_tag = ::pltxt2htm::details::concat(
                    ::pltxt2htm::details::U8LiteralString{u8"<span style=\"color:"},
                    ::pltxt2htm::PlA<ndebug>::get_color_literal(), ::pltxt2htm::details::U8LiteralString{u8";\">"});
                result.append(::fast_io::u8string_view{open_tag.data(), open_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_span: {
                auto const& span_color = node.as_html_span().get_color();
                auto const& span_font_size = node.as_html_span().get_font_size();
                bool const has_color = !span_color.empty();
                bool const has_font_size = span_font_size.has_value();
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_span().get_subast(), 0,
                    ::pltxt2htm::details::BackendContextWithHtmlSpanInfo{has_color, has_font_size}));
                ++current_index;
                result.append(u8"<span style=\"");
                if (has_color) {
                    result.append(u8"color:");
                    if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                        ::fast_io::u8string purified_color{};
                        ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                            purified_color, ::fast_io::u8string_view{span_color.data(), span_color.size()});
                        pltxt2htm_assert(
                            purified_color == span_color,
                            u8"Color value contains characters that cannot be directly used in HTML attributes.");
                    }
                    result.append(span_color);
                    result.push_back(u8';');
                }
                if (has_font_size) {
                    result.append(u8"font-size:");
                    result.append(::pltxt2htm::details::size_t2str(span_font_size.value()));
                    result.append(u8"px;");
                }
                result.append(u8"\">");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_double_emphasis_underscore().get_subast(),
                    ::pltxt2htm::NodeKind::md_double_emphasis_underscore, 0));
                ++current_index;
                result.append(u8"<strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_double_emphasis_asterisk().get_subast(),
                    ::pltxt2htm::NodeKind::md_double_emphasis_asterisk, 0));
                ++current_index;
                result.append(u8"<strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_b: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_b().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_b, 0));
                ++current_index;
                result.append(u8"<strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_strong: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_strong().get_subast(), ::pltxt2htm::NodeKind::html_strong, 0));
                ++current_index;
                result.append(u8"<strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_single_emphasis_underscore().get_subast(),
                    ::pltxt2htm::NodeKind::md_single_emphasis_underscore, 0));
                ++current_index;
                result.append(u8"<em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_single_emphasis_asterisk().get_subast(),
                    ::pltxt2htm::NodeKind::md_single_emphasis_asterisk, 0));
                ++current_index;
                result.append(u8"<em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_i: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_i().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_i, 0));
                ++current_index;
                result.append(u8"<em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_em: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_em().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_em, 0));
                ++current_index;
                result.append(u8"<em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_triple_emphasis_underscore().get_subast(),
                    ::pltxt2htm::NodeKind::md_triple_emphasis_underscore, 0));
                ++current_index;
                result.append(u8"<em><strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_triple_emphasis_asterisk().get_subast(),
                    ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk, 0));
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
            case ::pltxt2htm::NodeKind::text: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_text().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_experiment: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_experiment().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussion: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_discussion().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_user: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_user().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_size: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_size().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_p: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_p().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h1: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h1().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h2: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h2().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h3: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h3().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h4: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h4().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h5: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h5().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h6: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h6().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_del: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_del().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_del: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_del().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_ul: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_ul().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_ol: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_ol().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_li: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_li().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_ul: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_ul().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_ol: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_ol().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_li_checkbox: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_li_checkbox().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_li: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_li().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_code: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_code().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_inline: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_latex_inline().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_block: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_latex_block().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h1: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h1().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h2: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h2().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h3: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h3().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h4: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h4().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h5: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h5().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h6: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h6().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_pre: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_pre().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_blockquote: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_blockquote().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_table: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_table().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_thead: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_thead().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_tbody: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_tbody().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_tr: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_tr().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_th: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_th().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_td: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_td().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_table: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_table().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_tr: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_tr().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_td: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_td().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_th: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_th().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_thead: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_thead().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_tbody: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_tbody().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_tfoot: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_tfoot().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_caption: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_caption().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_colgroup: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_colgroup().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_block_quotes: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_block_quotes().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_fence_backtick: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_code_fence_backtick().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_fence_tilde: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_code_fence_tilde().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_code_span_1_backtick().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_code_span_2_backtick().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_code_span_3_backtick().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_external: {
                result.append(u8"<a href=\"");
                result.append(
                    ::pltxt2htm::details::url_ast_to_string<ndebug>(node.as_pl_external().get_url().get_url_ast()));
                result.append(u8"\">");
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_pl_external().get_subast(), ::pltxt2htm::NodeKind::pl_external, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_link: {
                result.append(u8"<a href=\"");
                result.append(
                    ::pltxt2htm::details::url_ast_to_string<ndebug>(node.as_md_link().get_url().get_url_ast()));
                result.append(u8"\">");
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_link().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_link, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::url: {
                result.append(u8"<a href=\"");
                result.append(::pltxt2htm::details::url_ast_to_string<ndebug>(node.as_url().get_url_ast()));
                result.append(u8"\">");
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_url().get_url_ast(),
                                                                                  ::pltxt2htm::NodeKind::url, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_image: {
                ++current_index;
                goto entry;
            }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected node kind in title backend");
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
            case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_strong:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_b: {
                result.append(u8"</strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_single_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_i:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_em: {
                result.append(u8"</em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
                result.append(u8"</strong></em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_span:
                [[fallthrough]];
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
                    pltxt2htm_unreachable(u8"Unexpected nested tag type in title backend");
                }
            }
        }
    }
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
