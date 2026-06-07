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
 * @brief Internal markdown-list AST node discriminator.
 */
enum class MdListNodeType : ::std::uint_least32_t {
    md_li = 0,
    md_li_checkbox,
    md_ul,
    md_ol,
};

// Forward declaration for recursive MdListAst
template<::pltxt2htm::Contracts ndebug>
class MdListBaseNode;

/**
 * @brief Internal markdown-list AST container type.
 *
 * @details The intermediate MdListAst is necessary because parsing a list requires a
 *          two-phase approach: first, `optionally_to_md_list_ast` calls `try_parse_item`
 *          once per item to build the full nested AST (determining hierarchy boundaries,
 *          marker types, and extracting item text). Second, `parse_pltxt` iterates the
 *          pre-built AST without re-parsing.
 *
 *          Without MdListAst, `parse_pltxt` would need to call `try_parse_item` twice
 *          for each item: once during a pre-scan (to determine the list boundary / item
 *          hierarchy before creating child frames) and again when actually producing the
 *          output. The pre-built AST avoids this redundant work — each item is parsed
 *          exactly once.
 */
template<::pltxt2htm::Contracts ndebug>
using MdListAst = ::fast_io::vector<::pltxt2htm::details::MdListBaseNode<ndebug>>;

/**
 * @brief Leaf markdown-list node that stores a single list-item text payload.
 */
class MdListLiNode {
    ::fast_io::u8string text;

public:
    constexpr MdListLiNode(::fast_io::u8string&& text_) noexcept
        : text(::std::move(text_)) {
    }

    constexpr MdListLiNode(::pltxt2htm::details::MdListLiNode const&) noexcept = default;

    constexpr MdListLiNode(::pltxt2htm::details::MdListLiNode&&) noexcept = default;

    constexpr ~MdListLiNode() noexcept = default;

    constexpr auto operator=(this ::pltxt2htm::details::MdListLiNode& self,
                             ::pltxt2htm::details::MdListLiNode const& other) noexcept
        -> ::pltxt2htm::details::MdListLiNode& = default;

    constexpr auto operator=(this ::pltxt2htm::details::MdListLiNode& self,
                             ::pltxt2htm::details::MdListLiNode&&) noexcept
        -> ::pltxt2htm::details::MdListLiNode& = default;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto operator==(this ::pltxt2htm::details::MdListLiNode const& self,
                              ::pltxt2htm::details::MdListLiNode const& other) noexcept -> bool {
        return self.text == other.text;
    }

    [[nodiscard]]
    constexpr auto get_text(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.text);
    }

    [[nodiscard]]
    constexpr auto get_text_view(this ::pltxt2htm::details::MdListLiNode const& self) noexcept {
        return ::fast_io::u8string_view{self.text.data(), self.text.size()};
    }
};

/**
 * @brief Leaf markdown-list node for checkbox items (- [ ] / - [x]).
 */
class MdListLiCheckboxNode {
    ::fast_io::u8string text;
    bool checked_{};

public:
    constexpr MdListLiCheckboxNode(::fast_io::u8string&& text_, bool checked) noexcept
        : text(::std::move(text_)),
          checked_(checked) {
    }

    constexpr MdListLiCheckboxNode(::pltxt2htm::details::MdListLiCheckboxNode const&) noexcept = default;

    constexpr MdListLiCheckboxNode(::pltxt2htm::details::MdListLiCheckboxNode&&) noexcept = default;

    constexpr ~MdListLiCheckboxNode() noexcept = default;

    constexpr auto operator=(this ::pltxt2htm::details::MdListLiCheckboxNode& self,
                             ::pltxt2htm::details::MdListLiCheckboxNode const& other) noexcept
        -> ::pltxt2htm::details::MdListLiCheckboxNode& = default;

    constexpr auto operator=(this ::pltxt2htm::details::MdListLiCheckboxNode& self,
                             ::pltxt2htm::details::MdListLiCheckboxNode&&) noexcept
        -> ::pltxt2htm::details::MdListLiCheckboxNode& = default;

    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    constexpr auto operator==(this ::pltxt2htm::details::MdListLiCheckboxNode const& self,
                              ::pltxt2htm::details::MdListLiCheckboxNode const& other) noexcept -> bool {
        return self.text == other.text && self.checked_ == other.checked_;
    }

