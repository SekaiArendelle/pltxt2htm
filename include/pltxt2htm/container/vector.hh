/**
 * @file vector.hh
 * @brief Dynamically sized contiguous containers for pltxt2htm.
 */

#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <ranges>
#include <type_traits>
#include <utility>

#include <fast_io/fast_io_core.h>

#include "../contracts.hh"
#include "../details/push_macro.hh"

namespace pltxt2htm::container {

/**
 * @brief A dynamically sized contiguous sequence.
 * @tparam T Element type.
 * @tparam Allocator Untyped fast_io allocator used for storage.
 *
 * Vector implements the operations currently needed by pltxt2htm while keeping
 * allocation compatible with the allocator facilities already used by the project.
 */
template<typename T, typename Allocator = ::fast_io::native_global_allocator>
class Vector {
public:
    using allocator_type = Allocator;
    using value_type = T;
    using size_type = ::std::size_t;
    using difference_type = ::std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = ::std::reverse_iterator<iterator>;
    using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;

private:
    using typed_allocator_type = ::fast_io::typed_generic_allocator_adapter<allocator_type, value_type>;

    pointer begin_pointer{};
    pointer current_pointer{};
    pointer end_pointer{};

    struct AllocationGuard {
        pointer begin_pointer{};
        pointer current_pointer{};
        size_type capacity{};

        constexpr AllocationGuard(pointer begin_pointer_, pointer current_pointer_, size_type capacity_) noexcept
            : begin_pointer{begin_pointer_},
              current_pointer{current_pointer_},
              capacity{capacity_} {
        }

        AllocationGuard(AllocationGuard const&) = delete;
        constexpr auto operator=(this AllocationGuard&, AllocationGuard const&) -> AllocationGuard& = delete;

        constexpr ~AllocationGuard() {
            if (begin_pointer == nullptr) {
                return;
            }
            ::std::destroy(begin_pointer, current_pointer);
            typed_allocator_type::deallocate_n(begin_pointer, capacity);
        }
    };

    constexpr void release(this Vector& self) noexcept {
        if (self.begin_pointer == nullptr) {
            return;
        }
        ::std::destroy(self.begin_pointer, self.current_pointer);
        typed_allocator_type::deallocate_n(self.begin_pointer, self.capacity());
        self.begin_pointer = nullptr;
        self.current_pointer = nullptr;
        self.end_pointer = nullptr;
    }

    constexpr void reallocate(this Vector& self, size_type requested_capacity) noexcept(
        ::std::is_nothrow_move_constructible_v<value_type>) {
        auto const allocation = typed_allocator_type::allocate_at_least(requested_capacity);
        AllocationGuard guard{allocation.ptr, allocation.ptr, allocation.count};

        for (pointer source{self.begin_pointer}; source != self.current_pointer; ++source) {
            ::std::construct_at(guard.current_pointer, ::std::move(*source));
            ++guard.current_pointer;
        }

        pointer const old_begin{self.begin_pointer};
        pointer const old_current{self.current_pointer};
        size_type const old_capacity{self.capacity()};
        self.begin_pointer = guard.begin_pointer;
        self.current_pointer = guard.current_pointer;
        self.end_pointer = guard.begin_pointer + guard.capacity;
        guard.begin_pointer = nullptr;

        if (old_begin != nullptr) {
            ::std::destroy(old_begin, old_current);
            typed_allocator_type::deallocate_n(old_begin, old_capacity);
        }
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void ensure_additional_capacity(this Vector& self, size_type additional_size) noexcept(
        ::std::is_nothrow_move_constructible_v<value_type>) {
        size_type const old_size{self.size()};
        pltxt2htm_assert(additional_size <= self.max_size() - old_size, u8"Vector size exceeds max_size");
        size_type const required_capacity{old_size + additional_size};
        if (required_capacity <= self.capacity()) {
            return;
        }

        size_type new_capacity{self.capacity()};
        if (new_capacity == 0) {
            new_capacity = 1;
        }
        if (new_capacity <= self.max_size() / 2) {
            new_capacity *= 2;
        }
        else {
            new_capacity = self.max_size();
        }
        if (new_capacity < required_capacity) {
            new_capacity = required_capacity;
        }
        self.reallocate(new_capacity);
    }

public:
    constexpr Vector() noexcept = default;

    constexpr Vector(::std::initializer_list<value_type> values) noexcept(
        ::std::is_nothrow_copy_constructible_v<value_type>) {
        if (values.size() == 0) {
            return;
        }

        auto const allocation = typed_allocator_type::allocate_at_least(values.size());
        AllocationGuard guard{allocation.ptr, allocation.ptr, allocation.count};
        for (auto const& value : values) {
            ::std::construct_at(guard.current_pointer, value);
            ++guard.current_pointer;
        }
        begin_pointer = guard.begin_pointer;
        current_pointer = guard.current_pointer;
        end_pointer = guard.begin_pointer + guard.capacity;
        guard.begin_pointer = nullptr;
    }

    constexpr Vector(Vector const& other) noexcept(::std::is_nothrow_copy_constructible_v<value_type>) {
        size_type const other_size{other.size()};
        if (other_size == 0) {
            return;
        }

        auto const allocation = typed_allocator_type::allocate_at_least(other_size);
        AllocationGuard guard{allocation.ptr, allocation.ptr, allocation.count};
        for (auto const& value : other) {
            ::std::construct_at(guard.current_pointer, value);
            ++guard.current_pointer;
        }
        begin_pointer = guard.begin_pointer;
        current_pointer = guard.current_pointer;
        end_pointer = guard.begin_pointer + guard.capacity;
        guard.begin_pointer = nullptr;
    }

    constexpr Vector(Vector&& other) noexcept
        : begin_pointer{::std::exchange(other.begin_pointer, nullptr)},
          current_pointer{::std::exchange(other.current_pointer, nullptr)},
          end_pointer{::std::exchange(other.end_pointer, nullptr)} {
    }

    constexpr auto operator=(this Vector& self,
                             Vector const& other) noexcept(::std::is_nothrow_copy_constructible_v<value_type>)
        -> Vector& {
        if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
            return self;
        }
        Vector temporary{other};
        self.swap(temporary);
        return self;
    }

