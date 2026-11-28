/**
 * @file integer.hh
 * @brief Decimal formatting for integral types.
 * @details Writes the decimal digits of an integer into a caller-provided buffer or a
 *          container::U8String. The conversion is allocation-free at the algorithm level and
 *          constexpr-friendly; the owning helpers allocate exactly once.
 */

#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

#include "../container/array.hh"
#include "../container/string.hh"

namespace pltxt2htm::format {

namespace details {

/**
 * @brief Generate the lookup table of two-digit decimal pairs ("00" through "99").
 */
[[nodiscard]]
consteval auto make_decimal_digit_pairs() noexcept -> ::pltxt2htm::container::Array<char8_t, 200> {
    auto pairs = ::pltxt2htm::container::Array<char8_t, 200>{};
    for (::std::size_t i{}; i < 100; ++i) {
        pairs[i * 2] = static_cast<char8_t>(u8'0' + i / 10);
        pairs[i * 2 + 1] = static_cast<char8_t>(u8'0' + i % 10);
    }
    return pairs;
}

/**
 * @brief Write the decimal digits of an unsigned integer backward into a buffer.
 * @details Digits are emitted into [returned pointer, buffer_end) so callers need
 *          no digit-count pre-pass and no reversal pass. Two digits are written per
 *          iteration through a 100-entry lookup table, halving the number of divisions.
 * @tparam T The unsigned integer type of value.
 * @param[in] value The value to write; any unsigned value is accepted, including 0.
 * @param[in] buffer_end One past the last writable element; the buffer must have room
 *            for digits10(T) + 1 characters.
 * @return Pointer to the first written digit.
 */
template<::std::unsigned_integral T>
[[nodiscard]]
constexpr auto write_decimal_digits_backward(T value, char8_t* const buffer_end) noexcept -> char8_t* {
    constexpr auto decimal_digit_pairs = ::pltxt2htm::format::details::make_decimal_digit_pairs();

    auto ptr = buffer_end;
    while (value >= 100) {
        auto const pair_index = static_cast<::std::size_t>(value % 100) * 2;
        value /= 100;
        // The ones digit sits at the higher address since we fill backward.
        *--ptr = decimal_digit_pairs[pair_index + 1];
        *--ptr = decimal_digit_pairs[pair_index];
    }
    if (value >= 10) {
        auto const pair_index = static_cast<::std::size_t>(value) * 2;
        // The ones digit sits at the higher address since we fill backward.
        *--ptr = decimal_digit_pairs[pair_index + 1];
        *--ptr = decimal_digit_pairs[pair_index];
    }
    else {
        *--ptr = static_cast<char8_t>(u8'0' + value);
    }
    return ptr;
}

} // namespace details

/**
 * @brief Append the decimal digits of an unsigned integer to a string.
 * @param[in,out] out Destination string.
 * @param[in] value The value to append.
 */
template<::std::unsigned_integral T>
constexpr void append_decimal(::pltxt2htm::container::U8String& out, T value) noexcept {
    auto buffer = ::pltxt2htm::container::Array<char8_t, ::std::numeric_limits<T>::digits10 + 2>{};
    char8_t const* const begin = details::write_decimal_digits_backward(value, buffer.data() + buffer.size());
    out.append(begin, buffer.data() + buffer.size());
}

/**
 * @brief Append the decimal digits of a signed integer to a string.
 * @param[in,out] out Destination string.
 * @param[in] value The value to append.
 */
template<::std::signed_integral T>
constexpr void append_decimal(::pltxt2htm::container::U8String& out, T value) noexcept {
    using unsigned_type = ::std::make_unsigned_t<T>;

    auto magnitude = static_cast<unsigned_type>(value);
    bool const negative = value < 0;
    if (negative) {
        magnitude = static_cast<unsigned_type>(0) - magnitude;
    }

    auto buffer = ::pltxt2htm::container::Array<char8_t, ::std::numeric_limits<unsigned_type>::digits10 + 3>{};
    char8_t* begin = details::write_decimal_digits_backward(magnitude, buffer.data() + buffer.size());
    if (negative) {
        --begin;
        *begin = u8'-';
    }
    out.append(begin, buffer.data() + buffer.size());
}

} // namespace pltxt2htm::format