    [[nodiscard]]
    constexpr auto is_checked(this auto const& self) noexcept -> bool {
        return self.checked_;
    }

    [[nodiscard]]
    constexpr auto get_text(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.text);
    }

    [[nodiscard]]
    constexpr auto get_text_view(this ::pltxt2htm::details::MdListLiCheckboxNode const& self) noexcept {
        return ::fast_io::u8string_view{self.text.data(), self.text.size()};
    }
};

// ---- MdListUlNode declaration (members defined after MdListBaseNode) ----

/**
 * @brief Internal unordered-list node containing nested list items.
 */
template<::pltxt2htm::Contracts ndebug>
class MdListUlNode {
    ::pltxt2htm::details::MdListAst<ndebug> sublist;

public:
    constexpr MdListUlNode(::pltxt2htm::details::MdListAst<ndebug>&& sublist_) noexcept;

    constexpr MdListUlNode(::pltxt2htm::details::MdListUlNode<ndebug> const&) noexcept = delete;

    constexpr MdListUlNode(::pltxt2htm::details::MdListUlNode<ndebug>&&) noexcept;

    constexpr ~MdListUlNode() noexcept;

    constexpr auto operator=(this ::pltxt2htm::details::MdListUlNode<ndebug>& self,
                             ::pltxt2htm::details::MdListUlNode<ndebug> const& other) noexcept
        -> ::pltxt2htm::details::MdListUlNode<ndebug>& = delete;

    constexpr auto operator=(this ::pltxt2htm::details::MdListUlNode<ndebug>& self,
                             ::pltxt2htm::details::MdListUlNode<ndebug>&&) noexcept
        -> ::pltxt2htm::details::MdListUlNode<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(::pltxt2htm::details::MdListUlNode<ndebug> const& other) const noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_sublist(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.sublist);
    }
};

// ---- MdListOlNode declaration (members defined after MdListBaseNode) ----

/**
 * @brief Internal ordered-list node containing nested list items.
 */
template<::pltxt2htm::Contracts ndebug>
class MdListOlNode {
    ::pltxt2htm::details::MdListAst<ndebug> sublist;

public:
    constexpr MdListOlNode(::pltxt2htm::details::MdListAst<ndebug>&& sublist_) noexcept;

    constexpr MdListOlNode(::pltxt2htm::details::MdListOlNode<ndebug> const&) noexcept = delete;

    constexpr MdListOlNode(::pltxt2htm::details::MdListOlNode<ndebug>&&) noexcept;

    constexpr ~MdListOlNode() noexcept;

    constexpr auto operator=(this ::pltxt2htm::details::MdListOlNode<ndebug>& self,
                             ::pltxt2htm::details::MdListOlNode<ndebug> const& other) noexcept
        -> ::pltxt2htm::details::MdListOlNode<ndebug>& = delete;

    constexpr auto operator=(this ::pltxt2htm::details::MdListOlNode<ndebug>& self,
                             ::pltxt2htm::details::MdListOlNode<ndebug>&&) noexcept
        -> ::pltxt2htm::details::MdListOlNode<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(::pltxt2htm::details::MdListOlNode<ndebug> const& other) const noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_sublist(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.sublist);
    }
};

// ---- MdListBaseNode (variant-style) ----

/**
 * @brief Variant-style base type for all internal markdown-list nodes.
 */
template<::pltxt2htm::Contracts ndebug>
class MdListBaseNode {
    union {
        ::pltxt2htm::details::MdListLiNode li_node;
        ::pltxt2htm::details::MdListLiCheckboxNode li_checkbox_node;
        ::pltxt2htm::details::MdListUlNode<ndebug> ul_node;
        ::pltxt2htm::details::MdListOlNode<ndebug> ol_node;
    };

    ::pltxt2htm::details::MdListNodeType type_;

public:
    constexpr MdListBaseNode() noexcept = delete;

    constexpr MdListBaseNode(::pltxt2htm::details::MdListLiNode&& node) noexcept
        : li_node(::std::move(node)),
          type_{::pltxt2htm::details::MdListNodeType::md_li} {
    }

