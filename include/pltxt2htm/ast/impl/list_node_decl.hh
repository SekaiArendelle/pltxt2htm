/**
 * @file list_node_decl.hh
 * @brief List AST node declarations (shared by HTML and Markdown lists)
 * @details Defines the list container and item nodes: ListUl, ListOl, ListLi and
 *          ListLiCheckbox.  Both the HTML &lt;ul&gt;/&lt;ol&gt; and the Markdown
 *          list scanners produce these nodes.
 */

#pragma once

#include <cstddef>
#include <utility>
#include "ast_decl.hh"

namespace pltxt2htm {

/**
 * @brief Unordered list node (&lt;ul&gt;...&lt;/ul&gt; or Markdown `- item`)
 */
template<::pltxt2htm::Contracts ndebug>
class ListUl {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit ListUl(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr ListUl(::pltxt2htm::ListUl<ndebug> const&) noexcept;
    constexpr ListUl(::pltxt2htm::ListUl<ndebug>&&) noexcept;
    constexpr ~ListUl() noexcept;
    constexpr auto operator=(::pltxt2htm::ListUl<ndebug> const&) noexcept -> ::pltxt2htm::ListUl<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::ListUl<ndebug>& self, ::pltxt2htm::ListUl<ndebug>&&) noexcept
        -> ::pltxt2htm::ListUl<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this ListUl const&, ListUl const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Ordered list node (&lt;ol&gt;...&lt;/ol&gt; or Markdown `1. item`)
 * @details The `start` member records the value of the HTML `start` attribute
 *          (Markdown lists always start at 1).
 */
template<::pltxt2htm::Contracts ndebug>
class ListOl {
    ::pltxt2htm::Ast<ndebug> subast;
    ::std::size_t start{1};

public:
    constexpr ListOl(::pltxt2htm::Ast<ndebug>&& subast_, ::std::size_t start_ = 1) noexcept;
    constexpr ListOl(::pltxt2htm::ListOl<ndebug> const&) noexcept;
    constexpr ListOl(::pltxt2htm::ListOl<ndebug>&&) noexcept;
    constexpr ~ListOl() noexcept;
    constexpr auto operator=(::pltxt2htm::ListOl<ndebug> const&) noexcept -> ::pltxt2htm::ListOl<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::ListOl<ndebug>& self, ::pltxt2htm::ListOl<ndebug>&&) noexcept
        -> ::pltxt2htm::ListOl<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this ListOl const&, ListOl const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto get_start(this ListOl const& self) noexcept -> ::std::size_t {
        return self.start;
    }
};

/**
 * @brief List item node (&lt;li&gt;...&lt;/li&gt; or Markdown list item)
 */
template<::pltxt2htm::Contracts ndebug>
class ListLi {
    ::pltxt2htm::Ast<ndebug> subast;

public:
    constexpr explicit ListLi(::pltxt2htm::Ast<ndebug>&& subast_) noexcept;
    constexpr ListLi(::pltxt2htm::ListLi<ndebug> const&) noexcept;
    constexpr ListLi(::pltxt2htm::ListLi<ndebug>&&) noexcept;
    constexpr ~ListLi() noexcept;
    constexpr auto operator=(::pltxt2htm::ListLi<ndebug> const&) noexcept -> ::pltxt2htm::ListLi<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::ListLi<ndebug>& self, ::pltxt2htm::ListLi<ndebug>&&) noexcept
        -> ::pltxt2htm::ListLi<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this ListLi const&, ListLi const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }
};

/**
 * @brief Checkbox list item node (Markdown `- [ ]` / `- [x]`)
 */
template<::pltxt2htm::Contracts ndebug>
class ListLiCheckbox {
    ::pltxt2htm::Ast<ndebug> subast;
    bool checked;

public:
    constexpr explicit ListLiCheckbox(::pltxt2htm::Ast<ndebug>&& subast_, bool checked_) noexcept;
    constexpr ListLiCheckbox(::pltxt2htm::ListLiCheckbox<ndebug> const&) noexcept;
    constexpr ListLiCheckbox(::pltxt2htm::ListLiCheckbox<ndebug>&&) noexcept;
    constexpr ~ListLiCheckbox() noexcept;
    constexpr auto operator=(::pltxt2htm::ListLiCheckbox<ndebug> const&) noexcept
        -> ::pltxt2htm::ListLiCheckbox<ndebug>& = delete;
    constexpr auto operator=(this ::pltxt2htm::ListLiCheckbox<ndebug>& self,
                             ::pltxt2htm::ListLiCheckbox<ndebug>&&) noexcept -> ::pltxt2htm::ListLiCheckbox<ndebug>&;

    [[nodiscard]]
    constexpr auto operator==(this ListLiCheckbox const&, ListLiCheckbox const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto is_checked(this auto&& self) noexcept -> bool {
        return self.checked;
    }
};

} // namespace pltxt2htm
