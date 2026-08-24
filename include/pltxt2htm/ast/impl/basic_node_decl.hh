/**
 * @file basic_node_decl.hh
 * @brief Basic AST node declarations for pltxt2htm
 * @details Defines fundamental node types: character nodes, text container, and URL.
 */

#pragma once

#include <utility>
#include <fast_io/fast_io_dsal/string.h>
#include "../code/ast.hh"
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
 * @brief ::pltxt2htm::EntityReference node
 * @details Represents an HTML entity reference like &amp;quot;, &amp;amp;, &amp;#38;.
 *          Stores the entity content between &amp; and ; (e.g. &amp;quot; stores "quot").
 *          The backend outputs it as &amp; + value + ; verbatim.
 */
class EntityReference {
    ::fast_io::u8string value;

public:
    constexpr EntityReference(::fast_io::u8string&& value_) noexcept
        : value(::std::move(value_)) {
    }

    constexpr EntityReference(::pltxt2htm::EntityReference const&) noexcept = default;
    constexpr EntityReference(::pltxt2htm::EntityReference&&) noexcept = default;
    constexpr ~EntityReference() noexcept = default;
    constexpr auto operator=(::pltxt2htm::EntityReference const&) noexcept -> ::pltxt2htm::EntityReference& = delete;
    constexpr auto operator=(this EntityReference& self, ::pltxt2htm::EntityReference&&) noexcept
        -> ::pltxt2htm::EntityReference& = default;

    [[nodiscard]]
    constexpr auto operator==(this EntityReference const&, EntityReference const&) noexcept -> bool = default;

    [[nodiscard]]
    constexpr auto get_value(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.value);
    }
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
    constexpr auto operator=(this Text<ndebug>& self, ::pltxt2htm::Text<ndebug>&&) noexcept
        -> ::pltxt2htm::Text<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this Text const&, Text const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown fenced code block
 * @details Contains a language-specific code AST. Syntax nodes retain their
 *          source-language meaning until a backend maps them to presentation.
 */
template<::pltxt2htm::Contracts ndebug>
class CodeFence {
    ::pltxt2htm::CodeAst<ndebug> ast;

public:
    /**
     * @brief Construct a fenced code block.
     * @param ast_value The parsed code content.
     */
    constexpr explicit CodeFence(::pltxt2htm::CodeAst<ndebug>&& ast_value) noexcept;
    constexpr CodeFence(::pltxt2htm::CodeFence<ndebug> const&) noexcept;
    constexpr CodeFence(::pltxt2htm::CodeFence<ndebug>&&) noexcept;
    constexpr ~CodeFence() noexcept;
    constexpr auto operator=(::pltxt2htm::CodeFence<ndebug> const&) noexcept
        -> ::pltxt2htm::CodeFence<ndebug>& = delete;
    constexpr auto operator=(this CodeFence<ndebug>& self, ::pltxt2htm::CodeFence<ndebug>&&) noexcept
        -> ::pltxt2htm::CodeFence<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this CodeFence const& self, CodeFence const& other) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_ast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.ast);
    }
};

/**
 * @brief URL node
 * @details Represents a semantic URL value stored independently of any backend's escaping syntax.
 */
class Url {
    ::fast_io::u8string url_str;

public:
    /**
     * @brief Construct a ::pltxt2htm::Url from a semantic URL string.
     * @param url The URL string without HTML attribute escaping.
     */
    constexpr explicit Url(::fast_io::u8string&& url) noexcept
        : url_str(::std::move(url)) {
    }

    constexpr Url(Url const&) noexcept = default;
    constexpr Url(Url&&) noexcept = default;
    constexpr ~Url() noexcept = default;
    constexpr auto operator=(Url const&) noexcept -> Url& = delete;
    constexpr auto operator=(this Url& self, Url&&) noexcept -> Url& = default;

    [[nodiscard]]
    constexpr auto operator==(this Url const&, Url const&) noexcept -> bool = default;

    [[nodiscard]]
    constexpr auto as_string(this Url const& self) noexcept -> ::fast_io::u8string const& {
        return self.url_str;
    }
};

} // namespace pltxt2htm