    constexpr MdListBaseNode(::pltxt2htm::details::MdListLiCheckboxNode&& node) noexcept
        : li_checkbox_node(::std::move(node)),
          type_{::pltxt2htm::details::MdListNodeType::md_li_checkbox} {
    }

    constexpr MdListBaseNode(::pltxt2htm::details::MdListUlNode<ndebug>&& node) noexcept
        : ul_node(::std::move(node)),
          type_{::pltxt2htm::details::MdListNodeType::md_ul} {
    }

    constexpr MdListBaseNode(::pltxt2htm::details::MdListOlNode<ndebug>&& node) noexcept
        : ol_node(::std::move(node)),
          type_{::pltxt2htm::details::MdListNodeType::md_ol} {
    }

    constexpr MdListBaseNode(::pltxt2htm::details::MdListBaseNode<ndebug> const&) noexcept = delete;

    constexpr MdListBaseNode(::pltxt2htm::details::MdListBaseNode<ndebug>&& other) noexcept
        : type_(other.type_) {
        switch (type_) /* -Werror=switch */ {
        case ::pltxt2htm::details::MdListNodeType::md_li: {
            new (::std::addressof(li_node))::pltxt2htm::details::MdListLiNode(::std::move(other.li_node));
            break;
        }
        case ::pltxt2htm::details::MdListNodeType::md_li_checkbox: {
            new (::std::addressof(li_checkbox_node))::pltxt2htm::details::MdListLiCheckboxNode(
                ::std::move(other.li_checkbox_node));
            break;
        }
        case ::pltxt2htm::details::MdListNodeType::md_ul: {
            new (::std::addressof(ul_node))::pltxt2htm::details::MdListUlNode<ndebug>(::std::move(other.ul_node));
            break;
        }
        case ::pltxt2htm::details::MdListNodeType::md_ol: {
            new (::std::addressof(ol_node))::pltxt2htm::details::MdListOlNode<ndebug>(::std::move(other.ol_node));
            break;
        }
#if 0
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
#endif
        }
    }

    constexpr ~MdListBaseNode() noexcept {
        switch (type_) /* -Werror=switch */ {
        case ::pltxt2htm::details::MdListNodeType::md_li: {
            li_node.~MdListLiNode();
            break;
        }
        case ::pltxt2htm::details::MdListNodeType::md_li_checkbox: {
            li_checkbox_node.~MdListLiCheckboxNode();
            break;
        }
        case ::pltxt2htm::details::MdListNodeType::md_ul: {
            ul_node.~MdListUlNode<ndebug>();
            break;
        }
        case ::pltxt2htm::details::MdListNodeType::md_ol: {
            ol_node.~MdListOlNode<ndebug>();
            break;
        }
#if 0
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
#endif
        }
    }

    constexpr auto operator=(this ::pltxt2htm::details::MdListBaseNode<ndebug>& self,
                             ::pltxt2htm::details::MdListBaseNode<ndebug> const& other) noexcept
        -> ::pltxt2htm::details::MdListBaseNode<ndebug>& = delete;

    constexpr auto operator=(this ::pltxt2htm::details::MdListBaseNode<ndebug>& self,
                             ::pltxt2htm::details::MdListBaseNode<ndebug>&& other) noexcept
        -> ::pltxt2htm::details::MdListBaseNode<ndebug>& {
        if (::std::addressof(other) != ::std::addressof(self)) {
            self.~MdListBaseNode();
            ::std::construct_at(::std::addressof(self), ::std::move(other));
        }
        return self;
    }

    [[nodiscard]]
    constexpr auto get_type(this auto&& self) noexcept -> ::pltxt2htm::details::MdListNodeType {
        return self.type_;
    }

