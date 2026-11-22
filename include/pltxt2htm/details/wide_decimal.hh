/**
 * @file wide_decimal.hh
 * @brief Exact fixed-point helpers shared by decimal parsing and fixed-point formatting.
 * @details A 192-bit unsigned integer is wide enough for every intermediate used by the
 *          supported font-size domain: `significand * 10^38` plus a fraction needs up to 191
 *          bits, and the scaling used for digit generation stays below 192 bits as well. All
 *          operations are integer only, so they are constexpr-friendly and independent of the
 *          floating-point rounding mode.
 */

#pragma once

#include <bit>
#include <cstddef>
#include <cstdint>

namespace pltxt2htm::details::wide {

/**
 * @brief A 192-bit unsigned integer stored as three little-endian 64-bit limbs.
 */
struct UInt192 {
    ::std::uint64_t low{};
    ::std::uint64_t middle{};
    ::std::uint64_t high{};
};

/**
 * @brief The 128-bit product of two 64-bit unsigned integers.
 */
struct UInt128 {
    ::std::uint64_t low{};
    ::std::uint64_t high{};
};

/**
 * @brief Multiply two 64-bit unsigned integers into a 128-bit result.
 * @details Portable schoolbook multiplication on 32-bit halves; no compiler intrinsics.
 */
[[nodiscard]]
constexpr auto multiply_64x64(::std::uint64_t const left, ::std::uint64_t const right) noexcept -> UInt128 {
    auto const left_low = static_cast<::std::uint64_t>(static_cast<::std::uint32_t>(left));
    auto const left_high = static_cast<::std::uint64_t>(left >> 32);
    auto const right_low = static_cast<::std::uint64_t>(static_cast<::std::uint32_t>(right));
    auto const right_high = static_cast<::std::uint64_t>(right >> 32);

    auto const low_product = left_low * right_low;
    auto const cross_left = left_low * right_high;
    auto const cross_right = left_high * right_low;
    auto const high_product = left_high * right_high;

    auto const middle = cross_left + cross_right;
    ::std::uint64_t const middle_carry = middle < cross_left ? 1 : 0;
    auto const low = low_product + (middle << 32);
    ::std::uint64_t const low_carry = low < low_product ? 1 : 0;
    auto const high = high_product + (middle >> 32) + (middle_carry << 32) + low_carry;
    return UInt128{.low = low, .high = high};
}

/**
 * @brief Return the number of significant bits, or 0 for zero.
 */
[[nodiscard]]
constexpr auto bit_length(UInt192 const value) noexcept -> unsigned {
    if (value.high != 0) {
        return static_cast<unsigned>(128 + ::std::bit_width(value.high));
    }
    if (value.middle != 0) {
        return static_cast<unsigned>(64 + ::std::bit_width(value.middle));
    }
    return static_cast<unsigned>(::std::bit_width(value.low));
}

[[nodiscard]]
constexpr auto is_zero(UInt192 const value) noexcept -> bool {
    return value.low == 0 && value.middle == 0 && value.high == 0;
}

/**
 * @brief Shift left by the given number of bits; bits shifted out of 192 are discarded.
 */
[[nodiscard]]
constexpr auto shift_left(UInt192 const value, unsigned const count) noexcept -> UInt192 {
    if (count >= 192) {
        return UInt192{};
    }
    if (count == 0) {
        return value;
    }
    auto const limb_shift = count / 64;
    auto const bit_shift = count % 64;
    ::std::uint64_t const source[3]{value.low, value.middle, value.high};
    ::std::uint64_t result[3]{};
    for (unsigned i{}; i < 3; ++i) {
        auto const destination = i + limb_shift;
        if (destination >= 3) {
            break;
        }
        result[destination] |= source[i] << bit_shift;
        if (bit_shift != 0 && destination + 1 < 3) {
            result[destination + 1] |= source[i] >> (64 - bit_shift);
        }
    }
    return UInt192{.low = result[0], .middle = result[1], .high = result[2]};
}

/**
 * @brief Shift right by the given number of bits.
 */
[[nodiscard]]
constexpr auto shift_right(UInt192 const value, unsigned const count) noexcept -> UInt192 {
    if (count >= 192) {
        return UInt192{};
    }
    if (count == 0) {
        return value;
    }
    auto const limb_shift = count / 64;
    auto const bit_shift = count % 64;
    ::std::uint64_t const source[3]{value.low, value.middle, value.high};
    ::std::uint64_t result[3]{};
    for (unsigned i{}; i < 3; ++i) {
        if (i < limb_shift) {
            continue;
        }
        result[i - limb_shift] |= source[i] >> bit_shift;
        if (bit_shift != 0 && i > limb_shift) {
            result[i - limb_shift - 1] |= source[i] << (64 - bit_shift);
        }
    }
    return UInt192{.low = result[0], .middle = result[1], .high = result[2]};
}

/**
 * @brief Compare two 192-bit integers.
 * @return Negative when left < right, zero when equal, positive when left > right.
 */
[[nodiscard]]
constexpr auto compare(UInt192 const left, UInt192 const right) noexcept -> int {
    if (left.high != right.high) {
        return left.high < right.high ? -1 : 1;
    }
    if (left.middle != right.middle) {
        return left.middle < right.middle ? -1 : 1;
    }
    if (left.low != right.low) {
        return left.low < right.low ? -1 : 1;
    }
    return 0;
}

/**
 * @brief Subtract right from left; assumes left >= right.
 */
[[nodiscard]]
constexpr auto subtract(UInt192 const left, UInt192 const right) noexcept -> UInt192 {
    auto const low = left.low - right.low;
    ::std::uint64_t const borrow_low = left.low < right.low ? 1 : 0;
    auto const middle = left.middle - right.middle;
    ::std::uint64_t const borrow_middle = left.middle < right.middle ? 1 : 0;
    auto const middle_adjusted = middle - borrow_low;
    ::std::uint64_t const borrow_adjusted = middle < borrow_low ? 1 : 0;
    auto const high = left.high - right.high - borrow_middle - borrow_adjusted;
    return UInt192{.low = low, .middle = middle_adjusted, .high = high};
}

/**
 * @brief Add two 192-bit integers; overflow beyond 192 bits is discarded.
 */
[[nodiscard]]
constexpr auto add(UInt192 const left, UInt192 const right) noexcept -> UInt192 {
    auto const low = left.low + right.low;
    ::std::uint64_t const carry_low = low < left.low ? 1 : 0;
    auto const middle = left.middle + right.middle;
    ::std::uint64_t const carry_middle = middle < left.middle ? 1 : 0;
    auto const middle_adjusted = middle + carry_low;
    ::std::uint64_t const carry_adjusted = middle_adjusted < carry_low ? 1 : 0;
    auto const high = left.high + right.high + carry_middle + carry_adjusted;
    return UInt192{.low = low, .middle = middle_adjusted, .high = high};
}

/**
 * @brief Multiply a 192-bit integer by a 64-bit factor, keeping the low 192 bits.
 */
[[nodiscard]]
constexpr auto multiply(UInt192 const value, ::std::uint64_t const factor) noexcept -> UInt192 {
    auto const low_product = wide::multiply_64x64(value.low, factor);
    auto const middle_product = wide::multiply_64x64(value.middle, factor);
    auto const high_product = wide::multiply_64x64(value.high, factor);

    auto const middle = middle_product.low + low_product.high;
    ::std::uint64_t const middle_carry = middle < low_product.high ? 1 : 0;
    auto const high = high_product.low + middle_product.high + middle_carry;
    return UInt192{.low = low_product.low, .middle = middle, .high = high};
}

/**
 * @brief Compute 5^exponent as a 192-bit integer.
 */
[[nodiscard]]
constexpr auto pow5(::std::size_t exponent) noexcept -> UInt192 {
    UInt192 result{.low = 1, .middle = 0, .high = 0};
    for (::std::size_t i{}; i < exponent; ++i) {
        result = wide::multiply(result, 5);
    }
    return result;
}

/**
 * @brief Compute 10^exponent as a 192-bit integer.
 */
[[nodiscard]]
constexpr auto pow10(::std::size_t exponent) noexcept -> UInt192 {
    UInt192 result{.low = 1, .middle = 0, .high = 0};
    for (::std::size_t i{}; i < exponent; ++i) {
        result = wide::multiply(result, 10);
    }
    return result;
}

/**
 * @brief The quotient and remainder of a 192-bit division.
 */
struct DivisionResult {
    UInt192 quotient;
    UInt192 remainder;
};

/**
 * @brief Divide a 192-bit integer by a non-zero 192-bit divisor.
 */
[[nodiscard]]
constexpr auto divide(UInt192 const numerator, UInt192 const divisor) noexcept -> DivisionResult {
    auto const bits = wide::bit_length(numerator);
    UInt192 quotient{};
    UInt192 remainder{};
    for (unsigned index = bits; index-- > 0;) {
        remainder = wide::shift_left(remainder, 1);
        ::std::uint64_t const limb = index < 64 ? numerator.low : (index < 128 ? numerator.middle : numerator.high);
        remainder.low |= (limb >> (index % 64)) & 1;
        if (wide::compare(remainder, divisor) >= 0) {
            remainder = wide::subtract(remainder, divisor);
            if (index < 64) {
                quotient.low |= ::std::uint64_t{1} << index;
            }
            else if (index < 128) {
                quotient.middle |= ::std::uint64_t{1} << (index - 64);
            }
            else {
                quotient.high |= ::std::uint64_t{1} << (index - 128);
            }
        }
    }
    return DivisionResult{.quotient = quotient, .remainder = remainder};
}

/**
 * @brief Round a positive rational to the nearest double with ties to even.
 * @param numerator The significand.
 * @param scale_digits The number of decimal places: the value is numerator / 10^scale_digits.
 * @param extra_sticky Set when the true value is slightly above numerator / 10^scale_digits
 *        (for example when a fraction tail was dropped); it forces the sticky bit for ties.
 * @pre `scale_digits <= 38`; the parser and formatter guarantee this, and the shifted operands
 *      below stay below 192 bits under that bound.
 * @details The quotient is computed with 57 or 58 significant bits, so the round bit and the
 *          sticky bits are exact.
 */
[[nodiscard]]
constexpr auto decimal_ratio_to_double(UInt192 const numerator, ::std::size_t const scale_digits,
                                       bool const extra_sticky = false) noexcept -> double {
    if (wide::is_zero(numerator)) {
        return 0.0;
    }
    auto const divisor = wide::pow10(scale_digits);
    auto const numerator_bits = wide::bit_length(numerator);
    auto const divisor_bits = wide::bit_length(divisor);
    // Target a quotient of 57-58 bits: the 53-bit significand, one round bit, and slack. With this
    // offset the shifted numerator stays below 2^184 and the shifted divisor below 2^135, so both
    // fit the 192-bit type without truncation.
    auto const shift = static_cast<int>(divisor_bits) + 57 - static_cast<int>(numerator_bits);
    UInt192 const scaled_numerator = shift >= 0 ? wide::shift_left(numerator, static_cast<unsigned>(shift)) : numerator;
    UInt192 const scaled_divisor = shift >= 0 ? divisor : wide::shift_left(divisor, static_cast<unsigned>(-shift));
    auto const division = wide::divide(scaled_numerator, scaled_divisor);

    auto const quotient = division.quotient;
    auto const quotient_bits = wide::bit_length(quotient);
    // Keep 53 bits and one round bit; drop_bits is 4 or 5 for the targeted quotient size, so the
    // round/sticky bits below the mantissa are exact.
    auto const drop_bits = quotient_bits - 53;
    auto mantissa = quotient.low >> drop_bits;
    bool const round_bit = ((quotient.low >> (drop_bits - 1)) & 1) != 0;
    ::std::uint64_t const low_mask = (::std::uint64_t{1} << (drop_bits - 1)) - 1;
    bool const sticky = (quotient.low & low_mask) != 0 || quotient.middle != 0 || quotient.high != 0 ||
                        wide::is_zero(division.remainder) == false || extra_sticky;
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

} // namespace pltxt2htm::details::wide