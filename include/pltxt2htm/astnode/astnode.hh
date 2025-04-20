#pragma once

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "enum.hh"
#include "basic.hh"
#include "plext.hh"
#include "html.hh"
#include "markdown.hh"
#include "exception/exception.hh"

namespace pltxt2htm {

/**
 * @brief Whether a type is pltxt node
 */
template<typename T>
concept is_pl_txt_node = ::std::is_base_of_v<PlTxtNode, T>;

/**
 * @brief Down cast a pltxt node to its real type
 * @tparam Func type, just let compiler infer it
 * @param node pltxt node
 * @param func
 * @example ```cpp
 *     down_cast(node, []<is_pl_txt_node Node>(Node&& node) static {
 *         if constexpr (::std::is_same_v<Node, Text>) {
 *             // do something
 *         }
 *         // ...
 *     })
 * ```
 */
template<typename Func, bool ndebug = false>
constexpr void down_cast(::pltxt2htm::PlTxtNode const& node, Func&& func) noexcept {
    switch (node.node_type()) {
    case ::pltxt2htm::NodeType::base:
        [[unlikely]] {
            ::exception::unreachable<ndebug>();
        }
    case ::pltxt2htm::NodeType::text: {
        func(static_cast<::pltxt2htm::Text const&>(node));
        break;
    }
    case ::pltxt2htm::NodeType::color: {
        func(static_cast<::pltxt2htm::Color const&>(node));
        break;
    }
    case ::pltxt2htm::NodeType::a: {
        func(static_cast<::pltxt2htm::A const&>(node));
        break;
    }
    case ::pltxt2htm::NodeType::experiment: {
        func(static_cast<::pltxt2htm::Experiment const&>(node));
        break;
    }
    case ::pltxt2htm::NodeType::discussion: {
        func(static_cast<::pltxt2htm::Discussion const&>(node));
        break;
    }
        // default:
        //     [[unlikely]] {
        //         ::exception::unreachable<ndebug>();
        //     }
    }
}

} // namespace pltxt2htm
