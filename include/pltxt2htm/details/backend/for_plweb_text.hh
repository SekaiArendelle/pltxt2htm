/**
 * @file for_plweb_text.hh
 * @brief Advanced HTML backend for pltxt2htm
 * @details Generates full-featured HTML output with comprehensive support for
 *          Physics-Lab tags, Markdown syntax, and HTML elements.
 */

#pragma once

#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../../ast/value_unit.hh"
#include "../../ast/vertical_align_value.hh"
#include "frame_context.hh"
#include "../../contracts.hh"
#include "../utils.hh"
#include "../parser/try_parse.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Convert a simple (leaf-only) AST to HTML text with proper escaping.
 * @tparam ndebug Contract checking mode.
 * @param ast The AST to convert (should only contain leaf/character-like nodes).
 * @param[out] out Output buffer receiving the escaped HTML text.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void convert_simple_pltxt_ast_to_plweb_text(::pltxt2htm::Ast<ndebug> const& ast,
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
            out.append(u8"&nbsp;");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::ampersand: {
            out.append(u8"&amp;");
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
            out.append(u8"&apos;");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::double_quote: {
            out.append(u8"&quot;");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::less_than: {
            out.append(u8"&lt;");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeKind::greater_than: {
            out.append(u8"&gt;");
            continue;
        }
        case ::pltxt2htm::NodeKind::tab: {
            out.append(u8"&nbsp;&nbsp;&nbsp;&nbsp;");
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
                pltxt2htm_unreachable(u8"Unexpected node kind in HTML escaping");
            }
        }
    }
}

/**
 * @brief Append a string to an HTML attribute value with proper escaping.
 * @tparam ndebug Contract checking mode.
 * @param result Output string to append to.
 * @param value Raw attribute value to escape.
 */
template<::pltxt2htm::Contracts ndebug>
constexpr void append_html_attr_escaped(::fast_io::u8string& result, ::fast_io::u8string_view value) noexcept {
    ::std::size_t const value_size{value.size()};
    for (::std::size_t index{}; index < value_size; ++index) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(value, index);
        switch (chr) {
        case u8'&':
            if (auto const opt_entity_len = ::pltxt2htm::details::try_parse_entity_reference<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(value, index));
                opt_entity_len.has_value()) {
                auto const entity_len = opt_entity_len.template value<ndebug>();
                result.append(::fast_io::u8string_view{value.data() + index, entity_len});
                index += entity_len - 1;
                break;
            }
            result.append(u8"&amp;");
            break;
        case u8'\"':
            result.append(u8"&quot;");
            break;
        case u8'\'':
            result.append(u8"&apos;");
            break;
        case u8'<':
            result.append(u8"&lt;");
            break;
        case u8'>':
            result.append(u8"&gt;");
            break;
        default:
            result.push_back(chr);
            break;
        }
    }
}

enum class PlWebTextBackendMode : unsigned {
    pltxt4unittest = 0,
    fixedadv_html,
    roundtrip,
};

/**
 * @brief Convert AST nodes to advanced HTML with full feature support
 * @details This backend generates comprehensive HTML output supporting:
 *          - Physics-Lab specific tags (color, experiment, discussion, user, size)
 *          - Markdown syntax (headers, lists, emphasis, links, etc.)
 *          - HTML elements (paragraphs, line breaks, etc.)
 *          - Proper HTML escaping and encoding
 * @tparam ndebug Contract checking mode - `::pltxt2htm::Contracts::ignore` is faster with fewer checks,
 *                while `::pltxt2htm::Contracts::quick_enforce` enables more safety checks at higher cost
 * @param[in] ast_init The AST to convert to HTML
 * @param[in] host Host URL for generating internal links (used for experiment/discussion links)
 * @param[in] project Project identifier for Physics-Lab context
 * @param[in] visitor Visitor identifier for Physics-Lab context
 * @param[in] author Author identifier for Physics-Lab context
 * @param[in] coauthors Co-authors identifier for Physics-Lab context
 * @return A string containing the generated HTML
 * @note To avoid stack overflow, this function manages call_stack manually using goto-based state machine
 */
template<::pltxt2htm::Contracts ndebug, PlWebTextBackendMode mode>
[[nodiscard]]
constexpr auto plweb_text_backend(::pltxt2htm::Ast<ndebug> const& ast_init, ::fast_io::u8string_view host,
                                  ::fast_io::u8string_view project, ::fast_io::u8string_view visitor,
                                  ::fast_io::u8string_view author, ::fast_io::u8string_view coauthors) noexcept
    -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    ::fast_io::stack<BackendFrameContext<ndebug>> call_stack{};
    call_stack.push(BackendFrameContext<ndebug>(ast_init, ::pltxt2htm::NodeKind::text, 0));

