/**
 * @file list_ast.hh
 * @brief Shared intermediate list AST used by the Markdown and HTML list scanners.
 * @details The list scanners first build a flat intermediate AST of list items
 *          (ListLiNode / ListLiCheckboxNode) and nested list containers
 *          (ListUlNode / ListOlNode); parse_pltxt then iterates it frame by frame.
 */

#pragma once

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include "../../container/string.hh"
#include "../../container/string_view.hh"
#include "../../container/vector.hh"
#include "../utils.hh"
#include "../../contracts.hh"
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
 *          output. The pre-built AST avoids this redundant work - each item is parsed
 *          exactly once.
 */
using ListAst = ::pltxt2htm::container::Vector<ListBaseNode>;

/**
 * @brief Leaf list node that stores a single list-item text payload.
 */
class ListLiNode {
    ::pltxt2htm::container::U8String text;

public:
    constexpr ListLiNode(::pltxt2htm::container::U8String&& text_) noexcept
        : text(::std::move(text_)) {
    }

    constexpr ListLiNode(ListLiNode const&) noexcept = default;

    constexpr ListLiNode(ListLiNode&&) noexcept = default;

    constexpr ~ListLiNode() noexcept = default;

    constexpr auto operator=(this ListLiNode& self, ListLiNode const& other) noexcept -> ListLiNode& = default;

    constexpr auto operator=(this ListLiNode& self, ListLiNode&&) noexcept -> ListLiNode& = default;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto operator==(this ListLiNode const& self, ListLiNode const& other) noexcept -> bool {
        return self.text == other.text;
    }

    [[nodiscard]]
    constexpr auto get_text(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.text);
    }

    [[nodiscard]]
    constexpr auto get_text_view(this ListLiNode const& self) noexcept {
        return ::pltxt2htm::container::U8StringView{self.text.data(), self.text.size()};
    }
};

/**
 * @brief Leaf list node for checkbox items (- [ ] / - [x]).
 */
class ListLiCheckboxNode {
    ::pltxt2htm::container::U8String text;
    bool checked{};

public:
    constexpr ListLiCheckboxNode(::pltxt2htm::container::U8String&& text_, bool checked_) noexcept
        : text(::std::move(text_)),
          checked(checked_) {
    }

    constexpr ListLiCheckboxNode(ListLiCheckboxNode const&) noexcept = default;

    constexpr ListLiCheckboxNode(ListLiCheckboxNode&&) noexcept = default;

    constexpr ~ListLiCheckboxNode() noexcept = default;

    constexpr auto operator=(this ListLiCheckboxNode& self, ListLiCheckboxNode const& other) noexcept
        -> ListLiCheckboxNode& = default;

    constexpr auto operator=(this ListLiCheckboxNode& self, ListLiCheckboxNode&&) noexcept
        -> ListLiCheckboxNode& = default;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto operator==(this ListLiCheckboxNode const& self, ListLiCheckboxNode const& other) noexcept -> bool {
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
    constexpr auto get_text_view(this ListLiCheckboxNode const& self) noexcept {
        return ::pltxt2htm::container::U8StringView{self.text.data(), self.text.size()};
    }
};

// ---- ListUlNode declaration (members defined after ListBaseNode) ----

/**
 * @brief Internal unordered-list node containing nested list items.
 */
class ListUlNode {
    ListAst sublist;

public:
    constexpr ListUlNode(ListAst&& sublist_) noexcept;

    constexpr ListUlNode(ListUlNode const&) noexcept = delete;

    constexpr ListUlNode(ListUlNode&&) noexcept;

    constexpr ~ListUlNode() noexcept = default;

    constexpr auto operator=(this ListUlNode& self, ListUlNode const& other) noexcept -> ListUlNode& = delete;

    constexpr auto operator=(this ListUlNode& self, ListUlNode&&) noexcept -> ListUlNode&;

    [[nodiscard]]
    constexpr auto operator==(this ListUlNode const& self, ListUlNode const& other) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_sublist(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.sublist);
    }
};

// ---- ListOlNode declaration (members defined after ListBaseNode) ----

/**
 * @brief Internal ordered-list node containing nested list items.
 */
class ListOlNode {
    ListAst sublist;
    ::std::size_t start;

public:
    constexpr ListOlNode(ListAst&& sublist_, ::std::size_t start_ = 1) noexcept;

    constexpr ListOlNode(ListOlNode const&) noexcept = delete;

