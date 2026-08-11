/**
 * @file md_list.hh
 * @brief Markdown list parser utilities for nested unordered/ordered list AST construction.
 */

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
#include "../../contracts.hh"
#include "../../ast/ast.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Internal list AST node discriminator (shared by Markdown and HTML lists).
 */
enum class ListNodeType : unsigned {
    list_li = 0,
    list_li_checkbox,
    list_ul,
    list_ol,
};

// Forward declaration for recursive ListAst
template<::pltxt2htm::Contracts ndebug>
class ListBaseNode;

/**
 * @brief Internal list AST container type (shared by Markdown and HTML lists).
 *
 * @details The intermediate ListAst is necessary because parsing a list requires a
 *          two-phase approach: first, `optionally_to_md_list_ast` calls `try_parse_item`
 *          once per item to build the full nested AST (determining hierarchy boundaries,
 *          marker types, and extracting item text). Second, `parse_pltxt` iterates the
 *          pre-built AST without re-parsing.
 *
 *          Without ListAst, `parse_pltxt` would need to call `try_parse_item` twice
 *          for each item: once during a pre-scan (to determine the list boundary / item
 *          hierarchy before creating child frames) and again when actually producing the
 *          output. The pre-built AST avoids this redundant work — each item is parsed
 *          exactly once.
 */
template<::pltxt2htm::Contracts ndebug>
using ListAst = ::fast_io::vector<::pltxt2htm::details::ListBaseNode<ndebug>>;

/**
 * @brief Leaf list node that stores a single list-item text payload.
 */
class ListLiNode {
    ::fast_io::u8string text;

public:
    constexpr ListLiNode(::fast_io::u8string&& text_) noexcept
        : text(::std::move(text_)) {
    }

    constexpr ListLiNode(::pltxt2htm::details::ListLiNode const&) noexcept = default;

    constexpr ListLiNode(::pltxt2htm::details::ListLiNode&&) noexcept = default;

    constexpr ~ListLiNode() noexcept = default;

    constexpr auto operator=(this ::pltxt2htm::details::ListLiNode& self,
                             ::pltxt2htm::details::ListLiNode const& other) noexcept
        -> ::pltxt2htm::details::ListLiNode& = default;

    constexpr auto operator=(this ::pltxt2htm::details::ListLiNode& self, ::pltxt2htm::details::ListLiNode&&) noexcept
        -> ::pltxt2htm::details::ListLiNode& = default;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto operator==(this ::pltxt2htm::details::ListLiNode const& self,
                              ::pltxt2htm::details::ListLiNode const& other) noexcept -> bool {
        return self.text == other.text;
    }

    [[nodiscard]]
    constexpr auto get_text(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.text);
    }

    [[nodiscard]]
    constexpr auto get_text_view(this ::pltxt2htm::details::ListLiNode const& self) noexcept {
        return ::fast_io::u8string_view{self.text.data(), self.text.size()};
    }
};

/**
 * @brief Leaf list node for checkbox items (- [ ] / - [x]).
 */
class ListLiCheckboxNode {
    ::fast_io::u8string text;
    bool checked{};

public:
    constexpr ListLiCheckboxNode(::fast_io::u8string&& text_, bool checked_) noexcept
        : text(::std::move(text_)),
          checked(checked_) {
    }

    constexpr ListLiCheckboxNode(::pltxt2htm::details::ListLiCheckboxNode const&) noexcept = default;

    constexpr ListLiCheckboxNode(::pltxt2htm::details::ListLiCheckboxNode&&) noexcept = default;

    constexpr ~ListLiCheckboxNode() noexcept = default;

    constexpr auto operator=(this ::pltxt2htm::details::ListLiCheckboxNode& self,
                             ::pltxt2htm::details::ListLiCheckboxNode const& other) noexcept
        -> ::pltxt2htm::details::ListLiCheckboxNode& = default;

    constexpr auto operator=(this ::pltxt2htm::details::ListLiCheckboxNode& self,
                             ::pltxt2htm::details::ListLiCheckboxNode&&) noexcept
        -> ::pltxt2htm::details::ListLiCheckboxNode& = default;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto operator==(this ::pltxt2htm::details::ListLiCheckboxNode const& self,
                              ::pltxt2htm::details::ListLiCheckboxNode const& other) noexcept -> bool {
        return self.text == other.text && self.checked == other.checked;
    }

