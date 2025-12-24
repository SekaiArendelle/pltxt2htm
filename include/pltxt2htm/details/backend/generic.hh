/**
 * @file advanced_html.hh
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
#include "../../details/utils.hh"
#include "../../astnode/basic.hh"
#include "../../astnode/node_type.hh"
#include "../../astnode/markdown_node.hh"
#include "../../astnode/physics_lab_node.hh"

namespace pltxt2htm::details {

enum class backend_mode : ::std::size_t {
    // normal_html only been used for unittest
    normal_html = 0,
    // TODO title_html implemented in common_html.hh, maybe I can migrate it to this file
    // title_html,
    // main_text_html - used for rendering main text in plweb2
    main_text_html,
};

/**
 * @brief Convert AST nodes to advanced HTML with full feature support
 * @details This backend generates comprehensive HTML output supporting:
 *          - Physics-Lab specific tags (color, experiment, discussion, user, size)
 *          - Markdown syntax (headers, lists, emphasis, links, etc.)
 *          - HTML elements (paragraphs, line breaks, etc.)
 *          - Proper HTML escaping and encoding
 * @tparam ndebug Debug mode flag - true for release mode (faster, fewer checks),
 *                false for debug mode (slower, more safety checks)
 * @param[in] ast_init The AST to convert to HTML
 * @param[in] host Host URL for generating internal links (used for experiment/discussion links)
 * @return A string containing the generated HTML
 * @note To avoid stack overflow, this function manages call_stack manually using goto-based state machine
 */
template<bool ndebug, ::pltxt2htm::details::backend_mode backend_mode>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto generic_backend(::pltxt2htm::Ast const& ast_init, ::fast_io::u8string_view host) noexcept
    -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    ::fast_io::stack<::pltxt2htm::details::BackendBasicFrameContext,
                     ::fast_io::list<::pltxt2htm::details::BackendBasicFrameContext>>
        call_stack{};
    call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(ast_init, ::pltxt2htm::NodeType::base, 0));

