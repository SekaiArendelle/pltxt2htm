/**
 * @file string_view_def.inc
 * @brief Out-of-line contract-checked definitions for container::BasicStringView.
 */

#pragma once

#include "string_view_decl.hh"
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

} // namespace pltxt2htm::container

#include "../../details/pop_macro.hh"
