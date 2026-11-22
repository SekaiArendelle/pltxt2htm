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

#include "../../container/string.hh"
#include "../literal_string.hh"

namespace pltxt2htm::details {

/**
 * @brief Generate the lookup table of two-digit decimal pairs ("00" through "99").
 */
[[nodiscard]]
consteval auto make_decimal_digit_pairs() noexcept -> ::pltxt2htm::details::U8LiteralString<200> {
    auto pairs = ::pltxt2htm::details::U8LiteralString<200>{};
    for (::std::size_t i{}; i < 100; ++i) {
        pairs[i * 2] = static_cast<char8_t>(u8'0' + i / 10);
        pairs[i * 2 + 1] = static_cast<char8_t>(u8'0' + i % 10);
    }
    return pairs;
}

inline constexpr auto decimal_digit_pairs = ::pltxt2htm::details::make_decimal_digit_pairs();

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
    auto ptr = buffer_end;
    while (value >= 100) {
        auto const pair_index = static_cast<::std::size_t>(value % 100) * 2;
        value /= 100;
        // The ones digit sits at the higher address since we fill backward.
        *--ptr = ::pltxt2htm::details::decimal_digit_pairs[pair_index + 1];
        *--ptr = ::pltxt2htm::details::decimal_digit_pairs[pair_index];
    }
    if (value >= 10) {
        auto const pair_index = static_cast<::std::size_t>(value) * 2;
        // The ones digit sits at the higher address since we fill backward.
        *--ptr = ::pltxt2htm::details::decimal_digit_pairs[pair_index + 1];
        *--ptr = ::pltxt2htm::details::decimal_digit_pairs[pair_index];
    }
    else {
        *--ptr = static_cast<char8_t>(u8'0' + value);
    }
    return ptr;
}

/**
 * @brief Convert an unsigned integer to a UTF-8 decimal string.
 * @param[in] num The number to convert.
 * @return A UTF-8 string representation of the number.
 * @note Digits are written backward into a fixed stack buffer and the result is constructed
 *       with the exact final size, so the conversion performs a single allocation with no
 *       reversal pass and no per-digit capacity checks.
 */
template<::std::unsigned_integral T>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto to_decimal_string(T num) noexcept -> ::pltxt2htm::container::U8String {
    // digits10(T) + 1 characters cover every value, including the extra digit that digits10
    // does not account for.
    char8_t buffer[::std::numeric_limits<T>::digits10 + 2];
    char8_t const* const begin = ::pltxt2htm::details::write_decimal_digits_backward(num, buffer + sizeof(buffer));
    return ::pltxt2htm::container::U8String{begin, buffer + sizeof(buffer)};
}

/**
 * @brief Convert a signed integer to a UTF-8 decimal string.
 * @details Handles negative values with a leading U+002D '-' sign written directly into the
 *          buffer, avoiding a post-hoc insert. Unsigned negation also covers the minimum value.
 */
template<::std::signed_integral T>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto to_decimal_string(T num) noexcept -> ::pltxt2htm::container::U8String {
    using unsigned_type = ::std::make_unsigned_t<T>;

    auto magnitude = static_cast<unsigned_type>(num);
    bool const negative = num < 0;
    if (negative) {
        magnitude = static_cast<unsigned_type>(0) - magnitude;
    }

    char8_t buffer[::std::numeric_limits<unsigned_type>::digits10 + 3];
    char8_t* begin = ::pltxt2htm::details::write_decimal_digits_backward(magnitude, buffer + sizeof(buffer));
    if (negative) {
        --begin;
        *begin = u8'-';
    }
    return ::pltxt2htm::container::U8String{begin, buffer + sizeof(buffer)};
}

/**
 * @brief Write the decimal digits of an unsigned integer into [first, last).
 * @param[in] first First writable character.
 * @param[in] last One past the last writable character.
 * @param[in] value The value to write.
 * @return One past the last written character, or nullptr when the range is too small.
 */
template<::std::unsigned_integral T>
[[nodiscard]]
constexpr auto to_chars(char8_t* const first, char8_t* const last, T value) noexcept -> char8_t* {
    char8_t buffer[::std::numeric_limits<T>::digits10 + 2];
    char8_t const* const begin = ::pltxt2htm::details::write_decimal_digits_backward(value, buffer + sizeof(buffer));
    auto const size = static_cast<::std::size_t>(buffer + sizeof(buffer) - begin);
    if (static_cast<::std::size_t>(last - first) < size) {
        return nullptr;
    }
    for (::std::size_t i{}; i < size; ++i) {
        first[i] = begin[i];
    }
    return first + size;
}

/**
 * @brief Write the decimal digits of a signed integer into [first, last).
 * @param[in] first First writable character.
 * @param[in] last One past the last writable character.
 * @param[in] value The value to write.
 * @return One past the last written character, or nullptr when the range is too small.
 */
template<::std::signed_integral T>
[[nodiscard]]
constexpr auto to_chars(char8_t* const first, char8_t* const last, T value) noexcept -> char8_t* {
    using unsigned_type = ::std::make_unsigned_t<T>;

    auto magnitude = static_cast<unsigned_type>(value);
    bool const negative = value < 0;
    if (negative) {
        magnitude = static_cast<unsigned_type>(0) - magnitude;
    }

    char8_t buffer[::std::numeric_limits<unsigned_type>::digits10 + 3];
    char8_t* begin = ::pltxt2htm::details::write_decimal_digits_backward(magnitude, buffer + sizeof(buffer));
    if (negative) {
        --begin;
        *begin = u8'-';
    }
    auto const size = static_cast<::std::size_t>(buffer + sizeof(buffer) - begin);
    if (static_cast<::std::size_t>(last - first) < size) {
        return nullptr;
    }
    for (::std::size_t i{}; i < size; ++i) {
        first[i] = begin[i];
    }
    return first + size;
}

/**
 * @brief Append the decimal digits of an unsigned integer to a string.
 * @param[in,out] out Destination string.
 * @param[in] value The value to append.
 */
template<::std::unsigned_integral T>
constexpr void append_decimal(::pltxt2htm::container::U8String& out, T value) noexcept {
    char8_t buffer[::std::numeric_limits<T>::digits10 + 2];
    char8_t const* const begin = ::pltxt2htm::details::write_decimal_digits_backward(value, buffer + sizeof(buffer));
    out.append(begin, buffer + sizeof(buffer));
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

    char8_t buffer[::std::numeric_limits<unsigned_type>::digits10 + 3];
    char8_t* begin = ::pltxt2htm::details::write_decimal_digits_backward(magnitude, buffer + sizeof(buffer));
    if (negative) {
        --begin;
        *begin = u8'-';
    }
    out.append(begin, buffer + sizeof(buffer));
}

} // namespace pltxt2htm::details