    constexpr auto operator=(this Vector& self, Vector&& other) noexcept -> Vector& {
        if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
            return self;
        }
        self.release();
        self.begin_pointer = ::std::exchange(other.begin_pointer, nullptr);
        self.current_pointer = ::std::exchange(other.current_pointer, nullptr);
        self.end_pointer = ::std::exchange(other.end_pointer, nullptr);
        return self;
    }

    constexpr ~Vector() {
        this->release();
    }

    [[nodiscard]]
    constexpr auto data(this Vector& self) noexcept -> pointer {
        return self.begin_pointer;
    }

    [[nodiscard]]
    constexpr auto data(this Vector const& self) noexcept -> const_pointer {
        return self.begin_pointer;
    }

    [[nodiscard]]
    constexpr auto empty(this Vector const& self) noexcept -> bool {
        return self.begin_pointer == self.current_pointer;
    }

    [[nodiscard]]
    constexpr auto size(this Vector const& self) noexcept -> size_type {
        if (self.begin_pointer == nullptr) {
            return 0;
        }
        return static_cast<size_type>(self.current_pointer - self.begin_pointer);
    }

    [[nodiscard]]
    constexpr auto capacity(this Vector const& self) noexcept -> size_type {
        if (self.begin_pointer == nullptr) {
            return 0;
        }
        return static_cast<size_type>(self.end_pointer - self.begin_pointer);
    }

    [[nodiscard]]
    static constexpr auto max_size() noexcept -> size_type {
        return ::std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    [[nodiscard]]
    constexpr auto begin(this Vector& self) noexcept -> iterator {
        return self.begin_pointer;
    }

    [[nodiscard]]
    constexpr auto begin(this Vector const& self) noexcept -> const_iterator {
        return self.begin_pointer;
    }

    [[nodiscard]]
    constexpr auto end(this Vector& self) noexcept -> iterator {
        return self.current_pointer;
    }

    [[nodiscard]]
    constexpr auto end(this Vector const& self) noexcept -> const_iterator {
        return self.current_pointer;
    }

    [[nodiscard]]
    constexpr auto cbegin(this Vector const& self) noexcept -> const_iterator {
        return self.begin_pointer;
    }

    [[nodiscard]]
    constexpr auto cend(this Vector const& self) noexcept -> const_iterator {
        return self.current_pointer;
    }

    [[nodiscard]]
    constexpr auto rbegin(this Vector& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.current_pointer};
    }

