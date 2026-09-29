/**
 * @file ast_decl.hh
 * @brief Abstract Syntax Tree container declaration for pltxt2htm
 * @details Defines ::pltxt2htm::Ast, the owned sequence of ::pltxt2htm::PlTxtNode<ndebug> that
 *          the parsing, optimization and backend stages all operate on.
 *
 *          Ast is a closed wrapper around ::pltxt2htm::container::Vector instead of an alias
 *          for it, so that the AST keeps a name of its own while the backing storage stays an
 *          implementation detail. Every public member of the backing vector is mirrored
 *          one-to-one below; when container/vector.hh gains or changes a public member, mirror
 *          it here as well.
 *
 * @note Ast is a distinct type rather than an alias, so the two are no longer interchangeable:
 *       an Ast does not convert implicitly to
 *       ::pltxt2htm::container::Vector<::pltxt2htm::PlTxtNode<ndebug>> (or the other way
 *       around), ::pltxt2htm::container::swap does not accept two Asts, and an Ast cannot be
 *       compared against a bare node vector. Code that relied on the alias identity has to
 *       name the type it actually wants; that is the intended consequence of giving the AST a
 *       type of its own.
 *
 * @warning Ast is instantiated while ::pltxt2htm::PlTxtNode<ndebug> may still be incomplete:
 *          the node classes hold an Ast member, and those classes are instantiated from
 *          ::pltxt2htm::PlTxtNode<ndebug>'s own storage union. No declaration in the class body
 *          may therefore require the element type to be complete - no class-scope static_assert,
 *          class-scope constexpr constant, sizeof, or completeness-dependent trait. Such
 *          expressions belong in a member function body or a trailing requires clause, which are
 *          evaluated at the point of use, where ::pltxt2htm::PlTxtNode<ndebug> is complete.
 */

#pragma once

#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <utility>

#include "../../container/vector.hh"
#include "../../contracts.hh"

namespace pltxt2htm {

template<::pltxt2htm::Contracts ndebug>
class PlTxtNode;

/**
 * @brief The Abstract Syntax Tree: an owned sequence of ::pltxt2htm::PlTxtNode<ndebug>.
 * @details Mirrors the public interface of ::pltxt2htm::container::Vector one-to-one; read the
 *          warning in this file's header comment before adding class-scope declarations.
 * @tparam ndebug Contract checking mode.
 */
template<::pltxt2htm::Contracts ndebug>
class Ast {
    using storage_type = ::pltxt2htm::container::Vector<::pltxt2htm::PlTxtNode<ndebug>>;

    storage_type storage{};

public:
    using allocator_type = typename storage_type::allocator_type; ///< Allocator used for node storage.
    using value_type = typename storage_type::value_type; ///< Node type stored by the AST.
    using size_type = typename storage_type::size_type; ///< Unsigned node count and index type.
    using difference_type = typename storage_type::difference_type; ///< Signed iterator difference type.
    using reference = typename storage_type::reference; ///< Mutable node reference.
    using const_reference = typename storage_type::const_reference; ///< Read-only node reference.
    using pointer = typename storage_type::pointer; ///< Mutable contiguous-storage pointer.
    using const_pointer = typename storage_type::const_pointer; ///< Read-only contiguous-storage pointer.
    using iterator = typename storage_type::iterator; ///< Mutable contiguous iterator.
    using const_iterator = typename storage_type::const_iterator; ///< Read-only contiguous iterator.
    using reverse_iterator = typename storage_type::reverse_iterator; ///< Mutable reverse iterator.
    using const_reverse_iterator = typename storage_type::const_reverse_iterator; ///< Read-only reverse iterator.

    constexpr Ast() noexcept = default;

    constexpr Ast(::std::initializer_list<value_type> values) noexcept
        : storage{values} {
    }

    /**
     * @note: See ::pltxt2htm::PlTxtNode for why copying is offered to external users only
     *        (PLTXT2HTM_INTERNAL_USE).
     */
    constexpr Ast(Ast const&) noexcept = default;

    constexpr Ast(Ast&&) noexcept = default;

    constexpr auto operator=(this Ast& self, Ast const& other) noexcept -> Ast& {
        self.storage = other.storage;
        return self;
    }

    constexpr auto operator=(this Ast& self, Ast&& other) noexcept -> Ast& {
        self.storage = ::std::move(other.storage);
        return self;
    }

    constexpr ~Ast() noexcept = default;

    [[nodiscard]]
    constexpr auto data(this Ast& self) noexcept -> pointer {
        return self.storage.data();
    }

    [[nodiscard]]
    constexpr auto data(this Ast const& self) noexcept -> const_pointer {
        return self.storage.data();
    }

    [[nodiscard]]
    constexpr auto is_empty(this Ast const& self) noexcept -> bool {
        return self.storage.is_empty();
    }

    /**
     * @brief Tests whether the AST has no nodes, for downstream users only.
     * @return true when size() is zero.
     */
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto empty(this Ast const&) noexcept -> bool = delete
    #if __cpp_deleted_function >= 202403L
        #if defined __clang__
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wc++26-extensions"
        #endif
        ("empty() is external-only; use is_empty() inside pltxt2htm")
        #if defined __clang__
            #pragma clang diagnostic pop
        #endif
    #endif
        ;
#else
    [[nodiscard]]
    constexpr auto empty(this Ast const& self) noexcept -> bool {
        return self.storage.is_empty();
    }
#endif

    [[nodiscard]]
    constexpr auto size(this Ast const& self) noexcept -> size_type {
        return self.storage.size();
    }