    constexpr ListOlNode(ListOlNode&&) noexcept;

    constexpr ~ListOlNode() noexcept = default;

    constexpr auto operator=(this ListOlNode& self, ListOlNode const& other) noexcept -> ListOlNode& = delete;

    constexpr auto operator=(this ListOlNode& self, ListOlNode&&) noexcept -> ListOlNode&;

    [[nodiscard]]
    constexpr auto operator==(this ListOlNode const& self, ListOlNode const& other) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_sublist(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.sublist);
    }

    [[nodiscard]]
    constexpr auto get_start(this auto const& self) noexcept -> ::std::size_t {
        return self.start;
    }
};

// ---- ListBaseNode (variant-style) ----

/**
 * @brief Variant-style base type for all internal list nodes.
 */
class ListBaseNode {
    union {
        ListLiNode li_node;
        ListLiCheckboxNode li_checkbox_node;
        ListUlNode ul_node;
        ListOlNode ol_node;
    };

    ListNodeType type;

public:
    constexpr ListBaseNode(ListLiNode&& node) noexcept
        : li_node(::std::move(node)),
          type{ListNodeType::list_li} {
    }

    constexpr ListBaseNode(ListLiCheckboxNode&& node) noexcept
        : li_checkbox_node(::std::move(node)),
          type{ListNodeType::list_li_checkbox} {
    }

    constexpr ListBaseNode(ListUlNode&& node) noexcept
        : ul_node(::std::move(node)),
          type{ListNodeType::list_ul} {
    }

    constexpr ListBaseNode(ListOlNode&& node) noexcept
        : ol_node(::std::move(node)),
          type{ListNodeType::list_ol} {
    }

    constexpr ListBaseNode(ListBaseNode const&) noexcept = delete;

