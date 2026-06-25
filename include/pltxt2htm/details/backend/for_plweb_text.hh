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
#include <exception/exception.hh>
#include "frame_context.hh"
#include "../../contracts.hh"
#include "../../details/utils.hh"
#include "../../details/parser/try_parse.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto convert_simple_pltxt_ast_to_plweb_text(::pltxt2htm::Ast<ndebug> const& ast) noexcept
    -> ::fast_io::u8string {
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
                pltxt2htm_unreachable(u8"Unexpected node kind in HTML escaping");
            }
        }
    }

    return result;
}

template<::pltxt2htm::Contracts ndebug>
constexpr void append_html_attr_escaped(::fast_io::u8string& result, ::fast_io::u8string_view value) noexcept {
    for (::std::size_t index{}; index < value.size(); ++index) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(value, index);
        switch (chr) {
        case u8'&':
            if (auto const opt_entity_len = ::pltxt2htm::details::try_parse_entity_reference<ndebug>(
                    ::pltxt2htm::details::u8string_view_subview<ndebug>(value, index));
                opt_entity_len.has_value()) {
                auto const entity_len = opt_entity_len.value();
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

template<::pltxt2htm::Contracts ndebug>
constexpr void append_url_attr_from_ast(::fast_io::u8string& result, ::pltxt2htm::Url<ndebug> const& url_ast) noexcept {
    auto const url_str = ::pltxt2htm::details::convert_simple_pltxt_ast_to_plweb_text<ndebug>(url_ast.get_url_ast());
    // Under normal circumstances, `url_str` should never contain characters that could enable XSS in HTML attributes.
    // To avoid masking upstream bugs (and to keep release-path performance), we only assert this in debug mode.
    // Do not try to hide such errors by routing output through `append_html_attr_escaped`.
    if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
        ::fast_io::u8string purified_url{};
        ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
            purified_url, ::fast_io::u8string_view{url_str.data(), url_str.size()});
        pltxt2htm_assert(purified_url == url_str,
                         u8"URL contains characters that cannot be directly used in HTML attributes. Please "
                         u8"check the URL or use a different backend that supports escaping.");
    }
    result.append(url_str);
}

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
template<::pltxt2htm::Contracts ndebug, bool isfixed>
[[nodiscard]]
constexpr auto plweb_text_backend(::pltxt2htm::Ast<ndebug> const& ast_init, ::fast_io::u8string_view host,
                                  ::fast_io::u8string_view project, ::fast_io::u8string_view visitor,
                                  ::fast_io::u8string_view author, ::fast_io::u8string_view coauthors) noexcept
    -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    ::fast_io::stack<::pltxt2htm::details::BackendFrameContext<ndebug>> call_stack{};
    call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(ast_init, ::pltxt2htm::NodeKind::text, 0));

