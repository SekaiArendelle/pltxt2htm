/**
 * @file cstring_view_def.inc
 * @brief Out-of-line contract-checked definitions for container::BasicCStringView.
 */

#pragma once

#include "../../details/push_macro.hh"

namespace pltxt2htm::container {

template<::pltxt2htm::details::is_char_type CharType>
template<typename PointerType>
    requires (::std::same_as<PointerType, CharType const*> || ::std::same_as<PointerType, CharType*>)
constexpr BasicCStringView<CharType>::BasicCStringView(PointerType string) noexcept
    : pointer{string} {
    pltxt2htm_assert(string != nullptr, u8"Null pointer passed to BasicCStringView");
    length = ::fast_io::cstr_len(string);
}

template<::pltxt2htm::details::is_char_type CharType>
constexpr BasicCStringView<CharType>::BasicCStringView(const_pointer string, size_type length_) noexcept
    : pointer{string},
      length{length_} {
    pltxt2htm_assert(string != nullptr && string[length_] == value_type{},
                     u8"BasicCStringView does not refer to a null-terminated string");
}

template<::pltxt2htm::details::is_char_type CharType>
template<::std::size_t size_with_null>
constexpr BasicCStringView<CharType>::BasicCStringView(value_type const (&string)[size_with_null]) noexcept
    : pointer{string},
      length{size_with_null - 1} {
    static_assert(size_with_null != 0);
    pltxt2htm_assert(string[size_with_null - 1] == value_type{},
                     u8"BasicCStringView array does not end with a null terminator");
}

template<::pltxt2htm::details::is_char_type CharType>
template<typename InputIterator, typename EndSentinel>
    requires (::std::contiguous_iterator<InputIterator> && ::std::sized_sentinel_for<EndSentinel, InputIterator> &&
              ::std::same_as<::std::iter_value_t<InputIterator>, CharType> &&
              !::std::convertible_to<EndSentinel, ::std::size_t>)
constexpr BasicCStringView<CharType>::BasicCStringView(InputIterator begin, EndSentinel end) noexcept(
    noexcept(::std::to_address(::std::declval<InputIterator&>())) &&
    noexcept(::std::declval<EndSentinel&>() - ::std::declval<InputIterator&>()) &&
    noexcept(::std::declval<InputIterator&>()[::std::size_t{}]))
    : pointer{::std::to_address(begin)},
      length{static_cast<::std::size_t>(end - begin)} {
    pltxt2htm_assert(begin[length] == CharType{}, u8"BasicCStringView does not refer to a null-terminated string");
}

template<::pltxt2htm::details::is_char_type CharType>
template<::pltxt2htm::container::details::is_ntcts_range<CharType> RangeType>
    requires (!::std::same_as<::std::remove_cvref_t<RangeType>, ::pltxt2htm::container::BasicCStringView<CharType>> &&
              !::std::same_as<::std::remove_cvref_t<RangeType>, ::pltxt2htm::container::BasicStringView<CharType>> &&
              !::pltxt2htm::details::is_literal_string<RangeType> &&
              !::pltxt2htm::details::is_basic_inplace_string<RangeType> &&
              !::pltxt2htm::container::details::has_allocator_type<RangeType> &&
              !::pltxt2htm::container::details::is_std_array<RangeType> &&
              !::std::same_as<::std::remove_cvref_t<RangeType>, ::std::basic_string_view<CharType>> &&
              !::std::same_as<::std::remove_cvref_t<RangeType>, ::fast_io::containers::basic_string_view<CharType>> &&
              !::std::convertible_to<RangeType, CharType const*>)
constexpr BasicCStringView<CharType>::BasicCStringView(RangeType&& range) noexcept(
    noexcept(::std::ranges::data(::std::declval<RangeType&>())) &&
    noexcept(::std::ranges::size(::std::declval<RangeType&>())))
    : pointer{::std::ranges::data(range)},
      length{static_cast<::std::size_t>(::std::ranges::size(range))} {
    pltxt2htm_assert(::std::ranges::data(range)[length] == CharType{},
                     u8"BasicCStringView does not refer to a null-terminated string");
}

template<::pltxt2htm::details::is_char_type CharType>
constexpr auto BasicCStringView<CharType>::operator[](this BasicCStringView const& self, size_type position) noexcept
    -> const_reference {
    pltxt2htm_assert(position <= self.length, u8"Index of BasicCStringView out of bound");
    return self.pointer[position];
}

template<::pltxt2htm::details::is_char_type CharType>
constexpr auto BasicCStringView<CharType>::front(this BasicCStringView const& self) noexcept -> const_reference {
    pltxt2htm_assert(self.length != 0, u8"Front of an empty BasicCStringView");
    return self.pointer[0];
}

template<::pltxt2htm::details::is_char_type CharType>
constexpr auto BasicCStringView<CharType>::back(this BasicCStringView const& self) noexcept -> const_reference {
    pltxt2htm_assert(self.length != 0, u8"Back of an empty BasicCStringView");
    return self.pointer[self.length - 1];
}

template<::pltxt2htm::details::is_char_type CharType>
constexpr auto BasicCStringView<CharType>::remove_prefix(this BasicCStringView& self, size_type count) noexcept
    -> void {
    pltxt2htm_assert(count <= self.length, u8"Prefix removal of BasicCStringView out of bound");
    self.pointer += count;
    self.length -= count;
}

template<::pltxt2htm::details::is_char_type CharType>
constexpr auto BasicCStringView<CharType>::copy(this BasicCStringView const& self, value_type* destination,
                                                size_type count, size_type position) noexcept -> size_type {
    pltxt2htm_assert(position <= self.length, u8"Copy position of BasicCStringView out of bound");

    size_type const remaining_size{self.length - position};
    size_type const copied_count{count < remaining_size ? count : remaining_size};
    ::std::copy_n(self.pointer + position, copied_count, destination);
    return copied_count;
}

template<::pltxt2htm::details::is_char_type CharType>
constexpr auto BasicCStringView<CharType>::subview(this BasicCStringView const& self, size_type position) noexcept
    -> BasicCStringView {
    pltxt2htm_assert(position <= self.length, u8"Subview position of BasicCStringView out of bound");
    return BasicCStringView{self.pointer + position, self.length - position};
}

template<::pltxt2htm::details::is_char_type CharType>
constexpr auto BasicCStringView<CharType>::subview(this BasicCStringView const& self, size_type position,
                                                   size_type count) noexcept -> BasicStringView<CharType> {
    pltxt2htm_assert(position <= self.length, u8"Subview position of BasicCStringView out of bound");

    size_type const remaining_size{self.length - position};
    if (count == npos) {
        count = remaining_size;
    }
    else {
        pltxt2htm_assert(count <= remaining_size, u8"Subview size of BasicCStringView out of bound");
    }

    const_pointer const subview_data{position == 0 ? self.pointer : self.pointer + position};
    return BasicStringView<CharType>{subview_data, count};
}

} // namespace pltxt2htm::container

#include "../../details/pop_macro.hh"
