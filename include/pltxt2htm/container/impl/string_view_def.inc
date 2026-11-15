/**
 * @file string_view_def.inc
 * @brief Out-of-line contract-checked definitions for container::BasicStringView.
 */

#pragma once

#include "../../details/push_macro.hh"

namespace pltxt2htm::container {

template<::pltxt2htm::details::is_char_type CharType>
constexpr auto BasicStringView<CharType>::operator[](this BasicStringView const& self, size_type position) noexcept
    -> const_reference {
    pltxt2htm_assert(position < self.length, u8"Index of BasicStringView out of bound");
    return self.pointer[position];
}

template<::pltxt2htm::details::is_char_type CharType>
constexpr auto BasicStringView<CharType>::subview(this BasicStringView const& self, size_type position,
                                                  size_type count) noexcept -> BasicStringView {
    pltxt2htm_assert(position <= self.length, u8"Subview position of BasicStringView out of bound");

    size_type const remaining_size{self.length - position};
    if (count == npos) {
        count = remaining_size;
    }
    else {
        pltxt2htm_assert(count <= remaining_size, u8"Subview size of BasicStringView out of bound");
    }

    const_pointer const subview_data{position == 0 ? self.pointer : self.pointer + position};
    return BasicStringView{subview_data, count};
}

template<::pltxt2htm::details::is_char_type CharType>
constexpr auto BasicStringView<CharType>::front(this BasicStringView const& self) noexcept -> const_reference {
    pltxt2htm_assert(self.length != 0, u8"Front of empty BasicStringView");
    return self.pointer[0];
}

template<::pltxt2htm::details::is_char_type CharType>
constexpr auto BasicStringView<CharType>::back(this BasicStringView const& self) noexcept -> const_reference {
    pltxt2htm_assert(self.length != 0, u8"Back of empty BasicStringView");
    return self.pointer[self.length - 1];
}

template<::pltxt2htm::details::is_char_type CharType>
constexpr auto BasicStringView<CharType>::remove_prefix(this BasicStringView& self, size_type count) noexcept -> void {
    pltxt2htm_assert(count <= self.length, u8"Remove_prefix count of BasicStringView out of bound");
    self.pointer += count;
    self.length -= count;
}

template<::pltxt2htm::details::is_char_type CharType>
constexpr auto BasicStringView<CharType>::remove_suffix(this BasicStringView& self, size_type count) noexcept -> void {
    pltxt2htm_assert(count <= self.length, u8"Remove_suffix count of BasicStringView out of bound");
    self.length -= count;
}

} // namespace pltxt2htm::container

#include "../../details/pop_macro.hh"