    [[nodiscard]]
    constexpr auto is_checked(this auto const& self) noexcept -> bool {
        return self.checked;
    }

    [[nodiscard]]
    constexpr auto get_text(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.text);
    }

    [[nodiscard]]
    constexpr auto get_text_view(this ::pltxt2htm::details::ListLiCheckboxNode const& self) noexcept {
        return ::fast_io::u8string_view{self.text.data(), self.text.size()};
    }
};

// ---- ListUlNode declaration (members defined after ListBaseNode) ----

/**
 * @brief Internal unordered-list node containing nested list items.
 */
template<::pltxt2htm::Contracts ndebug>
class ListUlNode {
    ::pltxt2htm::details::ListAst<ndebug> sublist;

public:
    constexpr ListUlNode(::pltxt2htm::details::ListAst<ndebug>&& sublist_) noexcept;

    constexpr ListUlNode(::pltxt2htm::details::ListUlNode<ndebug> const&) noexcept = delete;

    constexpr ListUlNode(::pltxt2htm::details::ListUlNode<ndebug>&&) noexcept;

    constexpr ~ListUlNode() noexcept;

    constexpr auto operator=(this ::pltxt2htm::details::ListUlNode<ndebug>& self,
                             ::pltxt2htm::details::ListUlNode<ndebug> const& other) noexcept
        -> ::pltxt2htm::details::ListUlNode<ndebug>& = delete;

    constexpr auto operator=(this ::pltxt2htm::details::ListUlNode<ndebug>& self,
                             ::pltxt2htm::details::ListUlNode<ndebug>&&) noexcept
        -> ::pltxt2htm::details::ListUlNode<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(::pltxt2htm::details::ListUlNode<ndebug> const& other) const noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_sublist(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.sublist);
    }
};

// ---- ListOlNode declaration (members defined after ListBaseNode) ----

/**
 * @brief Internal ordered-list node containing nested list items.
 */
template<::pltxt2htm::Contracts ndebug>
class ListOlNode {
    ::pltxt2htm::details::ListAst<ndebug> sublist;

public:
    constexpr ListOlNode(::pltxt2htm::details::ListAst<ndebug>&& sublist_) noexcept;

    constexpr ListOlNode(::pltxt2htm::details::ListOlNode<ndebug> const&) noexcept = delete;

    constexpr ListOlNode(::pltxt2htm::details::ListOlNode<ndebug>&&) noexcept;

    constexpr ~ListOlNode() noexcept;

    constexpr auto operator=(this ::pltxt2htm::details::ListOlNode<ndebug>& self,
                             ::pltxt2htm::details::ListOlNode<ndebug> const& other) noexcept
        -> ::pltxt2htm::details::ListOlNode<ndebug>& = delete;

    constexpr auto operator=(this ::pltxt2htm::details::ListOlNode<ndebug>& self,
                             ::pltxt2htm::details::ListOlNode<ndebug>&&) noexcept
        -> ::pltxt2htm::details::ListOlNode<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(::pltxt2htm::details::ListOlNode<ndebug> const& other) const noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_sublist(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.sublist);
    }
};

// ---- ListBaseNode (variant-style) ----

/**
 * @brief Variant-style base type for all internal list nodes.
 */
template<::pltxt2htm::Contracts ndebug>
class ListBaseNode {
    union {
        ::pltxt2htm::details::ListLiNode li_node;
        ::pltxt2htm::details::ListLiCheckboxNode li_checkbox_node;
        ::pltxt2htm::details::ListUlNode<ndebug> ul_node;
        ::pltxt2htm::details::ListOlNode<ndebug> ol_node;
    };

    ::pltxt2htm::details::ListNodeType type;

public:
    constexpr ListBaseNode(::pltxt2htm::details::ListLiNode&& node) noexcept
        : li_node(::std::move(node)),
          type{::pltxt2htm::details::ListNodeType::list_li} {
    }

    constexpr ListBaseNode(::pltxt2htm::details::ListLiCheckboxNode&& node) noexcept
        : li_checkbox_node(::std::move(node)),
          type{::pltxt2htm::details::ListNodeType::list_li_checkbox} {
    }

    constexpr ListBaseNode(::pltxt2htm::details::ListUlNode<ndebug>&& node) noexcept
        : ul_node(::std::move(node)),
          type{::pltxt2htm::details::ListNodeType::list_ul} {
    }

