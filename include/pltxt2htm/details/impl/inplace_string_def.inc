/**
 * @file inplace_string_def.inc
 * @brief Out-of-line contract-checked definitions for details::BasicInplaceString.
 */

#pragma once

#include "inplace_string_decl.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t extent>
    requires (extent != 0)
constexpr auto BasicInplaceString<CharType, extent>::operator=(this BasicInplaceString& self,
                                                               BasicInplaceString const& other) noexcept
    -> BasicInplaceString& {
#if defined(PLTXT2HTM_INTERNAL_USE)
    pltxt2htm_assert(::std::addressof(self) != ::std::addressof(other), u8"Internal self-assignment is not allowed");
#else
    if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
        return self;
    }
#endif
    auto const count = other.size();
    ::std::copy_n(other.storage, count, self.storage);
    self.size_storage = other.size_storage;
    return self;
}

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t extent>
    requires (extent != 0)
template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
    requires (::std::same_as<::std::iter_value_t<InputIterator>, CharType> &&
              ::std::constructible_from<CharType, ::std::iter_reference_t<InputIterator>>)
constexpr void BasicInplaceString<CharType, extent>::assign(
    this BasicInplaceString& self, InputIterator first,
    Sentinel last) noexcept(::std::is_nothrow_move_constructible_v<InputIterator> &&
                            ::std::is_nothrow_move_constructible_v<Sentinel> &&
                            is_nothrow_range_iteration<InputIterator, Sentinel>()) {
    if constexpr (::std::sized_sentinel_for<Sentinel, InputIterator>) {
        auto const count = static_cast<size_type>(last - first);
        pltxt2htm_assert(count <= extent, u8"BasicInplaceString capacity exceeded");
    }
    self.clear();
    self.append(::std::move(first), ::std::move(last));
}

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t extent>
    requires (extent != 0)
template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
    requires (::std::same_as<::std::iter_value_t<InputIterator>, CharType> &&
              ::std::constructible_from<CharType, ::std::iter_reference_t<InputIterator>>)
constexpr void BasicInplaceString<CharType, extent>::append(
    this BasicInplaceString& self, InputIterator first,
    Sentinel last) noexcept(is_nothrow_range_iteration<InputIterator, Sentinel>()) {
    if constexpr (::std::sized_sentinel_for<Sentinel, InputIterator>) {
        auto const count = static_cast<size_type>(last - first);
        pltxt2htm_assert(count <= extent - self.size(), u8"BasicInplaceString capacity exceeded");
        if constexpr (::std::is_pointer_v<InputIterator> && ::std::same_as<InputIterator, Sentinel>) {
            auto const new_size = self.size() + count;
            auto const output = self.end();
            if (first != output) {
                ::std::copy(first, last, output);
            }
            self.size_storage = static_cast<StoredSize>(new_size);
            return;
        }
        else {
            while (first != last) {
                self.push_back(static_cast<value_type>(*first));
                ++first;
            }
        }
    }
    else {
        while (first != last) {
            self.push_back(static_cast<value_type>(*first));
            ++first;
        }
    }
}

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t extent>
    requires (extent != 0)
constexpr auto BasicInplaceString<CharType, extent>::operator[](this BasicInplaceString& self,
                                                                size_type position) noexcept -> reference {
    pltxt2htm_assert(position < self.size(), u8"Index of BasicInplaceString out of bound");
    return self.storage[position];
}

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t extent>
    requires (extent != 0)
constexpr auto BasicInplaceString<CharType, extent>::operator[](this BasicInplaceString const& self,
                                                                size_type position) noexcept -> const_reference {
    pltxt2htm_assert(position < self.size(), u8"Index of BasicInplaceString out of bound");
    return self.storage[position];
}

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t extent>
    requires (extent != 0)
constexpr auto BasicInplaceString<CharType, extent>::front(this BasicInplaceString& self) noexcept -> reference {
    pltxt2htm_assert(!self.empty(), u8"front() called on an empty BasicInplaceString");
    return self.storage[0];
}

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t extent>
    requires (extent != 0)
constexpr auto BasicInplaceString<CharType, extent>::front(this BasicInplaceString const& self) noexcept
    -> const_reference {
    pltxt2htm_assert(!self.empty(), u8"front() called on an empty BasicInplaceString");
    return self.storage[0];
}

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t extent>
    requires (extent != 0)
constexpr auto BasicInplaceString<CharType, extent>::back(this BasicInplaceString& self) noexcept -> reference {
    pltxt2htm_assert(!self.empty(), u8"back() called on an empty BasicInplaceString");
    return self.storage[self.size() - 1];
}

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t extent>
    requires (extent != 0)
constexpr auto BasicInplaceString<CharType, extent>::back(this BasicInplaceString const& self) noexcept
    -> const_reference {
    pltxt2htm_assert(!self.empty(), u8"back() called on an empty BasicInplaceString");
    return self.storage[self.size() - 1];
}

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t extent>
    requires (extent != 0)
constexpr void BasicInplaceString<CharType, extent>::push_back(this BasicInplaceString& self,
                                                               value_type value) noexcept {
    pltxt2htm_assert(self.size() < extent, u8"BasicInplaceString capacity exceeded");
    self.storage[self.size_storage] = value;
    ++self.size_storage;
}

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t extent>
    requires (extent != 0)
constexpr void BasicInplaceString<CharType, extent>::pop_back(this BasicInplaceString& self) noexcept {
    pltxt2htm_assert(!self.empty(), u8"pop_back() called on an empty BasicInplaceString");
    --self.size_storage;
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
