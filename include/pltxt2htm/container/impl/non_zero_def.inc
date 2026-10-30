/**
 * @file non_zero_def.inc
 * @brief Out-of-line contract-checked definitions for container::NonZero.
 */

#pragma once

#include "non_zero_decl.hh"
#include "../../details/push_macro.hh"

namespace pltxt2htm::container {

template<::pltxt2htm::container::details::is_non_zero_unsigned_integer T>
constexpr auto NonZero<T>::from(T value) noexcept -> NonZero {
    pltxt2htm_assert(value != 0, u8"NonZero value must not be zero");
    return NonZero{value};
}

template<::pltxt2htm::container::details::is_non_zero_unsigned_integer T>
constexpr auto NonZero<T>::get(this NonZero const& self) noexcept -> T {
    auto const value = self.value_storage;
    pltxt2htm_assert(value != 0, u8"NonZero value must not be zero");
    return value;
}

} // namespace pltxt2htm::container

#include "../../details/pop_macro.hh"
