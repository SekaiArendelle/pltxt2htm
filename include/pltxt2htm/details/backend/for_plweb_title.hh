/**
 * @file for_plweb_title.hh
 * @brief HTML title backend for plweb – generates simple HTML suitable for page titles.
 * @details Provides `plweb_title_backend` for converting
 *          a pl-text AST into a simplified HTML fragment that only supports
 *          color, bold, and italic formatting.
 */
#pragma once

#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/array.h>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/vector.h>
#include "../../container/string.hh"
#include "../../container/string_view.hh"
#include "frame_context.hh"
#include "../utils.hh"
#include "../../ast/vertical_align_value.hh"
#include "../../contracts.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

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
constexpr auto plweb_title_backend(::pltxt2htm::Ast<ndebug> const& ast_init) noexcept
    -> ::pltxt2htm::container::u8string {
    ::pltxt2htm::container::u8string result{};
    ::fast_io::stack<BackendFrameContext<ndebug>> call_stack{};
    call_stack.push(BackendFrameContext<ndebug>(ast_init, ::pltxt2htm::NodeKind::text, 0));

entry:
    while (true) {
        auto&& current_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
        auto const& ast = current_frame.get_ast();
        auto&& current_index = current_frame.current_index;
        ::std::size_t const ast_size{ast.size()};
        for (; current_index < ast_size; ++current_index) {
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
            case ::pltxt2htm::NodeKind::entity_reference: {
                result.push_back(u8'&');
                result.append(node.as_entity_reference().get_value());
                result.push_back(u8';');
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
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_color().get_subast(), ::pltxt2htm::NodeKind::pl_color, 0));
                ++current_index;
                result.append(u8"<span style=\"color:");
                result.append(node.as_pl_color().get_color());
                result.append(u8";\">");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_a: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_a().get_subast(), ::pltxt2htm::NodeKind::pl_a, 0));
                ++current_index;
                constexpr auto open_tag = ::pltxt2htm::details::concat(U8LiteralString{u8"<span style=\"color:"},
                                                                       ::pltxt2htm::PlA<ndebug>::get_color_literal(),
                                                                       U8LiteralString{u8";\">"});
                result.append(::pltxt2htm::container::U8StringView{open_tag});
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_span: {
                auto const& span_color = node.as_html_span().get_color();
                auto const& span_font_size = node.as_html_span().get_font_size();
                auto const& span_vertical_align = node.as_html_span().get_vertical_align();
                bool const has_color = !span_color.empty();
                bool const has_font_size = span_font_size.has_value();
                bool const has_vertical_align = span_vertical_align.has_value();
                call_stack.push(BackendFrameContext<ndebug>(
                    node.as_html_span().get_subast(), 0,
                    BackendContextWithHtmlSpanInfo{.has_color = has_color,
                                                   .has_font_size = has_font_size,
                                                   .has_vertical_align = has_vertical_align}));
                ++current_index;
                result.append(u8"<span style=\"");
                if (has_color) {
                    result.append(u8"color:");
                    if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                        ::pltxt2htm::container::u8string purified_color{};
                        ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                            purified_color, ::pltxt2htm::container::U8StringView{span_color});
                        pltxt2htm_assert(
                            purified_color == span_color,
                            u8"Color value contains characters that cannot be directly used in HTML attributes.");
                    }
                    result.append(span_color);
                    result.push_back(u8';');
                }
                if (has_font_size) {
                    auto const& font_size = span_font_size.template value<ndebug>();
                    result.append(u8"font-size:");
                    result.append(::pltxt2htm::details::double2str(font_size.value));
                    switch (font_size.unit) /* -Werror=switch */ {
                    case ::pltxt2htm::Unit::percent: {
                        result.push_back(u8'%');
                        break;
                    }
                    case ::pltxt2htm::Unit::em: {
                        result.append(u8"em");
                        break;
                    }
                    case ::pltxt2htm::Unit::px: {
                        result.append(u8"px");
                        break;
                    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                    default:
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unexpected unit for font size");
                        }
#endif
                    }
                    result.push_back(u8';');
                }
                if (has_vertical_align) {
                    auto const& vertical_align = span_vertical_align.template value<ndebug>();
                    result.append(u8"vertical-align:");
                    if (vertical_align.get_kind() == ::pltxt2htm::VerticalAlignKind::keyword) {
                        result.append(
                            ::pltxt2htm::details::vertical_align_keyword_string<ndebug>(vertical_align.get_keyword()));
                    }
                    else {
                        result.append(::pltxt2htm::details::ptrdiff_t2str(vertical_align.get_length().value));
                        switch (vertical_align.get_length().unit) /* -Werror=switch */ {
                        case ::pltxt2htm::Unit::percent: {
                            result.push_back(u8'%');
                            break;
                        }
                        case ::pltxt2htm::Unit::em: {
                            result.append(u8"em");
                            break;
                        }
                        case ::pltxt2htm::Unit::px: {
                            result.append(u8"px");
                            break;
                        }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                        default:
                            [[unlikely]] {
                                pltxt2htm_unreachable(u8"Unexpected unit for vertical-align");
                            }
#endif
                        }
                    }
                    result.push_back(u8';');
                }
                result.append(u8"\">");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_a: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_a().get_subast(), ::pltxt2htm::NodeKind::html_a, 0));
                ++current_index;
                result.append(u8"<a href=\"");
                result.append(node.as_html_a().get_url().as_string());
                if (node.as_html_a().get_internal()) {
                    result.append(u8"\" internal>");
                }
                else {
                    result.append(u8"\">");
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_md_double_emphasis_underscore().get_subast(),
                                                            ::pltxt2htm::NodeKind::md_double_emphasis_underscore, 0));
                ++current_index;
                result.append(u8"<strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_md_double_emphasis_asterisk().get_subast(),
                                                            ::pltxt2htm::NodeKind::md_double_emphasis_asterisk, 0));
                ++current_index;
                result.append(u8"<strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_b: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_b().get_subast(), ::pltxt2htm::NodeKind::pl_b, 0));
                ++current_index;
                result.append(u8"<strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_strong: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_html_strong().get_subast(),
                                                            ::pltxt2htm::NodeKind::html_strong, 0));
                ++current_index;
                result.append(u8"<strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_md_single_emphasis_underscore().get_subast(),
                                                            ::pltxt2htm::NodeKind::md_single_emphasis_underscore, 0));
                ++current_index;
                result.append(u8"<em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_md_single_emphasis_asterisk().get_subast(),
                                                            ::pltxt2htm::NodeKind::md_single_emphasis_asterisk, 0));
                ++current_index;
                result.append(u8"<em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_i: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_i().get_subast(), ::pltxt2htm::NodeKind::pl_i, 0));
                ++current_index;
                result.append(u8"<em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_em: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_em().get_subast(), ::pltxt2htm::NodeKind::html_em, 0));
                ++current_index;
                result.append(u8"<em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_md_triple_emphasis_underscore().get_subast(),
                                                            ::pltxt2htm::NodeKind::md_triple_emphasis_underscore, 0));
                ++current_index;
                result.append(u8"<em><strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_md_triple_emphasis_asterisk().get_subast(),
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
            case ::pltxt2htm::NodeKind::html_hr: {
                pltxt2htm_unreachable(u8"Unexpected block node kind in title backend");
            }
            case ::pltxt2htm::NodeKind::html_note:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_col:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_img: {
                continue;
            }
            case ::pltxt2htm::NodeKind::text: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_text().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_experiment: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_experiment().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussion: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_discussion().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_experiments: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_experiments().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussions: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_discussions().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_user: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_user().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_trigger: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_trigger().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_internal: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_internal().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_size: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_size().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_voffset: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_voffset().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_align: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_align().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_margin: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_margin().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_div:
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
            case ::pltxt2htm::NodeKind::html_h6: {
                pltxt2htm_unreachable(u8"Unexpected block node kind in title backend");
            }
            case ::pltxt2htm::NodeKind::md_del: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_md_del().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_del: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_del().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_code: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_code().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_mark: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_mark().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_mark: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_mark().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_u: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_u().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_s: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_s().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sup: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_sup().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sub: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_sub().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_ul:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::list_ol:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::list_li_checkbox:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::list_li: {
                pltxt2htm_unreachable(u8"Unexpected block node kind in title backend");
            }
            case ::pltxt2htm::NodeKind::md_latex_inline: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_md_latex_inline().get_subast(),
                                                            ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_block: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_md_latex_block().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
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
            case ::pltxt2htm::NodeKind::table:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_tr:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_td:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_th:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_thead:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_tbody:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_tfoot:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_caption:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::table_colgroup:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_blockquote: {
                pltxt2htm_unreachable(u8"Unexpected block node kind in title backend");
            }
            case ::pltxt2htm::NodeKind::md_block_quotes:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::code_fence: {
                pltxt2htm_unreachable(u8"Unexpected block node kind in title backend");
            }
            case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_md_code_span_1_backtick().get_subast(),
                                                            ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_md_code_span_2_backtick().get_subast(),
                                                            ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_md_code_span_3_backtick().get_subast(),
                                                            ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_external: {
                result.append(u8"<a href=\"");
                result.append(node.as_pl_external().get_url().as_string());
                result.append(u8"\">");
                call_stack.push(BackendFrameContext<ndebug>(node.as_pl_external().get_subast(),
                                                            ::pltxt2htm::NodeKind::pl_external, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_link: {
                result.append(u8"<a href=\"");
                result.append(node.as_pl_link().get_url().as_string());
                result.append(u8"\">");
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_link().get_subast(), ::pltxt2htm::NodeKind::pl_link, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_link: {
                result.append(u8"<a href=\"");
                result.append(node.as_md_link().get_url().as_string());
                result.append(u8"\">");
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_md_link().get_subast(), ::pltxt2htm::NodeKind::md_link, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::url: {
                auto const& url_str = node.as_url().as_string();
                result.append(u8"<a href=\"");
                result.append(url_str);
                result.append(u8"\">");
                result.append(url_str);
                result.append(u8"</a>");
                continue;
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
            auto const top_frame =
                BackendFrameContext<ndebug>{::std::move(::pltxt2htm::details::stack_top<ndebug>(call_stack))};
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
            case ::pltxt2htm::NodeKind::pl_link:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_link:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::url:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_a: {
                result.append(u8"</a>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::text: {
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_trigger: {
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_internal: {
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
