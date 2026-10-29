/**
 * @file vector_def.inc
 * @brief Out-of-line contract-checked definitions for container::Vector.
 */

#pragma once

#include "vector_decl.hh"
#include "../../details/push_macro.hh"

namespace pltxt2htm::container {

template<typename T, typename Allocator>
constexpr auto Vector<T, Allocator>::growth_capacity(this Vector const& self, size_type additional_size) noexcept
    -> size_type {
    size_type const old_size{self.size()};
    pltxt2htm_assert(additional_size <= self.max_size() - old_size, u8"Vector size exceeds max_size");
    size_type const required_capacity{old_size + additional_size};
    size_type const old_capacity{self.capacity()};
    if (required_capacity <= old_capacity) {
        return old_capacity;
    }

    if (old_capacity == 0) {
        return required_capacity < 2 ? 2 : required_capacity;
    }

    size_type new_capacity{old_capacity};
    if (new_capacity <= self.max_size() / 2) {
        new_capacity *= 2;
    }
    else {
        new_capacity = self.max_size();
    }
    if (new_capacity < required_capacity) {
        new_capacity = required_capacity;
    }
    return new_capacity;
}

template<typename T, typename Allocator>
template<typename R>
constexpr void Vector<T, Allocator>::reallocate_and_append_range(this Vector& self, R&& range, size_type range_size,
                                                                 size_type new_capacity) noexcept
    requires (::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_destructible_v<value_type> &&
              is_nothrow_append_range<R>())
{
    size_type const old_size{self.size()};
    auto const allocation = typed_allocator_type::allocate_at_least(new_capacity);
    pointer trailing_current{allocation.ptr + old_size};
    pointer const trailing_end{trailing_current + range_size};
    for (auto&& value : range) {
        pltxt2htm_assert(trailing_current != trailing_end, u8"Range size changed while appending");
        if constexpr (::std::is_lvalue_reference_v<R&&>) {
            ::std::construct_at(trailing_current, ::std::forward<decltype(value)>(value));
        }
        else {
            static_assert(::std::is_rvalue_reference_v<R&&>);
            ::std::construct_at(trailing_current, ::std::forward_like<R>(value));
        }
        ++trailing_current;
    }
    pltxt2htm_assert(trailing_current == trailing_end, u8"Range size changed while appending");

    pointer new_current{allocation.ptr};
    pointer const old_end{self.end()};
    for (pointer source{self.begin_pointer}; source != old_end; ++source) {
        ::std::construct_at(new_current, ::std::move(*source));
        ++new_current;
    }

    pointer const old_begin{self.begin_pointer};
    size_type const old_capacity{self.current_capacity};
    self.begin_pointer = allocation.ptr;
    self.current_size = old_size + range_size;
    self.current_capacity = allocation.count;

    if (old_begin != nullptr) {
        ::std::destroy(old_begin, old_end);
        typed_allocator_type::deallocate_n(old_begin, old_capacity);
    }
}

template<typename T, typename Allocator>
constexpr auto Vector<T, Allocator>::operator=(this Vector& self, Vector const& other) noexcept -> Vector& {
    static_assert(::std::is_nothrow_copy_constructible_v<value_type>,
                  "Vector requires nothrow-copy-constructible elements when copied");
    static_assert(::std::is_nothrow_destructible_v<value_type>, "Vector requires nothrow-destructible elements");
#if defined(PLTXT2HTM_INTERNAL_USE)
    pltxt2htm_assert(::std::addressof(self) != ::std::addressof(other), u8"Internal self-assignment is not allowed");
#else
    if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
        return self;
    }
#endif
    Vector temporary{other};
    self.swap(temporary);
    return self;
}

template<typename T, typename Allocator>
constexpr auto Vector<T, Allocator>::operator=(this Vector& self, Vector&& other) noexcept -> Vector& {
    static_assert(::std::is_nothrow_destructible_v<value_type>, "Vector requires nothrow-destructible elements");
#if defined(PLTXT2HTM_INTERNAL_USE)
    pltxt2htm_assert(::std::addressof(self) != ::std::addressof(other), u8"Internal self-assignment is not allowed");
#else
    if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
        return self;
    }
#endif
    self.swap(other);
    return self;
}

template<typename T, typename Allocator>
constexpr auto Vector<T, Allocator>::front(this Vector& self) noexcept -> reference {
    pltxt2htm_assert(!self.is_empty(), u8"Accessing front but Vector is empty");
    return *self.begin_pointer;
}

template<typename T, typename Allocator>
constexpr auto Vector<T, Allocator>::front(this Vector const& self) noexcept -> const_reference {
    pltxt2htm_assert(!self.is_empty(), u8"Accessing front but Vector is empty");
    return *self.begin_pointer;
}

template<typename T, typename Allocator>
constexpr auto Vector<T, Allocator>::operator[](this Vector& self, size_type position) noexcept -> reference {
    pltxt2htm_assert(position < self.size(), u8"Index of Vector out of bound");
    return self.begin_pointer[position];
}

template<typename T, typename Allocator>
constexpr auto Vector<T, Allocator>::operator[](this Vector const& self, size_type position) noexcept
    -> const_reference {
    pltxt2htm_assert(position < self.size(), u8"Index of Vector out of bound");
    return self.begin_pointer[position];
}

template<typename T, typename Allocator>
constexpr void Vector<T, Allocator>::reserve(this Vector& self, size_type requested_capacity) noexcept
    requires (::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_destructible_v<value_type>)
{
    pltxt2htm_assert(requested_capacity <= self.max_size(), u8"Vector capacity exceeds max_size");
    if (requested_capacity <= self.capacity()) {
        return;
    }
    self.reallocate(requested_capacity);
}

template<typename T, typename Allocator>
constexpr void Vector<T, Allocator>::pop_back(this Vector& self) noexcept
    requires ::std::is_nothrow_destructible_v<value_type>
{
    pltxt2htm_assert(!self.is_empty(), u8"Popping back but Vector is empty");
    --self.current_size;
    ::std::destroy_at(self.begin_pointer + self.current_size);
}

} // namespace pltxt2htm::container

#include "../../details/pop_macro.hh"