    constexpr ListBaseNode(::pltxt2htm::details::ListOlNode<ndebug>&& node) noexcept
        : ol_node(::std::move(node)),
          type{::pltxt2htm::details::ListNodeType::list_ol} {
    }

    constexpr ListBaseNode(::pltxt2htm::details::ListBaseNode<ndebug> const&) noexcept = delete;

    constexpr ListBaseNode(::pltxt2htm::details::ListBaseNode<ndebug>&& other) noexcept
        : type(other.type) {
        switch (type) /* -Werror=switch */ {
        case ::pltxt2htm::details::ListNodeType::list_li: {
            new (::std::addressof(li_node))::pltxt2htm::details::ListLiNode(::std::move(other.li_node));
            break;
        }
        case ::pltxt2htm::details::ListNodeType::list_li_checkbox: {
            new (::std::addressof(li_checkbox_node))::pltxt2htm::details::ListLiCheckboxNode(
                ::std::move(other.li_checkbox_node));
            break;
        }
        case ::pltxt2htm::details::ListNodeType::list_ul: {
            new (::std::addressof(ul_node))::pltxt2htm::details::ListUlNode<ndebug>(::std::move(other.ul_node));
            break;
        }
        case ::pltxt2htm::details::ListNodeType::list_ol: {
            new (::std::addressof(ol_node))::pltxt2htm::details::ListOlNode<ndebug>(::std::move(other.ol_node));
            break;
        }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected ListNodeType in move constructor");
            }
#endif
        }
    }

    constexpr ~ListBaseNode() noexcept {
        switch (type) /* -Werror=switch */ {
        case ::pltxt2htm::details::ListNodeType::list_li: {
            li_node.~ListLiNode();
            break;
        }
        case ::pltxt2htm::details::ListNodeType::list_li_checkbox: {
            li_checkbox_node.~ListLiCheckboxNode();
            break;
        }
        case ::pltxt2htm::details::ListNodeType::list_ul: {
            ul_node.~ListUlNode<ndebug>();
            break;
        }
        case ::pltxt2htm::details::ListNodeType::list_ol: {
            ol_node.~ListOlNode<ndebug>();
            break;
        }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected ListNodeType in destructor");
            }
#endif
        }
    }

    constexpr auto operator=(this ::pltxt2htm::details::ListBaseNode<ndebug>& self,
                             ::pltxt2htm::details::ListBaseNode<ndebug> const& other) noexcept
        -> ::pltxt2htm::details::ListBaseNode<ndebug>& = delete;

    constexpr auto operator=(this ::pltxt2htm::details::ListBaseNode<ndebug>& self,
                             ::pltxt2htm::details::ListBaseNode<ndebug>&& other) noexcept
        -> ::pltxt2htm::details::ListBaseNode<ndebug>& {
        if (::std::addressof(other) != ::std::addressof(self)) {
            self.~ListBaseNode();
            ::std::construct_at(::std::addressof(self), ::std::move(other));
        }
        return self;
    }

    [[nodiscard]]
    constexpr auto get_type(this auto&& self) noexcept -> ::pltxt2htm::details::ListNodeType {
        return self.type;
    }

    [[nodiscard]]
    constexpr auto get_text(this auto&& self) noexcept -> decltype(auto) {
        switch (self.type) /* -Werror=switch */ {
        case ::pltxt2htm::details::ListNodeType::list_li: {
            return ::std::forward_like<decltype(self)>(self.li_node).get_text();
        }
        case ::pltxt2htm::details::ListNodeType::list_li_checkbox: {
            return ::std::forward_like<decltype(self)>(self.li_checkbox_node).get_text();
        }
        case ::pltxt2htm::details::ListNodeType::list_ul:
            [[fallthrough]];
        case ::pltxt2htm::details::ListNodeType::list_ol:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected list_ul/list_ol in get_text()");
            }
        }
        pltxt2htm_unreachable(u8"Unreachable after get_text() switch");
    }

    [[nodiscard]]
    constexpr auto get_text_view(this auto&& self) noexcept -> ::fast_io::u8string_view {
        switch (self.type) /* -Werror=switch */ {
        case ::pltxt2htm::details::ListNodeType::list_li: {
            return self.li_node.get_text_view();
        }
        case ::pltxt2htm::details::ListNodeType::list_li_checkbox: {
            return self.li_checkbox_node.get_text_view();
        }
        case ::pltxt2htm::details::ListNodeType::list_ul:
            [[fallthrough]];
        case ::pltxt2htm::details::ListNodeType::list_ol:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected list_ul/list_ol in get_text_view()");
            }
        }
        pltxt2htm_unreachable(u8"Unreachable after get_text_view() switch");
    }

    [[nodiscard]]
    constexpr auto get_sublist(this auto&& self) noexcept -> decltype(auto) {
        switch (self.type) /* -Werror=switch */ {
        case ::pltxt2htm::details::ListNodeType::list_ul: {
            return ::std::forward_like<decltype(self)>(self.ul_node).get_sublist();
        }
        case ::pltxt2htm::details::ListNodeType::list_ol: {
            return ::std::forward_like<decltype(self)>(self.ol_node).get_sublist();
        }
        case ::pltxt2htm::details::ListNodeType::list_li:
            [[fallthrough]];
        case ::pltxt2htm::details::ListNodeType::list_li_checkbox:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected list_li/list_li_checkbox in get_sublist()");
            }
        }
        pltxt2htm_unreachable(u8"Unreachable after get_sublist() switch");
    }

    [[nodiscard]]
    constexpr auto is_checked(this auto const& self) noexcept -> bool {
        pltxt2htm_assert(self.type == ::pltxt2htm::details::ListNodeType::list_li_checkbox, u8"node type mismatch");
        return self.li_checkbox_node.is_checked();
    }

    [[nodiscard]]
    friend constexpr auto operator==(::pltxt2htm::details::ListBaseNode<ndebug> const& self,
                                     ::pltxt2htm::details::ListBaseNode<ndebug> const& other) noexcept -> bool {
        if (self.type != other.type) {
            return false;
        }

        switch (self.type) {
        case ::pltxt2htm::details::ListNodeType::list_li: {
            return self.li_node == other.li_node;
        }
        case ::pltxt2htm::details::ListNodeType::list_li_checkbox: {
            return self.li_checkbox_node == other.li_checkbox_node;
        }
        case ::pltxt2htm::details::ListNodeType::list_ul: {
            return self.ul_node == other.ul_node;
        }
        case ::pltxt2htm::details::ListNodeType::list_ol: {
            return self.ol_node == other.ol_node;
        }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected ListNodeType in operator==");
            }
#endif
        }
        pltxt2htm_unreachable(u8"Unreachable after ListNodeType operator== switch");
    }
};