    [[nodiscard]]
    constexpr auto get_text(this auto&& self) noexcept -> decltype(auto) {
        switch (self.type_) /* -Werror=switch */ {
        case ::pltxt2htm::details::MdListNodeType::md_li: {
            return ::std::forward_like<decltype(self)>(self.li_node).get_text();
        }
        case ::pltxt2htm::details::MdListNodeType::md_li_checkbox: {
            return ::std::forward_like<decltype(self)>(self.li_checkbox_node).get_text();
        }
        case ::pltxt2htm::details::MdListNodeType::md_ul:
            [[fallthrough]];
        case ::pltxt2htm::details::MdListNodeType::md_ol:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
        ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
    }

    [[nodiscard]]
    constexpr auto get_text_view(this auto&& self) noexcept -> ::fast_io::u8string_view {
        switch (self.type_) /* -Werror=switch */ {
        case ::pltxt2htm::details::MdListNodeType::md_li: {
            return self.li_node.get_text_view();
        }
        case ::pltxt2htm::details::MdListNodeType::md_li_checkbox: {
            return self.li_checkbox_node.get_text_view();
        }
        case ::pltxt2htm::details::MdListNodeType::md_ul:
            [[fallthrough]];
        case ::pltxt2htm::details::MdListNodeType::md_ol:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
        ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
    }

    [[nodiscard]]
    constexpr auto get_sublist(this auto&& self) noexcept -> decltype(auto) {
        switch (self.type_) /* -Werror=switch */ {
        case ::pltxt2htm::details::MdListNodeType::md_ul: {
            return ::std::forward_like<decltype(self)>(self.ul_node).get_sublist();
        }
        case ::pltxt2htm::details::MdListNodeType::md_ol: {
            return ::std::forward_like<decltype(self)>(self.ol_node).get_sublist();
        }
        case ::pltxt2htm::details::MdListNodeType::md_li:
            [[fallthrough]];
        case ::pltxt2htm::details::MdListNodeType::md_li_checkbox:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
        }
        ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
    }

    [[nodiscard]]
    constexpr auto is_checked(this auto const& self) noexcept -> bool {
        pltxt2htm_assert(self.type_ == ::pltxt2htm::details::MdListNodeType::md_li_checkbox, u8"node type mismatch");
        return self.li_checkbox_node.is_checked();
    }

    [[nodiscard]]
    friend constexpr auto operator==(::pltxt2htm::details::MdListBaseNode<ndebug> const& self,
                                     ::pltxt2htm::details::MdListBaseNode<ndebug> const& other) noexcept -> bool {
        if (self.type_ != other.type_) {
            return false;
        }

        switch (self.type_) {
        case ::pltxt2htm::details::MdListNodeType::md_li: {
            return self.li_node == other.li_node;
        }
        case ::pltxt2htm::details::MdListNodeType::md_li_checkbox: {
            return self.li_checkbox_node == other.li_checkbox_node;
        }
        case ::pltxt2htm::details::MdListNodeType::md_ul: {
            return self.ul_node == other.ul_node;
        }
        case ::pltxt2htm::details::MdListNodeType::md_ol: {
            return self.ol_node == other.ol_node;
        }
#if 0
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
#endif
        }
        ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
    }
};

// ---- MdListUlNode member definitions (MdListBaseNode is now complete) ----

template<::pltxt2htm::Contracts ndebug>
constexpr MdListUlNode<ndebug>::MdListUlNode(::pltxt2htm::details::MdListAst<ndebug>&& sublist_) noexcept
    : sublist(::std::move(sublist_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdListUlNode<ndebug>::MdListUlNode(::pltxt2htm::details::MdListUlNode<ndebug>&&) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdListUlNode<ndebug>::~MdListUlNode() noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto MdListUlNode<ndebug>::operator=(this ::pltxt2htm::details::MdListUlNode<ndebug>& self,
                                               ::pltxt2htm::details::MdListUlNode<ndebug>&& other) noexcept
    -> ::pltxt2htm::details::MdListUlNode<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto MdListUlNode<ndebug>::operator==(::pltxt2htm::details::MdListUlNode<ndebug> const& other) const noexcept
    -> bool {
    return sublist == other.sublist;
}

// ---- MdListOlNode member definitions (MdListBaseNode is now complete) ----

template<::pltxt2htm::Contracts ndebug>
constexpr MdListOlNode<ndebug>::MdListOlNode(::pltxt2htm::details::MdListAst<ndebug>&& sublist_) noexcept
    : sublist(::std::move(sublist_)) {
}

template<::pltxt2htm::Contracts ndebug>
constexpr MdListOlNode<ndebug>::MdListOlNode(::pltxt2htm::details::MdListOlNode<ndebug>&&) noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr MdListOlNode<ndebug>::~MdListOlNode() noexcept = default;

template<::pltxt2htm::Contracts ndebug>
constexpr auto MdListOlNode<ndebug>::operator=(this ::pltxt2htm::details::MdListOlNode<ndebug>& self,
                                               ::pltxt2htm::details::MdListOlNode<ndebug>&& other) noexcept
    -> ::pltxt2htm::details::MdListOlNode<ndebug>& = default;

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto MdListOlNode<ndebug>::operator==(::pltxt2htm::details::MdListOlNode<ndebug> const& other) const noexcept
    -> bool {
    return sublist == other.sublist;
}

template<typename T>
constexpr bool is_md_list_ul_node_ = false;

template<::pltxt2htm::Contracts ndebug>
constexpr bool is_md_list_ul_node_<::pltxt2htm::details::MdListUlNode<ndebug>> = true;

template<typename T>
constexpr bool is_md_list_ol_node_ = false;

template<::pltxt2htm::Contracts ndebug>
constexpr bool is_md_list_ol_node_<::pltxt2htm::details::MdListOlNode<ndebug>> = true;

/**
 * @brief Concept matching the concrete node types stored in MdListBaseNode.
 */
template<typename T>
concept is_md_list_node_type = ::std::is_same_v<::std::remove_cvref_t<T>, ::pltxt2htm::details::MdListLiNode> ||
                               ::std::is_same_v<::std::remove_cvref_t<T>, ::pltxt2htm::details::MdListLiCheckboxNode> ||
                               ::pltxt2htm::details::is_md_list_ul_node_<::std::remove_cvref_t<T>> ||
                               ::pltxt2htm::details::is_md_list_ol_node_<::std::remove_cvref_t<T>>;

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
    ::pltxt2htm::details::MdListAst<ndebug> md_list_ast{};

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

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto is_valid_md_ol_list_hierarchy(
    ::fast_io::u8string_view pltext, ::std::size_t const space_hierarchy,
    ::exception::optional<::pltxt2htm::details::PreviousItemInfo> const expect) noexcept
    -> ::exception::optional<::std::size_t> {
    ::std::size_t pltext_size{pltext.size()};
    if (pltext_size < 4) {
        return ::exception::nullopt_t{};
    }
    if (::pltxt2htm::details::u8string_view_index<ndebug>(pltext, space_hierarchy) < u8'0' ||
        ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, space_hierarchy) > u8'9') {
        return ::exception::nullopt_t{};
    }
    {
        ::std::size_t i{space_hierarchy + 1};
        for (; i < pltext_size; ++i) {
            auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, i);
            if (chr < u8'0' || chr > u8'9') {
                break;
            }
        }
        if (i == pltext_size) {
            return ::exception::nullopt_t{};
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
    return ::exception::nullopt_t{};
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

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_item(
    ::fast_io::u8string_view pltext,
    ::exception::optional<::pltxt2htm::details::PreviousItemInfo> const expect = ::exception::nullopt_t{}) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseItemResult> {
    ::std::size_t current_index{};
    // parsing spaces before - or + or *
    for (; current_index < pltext.size(); ++current_index) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
        if (chr != u8' ' && chr != u8'\t') {
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
        return ::exception::nullopt_t{};
    }

    // - or + or * must be followed by space
    if (current_index == pltext.size()) {
        return ::exception::nullopt_t{};
    }
    if (char8_t const chr{::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index)};
        chr != u8' ' && chr != u8'\t') {
        return ::exception::nullopt_t{};
    }
    // parsing spaces after - or + or *
    for (; current_index < pltext.size(); ++current_index) {
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
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
        auto chr = ::pltxt2htm::details::u8string_view_index<ndebug>(pltext, current_index);
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
struct ToMdListAstResult {
    ::pltxt2htm::details::MdListAst<ndebug> ast;
    ::std::size_t advance_count;
    ::pltxt2htm::NodeKind item_kind;
};

template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto optionally_to_md_list_ast(::fast_io::u8string_view pltext) noexcept
    -> ::exception::optional<::pltxt2htm::details::ToMdListAstResult<ndebug>> {
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
                    ::pltxt2htm::details::MdListLiCheckboxNode(::std::move(text), checked));
            }
            else {
                current_frame.md_list_ast.emplace_back(::pltxt2htm::details::MdListLiNode(::std::move(text)));
            }
            if (advance_count >= current_frame.pltext.size()) {
                return ::pltxt2htm::details::ToMdListAstResult<ndebug>{
                    .ast = ::std::move(current_frame.md_list_ast),
                    .advance_count = advance_count,
                    .item_kind = item_kind == ::pltxt2htm::details::MdUlListItemKind::ordered_item
                                     ? ::pltxt2htm::NodeKind::md_ol
                                     : ::pltxt2htm::NodeKind::md_ul};
            }
            call_stack.push(::std::move(current_frame));
        }
        else {
            return ::exception::nullopt_t{};
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
        if (!opt_list_item.has_value()) {
            auto frame = ::std::move(top_frame);
            call_stack.pop();
            if (call_stack.empty()) {
                return ::pltxt2htm::details::ToMdListAstResult<ndebug>{
                    .ast = ::std::move(frame.md_list_ast),
                    .advance_count = frame.current_index,
                    .item_kind = frame.get_item_kind() == ::pltxt2htm::details::MdUlListItemKind::ordered_item
                                     ? ::pltxt2htm::NodeKind::md_ol
                                     : ::pltxt2htm::NodeKind::md_ul};
            }
            auto& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
            switch (frame.get_item_kind()) {
            case ::pltxt2htm::details::MdUlListItemKind::ordered_item: {
                parent_frame.md_list_ast.emplace_back(
                    ::pltxt2htm::details::MdListOlNode<ndebug>(::std::move(frame.md_list_ast)));
                break;
            }
            case ::pltxt2htm::details::MdUlListItemKind::hyphen:
                [[fallthrough]];
            case ::pltxt2htm::details::MdUlListItemKind::plus:
                [[fallthrough]];
            case ::pltxt2htm::details::MdUlListItemKind::asterisk: {
                parent_frame.md_list_ast.emplace_back(
                    ::pltxt2htm::details::MdListUlNode<ndebug>(::std::move(frame.md_list_ast)));
                break;
            }
#if 0
            default:
                [[unlikely]] {
                    ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
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
                    ::pltxt2htm::details::MdListLiCheckboxNode(::std::move(text), checked));
            }
            else {
                child_frame.md_list_ast.emplace_back(::pltxt2htm::details::MdListLiNode(::std::move(text)));
            }
            continue;
        }
        if (checkbox) {
            result.emplace_back(::pltxt2htm::details::MdListLiCheckboxNode(::std::move(text), checked));
        }
        else {
            result.emplace_back(::pltxt2htm::details::MdListLiNode(::std::move(text)));
        }
        top_frame.space_hierarchy = space_hierarchy;

        if (current_index < pltext_size) {
            continue;
        }
        auto frame = ::std::move(::pltxt2htm::details::stack_top<ndebug>(call_stack));
        call_stack.pop();
        if (call_stack.empty()) {
            return ::pltxt2htm::details::ToMdListAstResult<ndebug>{
                .ast = ::std::move(frame.md_list_ast),
                .advance_count = pltext_size,
                .item_kind = frame.get_item_kind() == ::pltxt2htm::details::MdUlListItemKind::ordered_item
                                 ? ::pltxt2htm::NodeKind::md_ol
                                 : ::pltxt2htm::NodeKind::md_ul};
        }
        auto&& parent_frame = ::pltxt2htm::details::stack_top<ndebug>(call_stack);
        switch (frame.get_item_kind()) {
        case ::pltxt2htm::details::MdUlListItemKind::ordered_item: {
            parent_frame.md_list_ast.emplace_back(
                ::pltxt2htm::details::MdListOlNode<ndebug>(::std::move(frame.md_list_ast)));
            break;
        }
        case ::pltxt2htm::details::MdUlListItemKind::hyphen:
            [[fallthrough]];
        case ::pltxt2htm::details::MdUlListItemKind::plus:
            [[fallthrough]];
        case ::pltxt2htm::details::MdUlListItemKind::asterisk: {
            parent_frame.md_list_ast.emplace_back(
                ::pltxt2htm::details::MdListUlNode<ndebug>(::std::move(frame.md_list_ast)));
            break;
        }
#if 0
        default:
            [[unlikely]] {
                ::exception::unreachable<ndebug == ::pltxt2htm::Contracts::ignore>();
            }
#endif
        }
        parent_frame.current_index += frame.current_index;
        continue;
    }
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
