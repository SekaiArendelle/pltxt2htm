/**
 * @file basic_node_decl.hh
 * @brief Basic AST node declarations for pltxt2htm
 * @details Defines character nodes and basic AST containers.
 */

#pragma once

#include <utility>
#include "../../container/string.hh"
#include "../../container/optional.hh"
#include "ast_decl.hh"

namespace pltxt2htm {

/**
 * @brief Line break (newline) node
 * @details Represents a line break character in the source text.
 */
class LineBreak {
public:
    [[nodiscard]]
    constexpr auto operator==(this LineBreak const&, LineBreak const&) noexcept -> bool = default;
};

/**
 * @brief ::pltxt2htm::Space character node
 * @details Represents a whitespace character (space, tab, etc.).
 */
class Space {
public:
    [[nodiscard]]
    constexpr auto operator==(this Space const&, Space const&) noexcept -> bool = default;
};

/**
 * @brief Less-than sign node
 * @details Represents the '<' character, which may need escaping in HTML.
 */
class LessThan {
public:
    [[nodiscard]]
    constexpr auto operator==(this LessThan const&, LessThan const&) noexcept -> bool = default;
};

/**
 * @brief Greater-than sign node
 * @details Represents the '>' character.
 */
class GreaterThan {
public:
    [[nodiscard]]
    constexpr auto operator==(this GreaterThan const&, GreaterThan const&) noexcept -> bool = default;
};

/**
 * @brief ::pltxt2htm::Tab character node
 * @details Represents a tab character in the source text.
 */
class Tab {
public:
    [[nodiscard]]
    constexpr auto operator==(this Tab const&, Tab const&) noexcept -> bool = default;
};

/**
 * @brief ::pltxt2htm::Ampersand node
 * @details Represents the semantic '&' character.
 */
class Ampersand {
public:
    [[nodiscard]]
    constexpr auto operator==(this Ampersand const&, Ampersand const&) noexcept -> bool = default;
};

/**
 * @brief Single quotation mark node
 * @details Represents the "'" character.
 */
class SingleQuote {
public:
    [[nodiscard]]
    constexpr auto operator==(this SingleQuote const&, SingleQuote const&) noexcept -> bool = default;
};

/**
 * @brief Double quotation mark node
 * @details Represents the '"' character.
 */
class DoubleQuote {
public:
    [[nodiscard]]
    constexpr auto operator==(this DoubleQuote const&, DoubleQuote const&) noexcept -> bool = default;
};

/**
 * @brief Invalid UTF-8 character node
 * @details Represents an invalid UTF-8 character encountered during parsing.
 */
class InvalidUtf8 {
public:
    [[nodiscard]]
    constexpr auto operator==(this InvalidUtf8 const&, InvalidUtf8 const&) noexcept -> bool = default;
};

/**
 * @brief ::pltxt2htm::Group container node
 * @details Holds a sub-AST representing text content and inline formatting.
 */
class Group {
    ::pltxt2htm::Ast subast;

public:
    /**
     * @brief Construct a ::pltxt2htm::Group node with a sub-AST.
     * @param subast The sub-AST to be contained.
     */
    constexpr Group(::pltxt2htm::Ast&& subast) noexcept;
    constexpr Group(::pltxt2htm::Group const&) noexcept = default;
    constexpr Group(::pltxt2htm::Group&&) noexcept;
    constexpr ~Group() noexcept = default;
    constexpr auto operator=(::pltxt2htm::Group const&) noexcept -> ::pltxt2htm::Group& = default;
    constexpr auto operator=(this Group& self, ::pltxt2htm::Group&&) noexcept
        -> ::pltxt2htm::Group&;

    [[nodiscard]]
    constexpr auto operator==(this Group const&, Group const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown fenced code block
 * @details Contains code content and an optional language identifier.
 */
class CodeFence {
    ::pltxt2htm::Ast subast;
    ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String> lang;

public:
    /**
     * @brief Construct a fenced code block.
     * @param subast The code content as an AST.
     * @param lang Optional language string.
     */
    constexpr explicit CodeFence(::pltxt2htm::Ast&& subast_,
                                 ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String>&& lang_) noexcept;
    constexpr CodeFence(::pltxt2htm::CodeFence const&) noexcept = default;
    constexpr CodeFence(::pltxt2htm::CodeFence&&) noexcept;
    constexpr ~CodeFence() noexcept = default;
    constexpr auto operator=(::pltxt2htm::CodeFence const&) noexcept
        -> ::pltxt2htm::CodeFence& = default;
    constexpr auto operator=(this CodeFence& self, ::pltxt2htm::CodeFence&&) noexcept
        -> ::pltxt2htm::CodeFence&;

    [[nodiscard]]
    constexpr auto operator==(this CodeFence const& self, CodeFence const& other) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_language(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.lang);
    }
};

} // namespace pltxt2htm
