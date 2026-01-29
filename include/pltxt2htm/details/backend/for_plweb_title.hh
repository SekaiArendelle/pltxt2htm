#pragma once

#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/array.h>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "exception/exception.hh"
#include "frame_context.hh"
#include "../../details/utils.hh"
#include "../../astnode/basic.hh"
#include "../../astnode/markdown_node.hh"
#include "../../astnode/physics_lab_node.hh"
#include "pltxt2htm/astnode/node_type.hh"

namespace pltxt2htm::details {

/**
 * @brief Translate pl-text's AST to common HTML (only supports color, b and i tags)
 * @details This backend generates simplified HTML output suitable for basic formatting.
 *          It only processes color tags, bold tags, and italic tags, ignoring
 *          more complex formatting like Markdown syntax or advanced HTML elements.
 *          This is typically used for rendering headers or simple text content.
 * @tparam ndebug Debug mode flag - controls assertion behavior
 * @param[in] ast_init The AST to convert to HTML
 * @return A string containing the generated HTML
 */
template<bool ndebug>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto plweb_title_backend(::pltxt2htm::Ast const& ast_init) noexcept -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    ::fast_io::stack<::pltxt2htm::details::BackendBasicFrameContext,
                     ::fast_io::list<::pltxt2htm::details::BackendBasicFrameContext>>
        call_stack{};
    call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(ast_init, ::pltxt2htm::NodeType::base, 0));

entry:
    auto const& ast = call_stack.top().ast_;
    auto&& current_index = call_stack.top().current_index_;
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
        case ::pltxt2htm::NodeType::space: {
            auto const escape_str = ::fast_io::array{u8'&', u8'n', u8'b', u8's', u8'p', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::ampersand: {
            auto const escape_str = ::fast_io::array{u8'&', u8'a', u8'm', u8'p', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::single_quote: {
            auto const escape_str = ::fast_io::array{u8'&', u8'a', u8'p', u8'o', u8's', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::double_quote: {
            auto const escape_str = ::fast_io::array{u8'&', u8'q', u8'u', u8'o', u8't', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::less_than: {
            auto const escape_str = ::fast_io::array{u8'&', u8'l', u8't', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            continue;
        }
        case ::pltxt2htm::NodeType::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::greater_than: {
            auto const escape_str = ::fast_io::array{u8'&', u8'g', u8't', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            continue;
        }
        case ::pltxt2htm::NodeType::tab: {
            auto const escape_str =
                ::fast_io::array{u8'&', u8'n', u8'b', u8's', u8'p', u8';', u8'&', u8'n', u8'b', u8's', u8'p', u8';',
                                 u8'&', u8'n', u8'b', u8's', u8'p', u8';', u8'&', u8'n', u8'b', u8's', u8'p', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            continue;
        }
        case ::pltxt2htm::NodeType::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_br: {
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
            auto const close_tag1 =
                ::fast_io::array{u8'<', u8's', u8'p',  u8'a', u8'n', u8' ', u8's', u8't', u8'y', u8'l',
                                 u8'e', u8'=', u8'\"', u8'c', u8'o', u8'l', u8'o', u8'r', u8':'};
            result.append(::fast_io::u8string_view{close_tag1.data(), close_tag1.size()});
            result.append(color->get_color());
            auto const close_tag2 = ::fast_io::array{u8';', u8'\"', u8'>'};
            result.append(::fast_io::u8string_view{close_tag2.data(), close_tag2.size()});
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
        case ::pltxt2htm::NodeType::pl_macro_project: {
            result.append(u8"{Project}");
            continue;
        }
        case ::pltxt2htm::NodeType::pl_macro_visitor: {
            result.append(u8"{Visitor}");
            continue;
        }
        case ::pltxt2htm::NodeType::pl_macro_author: {
            result.append(u8"{Author}");
            continue;
        }
        case ::pltxt2htm::NodeType::pl_macro_coauthors: {
            result.append(u8"{CoAuthors}");
            continue;
        }
        case ::pltxt2htm::NodeType::md_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_note: {
            continue;
        }
        case ::pltxt2htm::NodeType::pl_external:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::text:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_experiment:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_discussion:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_user:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_size:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_p:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_del:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_ul:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_ol:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_li:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_code:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_latex_inline:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_latex_block:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h1:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h2:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h3:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h4:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h5:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_atx_h6:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_pre:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_blockquote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_block_quotes:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_code_fence_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_code_fence_tilde:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_code_span_1_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_code_span_2_backtick:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::md_code_span_3_backtick: {
            // TODO common_html should not ignore the tag context
            // We should recover the tag context
            auto a_paired_tag = static_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(::pltxt2htm::details::BackendBasicFrameContext(a_paired_tag->get_subast(),
                                                                           ::pltxt2htm::NodeType::base, 0));
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeType::md_link: {
            // TODO common_html should not ignore md_link
            ++current_index;
            goto entry;
        }
        case ::pltxt2htm::NodeType::base:
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
            switch (top_frame.get_nested_tag_type()) {
            case ::pltxt2htm::NodeType::md_double_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_strong:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_b: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8's', u8't', u8'r', u8'o', u8'n', u8'g', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_single_emphasis_asterisk:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_i:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::html_em: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8'e', u8'm', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::md_triple_emphasis_asterisk: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8's', u8't', u8'r', u8'o', u8'n',
                                                        u8'g', u8'>', u8'<', u8'/', u8'e', u8'm', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::pl_a:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_color: {
                auto const close_tag = ::fast_io::array{u8'<', u8'/', u8's', u8'p', u8'a', u8'n', u8'>'};
                result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                goto entry;
            }
            case ::pltxt2htm::NodeType::base: {
                goto entry;
            }
            default:
                [[unlikely]] {
                    ::exception::unreachable<ndebug>();
                }
            }
        }
    }
}

} // namespace pltxt2htm::details
