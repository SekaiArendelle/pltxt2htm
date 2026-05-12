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
public:
    char8_t chr;
};

/**
 * @brief Invalid UTF-8 character node
 * @details Represents an invalid UTF-8 character encountered during parsing.
 */
class InvalidU8Char {};

/**
 * @brief ::pltxt2htm::ast2::Text<ndebug> container node
 * @details Holds a sub-AST representing text content and inline formatting.
 */
template<::pltxt2htm::Contracts ndebug>
class Text {
    ::pltxt2htm::ast2::Ast<ndebug> subast;

public:
    /**
     * @brief Construct a ::pltxt2htm::ast2::Text<ndebug> node with a sub-AST.
     * @param subast The sub-AST to be contained.
     */
    constexpr Text(::pltxt2htm::ast2::Ast<ndebug>&& subast) noexcept;
    constexpr Text(::pltxt2htm::ast2::Text<ndebug> const&) noexcept = delete;
    constexpr Text(::pltxt2htm::ast2::Text<ndebug>&&) noexcept;
    constexpr ~Text() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::Text<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::Text<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::Text<ndebug>&&) noexcept -> ::pltxt2htm::ast2::Text<ndebug>&;

    [[nodiscard]]
    constexpr auto&& get_subast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief URL node
 * @details Represents a URL stored as an AST of characters.
 */
template<::pltxt2htm::Contracts ndebug>
class Url {
    ::pltxt2htm::ast2::Ast<ndebug> url_ast{};

public:
    constexpr Url() noexcept;
    /**
     * @brief Construct a ::pltxt2htm::ast2::Url<ndebug> from an AST.
     * @param attr The AST representing the URL characters.
     */
    constexpr explicit Url(::pltxt2htm::ast2::Ast<ndebug>&& attr) noexcept;
    constexpr Url(::pltxt2htm::ast2::Url<ndebug> const&) noexcept = delete;
    constexpr Url(::pltxt2htm::ast2::Url<ndebug>&&) noexcept;
    constexpr ~Url() noexcept;
    constexpr auto operator=(::pltxt2htm::ast2::Url<ndebug> const&) noexcept
        -> ::pltxt2htm::ast2::Url<ndebug>& = delete;
    constexpr auto operator=(::pltxt2htm::ast2::Url<ndebug>&&) noexcept -> ::pltxt2htm::ast2::Url<ndebug>&;

    /**
     * @brief Get the underlying URL AST.
     * @return Reference to the URL AST (modifiable if this is an lvalue).
     */
    [[nodiscard]]
    constexpr auto&& get_url_ast(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.url_ast);
    }
};

} // namespace pltxt2htm::ast2
