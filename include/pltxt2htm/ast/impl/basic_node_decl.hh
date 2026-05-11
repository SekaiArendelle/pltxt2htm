/**
 * @file basic_node_decl.hh
 * @brief Basic AST node declarations for pltxt2htm
 * @details Defines fundamental node types: character nodes, text container, and URL.
 */

#pragma once

#include <utility>
#include "ast_decl.hh"

namespace pltxt2htm::ast2 {

/**
 * @brief UTF-8 character node
 * @details Represents a single UTF-8 character in the AST. This is a leaf node.
 */
class U8Char {
    char8_t chr;
};

/**
 * @brief Invalid UTF-8 character node
 * @details Represents an invalid UTF-8 character encountered during parsing.
 */
class InvalidU8Char {};

/**
 * @brief Text container node
 * @details Holds a sub-AST representing text content and inline formatting.
 */
class Text {
    ::pltxt2htm::ast2::Ast subast;

public:
    /**
     * @brief Construct a Text node with a sub-AST.
     * @param subast The sub-AST to be contained.
     */
    constexpr Text(::pltxt2htm::ast2::Ast&& subast) noexcept;
    constexpr Text(::pltxt2htm::ast2::Text const&) noexcept = delete;
    constexpr Text(::pltxt2htm::ast2::Text&&) noexcept;
    constexpr ~Text() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::Text const&) noexcept -> ::pltxt2htm::ast2::Text& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::Text&&) noexcept -> ::pltxt2htm::ast2::Text&;
};

/**
 * @brief URL node
 * @details Represents a URL stored as an AST of characters.
 */
class Url {
    ::pltxt2htm::ast2::Ast url_ast_{};

public:
    constexpr Url() noexcept;
    /**
     * @brief Construct a Url from an AST.
     * @param attr The AST representing the URL characters.
     */
    constexpr explicit Url(::pltxt2htm::ast2::Ast&& attr) noexcept;
    constexpr Url(Url const&) noexcept = delete;
    constexpr Url(Url&&) noexcept;
    constexpr ~Url() noexcept;
    constexpr auto operator=(Url const&) noexcept -> Url& = delete;
    constexpr auto operator=(Url&&) noexcept -> Url&;

    /**
     * @brief Get the underlying URL AST.
     * @return Reference to the URL AST (modifiable if this is an lvalue).
     */
    [[nodiscard]]
    constexpr auto&& get_url_ast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.url_ast_);
    }
};

} // namespace pltxt2htm::ast2
