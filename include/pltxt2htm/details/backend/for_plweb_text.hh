/**
 * @file for_plweb_text.hh
 * @brief Advanced HTML backend for pltxt2htm
 * @details Generates full-featured HTML output with comprehensive support for
 *          Physics-Lab tags, Markdown syntax, and HTML elements.
 */

#pragma once

#include <fast_io/fast_io_dsal/list.h>
#include "../call_stack.hh"
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include "../../container/string_view.hh"
#include "../../ast/value_unit.hh"
#include "../../ast/vertical_align_value.hh"
#include "frame_context.hh"
#include "html_escape.hh"
#include "../../contracts.hh"
#include "../utils.hh"
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
    ::std::size_t const ast_size{ast.size()};
    for (::std::size_t index{}; index < ast_size; ++index) {
        auto const& node = ::pltxt2htm::details::vector_index<ndebug>(ast, index);
        switch (node.get_node_kind()) {
        case ::pltxt2htm::NodeKind::u8char: {
            auto&& active_node = node.as_u8char();
            out.push_back(active_node.chr);
            continue;
        }
        case ::pltxt2htm::NodeKind::invalid_utf8: {
            out.append(u8"\uFFFD");
            continue;
        }
        case ::pltxt2htm::NodeKind::space: {
            out.append(u8"&nbsp;");
            continue;
        }
        case ::pltxt2htm::NodeKind::ampersand: {
            out.append(u8"&amp;");
            continue;
        }
        case ::pltxt2htm::NodeKind::single_quote: {
            out.append(u8"&apos;");
            continue;
        }
        case ::pltxt2htm::NodeKind::double_quote: {
            out.append(u8"&quot;");
            continue;
        }
        case ::pltxt2htm::NodeKind::less_than: {
            out.append(u8"&lt;");
            continue;
        }
        case ::pltxt2htm::NodeKind::greater_than: {
            out.append(u8"&gt;");
            continue;
        }
        case ::pltxt2htm::NodeKind::tab: {
            out.append(u8"&nbsp;&nbsp;&nbsp;&nbsp;");
            continue;
        }
        case ::pltxt2htm::NodeKind::md_escape: {
            auto&& active_node = node.as_md_escape();
            ::pltxt2htm::details::append_html_escaped_character(out, active_node.get_character());
            continue;
        }
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected node kind in HTML escaping");
            }
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
constexpr auto plweb_text_backend(::pltxt2htm::Ast<ndebug> const& ast_init, ::pltxt2htm::container::U8StringView host,
                                  ::pltxt2htm::container::U8StringView project,
                                  ::pltxt2htm::container::U8StringView visitor,
                                  ::pltxt2htm::container::U8StringView author,
                                  ::pltxt2htm::container::U8StringView coauthors) noexcept -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    ::pltxt2htm::details::CallStack<BackendFrame<ndebug>> call_stack{};
    call_stack.push_frame(BackendFrame<ndebug>(ast_init, ::pltxt2htm::NodeKind::group, 0));

entry:
    while (true) {
        auto&& current_frame = call_stack.template current_frame<ndebug>();
        auto const& ast = current_frame.get_ast();
        auto const nested_tag_type = current_frame.get_nested_tag_type();
        auto&& current_index = current_frame.current_index;
        ::std::size_t const ast_size{ast.size()};
        // TOOD: replace this to a while loop
        for (; current_index < ast_size; ++current_index) {
            auto&& node = ::pltxt2htm::details::vector_index<ndebug>(ast, current_index);

            switch (node.get_node_kind()) /* -Werror=switch */ {
            case ::pltxt2htm::NodeKind::u8char: {
                auto&& active_node = node.as_u8char();
                result.push_back(active_node.chr);
                continue;
            }
            case ::pltxt2htm::NodeKind::invalid_utf8: {
                result.append(u8"\uFFFD");
                continue;
            }
            case ::pltxt2htm::NodeKind::group: {
                auto&& active_node = node.as_group();
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::group, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::space: {
                result.append(u8"&nbsp;");
                continue;
            }
            case ::pltxt2htm::NodeKind::ampersand: {
                result.append(u8"&amp;");
                continue;
            }
            case ::pltxt2htm::NodeKind::single_quote: {
                result.append(u8"&apos;");
                continue;
            }
            case ::pltxt2htm::NodeKind::double_quote: {
                result.append(u8"&quot;");
                continue;
            }
            case ::pltxt2htm::NodeKind::less_than: {
                result.append(u8"&lt;");
                continue;
            }
            case ::pltxt2htm::NodeKind::greater_than: {
                result.append(u8"&gt;");
                continue;
            }
            case ::pltxt2htm::NodeKind::tab: {
                result.append(u8"&nbsp;&nbsp;&nbsp;&nbsp;");
                continue;
            }
            case ::pltxt2htm::NodeKind::pl_color: {
                auto&& active_node = node.as_pl_color();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_color, 0));
                ++current_index;
                result.append(u8"<span style=\"color:");
                auto const& color_value = active_node.get_color();
                // Under normal circumstances, `color_value` should never contain characters that could enable XSS in
                // HTML attributes. To avoid masking upstream bugs (and to keep release-path performance), we only
                // assert this in debug mode. Do not try to hide such errors by routing output through
                // `append_html_escaped_attribute_value`.
                if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                    ::fast_io::u8string purified_color_value{};
                    ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                        purified_color_value, ::pltxt2htm::container::U8StringView{color_value});
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
                auto&& active_node = node.as_pl_a();
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_a, 0));
                ++current_index;
                constexpr auto open_tag = ::pltxt2htm::details::concat(U8LiteralString{u8"<span style=\"color:"},
                                                                       ::pltxt2htm::PlA<ndebug>::get_color_literal(),
                                                                       U8LiteralString{u8";\">"});
                result.append(::pltxt2htm::container::U8StringView{open_tag});
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_experiment: {
                auto&& active_node = node.as_pl_experiment();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_experiment, 0));
                ++current_index;
                result.append(u8"<a href=\"");
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(result, host);
                if constexpr (mode != PlWebTextBackendMode::pltxt4unittest) {
                    result.append(u8"/p/Experiment/");
                }
                else {
                    result.append(u8"/ExperimentSummary/Experiment/");
                }
                auto const& experiment_id = active_node.get_id();
                // Under normal circumstances, `experiment_id` should never contain characters that could enable XSS in
                // HTML attributes. To avoid masking upstream bugs (and to keep release-path performance), we only
                // assert this in debug mode. Do not try to hide such errors by routing output through
                // `append_html_escaped_attribute_value`.
                if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                    ::fast_io::u8string purified_experiment_id{};
                    ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                        purified_experiment_id, ::pltxt2htm::container::U8StringView{experiment_id});
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
                auto&& active_node = node.as_pl_discussion();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_discussion, 0));
                ++current_index;
                result.append(u8"<a href=\"");
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(result, host);
                if constexpr (mode != PlWebTextBackendMode::pltxt4unittest) {
                    result.append(u8"/p/Discussion/");
                }
                else {
                    result.append(u8"/ExperimentSummary/Discussion/");
                }
                auto const& discussion_id = active_node.get_id();
                // Under normal circumstances, `discussion_id` should never contain characters that could enable XSS in
                // HTML attributes. To avoid masking upstream bugs (and to keep release-path performance), we only
                // assert this in debug mode. Do not try to hide such errors by routing output through
                // `append_html_escaped_attribute_value`.
                if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                    ::fast_io::u8string purified_discussion_id{};
                    ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                        purified_discussion_id, ::pltxt2htm::container::U8StringView{discussion_id});
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
                auto&& active_node = node.as_pl_experiments();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_experiments, 0));
                ++current_index;
                result.append(u8"&lt;experiments=");
                auto const& experiments_value = active_node.get_value();
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                    result, ::pltxt2htm::container::U8StringView{experiments_value});
                result.append(u8"&gt;");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussions: {
                auto&& active_node = node.as_pl_discussions();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_discussions, 0));
                ++current_index;
                result.append(u8"&lt;discussions=");
                auto const& discussions_value = active_node.get_value();
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                    result, ::pltxt2htm::container::U8StringView{discussions_value});
                result.append(u8"&gt;");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_trigger: {
                auto&& active_node = node.as_pl_trigger();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_trigger, 0));
                ++current_index;
                result.append(u8"&lt;trigger=");
                auto const& trigger_value = active_node.get_value();
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                    result, ::pltxt2htm::container::U8StringView{trigger_value});
                result.append(u8"&gt;");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_internal: {
                auto&& active_node = node.as_pl_internal();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_internal, 0));
                ++current_index;
                result.append(u8"&lt;internal=");
                auto const& internal_value = active_node.get_value();
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                    result, ::pltxt2htm::container::U8StringView{internal_value});
                result.append(u8"&gt;");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_user: {
                auto&& active_node = node.as_pl_user();
                ++current_index;
                if constexpr (mode == PlWebTextBackendMode::roundtrip) {
                    call_stack.push_frame(
                        BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::group, 0));
                    goto entry;
                }
                else {
                    call_stack.push_frame(
                        BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_user, 0));
                    result.append(u8"<span class='RUser' data-user='");
                    auto const& user_id = active_node.get_id();
                    // Under normal circumstances, `user_id` should never contain characters that could enable XSS in
                    // HTML attributes. To avoid masking upstream bugs (and to keep release-path performance), we only
                    // assert this in debug mode. Do not try to hide such errors by routing output through
                    // `append_html_escaped_attribute_value`.
                    if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                        ::fast_io::u8string purified_user_id{};
                        ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                            purified_user_id, ::pltxt2htm::container::U8StringView{user_id});
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
                auto&& active_node = node.as_pl_size();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_size, 0));
                ++current_index;
                result.append(u8"<span style=\"font-size:");
                auto const& pl_size = active_node.get_font_size();
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
                auto&& active_node = node.as_pl_voffset();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_voffset, 0));
                ++current_index;
                auto const voffset = active_node.get_value();
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
                auto&& active_node = node.as_pl_align();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_align, 0));
                ++current_index;
                auto const align = active_node.get_align();
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
                auto&& active_node = node.as_pl_margin();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_margin, 0));
                ++current_index;
                auto const margin_left = active_node.get_left();
                auto const margin_right = active_node.get_right();
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
                auto&& active_node = node.as_html_div();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_div, 0));
                ++current_index;
                auto const margin_left = active_node.get_left();
                auto const margin_right = active_node.get_right();
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
                auto&& active_node = node.as_html_span();
                auto const& span_color = active_node.get_color();
                auto const& span_font_size = active_node.get_font_size();
                auto const& span_vertical_align = active_node.get_vertical_align();
                bool const has_color = !span_color.empty();
                bool const has_font_size = span_font_size.has_value();
                bool const has_vertical_align = span_vertical_align.has_value();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), 0,
                                         BackendContextWithHtmlSpanInfo{.has_color = has_color,
                                                                        .has_font_size = has_font_size,
                                                                        .has_vertical_align = has_vertical_align}));
                ++current_index;
                result.append(u8"<span style=\"");
                if (has_color) {
                    result.append(u8"color:");
                    if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                        ::fast_io::u8string purified_color{};
                        ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
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
                auto&& active_node = node.as_html_a();
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_a, 0));
                ++current_index;
                result.append(u8"<a href=\"");
                auto const& html_a_url = active_node.get_url().as_string();
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                    result, ::pltxt2htm::container::U8StringView{html_a_url});
                if (active_node.get_internal()) {
                    result.append(u8"\" internal>");
                }
                else {
                    result.append(u8"\">");
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_double_emphasis_underscore: {
                auto&& active_node = node.as_md_double_emphasis_underscore();
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(),
                                                           ::pltxt2htm::NodeKind::md_double_emphasis_underscore, 0));
                ++current_index;
                result.append(u8"<strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_double_emphasis_asterisk: {
                auto&& active_node = node.as_md_double_emphasis_asterisk();
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(),
                                                           ::pltxt2htm::NodeKind::md_double_emphasis_asterisk, 0));
                ++current_index;
                result.append(u8"<strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_b: {
                auto&& active_node = node.as_pl_b();
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_b, 0));
                ++current_index;
                result.append(u8"<strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_strong: {
                auto&& active_node = node.as_html_strong();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_strong, 0));
                ++current_index;
                result.append(u8"<strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_mark: {
                auto&& active_node = node.as_html_mark();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_mark, 0));
                ++current_index;
                auto const& mark_background_color = active_node.get_background_color();
                result.append(u8"<mark style=\"background-color:");
                if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                    ::fast_io::u8string purified_color{};
                    ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                        purified_color, ::pltxt2htm::container::U8StringView{mark_background_color});
                    pltxt2htm_assert(purified_color == mark_background_color,
                                     u8"Color value contains characters that cannot be directly used in HTML "
                                     u8"attributes.");
                }
                result.append(mark_background_color);
                result.append(u8";\">");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_mark: {
                auto&& active_node = node.as_pl_mark();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_mark, 0));
                ++current_index;
                auto const& mark_background_color = active_node.get_background_color();
                result.append(u8"<mark style=\"background-color:");
                if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                    ::fast_io::u8string purified_color{};
                    ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                        purified_color, ::pltxt2htm::container::U8StringView{mark_background_color});
                    pltxt2htm_assert(purified_color == mark_background_color,
                                     u8"Color value contains characters that cannot be directly used in HTML "
                                     u8"attributes.");
                }
                result.append(mark_background_color);
                result.append(u8";\">");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_p: {
                auto&& active_node = node.as_html_p();
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_p, 0));
                ++current_index;
                auto const align = active_node.get_align();
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
                auto&& active_node = node.as_html_h1();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_h1, 0));
                ++current_index;
                result.append(u8"<h1>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h1: {
                auto&& active_node = node.as_md_atx_h1();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_atx_h1, 0));
                ++current_index;
                result.append(u8"<h1>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h2: {
                auto&& active_node = node.as_html_h2();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_h2, 0));
                ++current_index;
                result.append(u8"<h2>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h2: {
                auto&& active_node = node.as_md_atx_h2();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_atx_h2, 0));
                ++current_index;
                result.append(u8"<h2>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h3: {
                auto&& active_node = node.as_html_h3();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_h3, 0));
                ++current_index;
                result.append(u8"<h3>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h3: {
                auto&& active_node = node.as_md_atx_h3();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_atx_h3, 0));
                ++current_index;
                result.append(u8"<h3>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h4: {
                auto&& active_node = node.as_html_h4();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_h4, 0));
                ++current_index;
                result.append(u8"<h4>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h4: {
                auto&& active_node = node.as_md_atx_h4();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_atx_h4, 0));
                ++current_index;
                result.append(u8"<h4>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h5: {
                auto&& active_node = node.as_html_h5();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_h5, 0));
                ++current_index;
                result.append(u8"<h5>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h5: {
                auto&& active_node = node.as_md_atx_h5();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_atx_h5, 0));
                ++current_index;
                result.append(u8"<h5>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h6: {
                auto&& active_node = node.as_html_h6();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_h6, 0));
                ++current_index;
                result.append(u8"<h6>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h6: {
                auto&& active_node = node.as_md_atx_h6();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_atx_h6, 0));
                ++current_index;
                result.append(u8"<h6>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_del: {
                auto&& active_node = node.as_md_del();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_del, 0));
                ++current_index;
                result.append(u8"<del>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_del: {
                auto&& active_node = node.as_html_del();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_del, 0));
                ++current_index;
                result.append(u8"<del>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_code: {
                auto&& active_node = node.as_html_code();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_code, 0));
                ++current_index;
                result.append(u8"<code>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_u: {
                auto&& active_node = node.as_html_u();
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_u, 0));
                ++current_index;
                result.append(u8"<u>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_s: {
                auto&& active_node = node.as_html_s();
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_s, 0));
                ++current_index;
                result.append(u8"<s>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sup: {
                auto&& active_node = node.as_html_sup();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_sup, 0));
                ++current_index;
                result.append(u8"<sup>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_sub: {
                auto&& active_node = node.as_html_sub();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_sub, 0));
                ++current_index;
                result.append(u8"<sub>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_single_emphasis_underscore: {
                auto&& active_node = node.as_md_single_emphasis_underscore();
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(),
                                                           ::pltxt2htm::NodeKind::md_single_emphasis_underscore, 0));
                ++current_index;
                result.append(u8"<em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_single_emphasis_asterisk: {
                auto&& active_node = node.as_md_single_emphasis_asterisk();
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(),
                                                           ::pltxt2htm::NodeKind::md_single_emphasis_asterisk, 0));
                ++current_index;
                result.append(u8"<em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_i: {
                auto&& active_node = node.as_pl_i();
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_i, 0));
                ++current_index;
                result.append(u8"<em>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_em: {
                auto&& active_node = node.as_html_em();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_em, 0));
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
                auto&& active_node = node.as_list_ul();
                auto const& list_ul = active_node;
                pltxt2htm_assert(list_ul.get_subast().empty() == false, u8"List container must not be empty");
                call_stack.push_frame(BackendFrame<ndebug>(list_ul.get_subast(), ::pltxt2htm::NodeKind::list_ul, 0));
                ++current_index;
                result.append(u8"<ul>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_ol: {
                auto&& active_node = node.as_list_ol();
                auto const& list_ol = active_node;
                pltxt2htm_assert(list_ol.get_subast().empty() == false, u8"List container must not be empty");
                call_stack.push_frame(BackendFrame<ndebug>(list_ol.get_subast(), ::pltxt2htm::NodeKind::list_ol, 0));
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
                auto&& active_node = node.as_list_li_checkbox();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::list_li_checkbox, 0));
                ++current_index;
                result.append(u8"<li>");
                if (active_node.is_checked()) {
                    result.append(u8"<input type=\"checkbox\" disabled checked>");
                }
                else {
                    result.append(u8"<input type=\"checkbox\" disabled>");
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::list_li: {
                auto&& active_node = node.as_list_li();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::list_li, 0));
                ++current_index;
                result.append(u8"<li>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
                auto&& active_node = node.as_md_code_span_1_backtick();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_code_span_1_backtick, 0));
                ++current_index;
                result.append(u8"<code>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
                auto&& active_node = node.as_md_code_span_2_backtick();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_code_span_2_backtick, 0));
                ++current_index;
                result.append(u8"<code>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
                auto&& active_node = node.as_md_code_span_3_backtick();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_code_span_3_backtick, 0));
                ++current_index;
                result.append(u8"<code>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_latex_inline: {
                auto&& active_node = node.as_md_latex_inline();
                if constexpr (mode == PlWebTextBackendMode::roundtrip) {
                    continue;
                }
                else {
                    ++current_index;
                    call_stack.push_frame(
                        BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_latex_inline, 0));
                    result.push_back(u8'$');
                    goto entry;
                }
            }
            case ::pltxt2htm::NodeKind::md_latex_block: {
                auto&& active_node = node.as_md_latex_block();
                if constexpr (mode == PlWebTextBackendMode::roundtrip) {
                    continue;
                }
                else {
                    ++current_index;
                    call_stack.push_frame(
                        BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_latex_block, 0));
                    result.append(u8"$$");
                    goto entry;
                }
            }
            case ::pltxt2htm::NodeKind::md_block_quotes: {
                auto&& active_node = node.as_md_block_quotes();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_block_quotes, 0));
                ++current_index;
                result.append(u8"<blockquote>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_blockquote: {
                auto&& active_node = node.as_html_blockquote();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::html_blockquote, 0));
                ++current_index;
                result.append(u8"<blockquote>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table: {
                auto&& active_node = node.as_table();
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table, 0));
                ++current_index;
                result.append(u8"<table>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tr: {
                auto&& active_node = node.as_table_tr();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_tr, 0));
                ++current_index;
                result.append(u8"<tr>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_td: {
                auto&& active_node = node.as_table_td();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_td, 0));
                ++current_index;
                result.append(u8"<td");
                auto const align = active_node.get_align();
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
                auto&& active_node = node.as_table_th();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_th, 0));
                ++current_index;
                result.append(u8"<th");
                auto const align = active_node.get_align();
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
                auto&& active_node = node.as_table_thead();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_thead, 0));
                ++current_index;
                result.append(u8"<thead>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tbody: {
                auto&& active_node = node.as_table_tbody();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_tbody, 0));
                ++current_index;
                result.append(u8"<tbody>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_tfoot: {
                auto&& active_node = node.as_table_tfoot();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_tfoot, 0));
                ++current_index;
                result.append(u8"<tfoot>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_caption: {
                auto&& active_node = node.as_table_caption();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_caption, 0));
                ++current_index;
                result.append(u8"<caption>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_colgroup: {
                auto&& active_node = node.as_table_colgroup();
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::table_colgroup, 0));
                ++current_index;
                result.append(u8"<colgroup>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::table_col: {
                result.append(u8"<col>");
                continue;
            }
            case ::pltxt2htm::NodeKind::html_img: {
                auto&& active_node = node.as_html_img();
                result.append(u8"<img src=\"");
                auto const& src = active_node.get_src();
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                    result, ::pltxt2htm::container::U8StringView{src});
                result.append(u8"\" alt=\"");
                auto const& alt = active_node.get_alt();
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                    result, ::pltxt2htm::container::U8StringView{alt});
                result.append(u8"\">");
                continue;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_underscore: {
                auto&& active_node = node.as_md_triple_emphasis_underscore();
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(),
                                                           ::pltxt2htm::NodeKind::md_triple_emphasis_underscore, 0));
                ++current_index;
                result.append(u8"<em><strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk: {
                auto&& active_node = node.as_md_triple_emphasis_asterisk();
                call_stack.push_frame(BackendFrame<ndebug>(active_node.get_subast(),
                                                           ::pltxt2htm::NodeKind::md_triple_emphasis_asterisk, 0));
                ++current_index;
                result.append(u8"<em><strong>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::url: {
                auto&& active_node = node.as_url();
                auto const& url_str = active_node.as_string();
                ::fast_io::u8string escaped;
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                    escaped, ::pltxt2htm::container::U8StringView{url_str});
                result.append(u8"<a href=\"");
                result.append(escaped);
                result.append(u8"\">");
                result.append(escaped);
                result.append(u8"</a>");
                continue;
            }
            case ::pltxt2htm::NodeKind::md_link: {
                auto&& active_node = node.as_md_link();
                result.append(u8"<a href=\"");
                auto const& md_link_url = active_node.get_url().as_string();
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                    result, ::pltxt2htm::container::U8StringView{md_link_url});
                result.append(u8"\">");
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::md_link, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_external: {
                auto&& active_node = node.as_pl_external();
                result.append(u8"<a href=\"");
                auto const& ext_url = active_node.get_url().as_string();
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                    result, ::pltxt2htm::container::U8StringView{ext_url});
                result.append(u8"\">");
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_external, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_link: {
                auto&& active_node = node.as_pl_link();
                result.append(u8"<a href=\"");
                auto const& link_url = active_node.get_url().as_string();
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                    result, ::pltxt2htm::container::U8StringView{link_url});
                result.append(u8"\">");
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::pl_link, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_image: {
                auto&& active_node = node.as_md_image();
                result.append(u8"<img src=\"");
                auto const& img_url = active_node.get_url().as_string();
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                    result, ::pltxt2htm::container::U8StringView{img_url});
                result.append(u8"\" alt=\"");
                ::pltxt2htm::details::convert_simple_pltxt_ast_to_plweb_text<ndebug>(active_node.get_subast(), result);
                result.append(u8"\">");
                continue;
            }
            case ::pltxt2htm::NodeKind::md_escape: {
                auto&& active_node = node.as_md_escape();
                ::pltxt2htm::details::append_html_escaped_character(result, active_node.get_character());
                continue;
            }
            case ::pltxt2htm::NodeKind::code_fence: {
                auto&& active_node = node.as_code_fence();
                auto const& opt_language = active_node.get_language();
                if (opt_language.has_value()) {
                    auto const& language = opt_language.template value<ndebug>();
                    result.append(u8"<pre><code class=\"language-");
                    ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(
                        result, ::pltxt2htm::container::U8StringView{language});
                    result.append(u8"\">");
                }
                else {
                    result.append(u8"<pre><code>");
                }
                call_stack.push_frame(
                    BackendFrame<ndebug>(active_node.get_subast(), ::pltxt2htm::NodeKind::code_fence, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_macro_project: {
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(result, project);
                continue;
            }
            case ::pltxt2htm::NodeKind::pl_macro_visitor: {
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(result, visitor);
                continue;
            }
            case ::pltxt2htm::NodeKind::pl_macro_author: {
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(result, author);
                continue;
            }
            case ::pltxt2htm::NodeKind::pl_macro_coauthors: {
                ::pltxt2htm::details::append_html_escaped_attribute_value<ndebug>(result, coauthors);
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
            bool const has_parent{call_stack.has_parent()};
            auto const top_frame = call_stack.template pop_frame<ndebug>();
            if (!has_parent) {
                return result;
            }
            switch (top_frame.get_nested_tag_type()) {
            case ::pltxt2htm::NodeKind::group: {
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
            case ::pltxt2htm::NodeKind::html_u: {
                result.append(u8"</u>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_s: {
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
                auto const parent_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
                if (parent_tag_type == ::pltxt2htm::NodeKind::list_ul ||
                    parent_tag_type == ::pltxt2htm::NodeKind::list_ol) {
                    // A sibling-nested list defers the parent item's </li>. Consecutive
                    // sibling lists ([li, ul, ul]) share one deferred </li>, so keep it open
                    // while another list follows as the next sibling and emit it only after
                    // the last list of the run.
                    auto const& parent_ast = call_stack.template current_frame<ndebug>().get_ast();
                    auto const parent_index = call_stack.template current_frame<ndebug>().current_index;
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
                auto const parent_tag_type = call_stack.template current_frame<ndebug>().get_nested_tag_type();
                if (parent_tag_type == ::pltxt2htm::NodeKind::list_ul ||
                    parent_tag_type == ::pltxt2htm::NodeKind::list_ol) {
                    // See the list_ul case above: keep the deferred </li> open across
                    // consecutive sibling lists and close it after the last one.
                    auto const& parent_ast = call_stack.template current_frame<ndebug>().get_ast();
                    auto const parent_index = call_stack.template current_frame<ndebug>().current_index;
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
                auto const& parent_ast = call_stack.template current_frame<ndebug>().get_ast();
                auto const parent_index = call_stack.template current_frame<ndebug>().current_index;
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
