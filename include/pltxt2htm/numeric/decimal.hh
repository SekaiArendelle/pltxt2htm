/**
 * @file decimal.hh
 * @brief Exact rounding of decimal rationals to the nearest double.
 */

#pragma once

#include <bit>
#include <cstddef>
#include <cstdint>

#include "uint192.hh"

namespace pltxt2htm::numeric {

/**
 * @brief Round a positive rational to the nearest double with ties to even.
 * @param numerator The significand.
 * @param scale_digits The number of decimal places: the value is numerator / 10^scale_digits.
 * @param extra_sticky Set when the true value is slightly above numerator / 10^scale_digits
 *        (for example when a fraction tail was dropped); it forces the sticky bit for ties.
 * @pre `scale_digits <= 38`; the shifted operands below stay below 192 bits under that bound.
 * @details The quotient is computed with 57 or 58 significant bits, so the round bit and the
 *          sticky bits are exact. The result is assumed to lie in the normal double range.
 */
[[nodiscard]]
constexpr auto decimal_ratio_to_double(::pltxt2htm::numeric::UInt192 const numerator, ::std::size_t const scale_digits,
                                       bool const extra_sticky = false) noexcept -> double {
    if (numerator == ::pltxt2htm::numeric::UInt192{}) {
        return 0.0;
    }
    auto const divisor = ::pltxt2htm::numeric::pow(::pltxt2htm::numeric::UInt192{10}, scale_digits);
    auto const numerator_bits = numerator.bit_length();
    auto const divisor_bits = divisor.bit_length();
    // Target a quotient of 57-58 bits: the 53-bit significand, one round bit, and slack. With this
    // offset the shifted numerator stays below 2^184 and the shifted divisor below 2^135, so both
    // fit the 192-bit type without truncation.
    auto const shift = static_cast<int>(divisor_bits) + 57 - static_cast<int>(numerator_bits);
    ::pltxt2htm::numeric::UInt192 const scaled_numerator{shift >= 0 ? numerator << static_cast<unsigned>(shift)
                                                                    : numerator};
    ::pltxt2htm::numeric::UInt192 const scaled_divisor{shift >= 0 ? divisor : divisor << static_cast<unsigned>(-shift)};
    auto const division = ::pltxt2htm::numeric::divide(scaled_numerator, scaled_divisor);

    auto const quotient = division.quotient;
    auto const quotient_bits = quotient.bit_length();
    // Keep 53 bits and one round bit; drop_bits is 4 or 5 for the targeted quotient size, so the
    // round/sticky bits below the mantissa are exact.
    auto const drop_bits = quotient_bits - 53;
    auto const quotient_low = static_cast<::std::uint64_t>(quotient);
    auto mantissa = quotient_low >> drop_bits;
    bool const round_bit{((quotient_low >> (drop_bits - 1)) & 1) != 0};
    ::std::uint64_t const low_mask{(::std::uint64_t{1} << (drop_bits - 1)) - 1};
    bool const sticky{(quotient_low & low_mask) != 0 || division.remainder != ::pltxt2htm::numeric::UInt192{} ||
                      extra_sticky};
    if (round_bit && (sticky || (mantissa & 1) != 0)) {
        ++mantissa;
    }

    // The 53-bit significand mantissa represents value = mantissa * 2^(drop_bits - shift); an
    // IEEE double stores mantissa with an implicit 2^52 bit, so its unbiased exponent is that
    // power of two plus 52.
    auto binary_exponent = static_cast<int>(drop_bits) - shift + 52;
    if (mantissa == (::std::uint64_t{1} << 53)) {
        mantissa >>= 1;
        ++binary_exponent;
    }
    // The supported domain keeps every result in the normal range, so the biased exponent is
    // always positive and below the infinity encoding.
    auto const biased_exponent = binary_exponent + 1023;
    auto const bits = (static_cast<::std::uint64_t>(biased_exponent) << 52) | (mantissa - (::std::uint64_t{1} << 52));
    return ::std::bit_cast<double>(bits);
}

} // namespace pltxt2htm::numeric
