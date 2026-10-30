/**
 * @file optional_def.inc
 * @brief Out-of-line contract-checked definitions for container::Optional.
 */

#pragma once

#include "optional_decl.hh"
#include "../../details/push_macro.hh"

namespace pltxt2htm::container {

template<typename T>
constexpr auto Optional<T>::value(this auto&& self) noexcept -> decltype(auto)
    requires (!::std::is_reference_v<T>)
{
    pltxt2htm_assert(self.storage.has_value(), u8"optional does not contain a value");
    return ::std::forward_like<decltype(self)>(self.storage).value();
}

template<typename T>
constexpr auto Optional<T>::value(this Optional<T> const& self) noexcept -> T
    requires (::std::is_lvalue_reference_v<T>)
{
    pltxt2htm_assert(self.storage.has_value(), u8"optional does not contain a value");
    return self.storage.value();
}

} // namespace pltxt2htm::container

#include "../../details/pop_macro.hh"
