/**
 * @file basic.hh
 * @brief Basic AST node definitions for pltxt2htm
 * @details Contains the fundamental node types that form the Abstract Syntax Tree
 */

#pragma once

#include <utility>
#include <fast_io/fast_io_dsal/vector.h>

/**
 * @note Quantum-Physics's tag do not case upper / lower
 *       This means that tags like <color>, <Color>, and <COLOR> are treated the same
 */

namespace pltxt2htm {

class PlTxtNode;

// TODO wirte why I don't use fast_io::vector
// suck forward declaration
class Ast {
    ::pltxt2htm::PlTxtNode* begin_ptr{nullptr};
    ::pltxt2htm::PlTxtNode* curr_ptr{nullptr};
    ::std::size_t capacity{};

public:
    constexpr Ast() noexcept = default;

    constexpr Ast(::pltxt2htm::Ast const& other) noexcept = delete;

    constexpr Ast(::pltxt2htm::Ast&& other) noexcept
        : begin_ptr{other.begin_ptr},
          curr_ptr{other.curr_ptr},
          capacity{other.capacity} {
        other.begin_ptr = nullptr;
        other.curr_ptr = nullptr;
        other.capacity = 0;
    }

    constexpr auto operator=(::pltxt2htm::Ast const& other) noexcept = delete;

    constexpr auto operator=(this ::pltxt2htm::Ast& self, ::pltxt2htm::Ast&& other) noexcept -> ::pltxt2htm::Ast& {
        self.swap(other);
        return self;
    }

    constexpr void swap(this ::pltxt2htm::Ast& self, ::pltxt2htm::Ast& other) noexcept {
        auto tmp_begin_ptr = self.begin_ptr;
        auto tmp_curr_ptr = self.curr_ptr;
        auto tmp_capacity = self.capacity;
        self.begin_ptr = other.begin_ptr;
        self.curr_ptr = other.curr_ptr;
        self.capacity = other.capacity;
        other.begin_ptr = tmp_begin_ptr;
        other.curr_ptr = tmp_curr_ptr;
        self.capacity = tmp_capacity;
    }

    constexpr auto empty(this ::pltxt2htm::Ast const& self) noexcept -> bool {
        return self.begin_ptr == self.curr_ptr;
    }

    constexpr auto is_empty(this ::pltxt2htm::Ast const& self) noexcept -> bool {
        return self.begin_ptr == self.curr_ptr;
    }
};

/**
 * @brief UTF-8 character node
 * @details Represents a single UTF-8 character in the AST
 * @note This is a leaf node that contains actual text content
 */
class U8Char {
    char8_t data_;

public:
    constexpr U8Char() noexcept = delete;

    constexpr U8Char(char8_t const& chr) noexcept
        : data_{chr} {
    }

    constexpr U8Char(U8Char const& other) noexcept = delete;
    constexpr U8Char(U8Char&& other) noexcept = default;
    constexpr auto operator=(U8Char const& other) noexcept -> U8Char& = default;
    constexpr auto operator=(U8Char&& other) noexcept -> U8Char& = default;

    /**
     * @brief Get the UTF-8 character stored in this node
     * @param[in] self This U8Char instance
     * @return The UTF-8 character stored in this node
     * @retval char8_t The UTF-8 character data
     */
    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto get_u8char(this U8Char const& self) noexcept {
        return ::std::as_const(self.data_);
    }
};

class InvalidU8Char {
public:
    constexpr InvalidU8Char() noexcept = default;

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
class PairedTagBase {
protected:
    ::pltxt2htm::Ast subast_;

public:
    constexpr PairedTagBase() noexcept = delete;

    /**
     * @brief Construct a paired tag with sub-AST
     * @param[in] subast The sub-AST that represents the content inside this tag
     * @return New PairedTagBase instance
     * @retval PairedTagBase New paired tag base instance with specified properties
     */
    constexpr PairedTagBase(::pltxt2htm::Ast&& subast) noexcept
        : subast_(::std::move(subast)) {
    }

    constexpr PairedTagBase(PairedTagBase const&) noexcept = delete;
    constexpr PairedTagBase(PairedTagBase&&) noexcept = default;
    constexpr PairedTagBase& operator=(PairedTagBase const&) noexcept = delete;
    constexpr PairedTagBase& operator=(PairedTagBase&&) noexcept = default;

    /**
     * @brief Get the sub-AST contained in this paired tag
     * @param[in] self This PairedTagBase instance (non-const version)
     * @return Reference to the sub-AST
     * @retval pltxt2htm::Ast& Reference to the sub-AST (modifiable)
     */
    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto&& get_subast(this PairedTagBase& self) noexcept {
        return self.subast_;
    }

    /**
     * @brief Get the sub-AST contained in this paired tag
     * @param[in] self This PairedTagBase instance (const version)
     * @return Const reference to the sub-AST
     * @retval const pltxt2htm::Ast& Const reference to the sub-AST (non-modifiable)
     */
    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto&& get_subast(this PairedTagBase const& self) noexcept {
        return ::std::as_const(self.subast_);
    }

    /**
     * @brief Get the sub-AST contained in this paired tag (move semantics)
     * @param[in,out] self This PairedTagBase instance (moved from)
     * @return Rvalue reference to the sub-AST
     * @retval pltxt2htm::Ast&& Rvalue reference to the sub-AST (for move construction)
     */
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
     * @param[in] subast The sub-AST representing the text content and inline formatting
     * @return New Text instance
     * @retval Text New text node instance with specified sub-AST content
     */
    constexpr Text(::pltxt2htm::Ast&& subast) noexcept
        : ::pltxt2htm::details::PairedTagBase(::std::move(subast)) {
    }

    constexpr Text(::pltxt2htm::Text const& other) noexcept = delete;

    constexpr Text(::pltxt2htm::Text&& other) noexcept = default;

    constexpr Text& operator=(::pltxt2htm::Text const&) noexcept = delete;
    constexpr Text& operator=(::pltxt2htm::Text&&) noexcept = default;
};

} // namespace pltxt2htm
