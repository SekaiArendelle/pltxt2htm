#pragma once

#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../astnode/basic.hh"

namespace pltxt2htm::details {

class BackendBasicFrameContext {
public:
    ::pltxt2htm::Ast const& ast_;
    ::pltxt2htm::NodeType const nested_tag_type_;
    // TODO How about store iterator?
    ::std::size_t current_index_;

    BackendBasicFrameContext(::pltxt2htm::Ast const& ast, ::pltxt2htm::NodeType const nested_tag_type,
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
};

} // namespace pltxt2htm::details
