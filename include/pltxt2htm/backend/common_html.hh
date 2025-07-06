#pragma once

#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../heap_guard.hh"
#include "../astnode/basic.hh"
#include "../astnode/physics_lab_node.hh"

namespace pltxt2htm::details {

/**
 * @brief render in header, only enable color, b and i tag
 */
template<bool ndebug>
constexpr auto ast2common_html(::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>> const& ast,
                               ::pltxt2htm::PlTxtNode const* const extern_node = nullptr)
#if __cpp_exceptions < 199711L
    noexcept
#endif
    -> ::fast_io::u8string {
    ::fast_io::u8string result{};

    for (auto&& node : ast) {
        switch (node->node_type()) {
        case ::pltxt2htm::NodeType::u8char: {
            result.push_back(reinterpret_cast<::pltxt2htm::U8Char const*>(node.release_imul())->get_u8char());
            break;
        }
        case ::pltxt2htm::NodeType::space: {
            result.append(u8"&nbsp;");
            break;
        }
        case ::pltxt2htm::NodeType::ampersand: {
            result.append(u8"&amp;");
            break;
        }
        case ::pltxt2htm::NodeType::single_quote: {
            result.append(u8"&apos;");
            break;
        }
        case ::pltxt2htm::NodeType::double_quote: {
            result.append(u8"&quot;");
            break;
        }
        case ::pltxt2htm::NodeType::less_than: {
            result.append(u8"&lt;");
            break;
        }
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
            bool const is_not_same_tag =
                extern_node == nullptr || extern_node->node_type() != ::pltxt2htm::NodeType::pl_b;
            if (is_not_same_tag) {
                result.append(u8"<b>");
            }
            result.append(::pltxt2htm::details::ast2common_html<ndebug>(b->get_subast(), b));
            if (is_not_same_tag) {
                result.append(u8"</b>");
            }
            break;
        }
        case ::pltxt2htm::NodeType::pl_i: {
            auto i = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            bool const is_not_same_tag =
                extern_node == nullptr || extern_node->node_type() != ::pltxt2htm::NodeType::pl_i;
            if (is_not_same_tag) {
                result.append(u8"<i>");
            }
            result.append(::pltxt2htm::details::ast2common_html<ndebug>(i->get_subast(), i));
            if (is_not_same_tag) {
                result.append(u8"</i>");
            }
            break;
        }
        case ::pltxt2htm::NodeType::pl_color:
            [[fallthrough]];
        case ::pltxt2htm::NodeType::pl_a: {
            // <a> and <color> is the same tag&struct in fact
            auto color = reinterpret_cast<::pltxt2htm::Color const*>(node.release_imul());
            auto&& subast = color->get_subast();
            if (subast.size() == 1) {
                // <color=red><color=blue>text</color></color> can be optimized
                auto subnode = subast.index_unchecked(0).release_imul();
                if (subnode->node_type() == ::pltxt2htm::NodeType::pl_color) {
                    color = reinterpret_cast<::pltxt2htm::Color const*>(subnode);
                }
            }
            // Optimization: If the color is the same as the parent node, then ignore the nested tag.
            bool const is_not_same_tag =
                extern_node == nullptr ||
                (extern_node->node_type() != ::pltxt2htm::NodeType::pl_color &&
                 extern_node->node_type() != ::pltxt2htm::NodeType::pl_a) ||
                color->get_color() != reinterpret_cast<::pltxt2htm::Color const*>(extern_node)->get_color();
            if (is_not_same_tag) {
                result.append(u8"<span style=\"color:");
                result.append(color->get_color());
                result.append(u8";\">");
            }
            result.append(::pltxt2htm::details::ast2common_html<ndebug>(subast, color));
            if (is_not_same_tag) {
                result.append(u8"</span>");
            }
            break;
        }
        default: {
            auto a_paired_tag = reinterpret_cast<::pltxt2htm::details::PairedTagBase const*>(node.release_imul());
            result.append(::pltxt2htm::details::ast2common_html<ndebug>(a_paired_tag->get_subast(), a_paired_tag));
            break;
        }
        }
    }
    return result;
}

} // namespace pltxt2htm::details
