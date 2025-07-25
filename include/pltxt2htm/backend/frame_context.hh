#pragma once

#include <fast_io/fast_io_dsal/vector.h>
#include "../heap_guard.hh"
#include "../astnode/basic.hh"

namespace pltxt2htm::details {

class BackendBasicFrameContext {
public:
    ::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>> const& ast_;
    ::pltxt2htm::NodeType const nested_tag_type_;
    bool is_not_same_tag_;

    BackendBasicFrameContext(::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>> const& ast,
                             ::pltxt2htm::NodeType const nested_tag_type, bool is_not_same_tag)
        : ast_(ast),
          nested_tag_type_{nested_tag_type},
          is_not_same_tag_{is_not_same_tag} {
    }

    constexpr BackendBasicFrameContext(::pltxt2htm::details::BackendBasicFrameContext const&) noexcept = default;
    constexpr BackendBasicFrameContext(::pltxt2htm::details::BackendBasicFrameContext&&) noexcept = default;

    constexpr virtual ~BackendBasicFrameContext() noexcept = default;

    // const reference do not support operator=
    constexpr ::pltxt2htm::details::BackendBasicFrameContext& operator=(
        ::pltxt2htm::details::BackendBasicFrameContext const&) noexcept = delete;
    constexpr ::pltxt2htm::details::BackendBasicFrameContext& operator=(
        ::pltxt2htm::details::BackendBasicFrameContext&&) noexcept = delete;
};

// class BackendBareTagContext : public ::pltxt2htm::details::BackendBasicFrameContext {
// public:
//     BackendBareTagContext(::fast_io::vector<::pltxt2htm::details::HeapGuard<::pltxt2htm::PlTxtNode>> const& ast,
//                           ::pltxt2htm::NodeType const nested_tag_type, bool is_not_same_tag)
//         : ::pltxt2htm::details::BackendBasicFrameContext(ast, nested_tag_type, is_not_same_tag) {
//     }

//     constexpr BackendBareTagContext(::pltxt2htm::details::BackendBareTagContext const&) noexcept = default;
//     constexpr BackendBareTagContext(::pltxt2htm::details::BackendBareTagContext&&) noexcept = default;

//     constexpr ~BackendBareTagContext() noexcept = default;

//     // const reference do not support operator=
//     constexpr ::pltxt2htm::details::BackendBareTagContext& operator=(
//         ::pltxt2htm::details::BackendBareTagContext const&) noexcept = delete;
//     constexpr ::pltxt2htm::details::BackendBareTagContext& operator=(
//         ::pltxt2htm::details::BackendBareTagContext&&) noexcept = delete;
// };

} // namespace pltxt2htm::details
