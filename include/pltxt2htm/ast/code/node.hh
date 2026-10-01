/**
 * @file node.hh
 * @brief Backend-independent fenced-code IR nodes.
 */

#pragma once

#include <cstddef>
#include "../../contracts.hh"

namespace pltxt2htm {

/**
 * @brief Semantic highlighting roles emitted by language-specific parsers.
 * @details These roles intentionally do not retain the source language. A code
 *          fragment only needs enough information for pltxt2htm's backends to
 *          render it consistently.
 */
enum class CodeHighlightKind : unsigned {
    plain = 0,
    keyword,
    string,
    number,
    comment,
    function,
    macro,
    preprocessor,
};

enum class RenderedCodeNodeKind : unsigned {
    text = 0,
    entity_reference,
    style_begin,
    style_end,
};

class CodeNode {
    ::std::size_t begin{};
    ::std::size_t end{};
    ::pltxt2htm::CodeHighlightKind kind{};

    template<::pltxt2htm::Contracts>
    friend class HighlightedCodeAst;

    constexpr CodeNode(::std::size_t const begin_value, ::std::size_t const end_value,
                       ::pltxt2htm::CodeHighlightKind const kind_value) noexcept
        : begin(begin_value),
          end(end_value),
          kind(kind_value) {
    }

public:
    [[nodiscard]]
    constexpr auto get_begin(this CodeNode const& self) noexcept -> ::std::size_t {
        return self.begin;
    }

    [[nodiscard]]
    constexpr auto get_end(this CodeNode const& self) noexcept -> ::std::size_t {
        return self.end;
    }

    [[nodiscard]]
    constexpr auto get_kind(this CodeNode const& self) noexcept -> ::pltxt2htm::CodeHighlightKind {
        return self.kind;
    }

    [[nodiscard]]
    constexpr auto operator==(this CodeNode const&, CodeNode const&) noexcept -> bool = default;
};

class RenderedCodeNode {
    ::std::size_t begin{};
    ::std::size_t end{};
    ::std::size_t metadata{};
    ::pltxt2htm::RenderedCodeNodeKind kind{};

    template<::pltxt2htm::Contracts>
    friend class RenderedCodeAst;

    constexpr RenderedCodeNode(::std::size_t const begin_value, ::std::size_t const end_value,
                               ::std::size_t const metadata_value,
                               ::pltxt2htm::RenderedCodeNodeKind const kind_value) noexcept
        : begin(begin_value),
          end(end_value),
          metadata(metadata_value),
          kind(kind_value) {
    }

public:
    [[nodiscard]]
    constexpr auto get_begin(this RenderedCodeNode const& self) noexcept -> ::std::size_t {
        return self.begin;
    }

    [[nodiscard]]
    constexpr auto get_end(this RenderedCodeNode const& self) noexcept -> ::std::size_t {
        return self.end;
    }

    [[nodiscard]]
    constexpr auto get_kind(this RenderedCodeNode const& self) noexcept -> ::pltxt2htm::RenderedCodeNodeKind {
        return self.kind;
    }

    [[nodiscard]]
    constexpr auto operator==(this RenderedCodeNode const&, RenderedCodeNode const&) noexcept -> bool = default;
};

} // namespace pltxt2htm
