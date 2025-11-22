#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "../utils.hh"
#include "../../heap_guard.hh"

namespace pltxt2htm::details {

enum class MdListNodeType : ::std::uint_least32_t {
    text = 0,
    md_ul,
    md_ol,
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

    constexpr auto operator==(this ::pltxt2htm::details::MdListTextNode const& self,
                              ::pltxt2htm::details::MdListTextNode const& other) noexcept -> bool {
        return self.text == other.text;
    }

    constexpr auto&& get_text(this ::pltxt2htm::details::MdListTextNode const& self) noexcept {
        return ::std::as_const(self.text);
    }

    constexpr auto get_text_view(this ::pltxt2htm::details::MdListTextNode const& self) noexcept {
        return ::fast_io::u8string_view(::fast_io::mnp::os_c_str(self.text));
    }
};

using MdListAst = ::fast_io::vector<::pltxt2htm::HeapGuard<::pltxt2htm::details::MdListBaseNode>>;

class MdListUlNode : public ::pltxt2htm::details::MdListBaseNode {
    ::pltxt2htm::details::MdListAst sublist;

public:
    constexpr MdListUlNode(::pltxt2htm::details::MdListAst&& sublist_) noexcept
        : ::pltxt2htm::details::MdListBaseNode{::pltxt2htm::details::MdListNodeType::md_ul},
          sublist(::std::move(sublist_)) {
    }

    constexpr MdListUlNode(::pltxt2htm::details::MdListUlNode const&) noexcept = delete;

    constexpr MdListUlNode(::pltxt2htm::details::MdListUlNode&&) noexcept = default;

    constexpr ~MdListUlNode() noexcept = default;

    constexpr auto operator=(::pltxt2htm::details::MdListUlNode const&) & noexcept
        -> ::pltxt2htm::details::MdListUlNode& = delete;

    constexpr auto operator=(::pltxt2htm::details::MdListUlNode&&) & noexcept
        -> ::pltxt2htm::details::MdListUlNode& = default;

    constexpr auto operator==(this ::pltxt2htm::details::MdListUlNode const& self,
                              ::pltxt2htm::details::MdListUlNode const& other) noexcept -> bool;

    constexpr auto&& get_sublist(this ::pltxt2htm::details::MdListUlNode& self) noexcept {
        return self.sublist;
    }

    constexpr auto&& get_sublist(this ::pltxt2htm::details::MdListUlNode const& self) noexcept {
        return ::std::as_const(self.sublist);
    }

    constexpr auto&& get_sublist(this ::pltxt2htm::details::MdListUlNode&& self) noexcept {
        return ::std::move(self.sublist);
    }
};

class MdListOlNode : public ::pltxt2htm::details::MdListBaseNode {
    ::pltxt2htm::details::MdListAst sublist;

public:
    constexpr MdListOlNode(::pltxt2htm::details::MdListAst&& sublist_) noexcept
        : ::pltxt2htm::details::MdListBaseNode{::pltxt2htm::details::MdListNodeType::md_ul},
          sublist(::std::move(sublist_)) {
    }

    constexpr MdListOlNode(::pltxt2htm::details::MdListOlNode const&) noexcept = delete;

    constexpr MdListOlNode(::pltxt2htm::details::MdListOlNode&&) noexcept = default;

    constexpr ~MdListOlNode() noexcept = default;

    constexpr auto operator=(::pltxt2htm::details::MdListOlNode const&) & noexcept
        -> ::pltxt2htm::details::MdListOlNode& = delete;

    constexpr auto operator=(::pltxt2htm::details::MdListOlNode&&) & noexcept
        -> ::pltxt2htm::details::MdListOlNode& = default;

    constexpr auto operator==(this ::pltxt2htm::details::MdListOlNode const& self,
                              ::pltxt2htm::details::MdListOlNode const& other) noexcept -> bool;

    constexpr auto&& get_sublist(this ::pltxt2htm::details::MdListOlNode& self) noexcept {
        return self.sublist;
    }

    constexpr auto&& get_sublist(this ::pltxt2htm::details::MdListOlNode const& self) noexcept {
        return ::std::as_const(self.sublist);
    }

