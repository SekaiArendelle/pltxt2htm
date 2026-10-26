/**
 * @file ast_decl.hh
 * @brief Abstract Syntax Tree container declaration for pltxt2htm
 * @details Defines ::pltxt2htm::Ast, the owned sequence of ::pltxt2htm::PlTxtNode that
 *          the parsing, optimization and backend stages all operate on.
 *
 *          Ast is a closed wrapper around ::pltxt2htm::container::Vector instead of an alias
 *          for it, so that the AST keeps a name of its own while the backing storage stays an
 *          implementation detail. Its public container operations mirror the backing vector,
 *          while their contract checking mode is fixed by Ast's build-wide contract setting. Ast
 *          also owns AST-specific operations such as appending text while coalescing adjacent
 *          text nodes. When container/vector.hh gains or changes a public member, mirror it here
 *          as well.
 *
 * @note Ast is a distinct type rather than an alias, so the two are no longer interchangeable:
 *       an Ast does not convert implicitly to
 *       ::pltxt2htm::container::Vector<::pltxt2htm::PlTxtNode> (or the other way
 *       around), ::pltxt2htm::container::swap does not accept two Asts, and an Ast cannot be
 *       compared against a bare node vector. Code that relied on the alias identity has to
 *       name the type it actually wants; that is the intended consequence of giving the AST a
 *       type of its own.
 *
 * @warning Ast is instantiated while ::pltxt2htm::PlTxtNode may still be incomplete:
 *          the node classes hold an Ast member, and those classes are instantiated from
 *          ::pltxt2htm::PlTxtNode's own storage union. No declaration in the class body
 *          may therefore require the element type to be complete - no class-scope static_assert,
 *          class-scope constexpr constant, sizeof, or completeness-dependent trait. Such
 *          expressions belong in a member function body or a trailing requires clause, which are
 *          evaluated at the point of use, where ::pltxt2htm::PlTxtNode is complete.
 */

#pragma once

#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <utility>

#include "../../container/string_view.hh"
#include "../../container/vector.hh"
#include "../../details/contracts.hh"

namespace pltxt2htm {

class PlTxtNode;

/**
 * @brief The Abstract Syntax Tree: an owned sequence of ::pltxt2htm::PlTxtNode.
 * @details Mirrors the operations of ::pltxt2htm::container::Vector, with checks controlled by
 *          contract checking mode; read the warning in this file's header comment before adding
 *          class-scope declarations.
 * Contract mode follows PLTXT2HTM_CONTRACTS.
 */
class Ast {
    using storage_type = ::pltxt2htm::container::Vector<::pltxt2htm::PlTxtNode>;

    storage_type storage{};

public:
    using allocator_type = typename storage_type::allocator_type;
    using value_type = typename storage_type::value_type;
    using size_type = typename storage_type::size_type;
    using difference_type = typename storage_type::difference_type;
    using reference = typename storage_type::reference;
    using const_reference = typename storage_type::const_reference;
    using pointer = typename storage_type::pointer;
    using const_pointer = typename storage_type::const_pointer;
    using iterator = typename storage_type::iterator;
    using const_iterator = typename storage_type::const_iterator;
    using reverse_iterator = typename storage_type::reverse_iterator;
    using const_reverse_iterator = typename storage_type::const_reverse_iterator;

    constexpr Ast() noexcept = default;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr Ast(::std::initializer_list<value_type> values) noexcept = delete;
    constexpr Ast(Ast const&) noexcept = delete;
    constexpr auto operator=(this Ast& self, Ast const& other) noexcept -> Ast& = delete;
#else
    constexpr Ast(::std::initializer_list<value_type> values) noexcept;
    constexpr Ast(Ast const&) noexcept;
    constexpr auto operator=(this Ast& self, Ast const& other) noexcept -> Ast&;
#endif
    constexpr Ast(Ast&&) noexcept;
    constexpr auto operator=(this Ast& self, Ast&& other) noexcept -> Ast&;
    constexpr ~Ast() noexcept;