    [[nodiscard]]
    constexpr auto rbegin(this Vector const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.current_pointer};
    }

    [[nodiscard]]
    constexpr auto rend(this Vector& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.begin_pointer};
    }

    [[nodiscard]]
    constexpr auto rend(this Vector const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.begin_pointer};
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto front(this Vector& self) noexcept -> reference {
        pltxt2htm_assert(!self.empty(), u8"Accessing front but Vector is empty");
        return *self.begin_pointer;
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto front(this Vector const& self) noexcept -> const_reference {
        pltxt2htm_assert(!self.empty(), u8"Accessing front but Vector is empty");
        return *self.begin_pointer;
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto index(this Vector& self, size_type position) noexcept -> reference {
        pltxt2htm_assert(position < self.size(), u8"Index of Vector out of bound");
        return self.begin_pointer[position];
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto index(this Vector const& self, size_type position) noexcept -> const_reference {
        pltxt2htm_assert(position < self.size(), u8"Index of Vector out of bound");
        return self.begin_pointer[position];
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void reserve(this Vector& self,
                           size_type requested_capacity) noexcept(::std::is_nothrow_move_constructible_v<value_type>) {
        pltxt2htm_assert(requested_capacity <= self.max_size(), u8"Vector capacity exceeds max_size");
        if (requested_capacity <= self.capacity()) {
            return;
        }
        self.reallocate(requested_capacity);
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, typename... Args>
        requires ::std::constructible_from<value_type, Args...>
    constexpr auto emplace_back(this Vector& self,
                                Args&&... args) noexcept(::std::is_nothrow_move_constructible_v<value_type> &&
                                                         ::std::is_nothrow_constructible_v<value_type, Args...>)
        -> reference {
        self.template ensure_additional_capacity<ndebug>(1);
        pointer const element{::std::construct_at(self.current_pointer, ::std::forward<Args>(args)...)};
        ++self.current_pointer;
        return *element;
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void push_back(this Vector& self,
                             const_reference value) noexcept(::std::is_nothrow_move_constructible_v<value_type> &&
                                                             ::std::is_nothrow_copy_constructible_v<value_type>) {
        self.template emplace_back<ndebug>(value);
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void push_back(this Vector& self,
                             value_type&& value) noexcept(::std::is_nothrow_move_constructible_v<value_type>) {
        self.template emplace_back<ndebug>(::std::move(value));
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void pop_back(this Vector& self) noexcept {
        pltxt2htm_assert(!self.empty(), u8"Popping back but Vector is empty");
        --self.current_pointer;
        ::std::destroy_at(self.current_pointer);
    }

    constexpr void clear(this Vector& self) noexcept {
        if (self.begin_pointer == nullptr) {
            return;
        }
        ::std::destroy(self.begin_pointer, self.current_pointer);
        self.current_pointer = self.begin_pointer;
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, ::std::ranges::input_range R>
    constexpr void append_range(this Vector& self, R&& range) {
        if constexpr (::std::ranges::sized_range<R>) {
            self.template ensure_additional_capacity<ndebug>(static_cast<size_type>(::std::ranges::size(range)));
        }

        for (auto&& value : range) {
            self.template emplace_back<ndebug>(::std::forward_like<R>(value));
        }
    }

    constexpr auto erase(this Vector& self,
                         const_iterator position) noexcept(::std::is_nothrow_move_constructible_v<value_type>)
        -> iterator {
        difference_type const offset{position - self.begin_pointer};
        pointer destination{self.begin_pointer + offset};
        pointer source{destination + 1};
        ::std::destroy_at(destination);
        for (; source != self.current_pointer; ++source, ++destination) {
            ::std::construct_at(destination, ::std::move(*source));
            ::std::destroy_at(source);
        }
        --self.current_pointer;
        return self.begin_pointer + offset;
    }

    constexpr auto erase(this Vector& self, const_iterator first,
                         const_iterator last) noexcept(::std::is_nothrow_move_constructible_v<value_type>) -> iterator {
        if (first == last) {
            if (self.begin_pointer == nullptr) {
                return nullptr;
            }
            return self.begin_pointer + (first - self.begin_pointer);
        }

        difference_type const first_offset{first - self.begin_pointer};
        difference_type const last_offset{last - self.begin_pointer};
        pointer destination{self.begin_pointer + first_offset};
        pointer source{self.begin_pointer + last_offset};
        pointer const result{destination};
        ::std::destroy(destination, source);
        for (; source != self.current_pointer; ++source, ++destination) {
            ::std::construct_at(destination, ::std::move(*source));
            ::std::destroy_at(source);
        }
        self.current_pointer = destination;
        return result;
    }

    constexpr void swap(this Vector& self, Vector& other) noexcept {
        ::std::swap(self.begin_pointer, other.begin_pointer);
        ::std::swap(self.current_pointer, other.current_pointer);
        ::std::swap(self.end_pointer, other.end_pointer);
    }

    [[nodiscard]]
    constexpr auto operator==(this Vector const& self, Vector const& other) noexcept -> bool
        requires ::std::equality_comparable<value_type>
    {
        return self.size() == other.size() && ::std::equal(self.begin(), self.end(), other.begin());
    }
};

template<typename T, typename... U>
    requires (::std::same_as<T, U> && ...)
Vector(T, U...) -> Vector<T>;

template<typename T, typename Allocator>
constexpr void swap(Vector<T, Allocator>& left, Vector<T, Allocator>& right) noexcept {
    left.swap(right);
}

} // namespace pltxt2htm::container

#include "../details/pop_macro.hh"
