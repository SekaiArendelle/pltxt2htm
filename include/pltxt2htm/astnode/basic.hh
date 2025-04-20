#pragma once

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "enum.hh"

/**
 * @note Quantum-Physics's tag do not case upper / lower
 */

namespace pltxt2htm {

/**
 * @brief Base node of other nodes
 */
class PlTxtNode
#if __cpp_trivial_relocatability >= 202502L
    // https://en.cppreference.com/w/cpp/language/class_property_specifiers#Trivial_relocatability
    replaceable_if_eligible trivially_relocatable_if_eligible
#endif
{
protected:
    ::pltxt2htm::NodeType runtime_node_type_info = ::pltxt2htm::NodeType::base;

public:
    constexpr PlTxtNode() noexcept = default;

    constexpr PlTxtNode(::pltxt2htm::NodeType type) noexcept
        : runtime_node_type_info{type} {
    }

    constexpr PlTxtNode(PlTxtNode const&) noexcept = default;
    constexpr PlTxtNode(PlTxtNode&&) noexcept = default;
    constexpr auto operator=(PlTxtNode const&) noexcept -> PlTxtNode& = default;
    constexpr auto operator=(PlTxtNode&&) noexcept -> PlTxtNode& = default;
    constexpr ~PlTxtNode() noexcept = default;

    constexpr auto node_type(this PlTxtNode const& self) noexcept {
        return self.runtime_node_type_info;
    }
};

/**
 * @brief Text node
 */
class Text : public ::pltxt2htm::PlTxtNode {
    ::fast_io::u8string text;

public:
    constexpr Text() noexcept = delete;

    constexpr Text(::fast_io::u8string_view str) noexcept
        : PlTxtNode{NodeType::text},
          text{str} {
    }
};

} // namespace pltxt2htm
