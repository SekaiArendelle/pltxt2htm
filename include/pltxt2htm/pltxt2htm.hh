#pragma once

#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "astnode/basic.hh"
#include "astnode/enum.hh"
#include "heap_guard.hh"
#include "parser.hh"

namespace pltxt2htm {

namespace details {

/**
 * @brief Integrate nodes to HTML.
 * @tparam T: should not be marked const
 * @param tokens The tokens parsed from pl-text.
 */
template<bool ndebug, typename T>
    requires (::std::same_as<::std::remove_reference_t<T>, ::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>>>)
[[nodiscard]]
    constexpr auto integrate_ast(T&& ast, bool const is_inline) noexcept {
    ::fast_io::u8string result{};

    for (auto&& node : ast) {
        switch (node->node_type()) {
        case ::pltxt2htm::NodeType::u8char: {
            result.push_back(reinterpret_cast<::pltxt2htm::U8Char*>(node.release())->get_u8char());
            break;
        }
        case ::pltxt2htm::NodeType::space: {
            result.append(u8"&nbsp;");
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
        case ::pltxt2htm::NodeType::color: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::a: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::experiment: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::discussion: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::user: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::size: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::b: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::i: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::p: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::br: {
            if (!is_inline) {
                result.append(u8"<br>");
            }
            break;
        }
        case ::pltxt2htm::NodeType::h: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::img: {
            //
            break;
        }
        case ::pltxt2htm::NodeType::latex: {
            //
            break;
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

    return result;
}

} // namespace details

/**
 * @brief Convert Quantum Physics (aka. Physics-Lab, pl) text to HTML.
 *        Supported syntax are listed in pltxt2htm/astnode.hh: `enum class NodeType`
 * @tparam ndebug: show explanation in README.md Q/A
 * @param pltext The text of Quantum Physics.
 * @param is_inline Whether ignore <size> and line break.
 */
template<bool ndebug = false>
[[nodiscard]]
constexpr auto pltxt2html(::fast_io::u8string_view pltext, bool is_inline = false) noexcept {
    return ::pltxt2htm::details::integrate_ast<ndebug>(::pltxt2htm::parse_pltxt<ndebug>(pltext), is_inline);
}

} // namespace pltxt2htm
