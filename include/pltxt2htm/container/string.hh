/**
 * @file string.hh
 * @brief Dynamically allocated strings for pltxt2htm.
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <limits>
#include <memory>
#include <utility>

#include <fast_io/fast_io_dsal/string.h>

#include "../contracts.hh"
#include "../details/concepts.hh"
#include "../details/literal_string.hh"
#include "../details/push_macro.hh"
#include "string_view.hh"

namespace pltxt2htm::container {

/**
 * @brief An owning, null-terminated contiguous character sequence.
 * @tparam CharType Character type stored by the string.
 * @tparam Allocator fast_io-compatible allocator used for dynamic storage.
 */
template<::pltxt2htm::details::is_char_type CharType, typename Allocator = ::fast_io::native_global_allocator>
class BasicString {
public:
    using allocator_type = Allocator;
    using value_type = CharType;
    using char_type = value_type;
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
    using string_view_type = BasicStringView<value_type>;

private:
    using untyped_allocator_type = ::fast_io::generic_allocator_adapter<allocator_type>;
    using typed_allocator_type = ::fast_io::typed_generic_allocator_adapter<untyped_allocator_type, value_type>;

    pointer begin_pointer;
    pointer current_pointer;
    pointer end_pointer;

    static constexpr void start_lifetime(pointer storage, size_type count) noexcept {
        if consteval {
            for (size_type index{}; index < count; ++index) {
                ::std::construct_at(storage + index);
            }
        }
    }

    constexpr void reset(this BasicString& self) noexcept {
        if consteval {
            auto [new_pointer, allocated_size] = typed_allocator_type::allocate_at_least(2);
            self.start_lifetime(new_pointer, allocated_size);
            self.begin_pointer = new_pointer;
            self.current_pointer = new_pointer;
            self.end_pointer = new_pointer + static_cast<size_type>(allocated_size - 1);
            *new_pointer = value_type{};
        }
        else {
            pointer const empty_string{const_cast<pointer>(::fast_io::null_terminated_c_str_v<value_type>)};
            self.begin_pointer = empty_string;
            self.current_pointer = empty_string;
            self.end_pointer = empty_string;
        }
    }

    constexpr void destroy(this BasicString& self) noexcept {
        if (self.begin_pointer != self.end_pointer) {
            typed_allocator_type::deallocate_n(self.begin_pointer, self.capacity() + 1);
        }
    }

    constexpr void allocate(this BasicString& self, size_type capacity) noexcept {
        // Work around constructors being unable to receive an explicitly selected Contracts template argument.
#ifndef NDEBUG
        constexpr auto ndebug{::pltxt2htm::Contracts::quick_enforce};
        pltxt2htm_assert(capacity < self.max_size(), u8"BasicString capacity is too large");
#endif
        auto [new_pointer, allocated_size] = typed_allocator_type::allocate_at_least(capacity + 1);
        self.start_lifetime(new_pointer, allocated_size);
        self.begin_pointer = new_pointer;
        self.current_pointer = new_pointer;
        self.end_pointer = new_pointer + static_cast<size_type>(allocated_size - 1);
        *new_pointer = value_type{};
    }

    constexpr void construct(this BasicString& self, const_pointer first, size_type count) noexcept {
        if (count == 0) {
            self.reset();
            return;
        }
        self.allocate(count);
        ::std::copy_n(first, count, self.begin_pointer);
        self.current_pointer = self.begin_pointer + count;
        *self.current_pointer = value_type{};
    }

