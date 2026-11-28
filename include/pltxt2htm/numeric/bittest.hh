/**
 * @file bittest.hh
 * @brief Tests a bit in a built-in or extended-width integer.
 */

#pragma once

#include <cstddef>
#include <limits>
#include <type_traits>

#include "uint192.hh"
#include "../details/push_macro.hh"

namespace pltxt2htm::numeric {

namespace details {

// ADL customization point: UInt128 and UInt192 provide hidden-friend overloads.
// Keep calls unqualified so ADL can find them; this deleted overload is the poison pill.
void bittest_impl() = delete;

// Detection intentionally uses the same unqualified ADL call as the dispatcher below.
template<typename T>
concept has_bittest_impl = requires(T const& value, ::std::size_t index) { bittest_impl(value, index); };

template<typename T>
[[nodiscard]]
constexpr auto call_bittest_impl(T const& value, ::std::size_t const index) noexcept -> bool
    requires has_bittest_impl<T>
{
    // Keep this unqualified: it is the deliberate ADL hook for the hidden friends.
    return bittest_impl(value, index);
}

template<typename T>
struct unsigned_equivalent {
    using type = ::std::make_unsigned_t<T>;
};

#if defined(__SIZEOF_INT128__)
template<>
struct unsigned_equivalent<NativeUInt128> {
    using type = NativeUInt128;
};

template<>
struct unsigned_equivalent<NativeInt128> {
    using type = NativeUInt128;
};
#endif

template<typename T>
using unsigned_equivalent_t = typename unsigned_equivalent<T>::type;

template<typename T>
inline constexpr ::std::size_t unsigned_bit_digits{::std::numeric_limits<T>::digits};

#if defined(__SIZEOF_INT128__)
template<>
inline constexpr ::std::size_t unsigned_bit_digits<NativeUInt128>{128};
#endif

} // namespace details

/**
 * @brief Returns whether the zero-based bit at `index` is set in `value`.
 * @details An index outside the value's bit width violates an assertion contract. Signed
 *          built-in integers are tested using their corresponding unsigned bit pattern. UInt128
 *          and UInt192 use direct bit tests provided through argument-dependent lookup.
 */
template<typename T>
    requires (details::integer<T> || details::has_bittest_impl<T>)
[[nodiscard]]
constexpr auto bittest(T const value, ::std::size_t const index) noexcept -> bool {
    if constexpr (details::has_bittest_impl<T>) {
        return details::call_bittest_impl(value, index);
    }
    else if constexpr (::std::same_as<T, bool>) {
        pltxt2htm_assert(index == 0, u8"bittest index is out of range");
        return value;
    }
    else {
        using unsigned_type = details::unsigned_equivalent_t<T>;
        pltxt2htm_assert(index < details::unsigned_bit_digits<unsigned_type>, u8"bittest index is out of range");
        auto const unsigned_value = static_cast<unsigned_type>(value);
        return ((unsigned_value >> index) & unsigned_type{1}) != 0;
    }
}

} // namespace pltxt2htm::numeric

#include "../details/pop_macro.hh"
