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
#include "../../ast/value_unit.hh"
#include "../../ast/vertical_align_value.hh"
#include "frame_context.hh"
#include "../../contracts.hh"
#include "../../details/utils.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Convert a simple (leaf-only) AST to Unity Rich Text with unescaping.
 * @tparam ndebug Contract checking mode.
 * @param ast The AST to convert (should only contain leaf/character-like nodes).
 * @param[out] out Output buffer receiving the Unity Rich Text string.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void convert_simple_pltxt_ast_to_plunity_richtext(::pltxt2htm::Ast<ndebug> const& ast,
                                                            ::fast_io::u8string& out) noexcept {
    out.reserve(out.size() + ast.size() * 6);
    for (auto&& node : ast) {
        switch (node.get_node_kind()) {
        case ::pltxt2htm::NodeKind::u8char: {
            out.push_back(node.as_u8char().chr);
            continue;
        }
        case ::pltxt2htm::NodeKind::invalid_u8char: {
            out.append(u8"\uFFFD");
            continue;
        }
        case ::pltxt2htm::NodeKind::space: {
            out.append(u8"\u00A0");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::ampersand: {
            out.push_back(u8'&');
            continue;
        }
        case ::pltxt2htm::NodeKind::entity_reference: {
            out.push_back(u8'&');
            out.append(node.as_entity_reference().get_value());
            out.push_back(u8';');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::single_quote: {
            out.push_back(u8'\'');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::double_quote: {
            out.push_back(u8'\"');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::less_than: {
            out.push_back(u8'<');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::greater_than: {
            out.push_back(u8'>');
            continue;
        }
        case ::pltxt2htm::NodeKind::tab: {
            out.push_back(u8'\t');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_backslash: {
            out.push_back(u8'\\');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_exclamation: {
            out.push_back(u8'!');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_hash: {
            out.push_back(u8'#');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_dollar: {
            out.push_back(u8'$');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_percent: {
            out.push_back(u8'%');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_paren: {
            out.push_back(u8'(');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_paren: {
            out.push_back(u8')');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_asterisk: {
            out.push_back(u8'*');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_plus: {
            out.push_back(u8'+');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_comma: {
            out.push_back(u8',');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_hyphen: {
            out.push_back(u8'-');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_dot: {
            out.push_back(u8'.');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_slash: {
            out.push_back(u8'/');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_colon: {
            out.push_back(u8':');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_semicolon: {
            out.push_back(u8';');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_equals: {
            out.push_back(u8'=');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_question: {
            out.push_back(u8'?');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_at: {
            out.push_back(u8'@');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_bracket: {
            out.push_back(u8'[');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_bracket: {
            out.push_back(u8']');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_caret: {
            out.push_back(u8'^');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_underscore: {
            out.push_back(u8'_');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_backtick: {
            out.push_back(u8'`');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_left_brace: {
            out.push_back(u8'{');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_pipe: {
            out.push_back(u8'|');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_right_brace: {
            out.push_back(u8'}');
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_tilde: {
            out.push_back(u8'~');
            continue;
        }
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected node kind in Unity text escaping");
            }
        }
    }
}

/**
 * @brief Convert AST nodes to Unity Rich Text format.
 * @details Generates Unity Rich Text with support for Physics-Lab tags,
 *          Markdown syntax, and HTML-like elements mapped to Unity-compatible tags.
 *          Uses a goto-based state machine to avoid stack overflow.
 * @tparam ndebug Contract checking mode.
 * @param ast_init The AST to convert.
 * @param project Project identifier for Physics-Lab context.
 * @param visitor Visitor identifier for Physics-Lab context.
 * @param author Author identifier for Physics-Lab context.
 * @param coauthors Co-authors identifier for Physics-Lab context.
 * @return A string containing Unity Rich Text output.
 */
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
            case ::pltxt2htm::NodeKind::text: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_text().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::space: {
                result.append(u8"\u00A0");
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape_ampersand:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::ampersand: {
                result.push_back(u8'&');
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
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_a().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_a, 0));
                ++current_index;
                constexpr auto open_tag = ::pltxt2htm::details::concat(
                    ::pltxt2htm::details::U8LiteralString{u8"<color="}, ::pltxt2htm::PlA<ndebug>::get_color_literal(),
                    ::pltxt2htm::details::U8LiteralString{u8">"});
                result.append(::fast_io::u8string_view{open_tag.data(), open_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_experiment: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_pl_experiment().get_subast(), ::pltxt2htm::NodeKind::pl_experiment, 0));
                ++current_index;
                result.append(u8"<experiment=");
                result.append(node.as_pl_experiment().get_id());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussion: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_pl_discussion().get_subast(), ::pltxt2htm::NodeKind::pl_discussion, 0));
                ++current_index;
                result.append(u8"<discussion=");
                result.append(node.as_pl_discussion().get_id());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_external: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_pl_external().get_subast(), ::pltxt2htm::NodeKind::pl_external, 0));
                ++current_index;
                result.append(u8"<external=");
                result.append(node.as_pl_external().get_url().as_string());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_link: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_link().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_link, 0));
                ++current_index;
                result.append(u8"<link=\"");
                result.append(node.as_pl_link().get_url().as_string());
                result.append(u8"\">");
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
                auto const& pl_size = node.as_pl_size().get_font_size();
                result.append(::pltxt2htm::details::size_t2str(pl_size.value));
                switch (pl_size.unit) /* -Werror=switch */ {
                case ::pltxt2htm::Unit::percent: {
                    result.push_back(u8'%');
                    break;
                }
                case ::pltxt2htm::Unit::em: {
                    result.append(u8"em");
                    break;
                }
                case ::pltxt2htm::Unit::px: {
                    break;
                }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                default:
                    [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected unit for size");
                    }
#endif
                }
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_voffset: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_pl_voffset().get_subast(), ::pltxt2htm::NodeKind::pl_voffset, 0));
                ++current_index;
                auto const voffset = node.as_pl_voffset().get_value();
                result.append(u8"<voffset=");
                result.append(::pltxt2htm::details::ptrdiff_t2str(voffset.value));
                switch (voffset.unit) /* -Werror=switch */ {
                case ::pltxt2htm::Unit::em: {
                    result.append(u8"em");
                    break;
                }
                case ::pltxt2htm::Unit::px: {
                    result.append(u8"px");
                    break;
                }
                case ::pltxt2htm::Unit::percent:
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                    [[fallthrough]];
                default:
#endif
                    [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected unit for voffset");
                    }
                }
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_align: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_align().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_align, 0));
                ++current_index;
                if (result.empty() == false && result.back() != u8'\n') {
                    result.push_back(u8'\n');
                }
                auto const align = node.as_pl_align().get_align();
                switch (align) /* -Werror=switch */ {
                case ::pltxt2htm::TextAlign::left: {
                    result.append(u8"<align=left>");
                    break;
                }
                case ::pltxt2htm::TextAlign::center: {
                    result.append(u8"<align=center>");
                    break;
                }
                case ::pltxt2htm::TextAlign::right: {
                    result.append(u8"<align=right>");
                    break;
                }
                case ::pltxt2htm::TextAlign::justify: {
                    result.append(u8"<align=justified>");
                    break;
                }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                default:
                    [[unlikely]] {
                        pltxtpltxt2htm_unreachable(u8"Unexpected unit for align");
                    }
#endif
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_span: {
                auto const& span_color = node.as_html_span().get_color();
                auto const& span_font_size = node.as_html_span().get_font_size();
                auto const& span_vertical_align = node.as_html_span().get_vertical_align();
                bool const has_color = !span_color.empty();
                bool const has_font_size = span_font_size.has_value();
                // Only px lengths map to <voffset>; keywords/percent have no TMP_Text equivalent.
                bool const has_vertical_align = [&] {
                    if (span_vertical_align.has_value() == false) {
                        return false;
                    }
                    auto const& val = span_vertical_align.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                    return val.get_kind() == ::pltxt2htm::VerticalAlignKind::length &&
                           val.get_length().unit == ::pltxt2htm::Unit::px;
                }();
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_span().get_subast(), 0,
                    ::pltxt2htm::details::BackendContextWithHtmlSpanInfo{.has_color = has_color,
                                                                         .has_font_size = has_font_size,
                                                                         .has_vertical_align = has_vertical_align}));
                ++current_index;
                if (has_color) {
                    result.append(u8"<color=");
                    result.append(span_color);
                    result.push_back(u8'>');
                }
                if (has_font_size) {
                    auto const& font_size = span_font_size.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                    result.append(u8"<size=");
                    result.append(::pltxt2htm::details::size_t2str(font_size.value));
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
                        break;
                    }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                    default:
                        [[unlikely]] {
                            pltxt2htm_unreachable(u8"Unexpected unit for font size");
                        }
#endif
                    }
                    result.push_back(u8'>');
                }
                if (has_vertical_align) {
                    result.append(u8"<voffset=");
                    result.append(::pltxt2htm::details::ptrdiff_t2str(
                        span_vertical_align.template value<ndebug == ::pltxt2htm::Contracts::ignore>()
                            .get_length()
                            .value));
                    result.push_back(u8'>');
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_a: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_a().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_a, 0));
                ++current_index;
                result.append(u8"<external=");
                result.append(node.as_html_a().get_url().as_string());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_double_emphasis_underscore().get_subast(),
                    ::pltxt2htm::NodeKind::md_double_emphasis_underscore, 0));
                ++current_index;
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_double_emphasis_asterisk().get_subast(),
                    ::pltxt2htm::NodeKind::md_double_emphasis_asterisk, 0));
                ++current_index;
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_b: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_b().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_b, 0));
                ++current_index;
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_strong: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_strong().get_subast(), ::pltxt2htm::NodeKind::html_strong, 0));
                ++current_index;
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_mark: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_mark().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_mark, 0));
                ++current_index;
                auto const& mark_background_color = node.as_html_mark().get_background_color();
                result.append(u8"<mark=");
                if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                    ::fast_io::u8string purified_color{};
                    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                        purified_color,
                        ::fast_io::u8string_view{mark_background_color.data(), mark_background_color.size()});
                    pltxt2htm_assert(purified_color == mark_background_color,
                                     u8"Color value contains characters that cannot be directly used in Unity "
                                     u8"rich-text tags.");
                }
                result.append(mark_background_color);
                result.append(u8">");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_mark: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_mark().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_mark, 0));
                ++current_index;
                auto const& mark_background_color = node.as_pl_mark().get_background_color();
                result.append(u8"<mark=");
                if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                    ::fast_io::u8string purified_color{};
                    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                        purified_color,
                        ::fast_io::u8string_view{mark_background_color.data(), mark_background_color.size()});
                    pltxt2htm_assert(purified_color == mark_background_color,
                                     u8"Color value contains characters that cannot be directly used in Unity "
                                     u8"rich-text tags.");
                }
                result.append(mark_background_color);
                result.append(u8">");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_p: {
                auto const align = node.as_html_p().get_align();
                bool const has_align = align != ::pltxt2htm::TextAlign::left;
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_p().get_subast(), ::pltxt2htm::NodeKind::html_p, 0,
                    ::pltxt2htm::details::BackendContextWithAlignInfo{.has_align = has_align}));
                ++current_index;
                if (result.empty() == false && result.back() != u8'\n') {
                    result.push_back(u8'\n');
                }
                if (has_align) {
                    result.append(u8"<align=");
                    if (align == ::pltxt2htm::TextAlign::center) {
                        result.append(u8"center");
                    }
                    else if (align == ::pltxt2htm::TextAlign::right) {
                        result.append(u8"right");
                    }
                    else if (align == ::pltxt2htm::TextAlign::justify) {
                        result.append(u8"justified");
                    }
                    else [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected paragraph text alignment");
                    }
                    result.push_back(u8'>');
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_br:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::line_break: {
                result.push_back(u8'\n');
                continue;
            }
            case ::pltxt2htm::NodeKind::html_h1: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h1().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_h1, 0));
                ++current_index;
                if (result.empty() == false && result.back() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=38><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h1: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h1().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_atx_h1, 0));
                ++current_index;
                if (result.empty() == false && result.back() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=38><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h2: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h2().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_h2, 0));
                ++current_index;
                if (result.empty() == false && result.back() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=37><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h2: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h2().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_atx_h2, 0));
                ++current_index;
                if (result.empty() == false && result.back() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=37><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h3: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h3().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_h3, 0));
                ++current_index;
                if (result.empty() == false && result.back() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=36><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h3: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h3().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_atx_h3, 0));
                ++current_index;
                if (result.empty() == false && result.back() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=36><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h4: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h4().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_h4, 0));
                ++current_index;
                if (result.empty() == false && result.back() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=35><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h4: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h4().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_atx_h4, 0));
                ++current_index;
                if (result.empty() == false && result.back() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<size=35><b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h5: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h5().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_h5, 0));
                ++current_index;
                if (result.empty() == false && result.back() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h5: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h5().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_atx_h5, 0));
                ++current_index;
                if (result.empty() == false && result.back() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h6: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h6().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_h6, 0));
                ++current_index;
                if (result.empty() == false && result.back() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h6: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h6().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_atx_h6, 0));
                ++current_index;
                if (result.empty() == false && result.back() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"<b>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_del: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_del().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_del, 0));
                ++current_index;
                result.append(u8"<s>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_del: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_del().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_del, 0));
                ++current_index;
                result.append(u8"<s>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_s: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_s().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_s, 0));
                ++current_index;
                result.append(u8"<s>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sup: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_sup().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_sup, 0));
                ++current_index;
                result.append(u8"<sup>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sub: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_sub().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_sub, 0));
                ++current_index;
                result.append(u8"<sub>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_u: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_u().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_u, 0));
                ++current_index;
                result.append(u8"<u>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_single_emphasis_underscore().get_subast(),
                    ::pltxt2htm::NodeKind::md_single_emphasis_underscore, 0));
                ++current_index;
                result.append(u8"<i>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_single_emphasis_asterisk().get_subast(),
                    ::pltxt2htm::NodeKind::md_single_emphasis_asterisk, 0));
                ++current_index;
                result.append(u8"<i>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_i: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_i().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_i, 0));
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
            case ::pltxt2htm::NodeKind::md_hr:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_hr: {
                if (result.empty() == false && result.back() != u8'\n') {
                    result.push_back(u8'\n');
                }
                result.append(u8"---\n");
                continue;
            }
            case ::pltxt2htm::NodeKind::html_note: {
                continue;
            }
            case ::pltxt2htm::NodeKind::md_ul: {
                auto const parent_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
                if (parent_tag_type == ::pltxt2htm::NodeKind::html_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li_checkbox) {
                    result.push_back(u8'\n');
                }
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_ul().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_ul, 0));
                ++current_index;
                ++list_nesting_depth;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_ul: {
                auto const parent_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
                if (parent_tag_type == ::pltxt2htm::NodeKind::html_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li_checkbox) {
                    result.push_back(u8'\n');
                }
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_ul().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_ul, 0));
                ++current_index;
                ++list_nesting_depth;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_ol: {
                auto const parent_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
                if (parent_tag_type == ::pltxt2htm::NodeKind::html_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li_checkbox) {
                    result.push_back(u8'\n');
                }
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_ol().get_subast(), ::pltxt2htm::NodeKind::md_ol, 0,
                    ::pltxt2htm::details::BackendContextWithOlInfo{}));
                ++current_index;
                ++list_nesting_depth;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_ol: {
                auto const parent_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
                if (parent_tag_type == ::pltxt2htm::NodeKind::html_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li ||
                    parent_tag_type == ::pltxt2htm::NodeKind::md_li_checkbox) {
                    result.push_back(u8'\n');
                }
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_ol().get_subast(), ::pltxt2htm::NodeKind::html_ol, 0,
                    ::pltxt2htm::details::BackendContextWithOlInfo{}));
                ++current_index;
                ++list_nesting_depth;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_li: {
                auto const nested_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
                pltxt2htm_assert(nested_tag_type == ::pltxt2htm::NodeKind::html_ol ||
                                     nested_tag_type == ::pltxt2htm::NodeKind::html_ul,
                                 u8"Invalid tag type");
                [[fallthrough]];
            }
            case ::pltxt2htm::NodeKind::md_li: {
                if (node.get_node_kind() == ::pltxt2htm::NodeKind::html_li) {
                    call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                        node.as_html_li().get_subast(), ::pltxt2htm::NodeKind::html_li, 0));
                }
                else {
                    call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_li().get_subast(),
                                                                                      ::pltxt2htm::NodeKind::md_li, 0));
                }
                ++current_index;
                auto const indent_level = list_nesting_depth;
                for (::std::size_t i = 1; i < indent_level; ++i) {
                    result.append(u8"  ");
                }
                auto reverse_iter = call_stack.container.rbegin();
                ::pltxt2htm::details::BackendFrameContext<ndebug>& the_second_to_last_frame{*(++reverse_iter)};
                auto const nested_tag_type = the_second_to_last_frame.get_nested_tag_type();
                if (nested_tag_type == ::pltxt2htm::NodeKind::html_ol ||
                    nested_tag_type == ::pltxt2htm::NodeKind::md_ol) {
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
                    else [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected indent level remainder");
                    }
                }
                else [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected nested tag type for list item");
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_li_checkbox: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_li_checkbox().get_subast(), ::pltxt2htm::NodeKind::md_li_checkbox, 0));
                ++current_index;
                auto const indent_level = list_nesting_depth;
                for (::std::size_t i = 1; i < indent_level; ++i) {
                    result.append(u8"  ");
                }
                auto reverse_iter = call_stack.container.rbegin();
                ::pltxt2htm::details::BackendFrameContext<ndebug>& the_second_to_last_frame{*(++reverse_iter)};
                auto const nested_tag_type = the_second_to_last_frame.get_nested_tag_type();
                if (nested_tag_type == ::pltxt2htm::NodeKind::html_ol ||
                    nested_tag_type == ::pltxt2htm::NodeKind::md_ol) {
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
                    else [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected indent level remainder");
                    }
                    if (node.as_md_li_checkbox().is_checked()) {
                        result.append(u8"\u2611 ");
                    }
                    else {
                        result.append(u8"\u2610 ");
                    }
                }
                else [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected nested tag type for checkbox list item");
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_code_span_1_backtick().get_subast(), ::pltxt2htm::NodeKind::md_code_span_1_backtick, 0));
                ++current_index;
                result.append(u8"<font=\"PhysicsLab-NerdFont SDF\"> ");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_code_span_2_backtick().get_subast(), ::pltxt2htm::NodeKind::md_code_span_2_backtick, 0));
                ++current_index;
                result.append(u8"<font=\"PhysicsLab-NerdFont SDF\"> ");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_code_span_3_backtick().get_subast(), ::pltxt2htm::NodeKind::md_code_span_3_backtick, 0));
                ++current_index;
                result.append(u8"<font=\"PhysicsLab-NerdFont SDF\"> ");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_code: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_code().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_code, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>code");
                auto const& language = node.as_html_code().get_language();
                if (language.has_value()) {
                    result.append(u8" class=\"");
                    result.append(language.template value<ndebug == ::pltxt2htm::Contracts::ignore>());
                    result.push_back(u8'\"');
                }
                result.append(u8"<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_inline: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_latex_inline().get_subast(), ::pltxt2htm::NodeKind::md_latex_inline, 0));
                ++current_index;
                result.push_back(u8'$');
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
                auto const& pre_subast = node.as_html_pre().get_subast();
                bool const is_code_block = pre_subast.size() == 1 &&
                                           ::pltxt2htm::details::vector_index<ndebug>(pre_subast, 0).get_node_kind() ==
                                               ::pltxt2htm::NodeKind::html_code;
                if (is_code_block) {
                    auto const& code_subast =
                        ::pltxt2htm::details::vector_index<ndebug>(pre_subast, 0).as_html_code().get_subast();
                    call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                        code_subast, ::pltxt2htm::NodeKind::html_pre, 0,
                        ::pltxt2htm::details::BackendContextWithHtmlPreInfo{.is_code_block = true}));
                    ++current_index;
                    result.append(u8"<font=\"PhysicsLab-NerdFont SDF\">\n");
                    goto entry;
                }
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    pre_subast, ::pltxt2htm::NodeKind::html_pre, 0,
                    ::pltxt2htm::details::BackendContextWithHtmlPreInfo{.is_code_block = false}));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>pre<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_block_quotes: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_block_quotes().get_subast(), ::pltxt2htm::NodeKind::md_block_quotes, 0));
                ++current_index;
                result.append(u8"<margin-left=2em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_blockquote: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_blockquote().get_subast(), ::pltxt2htm::NodeKind::html_blockquote, 0));
                ++current_index;
                result.append(u8"<margin-left=2em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_table: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_table().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_table, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>table<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_table: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_table().get_subast(), ::pltxt2htm::NodeKind::html_table, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>table<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_tr: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_tr().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_tr, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>tr<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_tr: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_tr().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_tr, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>tr<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_td: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_td().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_td, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>td");
                auto const align = node.as_md_td().get_align();
                if (align == ::pltxt2htm::TableAlign::center) {
                    result.append(u8" style=\"text-align:center\"");
                }
                else if (align == ::pltxt2htm::TableAlign::right) {
                    result.append(u8" style=\"text-align:right\"");
                }
                result.append(u8"<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_td: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_td().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_td, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>td");
                auto const align = node.as_html_td().get_align();
                if (align == ::pltxt2htm::TableAlign::center) {
                    result.append(u8" style=\"text-align:center\"");
                }
                else if (align == ::pltxt2htm::TableAlign::right) {
                    result.append(u8" style=\"text-align:right\"");
                }
                result.append(u8"<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_th: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_th().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_th, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>th");
                auto const align = node.as_md_th().get_align();
                if (align == ::pltxt2htm::TableAlign::center) {
                    result.append(u8" style=\"text-align:center\"");
                }
                else if (align == ::pltxt2htm::TableAlign::right) {
                    result.append(u8" style=\"text-align:right\"");
                }
                result.append(u8"<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_th: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_th().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_th, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>th");
                auto const align = node.as_html_th().get_align();
                if (align == ::pltxt2htm::TableAlign::center) {
                    result.append(u8" style=\"text-align:center\"");
                }
                else if (align == ::pltxt2htm::TableAlign::right) {
                    result.append(u8" style=\"text-align:right\"");
                }
                result.append(u8"<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_thead: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_thead().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_thead, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>thead<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_thead: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_thead().get_subast(), ::pltxt2htm::NodeKind::html_thead, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>thead<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_tbody: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_tbody().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_tbody, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>tbody<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_tbody: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_tbody().get_subast(), ::pltxt2htm::NodeKind::html_tbody, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>tbody<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_tfoot: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_tfoot().get_subast(), ::pltxt2htm::NodeKind::html_tfoot, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>tfoot<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_caption: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_caption().get_subast(), ::pltxt2htm::NodeKind::html_caption, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>caption<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_colgroup: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_colgroup().get_subast(), ::pltxt2htm::NodeKind::html_colgroup, 0));
                ++current_index;
                result.append(u8"<size=20>\uff1c</size>colgroup<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_col: {
                result.append(u8"<size=20>\uff1c</size>col<size=20>\uff1e</size>");
                continue;
            }
            case ::pltxt2htm::NodeKind::html_input: {
                if (node.as_html_input().is_checked()) {
                    result.append(
                        u8"<size=20>\uff1c</size>input type=\"checkbox\" disabled checked<size=20>\uff1e</size>");
                }
                else {
                    result.append(u8"<size=20>\uff1c</size>input type=\"checkbox\" disabled<size=20>\uff1e</size>");
                }
                continue;
            }
            case ::pltxt2htm::NodeKind::html_img: {
                result.append(u8"<size=20>\uff1c</size>img src=\"");
                result.append(node.as_html_img().get_src());
                result.append(u8"\" alt=\"");
                result.append(node.as_html_img().get_alt());
                result.append(u8"\"<size=20>\uff1e</size>");
                continue;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_triple_emphasis_underscore().get_subast(),
                    ::pltxt2htm::NodeKind::md_triple_emphasis_underscore, 0));
                ++current_index;
                result.append(u8"<b><i>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_triple_emphasis_asterisk().get_subast(),
                    ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk, 0));
                ++current_index;
                result.append(u8"<b><i>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::url: {
                auto const& url_str = node.as_url().as_string();
                result.append(u8"<external=");
                result.append(url_str);
                result.push_back(u8'>');
                result.append(url_str);
                result.append(u8"</external>");
                continue;
            }
            case ::pltxt2htm::NodeKind::md_link: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_link().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_link, 0));
                ++current_index;
                result.append(u8"<external=");
                result.append(node.as_md_link().get_url().as_string());
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_image: {
                result.append(u8"![");
                ::pltxt2htm::details::convert_simple_pltxt_ast_to_plunity_richtext<ndebug>(
                    node.as_md_image().get_subast(), result);
                result.append(u8"](");
                result.append(node.as_md_image().get_url().as_string());
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
                result.append(u8"<font=\"PhysicsLab-NerdFont SDF\">\n");
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_code_fence_backtick().get_subast(), ::pltxt2htm::NodeKind::md_code_fence_backtick, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_fence_tilde: {
                result.append(u8"<font=\"PhysicsLab-NerdFont SDF\">\n");
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_code_fence_tilde().get_subast(), ::pltxt2htm::NodeKind::md_code_fence_tilde, 0));
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
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected macro node kind in Unity backend");
                }