// ---- ListUlNode member definitions (ListBaseNode is now complete) ----

template<::pltxt2htm::Contracts ndebug>
constexpr ListUlNode<ndebug>::ListUlNode(::pltxt2htm::details::ListAst<ndebug>&& sublist_) noexcept
    : sublist(::std::move(sublist_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ListUlNode<ndebug>::ListUlNode(::pltxt2htm::details::ListUlNode<ndebug>&&) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ListUlNode<ndebug>::~ListUlNode() noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ListUlNode<ndebug>::operator=(this ::pltxt2htm::details::ListUlNode<ndebug>& self,
                                             ::pltxt2htm::details::ListUlNode<ndebug>&& other) noexcept
    -> ::pltxt2htm::details::ListUlNode<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto ListUlNode<ndebug>::operator==(::pltxt2htm::details::ListUlNode<ndebug> const& other) const noexcept
    -> bool {
    return sublist == other.sublist;
}

// ---- ListOlNode member definitions (ListBaseNode is now complete) ----

template<::pltxt2htm::Contracts ndebug>
constexpr ListOlNode<ndebug>::ListOlNode(::pltxt2htm::details::ListAst<ndebug>&& sublist_) noexcept
    : sublist(::std::move(sublist_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ListOlNode<ndebug>::ListOlNode(::pltxt2htm::details::ListOlNode<ndebug>&&) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ListOlNode<ndebug>::~ListOlNode() noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ListOlNode<ndebug>::operator=(this ::pltxt2htm::details::ListOlNode<ndebug>& self,
                                             ::pltxt2htm::details::ListOlNode<ndebug>&& other) noexcept
    -> ::pltxt2htm::details::ListOlNode<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto ListOlNode<ndebug>::operator==(::pltxt2htm::details::ListOlNode<ndebug> const& other) const noexcept
    -> bool {
    return sublist == other.sublist;
}

template<typename T>
constexpr bool is_list_ul_node_ = false;

template<::pltxt2htm::Contracts ndebug>
constexpr bool is_list_ul_node_<::pltxt2htm::details::ListUlNode<ndebug>> = true;

template<typename T>
constexpr bool is_list_ol_node_ = false;

template<::pltxt2htm::Contracts ndebug>
constexpr bool is_list_ol_node_<::pltxt2htm::details::ListOlNode<ndebug>> = true;

/**
 * @brief Concept matching the concrete node types stored in ListBaseNode.
 */
template<typename T>
concept is_list_node_type = ::std::is_same_v<::std::remove_cvref_t<T>, ::pltxt2htm::details::ListLiNode> ||
                            ::std::is_same_v<::std::remove_cvref_t<T>, ::pltxt2htm::details::ListLiCheckboxNode> ||
                            ::pltxt2htm::details::is_list_ul_node_<::std::remove_cvref_t<T>> ||
                            ::pltxt2htm::details::is_list_ol_node_<::std::remove_cvref_t<T>>;

/**
 * @brief Marker describing parsed markdown list item style.
 */
enum class MdUlListItemKind : char8_t {
    hyphen = u8'-',
    plus = u8'+',
    asterisk = u8'*',
    ordered_item = u8',',
};

/**
 * @brief Stack frame used by the iterative markdown-list parser.
 */
template<::pltxt2htm::Contracts ndebug>
class MdListFrameContext {
    ::pltxt2htm::details::MdUlListItemKind item_kind;

public:
    ::std::size_t space_hierarchy;
    ::fast_io::u8string_view pltext;
    ::std::size_t current_index{};
    ::pltxt2htm::details::ListAst<ndebug> md_list_ast{};

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

    constexpr MdListFrameContext(::pltxt2htm::details::MdListFrameContext<ndebug>&&) noexcept = default;

    constexpr ~MdListFrameContext() noexcept = default;

    constexpr auto operator=(this ::pltxt2htm::details::MdListFrameContext<ndebug>& self,
                             ::pltxt2htm::details::MdListFrameContext<ndebug>&&) noexcept
        -> ::pltxt2htm::details::MdListFrameContext<ndebug>& = default;

    [[nodiscard]]
    constexpr auto get_item_kind(this ::pltxt2htm::details::MdListFrameContext<ndebug> const& self) noexcept
        -> ::pltxt2htm::details::MdUlListItemKind {
        return self.item_kind;
    }
};

/**
 * @brief Summary of the previously parsed item, used for hierarchy validation.
 */
struct PreviousItemInfo {
    ::std::size_t space_hierarchy;
    bool call_stack_is_single;
    ::pltxt2htm::details::MdUlListItemKind item_kind;
};

/**
 * @brief Validate that the current position starts a valid unordered-list item.
 * @tparam ndebug Contract checking mode.
 * @tparam item_kind The expected marker type (-, +, or *).
 * @param pltext Input text starting at the candidate marker.
 * @param space_hierarchy Number of leading whitespace characters.
 * @param expect Previous item info for hierarchy validation.
 * @return true if the marker is valid in the current list context.
 */
template<::pltxt2htm::Contracts ndebug, ::pltxt2htm::details::MdUlListItemKind item_kind>
[[nodiscard]]
constexpr auto is_valid_md_ul_list_hierarchy(
    ::fast_io::u8string_view pltext, ::std::size_t const space_hierarchy,
    ::exception::optional<::pltxt2htm::details::PreviousItemInfo> const expect) noexcept -> bool {
    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, space_hierarchy) != static_cast<char8_t>(item_kind)) {
        return false;
    }

    if ( // parsing the first line
        !expect.has_value() ||
        // e.g.
        // - test
        // - test
        //   - text <== here
        space_hierarchy > expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().space_hierarchy + 1 ||
        // e.g.
        // - test
        //   - test
        //   + test <== here
        (!expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().call_stack_is_single &&
         space_hierarchy >= expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().space_hierarchy) ||
        // e.g.
        // - test
        // - test
        // + test <== here, this line is invalid markdown list
        // Note that only the first hierarchy apply this rule
        // e.g.
        // - test
        //   - test
        //   + test <== here, this is allowed
        (expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().call_stack_is_single &&
         expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().item_kind == item_kind)) {
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

/**
 * @brief Validate that the current position starts a valid ordered-list item (e.g., "1.").
 * @tparam ndebug Contract checking mode.
 * @param pltext Input text starting at the candidate marker.
 * @param space_hierarchy Number of leading whitespace characters.
 * @param expect Previous item info for hierarchy validation.
 * @return Position after the dot if valid; nullopt otherwise.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto is_valid_md_ol_list_hierarchy(
    ::fast_io::u8string_view pltext, ::std::size_t const space_hierarchy,
    ::exception::optional<::pltxt2htm::details::PreviousItemInfo> const expect) noexcept
    -> ::exception::optional<::std::size_t> {
    ::std::size_t const pltext_size{pltext.size()};
    if (pltext_size < 4) {
        return ::exception::nullopt;
    }
    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, space_hierarchy) < u8'0' ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, space_hierarchy) > u8'9') {
        return ::exception::nullopt;
    }
    {
        ::std::size_t i{space_hierarchy + 1};
        for (; i < pltext_size; ++i) {
            auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i);
            if (chr < u8'0' || chr > u8'9') {
                break;
            }
        }
        if (i == pltext_size) {
            return ::exception::nullopt;
        }
        if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i) != u8'.') {
            return ::exception::nullopt;
        }
        if (++i >= pltext_size) {
            return ::exception::nullopt;
        }
        if ( // parsing the first line
            !expect.has_value() ||
            // e.g.
            // - test
            // - test
            //   1. text <== here
            space_hierarchy > expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().space_hierarchy + 1 ||
            // e.g.
            // - test
            //   1. test
            //   2. test <== here
            (!expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().call_stack_is_single &&
             space_hierarchy >= expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().space_hierarchy) ||
            // e.g.
            // - test
            // - test
            // 1. test <== here, this line is invalid markdown list
            (expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().call_stack_is_single &&
             expect.template value<ndebug == ::pltxt2htm::Contracts::ignore>().item_kind ==
                 ::pltxt2htm::details::MdUlListItemKind::ordered_item)) {
            return i;
        }
    }
    return ::exception::nullopt;
}

/**
 * @brief Parsed representation for a single markdown-list item candidate.
 */
struct TryParseItemResult {
    ::std::size_t space_hierarchy;
    ::std::size_t advance_count;
    ::fast_io::u8string text;
    ::pltxt2htm::details::MdUlListItemKind item_kind;
    bool checkbox{};
    bool checked{};
};

/**
 * @brief Parse a single markdown list item marker and its text content.
 * @tparam ndebug Contract checking mode.
 * @param pltext Input text starting at the candidate item position.
 * @param expect Previous item info for hierarchy validation (nullopt for first item).
 * @return Parsed item result on success; nullopt if no valid item is found.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_item(
    ::fast_io::u8string_view pltext,
    ::exception::optional<::pltxt2htm::details::PreviousItemInfo> const expect = ::exception::nullopt) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseItemResult> {
    ::std::size_t current_index{};
    // parsing spaces before - or + or *
    for (; current_index < pltext.size(); ++current_index) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if (chr != u8' ' && chr != u8'\t') {
            break;
        }
    }
    if (current_index == pltext.size()) {
        return ::exception::nullopt;
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
    }
    else if (::pltxt2htm::details::is_valid_md_ul_list_hierarchy<ndebug, ::pltxt2htm::details::MdUlListItemKind::plus>(
                 pltext, space_hierarchy, expect)) {
        item_kind = ::pltxt2htm::details::MdUlListItemKind::plus;

        ++current_index;
    }
    else if (::pltxt2htm::details::is_valid_md_ul_list_hierarchy<ndebug,
                                                                 ::pltxt2htm::details::MdUlListItemKind::asterisk>(
                 pltext, space_hierarchy, expect)) {
        item_kind = ::pltxt2htm::details::MdUlListItemKind::asterisk;
        ++current_index;
    }
    else if (auto opt_size =
                 ::pltxt2htm::details::is_valid_md_ol_list_hierarchy<ndebug>(pltext, space_hierarchy, expect);
             opt_size.has_value()) {
        item_kind = ::pltxt2htm::details::MdUlListItemKind::ordered_item;
        current_index = opt_size.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
    }
    else {
        return ::exception::nullopt;
    }

    // - or + or * must be followed by space
    if (current_index == pltext.size()) {
        return ::exception::nullopt;
    }
    if (char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};
        chr != u8' ' && chr != u8'\t') {
        return ::exception::nullopt;
    }
    // parsing spaces after - or + or *
    for (; current_index < pltext.size(); ++current_index) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if (chr != u8' ' && chr != u8'\t') {
            break;
        }
    }
    // detect markdown checkbox syntax: [ ] or [x]/[X] at start of text
    bool checkbox{};
    bool checked{};
    if (pltext.size() >= current_index + 4 &&
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index) == u8'[' &&
        (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 1) == u8' ' ||
         ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 1) == u8'x' ||
         ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 1) == u8'X') &&
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 2) == u8']' &&
        (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 3) == u8' ' ||
         ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 3) == u8'\t')) {
        checkbox = true;
        checked = (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 1) == u8'x' ||
                   ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index + 1) == u8'X');
        current_index += 4;
    }
    // parsing text after - or + or *
    ::fast_io::u8string text{};
    for (; current_index < pltext.size(); ++current_index) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if (chr == u8'\n') {
            ++current_index;
            break;
        }
        text.push_back(chr);
    }
    return ::pltxt2htm::details::TryParseItemResult{
        .space_hierarchy = space_hierarchy,
        .advance_count = current_index,
        .text = ::std::move(text),
        .item_kind = item_kind,
        .checkbox = checkbox,
        .checked = checked,
    };
}

/**
 * @brief Result of markdown-list AST conversion attempt.
 */
template<::pltxt2htm::Contracts ndebug>
struct ToListAstResult {
    ::pltxt2htm::details::ListAst<ndebug> ast;
    ::std::size_t advance_count;
    ::pltxt2htm::NodeKind item_kind;
};

/**
 * @brief Try to parse one or more consecutive markdown list lines into an intermediate AST.
 * @tparam ndebug Contract checking mode.
 * @param pltext Input text to parse.
 * @return Parsed list AST, advance count, and item kind on success; nullopt otherwise.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto optionally_to_md_list_ast(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::ToListAstResult<ndebug>> {
    ::fast_io::stack<::pltxt2htm::details::MdListFrameContext<ndebug>> call_stack{};

    // manually managing stack to avoid stack-overflow
    {
        if (auto opt_item = ::pltxt2htm::details::try_parse_item<ndebug>(pltext); opt_item.has_value()) {
            auto&& [space_hierarchy, advance_count, text, item_kind, checkbox, checked] =
                opt_item.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
            ::pltxt2htm::details::MdListFrameContext<ndebug> current_frame{item_kind, space_hierarchy, pltext,
                                                                           advance_count};
            if (checkbox) {
                current_frame.md_list_ast.emplace_back(
                    ::pltxt2htm::details::ListLiCheckboxNode(::std::move(text), checked));
            }
            else {
                current_frame.md_list_ast.emplace_back(::pltxt2htm::details::ListLiNode(::std::move(text)));
            }
            if (advance_count >= current_frame.pltext.size()) {
                return ::pltxt2htm::details::ToListAstResult<ndebug>{
                    .ast = ::std::move(current_frame.md_list_ast),
                    .advance_count = advance_count,
                    .item_kind = item_kind == ::pltxt2htm::details::MdUlListItemKind::ordered_item
                                     ? ::pltxt2htm::NodeKind::list_ol
                                     : ::pltxt2htm::NodeKind::list_ul};
            }
            call_stack.push(::std::move(current_frame));
        }
        else {
            return ::exception::nullopt;
        }
    }
    while (true) {
        auto&& top_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
        auto&& current_index = top_frame.current_index;
        auto&& result = top_frame.md_list_ast;
        ::std::size_t const pltext_size{top_frame.pltext.size()};
        auto opt_list_item = ::pltxt2htm::details::try_parse_item<ndebug>(
            ::pltxt2htm::details::u8string_view_subview<ndebug>(top_frame.pltext, current_index),
            ::pltxt2htm::details::PreviousItemInfo{.space_hierarchy = top_frame.space_hierarchy,
                                                   .call_stack_is_single = call_stack.size() == 1,
                                                   .item_kind = top_frame.get_item_kind()});
        if (opt_list_item.has_value() == false) {
            auto frame = ::std::move(top_frame);
            call_stack.pop();
            if (call_stack.empty()) {
                return ::pltxt2htm::details::ToListAstResult<ndebug>{
                    .ast = ::std::move(frame.md_list_ast),
                    .advance_count = frame.current_index,
                    .item_kind = frame.get_item_kind() == ::pltxt2htm::details::MdUlListItemKind::ordered_item
                                     ? ::pltxt2htm::NodeKind::list_ol
                                     : ::pltxt2htm::NodeKind::list_ul};
            }
            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
            switch (frame.get_item_kind()) {
            case ::pltxt2htm::details::MdUlListItemKind::ordered_item: {
                parent_frame.md_list_ast.emplace_back(
                    ::pltxt2htm::details::ListOlNode<ndebug>(::std::move(frame.md_list_ast)));
                break;
            }
            case ::pltxt2htm::details::MdUlListItemKind::hyphen:
                [[fallthrough]];
            case ::pltxt2htm::details::MdUlListItemKind::plus:
                [[fallthrough]];
            case ::pltxt2htm::details::MdUlListItemKind::asterisk: {
                parent_frame.md_list_ast.emplace_back(
                    ::pltxt2htm::details::ListUlNode<ndebug>(::std::move(frame.md_list_ast)));
                break;
            }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
            default:
                [[unlikely]] {
                    pltxt2htm_unreachable(u8"Unexpected MdUlListItemKind");
                }
#endif
            }
            parent_frame.current_index += frame.current_index;
            continue;
        }
        auto&& [space_hierarchy, advance_count, text, item_kind, checkbox, checked] =
            opt_list_item.template value<ndebug == ::pltxt2htm::Contracts::ignore>();
        current_index += advance_count;
        if (space_hierarchy > top_frame.space_hierarchy + 1) {
            call_stack.push(::pltxt2htm::details::MdListFrameContext<ndebug>{
                item_kind, space_hierarchy,
                ::pltxt2htm::details::u8string_view_subview<ndebug>(top_frame.pltext, current_index)});
            auto&& child_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
            if (checkbox) {
                child_frame.md_list_ast.emplace_back(
                    ::pltxt2htm::details::ListLiCheckboxNode(::std::move(text), checked));
            }
            else {
                child_frame.md_list_ast.emplace_back(::pltxt2htm::details::ListLiNode(::std::move(text)));
            }
            continue;
        }
        if (checkbox) {
            result.emplace_back(::pltxt2htm::details::ListLiCheckboxNode(::std::move(text), checked));
        }
        else {
            result.emplace_back(::pltxt2htm::details::ListLiNode(::std::move(text)));
        }
        top_frame.space_hierarchy = space_hierarchy;

        if (current_index < pltext_size) {
            continue;
        }
        auto frame = ::std::move(::pltxt2htm::details::stack_top<ndebug>(call_stack));
        call_stack.pop();
        if (call_stack.empty()) {
            return ::pltxt2htm::details::ToListAstResult<ndebug>{
                .ast = ::std::move(frame.md_list_ast),
                .advance_count = pltext_size,
                .item_kind = frame.get_item_kind() == ::pltxt2htm::details::MdUlListItemKind::ordered_item
                                 ? ::pltxt2htm::NodeKind::list_ol
                                 : ::pltxt2htm::NodeKind::list_ul};
        }
        auto&& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
        switch (frame.get_item_kind()) {
        case ::pltxt2htm::details::MdUlListItemKind::ordered_item: {
            parent_frame.md_list_ast.emplace_back(
                ::pltxt2htm::details::ListOlNode<ndebug>(::std::move(frame.md_list_ast)));
            break;
        }
        case ::pltxt2htm::details::MdUlListItemKind::hyphen:
            [[fallthrough]];
        case ::pltxt2htm::details::MdUlListItemKind::plus:
            [[fallthrough]];
        case ::pltxt2htm::details::MdUlListItemKind::asterisk: {
            parent_frame.md_list_ast.emplace_back(
                ::pltxt2htm::details::ListUlNode<ndebug>(::std::move(frame.md_list_ast)));
            break;
        }
#ifdef PLTXT2HTM_ENABLE_RUNTIME_EXHAUSTIVE_SWITCH_CHECK
        default:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected MdUlListItemKind");
            }
#endif
        }
        parent_frame.current_index += frame.current_index;
        continue;
    }
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