    [[nodiscard]]
    constexpr auto source_offset(this BasicString const& self, const_pointer source) noexcept -> size_type {
        for (size_type index{}; index <= self.size(); ++index) {
            if (source == self.begin_pointer + index) {
                return index;
            }
        }
        return self.npos;
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void ensure_capacity(this BasicString& self, size_type required_capacity) noexcept {
        if (required_capacity <= self.capacity()) {
            return;
        }
        pltxt2htm_assert(required_capacity < self.max_size(), u8"BasicString capacity is too large");

        size_type new_capacity{self.capacity()};
        if (new_capacity == 0) {
            new_capacity = 1;
        }
        while (new_capacity < required_capacity) {
            size_type const remaining{self.max_size() - new_capacity};
            if (new_capacity > remaining) {
                new_capacity = required_capacity;
                break;
            }
            new_capacity *= 2;
        }
        self.reserve<ndebug>(new_capacity);
    }

    constexpr void assign_impl(this BasicString& self, const_pointer source, size_type count) noexcept {
        if (count == 0) {
            self.clear();
            return;
        }
        size_type const offset{self.source_offset(source)};
        if (count > self.capacity()) {
            BasicString replacement{source, source + count};
            self = ::std::move(replacement);
            return;
        }

        if (offset == self.npos || offset == 0) {
            ::std::copy_n(source, count, self.begin_pointer);
        }
        else {
            for (size_type index{}; index < count; ++index) {
                self.begin_pointer[index] = self.begin_pointer[offset + index];
            }
        }
        self.current_pointer = self.begin_pointer + count;
        *self.current_pointer = value_type{};
    }

public:
    static constexpr size_type npos{::std::numeric_limits<size_type>::max()};

    constexpr BasicString() noexcept {
        this->reset();
    }

    constexpr explicit BasicString(size_type count) noexcept {
        if (count == 0) {
            this->reset();
            return;
        }
        this->allocate(count);
        ::std::fill_n(this->begin_pointer, count, value_type{});
        this->current_pointer = this->begin_pointer + count;
        *this->current_pointer = value_type{};
    }

    constexpr explicit BasicString(size_type count, value_type character) noexcept {
        if (count == 0) {
            this->reset();
            return;
        }
        this->allocate(count);
        ::std::fill_n(this->begin_pointer, count, character);
        this->current_pointer = this->begin_pointer + count;
        *this->current_pointer = value_type{};
    }

    constexpr explicit BasicString(const_pointer first, const_pointer last) noexcept {
        this->construct(first, static_cast<size_type>(last - first));
    }

    constexpr explicit BasicString(string_view_type string) noexcept {
        this->construct(string.data(), string.size());
    }

    constexpr explicit BasicString(::fast_io::basic_string_view<value_type> string) noexcept
        : BasicString{string_view_type{string}} {
    }

    template<::std::size_t size_with_null>
    constexpr explicit BasicString(value_type const (&string)[size_with_null]) noexcept {
        static_assert(size_with_null != 0);
        this->construct(string, size_with_null - 1);
    }

    template<::std::size_t size>
    constexpr explicit BasicString(::pltxt2htm::details::BasicLiteralString<value_type, size> const& string) noexcept {
        this->construct(string.data(), string.size());
    }

    template<typename OtherAllocator>
    constexpr explicit BasicString(
        ::fast_io::containers::basic_string<value_type, OtherAllocator> const& string) noexcept {
        this->construct(string.data(), string.size());
    }

    constexpr BasicString(BasicString const& other) noexcept {
        this->construct(other.data(), other.size());
    }

    constexpr BasicString(BasicString&& other) noexcept {
        this->begin_pointer = other.begin_pointer;
        this->current_pointer = other.current_pointer;
        this->end_pointer = other.end_pointer;
        other.reset();
    }

    constexpr ~BasicString() {
        this->destroy();
    }

    constexpr auto operator=(this BasicString& self, BasicString const& other) noexcept -> BasicString& {
        if (::std::addressof(self) != ::std::addressof(other)) {
            self.assign_impl(other.data(), other.size());
        }
        return self;
    }

    constexpr auto operator=(this BasicString& self, BasicString&& other) noexcept -> BasicString& {
        if (::std::addressof(self) == ::std::addressof(other)) {
            return self;
        }
        self.destroy();
        self.begin_pointer = other.begin_pointer;
        self.current_pointer = other.current_pointer;
        self.end_pointer = other.end_pointer;
        other.reset();
        return self;
    }

    constexpr auto operator=(this BasicString& self, string_view_type string) noexcept -> BasicString& {
        self.assign_impl(string.data(), string.size());
        return self;
    }

    [[nodiscard]]
    constexpr auto data(this BasicString& self) noexcept -> pointer {
        return self.begin_pointer;
    }

    [[nodiscard]]
    constexpr auto data(this BasicString const& self) noexcept -> const_pointer {
        return self.begin_pointer;
    }

    [[nodiscard]]
    constexpr auto c_str(this BasicString const& self) noexcept -> const_pointer {
        return self.begin_pointer;
    }

    [[nodiscard]]
    constexpr auto size(this BasicString const& self) noexcept -> size_type {
        return static_cast<size_type>(self.current_pointer - self.begin_pointer);
    }

    [[nodiscard]]
    constexpr auto size_bytes(this BasicString const& self) noexcept -> size_type {
        return self.size() * sizeof(value_type);
    }

    [[nodiscard]]
    constexpr auto capacity(this BasicString const& self) noexcept -> size_type {
        return static_cast<size_type>(self.end_pointer - self.begin_pointer);
    }

    [[nodiscard]]
    constexpr auto capacity_bytes(this BasicString const& self) noexcept -> size_type {
        return self.capacity() * sizeof(value_type);
    }

    [[nodiscard]]
    static constexpr auto max_size() noexcept -> size_type {
        return ::std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    [[nodiscard]]
    constexpr auto empty(this BasicString const& self) noexcept -> bool {
        return self.begin_pointer == self.current_pointer;
    }

    [[nodiscard]]
    constexpr auto is_empty(this BasicString const& self) noexcept -> bool {
        return self.empty();
    }

    [[nodiscard]]
    constexpr auto begin(this BasicString& self) noexcept -> iterator {
        return self.begin_pointer;
    }

    [[nodiscard]]
    constexpr auto begin(this BasicString const& self) noexcept -> const_iterator {
        return self.begin_pointer;
    }

    [[nodiscard]]
    constexpr auto cbegin(this BasicString const& self) noexcept -> const_iterator {
        return self.begin_pointer;
    }

    [[nodiscard]]
    constexpr auto end(this BasicString& self) noexcept -> iterator {
        return self.current_pointer;
    }

    [[nodiscard]]
    constexpr auto end(this BasicString const& self) noexcept -> const_iterator {
        return self.current_pointer;
    }

    [[nodiscard]]
    constexpr auto cend(this BasicString const& self) noexcept -> const_iterator {
        return self.current_pointer;
    }

    [[nodiscard]]
    constexpr auto rbegin(this BasicString& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto rbegin(this BasicString const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto rend(this BasicString& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto rend(this BasicString const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.begin()};
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto index(this BasicString& self, size_type position) noexcept -> reference {
        pltxt2htm_assert(position < self.size(), u8"Index of BasicString out of bound");
        return self.begin_pointer[position];
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto index(this BasicString const& self, size_type position) noexcept -> const_reference {
        pltxt2htm_assert(position < self.size(), u8"Index of BasicString out of bound");
        return self.begin_pointer[position];
    }

    constexpr auto operator[](this BasicString& self, size_type position) noexcept -> reference = delete
#if __cpp_deleted_function >= 202403L
        ("operator[] is deleted; use index() instead for bounds-checked access")
#endif
        ;

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto front(this BasicString& self) noexcept -> reference {
        return self.index<ndebug>(0);
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto front(this BasicString const& self) noexcept -> const_reference {
        return self.index<ndebug>(0);
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto back(this BasicString& self) noexcept -> reference {
        pltxt2htm_assert(!self.empty(), u8"back() called on empty BasicString");
        return self.current_pointer[-1];
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto back(this BasicString const& self) noexcept -> const_reference {
        pltxt2htm_assert(!self.empty(), u8"back() called on empty BasicString");
        return self.current_pointer[-1];
    }

    constexpr void clear(this BasicString& self) noexcept {
        if (self.begin_pointer == self.end_pointer) {
            return;
        }
        self.current_pointer = self.begin_pointer;
        *self.current_pointer = value_type{};
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void reserve(this BasicString& self, size_type requested_capacity) noexcept {
        if (requested_capacity <= self.capacity()) {
            return;
        }
        pltxt2htm_assert(requested_capacity < self.max_size(), u8"BasicString capacity is too large");

        size_type const old_size{self.size()};
        auto [new_pointer, allocated_size] = typed_allocator_type::allocate_at_least(requested_capacity + 1);
        self.start_lifetime(new_pointer, allocated_size);
        ::std::copy(self.begin_pointer, self.current_pointer, new_pointer);
        new_pointer[old_size] = value_type{};
        if (self.begin_pointer != self.end_pointer) {
            typed_allocator_type::deallocate_n(self.begin_pointer, self.capacity() + 1);
        }
        self.begin_pointer = new_pointer;
        self.current_pointer = new_pointer + old_size;
        self.end_pointer = new_pointer + static_cast<size_type>(allocated_size - 1);
    }

    constexpr void assign(this BasicString& self, string_view_type string) noexcept {
        self.assign_impl(string.data(), string.size());
    }

    constexpr void assign(this BasicString& self, const_pointer first, const_pointer last) noexcept {
        self.assign_impl(first, static_cast<size_type>(last - first));
    }

    constexpr void assign_characters(this BasicString& self, size_type count,
                                     value_type character = value_type{}) noexcept {
        if (count == 0) {
            self.clear();
            return;
        }
        self.ensure_capacity<::pltxt2htm::Contracts::quick_enforce>(count);
        ::std::fill_n(self.begin_pointer, count, character);
        self.current_pointer = self.begin_pointer + count;
        *self.current_pointer = value_type{};
    }

    constexpr void assign_with_character(this BasicString& self, value_type character) noexcept {
        self.assign_characters(1, character);
    }

    constexpr void push_back(this BasicString& self, value_type character) noexcept {
        self.ensure_capacity<::pltxt2htm::Contracts::quick_enforce>(self.size() + 1);
        *self.current_pointer++ = character;
        *self.current_pointer = value_type{};
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void pop_back(this BasicString& self) noexcept {
        pltxt2htm_assert(!self.empty(), u8"pop_back() called on empty BasicString");
        --self.current_pointer;
        *self.current_pointer = value_type{};
    }

    constexpr void append(this BasicString& self, const_pointer first, const_pointer last) noexcept {
        size_type const count{static_cast<size_type>(last - first)};
        if (count == 0) {
            return;
        }
        size_type const old_size{self.size()};
        size_type const offset{self.source_offset(first)};
        self.ensure_capacity<::pltxt2htm::Contracts::quick_enforce>(old_size + count);
        if (offset != self.npos) {
            first = self.begin_pointer + offset;
        }
        ::std::copy_n(first, count, self.begin_pointer + old_size);
        self.current_pointer = self.begin_pointer + old_size + count;
        *self.current_pointer = value_type{};
    }

    constexpr void append(this BasicString& self, const_pointer first, size_type count) noexcept {
        self.append(first, first + count);
    }

    constexpr void append(this BasicString& self, string_view_type string) noexcept {
        self.append(string.data(), string.data() + string.size());
    }

    constexpr void append(this BasicString& self, BasicString const& string) noexcept {
        self.append(string.data(), string.data() + string.size());
    }

    template<::std::size_t size_with_null>
    constexpr void append(this BasicString& self, value_type const (&string)[size_with_null]) noexcept {
        static_assert(size_with_null != 0);
        self.append(string, string + size_with_null - 1);
    }

    template<::std::size_t size>
    constexpr void append(this BasicString& self,
                          ::pltxt2htm::details::BasicLiteralString<value_type, size> const& string) noexcept {
        self.append(string.data(), string.data() + string.size());
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr auto insert(this BasicString& self, const_iterator position, string_view_type string) noexcept
        -> iterator {
        pltxt2htm_assert(position >= self.begin_pointer && position <= self.current_pointer,
                         u8"BasicString insert position out of bound");
        size_type const position_index{static_cast<size_type>(position - self.begin_pointer)};
        if (string.empty()) {
            return self.begin_pointer + position_index;
        }

        size_type const source_index{self.source_offset(string.data())};
        if (source_index != self.npos) {
            BasicString const copy{string};
            return self.insert<ndebug>(self.begin_pointer + position_index, string_view_type{copy});
        }

        size_type const old_size{self.size()};
        self.ensure_capacity<ndebug>(old_size + string.size());
        pointer const insertion_pointer{self.begin_pointer + position_index};
        ::std::move_backward(insertion_pointer, self.begin_pointer + old_size,
                             self.begin_pointer + old_size + string.size());
        ::std::copy(string.begin(), string.end(), insertion_pointer);
        self.current_pointer = self.begin_pointer + old_size + string.size();
        *self.current_pointer = value_type{};
        return insertion_pointer;
    }

    [[nodiscard]]
    constexpr operator string_view_type() const noexcept {
        return string_view_type{this->begin_pointer, this->size()};
    }

    [[nodiscard]]
    constexpr operator ::fast_io::basic_string_view<value_type>() const noexcept {
        return ::fast_io::basic_string_view<value_type>{this->begin_pointer, this->size()};
    }

    [[nodiscard]]
    constexpr auto operator==(this BasicString const& self, BasicString const& other) noexcept -> bool {
        return string_view_type{self} == string_view_type{other};
    }

    [[nodiscard]]
    constexpr auto operator==(this BasicString const& self, string_view_type other) noexcept -> bool {
        return string_view_type{self} == other;
    }
};

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t size_with_null>
BasicString(CharType const (&)[size_with_null]) -> BasicString<CharType>;

template<::pltxt2htm::details::is_char_type CharType>
BasicString(CharType const*, CharType const*) -> BasicString<CharType>;

template<::pltxt2htm::details::is_char_type CharType>
BasicString(BasicStringView<CharType>) -> BasicString<CharType>;

template<::pltxt2htm::details::is_char_type CharType>
BasicString(::fast_io::basic_string_view<CharType>) -> BasicString<CharType>;

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
BasicString(::fast_io::containers::basic_string<CharType, Allocator> const&) -> BasicString<CharType>;

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t size>
BasicString(::pltxt2htm::details::BasicLiteralString<CharType, size> const&) -> BasicString<CharType>;

using String = BasicString<char>;
using WString = BasicString<wchar_t>;
using U8String = BasicString<char8_t>;
using U16String = BasicString<char16_t>;
using U32String = BasicString<char32_t>;

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto print_alias_define(::fast_io::io_alias_t, BasicString<CharType, Allocator> const& string_) noexcept
    -> ::fast_io::basic_io_scatter_t<CharType> {
    return {string_.data(), string_.size()};
}

} // namespace pltxt2htm::container

#include "../details/pop_macro.hh"
