#pragma once

#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/array.h>
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
 * @brief Translate pl-text's ast to common html(only enable color, b and i tag).
 *        usually be used to render header
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
                     ::fast_io::list<::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBasicFrameContext>>>
        call_stack{};
    call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
        ast_init, ::pltxt2htm::NodeType::base, false, 0));

restart:
    auto&& ast = call_stack.top()->ast_;
    auto&& current_index = call_stack.top()->current_index_;
    for (; current_index < ast.size(); ++current_index) {
        auto&& node = ::pltxt2htm::details::vector_index<ndebug>(ast, current_index);

        switch (node->node_type()) {
        case ::pltxt2htm::NodeType::u8char: {
            result.push_back(reinterpret_cast<::pltxt2htm::U8Char const*>(node.release_imul())->get_u8char());
            break;
        }
        case ::pltxt2htm::NodeType::invalid_u8char: {
            auto escape_str = ::fast_io::array{char8_t{0xef}, 0xbf, 0xbd};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::space: {
            auto escape_str = ::fast_io::array{u8'&', u8'n', u8'b', u8's', u8'p', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_ampersand:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::ampersand: {
            auto escape_str = ::fast_io::array{u8'&', u8'a', u8'm', u8'p', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_single_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::single_quote: {
            auto escape_str = ::fast_io::array{u8'&', u8'a', u8'p', u8'o', u8's', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_double_quote:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::double_quote: {
            auto escape_str = ::fast_io::array{u8'&', u8'q', u8'u', u8'o', u8't', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_less_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::less_than: {
            auto escape_str = ::fast_io::array{u8'&', u8'l', u8't', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::md_escape_greater_than:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::greater_than: {
            auto escape_str = ::fast_io::array{u8'&', u8'g', u8't', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::tab: {
            auto escape_str =
                ::fast_io::array{u8'&', u8'n', u8'b', u8's', u8'p', u8';', u8'&', u8'n', u8'b', u8's', u8'p', u8';',
                                 u8'&', u8'n', u8'b', u8's', u8'p', u8';', u8'&', u8'n', u8'b', u8's', u8'p', u8';'};
            result.append(::fast_io::u8string_view{escape_str.data(), escape_str.size()});
            break;
        }
        case ::pltxt2htm::NodeType::line_break:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_br: {
            break;
        }
        case ::pltxt2htm::NodeType::pl_b: {
            auto b = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            if (b->get_subast().empty()) {
                // Optimization: if the tag is empty, we can skip it
                break;
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            bool const is_not_same_tag = nested_tag_type != ::pltxt2htm::NodeType::pl_b;
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                b->get_subast(), ::pltxt2htm::NodeType::pl_b, is_not_same_tag, 0));
            ++current_index;
            if (is_not_same_tag) {
                auto start_tag = ::fast_io::array{u8'<', u8'b', u8'>'};
                result.append(::fast_io::u8string_view{start_tag.data(), start_tag.size()});
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::pl_i: {
            auto i = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            if (i->get_subast().empty()) {
                // Optimization: if the tag is empty, we can skip it
                break;
            }
            auto&& nested_tag_type = call_stack.top()->nested_tag_type_;
            bool const is_not_same_tag{nested_tag_type != ::pltxt2htm::NodeType::pl_i};
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                i->get_subast(), ::pltxt2htm::NodeType::pl_i, is_not_same_tag, 0));
            ++current_index;
            if (is_not_same_tag) {
                auto start_tag = ::fast_io::array{u8'<', u8'e', u8'm', u8'>'};
                result.append(::fast_io::u8string_view{start_tag.data(), start_tag.size()});
            }
            goto restart;
        }
        case ::pltxt2htm::NodeType::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_a: {
            // <a> and <color> is the same tag&struct in fact
            auto color = reinterpret_cast<::pltxt2htm::Color const*>(node.release_imul());
            if (color->get_subast().empty()) {
                // Optimization: if the tag is empty, we can skip it
                break;
            }
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
                color->get_subast(), ::pltxt2htm::NodeType::pl_color, is_not_same_tag, 0,
                ::fast_io::mnp::os_c_str(color->get_color())));
            ++current_index;
            if (is_not_same_tag) {
                auto close_tag1 =
                    ::fast_io::array{u8'<', u8's', u8'p',  u8'a', u8'n', u8' ', u8's', u8't', u8'y', u8'l',
                                     u8'e', u8'=', u8'\"', u8'c', u8'o', u8'l', u8'o', u8'r', u8':'};
                result.append(::fast_io::u8string_view{close_tag1.data(), close_tag1.size()});
                result.append(color->get_color());
                auto close_tag2 = ::fast_io::array{u8';', u8'\"', u8'>'};
                result.append(::fast_io::u8string_view{close_tag2.data(), close_tag2.size()});
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
        case ::pltxt2htm::NodeType::html_hr:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::html_note: {
            break;
        }
        default: {
            auto a_paired_tag = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            if (a_paired_tag->get_subast().empty()) {
                // Optimization: if the tag is empty, we can skip it
                break;
            }
            call_stack.push(::pltxt2htm::details::HeapGuard<::pltxt2htm::details::BackendBareTagContext>(
                a_paired_tag->get_subast(), ::pltxt2htm::NodeType::base, false, 0));
            ++current_index;
            goto restart;
        }
        }
    }
    {
        bool is_not_same_tag{call_stack.top()->is_not_same_tag_};
        ::pltxt2htm::NodeType const nested_tag_type{call_stack.top()->nested_tag_type_};
        call_stack.pop();
        if (call_stack.empty()) {
            return result;
        } else {
            switch (nested_tag_type) {
            case ::pltxt2htm::NodeType::pl_b: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8'b', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::pl_i: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8'e', u8'm', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::pl_a:
                [[fallthrough]];
            case ::pltxt2htm::NodeType::pl_color: {
                if (is_not_same_tag) {
                    auto close_tag = ::fast_io::array{u8'<', u8'/', u8's', u8'p', u8'a', u8'n', u8'>'};
                    result.append(::fast_io::u8string_view{close_tag.data(), close_tag.size()});
                }
                goto restart;
            }
            case ::pltxt2htm::NodeType::base: {
                goto restart;
            }
            default:
                [[unlikely]] ::exception::unreachable<ndebug>();
            }
        }
    }
}

} // namespace pltxt2htm::details
