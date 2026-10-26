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
class ListUl {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit ListUl(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr ListUl(::pltxt2htm::ListUl const&) noexcept = delete;
#else
    constexpr ListUl(::pltxt2htm::ListUl const&) noexcept = default;
#endif
    constexpr ListUl(::pltxt2htm::ListUl&&) noexcept;
    constexpr ~ListUl() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::ListUl const&) noexcept -> ::pltxt2htm::ListUl& = delete;
#else
    constexpr auto operator=(this ListUl& self, ::pltxt2htm::ListUl const&) noexcept -> ::pltxt2htm::ListUl& = default;
#endif
    constexpr auto operator=(this ListUl& self, ::pltxt2htm::ListUl&&) noexcept -> ::pltxt2htm::ListUl&;

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
class ListOl {
    ::pltxt2htm::Ast subast;
    ::std::size_t start;

public:
    constexpr ListOl(::pltxt2htm::Ast&& subast_, ::std::size_t start_ = 1) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr ListOl(::pltxt2htm::ListOl const&) noexcept = delete;
#else
    constexpr ListOl(::pltxt2htm::ListOl const&) noexcept = default;
#endif
    constexpr ListOl(::pltxt2htm::ListOl&&) noexcept;
    constexpr ~ListOl() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::ListOl const&) noexcept -> ::pltxt2htm::ListOl& = delete;
#else
    constexpr auto operator=(this ListOl& self, ::pltxt2htm::ListOl const&) noexcept -> ::pltxt2htm::ListOl& = default;
#endif
    constexpr auto operator=(this ListOl& self, ::pltxt2htm::ListOl&&) noexcept -> ::pltxt2htm::ListOl&;

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
class ListLi {
    ::pltxt2htm::Ast subast;

public:
    constexpr explicit ListLi(::pltxt2htm::Ast&& subast_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr ListLi(::pltxt2htm::ListLi const&) noexcept = delete;
#else
    constexpr ListLi(::pltxt2htm::ListLi const&) noexcept = default;
#endif
    constexpr ListLi(::pltxt2htm::ListLi&&) noexcept;
    constexpr ~ListLi() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::ListLi const&) noexcept -> ::pltxt2htm::ListLi& = delete;
#else
    constexpr auto operator=(this ListLi& self, ::pltxt2htm::ListLi const&) noexcept -> ::pltxt2htm::ListLi& = default;
#endif
    constexpr auto operator=(this ListLi& self, ::pltxt2htm::ListLi&&) noexcept -> ::pltxt2htm::ListLi&;

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
class ListLiCheckbox {
    ::pltxt2htm::Ast subast;
    bool checked;

public:
    constexpr explicit ListLiCheckbox(::pltxt2htm::Ast&& subast_, bool checked_) noexcept;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr ListLiCheckbox(::pltxt2htm::ListLiCheckbox const&) noexcept = delete;
#else
    constexpr ListLiCheckbox(::pltxt2htm::ListLiCheckbox const&) noexcept = default;
#endif
    constexpr ListLiCheckbox(::pltxt2htm::ListLiCheckbox&&) noexcept;
    constexpr ~ListLiCheckbox() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator=(::pltxt2htm::ListLiCheckbox const&) noexcept -> ::pltxt2htm::ListLiCheckbox& = delete;
#else
    constexpr auto operator=(this ListLiCheckbox& self, ::pltxt2htm::ListLiCheckbox const&) noexcept -> ::pltxt2htm::ListLiCheckbox& = default;
#endif
    constexpr auto operator=(this ListLiCheckbox& self, ::pltxt2htm::ListLiCheckbox&&) noexcept
        -> ::pltxt2htm::ListLiCheckbox&;

    [[nodiscard]]
    constexpr auto operator==(this ListLiCheckbox const&, ListLiCheckbox const&) noexcept -> bool;

    [[nodiscard]]
    constexpr auto get_subast(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.subast);
    }

    [[nodiscard]]
    constexpr auto is_checked(this ListLiCheckbox const& self) noexcept -> bool {
        return self.checked;
    }
};

} // namespace pltxt2htm
