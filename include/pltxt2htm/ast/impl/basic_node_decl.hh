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
class LineBreak {
public:
    constexpr auto operator==(this LineBreak const&, LineBreak const&) noexcept -> bool = default;
};

/**
 * @brief ::pltxt2htm::Space character node
 * @details Represents a whitespace character (space, tab, etc.).
 */
class Space {
public:
    constexpr auto operator==(this Space const&, Space const&) noexcept -> bool = default;
};

/**
 * @brief Less-than sign node
 * @details Represents the '<' character, which may need escaping in HTML.
 */
class LessThan {
public:
    constexpr auto operator==(this LessThan const&, LessThan const&) noexcept -> bool = default;
};

/**
 * @brief Greater-than sign node
 * @details Represents the '>' character.
 */
class GreaterThan {
public:
    constexpr auto operator==(this GreaterThan const&, GreaterThan const&) noexcept -> bool = default;
};

/**
 * @brief ::pltxt2htm::Tab character node
 * @details Represents a tab character in the source text.
 */
class Tab {
public:
    constexpr auto operator==(this Tab const&, Tab const&) noexcept -> bool = default;
};

/**
 * @brief ::pltxt2htm::Ampersand node
 * @details Represents the '&' character, which may be part of an HTML entity.
 */
class Ampersand {
public:
    constexpr auto operator==(this Ampersand const&, Ampersand const&) noexcept -> bool = default;
};

/**
 * @brief Single quotation mark node
 * @details Represents the "'" character.
 */
class SingleQuote {
public:
    constexpr auto operator==(this SingleQuote const&, SingleQuote const&) noexcept -> bool = default;
};

/**
 * @brief Double quotation mark node
 * @details Represents the '"' character.
 */
class DoubleQuote {
public:
    constexpr auto operator==(this DoubleQuote const&, DoubleQuote const&) noexcept -> bool = default;
};

/**
 * @brief UTF-8 character node
 * @details Represents a single UTF-8 character in the AST. This is a leaf node.
 */
class U8Char {
public:
    char8_t chr;
    constexpr auto operator==(this U8Char const&, U8Char const&) noexcept -> bool = default;
};

/**
 * @brief Invalid UTF-8 character node
 * @details Represents an invalid UTF-8 character encountered during parsing.
 */
class InvalidU8Char {
public:
    constexpr auto operator==(this InvalidU8Char const&, InvalidU8Char const&) noexcept -> bool = default;
};

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
    constexpr Text(::pltxt2htm::Text<ndebug> const&) noexcept;
    constexpr Text(::pltxt2htm::Text<ndebug>&&) noexcept;
    constexpr ~Text() noexcept;
    constexpr auto operator=(::pltxt2htm::Text<ndebug> const&) noexcept -> ::pltxt2htm::Text<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::Text<ndebug>& self, ::pltxt2htm::Text<ndebug>&&) noexcept
        -> ::pltxt2htm::Text<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this Text const&, Text const&) noexcept -> bool;

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
    constexpr Url(::pltxt2htm::Url<ndebug> const&) noexcept;
    constexpr Url(::pltxt2htm::Url<ndebug>&&) noexcept;
    constexpr ~Url() noexcept;
    constexpr auto operator=(::pltxt2htm::Url<ndebug> const&) noexcept -> ::pltxt2htm::Url<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::Url<ndebug>& self, ::pltxt2htm::Url<ndebug>&&) noexcept
        -> ::pltxt2htm::Url<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this Url const&, Url const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_url_ast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.url_ast);
    }
};

} // namespace pltxt2htm
