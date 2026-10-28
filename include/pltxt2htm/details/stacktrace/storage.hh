/**
 * @file storage.hh
 * @brief Minimal growable array owned by the stacktrace implementation.
 * @details Mirrors only the subset of `container::Vector` that stacktrace needs,
 *          so the panic path can include stacktrace headers without pulling in
 *          the container layer (which includes the assert/panic machinery).
 */

#pragma once

#include <cstddef>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>

#include <fast_io/fast_io_core.h>

namespace pltxt2htm::details::stacktrace {

/**
 * @brief Contiguous owning array for stacktrace internals.
 * @tparam T Element type; it must be nothrow move constructible and destructible.
 * @tparam Allocator Untyped fast_io allocator used for storage.
 * @details Allocation failure terminates, matching fast_io containers. The class
 *          intentionally omits element insertion by copy, iteration helpers, and
 *          contract checks that stacktrace does not need.
 */
template<typename T, typename Allocator>
class Buffer {
    static_assert(::std::is_nothrow_move_constructible_v<T>, "stacktrace storage requires nothrow-movable elements");
    static_assert(::std::is_nothrow_destructible_v<T>, "stacktrace storage requires nothrow-destructible elements");

public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = ::std::size_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;

private:
    using storage_allocator = ::fast_io::generic_allocator_adapter<allocator_type>;
    using typed_allocator_type = ::fast_io::typed_generic_allocator_adapter<storage_allocator, value_type>;

    pointer begin_pointer{};
    size_type current_size{};
    size_type current_capacity{};

    constexpr void release(this Buffer& self) noexcept {
        if (self.begin_pointer == nullptr) {
            return;
        }
        ::std::destroy(self.begin_pointer, self.begin_pointer + self.current_size);
        typed_allocator_type::deallocate_n(self.begin_pointer, self.current_capacity);
        self.begin_pointer = nullptr;
        self.current_size = 0;
        self.current_capacity = 0;
    }

    [[nodiscard]]
    static constexpr auto growth_capacity(size_type old_capacity, size_type required_capacity) noexcept -> size_type {
        if (required_capacity <= old_capacity) {
            return old_capacity;
        }
        if (old_capacity == 0) {
            return required_capacity < 2 ? 2 : required_capacity;
        }
        auto const limit = max_size();
        size_type new_capacity = old_capacity <= limit / 2 ? old_capacity * 2 : limit;
        return new_capacity < required_capacity ? required_capacity : new_capacity;
    }

public:
    constexpr Buffer() noexcept = default;

    constexpr Buffer(Buffer const& other) noexcept {
        static_assert(::std::is_nothrow_copy_constructible_v<value_type>,
                      "stacktrace storage requires nothrow-copy-constructible elements for copies");
        if (other.current_size == 0) {
            return;
        }
        auto const allocation = typed_allocator_type::allocate_at_least(other.current_size);
        for (size_type index{}; index < other.current_size; ++index) {
            ::std::construct_at(allocation.ptr + index, other.begin_pointer[index]);
        }
        begin_pointer = allocation.ptr;
        current_size = other.current_size;
        current_capacity = allocation.count;
    }

    constexpr Buffer(Buffer&& other) noexcept
        : begin_pointer{::std::exchange(other.begin_pointer, nullptr)},
          current_size{::std::exchange(other.current_size, 0)},
          current_capacity{::std::exchange(other.current_capacity, 0)} {
    }

    constexpr ~Buffer() {
        release();
    }

    constexpr auto operator=(this Buffer& self, Buffer const& other) noexcept -> Buffer& {
        if (::std::addressof(self) == ::std::addressof(other)) {
            return self;
        }
        Buffer temporary{other};
        self.swap(temporary);
        return self;
    }

    constexpr auto operator=(this Buffer& self, Buffer&& other) noexcept -> Buffer& {
        if (::std::addressof(self) == ::std::addressof(other)) {
            return self;
        }
        self.release();
        self.begin_pointer = ::std::exchange(other.begin_pointer, nullptr);
        self.current_size = ::std::exchange(other.current_size, 0);
        self.current_capacity = ::std::exchange(other.current_capacity, 0);
        return self;
    }

    [[nodiscard]]
    constexpr auto data(this Buffer& self) noexcept -> pointer {
        return self.begin_pointer;
    }

    [[nodiscard]]
    constexpr auto data(this Buffer const& self) noexcept -> const_pointer {
        return self.begin_pointer;
    }

    [[nodiscard]]
    constexpr auto size(this Buffer const& self) noexcept -> size_type {
        return self.current_size;
    }

    [[nodiscard]]
    constexpr auto empty(this Buffer const& self) noexcept -> bool {
        return self.current_size == 0;
    }

    [[nodiscard]]
    static constexpr auto max_size() noexcept -> size_type {
        return (::std::numeric_limits<size_type>::max)() / sizeof(value_type);
    }

    [[nodiscard]]
    constexpr auto operator[](this Buffer const& self, size_type index) noexcept -> const_reference {
        return self.begin_pointer[index];
    }

    constexpr void reserve(this Buffer& self, size_type requested_capacity) noexcept {
        if (requested_capacity <= self.current_capacity) {
            return;
        }
        auto const allocation = typed_allocator_type::allocate_at_least(requested_capacity);
        for (size_type index{}; index < self.current_size; ++index) {
            ::std::construct_at(allocation.ptr + index, ::std::move(self.begin_pointer[index]));
        }
        pointer const old_begin{self.begin_pointer};
        size_type const old_size{self.current_size};
        size_type const old_capacity{self.current_capacity};
        self.begin_pointer = allocation.ptr;
        self.current_capacity = allocation.count;
        if (old_begin != nullptr) {
            ::std::destroy(old_begin, old_begin + old_size);
            typed_allocator_type::deallocate_n(old_begin, old_capacity);
        }
    }

    template<typename... Args>
    constexpr auto emplace_back(this Buffer& self, Args&&... args) noexcept -> reference {
        if (self.current_size == self.current_capacity) {
            self.reserve(growth_capacity(self.current_capacity, self.current_size + 1));
        }
        ::std::construct_at(self.begin_pointer + self.current_size, ::std::forward<Args>(args)...);
        ++self.current_size;
        return self.begin_pointer[self.current_size - 1];
    }

    constexpr void clear(this Buffer& self) noexcept {
        if (self.begin_pointer != nullptr) {
            ::std::destroy(self.begin_pointer, self.begin_pointer + self.current_size);
        }
        self.current_size = 0;
    }

    constexpr void swap(this Buffer& self, Buffer& other) noexcept {
        ::std::swap(self.begin_pointer, other.begin_pointer);
        ::std::swap(self.current_size, other.current_size);
        ::std::swap(self.current_capacity, other.current_capacity);
    }
};

template<typename T, typename Allocator>
constexpr void swap(Buffer<T, Allocator>& left, Buffer<T, Allocator>& right) noexcept {
    left.swap(right);
}

} // namespace pltxt2htm::details::stacktrace
