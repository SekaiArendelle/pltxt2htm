/**
 * @file array_def.inc
 * @brief Out-of-line contract-checked definitions for container::Array.
 */

#pragma once

#include "array_decl.hh"
#include "../../details/push_macro.hh"

namespace pltxt2htm::container {

template<typename T, ::std::size_t extent>
    requires (extent != 0)
constexpr auto Array<T, extent>::operator[](this Array& self, size_type position) noexcept -> reference {
    pltxt2htm_assert(position < extent, u8"Index of array out of bound");
    return self.element[position];
}

template<typename T, ::std::size_t extent>
    requires (extent != 0)
constexpr auto Array<T, extent>::operator[](this Array const& self, size_type position) noexcept -> const_reference {
    pltxt2htm_assert(position < extent, u8"Index of array out of bound");
    return self.element[position];
}

template<typename T, ::std::size_t extent>
    requires (extent != 0)
constexpr auto Array<T, extent>::front(this Array& self) noexcept -> reference {
    pltxt2htm_assert(extent != 0, u8"front() called on an empty array");
    return self.element[0];
}

template<typename T, ::std::size_t extent>
    requires (extent != 0)
constexpr auto Array<T, extent>::front(this Array const& self) noexcept -> const_reference {
    pltxt2htm_assert(extent != 0, u8"front() called on an empty array");
    return self.element[0];
}

template<typename T, ::std::size_t extent>
    requires (extent != 0)
constexpr auto Array<T, extent>::back(this Array& self) noexcept -> reference {
    pltxt2htm_assert(extent != 0, u8"back() called on an empty array");
    return self.element[extent - 1];
}

template<typename T, ::std::size_t extent>
    requires (extent != 0)
constexpr auto Array<T, extent>::back(this Array const& self) noexcept -> const_reference {
    pltxt2htm_assert(extent != 0, u8"back() called on an empty array");
    return self.element[extent - 1];
}

} // namespace pltxt2htm::container

#include "../../details/pop_macro.hh"
