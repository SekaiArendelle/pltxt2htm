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
#include "../../heap_guard.hh"
#include "../../details/utils.hh"
#include "../../astnode/basic.hh"
#include "../../astnode/node_type.hh"
#include "../../astnode/markdown_node.hh"
#include "../../astnode/physics_lab_node.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

class OlFrameContext : public ::pltxt2htm::details::BackendBasicFrameContext {
public:
    ::std::size_t ol_li_count{1};

    constexpr OlFrameContext(::pltxt2htm::Ast const& ast) noexcept
        : BackendBasicFrameContext(ast, ::pltxt2htm::NodeType::html_ol, 0) {
    }

    constexpr OlFrameContext(::pltxt2htm::details::OlFrameContext const&) noexcept = default;
    constexpr OlFrameContext(::pltxt2htm::details::OlFrameContext&&) noexcept = default;
    constexpr ~OlFrameContext() noexcept = default;
    constexpr auto operator=(::pltxt2htm::details::OlFrameContext const&) noexcept
        -> ::pltxt2htm::details::OlFrameContext& = delete;
    constexpr auto operator=(::pltxt2htm::details::OlFrameContext&&) noexcept
        -> ::pltxt2htm::details::OlFrameContext& = delete;
};

template<bool ndebug>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto plunity_text_backend(::pltxt2htm::Ast const& ast_init, ::fast_io::u8string_view project,
                                    ::fast_io::u8string_view visitor, ::fast_io::u8string_view author,
                                    ::fast_io::u8string_view coauthors) noexcept -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    ::fast_io::stack<::pltxt2htm::HeapGuard<::pltxt2htm::details::BackendBasicFrameContext>,
                     ::fast_io::list<::pltxt2htm::HeapGuard<::pltxt2htm::details::BackendBasicFrameContext>>>
        call_stack{};
    call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(ast_init, ::pltxt2htm::NodeType::base, 0));