    constexpr auto&& get_sublist(this ::pltxt2htm::details::MdListOlNode&& self) noexcept {
        return ::std::move(self.sublist);
    }
};

constexpr auto operator==(::pltxt2htm::details::MdListBaseNode const& self,
                          ::pltxt2htm::details::MdListBaseNode const& other) noexcept -> bool {
    if (self.get_type() != other.get_type()) {
        return false;
    }

    switch (self.get_type()) {
    case ::pltxt2htm::details::MdListNodeType::text: {
        return static_cast<::pltxt2htm::details::MdListTextNode const&>(self) ==
               static_cast<::pltxt2htm::details::MdListTextNode const&>(other);
    }
    case ::pltxt2htm::details::MdListNodeType::md_ul: {
        return static_cast<::pltxt2htm::details::MdListUlNode const&>(self) ==
               static_cast<::pltxt2htm::details::MdListUlNode const&>(other);
    }
    case ::pltxt2htm::details::MdListNodeType::md_ol: {
        return static_cast<::pltxt2htm::details::MdListOlNode const&>(self) ==
               static_cast<::pltxt2htm::details::MdListOlNode const&>(other);
    }
#if 0
    default:
        [[unlikely]] {
            ::exception::unreachable/*<false>*/();
        }
#endif
    }
    ::exception::unreachable<false>();
}

constexpr auto MdListUlNode::operator==(this ::pltxt2htm::details::MdListUlNode const& self,
                                        ::pltxt2htm::details::MdListUlNode const& other) noexcept -> bool {
    return self.sublist == other.sublist;
}

constexpr auto MdListOlNode::operator==(this ::pltxt2htm::details::MdListOlNode const& self,
                                        ::pltxt2htm::details::MdListOlNode const& other) noexcept -> bool {
    return self.sublist == other.sublist;
}

template<typename T>
concept is_md_list_node = ::std::derived_from<::std::remove_cvref_t<T>, ::pltxt2htm::details::MdListBaseNode>;

enum class MdUlListItemKind : char8_t {
    hyphen = u8'-',
    plus = u8'+',
    asterisk = u8'*',
    ordered_item = u8',',
};

class MdListFrameContext {
    ::pltxt2htm::details::MdUlListItemKind item_kind;

public:
    ::std::size_t space_hierarchy;
    ::fast_io::u8string_view pltext;
    ::std::size_t current_index{};
    ::pltxt2htm::details::MdListAst result{};

    constexpr MdListFrameContext(::pltxt2htm::details::MdUlListItemKind item_kind_, ::std::size_t space_hierarchy_,
                                 ::fast_io::u8string_view pltext_) noexcept
        : item_kind(item_kind_),
          space_hierarchy(space_hierarchy_),
          pltext(::std::move(pltext_)) {
    }

    constexpr MdListFrameContext(::pltxt2htm::details::MdUlListItemKind item_kind_, ::std::size_t space_hierarchy_,
                                 ::fast_io::u8string_view pltext_, ::std::size_t current_index_) noexcept
        : item_kind(item_kind_),
          space_hierarchy(space_hierarchy_),
          pltext(::std::move(pltext_)),
          current_index{current_index_} {
    }

    constexpr MdListFrameContext(::pltxt2htm::details::MdListFrameContext&&) noexcept = default;

    constexpr ~MdListFrameContext() noexcept = default;

    constexpr auto operator=(this ::pltxt2htm::details::MdListFrameContext& self,
                             ::pltxt2htm::details::MdListFrameContext&&) noexcept
        -> ::pltxt2htm::details::MdListFrameContext& = default;

    constexpr auto&& get_item_kind(this ::pltxt2htm::details::MdListFrameContext const& self) noexcept {
        return ::std::as_const(self.item_kind);
    }
};

struct PreviousItemInfo {
    ::std::size_t space_hierarchy;
    bool call_stack_is_single;
    ::pltxt2htm::details::MdUlListItemKind item_kind;
};

template<bool ndebug, ::pltxt2htm::details::MdUlListItemKind item_kind>
[[nodiscard]]
constexpr auto is_valid_md_ul_list_hierarchy(
    ::fast_io::u8string_view pltext, ::std::size_t const space_hierarchy,
    ::exception::optional<::pltxt2htm::details::PreviousItemInfo> const expect) noexcept -> bool {
    if (expect.has_value() &&
        expect.template value<ndebug>().item_kind != ::pltxt2htm::details::MdUlListItemKind::hyphen &&
        expect.template value<ndebug>().item_kind != ::pltxt2htm::details::MdUlListItemKind::plus &&
        expect.template value<ndebug>().item_kind != ::pltxt2htm::details::MdUlListItemKind::asterisk) {
        return false;
    }
    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, space_hierarchy) != static_cast<char8_t>(item_kind)) {
        return false;
    }

    if ( // parsing the first line
        !expect.has_value() ||
        // e.g.
        // - test
        // - test
        //   - text <== here
        space_hierarchy > expect.template value<ndebug>().space_hierarchy + 1 ||
        // e.g.
        // - test
        //   - test
        //   + test <== here
        (!expect.template value<ndebug>().call_stack_is_single &&
         space_hierarchy >= expect.template value<ndebug>().space_hierarchy) ||
        // e.g.
        // - test
        // - test
        // + test <== here, this line is invalid markdown list
        // Note that only the first hierarchy apply this rule
        // e.g.
        // - test
        //   - test
        //   + test <== here, this is allowed
        (expect.template value<ndebug>().call_stack_is_single &&
         expect.template value<ndebug>().item_kind == item_kind)) {
        return true;
    }

    // e.g.
    // - test
    // - test
    //   - text
    // - text <== here
    // Despite this is a valid list, but we will return nullopt to leave it as the problem of
    // previous frame of call_stack
    // Or rest of those invalid situations
    return false;
}

