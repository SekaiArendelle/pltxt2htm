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
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <fast_io/fast_io_dsal/vector.h>
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
using ListAst = ::fast_io::vector<ListBaseNode<ndebug>>;

/**
 * @brief Leaf list node that stores a single list-item text payload.
 */
class ListLiNode {
    ::fast_io::u8string text;

public:
    constexpr ListLiNode(::fast_io::u8string&& text_) noexcept
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
        return ::fast_io::u8string_view{self.text.data(), self.text.size()};
    }
};

// ---- ListUlNode declaration (members defined after ListBaseNode) ----

/**
 * @brief Internal unordered-list node containing nested list items.
 */
template<::pltxt2htm::Contracts ndebug>
class ListUlNode {
    ListAst<ndebug> sublist;

public:
    constexpr ListUlNode(ListAst<ndebug>&& sublist_) noexcept;

    constexpr ListUlNode(ListUlNode<ndebug> const&) noexcept = delete;

    constexpr ListUlNode(ListUlNode<ndebug>&&) noexcept;

    constexpr ~ListUlNode() noexcept;

    constexpr auto operator=(this ListUlNode<ndebug>& self, ListUlNode<ndebug> const& other) noexcept
        -> ListUlNode<ndebug>& = delete;

    constexpr auto operator=(this ListUlNode<ndebug>& self, ListUlNode<ndebug>&&) noexcept -> ListUlNode<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(ListUlNode<ndebug> const& other) const noexcept -> bool;

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
    ListAst<ndebug> sublist;
    ::std::size_t start;

public:
    constexpr ListOlNode(ListAst<ndebug>&& sublist_, ::std::size_t start_ = 1) noexcept;

    constexpr ListOlNode(ListOlNode<ndebug> const&) noexcept = delete;

    constexpr ListOlNode(ListOlNode<ndebug>&&) noexcept;

    constexpr ~ListOlNode() noexcept;

    constexpr auto operator=(this ListOlNode<ndebug>& self, ListOlNode<ndebug> const& other) noexcept
        -> ListOlNode<ndebug>& = delete;

    constexpr auto operator=(this ListOlNode<ndebug>& self, ListOlNode<ndebug>&&) noexcept -> ListOlNode<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(ListOlNode<ndebug> const& other) const noexcept -> bool;

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
template<::pltxt2htm::Contracts ndebug>
class ListBaseNode {
    union {
        ListLiNode li_node;
        ListLiCheckboxNode li_checkbox_node;
        ListUlNode<ndebug> ul_node;
        ListOlNode<ndebug> ol_node;
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

    constexpr ListBaseNode(ListUlNode<ndebug>&& node) noexcept
        : ul_node(::std::move(node)),
          type{ListNodeType::list_ul} {
    }

    constexpr ListBaseNode(ListOlNode<ndebug>&& node) noexcept
        : ol_node(::std::move(node)),
          type{ListNodeType::list_ol} {
    }

    constexpr ListBaseNode(ListBaseNode<ndebug> const&) noexcept = delete;

    constexpr ListBaseNode(ListBaseNode<ndebug>&& other) noexcept
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
            new (::std::addressof(ul_node)) ListUlNode<ndebug>(::std::move(other.ul_node));
            break;
        }
        case ListNodeType::list_ol: {
            new (::std::addressof(ol_node)) ListOlNode<ndebug>(::std::move(other.ol_node));
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
            ul_node.~ListUlNode<ndebug>();
            break;
        }
        case ListNodeType::list_ol: {
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

    constexpr auto operator=(this ListBaseNode<ndebug>& self, ListBaseNode<ndebug> const& other) noexcept
        -> ListBaseNode<ndebug>& = delete;

    constexpr auto operator=(this ListBaseNode<ndebug>& self, ListBaseNode<ndebug>&& other) noexcept
        -> ListBaseNode<ndebug>& {
        if (::std::addressof(other) != ::std::addressof(self)) {
            self.~ListBaseNode();
            ::std::construct_at(::std::addressof(self), ::std::move(other));
        }
        return self;
    }

    [[nodiscard]]
    constexpr auto get_type(this ListBaseNode<ndebug> const& self) noexcept -> ListNodeType {
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
    constexpr auto get_text_view(this ListBaseNode<ndebug> const& self) noexcept -> ::fast_io::u8string_view {
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
    friend constexpr auto operator==(ListBaseNode<ndebug> const& self, ListBaseNode<ndebug> const& other) noexcept
        -> bool {
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

template<::pltxt2htm::Contracts ndebug>
constexpr ListUlNode<ndebug>::ListUlNode(ListAst<ndebug>&& sublist_) noexcept
    : sublist(::std::move(sublist_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ListUlNode<ndebug>::ListUlNode(ListUlNode<ndebug>&&) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ListUlNode<ndebug>::~ListUlNode() noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ListUlNode<ndebug>::operator=(this ListUlNode<ndebug>& self, ListUlNode<ndebug>&& other) noexcept
    -> ListUlNode<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto ListUlNode<ndebug>::operator==(ListUlNode<ndebug> const& other) const noexcept -> bool {
    return sublist == other.sublist;
}

// ---- ListOlNode member definitions (ListBaseNode is now complete) ----

template<::pltxt2htm::Contracts ndebug>
constexpr ListOlNode<ndebug>::ListOlNode(ListAst<ndebug>&& sublist_, ::std::size_t start_) noexcept
    : sublist(::std::move(sublist_)),
      start(start_) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr ListOlNode<ndebug>::ListOlNode(ListOlNode<ndebug>&&) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr ListOlNode<ndebug>::~ListOlNode() noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto ListOlNode<ndebug>::operator=(this ListOlNode<ndebug>& self, ListOlNode<ndebug>&& other) noexcept
    -> ListOlNode<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto ListOlNode<ndebug>::operator==(ListOlNode<ndebug> const& other) const noexcept -> bool {
    return sublist == other.sublist && start == other.start;
}

template<typename T>
constexpr bool is_list_ul_node_ = false;

template<::pltxt2htm::Contracts ndebug>
constexpr bool is_list_ul_node_<ListUlNode<ndebug>> = true;

template<typename T>
constexpr bool is_list_ol_node_ = false;

template<::pltxt2htm::Contracts ndebug>
constexpr bool is_list_ol_node_<ListOlNode<ndebug>> = true;

/**
 * @brief Concept matching the concrete node types stored in ListBaseNode.
 */
template<typename T>
concept is_list_node_type = ::std::is_same_v<::std::remove_cvref_t<T>, ListLiNode> ||
                            ::std::is_same_v<::std::remove_cvref_t<T>, ListLiCheckboxNode> ||
                            ::pltxt2htm::details::is_list_ul_node_<::std::remove_cvref_t<T>> ||
                            ::pltxt2htm::details::is_list_ol_node_<::std::remove_cvref_t<T>>;

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
