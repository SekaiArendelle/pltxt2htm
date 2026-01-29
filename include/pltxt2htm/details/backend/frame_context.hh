/**
 * @file frame_context.hh
 * @brief Backend frame context for HTML generation
 * @details Defines the context structure used by the HTML backend to track
 *          the current state during AST to HTML conversion
 */

#pragma once

#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../../astnode/basic.hh"

namespace pltxt2htm::details {

/**
 * @brief Basic frame context for HTML backend processing
 * @details This context keeps track of the current AST being processed,
 *          the type of nested tag, and the current index position
 * @note This is used during the recursive AST traversal for HTML generation
 */
class BackendBasicFrameContext {
public:
    ::pltxt2htm::Ast const& ast_; ///< Reference to the AST being processed
private:
    ::pltxt2htm::NodeType nested_tag_type_; ///< Type of the current nested tag context
public:
    // TODO How about store iterator?
    ::std::size_t current_index_; ///< Current index position in the AST

    constexpr BackendBasicFrameContext(::pltxt2htm::Ast const& ast, ::pltxt2htm::NodeType const nested_tag_type,
                             ::std::size_t current_index) noexcept
        : ast_(ast),
          nested_tag_type_{nested_tag_type},
          current_index_{current_index} {
    }

    constexpr BackendBasicFrameContext(::pltxt2htm::details::BackendBasicFrameContext const&) noexcept = default;
    constexpr BackendBasicFrameContext(::pltxt2htm::details::BackendBasicFrameContext&&) noexcept = default;

    constexpr ~BackendBasicFrameContext() noexcept = default;

    // const reference do not support operator=
    constexpr ::pltxt2htm::details::BackendBasicFrameContext& operator=(
        ::pltxt2htm::details::BackendBasicFrameContext const&) noexcept = delete;
    constexpr ::pltxt2htm::details::BackendBasicFrameContext& operator=(
        ::pltxt2htm::details::BackendBasicFrameContext&&) noexcept = delete;

    constexpr auto get_nested_tag_type(this BackendBasicFrameContext const& self) noexcept {
        return self.nested_tag_type_;
    }
};

} // namespace pltxt2htm::details