template<bool ndebug>
[[nodiscard]]
constexpr auto is_valid_md_ol_list_hierarchy(
    ::fast_io::u8string_view pltext, ::std::size_t const space_hierarchy,
    ::exception::optional<::pltxt2htm::details::PreviousItemInfo> const expect) noexcept
    -> ::exception::optional<::std::size_t> {
    if (expect.has_value() &&
        expect.template value<ndebug>().item_kind != ::pltxt2htm::details::MdUlListItemKind::ordered_item) {
        return ::exception::nullopt_t{};
    }
    ::std::size_t pltext_size{pltext.size()};
    if (pltext_size < 4) {
        return ::exception::nullopt_t{};
    }
    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 0) < u8'0' ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, 1) > u8'9') {
        return false;
    }
    {
        ::std::size_t i{1};
        for (; i < pltext_size; ++i) {
            auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i);
            if (chr < u8'0' || chr > u8'9') {
                break;
            }
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i) != u8'.') {
            return ::exception::nullopt_t{};
        }
        if (++i >= pltext_size) {
            return ::exception::nullopt_t{};
        }
        if ( // parsing the first line
            !expect.has_value() ||
            // e.g.
            // - test
            // - test
            //   1. text <== here
            space_hierarchy > expect.template value<ndebug>().space_hierarchy + 1 ||
            // e.g.
            // - test
            //   1. test
            //   2. test <== here
            (!expect.template value<ndebug>().call_stack_is_single &&
             space_hierarchy >= expect.template value<ndebug>().space_hierarchy) ||
            // e.g.
            // 1. test
            // 2. test
            // 3. test <== here, this line is invalid markdown list
            expect.template value<ndebug>().call_stack_is_single) {
            return i;
        }
    }
    return ::exception::nullopt_t{};
}

struct TryParseItemResult {
    ::std::size_t space_hierarchy;
    ::std::size_t forward_index;
    ::fast_io::u8string text;
    ::pltxt2htm::details::MdUlListItemKind item_kind;
};

template<bool ndebug>
[[nodiscard]]
constexpr auto try_parse_item(
    ::fast_io::u8string_view pltext,
    ::exception::optional<::pltxt2htm::details::PreviousItemInfo> const expect = ::exception::nullopt_t{}) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseItemResult> {
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
    ::std::size_t const space_hierarchy{current_index};

    // parsing item kind
    ::pltxt2htm::details::MdUlListItemKind item_kind
#if __has_cpp_attribute(indeterminate)
        // https://en.cppreference.com/w/cpp/language/attributes/indeterminate.html
        [[indeterminate]]
#endif
        ;
    if (::pltxt2htm::details::is_valid_md_ul_list_hierarchy<ndebug, ::pltxt2htm::details::MdUlListItemKind::hyphen>(
            pltext, space_hierarchy, expect)) {
        item_kind = ::pltxt2htm::details::MdUlListItemKind::hyphen;
        ++current_index;
    } else if (::pltxt2htm::details::is_valid_md_ul_list_hierarchy<ndebug,
                                                                   ::pltxt2htm::details::MdUlListItemKind::plus>(
                   pltext, space_hierarchy, expect)) {
        item_kind = ::pltxt2htm::details::MdUlListItemKind::plus;

        ++current_index;
    } else if (::pltxt2htm::details::is_valid_md_ul_list_hierarchy<ndebug,
                                                                   ::pltxt2htm::details::MdUlListItemKind::asterisk>(
                   pltext, space_hierarchy, expect)) {
        item_kind = ::pltxt2htm::details::MdUlListItemKind::asterisk;
        ++current_index;
    } else if (auto opt_size =
                   ::pltxt2htm::details::is_valid_md_ol_list_hierarchy<ndebug>(pltext, space_hierarchy, expect);
               opt_size.has_value()) {
        item_kind = ::pltxt2htm::details::MdUlListItemKind::ordered_item;
        current_index += opt_size.template value<ndebug>();
    } else {
        return ::exception::nullopt_t{};
    }

    // - or + or * must be followed by space
    if (current_index == pltext.size() ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) != u8' ') {
        return ::exception::nullopt_t{};
    }
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
    return ::pltxt2htm::details::TryParseItemResult{
        .space_hierarchy = space_hierarchy,
        .forward_index = current_index,
        .text = ::std::move(text),
        .item_kind = item_kind,
    };
}

