#pragma once

#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../heap_guard.hh"
#include "../astnode/basic.hh"

namespace pltxt2htm::details {

class BackendBasicFrameContext {
public:
    ::pltxt2htm::Ast const& ast_;
    ::pltxt2htm::NodeType const nested_tag_type_;
    bool is_not_same_tag_;
    // TODO How about store iterator?
    ::std::size_t current_index_;

protected:
    BackendBasicFrameContext(::pltxt2htm::Ast const& ast, ::pltxt2htm::NodeType const nested_tag_type,
                             bool is_not_same_tag, ::std::size_t current_index) noexcept
        : ast_(ast),
          nested_tag_type_{nested_tag_type},
          is_not_same_tag_{is_not_same_tag},
          current_index_{current_index} {
    }

public:
    constexpr BackendBasicFrameContext(::pltxt2htm::details::BackendBasicFrameContext const&) noexcept = default;
    constexpr BackendBasicFrameContext(::pltxt2htm::details::BackendBasicFrameContext&&) noexcept = default;

    constexpr ~BackendBasicFrameContext() noexcept = default;

    // const reference do not support operator=
    constexpr ::pltxt2htm::details::BackendBasicFrameContext& operator=(
        ::pltxt2htm::details::BackendBasicFrameContext const&) noexcept = delete;
    constexpr ::pltxt2htm::details::BackendBasicFrameContext& operator=(
        ::pltxt2htm::details::BackendBasicFrameContext&&) noexcept = delete;
};

class BackendHtmlPreCodeContext : public ::pltxt2htm::details::BackendBasicFrameContext {
public:
    constexpr BackendHtmlPreCodeContext(::pltxt2htm::Ast const& ast, ::pltxt2htm::NodeType const nested_tag_type,
                                        ::std::size_t current_index) noexcept
        : ::pltxt2htm::details::BackendBasicFrameContext(ast, nested_tag_type, true, current_index) {
    }
};

class BackendBareTagContext : public ::pltxt2htm::details::BackendBasicFrameContext {
public:
    BackendBareTagContext(::pltxt2htm::Ast const& ast, ::pltxt2htm::NodeType const nested_tag_type,
                          bool is_not_same_tag, ::std::size_t current_index) noexcept
        : ::pltxt2htm::details::BackendBasicFrameContext(ast, nested_tag_type, is_not_same_tag, current_index) {
    }

    constexpr BackendBareTagContext(::pltxt2htm::details::BackendBareTagContext const&) noexcept = default;
    constexpr BackendBareTagContext(::pltxt2htm::details::BackendBareTagContext&&) noexcept = default;

    constexpr ~BackendBareTagContext() noexcept = default;

    // const reference do not support operator=
    constexpr ::pltxt2htm::details::BackendBareTagContext& operator=(
        ::pltxt2htm::details::BackendBareTagContext const&) noexcept = delete;
    constexpr ::pltxt2htm::details::BackendBareTagContext& operator=(
        ::pltxt2htm::details::BackendBareTagContext&&) noexcept = delete;
};

class BackendEqualSignTagContext : public ::pltxt2htm::details::BackendBasicFrameContext {
public:
    ::fast_io::u8string_view id_;

    BackendEqualSignTagContext(::pltxt2htm::Ast const& ast, ::pltxt2htm::NodeType const nested_tag_type,
                               bool is_not_same_tag, ::std::size_t current_index, ::fast_io::u8string_view id) noexcept
        : ::pltxt2htm::details::BackendBasicFrameContext(ast, nested_tag_type, is_not_same_tag, current_index),
          id_{id} {
    }

    constexpr BackendEqualSignTagContext(::pltxt2htm::details::BackendEqualSignTagContext const&) noexcept = default;
    constexpr BackendEqualSignTagContext(::pltxt2htm::details::BackendEqualSignTagContext&&) noexcept = default;

    constexpr ~BackendEqualSignTagContext() noexcept = default;

    // const reference do not support operator=
    constexpr ::pltxt2htm::details::BackendEqualSignTagContext& operator=(
        ::pltxt2htm::details::BackendEqualSignTagContext const&) noexcept = delete;
    constexpr ::pltxt2htm::details::BackendEqualSignTagContext& operator=(
        ::pltxt2htm::details::BackendEqualSignTagContext&&) noexcept = delete;
};

class BackendPlSizeTagContext : public ::pltxt2htm::details::BackendBasicFrameContext {
public:
    ::std::size_t id_;

    BackendPlSizeTagContext(::pltxt2htm::Ast const& ast, ::pltxt2htm::NodeType const nested_tag_type,
                            bool is_not_same_tag, ::std::size_t current_index, ::std::size_t id) noexcept
        : ::pltxt2htm::details::BackendBasicFrameContext(ast, nested_tag_type, is_not_same_tag, current_index),
          id_{id} {
    }

    constexpr BackendPlSizeTagContext(::pltxt2htm::details::BackendPlSizeTagContext const&) noexcept = default;
    constexpr BackendPlSizeTagContext(::pltxt2htm::details::BackendPlSizeTagContext&&) noexcept = default;

    constexpr ~BackendPlSizeTagContext() noexcept = default;

    // const reference do not support operator=
    constexpr ::pltxt2htm::details::BackendPlSizeTagContext& operator=(
        ::pltxt2htm::details::BackendPlSizeTagContext const&) noexcept = delete;
    constexpr ::pltxt2htm::details::BackendPlSizeTagContext& operator=(
        ::pltxt2htm::details::BackendPlSizeTagContext&&) noexcept = delete;
};

} // namespace pltxt2htm::details