entry:
    while (true) {
        auto const& ast = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_ast();
        auto const& nested_tag_type = ::pltxt2htm::details::stack_top<ndebug>(call_stack).get_nested_tag_type();
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
            case ::pltxt2htm::NodeKind::pl_color: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_color().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_color, 0));
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
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_a().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_a, 0));
                ++current_index;
                constexpr auto open_tag = ::pltxt2htm::details::concat(
                    ::pltxt2htm::details::U8LiteralString{u8"<span style=\"color:"},
                    ::pltxt2htm::PlA<ndebug>::get_color_literal(), ::pltxt2htm::details::U8LiteralString{u8";\">"});
                result.append(::fast_io::u8string_view{open_tag.data(), open_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_experiment: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_pl_experiment().get_subast(), ::pltxt2htm::NodeKind::pl_experiment, 0));
                ++current_index;
                result.append(u8"<a href=\"");
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result, host);
                if constexpr (isfixed) {
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
                    bool const is_valid_experiment_id{purified_experiment_id == experiment_id};
                    pltxt2htm_assert(is_valid_experiment_id,
                                     u8"Experiment ID contains characters that cannot be directly used in HTML "
                                     u8"attributes. Please check "
                                     u8"the experiment ID or use a different backend that supports escaping.");
                }
                result.append(experiment_id);
                result.append(u8"\" internal>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_discussion: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_pl_discussion().get_subast(), ::pltxt2htm::NodeKind::pl_discussion, 0));
                ++current_index;
                result.append(u8"<a href=\"");
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result, host);
                if constexpr (isfixed) {
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
                    bool const is_valid_discussion_id{purified_discussion_id == discussion_id};
                    pltxt2htm_assert(is_valid_discussion_id,
                                     u8"Discussion ID contains characters that cannot be directly used in HTML "
                                     u8"attributes. Please check "
                                     u8"the discussion ID or use a different backend that supports escaping.");
                }
                result.append(discussion_id);
                result.append(u8"\" internal>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_user: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_user().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_user, 0));
                ++current_index;
                result.append(u8"<span class='RUser' data-user='");
                auto const& user_id = node.as_pl_user().get_id();
                // Under normal circumstances, `user_id` should never contain characters that could enable XSS in HTML
                // attributes. To avoid masking upstream bugs (and to keep release-path performance), we only assert
                // this in debug mode. Do not try to hide such errors by routing output through
                // `append_html_attr_escaped`.
                if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                    ::fast_io::u8string purified_user_id{};
                    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                        purified_user_id, ::fast_io::u8string_view{user_id.data(), user_id.size()});
                    bool const is_valid_user_id{purified_user_id == user_id};
                    pltxt2htm_assert(is_valid_user_id,
                                     u8"User ID contains characters that cannot be directly used in HTML attributes. "
                                     u8"Please check the "
                                     u8"user ID or use a different backend that supports escaping.");
                }
                result.append(user_id);
                result.append(u8"'>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_size: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_pl_size().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::pl_size, 0));
                ++current_index;
                result.append(u8"<span style=\"font-size:");
                result.append(::pltxt2htm::details::size_t2str(node.as_pl_size().get_size() / 2));
                result.append(u8"px\">");
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
            case ::pltxt2htm::NodeKind::html_a: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_a().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_a, 0));
                ++current_index;
                result.append(u8"<a href=\"");
                ::pltxt2htm::details::append_url_attr_from_ast<ndebug>(result, node.as_html_a().get_url());
                if (node.as_html_a().get_internal()) {
                    result.append(u8"\" internal>");
                }
                else {
                    result.append(u8"\">");
                }
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
            case ::pltxt2htm::NodeKind::html_p: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_p().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_p, 0));
                ++current_index;
                result.append(u8"<p>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::line_break: {
                if (nested_tag_type == ::pltxt2htm::NodeKind::md_code_fence_backtick ||
                    nested_tag_type == ::pltxt2htm::NodeKind::md_code_fence_tilde) {
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
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h1().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_h1, 0));
                ++current_index;
                result.append(u8"<h1>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h1: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h1().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_atx_h1, 0));
                ++current_index;
                result.append(u8"<h1>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h2: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h2().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_h2, 0));
                ++current_index;
                result.append(u8"<h2>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h2: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h2().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_atx_h2, 0));
                ++current_index;
                result.append(u8"<h2>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h3: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h3().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_h3, 0));
                ++current_index;
                result.append(u8"<h3>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h3: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h3().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_atx_h3, 0));
                ++current_index;
                result.append(u8"<h3>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h4: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h4().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_h4, 0));
                ++current_index;
                result.append(u8"<h4>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h4: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h4().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_atx_h4, 0));
                ++current_index;
                result.append(u8"<h4>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h5: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h5().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_h5, 0));
                ++current_index;
                result.append(u8"<h5>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h5: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h5().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_atx_h5, 0));
                ++current_index;
                result.append(u8"<h5>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_h6: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_h6().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_h6, 0));
                ++current_index;
                result.append(u8"<h6>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_atx_h6: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_atx_h6().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_atx_h6, 0));
                ++current_index;
                result.append(u8"<h6>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_del: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_del().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_del, 0));
                ++current_index;
                result.append(u8"<del>");
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
            case ::pltxt2htm::NodeKind::md_hr:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_hr: {
                result.append(u8"<hr>");
                continue;
            }
            case ::pltxt2htm::NodeKind::html_note: {
                continue;
            }
            case ::pltxt2htm::NodeKind::md_ul: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_ul().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_ul, 0));
                ++current_index;
                result.append(u8"<ul>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_ul: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_ul().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_ul, 0));
                ++current_index;
                result.append(u8"<ul>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_ol: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_ol().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_ol, 0));
                ++current_index;
                result.append(u8"<ol>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_ol: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_ol().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_ol, 0));
                ++current_index;
                result.append(u8"<ol>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_li_checkbox: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_li_checkbox().get_subast(), ::pltxt2htm::NodeKind::md_li_checkbox, 0));
                ++current_index;
                result.append(u8"<li>");
                if (node.as_md_li_checkbox().is_checked()) {
                    result.append(u8"<input type=\"checkbox\" disabled checked>");
                }
                else {
                    result.append(u8"<input type=\"checkbox\" disabled>");
                }
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_li: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_li().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_li, 0));
                ++current_index;
                result.append(u8"<li>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_li: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_html_li().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::html_li, 0));
                ++current_index;
                result.append(u8"<li>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_1_backtick: {
                /// @note Despite `<code></code>` is empty, we still need to handle it
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_code_span_1_backtick().get_subast(), ::pltxt2htm::NodeKind::md_code_span_1_backtick, 0));
                ++current_index;
                result.append(u8"<code>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_2_backtick: {
                /// @note Despite `<code></code>` is empty, we still need to handle it
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_code_span_2_backtick().get_subast(), ::pltxt2htm::NodeKind::md_code_span_2_backtick, 0));
                ++current_index;
                result.append(u8"<code>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_3_backtick: {
                /// @note Despite `<code></code>` is empty, we still need to handle it
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_code_span_3_backtick().get_subast(), ::pltxt2htm::NodeKind::md_code_span_3_backtick, 0));
                ++current_index;
                result.append(u8"<code>");
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
                // Note: Despite `<pre></pre>` is empty, we still need to handle it
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
                result.append(u8"<blockquote>");
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
                                                                                  ::pltxt2htm::NodeKind::md_table, 0));
                ++current_index;
                result.append(u8"<table>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_table: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_table().get_subast(), ::pltxt2htm::NodeKind::html_table, 0));
                ++current_index;
                result.append(u8"<table>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_tr: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_tr().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_tr, 0));
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
                                                                                  ::pltxt2htm::NodeKind::md_td, 0));
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
                result.append(u8"<td");
                auto const align = node.as_html_td().get_align();
                if (align == ::pltxt2htm::MdTableAlign::center) {
                    result.append(u8" style=\"text-align:center\"");
                }
                else if (align == ::pltxt2htm::MdTableAlign::right) {
                    result.append(u8" style=\"text-align:right\"");
                }
                result.push_back(u8'>');
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_th: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_th().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_th, 0));
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
                                                                                  ::pltxt2htm::NodeKind::md_thead, 0));
                ++current_index;
                result.append(u8"<thead>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_thead: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_thead().get_subast(), ::pltxt2htm::NodeKind::html_thead, 0));
                ++current_index;
                result.append(u8"<thead>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_tbody: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_tbody().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_tbody, 0));
                ++current_index;
                result.append(u8"<tbody>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_tbody: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_tbody().get_subast(), ::pltxt2htm::NodeKind::html_tbody, 0));
                ++current_index;
                result.append(u8"<tbody>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_tfoot: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_tfoot().get_subast(), ::pltxt2htm::NodeKind::html_tfoot, 0));
                ++current_index;
                result.append(u8"<tfoot>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_caption: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_caption().get_subast(), ::pltxt2htm::NodeKind::html_caption, 0));
                ++current_index;
                result.append(u8"<caption>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_colgroup: {
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_html_colgroup().get_subast(), ::pltxt2htm::NodeKind::html_colgroup, 0));
                ++current_index;
                result.append(u8"<colgroup>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::html_col: {
                result.append(u8"<col>");
                continue;
            }
            case ::pltxt2htm::NodeKind::html_input: {
                if (node.as_html_input().is_checked()) {
                    result.append(u8"<input type=\"checkbox\" disabled checked>");
                }
                else {
                    result.append(u8"<input type=\"checkbox\" disabled>");
                }
                continue;
            }
            case ::pltxt2htm::NodeKind::html_img: {
                result.append(u8"<img src=\"");
                {
                    auto const& src = node.as_html_img().get_src();
                    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result,
                                                                          ::fast_io::u8string_view{src.data(), src.size()});
                }
                result.append(u8"\" alt=\"");
                {
                    auto const& alt = node.as_html_img().get_alt();
                    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result,
                                                                          ::fast_io::u8string_view{alt.data(), alt.size()});
                }
                result.append(u8"\">");
                continue;
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
            case ::pltxt2htm::NodeKind::url: {
                result.append(u8"<a href=\"");
                ::pltxt2htm::details::append_url_attr_from_ast<ndebug>(result, node.as_url());
                result.append(u8"\">");
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_url().get_url_ast(),
                                                                                  ::pltxt2htm::NodeKind::url, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_link: {
                result.append(u8"<a href=\"");
                ::pltxt2htm::details::append_url_attr_from_ast<ndebug>(result, node.as_md_link().get_url());
                result.append(u8"\">");
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(node.as_md_link().get_subast(),
                                                                                  ::pltxt2htm::NodeKind::md_link, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_external: {
                result.append(u8"<a href=\"");
                ::pltxt2htm::details::append_url_attr_from_ast<ndebug>(result, node.as_pl_external().get_url());
                result.append(u8"\">");
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_pl_external().get_subast(), ::pltxt2htm::NodeKind::pl_external, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_image: {
                result.append(u8"<img src=\"");
                ::pltxt2htm::details::append_url_attr_from_ast<ndebug>(result, node.as_md_image().get_url());
                result.append(u8"\" alt=\"");
                result.append(::pltxt2htm::details::convert_simple_pltxt_ast_to_plweb_text<ndebug>(
                    node.as_md_image().get_subast()));
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
            case ::pltxt2htm::NodeKind::md_code_fence_backtick: {
                auto const& opt_language = node.as_md_code_fence_backtick().get_language();
                if (opt_language.has_value()) {
                    auto const& language = opt_language.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                    result.append(u8"<pre><code class=\"language-");
                    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                        result, ::fast_io::u8string_view{language.data(), language.size()});
                    result.append(u8"\">");
                }
                else {
                    result.append(u8"<pre><code>");
                }
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_code_fence_backtick().get_subast(), ::pltxt2htm::NodeKind::md_code_fence_backtick, 0));
                ++current_index;
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_fence_tilde: {
                auto const& opt_language = node.as_md_code_fence_tilde().get_language();
                if (opt_language.has_value()) {
                    auto const& language = opt_language.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                    result.append(u8"<pre><code class=\"language-");
                    ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                        result, ::fast_io::u8string_view{language.data(), language.size()});
                    result.append(u8"\">");
                }
                else {
                    result.append(u8"<pre><code>");
                }
                call_stack.push(::pltxt2htm::details::BackendFrameContext<ndebug>(
                    node.as_md_code_fence_tilde().get_subast(), ::pltxt2htm::NodeKind::md_code_fence_tilde, 0));
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
            auto top_frame = ::std::move(::pltxt2htm::details::stack_top<ndebug>(call_stack));
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
            case ::pltxt2htm::NodeKind::pl_user: {
                result.append(u8"</span>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_size: {
                result.append(u8"</span>");
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
            case ::pltxt2htm::NodeKind::md_ul:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_ul: {
                result.append(u8"</ul>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_ol:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_ol: {
                result.append(u8"</ol>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_li_checkbox:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_li:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_li: {
                result.append(u8"</li>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::md_code_span_1_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_code_span_2_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_code_span_3_backtick:
                [[fallthrough]];
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
            case ::pltxt2htm::NodeKind::md_block_quotes:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::html_blockquote: {
                result.append(u8"</blockquote>");
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
            case ::pltxt2htm::NodeKind::md_code_fence_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeKind::md_code_fence_tilde: {
                result.append(u8"</code></pre>");
                goto entry;
            }
            case ::pltxt2htm::NodeKind::pl_external:
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