    [[nodiscard]]
    constexpr auto capacity(this Ast const& self) noexcept -> size_type {
        return self.storage.capacity();
    }

    [[nodiscard]]
    static constexpr auto max_size() noexcept -> size_type {
        return storage_type::max_size();
    }

    [[nodiscard]]
    constexpr auto begin(this Ast& self) noexcept -> iterator {
        return self.storage.begin();
    }

    [[nodiscard]]
    constexpr auto begin(this Ast const& self) noexcept -> const_iterator {
        return self.storage.begin();
    }

    [[nodiscard]]
    constexpr auto end(this Ast& self) noexcept -> iterator {
        return self.storage.end();
    }

    [[nodiscard]]
    constexpr auto end(this Ast const& self) noexcept -> const_iterator {
        return self.storage.end();
    }

    [[nodiscard]]
    constexpr auto cbegin(this Ast const& self) noexcept -> const_iterator {
        return self.storage.cbegin();
    }

    [[nodiscard]]
    constexpr auto cend(this Ast const& self) noexcept -> const_iterator {
        return self.storage.cend();
    }

    [[nodiscard]]
    constexpr auto rbegin(this Ast& self) noexcept -> reverse_iterator {
        return self.storage.rbegin();
    }

    [[nodiscard]]
    constexpr auto rbegin(this Ast const& self) noexcept -> const_reverse_iterator {
        return self.storage.rbegin();
    }

    [[nodiscard]]
    constexpr auto rend(this Ast& self) noexcept -> reverse_iterator {
        return self.storage.rend();
    }

    [[nodiscard]]
    constexpr auto rend(this Ast const& self) noexcept -> const_reverse_iterator {
        return self.storage.rend();
    }

    template<::pltxt2htm::Contracts contracts>
    [[nodiscard]]
    constexpr auto front(this Ast& self) noexcept -> reference {
        return self.storage.template front<contracts>();
    }

    template<::pltxt2htm::Contracts contracts>
    [[nodiscard]]
    constexpr auto front(this Ast const& self) noexcept -> const_reference {
        return self.storage.template front<contracts>();
    }

    template<::pltxt2htm::Contracts contracts>
    [[nodiscard]]
    constexpr auto index(this Ast& self, size_type position) noexcept -> reference {
        return self.storage.template index<contracts>(position);
    }

    template<::pltxt2htm::Contracts contracts>
    [[nodiscard]]
    constexpr auto index(this Ast const& self, size_type position) noexcept -> const_reference {
        return self.storage.template index<contracts>(position);
    }

    template<::pltxt2htm::Contracts contracts = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void reserve(this Ast& self, size_type requested_capacity) noexcept
        requires (::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
        self.storage.template reserve<contracts>(requested_capacity);
    }

    template<::pltxt2htm::Contracts contracts = ::pltxt2htm::Contracts::quick_enforce, typename... Args>
        requires (::std::is_nothrow_move_constructible_v<value_type> &&
                  ::std::is_nothrow_constructible_v<value_type, Args...> &&
                  ::std::is_nothrow_destructible_v<value_type>)
    constexpr auto emplace_back(this Ast& self, Args&&... args) noexcept -> reference {
        return self.storage.template emplace_back<contracts>(::std::forward<Args>(args)...);
    }

    template<::pltxt2htm::Contracts contracts = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void push_back(this Ast& self, const_reference value) noexcept
        requires (::std::is_nothrow_move_constructible_v<value_type> &&
                  ::std::is_nothrow_copy_constructible_v<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
        self.storage.template push_back<contracts>(value);
    }

    template<::pltxt2htm::Contracts contracts = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void push_back(this Ast& self, value_type&& value) noexcept
        requires (::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
        self.storage.template push_back<contracts>(::std::move(value));
    }

    template<::pltxt2htm::Contracts contracts = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void pop_back(this Ast& self) noexcept
        requires ::std::is_nothrow_destructible_v<value_type>
    {
        self.storage.template pop_back<contracts>();
    }

    constexpr void clear(this Ast& self) noexcept
        requires ::std::is_nothrow_destructible_v<value_type>
    {
        self.storage.clear();
    }

    /**
     * @pre A single-pass input range must not reference elements in this Ast.
     */
    template<::pltxt2htm::Contracts contracts = ::pltxt2htm::Contracts::quick_enforce, ::std::ranges::input_range R>
    constexpr void append_range(this Ast& self, R&& range) noexcept
        requires (::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_destructible_v<value_type>) &&
                 requires(storage_type& probe_storage, R&& probe_range) {
                     probe_storage.template append_range<contracts>(::std::forward<R>(probe_range));
                 }
    {
        self.storage.template append_range<contracts>(::std::forward<R>(range));
    }

    constexpr auto erase(this Ast& self, const_iterator position) noexcept -> iterator
        requires (::std::is_nothrow_move_assignable_v<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
        return self.storage.erase(position);
    }

    constexpr auto erase(this Ast& self, const_iterator first, const_iterator last) noexcept -> iterator
        requires (::std::is_nothrow_move_assignable_v<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
        return self.storage.erase(first, last);
    }

    constexpr void swap(this Ast& self, Ast& other) noexcept {
        self.storage.swap(other.storage);
    }

    [[nodiscard]]
    constexpr auto operator==(this Ast const& self, Ast const& other) noexcept -> bool
        requires requires(const_reference left, const_reference right) {
            { static_cast<bool>(left == right) } noexcept -> ::std::same_as<bool>;
        }
    {
        return self.storage == other.storage;
    }
};

} // namespace pltxt2htm
