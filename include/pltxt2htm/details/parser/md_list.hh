#pragma once

#include <cstddef>
#include <cstdint>
#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "../../heap_guard.hh"

namespace pltxt2htm::details {

enum class MdListNodeType : ::std::uint_least32_t {
    text = 0,
    sublist,
};

class MdListBaseNode {
    ::pltxt2htm::details::MdListNodeType md_list_node_type;

public:
    constexpr MdListBaseNode(::pltxt2htm::details::MdListNodeType md_list_node_type_) noexcept
        : md_list_node_type(md_list_node_type_) {
    }

    constexpr MdListBaseNode(MdListBaseNode const&) noexcept = default;

    constexpr MdListBaseNode(MdListBaseNode&&) noexcept = default;

    constexpr auto operator=(MdListBaseNode const&) & noexcept -> MdListBaseNode& = default;

    constexpr auto operator=(MdListBaseNode&&) & noexcept -> MdListBaseNode& = default;

    constexpr auto&& get_type(this ::pltxt2htm::details::MdListBaseNode const& self) noexcept {
        return ::std::as_const(self.md_list_node_type);
    }
};

class MdListTextNode : public ::pltxt2htm::details::MdListBaseNode {
    ::fast_io::u8string text;

public:
    constexpr MdListTextNode(::fast_io::u8string&& text_) noexcept
        : ::pltxt2htm::details::MdListBaseNode{::pltxt2htm::details::MdListNodeType::text},
          text(::std::move(text_)) {
    }

    constexpr MdListTextNode(::pltxt2htm::details::MdListTextNode const&) noexcept = default;

    constexpr MdListTextNode(::pltxt2htm::details::MdListTextNode&&) noexcept = default;

    constexpr auto operator=(::pltxt2htm::details::MdListTextNode const&) & noexcept
        -> ::pltxt2htm::details::MdListTextNode& = default;

    constexpr auto operator=(::pltxt2htm::details::MdListTextNode&&) & noexcept
        -> ::pltxt2htm::details::MdListTextNode& = default;

    constexpr auto&& get_text(this ::pltxt2htm::details::MdListTextNode const& self) noexcept {
        return ::std::as_const(self.text);
    }
};

using MdListAst = ::fast_io::list<::pltxt2htm::HeapGuard<::pltxt2htm::details::MdListBaseNode>>;

class MdListSublistNode : public ::pltxt2htm::details::MdListBaseNode {
    ::pltxt2htm::details::MdListAst sublist;

public:
    constexpr MdListSublistNode(::pltxt2htm::details::MdListAst&& sublist_) noexcept
        : ::pltxt2htm::details::MdListBaseNode{::pltxt2htm::details::MdListNodeType::sublist},
          sublist(::std::move(sublist_)) {
    }

    constexpr MdListSublistNode(::pltxt2htm::details::MdListSublistNode const&) noexcept = delete;

    constexpr MdListSublistNode(::pltxt2htm::details::MdListSublistNode&&) noexcept = default;

    constexpr auto operator=(::pltxt2htm::details::MdListSublistNode const&) & noexcept
        -> ::pltxt2htm::details::MdListSublistNode& = delete;

    constexpr auto operator=(::pltxt2htm::details::MdListSublistNode&&) & noexcept
        -> ::pltxt2htm::details::MdListSublistNode& = default;

    constexpr auto&& get_sublist(this ::pltxt2htm::details::MdListSublistNode const& self) noexcept {
        return ::std::as_const(self.sublist);
    }
};

enum class MdListItemKind : ::std::uint_least32_t {
    // -
    hyphen = 0,
    // +
    plus,
    // *
    asterisk,
    // 1. 2. e.t.c.
    number,
};

template<bool ndebug>
constexpr auto optionally_to_md_list_ast(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::MdListAst> {
    ::pltxt2htm::details::MdListAst result{};
    ::std::size_t const pltext_size{pltext.size()};

        // TODO
}

} // namespace pltxt2htm::details