struct ToMdListAstResult {
    ::pltxt2htm::details::MdListAst ast;
    ::std::size_t forward_index;
};

template<bool ndebug>
[[nodiscard]]
constexpr auto optionally_to_md_list_ast(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::ToMdListAstResult> {
    ::fast_io::stack<::pltxt2htm::details::MdListFrameContext,
                     ::fast_io::vector<::pltxt2htm::details::MdListFrameContext>>
        call_stack{};

    // manually managing stack to avoid stack-overflow
    {
        if (auto opt_ul_item = ::pltxt2htm::details::try_parse_item<ndebug>(pltext); opt_ul_item.has_value()) {
            auto&& [space_hierarchy, forward_index, text, item_kind] = opt_ul_item.template value<ndebug>();
            ::pltxt2htm::details::MdListFrameContext current_frame{item_kind, space_hierarchy, pltext, forward_index};
            current_frame.result.emplace_back(
                ::pltxt2htm::HeapGuard<::pltxt2htm::details::MdListTextNode>(::std::move(text)));
            if (forward_index >= current_frame.pltext.size()) {
                return ::pltxt2htm::details::ToMdListAstResult{::std::move(current_frame.result), forward_index};
            }
            call_stack.push(::std::move(current_frame));
        } else {
            return ::exception::nullopt_t{};
        }
    }
    while (true) {
        auto&& current_index = call_stack.top().current_index;
        auto&& result = call_stack.top().result;
        ::std::size_t const pltext_size{call_stack.top().pltext.size()};
        auto opt_list_item = ::pltxt2htm::details::try_parse_item<ndebug>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(call_stack.top().pltext, current_index),
            ::pltxt2htm::details::PreviousItemInfo{.space_hierarchy = call_stack.top().space_hierarchy,
                                                   .call_stack_is_single = call_stack.size() == 1,
                                                   .item_kind = call_stack.top().get_item_kind()});
        if (!opt_list_item.has_value()) {
            auto frame = ::std::move(call_stack.top());
            call_stack.pop();
            if (call_stack.empty()) {
                return ::pltxt2htm::details::ToMdListAstResult{::std::move(frame.result), frame.current_index};
            } else {
                call_stack.top().result.emplace_back(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::details::MdListUlNode>(::std::move(frame.result)));
                call_stack.top().current_index += frame.current_index;
                continue;
            }
        }
        auto&& [space_hierarchy, forward_index, text, item_kind] = opt_list_item.template value<ndebug>();
        current_index += forward_index;
        if (space_hierarchy > call_stack.top().space_hierarchy + 1) {
            call_stack.push(::pltxt2htm::details::MdListFrameContext{
                item_kind, space_hierarchy,
                ::pltxt2htm::details::u8string_view_subview<ndebug>(call_stack.top().pltext, current_index)});
            call_stack.top().result.emplace_back(
                ::pltxt2htm::HeapGuard<::pltxt2htm::details::MdListTextNode>(::std::move(text)));
            continue;
        } else {
            result.emplace_back(::pltxt2htm::HeapGuard<::pltxt2htm::details::MdListTextNode>(::std::move(text)));
            call_stack.top().space_hierarchy = space_hierarchy;

            if (current_index < pltext_size) {
                continue;
            }
            auto frame = ::std::move(call_stack.top());
            call_stack.pop();
            if (call_stack.empty()) {
                return ::pltxt2htm::details::ToMdListAstResult{::std::move(frame.result), pltext_size};
            } else {
                call_stack.top().result.emplace_back(
                    ::pltxt2htm::HeapGuard<::pltxt2htm::details::MdListUlNode>(::std::move(frame.result)));
                call_stack.top().current_index += frame.current_index;
                continue;
            }
        }
    }
}

} // namespace pltxt2htm::details
