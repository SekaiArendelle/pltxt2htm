#pragma once

#include <cstddef>
#include <cstdint>
#include <fast_io/fast_io_dsal/list.h>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "../utils.hh"
#include "../../heap_guard.hh"

namespace pltxt2htm::details {

enum class MdListNodeType : ::std::uint_least32_t {
    text = 0,
    sublist,
};

class MdListBaseNode {
    ::pltxt2htm::details::MdListNodeType md_list_node_type;

protected:
    constexpr MdListBaseNode(::pltxt2htm::details::MdListNodeType md_list_node_type_) noexcept
        : md_list_node_type(md_list_node_type_) {
    }

    constexpr MdListBaseNode(MdListBaseNode const&) noexcept = default;

    constexpr MdListBaseNode(MdListBaseNode&&) noexcept = default;

    constexpr ~MdListBaseNode() noexcept = default;

    constexpr auto operator=(MdListBaseNode const&) & noexcept -> MdListBaseNode& = default;

    constexpr auto operator=(MdListBaseNode&&) & noexcept -> MdListBaseNode& = default;

public:
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

    constexpr ~MdListTextNode() noexcept = default;

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

    constexpr ~MdListSublistNode() noexcept = default;

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
};

class MdListFrameContext {
    ::pltxt2htm::details::MdListItemKind item_kind;

public:
    ::std::size_t space_hierarchy;
    ::std::size_t current_index{};
    ::pltxt2htm::details::MdListAst result{};

    constexpr MdListFrameContext(::pltxt2htm::details::MdListItemKind item_kind_,
                                 ::std::size_t space_hierarchy_) noexcept
        : item_kind(item_kind_),
          space_hierarchy(space_hierarchy_) {
    }

    constexpr MdListFrameContext(::pltxt2htm::details::MdListItemKind item_kind_, ::std::size_t space_hierarchy_,
                                 ::std::size_t current_index_) noexcept
        : item_kind(item_kind_),
          space_hierarchy(space_hierarchy_),
          current_index(current_index_) {
    }

    constexpr MdListFrameContext(::pltxt2htm::details::MdListFrameContext&&) noexcept = default;

    constexpr ~MdListFrameContext() noexcept = default;

    constexpr auto operator=(::pltxt2htm::details::MdListFrameContext&&) & noexcept
        -> ::pltxt2htm::details::MdListFrameContext& = default;

    constexpr auto&& get_item_kind(this ::pltxt2htm::details::MdListFrameContext const& self) noexcept {
        return ::std::as_const(self.item_kind);
    }
};

struct TryParseAListItemResult {
    ::std::size_t space_hierarchy;
    ::std::size_t forward_index;
    ::fast_io::u8string text;
    ::pltxt2htm::details::MdListItemKind item_kind;
};

template<bool ndebug>
[[nodiscard]]
constexpr auto try_parse_a_list_item(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseAListItemResult> {
    ::std::size_t current_index{};
    // parsing spaces before - or + or *
    for (; current_index < pltext.size(); ++current_index) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if (chr != u8' ') {
            break;
        }
    }
    if (current_index == pltext.size()) {
        return ::exception::nullopt_t{};
    }
    ::std::size_t space_hierarchy{current_index};

    // parsing item kind
    ::pltxt2htm::details::MdListItemKind item_kind
#if __has_cpp_attribute(indeterminate)
        // https://en.cppreference.com/w/cpp/language/attributes/indeterminate.html
        [[indeterminate]]
#endif
        ;
    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8'-') {
        item_kind = ::pltxt2htm::details::MdListItemKind::hyphen;
    } else if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8'+') {
        item_kind = ::pltxt2htm::details::MdListItemKind::plus;
    } else if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8'*') {
        item_kind = ::pltxt2htm::details::MdListItemKind::asterisk;
    } else {
        return ::exception::nullopt_t{};
    }
    ++current_index;
    // parsing spaces after - or + or *
    for (; current_index < pltext.size(); ++current_index) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if (chr != u8' ') {
            break;
        }
    }
    // parsing text after - or + or *
    ::fast_io::u8string text{};
    for (; current_index < pltext.size(); ++current_index) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if (chr == u8'\n') {
            ++current_index;
            break;
        }
        text.push_back(chr);
    }
    return ::pltxt2htm::details::TryParseAListItemResult{
        .space_hierarchy = space_hierarchy,
        .forward_index = current_index,
        .text = ::std::move(text),
        .item_kind = item_kind,
    };
}

template<bool ndebug>
constexpr auto optionally_to_md_list_ast(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::MdListAst> {
    ::std::size_t const pltext_size{pltext.size()};
    ::fast_io::stack<::pltxt2htm::details::MdListFrameContext,
                     ::fast_io::list<::pltxt2htm::details::MdListFrameContext>>
        call_stack{};

    // entry:
    {
        auto opt_list_item = ::pltxt2htm::details::try_parse_a_list_item<ndebug>(pltext);
        if (!opt_list_item.has_value()) {
            return ::exception::nullopt_t{};
        }
        auto&& [space_hierarchy, forward_index, text, item_kind] = opt_list_item.template value<ndebug>();
        ::pltxt2htm::details::MdListFrameContext current_frame{item_kind, space_hierarchy, forward_index};
        current_frame.result.emplace_back(
            ::pltxt2htm::HeapGuard<::pltxt2htm::details::MdListTextNode>(::std::move(text)));
        if (forward_index >= pltext_size) {
            return ::std::move(current_frame.result);
        }
        call_stack.push(::std::move(current_frame));
    }
    while (true) {
        auto&& current_index = call_stack.top().current_index;
        auto&& result = call_stack.top().result;
        auto opt_list_item = ::pltxt2htm::details::try_parse_a_list_item<ndebug>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(pltext, current_index));
        if (!opt_list_item.has_value()) {
            return ::std::move(result);
        }
        auto&& [space_hierarchy, forward_index, text, item_kind] = opt_list_item.template value<ndebug>();
        if (item_kind != call_stack.top().get_item_kind()) {
            return ::std::move(result);
        }
        if (space_hierarchy > call_stack.top().space_hierarchy + 1) {
            //
        } else {
            result.emplace_back(::pltxt2htm::HeapGuard<::pltxt2htm::details::MdListTextNode>(::std::move(text)));
            current_index += forward_index;
            call_stack.top().space_hierarchy = space_hierarchy;

            if (current_index >= pltext_size) {
                return ::std::move(result);
            }
        }
    }
}

} // namespace pltxt2htm::details
