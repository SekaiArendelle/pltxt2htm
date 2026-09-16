/**
 * @file vector.hh
 * @brief Dynamically sized contiguous containers for pltxt2htm.
 */

#pragma once

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
 * Element and range operations used by Vector must not throw.
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

    // Keep size and capacity as element counts instead of past-the-end pointers.
    // This makes size() and capacity() direct loads; boundary pointers are computed
    // only by operations that actually need them.
    pointer begin_pointer{nullptr};
    size_type current_size{};
    size_type current_capacity{};

    constexpr void release(this Vector& self) noexcept {
        if (self.begin_pointer == nullptr) {
            return;
        }
        ::std::destroy(self.begin_pointer, self.end());
        typed_allocator_type::deallocate_n(self.begin_pointer, self.current_capacity);
        self.begin_pointer = nullptr;
        self.current_size = 0;
        self.current_capacity = 0;
    }

    template<typename R>
    [[nodiscard]]
    static consteval auto is_nothrow_append_range() noexcept -> bool {
        using iterator_type = ::std::ranges::iterator_t<R>;
        using sentinel_type = ::std::ranges::sentinel_t<R>;
        if constexpr (!requires(R& range, iterator_type& iterator, sentinel_type& sentinel) {
                          { ::std::ranges::begin(range) } noexcept;
                          { ::std::ranges::end(range) } noexcept;
                          { *iterator } noexcept;
                          { ++iterator } noexcept;
                          { static_cast<bool>(iterator != sentinel) } noexcept -> ::std::same_as<bool>;
                      }) {
            return false;
        }

        using range_reference = ::std::ranges::range_reference_t<R>;
        if constexpr (::std::is_lvalue_reference_v<R>) {
            using forwarded_reference = decltype(::std::forward<range_reference>(::std::declval<range_reference>()));
            return ::std::is_nothrow_constructible_v<value_type, forwarded_reference>;
        }
        else {
            static_assert(::std::is_rvalue_reference_v<R&&>);
            using forwarded_reference = decltype(::std::forward_like<R>(::std::declval<range_reference>()));
            return ::std::is_nothrow_constructible_v<value_type, forwarded_reference>;
        }
    }

    template<::std::ranges::forward_range R>
    [[nodiscard]]
    static constexpr auto count_range(R& range) noexcept -> size_type {
        if constexpr (requires {
                          { ::std::ranges::size(range) } noexcept;
                      }) {
            if !consteval {
                return static_cast<size_type>(::std::ranges::size(range));
            }
        }

        size_type result{};
        auto iterator = ::std::ranges::begin(range);
        auto sentinel = ::std::ranges::end(range);
        for (; iterator != sentinel; ++iterator) {
            ++result;
        }
        return result;
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto growth_capacity(this Vector const& self, size_type additional_size) noexcept -> size_type {
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

    constexpr void reallocate(this Vector& self, size_type requested_capacity) noexcept
        requires (::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
        auto const allocation = typed_allocator_type::allocate_at_least(requested_capacity);
        pointer new_current{allocation.ptr};
        pointer const old_end{self.end()};

        for (pointer source{self.begin_pointer}; source != old_end; ++source) {
            ::std::construct_at(new_current, ::std::move(*source));
            ++new_current;
        }

        pointer const old_begin{self.begin_pointer};
        size_type const old_capacity{self.current_capacity};
        self.begin_pointer = allocation.ptr;
        self.current_capacity = allocation.count;

        if (old_begin != nullptr) {
            ::std::destroy(old_begin, old_end);
            typed_allocator_type::deallocate_n(old_begin, old_capacity);
        }
    }

    template<::pltxt2htm::Contracts ndebug, typename... Args>
    constexpr auto reallocate_and_emplace(this Vector& self, Args&&... args) noexcept -> reference
        requires (::std::is_nothrow_move_constructible_v<value_type> &&
                  ::std::is_nothrow_constructible_v<value_type, Args...> &&
                  ::std::is_nothrow_destructible_v<value_type>)
    {
        size_type const old_size{self.size()};
        auto const allocation = typed_allocator_type::allocate_at_least(self.template growth_capacity<ndebug>(1));
        pointer const new_element{allocation.ptr + old_size};
        ::std::construct_at(new_element, ::std::forward<Args>(args)...);
        pointer new_current{allocation.ptr};
        pointer const old_end{self.end()};

        for (pointer source{self.begin_pointer}; source != old_end; ++source) {
            ::std::construct_at(new_current, ::std::move(*source));
            ++new_current;
        }

        pointer const old_begin{self.begin_pointer};
        size_type const old_capacity{self.current_capacity};
        self.begin_pointer = allocation.ptr;
        self.current_size = old_size + 1;
        self.current_capacity = allocation.count;

        if (old_begin != nullptr) {
            ::std::destroy(old_begin, old_end);
            typed_allocator_type::deallocate_n(old_begin, old_capacity);
        }
        return *new_element;
    }

    template<::pltxt2htm::Contracts ndebug, typename R>
    constexpr void reallocate_and_append_range(this Vector& self, R&& range, size_type range_size,
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

    template<::pltxt2htm::Contracts ndebug>
    constexpr void ensure_additional_capacity(this Vector& self, size_type additional_size) noexcept
        requires (::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
        size_type const old_capacity{self.capacity()};
        size_type const new_capacity{self.template growth_capacity<ndebug>(additional_size)};
        if (new_capacity == old_capacity) {
            return;
        }
        self.reallocate(new_capacity);
    }

public:
    constexpr Vector() noexcept = default;

    constexpr Vector(::std::initializer_list<value_type> values) noexcept {
        static_assert(::std::is_nothrow_copy_constructible_v<value_type>,
                      "Vector requires nothrow-copy-constructible elements when initialized from a list");
        static_assert(::std::is_nothrow_destructible_v<value_type>, "Vector requires nothrow-destructible elements");
        if (values.size() == 0) {
            return;
        }

        auto const allocation = typed_allocator_type::allocate_at_least(values.size());
        pointer new_current{allocation.ptr};
        for (auto const& value : values) {
            ::std::construct_at(new_current, value);
            ++new_current;
        }
        begin_pointer = allocation.ptr;
        current_size = values.size();
        current_capacity = allocation.count;
    }

    constexpr Vector(Vector const& other) noexcept {
        static_assert(::std::is_nothrow_copy_constructible_v<value_type>,
                      "Vector requires nothrow-copy-constructible elements when copied");
        static_assert(::std::is_nothrow_destructible_v<value_type>, "Vector requires nothrow-destructible elements");
        size_type const other_size{other.size()};
        if (other_size == 0) {
            return;
        }

        auto const allocation = typed_allocator_type::allocate_at_least(other_size);
        pointer new_current{allocation.ptr};
        for (auto const& value : other) {
            ::std::construct_at(new_current, value);
            ++new_current;
        }
        begin_pointer = allocation.ptr;
        current_size = other_size;
        current_capacity = allocation.count;
    }

    constexpr Vector(Vector&& other) noexcept
        : begin_pointer{::std::exchange(other.begin_pointer, nullptr)},
          current_size{::std::exchange(other.current_size, 0)},
          current_capacity{::std::exchange(other.current_capacity, 0)} {
    }

    constexpr auto operator=(this Vector& self, Vector const& other) noexcept -> Vector& {
        static_assert(::std::is_nothrow_copy_constructible_v<value_type>,
                      "Vector requires nothrow-copy-constructible elements when copied");
        static_assert(::std::is_nothrow_destructible_v<value_type>, "Vector requires nothrow-destructible elements");
        if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
            return self;
        }
        Vector temporary{other};
        self.swap(temporary);
        return self;
    }

    constexpr auto operator=(this Vector& self, Vector&& other) noexcept -> Vector& {
        static_assert(::std::is_nothrow_destructible_v<value_type>, "Vector requires nothrow-destructible elements");
        if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
            return self;
        }
        self.swap(other);
        return self;
    }

    constexpr ~Vector() noexcept {
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

    // TODO rename to `is_empty`
    [[nodiscard]]
    constexpr auto empty(this Vector const& self) noexcept -> bool {
        return self.current_size == 0;
    }

    [[nodiscard]]
    constexpr auto size(this Vector const& self) noexcept -> size_type {
        return self.current_size;
    }

    [[nodiscard]]
    constexpr auto capacity(this Vector const& self) noexcept -> size_type {
        return self.current_capacity;
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
        if (self.begin_pointer == nullptr) {
            return nullptr;
        }
        return self.begin_pointer + self.current_size;
    }

    [[nodiscard]]
    constexpr auto end(this Vector const& self) noexcept -> const_iterator {
        if (self.begin_pointer == nullptr) {
            return nullptr;
        }
        return self.begin_pointer + self.current_size;
    }

    [[nodiscard]]
    constexpr auto cbegin(this Vector const& self) noexcept -> const_iterator {
        return self.begin_pointer;
    }

    [[nodiscard]]
    constexpr auto cend(this Vector const& self) noexcept -> const_iterator {
        return self.end();
    }

    [[nodiscard]]
    constexpr auto rbegin(this Vector& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto rbegin(this Vector const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.end()};
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
    constexpr void reserve(this Vector& self, size_type requested_capacity) noexcept
        requires (::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
        pltxt2htm_assert(requested_capacity <= self.max_size(), u8"Vector capacity exceeds max_size");
        if (requested_capacity <= self.capacity()) {
            return;
        }
        self.reallocate(requested_capacity);
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, typename... Args>
        requires (::std::is_nothrow_move_constructible_v<value_type> &&
                  ::std::is_nothrow_constructible_v<value_type, Args...> &&
                  ::std::is_nothrow_destructible_v<value_type>)
    constexpr auto emplace_back(this Vector& self, Args&&... args) noexcept -> reference {
        if (self.current_size != self.current_capacity) {
            pointer const element{
                ::std::construct_at(self.begin_pointer + self.current_size, ::std::forward<Args>(args)...)};
            ++self.current_size;
            return *element;
        }
        return self.template reallocate_and_emplace<ndebug>(::std::forward<Args>(args)...);
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void push_back(this Vector& self, const_reference value) noexcept
        requires (::std::is_nothrow_move_constructible_v<value_type> &&
                  ::std::is_nothrow_copy_constructible_v<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
        self.template emplace_back<ndebug>(value);
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void push_back(this Vector& self, value_type&& value) noexcept
        requires (::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
        self.template emplace_back<ndebug>(::std::move(value));
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void pop_back(this Vector& self) noexcept
        requires ::std::is_nothrow_destructible_v<value_type>
    {
        pltxt2htm_assert(!self.empty(), u8"Popping back but Vector is empty");
        --self.current_size;
        ::std::destroy_at(self.begin_pointer + self.current_size);
    }

    constexpr void clear(this Vector& self) noexcept
        requires ::std::is_nothrow_destructible_v<value_type>
    {
        if (self.begin_pointer == nullptr) {
            return;
        }
        ::std::destroy(self.begin_pointer, self.end());
        self.current_size = 0;
    }

    /**
     * @pre A single-pass input range must not reference elements in this Vector.
     */
    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, ::std::ranges::input_range R>
    constexpr void append_range(this Vector& self, R&& range) noexcept
        requires (::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_destructible_v<value_type> &&
                  is_nothrow_append_range<R>())
    {
        if constexpr (::std::ranges::forward_range<R>) {
            size_type const range_size{count_range(range)};
            size_type const old_capacity{self.capacity()};
            size_type const new_capacity{self.template growth_capacity<ndebug>(range_size)};
            if (new_capacity != old_capacity) {
                self.template reallocate_and_append_range<ndebug>(::std::forward<R>(range), range_size, new_capacity);
                return;
            }
        }

        for (auto&& value : range) {
            if constexpr (::std::is_lvalue_reference_v<R&&>) {
                self.template emplace_back<ndebug>(::std::forward<decltype(value)>(value));
            }
            else {
                static_assert(::std::is_rvalue_reference_v<R&&>);
                self.template emplace_back<ndebug>(::std::forward_like<R>(value));
            }
        }
    }

    constexpr auto erase(this Vector& self, const_iterator position) noexcept -> iterator
        requires (::std::is_nothrow_move_assignable_v<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
        difference_type const offset{position - self.begin_pointer};
        pointer destination{self.begin_pointer + offset};
        pointer source{destination + 1};
        pointer const old_end{self.end()};
        for (; source != old_end; ++source, ++destination) {
            *destination = ::std::move(*source);
        }
        --self.current_size;
        ::std::destroy_at(self.begin_pointer + self.current_size);
        return self.begin_pointer + offset;
    }

    constexpr auto erase(this Vector& self, const_iterator first, const_iterator last) noexcept -> iterator
        requires (::std::is_nothrow_move_assignable_v<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
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
        pointer const old_end{self.end()};
        for (; source != old_end; ++source, ++destination) {
            *destination = ::std::move(*source);
        }
        ::std::destroy(destination, old_end);
        self.current_size -= static_cast<size_type>(last_offset - first_offset);
        return result;
    }

    constexpr void swap(this Vector& self, Vector& other) noexcept {
        pointer const begin_pointer{self.begin_pointer};
        size_type const current_size{self.current_size};
        size_type const current_capacity{self.current_capacity};
        self.begin_pointer = other.begin_pointer;
        self.current_size = other.current_size;
        self.current_capacity = other.current_capacity;
        other.begin_pointer = begin_pointer;
        other.current_size = current_size;
        other.current_capacity = current_capacity;
    }

    [[nodiscard]]
    constexpr auto operator==(this Vector const& self, Vector const& other) noexcept -> bool
        requires requires(const_reference left, const_reference right) {
            { static_cast<bool>(left == right) } noexcept -> ::std::same_as<bool>;
        }
    {
        if (self.size() != other.size()) {
            return false;
        }
        const_iterator left{self.begin_pointer};
        const_iterator right{other.begin_pointer};
        const_iterator const left_end{self.end()};
        for (; left != left_end; ++left, ++right) {
            if (!static_cast<bool>(*left == *right)) {
                return false;
            }
        }
        return true;
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
