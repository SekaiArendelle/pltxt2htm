/**
 * @file floating.hh
 * @brief Fixed-point decimal formatting for non-negative floating-point values.
 * @details Emits the shortest fixed-point decimal that the library's parser reads back to the
 *          same double. No exponent notation is ever produced, because the values are emitted
 *          into CSS font-size declarations. The supported domain is defined in
 *          font_size_domain.hh; values outside it are rejected by the strict APIs.
 *
 *          Candidate digits are computed exactly with 192-bit integer arithmetic, so a
 *          long-mantissa value is never rounded through a double intermediate, and a candidate
 *          is only returned after the parser confirms it round-trips exactly.
 */

#pragma once

#include <bit>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <type_traits>

#include "../../container/array.hh"
#include "../../container/optional.hh"
#include "../../container/string.hh"
#include "../../container/string_view.hh"
#include "../font_size_domain.hh"
#include "../parser/decimal.hh"
#include "../../numeric/uint192.hh"
#include "integer.hh"

namespace pltxt2htm::details {

namespace format_details {

/// Maximum number of characters a fixed-point font-size value can occupy.
inline constexpr ::std::size_t max_fixed_point_length{48};

/**
 * @brief Return 10^exponent as a double for the comparison loop below.
 * @details Only exponents in [-18, 20] occur for values inside the font-size domain.
 */
[[nodiscard]]
constexpr auto pow10_as_double(int const exponent) noexcept -> double {
    /// 10^0 through 10^20 as doubles; 10^20 is the first inexact entry but only compares.
    constexpr auto decimal_powers =
        ::pltxt2htm::container::Array{1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9, 1e10,
                                      1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20};
    static_assert(::std::is_same_v<decltype(decimal_powers)::value_type, double>);
    if (exponent >= 0) {
        return decimal_powers[exponent];
    }
    return 1.0 / decimal_powers[-exponent];
}

/**
 * @brief Return floor(log10(value)) for a positive finite value in the domain.
 */
[[nodiscard]]
constexpr auto decimal_exponent_of(double const value) noexcept -> int {
    auto const bits = ::std::bit_cast<::std::uint64_t>(value);
    auto const binary_exponent = static_cast<int>((bits >> 52) & 0x7ff) - 1023;
    // 30103 / 100000 approximates log10(2); the loops below correct any off-by-one.
    auto exponent = static_cast<int>(static_cast<long long>(binary_exponent) * 30103 / 100000);
    while (value < ::pltxt2htm::details::format_details::pow10_as_double(exponent)) {
        --exponent;
    }
    while (value >= ::pltxt2htm::details::format_details::pow10_as_double(exponent + 1)) {
        ++exponent;
    }
    return exponent;
}

/**
 * @brief Return the bit at index, or false when index is beyond 191.
 */
[[nodiscard]]
constexpr auto bit_at(::pltxt2htm::numeric::UInt192 const value, unsigned const index) noexcept -> bool {
    return (static_cast<::std::uint64_t>(value >> index) & 1u) != 0;
}

/**
 * @brief Shift a 192-bit integer right, rounding the discarded bits to nearest with ties to even.
 * @param[out] result The rounded 64-bit result.
 * @return false when the truncated result does not fit 64 bits.
 */
[[nodiscard]]
constexpr auto rounded_shift_right(::pltxt2htm::numeric::UInt192 const value, unsigned const count,
                                   ::std::uint64_t& result) noexcept -> bool {
    if (value.bit_length() > count + 63) {
        return false;
    }
    auto const shifted = value >> count;
    if (shifted.bit_length() > 64) {
        return false;
    }
    result = static_cast<::std::uint64_t>(shifted);
    if (count == 0) {
        return true;
    }
    bool const round_bit = ::pltxt2htm::details::format_details::bit_at(value, count - 1);
    bool sticky{};
    for (unsigned index{}; index + 1 < count; ++index) {
        if (::pltxt2htm::details::format_details::bit_at(value, index)) {
            sticky = true;
            break;
        }
    }
    if (round_bit && (sticky || (result & 1) != 0)) {
        if (result == ~::std::uint64_t{0}) {
            return false;
        }
        ++result;
    }
    return true;
}

/**
 * @brief Compute round_half_even(value * 10^decimal_shift) exactly.
 * @param value A positive finite double in the domain.
 * @param decimal_shift The signed power of ten applied to value.
 * @return The rounded digits, or 0 when the result does not fit 64 bits.
 */
[[nodiscard]]
constexpr auto scaled_digits(double const value, int const decimal_shift) noexcept -> ::std::uint64_t {
    auto const bits = ::std::bit_cast<::std::uint64_t>(value);
    auto const binary_exponent = static_cast<int>((bits >> 52) & 0x7ff) - 1023 - 52;
    auto const significand = (bits & ((::std::uint64_t{1} << 52) - 1)) | (::std::uint64_t{1} << 52);

    if (decimal_shift >= 0) {
        auto const product =
            ::pltxt2htm::numeric::pow(::pltxt2htm::numeric::UInt192{5}, static_cast<::std::size_t>(decimal_shift)) *
            significand;
        auto const shift = binary_exponent + decimal_shift;
        if (shift >= 0) {
            auto const shifted = product << static_cast<unsigned>(shift);
            if (shifted.bit_length() > 64) {
                return 0;
            }
            return static_cast<::std::uint64_t>(shifted);
        }
        ::std::uint64_t result{};
        if (::pltxt2htm::details::format_details::rounded_shift_right(product, static_cast<unsigned>(-shift), result) ==
            false) {
            return 0;
        }
        return result;
    }

    // value * 10^decimal_shift = significand * 2^(binary_exponent + decimal_shift) / 5^-decimal_shift.
    auto const divisor_base =
        ::pltxt2htm::numeric::pow(::pltxt2htm::numeric::UInt192{5}, static_cast<::std::size_t>(-decimal_shift));
    ::pltxt2htm::numeric::UInt192 numerator{significand};
    ::pltxt2htm::numeric::UInt192 divisor{};
    if (binary_exponent + decimal_shift >= 0) {
        numerator = numerator << static_cast<unsigned>(binary_exponent + decimal_shift);
        divisor = divisor_base;
    }
    else {
        divisor = divisor_base << static_cast<unsigned>(-(binary_exponent + decimal_shift));
    }
    auto const division = ::pltxt2htm::numeric::divide(numerator, divisor);
    if (division.quotient.bit_length() > 64) {
        return 0;
    }
    auto digits = static_cast<::std::uint64_t>(division.quotient);
    auto const doubled_remainder = division.remainder << 1;
    if (doubled_remainder > divisor || (doubled_remainder == divisor && (digits & 1) != 0)) {
        ++digits;
    }
    return digits;
}

/**
 * @brief Render digits * 10^decimal_exponent in fixed-point notation.
 * @return The number of characters written to buffer.
 */
[[nodiscard]]
constexpr auto render_fixed_point(::std::uint64_t const digits, int const decimal_exponent,
                                  char8_t* const buffer) noexcept -> ::std::size_t {
    char8_t digit_buffer[24];
    char8_t const* const begin =
        ::pltxt2htm::details::write_decimal_digits_backward(digits, digit_buffer + sizeof(digit_buffer));
    auto const digit_count = static_cast<::std::size_t>(digit_buffer + sizeof(digit_buffer) - begin);

    ::std::size_t length{};
    if (decimal_exponent >= 0) {
        for (::std::size_t i{}; i < digit_count; ++i) {
            buffer[length++] = begin[i];
        }
        for (int i{}; i < decimal_exponent; ++i) {
            buffer[length++] = u8'0';
        }
        return length;
    }

    auto const fractional_digits = static_cast<::std::size_t>(-decimal_exponent);
    if (fractional_digits < digit_count) {
        auto const integer_digits = digit_count - fractional_digits;
        for (::std::size_t i{}; i < integer_digits; ++i) {
            buffer[length++] = begin[i];
        }
        buffer[length++] = u8'.';
        for (::std::size_t i{integer_digits}; i < digit_count; ++i) {
            buffer[length++] = begin[i];
        }
        return length;
    }

    buffer[length++] = u8'0';
    buffer[length++] = u8'.';
    for (::std::size_t i{}; i < fractional_digits - digit_count; ++i) {
        buffer[length++] = u8'0';
    }
    for (::std::size_t i{}; i < digit_count; ++i) {
        buffer[length++] = begin[i];
    }
    return length;
}

/**
 * @brief Render the shortest fixed-point representation that parses back to value.
 * @param value A value inside the font-size domain.
 * @return The number of characters written to buffer.
 * @details Tries significant-digit counts from 1 to 17, using exactly rounded candidates. If no
 *          candidate round-trips (only possible for values outside the domain, which callers
 *          exclude), it falls back to the truncated integer, which is a fixed point of the
 *          format/parse composition and therefore never drifts between passes.
 */
[[nodiscard]]
constexpr auto format_fixed_point(double const value, char8_t* const buffer) noexcept -> ::std::size_t {
    if (value == 0) {
        buffer[0] = u8'0';
        return 1;
    }
    // Integral values print their exact integer digits: the value itself is the canonical
    // shortest representation, it always round-trips, and it avoids trailing-zero forms for
    // large sizes.
    if (value >= 1.0) {
        auto const truncated = static_cast<::std::uint64_t>(value);
        if (static_cast<double>(truncated) == value) {
            return ::pltxt2htm::details::format_details::render_fixed_point(truncated, 0, buffer);
        }
    }
    auto const exponent = ::pltxt2htm::details::format_details::decimal_exponent_of(value);
    for (unsigned significant_digits{1}; significant_digits <= 17; ++significant_digits) {
        auto const decimal_shift = static_cast<int>(significant_digits) - 1 - exponent;
        auto const digits = ::pltxt2htm::details::format_details::scaled_digits(value, decimal_shift);
        if (digits == 0) {
            continue;
        }
        auto const length = ::pltxt2htm::details::format_details::render_fixed_point(digits, -decimal_shift, buffer);
        auto const parsed =
            ::pltxt2htm::details::try_parse_double_decimal_value(::pltxt2htm::container::U8StringView{buffer, length});
        if (parsed.has_value() && parsed.value().value == value) {
            return length;
        }
    }
    return ::pltxt2htm::details::format_details::render_fixed_point(static_cast<::std::uint64_t>(value), 0, buffer);
}

/// True when value can be parsed from and formatted into a font-size tag.
[[nodiscard]]
constexpr auto is_in_font_size_domain(double const value) noexcept -> bool {
    return value == 0 ||
           (value >= ::pltxt2htm::details::min_font_size_value && value < ::pltxt2htm::details::max_font_size_value);
}

} // namespace format_details

/**
 * @brief Write the fixed-point decimal representation of a value into [first, last).
 * @param[in] first First writable character.
 * @param[in] last One past the last writable character; only used as a bound.
 * @param[in] value The value to write.
 * @return One past the last written character, or nullptr when the range is too small or the
 *         value is outside the font-size domain (mirroring `std::to_chars`'s
 *         `errc::value_too_large` for the buffer case).
 */
[[nodiscard]]
constexpr auto to_chars(char8_t* const first, char8_t const* const last, double const value) noexcept -> char8_t* {
    if (::pltxt2htm::details::format_details::is_in_font_size_domain(value) == false) {
        return nullptr;
    }
    char8_t buffer[::pltxt2htm::details::format_details::max_fixed_point_length];
    auto const length = ::pltxt2htm::details::format_details::format_fixed_point(value, buffer);
    if (::std::cmp_less(last - first, length)) {
        return nullptr;
    }
    for (::std::size_t i{}; i < length; ++i) {
        first[i] = buffer[i];
    }
    return first + length;
}

/**
 * @brief Convert a value to a fixed-point decimal string.
 * @return The string, or nullopt when the value is outside the font-size domain.
 */
[[nodiscard]]
constexpr auto to_decimal_string(double const value) noexcept
    -> ::pltxt2htm::container::Optional<::pltxt2htm::container::U8String> {
    if (::pltxt2htm::details::format_details::is_in_font_size_domain(value) == false) {
        return ::pltxt2htm::container::nullopt;
    }
    char8_t buffer[::pltxt2htm::details::format_details::max_fixed_point_length];
    auto const length = ::pltxt2htm::details::format_details::format_fixed_point(value, buffer);
    return ::pltxt2htm::container::U8String{buffer, buffer + length};
}

/**
 * @brief Append the fixed-point decimal representation of a value to a string.
 * @details Out-of-domain values are clamped: non-positive values, NaN, and values below the
 *          domain minimum append "0", and values at or above 2^64 append the size-tag maximum.
 *          Call sites are expected to pass parser-produced values (or explicitly saturated
 *          values).
 */
constexpr void append_decimal(::pltxt2htm::container::U8String& out, double value) noexcept {
    if (value >= ::pltxt2htm::details::min_font_size_value) {
        if (value >= ::pltxt2htm::details::max_font_size_value) {
            value = ::pltxt2htm::details::max_size_tag_value;
        }
    }
    else {
        // Non-positive values, NaN, and values below the domain minimum all become an inert zero
        // size; no parser-produced value reaches this branch.
        value = 0;
    }
    char8_t buffer[::pltxt2htm::details::format_details::max_fixed_point_length];
    auto const length = ::pltxt2htm::details::format_details::format_fixed_point(value, buffer);
    out.append(buffer, buffer + length);
}

/**
 * @brief Round a non-negative domain value up to the nearest integer.
 * @pre value is inside the font-size domain, so the conversion to `::std::uint64_t` is defined.
 */
[[nodiscard]]
constexpr auto double_to_uint64_ceil(double const value) noexcept -> ::std::uint64_t {
    auto const truncated = static_cast<::std::uint64_t>(value);
    return static_cast<double>(truncated) < value ? truncated + 1 : truncated;
}

} // namespace pltxt2htm::details