entry:
    while (true) {
        auto&& current_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
        auto const& ast = current_frame.get_ast();
        auto const nested_tag_type = current_frame.get_nested_tag_type();
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
            case ::pltxt2htm::NodeKind::text: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_text().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                ++current_index;
                goto entry;
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
            case ::pltxt2htm::NodeKind::pl_color: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_color().get_subast(), ::pltxt2htm::NodeKind::pl_color, 0));
                ++current_index;
                result.append(u8"<span style=\"color:");
                auto const& color_value = node.as_pl_color().get_color();
                // Under normal circumstances, `color_value` should never contain characters that could enable XSS in
                // HTML attributes. To avoid masking upstream bugs (and to keep release-path performance), we only
                // assert this in debug mode. Do not try to hide such errors by routing output through
                // `append_html_attr_escaped`.
                if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                    ::fast_io::u8string purified_color_value{};
                    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                        purified_color_value, ::fast_io::u8string_view{color_value.data(), color_value.size()});
                    bool const is_valid_color_value{purified_color_value == color_value};
                    pltxt2htm_assert(
                        is_valid_color_value,
                        u8"Color value contains characters that cannot be directly used in HTML attributes. Please "
                        u8"check the color value or use a different backend that supports escaping.");
                }
                result.append(color_value);
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
                result.append(::fast_io::u8string_view{open_tag.data(), open_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_experiment: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_pl_experiment().get_subast(),
                                                            ::pltxt2htm::NodeKind::pl_experiment, 0));
                ++current_index;
                result.append(u8"<a href=\"");
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result, host);
                if constexpr (mode != PlWebTextBackendMode::pltxt4unittest) {
                    result.append(u8"/p/Experiment/");
                }
                else {
                    result.append(u8"/ExperimentSummary/Experiment/");
                }
                auto const& experiment_id = node.as_pl_experiment().get_id();
                // Under normal circumstances, `experiment_id` should never contain characters that could enable XSS in
                // HTML attributes. To avoid masking upstream bugs (and to keep release-path performance), we only
                // assert this in debug mode. Do not try to hide such errors by routing output through
                // `append_html_attr_escaped`.
                if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                    ::fast_io::u8string purified_experiment_id{};
                    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                        purified_experiment_id, ::fast_io::u8string_view{experiment_id.data(), experiment_id.size()});
                    pltxt2htm_assert(purified_experiment_id == experiment_id,
                                     u8"Experiment ID contains characters that cannot be directly used in HTML "
                                     u8"attributes. Please check "
                                     u8"the experiment ID or use a different backend that supports escaping.");
                }
                result.append(experiment_id);
                result.append(u8"\" internal>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussion: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_pl_discussion().get_subast(),
                                                            ::pltxt2htm::NodeKind::pl_discussion, 0));
                ++current_index;
                result.append(u8"<a href=\"");
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result, host);
                if constexpr (mode != PlWebTextBackendMode::pltxt4unittest) {
                    result.append(u8"/p/Discussion/");
                }
                else {
                    result.append(u8"/ExperimentSummary/Discussion/");
                }
                auto const& discussion_id = node.as_pl_discussion().get_id();
                // Under normal circumstances, `discussion_id` should never contain characters that could enable XSS in
                // HTML attributes. To avoid masking upstream bugs (and to keep release-path performance), we only
                // assert this in debug mode. Do not try to hide such errors by routing output through
                // `append_html_attr_escaped`.
                if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                    ::fast_io::u8string purified_discussion_id{};
                    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                        purified_discussion_id, ::fast_io::u8string_view{discussion_id.data(), discussion_id.size()});
                    pltxt2htm_assert(purified_discussion_id == discussion_id,
                                     u8"Discussion ID contains characters that cannot be directly used in HTML "
                                     u8"attributes. Please check "
                                     u8"the discussion ID or use a different backend that supports escaping.");
                }
                result.append(discussion_id);
                result.append(u8"\" internal>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_experiments: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_pl_experiments().get_subast(),
                                                            ::pltxt2htm::NodeKind::pl_experiments, 0));
                ++current_index;
                result.append(u8"&lt;experiments=");
                auto const& experiments_value = node.as_pl_experiments().get_value();
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    result, ::fast_io::u8string_view{experiments_value.data(), experiments_value.size()});
                result.append(u8"&gt;");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussions: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_pl_discussions().get_subast(),
                                                            ::pltxt2htm::NodeKind::pl_discussions, 0));
                ++current_index;
                result.append(u8"&lt;discussions=");
                auto const& discussions_value = node.as_pl_discussions().get_value();
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    result, ::fast_io::u8string_view{discussions_value.data(), discussions_value.size()});
                result.append(u8"&gt;");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_trigger: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_pl_trigger().get_subast(),
                                                            ::pltxt2htm::NodeKind::pl_trigger, 0));
                ++current_index;
                result.append(u8"&lt;trigger=");
                auto const& trigger_value = node.as_pl_trigger().get_value();
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    result, ::fast_io::u8string_view{trigger_value.data(), trigger_value.size()});
                result.append(u8"&gt;");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_internal: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_pl_internal().get_subast(),
                                                            ::pltxt2htm::NodeKind::pl_internal, 0));
                ++current_index;
                result.append(u8"&lt;internal=");
                auto const& internal_value = node.as_pl_internal().get_value();
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    result, ::fast_io::u8string_view{internal_value.data(), internal_value.size()});
                result.append(u8"&gt;");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_user: {
                ++current_index;
                if constexpr (mode == PlWebTextBackendMode::roundtrip) {
                    call_stack.push(
                        BackendFrameContext<ndebug>(node.as_pl_user().get_subast(), ::pltxt2htm::NodeKind::text, 0));
                    goto entry;
                }
                else {
                    call_stack.push(
                        BackendFrameContext<ndebug>(node.as_pl_user().get_subast(), ::pltxt2htm::NodeKind::pl_user, 0));
                    result.append(u8"<span class='RUser' data-user='");
                    auto const& user_id = node.as_pl_user().get_id();
                    // Under normal circumstances, `user_id` should never contain characters that could enable XSS in
                    // HTML attributes. To avoid masking upstream bugs (and to keep release-path performance), we only
                    // assert this in debug mode. Do not try to hide such errors by routing output through
                    // `append_html_attr_escaped`.
                    if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                        ::fast_io::u8string purified_user_id{};
                        ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                            purified_user_id, ::fast_io::u8string_view{user_id.data(), user_id.size()});
                        bool const is_valid_user_id{purified_user_id == user_id};
                        pltxt2htm_assert(
                            is_valid_user_id,
                            u8"User ID contains characters that cannot be directly used in HTML attributes. Please "
                            u8"check the "
                            u8"user ID or use a different backend that supports escaping.");
                    }
                    result.append(user_id);
                    result.append(u8"'>");
                    goto entry;
                }
            }
            case ::pltxt2htm::NodeKind::pl_size: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_size().get_subast(), ::pltxt2htm::NodeKind::pl_size, 0));
                ++current_index;
                result.append(u8"<span style=\"font-size:");
                auto const& pl_size = node.as_pl_size().get_font_size();
                switch (pl_size.unit) /* -Werror=switch */ {
                case ::pltxt2htm::Unit::percent: {
                    result.append(::pltxt2htm::details::double2str(pl_size.value));
                    result.append(u8"%;\">");
                    break;
                }
                case ::pltxt2htm::Unit::em: {
                    result.append(::pltxt2htm::details::double2str(pl_size.value));
                    result.append(u8"em;\">");
                    break;
                }
                case ::pltxt2htm::Unit::px: {
                    // Round ceil(pl_size / 2) up to preserve the historical integer mapping
                    // (e.g. size=11 -> 6px) while accepting fractional sizes such as 12.5 -> 7px.
                    result.append(::pltxt2htm::details::size_t2str(
                        ::pltxt2htm::details::double_to_size_t_ceil(pl_size.value / 2)));
                    result.append(u8"px;\">");
                    break;
                }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                default:
                    [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected unit for size");
                    }
#endif
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_voffset: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_pl_voffset().get_subast(),
                                                            ::pltxt2htm::NodeKind::pl_voffset, 0));
                ++current_index;
                auto const voffset = node.as_pl_voffset().get_value();
                result.append(u8"<span style=\"vertical-align:");
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
                result.append(u8";\">");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_align: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_align().get_subast(), ::pltxt2htm::NodeKind::pl_align, 0));
                ++current_index;
                auto const align = node.as_pl_align().get_align();
                switch (align) /* -Werror=switch */ {
                case ::pltxt2htm::TextAlign::left: {
                    result.append(u8"<p style=\"text-align:left\">");
                    break;
                }
                case ::pltxt2htm::TextAlign::center: {
                    result.append(u8"<p style=\"text-align:center\">");
                    break;
                }
                case ::pltxt2htm::TextAlign::right: {
                    result.append(u8"<p style=\"text-align:right\">");
                    break;
                }
                case ::pltxt2htm::TextAlign::justify: {
                    result.append(u8"<p style=\"text-align:justify\">");
                    break;
                }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                default:
                    [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected text alignment");
                    }
#endif
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_margin: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_margin().get_subast(), ::pltxt2htm::NodeKind::pl_margin, 0));
                ++current_index;
                auto const margin_left = node.as_pl_margin().get_left();
                auto const margin_right = node.as_pl_margin().get_right();
                result.append(u8"<div style=\"");
                if (margin_left.has_value()) {
                    result.append(u8"margin-left:");
                    auto const& margin_value = margin_left.template value<ndebug>();
                    result.append(::pltxt2htm::details::size_t2str(margin_value.value));
                    switch (margin_value.unit) /* -Werror=switch */ {
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
                            pltxt2htm_unreachable(u8"Unexpected unit for margin");
                        }
#endif
                    }
                    result.push_back(u8';');
                }
                if (margin_right.has_value()) {
                    result.append(u8"margin-right:");
                    auto const& margin_value = margin_right.template value<ndebug>();
                    result.append(::pltxt2htm::details::size_t2str(margin_value.value));
                    switch (margin_value.unit) /* -Werror=switch */ {
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
                            pltxt2htm_unreachable(u8"Unexpected unit for margin");
                        }
#endif
                    }
                    result.push_back(u8';');
                }
                result.append(u8"\">");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_div: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_div().get_subast(), ::pltxt2htm::NodeKind::html_div, 0));
                ++current_index;
                auto const margin_left = node.as_html_div().get_left();
                auto const margin_right = node.as_html_div().get_right();
                result.append(u8"<div style=\"");
                if (margin_left.has_value()) {
                    result.append(u8"margin-left:");
                    auto const& margin_value = margin_left.template value<ndebug>();
                    result.append(::pltxt2htm::details::size_t2str(margin_value.value));
                    switch (margin_value.unit) /* -Werror=switch */ {
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
                            pltxt2htm_unreachable(u8"Unexpected unit for div");
                        }
#endif
                    }
                    result.push_back(u8';');
                }
                if (margin_right.has_value()) {
                    result.append(u8"margin-right:");
                    auto const& margin_value = margin_right.template value<ndebug>();
                    result.append(::pltxt2htm::details::size_t2str(margin_value.value));
                    switch (margin_value.unit) /* -Werror=switch */ {
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
                            pltxt2htm_unreachable(u8"Unexpected unit for div");
                        }
#endif
                    }
                    result.push_back(u8';');
                }
                result.append(u8"\">");
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
                auto const& html_a_url = node.as_html_a().get_url().as_string();
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    result, ::fast_io::u8string_view{html_a_url.data(), html_a_url.size()});
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
            case ::pltxt2htm::NodeKind::html_mark: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_mark().get_subast(), ::pltxt2htm::NodeKind::html_mark, 0));
                ++current_index;
                auto const& mark_background_color = node.as_html_mark().get_background_color();
                result.append(u8"<mark style=\"background-color:");
                if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                    ::fast_io::u8string purified_color{};
                    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                        purified_color,
                        ::fast_io::u8string_view{mark_background_color.data(), mark_background_color.size()});
                    pltxt2htm_assert(purified_color == mark_background_color,
                                     u8"Color value contains characters that cannot be directly used in HTML "
                                     u8"attributes.");
                }
                result.append(mark_background_color);
                result.append(u8";\">");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_mark: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_mark().get_subast(), ::pltxt2htm::NodeKind::pl_mark, 0));
                ++current_index;
                auto const& mark_background_color = node.as_pl_mark().get_background_color();
                result.append(u8"<mark style=\"background-color:");
                if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                    ::fast_io::u8string purified_color{};
                    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                        purified_color,
                        ::fast_io::u8string_view{mark_background_color.data(), mark_background_color.size()});
                    pltxt2htm_assert(purified_color == mark_background_color,
                                     u8"Color value contains characters that cannot be directly used in HTML "
                                     u8"attributes.");
                }
                result.append(mark_background_color);
                result.append(u8";\">");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_p: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_p().get_subast(), ::pltxt2htm::NodeKind::html_p, 0));
                ++current_index;
                auto const align = node.as_html_p().get_align();
                switch (align) /* -Werror=switch */ {
                case ::pltxt2htm::TextAlign::left: {
                    result.append(u8"<p style=\"text-align:left\">");
                    break;
                }
                case ::pltxt2htm::TextAlign::center: {
                    result.append(u8"<p style=\"text-align:center\">");
                    break;
                }
                case ::pltxt2htm::TextAlign::right: {
                    result.append(u8"<p style=\"text-align:right\">");
                    break;
                }
                case ::pltxt2htm::TextAlign::justify: {
                    result.append(u8"<p style=\"text-align:justify\">");
                    break;
                }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
                default:
                    [[unlikely]] {
                        pltxt2htm_unreachable(u8"Unexpected text alignment");
                    }
#endif
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::line_break: {
                if (nested_tag_type == ::pltxt2htm::NodeKind::code_fence) {
                    result.push_back(u8'\n');
                }
                else {
                    result.append(u8"<br>");
                }
                continue;
            }
            case ::pltxt2htm::NodeKind::html_br: {
                result.append(u8"<br>");
                continue;
            }
            case ::pltxt2htm::NodeKind::html_h1: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_h1().get_subast(), ::pltxt2htm::NodeKind::html_h1, 0));
                ++current_index;
                result.append(u8"<h1>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h1: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_md_atx_h1().get_subast(), ::pltxt2htm::NodeKind::md_atx_h1, 0));
                ++current_index;
                result.append(u8"<h1>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h2: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_h2().get_subast(), ::pltxt2htm::NodeKind::html_h2, 0));
                ++current_index;
                result.append(u8"<h2>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h2: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_md_atx_h2().get_subast(), ::pltxt2htm::NodeKind::md_atx_h2, 0));
                ++current_index;
                result.append(u8"<h2>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h3: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_h3().get_subast(), ::pltxt2htm::NodeKind::html_h3, 0));
                ++current_index;
                result.append(u8"<h3>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h3: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_md_atx_h3().get_subast(), ::pltxt2htm::NodeKind::md_atx_h3, 0));
                ++current_index;
                result.append(u8"<h3>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h4: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_h4().get_subast(), ::pltxt2htm::NodeKind::html_h4, 0));
                ++current_index;
                result.append(u8"<h4>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h4: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_md_atx_h4().get_subast(), ::pltxt2htm::NodeKind::md_atx_h4, 0));
                ++current_index;
                result.append(u8"<h4>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h5: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_h5().get_subast(), ::pltxt2htm::NodeKind::html_h5, 0));
                ++current_index;
                result.append(u8"<h5>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h5: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_md_atx_h5().get_subast(), ::pltxt2htm::NodeKind::md_atx_h5, 0));
                ++current_index;
                result.append(u8"<h5>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h6: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_h6().get_subast(), ::pltxt2htm::NodeKind::html_h6, 0));
                ++current_index;
                result.append(u8"<h6>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h6: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_md_atx_h6().get_subast(), ::pltxt2htm::NodeKind::md_atx_h6, 0));
                ++current_index;
                result.append(u8"<h6>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_del: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_md_del().get_subast(), ::pltxt2htm::NodeKind::html_del, 0));
                ++current_index;
                result.append(u8"<del>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_del: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_del().get_subast(), ::pltxt2htm::NodeKind::html_del, 0));
                ++current_index;
                result.append(u8"<del>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_code: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_code().get_subast(), ::pltxt2htm::NodeKind::html_code, 0));
                ++current_index;
                result.append(u8"<code>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_u: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_u().get_subast(), ::pltxt2htm::NodeKind::pl_u, 0));
                ++current_index;
                result.append(u8"<u>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_s: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_s().get_subast(), ::pltxt2htm::NodeKind::pl_s, 0));
                ++current_index;
                result.append(u8"<s>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sup: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_sup().get_subast(), ::pltxt2htm::NodeKind::html_sup, 0));
                ++current_index;
                result.append(u8"<sup>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sub: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_html_sub().get_subast(), ::pltxt2htm::NodeKind::html_sub, 0));
                ++current_index;
                result.append(u8"<sub>");
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
            case ::pltxt2htm::NodeKind::md_hr:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_hr: {
                result.append(u8"<hr>");
                continue;
            }
            case ::pltxt2htm::NodeKind::html_note: {
                continue;
            }
            case ::pltxt2htm::NodeKind::list_ul: {
                auto const& list_ul = node.as_list_ul();
                pltxt2htm_assert(list_ul.get_subast().empty() == false, u8"List container must not be empty");
                call_stack.push(BackendFrameContext<ndebug>(list_ul.get_subast(), ::pltxt2htm::NodeKind::list_ul, 0));
                ++current_index;
                result.append(u8"<ul>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_ol: {
                auto const& list_ol = node.as_list_ol();
                pltxt2htm_assert(list_ol.get_subast().empty() == false, u8"List container must not be empty");
                call_stack.push(BackendFrameContext<ndebug>(list_ol.get_subast(), ::pltxt2htm::NodeKind::list_ol, 0));
                ++current_index;
                result.append(u8"<ol");
                if (list_ol.get_start() != 1) {
                    result.append(u8" start=\"");
                    result.append(::pltxt2htm::details::size_t2str(list_ol.get_start()));
                    result.push_back(u8'\"');
                }
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_li_checkbox: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_list_li_checkbox().get_subast(),
                                                            ::pltxt2htm::NodeKind::list_li_checkbox, 0));
                ++current_index;
                result.append(u8"<li>");
                if (node.as_list_li_checkbox().is_checked()) {
                    result.append(u8"<input type=\"checkbox\" disabled checked>");
                }
                else {
                    result.append(u8"<input type=\"checkbox\" disabled>");
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_li: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_list_li().get_subast(), ::pltxt2htm::NodeKind::list_li, 0));
                ++current_index;
                result.append(u8"<li>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_md_code_span_1_backtick().get_subast(),
                                                            ::pltxt2htm::NodeKind::md_code_span_1_backtick, 0));
                ++current_index;
                result.append(u8"<code>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_md_code_span_2_backtick().get_subast(),
                                                            ::pltxt2htm::NodeKind::md_code_span_2_backtick, 0));
                ++current_index;
                result.append(u8"<code>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_md_code_span_3_backtick().get_subast(),
                                                            ::pltxt2htm::NodeKind::md_code_span_3_backtick, 0));
                ++current_index;
                result.append(u8"<code>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_inline: {
                if constexpr (mode == PlWebTextBackendMode::roundtrip) {
                    continue;
                }
                else {
                    ++current_index;
                    call_stack.push(BackendFrameContext<ndebug>(node.as_md_latex_inline().get_subast(),
                                                                ::pltxt2htm::NodeKind::md_latex_inline, 0));
                    result.push_back(u8'$');
                    goto entry;
                }
            }
            case ::pltxt2htm::NodeKind::md_latex_block: {
                if constexpr (mode == PlWebTextBackendMode::roundtrip) {
                    continue;
                }
                else {
                    ++current_index;
                    call_stack.push(BackendFrameContext<ndebug>(node.as_md_latex_block().get_subast(),
                                                                ::pltxt2htm::NodeKind::md_latex_block, 0));
                    result.append(u8"$$");
                    goto entry;
                }
            }
            case ::pltxt2htm::NodeKind::md_block_quotes: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_md_block_quotes().get_subast(),
                                                            ::pltxt2htm::NodeKind::md_block_quotes, 0));
                ++current_index;
                result.append(u8"<blockquote>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_blockquote: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_html_blockquote().get_subast(),
                                                            ::pltxt2htm::NodeKind::html_blockquote, 0));
                ++current_index;
                result.append(u8"<blockquote>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_table().get_subast(), ::pltxt2htm::NodeKind::table, 0));
                ++current_index;
                result.append(u8"<table>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tr: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_table_tr().get_subast(), ::pltxt2htm::NodeKind::table_tr, 0));
                ++current_index;
                result.append(u8"<tr>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_td: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_table_td().get_subast(), ::pltxt2htm::NodeKind::table_td, 0));
                ++current_index;
                result.append(u8"<td");
                auto const align = node.as_table_td().get_align();
                if (align == ::pltxt2htm::TableAlign::center) {
                    result.append(u8" style=\"text-align:center\"");
                }
                else if (align == ::pltxt2htm::TableAlign::right) {
                    result.append(u8" style=\"text-align:right\"");
                }
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_th: {
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_table_th().get_subast(), ::pltxt2htm::NodeKind::table_th, 0));
                ++current_index;
                result.append(u8"<th");
                auto const align = node.as_table_th().get_align();
                if (align == ::pltxt2htm::TableAlign::center) {
                    result.append(u8" style=\"text-align:center\"");
                }
                else if (align == ::pltxt2htm::TableAlign::right) {
                    result.append(u8" style=\"text-align:right\"");
                }
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_thead: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_table_thead().get_subast(),
                                                            ::pltxt2htm::NodeKind::table_thead, 0));
                ++current_index;
                result.append(u8"<thead>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tbody: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_table_tbody().get_subast(),
                                                            ::pltxt2htm::NodeKind::table_tbody, 0));
                ++current_index;
                result.append(u8"<tbody>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tfoot: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_table_tfoot().get_subast(),
                                                            ::pltxt2htm::NodeKind::table_tfoot, 0));
                ++current_index;
                result.append(u8"<tfoot>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_caption: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_table_caption().get_subast(),
                                                            ::pltxt2htm::NodeKind::table_caption, 0));
                ++current_index;
                result.append(u8"<caption>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_colgroup: {
                call_stack.push(BackendFrameContext<ndebug>(node.as_table_colgroup().get_subast(),
                                                            ::pltxt2htm::NodeKind::table_colgroup, 0));
                ++current_index;
                result.append(u8"<colgroup>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_col: {
                result.append(u8"<col>");
                continue;
            }
            case ::pltxt2htm::NodeKind::html_img: {
                result.append(u8"<img src=\"");
                auto const& src = node.as_html_img().get_src();
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    result, ::fast_io::u8string_view{src.data(), src.size()});
                result.append(u8"\" alt=\"");
                auto const& alt = node.as_html_img().get_alt();
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    result, ::fast_io::u8string_view{alt.data(), alt.size()});
                result.append(u8"\">");
                continue;
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
            case ::pltxt2htm::NodeKind::url: {
                auto const& url_str = node.as_url().as_string();
                ::fast_io::u8string escaped;
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    escaped, ::fast_io::u8string_view{url_str.data(), url_str.size()});
                result.append(u8"<a href=\"");
                result.append(escaped);
                result.append(u8"\">");
                result.append(escaped);
                result.append(u8"</a>");
                continue;
            }
            case ::pltxt2htm::NodeKind::md_link: {
                result.append(u8"<a href=\"");
                auto const& md_link_url = node.as_md_link().get_url().as_string();
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    result, ::fast_io::u8string_view{md_link_url.data(), md_link_url.size()});
                result.append(u8"\">");
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_md_link().get_subast(), ::pltxt2htm::NodeKind::md_link, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_external: {
                result.append(u8"<a href=\"");
                auto const& ext_url = node.as_pl_external().get_url().as_string();
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    result, ::fast_io::u8string_view{ext_url.data(), ext_url.size()});
                result.append(u8"\">");
                call_stack.push(BackendFrameContext<ndebug>(node.as_pl_external().get_subast(),
                                                            ::pltxt2htm::NodeKind::pl_external, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_link: {
                result.append(u8"<a href=\"");
                auto const& link_url = node.as_pl_link().get_url().as_string();
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    result, ::fast_io::u8string_view{link_url.data(), link_url.size()});
                result.append(u8"\">");
                call_stack.push(
                    BackendFrameContext<ndebug>(node.as_pl_link().get_subast(), ::pltxt2htm::NodeKind::pl_link, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_image: {
                result.append(u8"<img src=\"");
                auto const& img_url = node.as_md_image().get_url().as_string();
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    result, ::fast_io::u8string_view{img_url.data(), img_url.size()});
                result.append(u8"\" alt=\"");
                ::pltxt2htm::details::convert_simple_pltxt_ast_to_plweb_text<ndebug>(node.as_md_image().get_subast(),
                                                                                     result);
                result.append(u8"\">");
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
            case ::pltxt2htm::NodeKind::code_fence: {
                auto const& opt_language = node.as_code_fence().get_language();
                if (opt_language.has_value()) {
                    auto const& language = opt_language.template value<ndebug>();
                    result.append(u8"<pre><code class=\"language-");
                    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                        result, ::fast_io::u8string_view{language.data(), language.size()});
                    result.append(u8"\">");
                }
                else {
                    result.append(u8"<pre><code>");
                }
                call_stack.push(BackendFrameContext<ndebug>(node.as_code_fence().get_subast(),
                                                            ::pltxt2htm::NodeKind::code_fence, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_macro_project: {
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result, project);
                continue;
            }
            case ::pltxt2htm::NodeKind::pl_macro_visitor: {
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result, visitor);
                continue;
            }
            case ::pltxt2htm::NodeKind::pl_macro_author: {
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result, author);
                continue;
            }
            case ::pltxt2htm::NodeKind::pl_macro_coauthors: {
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result, coauthors);
                continue;
            }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected macro node kind in text backend");
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
            case ::pltxt2htm::NodeKind::text: {
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
            case ::pltxt2htm::NodeKind::pl_experiment: {
                result.append(u8"</a>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussion: {
                result.append(u8"</a>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_experiments: {
                result.append(u8"&lt;/experiments&gt;");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussions: {
                result.append(u8"&lt;/discussions&gt;");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_user: {
                result.append(u8"</span>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_trigger: {
                result.append(u8"&lt;/trigger&gt;");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_internal: {
                result.append(u8"&lt;/internal&gt;");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_size: {
                result.append(u8"</span>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_voffset: {
                result.append(u8"</span>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_align: {
                result.append(u8"</p>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_margin: {
                result.append(u8"</div>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_div: {
                result.append(u8"</div>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_double_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::pl_b:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_strong: {
                result.append(u8"</strong>");
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
                result.append(u8"</em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
                result.append(u8"</strong></em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_p: {
                result.append(u8"</p>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h1:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h1: {
                result.append(u8"</h1>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h2:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h2: {
                result.append(u8"</h2>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h3:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h3: {
                result.append(u8"</h3>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h4:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h4: {
                result.append(u8"</h4>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h5:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h5: {
                result.append(u8"</h5>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h6:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_h6: {
                result.append(u8"</h6>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_del:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_del: {
                result.append(u8"</del>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_code: {
                result.append(u8"</code>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_u: {
                result.append(u8"</u>");
                goto entry;
            }
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
            case ::pltxt2htm::NodeKind::list_ul: {
                result.append(u8"</ul>");
                auto const parent_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
                if (parent_tag_type == ::pltxt2htm::NodeKind::list_ul ||
                    parent_tag_type == ::pltxt2htm::NodeKind::list_ol) {
                    // A sibling-nested list defers the parent item's </li>. Consecutive
                    // sibling lists ([li, ul, ul]) share one deferred </li>, so keep it open
                    // while another list follows as the next sibling and emit it only after
                    // the last list of the run.
                    auto const& parent_ast = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_ast();
                    auto const parent_index = ::pltxt2htm::details::stack_top<ndebug>(call_stack).current_index;
                    if (parent_index >= parent_ast.size() ||
                        (::pltxt2htm::details::vector_index<ndebug>(parent_ast, parent_index).get_node_kind() !=
                             ::pltxt2htm::NodeKind::list_ul &&
                         ::pltxt2htm::details::vector_index<ndebug>(parent_ast, parent_index).get_node_kind() !=
                             ::pltxt2htm::NodeKind::list_ol)) {
                        result.append(u8"</li>");
                    }
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_ol: {
                result.append(u8"</ol>");
                auto const parent_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
                if (parent_tag_type == ::pltxt2htm::NodeKind::list_ul ||
                    parent_tag_type == ::pltxt2htm::NodeKind::list_ol) {
                    // See the list_ul case above: keep the deferred </li> open across
                    // consecutive sibling lists and close it after the last one.
                    auto const& parent_ast = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_ast();
                    auto const parent_index = ::pltxt2htm::details::stack_top<ndebug>(call_stack).current_index;
                    if (parent_index >= parent_ast.size() ||
                        (::pltxt2htm::details::vector_index<ndebug>(parent_ast, parent_index).get_node_kind() !=
                             ::pltxt2htm::NodeKind::list_ul &&
                         ::pltxt2htm::details::vector_index<ndebug>(parent_ast, parent_index).get_node_kind() !=
                             ::pltxt2htm::NodeKind::list_ol)) {
                        result.append(u8"</li>");
                    }
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_li_checkbox:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::list_li: {
                // If the next sibling in the parent list is itself a list (the Markdown
                // nested-list shape), defer </li> so the nested list renders inside this
                // item; the nested list's closing appends the deferred </li>.
                auto const& parent_ast = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_ast();
                auto const parent_index = ::pltxt2htm::details::stack_top<ndebug>(call_stack).current_index;
                if (parent_index < parent_ast.size()) {
                    auto const next_kind =
                        ::pltxt2htm::details::vector_index<ndebug>(parent_ast, parent_index).get_node_kind();
                    if (next_kind == ::pltxt2htm::NodeKind::list_ul || next_kind == ::pltxt2htm::NodeKind::list_ol) {
                        goto entry;
                    }
                }
                result.append(u8"</li>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_1_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_code_span_2_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
                result.append(u8"</code>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_inline: {
                pltxt2htm_assert(mode != PlWebTextBackendMode::roundtrip,
                                 u8"Unexpected md_latex_inline node in roundtrip mode");
                result.push_back(u8'$');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_block: {
                pltxt2htm_assert(mode != PlWebTextBackendMode::roundtrip,
                                 u8"Unexpected md_latex_block node in roundtrip mode");
                result.append(u8"$$");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_block_quotes:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_blockquote: {
                result.append(u8"</blockquote>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table: {
                result.append(u8"</table>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tr: {
                result.append(u8"</tr>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_td: {
                result.append(u8"</td>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_th: {
                result.append(u8"</th>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_thead: {
                result.append(u8"</thead>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tbody: {
                result.append(u8"</tbody>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tfoot: {
                result.append(u8"</tfoot>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_caption: {
                result.append(u8"</caption>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_colgroup: {
                result.append(u8"</colgroup>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::code_fence: {
                result.append(u8"</code></pre>");
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
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected nested tag type in text backend");
                }
            }
        }
    }
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