    [[nodiscard]] constexpr auto data(this Ast& self) noexcept -> pointer;
    [[nodiscard]] constexpr auto data(this Ast const& self) noexcept -> const_pointer;
    [[nodiscard]] constexpr auto is_empty(this Ast const& self) noexcept -> bool;
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto empty(this Ast const&) noexcept -> bool = delete;
#else
    [[nodiscard]] constexpr auto empty(this Ast const& self) noexcept -> bool;
#endif
    [[nodiscard]] constexpr auto size(this Ast const& self) noexcept -> size_type;
    [[nodiscard]] constexpr auto capacity(this Ast const& self) noexcept -> size_type;
    [[nodiscard]] static constexpr auto max_size() noexcept -> size_type;
    [[nodiscard]] constexpr auto begin(this Ast& self) noexcept -> iterator;
    [[nodiscard]] constexpr auto begin(this Ast const& self) noexcept -> const_iterator;
    [[nodiscard]] constexpr auto end(this Ast& self) noexcept -> iterator;
    [[nodiscard]] constexpr auto end(this Ast const& self) noexcept -> const_iterator;
    [[nodiscard]] constexpr auto cbegin(this Ast const& self) noexcept -> const_iterator;
    [[nodiscard]] constexpr auto cend(this Ast const& self) noexcept -> const_iterator;
    [[nodiscard]] constexpr auto rbegin(this Ast& self) noexcept -> reverse_iterator;
    [[nodiscard]] constexpr auto rbegin(this Ast const& self) noexcept -> const_reverse_iterator;
    [[nodiscard]] constexpr auto rend(this Ast& self) noexcept -> reverse_iterator;
    [[nodiscard]] constexpr auto rend(this Ast const& self) noexcept -> const_reverse_iterator;
    [[nodiscard]] constexpr auto front(this Ast& self) noexcept -> reference;
    [[nodiscard]] constexpr auto front(this Ast const& self) noexcept -> const_reference;
    [[nodiscard]] constexpr auto index(this Ast& self, size_type position) noexcept -> reference;
    [[nodiscard]] constexpr auto index(this Ast const& self, size_type position) noexcept -> const_reference;
    constexpr void reserve(this Ast& self, size_type requested_capacity) noexcept;

    template<typename... Args>
    constexpr auto emplace_back(this Ast& self, Args&&... args) noexcept -> reference {
        return self.storage.emplace_back(::std::forward<Args>(args)...);
    }

#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr void push_back(this Ast& self, const_reference value) noexcept = delete;
#else
    constexpr void push_back(this Ast& self, const_reference value) noexcept;
#endif
    constexpr void push_back(this Ast& self, value_type&& value) noexcept;
    constexpr void pop_back(this Ast& self) noexcept;
    constexpr void clear(this Ast& self) noexcept;
    constexpr void append_text(this Ast& self, char8_t code_unit) noexcept;
    constexpr void append_text(this Ast& self, ::pltxt2htm::container::U8StringView text) noexcept;
    constexpr void append_code_point(this Ast& self, char32_t code_point) noexcept;

    template<::std::ranges::input_range R>
    constexpr void append_range(this Ast& self, R&& range) noexcept
        requires requires(storage_type& probe_storage, R&& probe_range) {
            probe_storage.append_range(::std::forward<R>(probe_range));
        }
    {
        self.storage.append_range(::std::forward<R>(range));
    }

    constexpr auto erase(this Ast& self, const_iterator position) noexcept -> iterator;
    constexpr auto erase(this Ast& self, const_iterator first, const_iterator last) noexcept -> iterator;
    constexpr void swap(this Ast& self, Ast& other) noexcept;
    [[nodiscard]] constexpr auto operator==(this Ast const& self, Ast const& other) noexcept -> bool;
};

} // namespace pltxt2htm
