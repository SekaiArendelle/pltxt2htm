/**
 * @file inplace_string.hh
 * @brief Fixed-capacity character aliases for container::InplaceVector.
 */

#pragma once

#include <cstddef>
#include <type_traits>

#include "../container/inplace_vector.hh"
#include "concepts.hh"

namespace pltxt2htm::details {

/**
 * @brief A character vector with fixed compile-time capacity and inline storage.
 * @tparam CharType Character code-unit type.
 * @tparam extent Nonzero maximum number of code units. No terminator is reserved.
 *
 * Embedded null code units are ordinary elements. All operations have
 * container::InplaceVector semantics: {character} constructs one element,
 * (count) constructs count value-initialized elements, push_back returns a
 * reference, and try_push_back returns Optional<CharType&>. Internal callers
 * use is_empty(). Sources passed to assign must not refer to elements it destroys.
 */
template<::pltxt2htm::details::is_char_type CharType, ::std::size_t extent>
    requires (extent != 0)
using BasicInplaceString = ::pltxt2htm::container::InplaceVector<CharType, extent>;

namespace details {

template<typename T>
inline constexpr bool is_basic_inplace_string_impl = false;

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t extent>
    requires (extent != 0)
inline constexpr bool is_basic_inplace_string_impl<::pltxt2htm::container::InplaceVector<CharType, extent>> = true;

} // namespace details

// An alias has the same type identity as its character vector.
template<typename T>
concept is_basic_inplace_string = ::pltxt2htm::details::details::is_basic_inplace_string_impl<::std::remove_cvref_t<T>>;

template<::std::size_t extent>
using InplaceString = BasicInplaceString<char, extent>;

template<::std::size_t extent>
using WInplaceString = BasicInplaceString<wchar_t, extent>;

template<::std::size_t extent>
using U8InplaceString = BasicInplaceString<char8_t, extent>;

template<::std::size_t extent>
using U16InplaceString = BasicInplaceString<char16_t, extent>;

template<::std::size_t extent>
using U32InplaceString = BasicInplaceString<char32_t, extent>;

} // namespace pltxt2htm::details
