/**
 * @file basic.hh
 * @brief Basic AST node definitions for pltxt2htm
 * @brief Contains the fundamental node types that form the Abstract Syntax Tree
 */

#pragma once

#include <utility>
#include <fast_io/fast_io_dsal/vector.h>
#include "node_type.hh"
#include "../heap_guard.hh"

/**
 * @note Quantum-Physics's tag do not case upper / lower
 *       This means that tags like <color>, <Color>, and <COLOR> are treated the same
 */

namespace pltxt2htm {

/**
 * @brief Base node class for all AST nodes
 * @details This is the fundamental base class for all nodes in the Abstract Syntax Tree.
 *          Every AST node inherits from this class and has a NodeType that identifies
 *          what kind of node it is.
 * @note The copy constructor is deleted to prevent accidental copying which could be expensive
 */
class PlTxtNode {
protected:
    ::pltxt2htm::NodeType runtime_node_type_info{::pltxt2htm::NodeType::base};

public:
    constexpr PlTxtNode() noexcept = default;

    constexpr PlTxtNode(::pltxt2htm::NodeType type) noexcept
        : runtime_node_type_info{type} {
    }

    constexpr PlTxtNode(PlTxtNode const&) noexcept = delete; // copy construct costs
    constexpr PlTxtNode(PlTxtNode&&) noexcept = default;
    constexpr auto operator=(PlTxtNode const&) noexcept -> PlTxtNode& = default;
    constexpr auto operator=(PlTxtNode&&) noexcept -> PlTxtNode& = default;
    constexpr ~PlTxtNode() noexcept = default;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto node_type(this PlTxtNode const& self) noexcept {
        return self.runtime_node_type_info;
    }
};

using Ast = ::fast_io::vector<::pltxt2htm::HeapGuard<::pltxt2htm::PlTxtNode>>;

/**
 * @brief UTF-8 character node
 * @details Represents a single UTF-8 character in the AST
 * @note This is a leaf node that contains actual text content
 */
class U8Char : public ::pltxt2htm::PlTxtNode {
    char8_t data_;

public:
    constexpr U8Char() noexcept = delete;

    constexpr U8Char(char8_t const& chr) noexcept
        : PlTxtNode{::pltxt2htm::NodeType::u8char},
          data_{chr} {
    }

    constexpr U8Char(U8Char const& other) noexcept = delete;
    constexpr U8Char(U8Char&& other) noexcept = default;
    constexpr auto operator=(U8Char const& other) noexcept -> U8Char& = default;
    constexpr auto operator=(U8Char&& other) noexcept -> U8Char& = default;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto get_u8char(this U8Char const& self) noexcept {
        return ::std::as_const(self.data_);
    }
};

class InvalidU8Char : public ::pltxt2htm::PlTxtNode {
public:
    constexpr InvalidU8Char() noexcept
        : PlTxtNode(::pltxt2htm::NodeType::invalid_u8char) {
    }

    constexpr InvalidU8Char(InvalidU8Char const& other) noexcept = delete;
    constexpr InvalidU8Char(InvalidU8Char&& other) noexcept = default;
    constexpr InvalidU8Char& operator=(InvalidU8Char const&) noexcept = delete;
    constexpr InvalidU8Char& operator=(InvalidU8Char&& other) noexcept = default;
};

namespace details {

/**
 * @brief Base class for paired HTML-like tags
 * @details This class serves as a base for nodes that have opening and closing tags
 *          and contain sub-content (sub-AST). Examples include <color>, <b>, <i>, etc.
 * @note The copy constructor and assignment operator are deleted to prevent expensive copies
 */
class PairedTagBase : public ::pltxt2htm::PlTxtNode {
protected:
    ::pltxt2htm::Ast subast_;

public:
    constexpr PairedTagBase() noexcept = delete;

    /**
     * @brief Construct a paired tag with specified node type and sub-AST
     * @param node_type The type of this node (from NodeType enum)
     * @param subast The sub-AST that represents the content inside this tag
     */
    constexpr PairedTagBase(::pltxt2htm::NodeType node_type, ::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::PlTxtNode{node_type},
          subast_(::std::move(subast)) {
    }

    constexpr PairedTagBase(PairedTagBase const&) noexcept = delete;
    constexpr PairedTagBase(PairedTagBase&&) noexcept = default;
    constexpr PairedTagBase& operator=(PairedTagBase const&) noexcept = delete;
    constexpr PairedTagBase& operator=(PairedTagBase&&) noexcept = default;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto&& get_subast(this PairedTagBase& self) noexcept {
        return self.subast_;
    }

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto&& get_subast(this PairedTagBase const& self) noexcept {
        return ::std::as_const(self.subast_);
    }

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto&& get_subast(this PairedTagBase&& self) noexcept {
        return ::std::move(self.subast_);
    }
};

} // namespace details

/**
 * @brief Text node - container for text content
 * @details This node represents a text container that can hold various sub-nodes
 *          including characters, formatting tags, and other inline elements
 * @note Text nodes are fundamental building blocks that group together
 *       characters and inline formatting
 */
class Text : public ::pltxt2htm::details::PairedTagBase {
public:
    constexpr Text() noexcept = delete;

    /**
     * @brief Construct a text node with sub-AST content
     * @param subast The sub-AST representing the text content and inline formatting
     */
    constexpr Text(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase(::pltxt2htm::NodeType::text, ::std::move(subast)) {
    }

    constexpr Text(::pltxt2htm::Text const& other) noexcept = delete;

    constexpr Text(::pltxt2htm::Text&& other) noexcept = default;

    constexpr Text& operator=(::pltxt2htm::Text const&) noexcept = delete;
    constexpr Text& operator=(::pltxt2htm::Text&&) noexcept = default;
};

} // namespace pltxt2htm