#endif
            }
        }

        {
            auto const top_frame = ::pltxt2htm::details::BackendFrameContext<ndebug>{
                ::std::move(::pltxt2htm::details::stack_top<ndebug>(call_stack))};
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
            case ::pltxt2htm::NodeKind::html_span: {
                auto const& span_info = top_frame.get_html_span_info();
                if (span_info.has_vertical_align) {
                    result.append(u8"</voffset>");
                }
                if (span_info.has_font_size) {
                    result.append(u8"</size>");
                }
                if (span_info.has_color) {
                    result.append(u8"</color>");
                }
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
            case ::pltxt2htm::NodeKind::pl_link: {
                result.append(u8"</link>");
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
            case ::pltxt2htm::NodeKind::pl_voffset: {
                result.append(u8"</voffset>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_align: {
                result.append(u8"</align>");
                auto const& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                auto const& parent_ast = parent_frame.get_ast();
                if (parent_frame.current_index < parent_ast.size()) {
                    auto const next_kind =
                        ::pltxt2htm::details::vector_index<ndebug>(parent_ast, parent_frame.current_index)
                            .get_node_kind();
                    if (next_kind != ::pltxt2htm::NodeKind::line_break && next_kind != ::pltxt2htm::NodeKind::html_br &&
                        result.empty() == false && result.back() != u8'\n') {
                        result.push_back(u8'\n');
                    }
                }
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
            case ::pltxt2htm::NodeKind::html_mark: {
                result.append(u8"</mark>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_mark: {
                result.append(u8"</mark>");
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
                if (top_frame.get_align_info().has_align) {
                    result.append(u8"</align>");
                }
                auto const& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                auto const& parent_ast = parent_frame.get_ast();
                if (parent_frame.current_index < parent_ast.size()) {
                    auto const next_kind =
                        ::pltxt2htm::details::vector_index<ndebug>(parent_ast, parent_frame.current_index)
                            .get_node_kind();
                    if (next_kind != ::pltxt2htm::NodeKind::line_break && next_kind != ::pltxt2htm::NodeKind::html_br &&
                        result.empty() == false && result.back() != u8'\n') {
                        result.push_back(u8'\n');
                    }
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h1:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h1:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_atx_h2:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h2:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_atx_h3:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h3:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_atx_h4:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h4: {
                result.append(u8"</b></size>");
                auto const& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                auto const& parent_ast = parent_frame.get_ast();
                if (parent_frame.current_index < parent_ast.size()) {
                    auto const next_kind =
                        ::pltxt2htm::details::vector_index<ndebug>(parent_ast, parent_frame.current_index)
                            .get_node_kind();
                    if (next_kind != ::pltxt2htm::NodeKind::line_break && next_kind != ::pltxt2htm::NodeKind::html_br &&
                        result.empty() == false && result.back() != u8'\n') {
                        result.push_back(u8'\n');
                    }
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h5:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h5: {
                result.append(u8"</b>");
                auto const& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                auto const& parent_ast = parent_frame.get_ast();
                if (parent_frame.current_index < parent_ast.size()) {
                    auto const next_kind =
                        ::pltxt2htm::details::vector_index<ndebug>(parent_ast, parent_frame.current_index)
                            .get_node_kind();
                    if (next_kind != ::pltxt2htm::NodeKind::line_break && next_kind != ::pltxt2htm::NodeKind::html_br &&
                        result.empty() == false && result.back() != u8'\n') {
                        result.push_back(u8'\n');
                    }
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h6:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h6: {
                result.append(u8"</b>");
                auto const& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
                auto const& parent_ast = parent_frame.get_ast();
                if (parent_frame.current_index < parent_ast.size()) {
                    auto const next_kind =
                        ::pltxt2htm::details::vector_index<ndebug>(parent_ast, parent_frame.current_index)
                            .get_node_kind();
                    if (next_kind != ::pltxt2htm::NodeKind::line_break && next_kind != ::pltxt2htm::NodeKind::html_br &&
                        result.empty() == false && result.back() != u8'\n') {
                        result.push_back(u8'\n');
                    }
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_del:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_del:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_s: {
                result.append(u8"</s>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sup: {
                result.append(u8"</sup>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sub: {
                result.append(u8"</sub>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_u: {
                result.append(u8"</u>");
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
                result.append(u8" </font>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_code: {
                result.append(u8"<size=20>\uff1c</size>/code<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_inline: {
                result.push_back(u8'$');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_block: {
                result.append(u8"$$");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_pre: {
                if (top_frame.get_html_pre_info().is_code_block) {
                    result.append(u8"\n</font>");
                }
                else {
                    result.append(u8"<size=20>\uff1c</size>/pre<size=20>\uff1e</size>");
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_block_quotes: {
                result.append(u8"</margin>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_table:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_table: {
                result.append(u8"<size=20>\uff1c</size>/table<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_tr:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_tr: {
                result.append(u8"<size=20>\uff1c</size>/tr<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_td:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_td: {
                result.append(u8"<size=20>\uff1c</size>/td<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_th:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_th: {
                result.append(u8"<size=20>\uff1c</size>/th<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_thead:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_thead: {
                result.append(u8"<size=20>\uff1c</size>/thead<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_tbody:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_tbody: {
                result.append(u8"<size=20>\uff1c</size>/tbody<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_tfoot: {
                result.append(u8"<size=20>\uff1c</size>/tfoot<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_caption: {
                result.append(u8"<size=20>\uff1c</size>/caption<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_colgroup: {
                result.append(u8"<size=20>\uff1c</size>/colgroup<size=20>\uff1e</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_blockquote: {
                result.append(u8"</margin>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_fence_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_code_fence_tilde: {
                result.append(u8"\n</font>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_link:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::url:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_a: {
                result.append(u8"</external>");
                goto entry;
            }
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected nested tag type in Unity text backend");
                }
            }
        }
    }
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