entry:
    auto const& ast = call_stack.top().ast_;
    auto const& nested_tag_type = call_stack.top().nested_tag_type_;
    auto&& current_index = call_stack.top().current_index_;
    for (; current_index < ast.size(); ++current_index) {
        auto&& node = ::pltxt2htm::details::vector_index<ndebug>(ast, current_index);

        switch (node->node_type()) {
        case ::pltxt2htm::NodeType::u8char: {
            result.push_back(static_cast<::pltxt2htm::U8Char const*>(node.release_imul())->get_u8char());
            break;
        }
        case ::pltxt2htm::NodeType::invalid_u8char: {
            auto const escape_str = ::fast_io::array{char8_t{0xef}, 0xbf, 0xbd};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::text: {
            auto text = static_cast<::pltxt2htm::Text const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(text->get_subast(), ::pltxt2htm::NodeType::text, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeType::space: {
            auto const escape_str = ::fast_io::array{u8'&', u8'n', u8'b', u8's', u8'p', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::ampersand: {
            auto const escape_str = ::fast_io::array{u8'&', u8'a', u8'm', u8'p', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::single_quote: {
            auto const escape_str = ::fast_io::array{u8'&', u8'a', u8'p', u8'o', u8's', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::double_quote: {
            auto const escape_str = ::fast_io::array{u8'&', u8'q', u8'u', u8'o', u8't', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::less_than: {
            auto const escape_str = ::fast_io::array{u8'&', u8'l', u8't', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::greater_than: {
            auto const escape_str = ::fast_io::array{u8'&', u8'g', u8't', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::tab: {
            auto const escape_str =
                ::fast_io::array{u8'&', u8'n', u8'b', u8's', u8'p', u8';', u8'&', u8'n', u8'b', u8's', u8'p', u8';',
                                 u8'&', u8'n', u8'b', u8's', u8'p', u8';', u8'&', u8'n', u8'b', u8's', u8'p', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_a: {
            // <a> and <color> is the same tag&struct in fact
            auto color = static_cast<::pltxt2htm::Color const*>(node.release_imul());

            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(color->get_subast(),
                                                                           ::pltxt2htm::NodeType::pl_color, 0));
            ++current_index;
            auto const close_tag1 =
                ::fast_io::array{u8'<', u8's', u8'p',  u8'a', u8'n', u8' ', u8's', u8't', u8'y', u8'l',
                                 u8'e', u8'=', u8'\"', u8'c', u8'o', u8'l', u8'o', u8'r', u8':'};
            result.append(::fast_io::u8string_view{close_tag1.data(), close_tag1.size()});
            result.append(color->get_color());
            auto const close_tag2 = ::fast_io::array{u8';', u8'\"', u8'>'};
            result.append(::fast_io::u8string_view{close_tag2.data(), close_tag2.size()});
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_experiment: {
            auto experiment = static_cast<::pltxt2htm::Experiment const*>(node.release_imul());
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(experiment->get_subast(),
                                                                           ::pltxt2htm::NodeType::pl_experiment, 0));
            ++current_index;
            result.append(u8"<a href=\"");
            result.append(host);
            result.append(u8"/ExperimentSummary/Experiment/");
            result.append(experiment->get_id());
            result.append(u8"\" internal>");
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_discussion: {
            auto discussion = static_cast<::pltxt2htm::Discussion const*>(node.release_imul());
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(discussion->get_subast(),
                                                                           ::pltxt2htm::NodeType::pl_discussion, 0));
            ++current_index;
            result.append(u8"<a href=\"");
            result.append(host);
            result.append(u8"/ExperimentSummary/Discussion/");
            result.append(discussion->get_id());
            result.append(u8"\" internal>");
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_user: {
            auto user = static_cast<::pltxt2htm::User const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(user->get_subast(), ::pltxt2htm::NodeType::pl_user, 0));
            ++current_index;
            auto const open_tag1 =
                ::fast_io::array{u8'<', u8's',  u8'p', u8'a', u8'n', u8' ', u8'c', u8'l',  u8'a', u8's', u8's',
                                 u8'=', u8'\'', u8'R', u8'U', u8's', u8'e', u8'r', u8'\'', u8' ', u8'd', u8'a',
                                 u8't', u8'a',  u8'-', u8'u', u8's', u8'e', u8'r', u8'=',  u8'\''};
            result.append(::fast_io::u8string_view{open_tag1.data(), open_tag1.size()});
            result.append(user->get_id());
            auto const open_tag2 = ::fast_io::array{u8'\'', u8'>'};
            result.append(::fast_io::u8string_view{open_tag2.data(), open_tag2.size()});
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_size: {
            auto size = static_cast<::pltxt2htm::Size const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(size->get_subast(), ::pltxt2htm::NodeType::pl_size, 0));
            ++current_index;
            auto const open_tag1 =
                ::fast_io::array{u8'<',  u8's', u8'p', u8'a', u8'n', u8' ', u8's', u8't', u8'y', u8'l', u8'e', u8'=',
                                 u8'\"', u8'f', u8'o', u8'n', u8't', u8'-', u8's', u8'i', u8'z', u8'e', u8':'};
            result.append(::fast_io::u8string_view{open_tag1.data(), open_tag1.size()});
            result.append(::pltxt2htm::details::size_t2str(size->get_id() / 2));
            auto const open_tag2 = ::fast_io::array{u8'p', u8'x', u8'\"', u8'>'};
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
            auto const start_tag = ::fast_io::array{u8'<', u8's', u8't', u8'r', u8'o', u8'n', u8'g', u8'>'};
            result.append(::fast_io::u8string_view{start_tag.data(), start_tag.size()});
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_p: {
            auto p = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(p->get_subast(), ::pltxt2htm::NodeType::html_p, 0));
            ++current_index;
            auto const start_tag = ::fast_io::array{u8'<', u8'p', u8'>'};
            result.append(::fast_io::u8string_view{start_tag.data(), start_tag.size()});
            goto entry;
        }
        case ::pltxt2htm::NodeType::line_break: {
            if (nested_tag_type == ::pltxt2htm::NodeType::md_code_fence_backtick ||
                nested_tag_type == ::pltxt2htm::NodeType::md_code_fence_tilde) {
                result.push_back('\n');
            } else {
                auto const start_tag = ::fast_io::array{u8'<', u8'b', u8'r', u8'>'};
                result.append(::fast_io::u8string_view{start_tag.data(), start_tag.size()});
            }
            break;
        }
        case ::pltxt2htm::NodeType::html_br: {
            auto const start_tag = ::fast_io::array{u8'<', u8'b', u8'r', u8'>'};
            result.append(::fast_io::u8string_view{start_tag.data(), start_tag.size()});
            break;
        }
        case ::pltxt2htm::NodeType::html_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h1: {
            auto h1 = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(h1->get_subast(), ::pltxt2htm::NodeType::html_h1, 0));
            ++current_index;
            auto const start_tag = ::fast_io::array{u8'<', u8'h', u8'1', u8'>'};
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
            auto const start_tag = ::fast_io::array{u8'<', u8'h', u8'2', u8'>'};
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
            auto const start_tag = ::fast_io::array{u8'<', u8'h', u8'3', u8'>'};
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
            auto const start_tag = ::fast_io::array{u8'<', u8'h', u8'4', u8'>'};
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
            auto const start_tag = ::fast_io::array{u8'<', u8'h', u8'5', u8'>'};
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
            auto const start_tag = ::fast_io::array{u8'<', u8'h', u8'6', u8'>'};
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
            auto const start_tag = ::fast_io::array{u8'<', u8'd', u8'e', u8'l', u8'>'};
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
            auto const start_tag = ::fast_io::array{u8'<', u8'e', u8'm', u8'>'};
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_hr: {
            auto const start_tag = ::fast_io::array{u8'<', u8'h', u8'r', u8'>'};
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            break;
        }
        case ::pltxt2htm::NodeType::html_note: {
            break;
        }
        case ::pltxt2htm::NodeType::md_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_ul: {
            auto ul = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(ul->get_subast(), ::pltxt2htm::NodeType::html_ul, 0));
            ++current_index;
            auto const start_tag = ::fast_io::array{u8'<', u8'u', u8'l', u8'>'};
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
            auto const start_tag = ::fast_io::array{u8'<', u8'o', u8'l', u8'>'};
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
            auto const start_tag = ::fast_io::array{u8'<', u8'l', u8'i', u8'>'};
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
            auto const start_tag = ::fast_io::array{u8'<', u8'c', u8'o', u8'd', u8'e', u8'>'};
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_pre: {
            auto pre = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            // Note: Despite `<pre></pre>` is empty, we still need to handle it
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(pre->get_subast(), ::pltxt2htm::NodeType::html_pre, 0));
            ++current_index;
            auto const start_tag = ::fast_io::array{u8'<', u8'p', u8'r', u8'e', u8'>'};
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
            auto const start_tag =
                ::fast_io::array{u8'<', u8'b', u8'l', u8'o', u8'c', u8'k', u8'q', u8'u', u8'o', u8't', u8'e', u8'>'};
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
            auto const start_tag =
                ::fast_io::array{u8'<', u8'e', u8'm', u8'>', u8'<', u8's', u8't', u8'r', u8'o', u8'n', u8'g', u8'>'};
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_link: {
            auto a_link = static_cast<::pltxt2htm::MdLink const*>(node.release_imul());
            auto const start_tag = ::fast_io::array{u8'<', u8'a', u8' ', u8'h', u8'r', u8'e', u8'f', u8'=', u8'\"'};
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            result.append(a_link->url_);
            auto const mid_tag = ::fast_io::array{u8'\"', u8'>'};
            result.append(::fast_io::u8string_view(mid_tag.begin(), mid_tag.size()));
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(a_link->get_subast(),
                                                                           ::pltxt2htm::NodeType::md_link, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_escape_backslash: {
            result.push_back(u8'\\');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_exclamation: {
            result.push_back(u8'!');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_hash: {
            result.push_back(u8'#');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_dollar: {
            result.push_back(u8'$');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_percent: {
            result.push_back(u8'%');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_left_paren: {
            result.push_back(u8'(');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_right_paren: {
            result.push_back(u8')');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_asterisk: {
            result.push_back(u8'*');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_plus: {
            result.push_back(u8'+');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_comma: {
            result.push_back(u8',');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_hyphen: {
            result.push_back(u8'-');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_dot: {
            result.push_back(u8'.');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_slash: {
            result.push_back(u8'/');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_colon: {
            result.push_back(u8':');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_semicolon: {
            result.push_back(u8';');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_equals: {
            result.push_back(u8'=');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_question: {
            result.push_back(u8'?');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_at: {
            result.push_back(u8'@');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_left_bracket: {
            result.push_back(u8'[');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_right_bracket: {
            result.push_back(u8']');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_caret: {
            result.push_back(u8'^');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_underscore: {
            result.push_back(u8'_');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_backtick: {
            result.push_back(u8'`');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_left_brace: {
            result.push_back(u8'{');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_pipe: {
            result.push_back(u8'|');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_right_brace: {
            result.push_back(u8'}');
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_tilde: {
            result.push_back(u8'~');
            break;
        }
        case ::pltxt2htm::NodeType::md_code_fence_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_code_fence_tilde: {
            auto code_fence = static_cast<::pltxt2htm::MdCodeFenceBacktick const*>(node.release_imul());
            auto const& opt_language = code_fence->get_language();
            if (opt_language.has_value()) {
                auto const& language = opt_language.template value<ndebug>();
                auto const start_tag = ::fast_io::array{u8'<', u8'p', u8'r', u8'e', u8'>', u8'<', u8'c', u8'o', u8'd',
                                                        u8'e', u8' ', u8'c', u8'l', u8'a', u8's', u8's', u8'=', u8'\"',
                                                        u8'l', u8'a', u8'n', u8'g', u8'u', u8'a', u8'g', u8'e', u8'-'};
                result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
                result.append(language);
                auto const start_tag2 = ::fast_io::array{u8'\"', u8'>'};
                result.append(::fast_io::u8string_view(start_tag2.begin(), start_tag2.size()));
            } else {
                auto const start_tag =
                    ::fast_io::array{u8'<', u8'p', u8'r', u8'e', u8'>', u8'<', u8'c', u8'o', u8'd', u8'e', u8'>'};
                result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            }
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(
                code_fence->get_subast(), ::pltxt2htm::NodeType::md_code_fence_backtick, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeType::base:
#if 0
     [[unlikely]] [[fallthrough]];
     default:
#endif
            [[unlikely]] {
                ::exception::unreachable<ndebug>();
            }
        }
    }

    {
        auto top_frame = ::std::move(call_stack.top());
        call_stack.pop();
        if (call_stack.empty()) {
            return result;
        } else {
            switch (top_frame.nested_tag_type_) {
            case ::pltxt2htm::NodeType::text: {
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_a:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_color: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8's', u8'p', u8'a', u8'n', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_experiment: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'a', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_discussion: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'a', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_user: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8's', u8'p', u8'a', u8'n', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_size: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8's', u8'p', u8'a', u8'n', u8'>'};
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
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8's', u8't', u8'r', u8'o', u8'n', u8'g', u8'>'};
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
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'e', u8'm', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_triple_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8's', u8't', u8'r', u8'o', u8'n',
                                                        u8'g', u8'>', u8'<', u8'/', u8'e', u8'm', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_p: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'p', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h1: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'h', u8'1', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h2: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'h', u8'2', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h3: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'h', u8'3', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h4: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'h', u8'4', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h5: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'h', u8'5', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h6: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'h', u8'6', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_del:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_del: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'd', u8'e', u8'l', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_ul:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_ul: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'u', u8'l', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_ol:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_ol: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'o', u8'l', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_li: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'l', u8'i', u8'>'};
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
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'c', u8'o', u8'd', u8'e', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_pre: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'p', u8'r', u8'e', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_block_quotes:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_blockquote: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'b', u8'l', u8'o', u8'c', u8'k',
                                                        u8'q', u8'u', u8'o', u8't', u8'e', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_code_fence_backtick:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_code_fence_tilde: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'c', u8'o', u8'd', u8'e', u8'>',
                                                        u8'<', u8'/', u8'p', u8'r', u8'e', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_link: {
                auto const end_tag = ::fast_io::array{u8'<', u8'/', u8'a', u8'>'};
                result.append(::fast_io::u8string_view(end_tag.begin(), end_tag.size()));
                goto entry;
            }
            case ::pltxt2htm::NodeType::base:
                [[fallthrough]];
            default:
                [[unlikely]] {
                    ::exception::unreachable<ndebug>();
                }
            }
        }
    }
}

} // namespace pltxt2htm::details
