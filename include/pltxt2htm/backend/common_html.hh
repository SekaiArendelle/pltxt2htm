#pragma once

#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "frame_context.hh"
#include "../utils.hh"
#include "../heap_guard.hh"
#include "../astnode/basic.hh"
#include "../astnode/physics_lab_node.hh"
#include "pltxt2htm/astnode/node_type.hh"

namespace pltxt2htm::details {

/**
 * @brief render in header, only enable color, b and i tag
 */
template<bool ndebug>
constexpr auto ast2common_html(
    ::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>> const& ast_init)
#if __cpp_exceptions < 199711L
    noexcept
#endif
    -> ::fast_io::u8string {
    ::fast_io::u8string result{};
    ::fast_io::stack<::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBasicFrameContext>,
                     ::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBasicFrameContext>>>
        call_stack{};
    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
        ast_init, ::pltxt2htm::NodeType::base, false));

restart:
    auto&& ast = call_stack.top()->ast_;
    for (auto&& node : ast) {
        switch (node->node_type()) {
        case ::pltxt2htm::NodeType::u8char: {
            result.push_back(reinterpret_cast<::pltxt2htm::U8Char const*>(node.release_imul())->get_u8char());
            break;
        }
        case ::pltxt2htm::NodeType::invalid_u8char: {
            result.append(u8"\xEF\xBF\xBD");
            break;
        }
        case ::pltxt2htm::NodeType::space: {
            result.append(u8"&nbsp;");
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::ampersand: {
            result.append(u8"&amp;");
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::single_quote: {
            result.append(u8"&apos;");
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::double_quote: {
            result.append(u8"&quot;");
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::less_than: {
            result.append(u8"&lt;");
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::greater_than: {
            result.append(u8"&gt;");
            break;
        }
        case ::pltxt2htm::NodeType::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_br: {
            break;
        }
        case ::pltxt2htm::NodeType::pl_b: {
            auto b = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            bool const is_not_same_tag = nested_tag_type != ::pltxt2htm::NodeType::pl_b;
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                b->get_subast(), ::pltxt2htm::NodeType::pl_b, is_not_same_tag));
            if (is_not_same_tag) {
                result.append(u8"<b>");
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::pl_i: {
            auto i = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::pl_i};
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                i->get_subast(), ::pltxt2htm::NodeType::pl_i, is_not_same_tag));
            if (is_not_same_tag) {
                char8_t i_start_tag[]{u8'<', u8'i', u8'>'};
                result.append(::fast_io::u8string_view{i_start_tag, sizeof(i_start_tag)});
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_a: {
            // <a> and <color> is the same tag&struct in fact
            auto color = reinterpret_cast<::pltxt2htm::Color const*>(node.release_imul());
            {
                // <color=red><color=blue>text</color></color> can be optimized
                auto&& subast = color->get_subast();
                if (subast.size() == 1) {
                    auto subnode = ::pltxt2htm::details::vector_front<ndebug>(subast).release_imul();
                    if (subnode->node_type() == ::pltxt2htm::NodeType::pl_color) {
                        color = reinterpret_cast<::pltxt2htm::Color const*>(subnode);
                    }
                }
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            // Optimization: If the color is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag =
                (nested_tag_type != ::pltxt2htm::NodeType::pl_color &&
                 nested_tag_type != ::pltxt2htm::NodeType::pl_a) ||
                color->get_color() != reinterpret_cast<::pltxt2htm::details::BackendEqualSignTagContext const*>(
                                          call_stack.top().release_imul())
                                          ->id_;
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendEqualSignTagContext>(
                color->get_subast(), ::pltxt2htm::NodeType::pl_color, is_not_same_tag,
                ::fast_io::mnp::os_c_str(color->get_color())));
            if (is_not_same_tag) {
                result.append(u8"<span style=\"color:");
                result.append(color->get_color());
                result.append(u8";\">");
            }
            goto restart;
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
        case ::pltxt2htm::NodeType::html_hr: {
            break;
        }
        default: {
            auto a_paired_tag = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                a_paired_tag->get_subast(), ::pltxt2htm::NodeType::base, false));
            goto restart;
        }
        }
    }
    while (true) {
        bool is_not_same_tag{call_stack.top()->is_not_same_tag_};
        ::pltxt2htm::NodeType const nested_tag_type{call_stack.top()->nested_tag_type_};
        call_stack.pop();
        if (call_stack.empty()) {
            return result;
        } else {
            switch (nested_tag_type) {
            case ::pltxt2htm::NodeType::pl_b: {
                if (is_not_same_tag) {
                    result.append(u8"</b>");
                }
                continue;
            }
            case ::pltxt2htm::NodeType::pl_i: {
                if (is_not_same_tag) {
                    result.append(u8"</i>");
                }
                continue;
            }
            case ::pltxt2htm::NodeType::pl_a:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_color: {
                if (is_not_same_tag) {
                    result.append(u8"</span>");
                }
                continue;
            }
            case ::pltxt2htm::NodeType::base: {
                continue;
            }
            default:
                ::exception::unreachable<ndebug>();
            }
        }
    }
}

} // namespace pltxt2htm::details