entry:
    auto const& ast = call_stack.top()->ast_;
    // auto const& nested_tag_type = call_stack.top().nested_tag_type_;
    auto&& current_index = call_stack.top()->current_index_;
    for (; current_index < ast.size(); ++current_index) {
        auto&& node = ::pltxt2htm::details::vector_index<ndebug>(ast, current_index);

        switch (node->node_type()) {
        case ::pltxt2htm::NodeType::u8char: {
            result.push_back(static_cast<::pltxt2htm::U8Char const*>(node.release_imul())->get_u8char());
            continue;
        }
        case ::pltxt2htm::NodeType::invalid_u8char: {
            auto const escape_str = ::fast_io::array{char8_t{0xef}, 0xbf, 0xbd};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
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
            result.push_back(u8' ');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::ampersand: {
            result.push_back(u8'&');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::single_quote: {
            result.push_back(u8'\'');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::double_quote: {
            result.push_back(u8'\"');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::less_than: {
            result.push_back(u8'<');
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::greater_than: {
            result.push_back(u8'>');
            continue;
        }
        case ::pltxt2htm::NodeType::tab: {
            result.push_back(u8'\t');
            continue;
        }
        case ::pltxt2htm::NodeType::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_a: {
            // <a> and <color> is the same tag&struct in fact
            auto color = static_cast<::pltxt2htm::Color const*>(node.release_imul());

            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(color->get_subast(),
                                                                           ::pltxt2htm::NodeType::pl_color, 0));
            ++current_index;
            result.append(u8"<color=");
            result.append(color->get_color());
            result.push_back(u8'>');
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_experiment: {
            auto experiment = static_cast<::pltxt2htm::Experiment const*>(node.release_imul());
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(experiment->get_subast(),
                                                                           ::pltxt2htm::NodeType::pl_experiment, 0));
            ++current_index;
            result.append(u8"<experiment=");
            result.append(experiment->get_id());
            result.append(u8">");
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_discussion: {
            auto discussion = static_cast<::pltxt2htm::Discussion const*>(node.release_imul());
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(discussion->get_subast(),
                                                                           ::pltxt2htm::NodeType::pl_discussion, 0));
            ++current_index;
            result.append(u8"<discussion=");
            result.append(discussion->get_id());
            result.append(u8">");
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_external: {
            auto external = static_cast<::pltxt2htm::External const*>(node.release_imul());
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(external->get_subast(),
                                                                           ::pltxt2htm::NodeType::pl_external, 0));
            ++current_index;
            result.append(u8"<external=");
            result.append(external->get_url());
            result.append(u8">");
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_user: {
            auto user = static_cast<::pltxt2htm::User const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(user->get_subast(), ::pltxt2htm::NodeType::pl_user, 0));
            ++current_index;
            result.append(u8"<user=");
            result.append(user->get_id());
            result.push_back(u8'>');
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_size: {
            auto size = static_cast<::pltxt2htm::Size const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(size->get_subast(), ::pltxt2htm::NodeType::pl_size, 0));
            ++current_index;
            result.append(u8"<size=");
            result.append(::pltxt2htm::details::size_t2str(size->get_id()));
            result.push_back(u8'>');
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
            result.append(u8"<b>");
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
        case ::pltxt2htm::NodeType::html_br:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::line_break: {
            result.push_back('\n');
            continue;
        }
        case ::pltxt2htm::NodeType::html_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h1: {
            auto h1 = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(h1->get_subast(), ::pltxt2htm::NodeType::html_h1, 0));
            ++current_index;
            result.append(u8"<size=38><b>");
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h2: {
            auto h2 = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(h2->get_subast(), ::pltxt2htm::NodeType::html_h2, 0));
            ++current_index;
            result.append(u8"<size=37><b>");
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h3: {
            auto h3 = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(h3->get_subast(), ::pltxt2htm::NodeType::html_h3, 0));
            ++current_index;
            result.append(u8"<size=36><b>");
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h4: {
            auto h4 = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(h4->get_subast(), ::pltxt2htm::NodeType::html_h4, 0));
            ++current_index;
            result.append(u8"<size=35><b>");
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h5: {
            auto h5 = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(h5->get_subast(), ::pltxt2htm::NodeType::html_h5, 0));
            ++current_index;
            result.append(u8"<b>");
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h6: {
            auto h6 = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(h6->get_subast(), ::pltxt2htm::NodeType::html_h6, 0));
            ++current_index;
            result.append(u8"<b>");
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
            auto const start_tag = ::fast_io::array{u8'<', u8'i', u8'>'};
            result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_hr: {
            auto const start_tag = ::fast_io::array{u8'<', u8'h', u8'r', u8'>'};
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
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_ol: {
            auto ol = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::HeapGuard<::pltxt2htm::details::OlFrameContext>(ol->get_subast()));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeType::html_li: {
            auto const nested_tag_type = call_stack.top()->get_nested_tag_type();
            pltxt2htm_assert(
                nested_tag_type == ::pltxt2htm::NodeType::html_ol || nested_tag_type == ::pltxt2htm::NodeType::html_ul,
                u8"Invalid tag type");
            [[fallthrough]];
        }
        case ::pltxt2htm::NodeType::md_li: {
            auto li = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(
                ::pltxt2htm::details::BackendBasicFrameContext(li->get_subast(), ::pltxt2htm::NodeType::html_li, 0));
            ++current_index;
            ::std::size_t indent_level{};
            for (auto const& frame : call_stack.container) {
                if (frame->get_nested_tag_type() == ::pltxt2htm::NodeType::html_ol ||
                    frame->get_nested_tag_type() == ::pltxt2htm::NodeType::html_ul ||
                    frame->get_nested_tag_type() == ::pltxt2htm::NodeType::md_ol ||
                    frame->get_nested_tag_type() == ::pltxt2htm::NodeType::md_ul) {
                    if (indent_level > 0) {
                        result.append(u8"  ");
                    }
                    ++indent_level;
                }
            }
            auto reverse_iter = call_stack.container.crbegin();
            ::pltxt2htm::details::BackendBasicFrameContext* the_second_to_last_frame{(++reverse_iter)->get_unsafe()};
            auto const nested_tag_type = the_second_to_last_frame->get_nested_tag_type();
            if (nested_tag_type == ::pltxt2htm::NodeType::html_ol || nested_tag_type == ::pltxt2htm::NodeType::md_ol) {
                ::std::size_t& ol_li_count = static_cast<::pltxt2htm::details::OlFrameContext *>(
                                                 the_second_to_last_frame)
                                               ->ol_li_count;
                result.append(::pltxt2htm::details::size_t2str(ol_li_count));
                result.append(u8". ");
                ++ol_li_count;
            }
            else if (nested_tag_type == ::pltxt2htm::NodeType::html_ul ||
                     nested_tag_type == ::pltxt2htm::NodeType::md_ul) {
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
                    ::exception::unreachable<ndebug>();
                }
            }
            else {
                ::exception::unreachable<ndebug>();
            }
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
            result.append(u8"<b><i>");
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_link: {
            auto a_link = static_cast<::pltxt2htm::MdLink const*>(node.release_imul());
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(a_link->get_subast(),
                                                                           ::pltxt2htm::NodeType::md_link, 0));
            ++current_index;
            result.append(u8"<external=");
            result.append(a_link->url_);
            result.push_back(u8'>');
            goto entry;
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
                auto const& language = opt_language.template value<ndebug>();
                auto const start_tag = ::fast_io::array{u8'<', u8'p', u8'r', u8'e', u8'>', u8'<', u8'c', u8'o', u8'd',
                                                        u8'e', u8' ', u8'c', u8'l', u8'a', u8's', u8's', u8'=', u8'\"',
                                                        u8'l', u8'a', u8'n', u8'g', u8'u', u8'a', u8'g', u8'e', u8'-'};
                result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
                result.append(language);
                auto const start_tag2 = ::fast_io::array{u8'\"', u8'>'};
                result.append(::fast_io::u8string_view(start_tag2.begin(), start_tag2.size()));
            }
            else {
                auto const start_tag =
                    ::fast_io::array{u8'<', u8'p', u8'r', u8'e', u8'>', u8'<', u8'c', u8'o', u8'd', u8'e', u8'>'};
                result.append(::fast_io::u8string_view(start_tag.begin(), start_tag.size()));
            }
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(
                code_fence->get_subast(), ::pltxt2htm::NodeType::md_code_fence_backtick, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeType::pl_macro_project: {
            result.append(project);
            continue;
        }
        case ::pltxt2htm::NodeType::pl_macro_visitor: {
            result.append(visitor);
            continue;
        }
        case ::pltxt2htm::NodeType::pl_macro_author: {
            result.append(author);
            continue;
        }
        case ::pltxt2htm::NodeType::pl_macro_coauthors: {
            result.append(coauthors);
            continue;
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
        }
        else {
            switch (top_frame->get_nested_tag_type()) {
            case ::pltxt2htm::NodeType::text: {
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_a:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_color: {
                result.append(u8"</color>");
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_experiment: {
                result.append(u8"</experiment>");
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_discussion: {
                result.append(u8"</discussion>");
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_external: {
                result.append(u8"</external>");
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_user: {
                result.append(u8"</user>");
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_size: {
                result.append(u8"</size>");
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_double_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_double_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_b:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_strong: {
                result.append(u8"</b>");
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_single_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_single_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_i:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_em: {
                result.append(u8"</i>");
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_triple_emphasis_underscore:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk: {
                result.append(u8"</i></b>");
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_p: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'p', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h1: {
                result.append(u8"</b></size>");
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h2: {
                result.append(u8"</b></size>");
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h3: {
                result.append(u8"</b></size>");
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h4: {
                result.append(u8"</b></size>");
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h5: {
                result.append(u8"</b>");
                goto entry;
            }
            case ::pltxt2htm::NodeType::html_h6: {
                result.append(u8"</b>");
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
            case ::pltxt2htm::NodeType::html_ul:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::md_ol:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_ol: {
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_li:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_li: {
                result.push_back(u8'\n');
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
            case ::pltxt2htm::NodeType::md_latex_inline: {
                result.append(u8"$");
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_latex_block: {
                result.append(u8"$$");
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
                result.append(u8"</external>");
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

#include "../pop_macro.hh"
