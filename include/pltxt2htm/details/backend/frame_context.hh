/**
 * @file frame_context.hh
 * @brief Backend frame context for HTML generation
 * @details Defines the context structure used by the HTML backend to track
 *          the current state during AST to HTML conversion
 */

#pragma once

#include <cstddef>
#include <utility>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../../astnode/basic.hh"
#include "../../astnode/node_type.hh"
#include "../../contracts.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

class BackendContextWithoutInfo {};

class BackendContextWithOlInfo {
public:
    ::std::size_t ol_li_count{1};
};

class BackendContextVariant {
public:
    union {
        ::pltxt2htm::details::BackendContextWithoutInfo without_info;
        ::pltxt2htm::details::BackendContextWithOlInfo ol_info;
    };

    ::pltxt2htm::NodeType kind;

    constexpr BackendContextVariant(::pltxt2htm::NodeType const kind_) noexcept
        : without_info{},
          kind{kind_} {
    }

    constexpr BackendContextVariant(::pltxt2htm::details::BackendContextWithOlInfo&& ol_info_context) noexcept
        : ol_info{::std::move(ol_info_context)},
          kind{::pltxt2htm::NodeType::html_ol} {
    }

    constexpr ~BackendContextVariant() noexcept = default;
};

/**
 * @brief Basic frame context for HTML backend processing
 * @details This context keeps track of the current AST being processed,
 *          the type of nested tag, and the current index position
 * @note This is used during the recursive AST traversal for HTML generation
 */
template<::pltxt2htm::Contracts ndebug>
class BackendFrameContext {
    ::pltxt2htm::details::BackendContextVariant context_data;
    /* [[nonnull]] */ ::pltxt2htm::Ast const* ast_; ///< Reference to the AST being processed

public:
    ::std::size_t current_index_; ///< Current index position in the AST

    /**
     * @note construct ast from reference to avoid nullptr issue
     */
    constexpr BackendFrameContext(::pltxt2htm::Ast const& ast, ::pltxt2htm::NodeType const nested_tag_type,
                                  ::std::size_t current_index) noexcept
        : context_data{nested_tag_type},
          ast_(::std::addressof(ast)),
          current_index_{current_index} {
    }

    constexpr BackendFrameContext(::pltxt2htm::Ast const& ast, ::std::size_t current_index,
                                  ::pltxt2htm::details::BackendContextWithOlInfo&& ol_info_context) noexcept
        : context_data{::std::move(ol_info_context)},
          ast_(::std::addressof(ast)),
          current_index_{current_index} {
    }

    constexpr BackendFrameContext(::pltxt2htm::details::BackendFrameContext<ndebug> const&) noexcept = default;
    constexpr BackendFrameContext(::pltxt2htm::details::BackendFrameContext<ndebug>&&) noexcept = default;

    constexpr ~BackendFrameContext() noexcept = default;

    // const reference do not support operator=
    constexpr ::pltxt2htm::details::BackendFrameContext<ndebug>& operator=(
        ::pltxt2htm::details::BackendFrameContext<ndebug> const&) noexcept = delete;
    constexpr ::pltxt2htm::details::BackendFrameContext<ndebug>& operator=(
        ::pltxt2htm::details::BackendFrameContext<ndebug>&&) noexcept = delete;

    constexpr auto get_nested_tag_type(this ::pltxt2htm::details::BackendFrameContext<ndebug> const& self) noexcept {
        return self.context_data.kind;
    }

    constexpr auto get_ast(this ::pltxt2htm::details::BackendFrameContext<ndebug> const& self) noexcept
        -> ::pltxt2htm::Ast const& {
        return *(self.ast_);
    }

    constexpr auto get_ol_li_count(this auto&& self) noexcept -> ::std::size_t& {
        bool const is_ol_type{self.context_data.kind == ::pltxt2htm::NodeType::html_ol ||
                              self.context_data.kind == ::pltxt2htm::NodeType::md_ol};
        pltxt2htm_assert(is_ol_type, u8"context kind mismatch");
        return self.context_data.ol_info.ol_li_count;
    }
};

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
