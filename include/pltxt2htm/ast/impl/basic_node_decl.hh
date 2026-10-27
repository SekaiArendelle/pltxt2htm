/**
 * @file basic_node_decl.hh
 * @brief Basic AST node declarations for pltxt2htm
 * @details Defines character nodes and basic AST containers.
 */

#pragma once

#include <utility>

#include "../code/ast.hh"
#include "ast_decl.hh"
#include "../../details/push_macro.hh"

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
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr Group(::pltxt2htm::Group const&) noexcept = delete;
#else
    constexpr Group(::pltxt2htm::Group const&) noexcept = default;
#endif
    constexpr Group(::pltxt2htm::Group&&) noexcept;
    constexpr ~Group() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::Group const&) noexcept -> ::pltxt2htm::Group& = delete;
#else
    constexpr auto operator=(this Group& self, ::pltxt2htm::Group const&) noexcept -> ::pltxt2htm::Group& = default;
#endif
    constexpr auto operator=(this Group& self, ::pltxt2htm::Group&&) noexcept -> ::pltxt2htm::Group&;

    [[nodiscard]]
    constexpr auto operator==(this Group const&, Group const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Markdown fenced code block
 * @details Contains either language-neutral highlighting IR or explicitly
 *          rendered inline code.
 */
enum class CodeFenceKind : unsigned {
    highlighted = 0,
    rendered,
};

class CodeFence {
    ::pltxt2htm::CodeFenceKind kind;

    union {
        ::pltxt2htm::HighlightedCodeAst highlighted_ast;
        ::pltxt2htm::RenderedCodeAst rendered_ast;
    };

    constexpr void destroy_active(this CodeFence& self) noexcept;

public:
    /**
     * @brief Construct a fenced code block.
     * @param ast_value The parsed code content.
     */
    constexpr explicit CodeFence(::pltxt2htm::HighlightedCodeAst&& ast_value) noexcept;
    constexpr explicit CodeFence(::pltxt2htm::RenderedCodeAst&& ast_value) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr CodeFence(::pltxt2htm::CodeFence const&) noexcept = delete;
#else
    constexpr CodeFence(::pltxt2htm::CodeFence const&) noexcept;
#endif
    constexpr CodeFence(::pltxt2htm::CodeFence&&) noexcept;
    constexpr ~CodeFence() noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(this CodeFence& self, ::pltxt2htm::CodeFence const&) noexcept
        -> ::pltxt2htm::CodeFence& = delete;
#else
    constexpr auto operator=(this CodeFence& self, ::pltxt2htm::CodeFence const& other) noexcept
        -> ::pltxt2htm::CodeFence&;
#endif
    constexpr auto operator=(this CodeFence& self, ::pltxt2htm::CodeFence&&) noexcept -> ::pltxt2htm::CodeFence&;

    [[nodiscard]]
    constexpr auto operator==(this CodeFence const& self, CodeFence const& other) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_kind(this CodeFence const& self) noexcept -> ::pltxt2htm::CodeFenceKind {
        return self.kind;
    }

    [[nodiscard]]
    constexpr auto get_highlighted_ast(this CodeFence& self) noexcept -> ::pltxt2htm::HighlightedCodeAst& {
        pltxt2htm_assert(self.kind == ::pltxt2htm::CodeFenceKind::highlighted,
                         u8"highlighted AST requested from rendered code fence");
        return self.highlighted_ast;
    }

    [[nodiscard]]
    constexpr auto get_highlighted_ast(this CodeFence const& self) noexcept -> ::pltxt2htm::HighlightedCodeAst const& {
        pltxt2htm_assert(self.kind == ::pltxt2htm::CodeFenceKind::highlighted,
                         u8"highlighted AST requested from rendered code fence");
        return self.highlighted_ast;
    }

    [[nodiscard]]
    constexpr auto get_rendered_ast(this CodeFence& self) noexcept -> ::pltxt2htm::RenderedCodeAst& {
        pltxt2htm_assert(self.kind == ::pltxt2htm::CodeFenceKind::rendered,
                         u8"rendered AST requested from highlighted code fence");
        return self.rendered_ast;
    }

    [[nodiscard]]
    constexpr auto get_rendered_ast(this CodeFence const& self) noexcept -> ::pltxt2htm::RenderedCodeAst const& {
        pltxt2htm_assert(self.kind == ::pltxt2htm::CodeFenceKind::rendered,
                         u8"rendered AST requested from highlighted code fence");
        return self.rendered_ast;
    }
};

} // namespace pltxt2htm

#include "../../details/pop_macro.hh"
