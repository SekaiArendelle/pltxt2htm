/**
 * @file for_plweb_text.hh
 * @brief Advanced HTML backend for pltxt2htm
 * @details Generates full-featured HTML output with comprehensive support for
 *          Physics-Lab tags, Markdown syntax, and HTML elements.
 */

#pragma once

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
#include "../../details/parser/try_parse.hh"
#include "../../astnode/basic.hh"
#include "../../astnode/node_type.hh"
#include "../../astnode/markdown_node.hh"
#include "../../astnode/physics_lab_node.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto convert_simple_pltxt_ast_to_plweb_text(::pltxt2htm::Ast const& ast) noexcept -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    for (auto&& node : ast) {
        switch (node->node_type()) {
        case ::pltxt2htm::NodeType::u8char: {
            result.push_back(static_cast<::pltxt2htm::U8Char const*>(node.release_imul())->get_u8char());
            continue;
        }
        case ::pltxt2htm::NodeType::invalid_u8char: {
            result.append(u8"\uFFFD");
            continue;
        }
        case ::pltxt2htm::NodeType::space: {
            result.append(u8"&nbsp;");
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::ampersand: {
            result.append(u8"&amp;");
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::single_quote: {
            result.append(u8"&apos;");
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::double_quote: {
            result.append(u8"&quot;");
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::less_than: {
            result.append(u8"&lt;");
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::greater_than: {
            result.append(u8"&gt;");
            continue;
        }
        case ::pltxt2htm::NodeType::tab: {
            result.append(u8"&nbsp;&nbsp;&nbsp;&nbsp;");
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_backslash: {
            result.push_back(u8'\\');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_exclamation: {
            result.push_back(u8'!');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_hash: {
            result.push_back(u8'#');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_dollar: {
            result.push_back(u8'$');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_percent: {
            result.push_back(u8'%');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_left_paren: {
            result.push_back(u8'(');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_right_paren: {
            result.push_back(u8')');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_asterisk: {
            result.push_back(u8'*');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_plus: {
            result.push_back(u8'+');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_comma: {
            result.push_back(u8',');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_hyphen: {
            result.push_back(u8'-');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_dot: {
            result.push_back(u8'.');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_slash: {
            result.push_back(u8'/');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_colon: {
            result.push_back(u8':');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_semicolon: {
            result.push_back(u8';');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_equals: {
            result.push_back(u8'=');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_question: {
            result.push_back(u8'?');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_at: {
            result.push_back(u8'@');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_left_bracket: {
            result.push_back(u8'[');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_right_bracket: {
            result.push_back(u8']');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_caret: {
            result.push_back(u8'^');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_underscore: {
            result.push_back(u8'_');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_backtick: {
            result.push_back(u8'`');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_left_brace: {
            result.push_back(u8'{');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_pipe: {
            result.push_back(u8'|');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_right_brace: {
            result.push_back(u8'}');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_tilde: {
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
constexpr void append_url_attr_from_ast(::fast_io::u8string& result, ::pltxt2htm::Ast const& url_ast) noexcept {
    auto const url_str = ::pltxt2htm::details::convert_simple_pltxt_ast_to_plweb_text<ndebug>(url_ast);
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
template<::pltxt2htm::Contracts ndebug, bool isfixed = false>
[[nodiscard]]
constexpr auto plweb_text_backend(::pltxt2htm::Ast const& ast_init, ::fast_io::u8string_view host,
                                  ::fast_io::u8string_view project, ::fast_io::u8string_view visitor,
                                  ::fast_io::u8string_view author, ::fast_io::u8string_view coauthors) noexcept
    -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    ::fast_io::stack<::pltxt2htm::details::BackendBasicFrameContext> call_stack{};
    call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(ast_init, ::pltxt2htm::NodeType::base, 0));

entry:
    auto const& ast = call_stack.top().ast_;
    auto const& nested_tag_type = call_stack.top().get_nested_tag_type();
    auto&& current_index = call_stack.top().current_index_;
    for (; current_index < ast.size(); ++current_index) {
        auto&& node = ::pltxt2htm::details::vector_index<ndebug>(ast, current_index);

        switch (node->node_type()) {
        case ::pltxt2htm::NodeType::u8char: {
            result.push_back(static_cast<::pltxt2htm::U8Char const*>(node.release_imul())->get_u8char());
            continue;
        }
        case ::pltxt2htm::NodeType::invalid_u8char: {
            result.append(u8"\uFFFD");
            continue;
        }
        case ::pltxt2htm::NodeType::text: {
            auto text = static_cast<::pltxt2htm::Text const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(text->get_subast(), ::pltxt2htm::NodeType::text, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeType::space: {
            result.append(u8"&nbsp;");
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::ampersand: {
            result.append(u8"&amp;");
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::single_quote: {
            result.append(u8"&apos;");
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::double_quote: {
            result.append(u8"&quot;");
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::less_than: {
            result.append(u8"&lt;");
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::greater_than: {
            result.append(u8"&gt;");
            continue;
        }
        case ::pltxt2htm::NodeType::tab: {
            result.append(u8"&nbsp;&nbsp;&nbsp;&nbsp;");
            continue;
        }
        case ::pltxt2htm::NodeType::pl_color: {
            auto color = static_cast<::pltxt2htm::Color const*>(node.release_imul());

            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(color->get_subast(),
                                                                           ::pltxt2htm::NodeType::pl_color, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view close_tag1 = u8"<span style=\"color:";
            result.append(::fast_io::u8string_view{close_tag1.data(), close_tag1.size()});
            auto const& color_value = color->get_color();
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
            constexpr ::fast_io::u8string_view close_tag2 = u8";\">";
            result.append(::fast_io::u8string_view{close_tag2.data(), close_tag2.size()});
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_a: {
            auto anchor = static_cast<::pltxt2htm::A const*>(node.release_imul());

            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(anchor->get_subast(), ::pltxt2htm::NodeType::pl_a, 0));
            ++current_index;
            constexpr auto open_tag = ::pltxt2htm::details::concat(
                ::pltxt2htm::details::U8LiteralString{u8"<span style=\"color:"}, ::pltxt2htm::A::get_color_literal(),
                ::pltxt2htm::details::U8LiteralString{u8";\">"});
            result.append(::fast_io::u8string_view{open_tag.data(), open_tag.size()});
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_experiment: {
            auto experiment = static_cast<::pltxt2htm::Experiment const*>(node.release_imul());
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(experiment->get_subast(),
                                                                           ::pltxt2htm::NodeType::pl_experiment, 0));
            ++current_index;
            result.append(u8"<a href=\"");
            ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result, host);
            if constexpr (isfixed) {
                result.append(u8"/p/Experiment/");
            } else {
                result.append(u8"/ExperimentSummary/Experiment/");
            }
            auto const& experiment_id = experiment->get_id();
            // Under normal circumstances, `experiment_id` should never contain characters that could enable XSS in
            // HTML attributes. To avoid masking upstream bugs (and to keep release-path performance), we only
            // assert this in debug mode. Do not try to hide such errors by routing output through
            // `append_html_attr_escaped`.
            if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                ::fast_io::u8string purified_experiment_id{};
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    purified_experiment_id, ::fast_io::u8string_view{experiment_id.data(), experiment_id.size()});
                bool const is_valid_experiment_id{purified_experiment_id == experiment_id};
                pltxt2htm_assert(
                    is_valid_experiment_id,
                    u8"Experiment ID contains characters that cannot be directly used in HTML attributes. Please check "
                    u8"the experiment ID or use a different backend that supports escaping.");
            }
            result.append(experiment_id);
            result.append(u8"\" internal>");
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_discussion: {
            auto discussion = static_cast<::pltxt2htm::Discussion const*>(node.release_imul());
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(discussion->get_subast(),
                                                                           ::pltxt2htm::NodeType::pl_discussion, 0));
            ++current_index;
            result.append(u8"<a href=\"");
            ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result, host);
            if constexpr (isfixed) {
                result.append(u8"/p/Discussion/");
            } else {
                result.append(u8"/ExperimentSummary/Discussion/");
            }
            auto const& discussion_id = discussion->get_id();
            // Under normal circumstances, `discussion_id` should never contain characters that could enable XSS in
            // HTML attributes. To avoid masking upstream bugs (and to keep release-path performance), we only
            // assert this in debug mode. Do not try to hide such errors by routing output through
            // `append_html_attr_escaped`.
            if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                ::fast_io::u8string purified_discussion_id{};
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    purified_discussion_id, ::fast_io::u8string_view{discussion_id.data(), discussion_id.size()});
                bool const is_valid_discussion_id{purified_discussion_id == discussion_id};
                pltxt2htm_assert(
                    is_valid_discussion_id,
                    u8"Discussion ID contains characters that cannot be directly used in HTML attributes. Please check "
                    u8"the discussion ID or use a different backend that supports escaping.");
            }
            result.append(discussion_id);
            result.append(u8"\" internal>");
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_user: {
            auto user = static_cast<::pltxt2htm::User const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(user->get_subast(), ::pltxt2htm::NodeType::pl_user, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view open_tag1 = u8"<span class='RUser' data-user='";
            result.append(::fast_io::u8string_view{open_tag1.data(), open_tag1.size()});
            auto const& user_id = user->get_id();
            // Under normal circumstances, `user_id` should never contain characters that could enable XSS in HTML
            // attributes. To avoid masking upstream bugs (and to keep release-path performance), we only assert
            // this in debug mode. Do not try to hide such errors by routing output through
            // `append_html_attr_escaped`.
            if constexpr (ndebug == ::pltxt2htm::Contracts::quick_enforce) {
                ::fast_io::u8string purified_user_id{};
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    purified_user_id, ::fast_io::u8string_view{user_id.data(), user_id.size()});
                bool const is_valid_user_id{purified_user_id == user_id};
                pltxt2htm_assert(
                    is_valid_user_id,
                    u8"User ID contains characters that cannot be directly used in HTML attributes. Please check the "
                    u8"user ID or use a different backend that supports escaping.");
            }
            result.append(user_id);
            constexpr ::fast_io::u8string_view open_tag2 = u8"'>";
            result.append(::fast_io::u8string_view{open_tag2.data(), open_tag2.size()});
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_size: {
            auto size = static_cast<::pltxt2htm::Size const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(size->get_subast(), ::pltxt2htm::NodeType::pl_size, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view open_tag1 = u8"<span style=\"font-size:";
            result.append(::fast_io::u8string_view{open_tag1.data(), open_tag1.size()});
            result.append(::pltxt2htm::details::size_t2str(size->get_id() / 2));
            constexpr ::fast_io::u8string_view open_tag2 = u8"px\">";
            result.append(::fast_io::u8string_view{open_tag2.data(), open_tag2.size()});
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_double_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_double_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_b:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_strong: {
            auto b = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(b->get_subast(), ::pltxt2htm::NodeType::html_strong, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<strong>";
            result.append(::fast_io::u8string_view{start_tag.data(), start_tag.size()});
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_p: {
            auto p = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(p->get_subast(), ::pltxt2htm::NodeType::html_p, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<p>";
            result.append(::fast_io::u8string_view{start_tag.data(), start_tag.size()});
            goto entry;
        }
        case ::pltxt2htm::NodeType::line_break: {
            if (nested_tag_type == ::pltxt2htm::NodeType::md_code_fence_backtick ||
                nested_tag_type == ::pltxt2htm::NodeType::md_code_fence_tilde) {
                result.push_back('\n');
            }
            else {
                constexpr ::fast_io::u8string_view start_tag = u8"<br>";
                result.append(::fast_io::u8string_view{start_tag.data(), start_tag.size()});
            }
            continue;
        }
        case ::pltxt2htm::NodeType::html_br: {
            constexpr ::fast_io::u8string_view start_tag = u8"<br>";
            result.append(::fast_io::u8string_view{start_tag.data(), start_tag.size()});
            continue;
        }
        case ::pltxt2htm::NodeType::html_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h1: {
            auto h1 = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(h1->get_subast(), ::pltxt2htm::NodeType::html_h1, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<h1>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h2: {
            auto h2 = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(h2->get_subast(), ::pltxt2htm::NodeType::html_h2, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<h2>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h3: {
            auto h3 = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(h3->get_subast(), ::pltxt2htm::NodeType::html_h3, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<h3>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h4: {
            auto h4 = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(h4->get_subast(), ::pltxt2htm::NodeType::html_h4, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<h4>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h5: {
            auto h5 = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(h5->get_subast(), ::pltxt2htm::NodeType::html_h5, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<h5>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h6: {
            auto h6 = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(h6->get_subast(), ::pltxt2htm::NodeType::html_h6, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<h6>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_del: {
            auto del = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(del->get_subast(), ::pltxt2htm::NodeType::html_del, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<del>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_single_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_single_emphasis_asterisk:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_i:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_em: {
            auto em = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(em->get_subast(), ::pltxt2htm::NodeType::html_em, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<em>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_hr: {
            constexpr ::fast_io::u8string_view start_tag = u8"<hr>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            continue;
        }
        case ::pltxt2htm::NodeType::html_note: {
            continue;
        }
        case ::pltxt2htm::NodeType::md_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_ul: {
            auto ul = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(ul->get_subast(), ::pltxt2htm::NodeType::html_ul, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<ul>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_ol: {
            auto ol = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(ol->get_subast(), ::pltxt2htm::NodeType::html_ol, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<ol>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_li: {
            auto li = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(li->get_subast(), ::pltxt2htm::NodeType::html_li, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<li>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_code_span_1_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_code_span_2_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_code_span_3_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_code: {
            auto code = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            // Note: Despite `<code></code>` is empty, we still need to handle it
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(code->get_subast(),
                                                                           ::pltxt2htm::NodeType::html_code, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<code>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_latex_inline: {
            auto latex_inline = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(latex_inline->get_subast(),
                                                                           ::pltxt2htm::NodeType::md_latex_inline, 0));
            ++current_index;
            result.append(u8"$");
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_latex_block: {
            auto latex_block = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(latex_block->get_subast(),
                                                                           ::pltxt2htm::NodeType::md_latex_block, 0));
            ++current_index;
            result.append(u8"$$");
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_pre: {
            auto pre = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            // Note: Despite `<pre></pre>` is empty, we still need to handle it
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(pre->get_subast(), ::pltxt2htm::NodeType::html_pre, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<pre>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_block_quotes:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_blockquote: {
            auto blockquote = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            // Note: Despite `<blockquote></blockquote>` is empty, we still need to handle it
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(blockquote->get_subast(),
                                                                           ::pltxt2htm::NodeType::html_blockquote, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<blockquote>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_triple_emphasis_underscore:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk: {
            auto triple_emphasis = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(
                triple_emphasis->get_subast(), ::pltxt2htm::NodeType::md_triple_emphasis_asterisk, 0));
            ++current_index;
            constexpr ::fast_io::u8string_view start_tag = u8"<em><strong>";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_link: {
            auto a_link = static_cast<::pltxt2htm::MdLink const*>(node.release_imul());
            constexpr ::fast_io::u8string_view start_tag = u8"<a href=\"";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            ::pltxt2htm::details::append_url_attr_from_ast<ndebug>(result, a_link->url_.get_url_ast());
            constexpr ::fast_io::u8string_view mid_tag = u8"\">";
            result.append(::fast_io::u8string_view(mid_tag.begin(), mid_tag.size()));
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(a_link->get_subast(),
                                                                           ::pltxt2htm::NodeType::md_link, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_external: {
            auto external = static_cast<::pltxt2htm::External const*>(node.release_imul());
            constexpr ::fast_io::u8string_view start_tag = u8"<a href=\"";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            ::pltxt2htm::details::append_url_attr_from_ast<ndebug>(result, external->get_url().get_url_ast());
            constexpr ::fast_io::u8string_view mid_tag = u8"\">";
            result.append(::fast_io::u8string_view(mid_tag.begin(), mid_tag.size()));
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(external->get_subast(),
                                                                           ::pltxt2htm::NodeType::pl_external, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_image: {
            auto a_image = static_cast<::pltxt2htm::MdImage const*>(node.release_imul());

            constexpr ::fast_io::u8string_view start_tag = u8"<img src=\"";
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            ::pltxt2htm::details::append_url_attr_from_ast<ndebug>(result, a_image->url_.get_url_ast());
            constexpr ::fast_io::u8string_view mid_tag = u8"\" alt=\"";
            result.append(::fast_io::u8string_view(mid_tag.begin(), mid_tag.size()));
            result.append(::pltxt2htm::details::convert_simple_pltxt_ast_to_plweb_text<ndebug>(a_image->get_subast()));
            constexpr ::fast_io::u8string_view end_tag = u8"\">";
            result.append(::fast_io::u8string_view(end_tag.begin(), end_tag.size()));
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_backslash: {
            result.push_back(u8'\\');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_exclamation: {
            result.push_back(u8'!');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_hash: {
            result.push_back(u8'#');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_dollar: {
            result.push_back(u8'$');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_percent: {
            result.push_back(u8'%');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_left_paren: {
            result.push_back(u8'(');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_right_paren: {
            result.push_back(u8')');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_asterisk: {
            result.push_back(u8'*');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_plus: {
            result.push_back(u8'+');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_comma: {
            result.push_back(u8',');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_hyphen: {
            result.push_back(u8'-');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_dot: {
            result.push_back(u8'.');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_slash: {
            result.push_back(u8'/');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_colon: {
            result.push_back(u8':');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_semicolon: {
            result.push_back(u8';');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_equals: {
            result.push_back(u8'=');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_question: {
            result.push_back(u8'?');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_at: {
            result.push_back(u8'@');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_left_bracket: {
            result.push_back(u8'[');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_right_bracket: {
            result.push_back(u8']');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_caret: {
            result.push_back(u8'^');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_underscore: {
            result.push_back(u8'_');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_backtick: {
            result.push_back(u8'`');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_left_brace: {
            result.push_back(u8'{');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_pipe: {
            result.push_back(u8'|');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_right_brace: {
            result.push_back(u8'}');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_tilde: {
            result.push_back(u8'~');
            continue;
        }
        case ::pltxt2htm::NodeType::md_code_fence_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_code_fence_tilde: {
            auto code_fence = static_cast<::pltxt2htm::MdCodeFenceBacktick const*>(node.release_imul());
            auto const& opt_language = code_fence->get_language();
            if (opt_language.has_value()) {
                auto const& language = opt_language.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
                constexpr ::fast_io::u8string_view start_tag = u8"<pre><code class=\"language-";
                result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
                ::pltxt2htm::details::append_html_attr_escaped<ndebug>(
                    result, ::fast_io::u8string_view{language.data(), language.size()});
                constexpr ::fast_io::u8string_view start_tag2 = u8"\">";
                result.append(::fast_io::u8string_view(start_tag2.begin(), start_tag2.size()));
            }
            else {
                constexpr ::fast_io::u8string_view start_tag = u8"<pre><code>";
                result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            }
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(
                code_fence->get_subast(), ::pltxt2htm::NodeType::md_code_fence_backtick, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_macro_project: {
            ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result, project);
            continue;
        }
        case ::pltxt2htm::NodeType::pl_macro_visitor: {
            ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result, visitor);
            continue;
        }
        case ::pltxt2htm::NodeType::pl_macro_author: {
            ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result, author);
            continue;
        }
        case ::pltxt2htm::NodeType::pl_macro_coauthors: {
            ::pltxt2htm::details::append_html_attr_escaped<ndebug>(result, coauthors);
            continue;
        }
        case ::pltxt2htm::NodeType::base:
#if 0
            [[unlikely]] [[fallthrough]];
        default:
#endif
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
    }

    {
        auto top_frame = ::std::move(call_stack.top());
        call_stack.pop();
        if (call_stack.empty()) {
            return result;
        }
        else {
            switch (top_frame.get_nested_tag_type()) {
            case ::pltxt2htm::NodeType::text: {
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_a:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_color: {
                constexpr ::fast_io::u8string_view close_tag = u8"</span>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_experiment: {
                constexpr ::fast_io::u8string_view close_tag = u8"</a>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_discussion: {
                constexpr ::fast_io::u8string_view close_tag = u8"</a>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_user: {
                constexpr ::fast_io::u8string_view close_tag = u8"</span>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_size: {
                constexpr ::fast_io::u8string_view close_tag = u8"</span>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_double_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_double_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_b:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_strong: {
                constexpr ::fast_io::u8string_view close_tag = u8"</strong>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_single_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_single_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_i:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_em: {
                constexpr ::fast_io::u8string_view close_tag = u8"</em>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_triple_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk: {
                constexpr ::fast_io::u8string_view close_tag = u8"</strong></em>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_p: {
                constexpr ::fast_io::u8string_view close_tag = u8"</p>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h1: {
                constexpr ::fast_io::u8string_view close_tag = u8"</h1>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h2: {
                constexpr ::fast_io::u8string_view close_tag = u8"</h2>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h3: {
                constexpr ::fast_io::u8string_view close_tag = u8"</h3>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h4: {
                constexpr ::fast_io::u8string_view close_tag = u8"</h4>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h5: {
                constexpr ::fast_io::u8string_view close_tag = u8"</h5>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h6: {
                constexpr ::fast_io::u8string_view close_tag = u8"</h6>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_del:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_del: {
                constexpr ::fast_io::u8string_view close_tag = u8"</del>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_ul:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_ul: {
                constexpr ::fast_io::u8string_view close_tag = u8"</ul>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_ol:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_ol: {
                constexpr ::fast_io::u8string_view close_tag = u8"</ol>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_li: {
                constexpr ::fast_io::u8string_view close_tag = u8"</li>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_code_span_1_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_code_span_2_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_code_span_3_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_code: {
                constexpr ::fast_io::u8string_view close_tag = u8"</code>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_latex_inline: {
                result.append(u8"$");
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_latex_block: {
                result.append(u8"$$");
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_pre: {
                constexpr ::fast_io::u8string_view close_tag = u8"</pre>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_block_quotes:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_blockquote: {
                constexpr ::fast_io::u8string_view close_tag = u8"</blockquote>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_code_fence_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_code_fence_tilde: {
                constexpr ::fast_io::u8string_view close_tag = u8"</code></pre>";
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_external:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_link: {
                constexpr ::fast_io::u8string_view end_tag = u8"</a>";
                result.append(::fast_io::u8string_view(end_tag.begin(), end_tag.size()));
                goto entry;
            }
            case ::pltxt2htm::NodeType::base:
                [[fallthrough]];
            default:
                [[unlikely]] {
                    ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
                }
            }
        }
    }
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
