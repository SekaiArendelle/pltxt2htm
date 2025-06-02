#pragma once

#include "enum.hh"

/**
 * @note Quantum-Physics's tag do not case upper / lower
 */

namespace pltxt2htm {

/**
 * @brief Base node of other nodes
 */
class PlTxtNode
// #if __cpp_trivial_relocatability >= 202502L
//     // https://en.cppreference.com/w/cpp/language/class_property_specifiers#Trivial_relocatability
//     replaceable_if_eligible trivially_relocatable_if_eligible
// #endif
{
protected:
    ::pltxt2htm::NodeType runtime_node_type_info{::pltxt2htm::NodeType::base};

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

    [[nodiscard]]
    constexpr auto node_type(this PlTxtNode const& self) noexcept {
        return self.runtime_node_type_info;
    }
};

/**
 * @brief UTF-8 char/string node
 */
class U8Char : public ::pltxt2htm::PlTxtNode {
    char8_t data_;

public:
    constexpr U8Char() noexcept = delete;

    constexpr U8Char(char8_t const& chr) noexcept
        : PlTxtNode{NodeType::u8char},
          data_{chr} {
    }

    constexpr U8Char(U8Char const& other) noexcept = default;
    constexpr U8Char(U8Char&& other) noexcept = default;
    constexpr auto operator=(U8Char const& other) noexcept -> U8Char& = default;
    constexpr auto operator=(U8Char&& other) noexcept -> U8Char& = default;

    [[nodiscard]]
    constexpr auto get_u8char(this U8Char const& self) noexcept {
        return self.data_;
    }
};

} // namespace pltxt2htm
