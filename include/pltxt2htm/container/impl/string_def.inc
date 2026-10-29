/**
 * @file string_def.inc
 * @brief Out-of-line contract-checked definitions for container::BasicString.
 */

#pragma once

#include "string_decl.hh"
#include "../../details/push_macro.hh"

namespace pltxt2htm::container {

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr void BasicString<CharType, Allocator>::construct_zero(this BasicString& self, size_type count) noexcept {
    pltxt2htm_assert(count != 0, u8"BasicString capacity can not be zero");
    pltxt2htm_assert(count < self.max_size(), u8"BasicString capacity is too large");
    if consteval {
        auto [new_pointer, allocated_size] = typed_allocator_type::allocate_at_least(count + 1);
        self.start_lifetime(new_pointer, allocated_size);
        self.begin_pointer = new_pointer;
        self.current_pointer = new_pointer + count;
        self.end_pointer = new_pointer + static_cast<size_type>(allocated_size - 1);
    }
    else {
        pointer const new_pointer{typed_allocator_type::allocate_zero(count + 1)};
        self.begin_pointer = new_pointer;
        self.current_pointer = new_pointer + count;
        self.end_pointer = new_pointer + count;
    }
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr void BasicString<CharType, Allocator>::construct(this BasicString& self, const_pointer first,
                                                           size_type count) noexcept {
    if (count == 0) {
        self.reset();
        return;
    }
    pltxt2htm_assert(count < self.max_size(), u8"BasicString capacity is too large");
    auto [new_pointer, allocated_size] = typed_allocator_type::allocate_at_least(count + 1);
    self.start_lifetime(new_pointer, allocated_size);
    ::std::copy_n(first, count, new_pointer);
    self.begin_pointer = new_pointer;
    self.current_pointer = new_pointer + count;
    self.end_pointer = new_pointer + static_cast<size_type>(allocated_size - 1);
    *self.current_pointer = value_type{};
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr void BasicString<CharType, Allocator>::append_reallocate(this BasicString& self, const_pointer source,
                                                                   size_type count) noexcept {
    size_type const old_size{self.size()};
    pltxt2htm_assert(count < self.max_size() - old_size, u8"BasicString capacity is too large");
    size_type const new_size{old_size + count};
    size_type const new_capacity{self.grown_capacity(new_size)};
    auto [new_pointer, allocated_size] = typed_allocator_type::allocate_at_least(new_capacity + 1);
    self.start_lifetime(new_pointer, allocated_size);
    ::std::copy(self.begin_pointer, self.current_pointer, new_pointer);
    ::std::copy_n(source, count, new_pointer + old_size);
    new_pointer[new_size] = value_type{};
    self.destroy();
    self.begin_pointer = new_pointer;
    self.current_pointer = new_pointer + new_size;
    self.end_pointer = new_pointer + static_cast<size_type>(allocated_size - 1);
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto BasicString<CharType, Allocator>::insert_reallocate(this BasicString& self, size_type position_index,
                                                                   string_view_type string) noexcept -> iterator {
    size_type const old_size{self.size()};
    pltxt2htm_assert(string.size() < self.max_size() - old_size, u8"BasicString capacity is too large");
    size_type const new_size{old_size + string.size()};
    size_type const new_capacity{self.grown_capacity(new_size)};
    auto [new_pointer, allocated_size] = typed_allocator_type::allocate_at_least(new_capacity + 1);
    self.start_lifetime(new_pointer, allocated_size);
    ::std::copy_n(self.begin_pointer, position_index, new_pointer);
    ::std::copy(string.begin(), string.end(), new_pointer + position_index);
    ::std::copy(self.begin_pointer + position_index, self.current_pointer,
                new_pointer + position_index + string.size());
    new_pointer[new_size] = value_type{};
    self.destroy();
    self.begin_pointer = new_pointer;
    self.current_pointer = new_pointer + new_size;
    self.end_pointer = new_pointer + static_cast<size_type>(allocated_size - 1);
    return new_pointer + position_index;
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr void BasicString<CharType, Allocator>::ensure_capacity(this BasicString& self,
                                                                 size_type required_capacity) noexcept {
    if (required_capacity <= self.capacity()) {
        return;
    }
    pltxt2htm_assert(required_capacity < self.max_size(), u8"BasicString capacity is too large");

    self.reserve(self.grown_capacity(required_capacity));
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto BasicString<CharType, Allocator>::operator=(this BasicString& self, BasicString const& other) noexcept
    -> BasicString& {
#if defined(PLTXT2HTM_INTERNAL_USE)
    pltxt2htm_assert(::std::addressof(self) != ::std::addressof(other), u8"Internal self-assignment is not allowed");
#else
    if (::std::addressof(self) == ::std::addressof(other)) {
        return self;
    }
#endif
    self.assign_impl(other.data(), other.size());
    return self;
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto BasicString<CharType, Allocator>::operator=(this BasicString& self, BasicString&& other) noexcept
    -> BasicString& {
#if defined(PLTXT2HTM_INTERNAL_USE)
    pltxt2htm_assert(::std::addressof(self) != ::std::addressof(other),
                     u8"Internal self-move-assignment is not allowed");
#else
    if (::std::addressof(self) == ::std::addressof(other)) {
        return self;
    }
#endif
    self.destroy();
    self.begin_pointer = other.begin_pointer;
    self.current_pointer = other.current_pointer;
    self.end_pointer = other.end_pointer;
    other.reset();
    return self;
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto BasicString<CharType, Allocator>::operator[](this BasicString& self, size_type position) noexcept
    -> reference {
    pltxt2htm_assert(position < self.size(), u8"Index of BasicString out of bound");
    return self.begin_pointer[position];
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto BasicString<CharType, Allocator>::operator[](this BasicString const& self, size_type position) noexcept
    -> const_reference {
    pltxt2htm_assert(position < self.size(), u8"Index of BasicString out of bound");
    return self.begin_pointer[position];
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto BasicString<CharType, Allocator>::back(this BasicString& self) noexcept -> reference {
    pltxt2htm_assert(!self.is_empty(), u8"back() called on empty BasicString");
    return self.current_pointer[-1];
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto BasicString<CharType, Allocator>::back(this BasicString const& self) noexcept -> const_reference {
    pltxt2htm_assert(!self.is_empty(), u8"back() called on empty BasicString");
    return self.current_pointer[-1];
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr void BasicString<CharType, Allocator>::reserve(this BasicString& self,
                                                         size_type requested_capacity) noexcept {
    if (requested_capacity <= self.capacity()) {
        return;
    }
    pltxt2htm_assert(requested_capacity < self.max_size(), u8"BasicString capacity is too large");

    size_type const old_size{self.size()};
    size_type const old_capacity{self.capacity()};
    pointer new_pointer{};
    size_type allocated_size{};
    if consteval {
        auto const allocation = typed_allocator_type::allocate_at_least(requested_capacity + 1);
        new_pointer = allocation.ptr;
        allocated_size = allocation.count;
        self.start_lifetime(new_pointer, allocated_size);
        ::std::copy(self.begin_pointer, self.current_pointer, new_pointer);
        if (old_capacity != 0) {
            typed_allocator_type::deallocate_n(self.begin_pointer, old_capacity + 1);
        }
    }
    else {
        pointer const allocation_pointer{old_capacity == 0 ? nullptr : self.begin_pointer};
        if constexpr (typed_allocator_type::has_reallocate) {
            auto const allocation =
                typed_allocator_type::reallocate_at_least(allocation_pointer, requested_capacity + 1);
            new_pointer = allocation.ptr;
            allocated_size = allocation.count;
        }
        else {
            auto allocation = typed_allocator_type::reallocate_n_at_least(
                allocation_pointer, old_capacity == 0 ? 0 : old_capacity + 1, requested_capacity + 1);
            new_pointer = allocation.ptr;
            allocated_size = allocation.count;
        }
    }
    self.begin_pointer = new_pointer;
    self.current_pointer = new_pointer + old_size;
    self.end_pointer = new_pointer + static_cast<size_type>(allocated_size - 1);
    *self.current_pointer = value_type{};
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr void BasicString<CharType, Allocator>::pop_back(this BasicString& self) noexcept {
    pltxt2htm_assert(!self.is_empty(), u8"pop_back() called on empty BasicString");
    --self.current_pointer;
    *self.current_pointer = value_type{};
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto BasicString<CharType, Allocator>::insert(this BasicString& self, const_iterator position,
                                                        string_view_type string) noexcept -> iterator {
    pltxt2htm_assert(position >= self.begin_pointer && position <= self.current_pointer,
                     u8"BasicString insert position out of bound");
    size_type const position_index{static_cast<size_type>(position - self.begin_pointer)};
    if (string.is_empty()) {
        return self.begin_pointer + position_index;
    }

    if (string.size() > static_cast<size_type>(self.end_pointer - self.current_pointer)) {
        return self.insert_reallocate(position_index, string);
    }
    size_type const old_size{self.size()};
    size_type const source_index{self.source_offset(string.data())};
    pointer const insertion_pointer{self.begin_pointer + position_index};
    ::std::move_backward(insertion_pointer, self.begin_pointer + old_size,
                         self.begin_pointer + old_size + string.size());
    const_pointer source{string.data()};
    if (source_index != self.npos && source_index >= position_index) {
        source = self.begin_pointer + source_index + string.size();
    }
    if consteval {
        if (source_index != self.npos && source_index < position_index &&
            position_index < source_index + string.size()) {
            ::std::copy_backward(source, source + string.size(), insertion_pointer + string.size());
        }
        else {
            ::std::copy_n(source, string.size(), insertion_pointer);
        }
    }
    else {
        ::fast_io::freestanding::overlapped_copy_n(source, string.size(), insertion_pointer);
    }
    self.current_pointer = self.begin_pointer + old_size + string.size();
    *self.current_pointer = value_type{};
    return insertion_pointer;
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto BasicString<CharType, Allocator>::source_offset(this BasicString const& self,
                                                               const_pointer source) noexcept -> size_type {
    if consteval {
        for (size_type index{}; index <= self.size(); ++index) {
            if (source == self.begin_pointer + index) {
                return index;
            }
        }
        return self.npos;
    }
    else {
        ::std::less<const_pointer> const less{};
        if (!less(source, self.begin_pointer) && !less(self.current_pointer, source)) {
            return static_cast<size_type>(source - self.begin_pointer);
        }
        return self.npos;
    }
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto BasicString<CharType, Allocator>::grown_capacity(this BasicString const& self,
                                                                size_type required_capacity) noexcept -> size_type {
    size_type new_capacity{self.capacity()};
    if (new_capacity == 0) {
        new_capacity = 1;
    }
    while (new_capacity < required_capacity) {
        size_type const remaining{self.max_size() - new_capacity};
        if (new_capacity > remaining) {
            return required_capacity;
        }
        new_capacity *= 2;
    }
    return new_capacity;
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr void BasicString<CharType, Allocator>::assign_impl(this BasicString& self, const_pointer source,
                                                             size_type count) noexcept {
    if (count == 0) {
        self.clear();
        return;
    }
    if (count > self.capacity()) {
        self.reserve(count);
    }

    if consteval {
        for (size_type index{}; index != count; ++index) {
            self.begin_pointer[index] = source[index];
        }
    }
    else {
        ::fast_io::freestanding::overlapped_copy_n(source, count, self.begin_pointer);
    }
    self.current_pointer = self.begin_pointer + count;
    *self.current_pointer = value_type{};
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr void BasicString<CharType, Allocator>::assign(this BasicString& self, string_view_type string) noexcept {
    self.assign_impl(string.data(), string.size());
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr void BasicString<CharType, Allocator>::assign(this BasicString& self, const_pointer first,
                                                        const_pointer last) noexcept {
    self.assign_impl(first, static_cast<size_type>(last - first));
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr void BasicString<CharType, Allocator>::assign_characters(this BasicString& self, size_type count,
                                                                   value_type character) noexcept {
    if (count == 0) {
        self.clear();
        return;
    }
    self.ensure_capacity(count);
    ::std::fill_n(self.begin_pointer, count, character);
    self.current_pointer = self.begin_pointer + count;
    *self.current_pointer = value_type{};
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr void BasicString<CharType, Allocator>::append(this BasicString& self, const_pointer first,
                                                        const_pointer last) noexcept {
    size_type const count{static_cast<size_type>(last - first)};
    if (count == 0) {
        return;
    }
    size_type const old_size{self.size()};
    if (count > static_cast<size_type>(self.end_pointer - self.current_pointer)) {
        self.append_reallocate(first, count);
        return;
    }
    if consteval {
        for (size_type index{}; index != count; ++index) {
            self.current_pointer[index] = first[index];
        }
    }
    else {
        ::fast_io::freestanding::overlapped_copy_n(first, count, self.current_pointer);
    }
    self.current_pointer = self.begin_pointer + old_size + count;
    *self.current_pointer = value_type{};
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr void BasicString<CharType, Allocator>::append(this BasicString& self, const_pointer first,
                                                        size_type count) noexcept {
    self.append(first, first + count);
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr void BasicString<CharType, Allocator>::append(this BasicString& self, string_view_type string) noexcept {
    self.append(string.data(), string.data() + string.size());
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr void BasicString<CharType, Allocator>::append(this BasicString& self, BasicString const& string) noexcept {
    self.append(string.data(), string.data() + string.size());
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
template<::std::size_t size_with_null>
constexpr void BasicString<CharType, Allocator>::append(this BasicString& self,
                                                        value_type const (&string)[size_with_null]) noexcept {
    static_assert(size_with_null != 0);
    self.append(string, string + size_with_null - 1);
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
template<::std::size_t literal_size>
constexpr void BasicString<CharType, Allocator>::append(
    this BasicString& self, ::pltxt2htm::details::BasicLiteralString<value_type, literal_size> const& string) noexcept {
    self.append(string.data(), string.data() + string.size());
}

} // namespace pltxt2htm::container

#include "../../details/pop_macro.hh"
