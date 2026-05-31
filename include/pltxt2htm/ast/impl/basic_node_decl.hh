/**
 * @file basic_node_decl.hh
 * @brief Basic AST node declarations for pltxt2htm
 * @details Defines fundamental node types: character nodes, text container, and URL.
 */

#pragma once

#include <utility>
#include "ast_decl.hh"

namespace pltxt2htm {

/**
 * @brief Line break (newline) node
 * @details Represents a line break character in the source text.
 */
class LineBreak {};

/**
 * @brief ::pltxt2htm::Space character node
 * @details Represents a whitespace character (space, tab, etc.).
 */
class Space {};

/**
 * @brief Less-than sign node
 * @details Represents the '<' character, which may need escaping in HTML.
 */
class LessThan {};

/**
 * @brief Greater-than sign node
 * @details Represents the '>' character.
 */
class GreaterThan {};

/**
 * @brief ::pltxt2htm::Tab character node
 * @details Represents a tab character in the source text.
 */
class Tab {};

/**
 * @brief ::pltxt2htm::Ampersand node
 * @details Represents the '&' character, which may be part of an HTML entity.
 */
class Ampersand {};

/**
 * @brief Single quotation mark node
 * @details Represents the "'" character.
 */
class SingleQuotationMark {};

/**
 * @brief Double quotation mark node
 * @details Represents the '"' character.
 */
class DoubleQuotationMark {};

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
 * @brief ::pltxt2htm::Text<ndebug> container node
 * @details Holds a sub-AST representing text content and inline formatting.
 */
template<::pltxt2htm::Contracts ndebug>
class Text {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    /**
     * @brief Construct a ::pltxt2htm::Text<ndebug> node with a sub-AST.
     * @param subast The sub-AST to be contained.
     */
    constexpr Text(::pltxt2htm::Ast<ndebug>&& subast) noexcept;
    constexpr Text(::pltxt2htm::Text<ndebug> const&) noexcept = delete;
    constexpr Text(::pltxt2htm::Text<ndebug>&&) noexcept;
    constexpr ~Text() noexcept;
    constexpr auto operator=(::pltxt2htm::Text<ndebug> const&) noexcept -> ::pltxt2htm::Text<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::Text<ndebug>& self, ::pltxt2htm::Text<ndebug>&&) noexcept
        -> ::pltxt2htm::Text<ndebug>&;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief URL node
 * @details Represents a URL stored as an AST of characters.
 */
template<::pltxt2htm::Contracts ndebug>
class Url {
    ::pltxt2htm::Ast<ndebug> url_ast{};

public:
    /**
     * @brief Construct a ::pltxt2htm::Url<ndebug> from an AST.
     * @param attr The AST representing the URL characters.
     */
    constexpr explicit Url(::pltxt2htm::Ast<ndebug>&& attr) noexcept;
    constexpr Url(::pltxt2htm::Url<ndebug> const&) noexcept = delete;
    constexpr Url(::pltxt2htm::Url<ndebug>&&) noexcept;
    constexpr ~Url() noexcept;
    constexpr auto operator=(::pltxt2htm::Url<ndebug> const&) noexcept -> ::pltxt2htm::Url<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::Url<ndebug>& self, ::pltxt2htm::Url<ndebug>&&) noexcept
        -> ::pltxt2htm::Url<ndebug>&;

    /**
     * @brief Get the underlying URL AST.
     * @return Reference to the URL AST (modifiable if this is an lvalue).
     */
    [[nodiscard]]
    constexpr auto get_url_ast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.url_ast);
    }
};

} // namespace pltxt2htm