    constexpr ListBaseNode(ListBaseNode&& other) noexcept
        : type(other.type) {
        switch (type) /* -Werror=switch */ {
        case ListNodeType::list_li: {
            new (::std::addressof(li_node)) ListLiNode(::std::move(other.li_node));
            break;
        }
        case ListNodeType::list_li_checkbox: {
            new (::std::addressof(li_checkbox_node)) ListLiCheckboxNode(::std::move(other.li_checkbox_node));
            break;
        }
        case ListNodeType::list_ul: {
            new (::std::addressof(ul_node)) ListUlNode(::std::move(other.ul_node));
            break;
        }
        case ListNodeType::list_ol: {
            new (::std::addressof(ol_node)) ListOlNode(::std::move(other.ol_node));
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
        case ListNodeType::list_li: {
            li_node.~ListLiNode();
            break;
        }
        case ListNodeType::list_li_checkbox: {
            li_checkbox_node.~ListLiCheckboxNode();
            break;
        }
        case ListNodeType::list_ul: {
            ul_node.~ListUlNode();
            break;
        }
        case ListNodeType::list_ol: {
            ol_node.~ListOlNode();
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

    constexpr auto operator=(this ListBaseNode& self, ListBaseNode const& other) noexcept -> ListBaseNode& = delete;

    constexpr auto operator=(this ListBaseNode& self, ListBaseNode&& other) noexcept -> ListBaseNode& {
        if (::std::addressof(other) != ::std::addressof(self)) {
            self.~ListBaseNode();
            ::std::construct_at(::std::addressof(self), ::std::move(other));
        }
        return self;
    }

    [[nodiscard]]
    constexpr auto get_type(this ListBaseNode const& self) noexcept -> ListNodeType {
        return self.type;
    }

    [[nodiscard]]
    constexpr auto get_text(this auto&& self) noexcept -> decltype(auto) {
        switch (self.type) /* -Werror=switch */ {
        case ListNodeType::list_li: {
            return ::std::forward_like<decltype(self)>(self.li_node).get_text();
        }
        case ListNodeType::list_li_checkbox: {
            return ::std::forward_like<decltype(self)>(self.li_checkbox_node).get_text();
        }
        case ListNodeType::list_ul:
            [[fallthrough]];
        case ListNodeType::list_ol:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected list_ul/list_ol in get_text()");
            }
        }
        pltxt2htm_unreachable(u8"Unreachable after get_text() switch");
    }

    [[nodiscard]]
    constexpr auto get_text_view(this ListBaseNode const& self) noexcept -> ::pltxt2htm::container::U8StringView {
        switch (self.type) /* -Werror=switch */ {
        case ListNodeType::list_li: {
            return self.li_node.get_text_view();
        }
        case ListNodeType::list_li_checkbox: {
            return self.li_checkbox_node.get_text_view();
        }
        case ListNodeType::list_ul:
            [[fallthrough]];
        case ListNodeType::list_ol:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected list_ul/list_ol in get_text_view()");
            }
        }
        pltxt2htm_unreachable(u8"Unreachable after get_text_view() switch");
    }

    [[nodiscard]]
    constexpr auto get_sublist(this auto&& self) noexcept -> decltype(auto) {
        switch (self.type) /* -Werror=switch */ {
        case ListNodeType::list_ul: {
            return ::std::forward_like<decltype(self)>(self.ul_node).get_sublist();
        }
        case ListNodeType::list_ol: {
            return ::std::forward_like<decltype(self)>(self.ol_node).get_sublist();
        }
        case ListNodeType::list_li:
            [[fallthrough]];
        case ListNodeType::list_li_checkbox:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected list_li/list_li_checkbox in get_sublist()");
            }
        }
        pltxt2htm_unreachable(u8"Unreachable after get_sublist() switch");
    }

    [[nodiscard]]
    constexpr auto get_start(this auto const& self) noexcept -> ::std::size_t {
        switch (self.type) /* -Werror=switch */ {
        case ListNodeType::list_ol: {
            return self.ol_node.get_start();
        }
        case ListNodeType::list_ul: {
            return 1;
        }
        case ListNodeType::list_li:
            [[fallthrough]];
        case ListNodeType::list_li_checkbox:
            [[unlikely]] {
                pltxt2htm_unreachable(u8"Unexpected list_li/list_li_checkbox in get_start()");
            }
        }
        pltxt2htm_unreachable(u8"Unreachable after get_start() switch");
    }

    [[nodiscard]]
    constexpr auto is_checked(this auto const& self) noexcept -> bool {
        pltxt2htm_assert(self.type == ListNodeType::list_li_checkbox, u8"node type mismatch");
        return self.li_checkbox_node.is_checked();
    }

    [[nodiscard]]
    friend constexpr auto operator==(ListBaseNode const& self, ListBaseNode const& other) noexcept -> bool {
        if (self.type != other.type) {
            return false;
        }

        switch (self.type) {
        case ListNodeType::list_li: {
            return self.li_node == other.li_node;
        }
        case ListNodeType::list_li_checkbox: {
            return self.li_checkbox_node == other.li_checkbox_node;
        }
        case ListNodeType::list_ul: {
            return self.ul_node == other.ul_node;
        }
        case ListNodeType::list_ol: {
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

constexpr ListUlNode::ListUlNode(ListAst&& sublist_) noexcept
    : sublist(::std::move(sublist_)) {
}

constexpr ListUlNode::ListUlNode(ListUlNode&&) noexcept = default;

constexpr auto ListUlNode::operator=(this ListUlNode& self, ListUlNode&& other) noexcept -> ListUlNode& = default;

[[nodiscard]]
constexpr auto ListUlNode::operator==(this ListUlNode const& self, ListUlNode const& other) noexcept -> bool {
    return self.sublist == other.sublist;
}

// ---- ListOlNode member definitions (ListBaseNode is now complete) ----

constexpr ListOlNode::ListOlNode(ListAst&& sublist_, ::std::size_t start_) noexcept
    : sublist(::std::move(sublist_)),
      start(start_) {
}

constexpr ListOlNode::ListOlNode(ListOlNode&&) noexcept = default;

constexpr auto ListOlNode::operator=(this ListOlNode& self, ListOlNode&& other) noexcept -> ListOlNode& = default;

[[nodiscard]]
constexpr auto ListOlNode::operator==(this ListOlNode const& self, ListOlNode const& other) noexcept -> bool {
    return self.sublist == other.sublist && self.start == other.start;
}

/**
 * @brief Concept matching the concrete node types stored in ListBaseNode.
 */
template<typename T>
concept is_list_node_type =
    ::std::is_same_v<::std::remove_cvref_t<T>, ListLiNode> ||
    ::std::is_same_v<::std::remove_cvref_t<T>, ListLiCheckboxNode> ||
    ::std::is_same_v<::std::remove_cvref_t<T>, ListUlNode> || ::std::is_same_v<::std::remove_cvref_t<T>, ListOlNode>;

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
