/**
 * @file uint192.hh
 * @brief 192-bit unsigned integer and the exact decimal helpers built on it.
 * @details A 192-bit unsigned integer is wide enough for every intermediate used by the
 *          supported font-size domain: `significand * 10^38` plus a fraction needs up to 191
 *          bits, and the scaling used for digit generation stays below 192 bits as well. All
 *          operations are integer only, so they are constexpr-friendly and independent of the
 *          floating-point rounding mode.
 *
 *          UInt192 models unsigned arithmetic modulo 2^192: the operators wrap on overflow and
 *          shifts discard the bits that leave the type. It is an internal building block for
 *          exact decimal conversion, not a general-purpose bignum.
 */

#pragma once

#include <bit>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>

#include "uint128.hh"

namespace pltxt2htm::numeric {

class UInt192;

namespace details {

/**
 * @brief Targets the explicit UInt192 conversion accepts.
 * @details All standard unsigned integer types, plus the native 128-bit type when it is
 *          available. Some standard libraries do not classify the native 128-bit type as an
 *          integral type, so it is listed explicitly.
 */
#if defined(__SIZEOF_INT128__)
template<typename T>
concept unsigned_conversion_target = ::std::unsigned_integral<T> || ::std::same_as<T, UInt128>;
#else
template<typename T>
concept unsigned_conversion_target = ::std::unsigned_integral<T>;
#endif

/**
 * @brief Integer operands the UInt192 arithmetic operators accept.
 * @details Standard integer types plus the two extended-width types. Signed operands are
 *          sign-extended to 192 bits, so arithmetic follows modulo 2^192.
 */
template<typename T>
concept integer_operand = ::std::integral<T> || ::std::same_as<T, UInt192> || ::std::same_as<T, UInt128>;

} // namespace details

/**
 * @brief A 192-bit unsigned integer with unsigned (modulo 2^192) arithmetic.
 */
class UInt192 {
    ::std::uint64_t low_limb{}; ///< Least significant 64-bit limb.
    ::std::uint64_t middle_limb{}; ///< Middle 64-bit limb.
    ::std::uint64_t high_limb{}; ///< Most significant 64-bit limb.

    /// Constructs a value from little-endian limbs. Callers inside the class and its hidden
    /// friends use this; external callers assemble multi-limb values with shifts and `operator|`.
    constexpr UInt192(::std::uint64_t const low, ::std::uint64_t const middle, ::std::uint64_t const high) noexcept
        : low_limb{low},
          middle_limb{middle},
          high_limb{high} {
    }

public:
    constexpr UInt192() noexcept = default;

    /// Constructs a value from an unsigned integer; the conversion is lossless and implicit.
    template<typename T>
        requires (::std::unsigned_integral<T> || ::std::same_as<T, UInt128>)
    constexpr UInt192(T const value) noexcept
        : low_limb{static_cast<::std::uint64_t>(value)} {
        if constexpr (sizeof(T) == 16) {
            middle_limb = static_cast<::std::uint64_t>(value >> 64);
        }
    }

    /// Constructs a value from a signed integer, sign-extending it to 192 bits.
    template<::std::signed_integral T>
    explicit constexpr UInt192(T const value) noexcept
        : low_limb{static_cast<::std::uint64_t>(value)} {
        if constexpr (sizeof(T) == 16) {
            middle_limb = static_cast<::std::uint64_t>(value >> 64);
            if (value < 0) {
                high_limb = 0xFFFFFFFFFFFFFFFFull;
            }
        }
        else if (value < 0) {
            middle_limb = 0xFFFFFFFFFFFFFFFFull;
            high_limb = 0xFFFFFFFFFFFFFFFFull;
        }
    }

    /// Explicit conversion to an unsigned integer-like target.
    /// @details `bool` reports whether the value is non-zero. Targets up to 64 bits keep the low
    /// bits and truncate; a 128-bit target keeps the low 128 bits. Conversions are explicit, so
    /// narrowing never happens implicitly.
    template<details::unsigned_conversion_target T>
    [[nodiscard]]
    constexpr explicit operator T(this UInt192 const& self) noexcept {
        if constexpr (::std::same_as<T, bool>) {
            return self != UInt192{};
        }
        else if constexpr (sizeof(T) <= 8) {
            return static_cast<T>(self.low_limb);
        }
        else {
            static_assert(sizeof(T) == 16, "UInt192 converts to at most 128 bits");
            return (static_cast<T>(self.middle_limb) << 64) | static_cast<T>(self.low_limb);
        }
    }

    /// Number of significant bits, or 0 for zero.
    [[nodiscard]]
    constexpr auto bit_length(this UInt192 const& self) noexcept -> unsigned {
        if (self.high_limb != 0) {
            return static_cast<unsigned>(128 + ::std::bit_width(self.high_limb));
        }
        if (self.middle_limb != 0) {
            return static_cast<unsigned>(64 + ::std::bit_width(self.middle_limb));
        }
        return static_cast<unsigned>(::std::bit_width(self.low_limb));
    }

    /// Adds two values modulo 2^192.
    [[nodiscard]]
    constexpr auto operator+(this UInt192 const& self, UInt192 const& right) noexcept -> UInt192 {
        auto const low = self.low_limb + right.low_limb;
        ::std::uint64_t const carry_low = low < self.low_limb ? 1 : 0;
        auto const middle = self.middle_limb + right.middle_limb;
        ::std::uint64_t const carry_middle = middle < self.middle_limb ? 1 : 0;
        auto const middle_adjusted = middle + carry_low;
        ::std::uint64_t const carry_adjusted = middle_adjusted < carry_low ? 1 : 0;
        auto const high = self.high_limb + right.high_limb + carry_middle + carry_adjusted;
        return UInt192{low, middle_adjusted, high};
    }

    /// Subtracts two values modulo 2^192.
    [[nodiscard]]
    constexpr auto operator-(this UInt192 const& self, UInt192 const& right) noexcept -> UInt192 {
        auto const low = self.low_limb - right.low_limb;
        ::std::uint64_t const borrow_low = self.low_limb < right.low_limb ? 1 : 0;
        auto const middle = self.middle_limb - right.middle_limb;
        ::std::uint64_t const borrow_middle = self.middle_limb < right.middle_limb ? 1 : 0;
        auto const middle_adjusted = middle - borrow_low;
        ::std::uint64_t const borrow_adjusted = middle < borrow_low ? 1 : 0;
        auto const high = self.high_limb - right.high_limb - borrow_middle - borrow_adjusted;
        return UInt192{low, middle_adjusted, high};
    }

private:
    /// Multiplies by a 64-bit factor, keeping the low 192 bits.
    [[nodiscard]]
    constexpr auto multiply_64(this UInt192 const& self, ::std::uint64_t const other) noexcept -> UInt192 {
        auto const low_product = UInt128{self.low_limb} * other;
        auto const middle_product = UInt128{self.middle_limb} * other;
        auto const high_product = UInt128{self.high_limb} * other;

        auto const low_high = static_cast<::std::uint64_t>(low_product >> 64);
        auto const middle = static_cast<::std::uint64_t>(middle_product) + low_high;
        ::std::uint64_t const middle_carry = middle < low_high ? 1 : 0;
        auto const high = static_cast<::std::uint64_t>(high_product) +
                          static_cast<::std::uint64_t>(middle_product >> 64) + middle_carry;
        return UInt192{static_cast<::std::uint64_t>(low_product), middle, high};
    }

    /// Multiplies by a 128-bit factor, keeping the low 192 bits.
    [[nodiscard]]
    constexpr auto multiply_128(this UInt192 const& self, UInt128 const other) noexcept -> UInt192 {
        auto const factor_low = static_cast<::std::uint64_t>(other);
        auto const factor_high = static_cast<::std::uint64_t>(other >> 64);

        auto const p00 = UInt128{self.low_limb} * factor_low;
        auto const p01 = UInt128{self.low_limb} * factor_high;
        auto const p10 = UInt128{self.middle_limb} * factor_low;
        auto const p11 = UInt128{self.middle_limb} * factor_high;
        auto const p20 = UInt128{self.high_limb} * factor_low;

        auto const low = static_cast<::std::uint64_t>(p00);
        auto const carry_low = static_cast<::std::uint64_t>(p00 >> 64);

        auto const sum_low = static_cast<::std::uint64_t>(p01) + static_cast<::std::uint64_t>(p10);
        auto const carry_sum_low = sum_low < static_cast<::std::uint64_t>(p01) ? 1 : 0;
        auto const middle = sum_low + carry_low;
        auto const carry_middle = middle < sum_low ? 1 : 0;
        auto const carry_products = static_cast<::std::uint64_t>(p01 >> 64) + static_cast<::std::uint64_t>(p10 >> 64) +
                                    carry_sum_low + carry_middle;

        auto const high = static_cast<::std::uint64_t>(p11) + static_cast<::std::uint64_t>(p20) + carry_products;
        return UInt192{low, middle, high};
    }

    /// Multiplies by a full 192-bit factor, keeping the low 192 bits.
    [[nodiscard]]
    constexpr auto multiply_192(this UInt192 const& self, UInt192 const& other) noexcept -> UInt192 {
        auto const p00 = UInt128{self.low_limb} * other.low_limb;
        auto const p01 = UInt128{self.low_limb} * other.middle_limb;
        auto const p10 = UInt128{self.middle_limb} * other.low_limb;
        auto const p02 = UInt128{self.low_limb} * other.high_limb;
        auto const p11 = UInt128{self.middle_limb} * other.middle_limb;
        auto const p20 = UInt128{self.high_limb} * other.low_limb;

        auto const low = static_cast<::std::uint64_t>(p00);
        auto const carry_low = static_cast<::std::uint64_t>(p00 >> 64);

        auto const sum_low = static_cast<::std::uint64_t>(p01) + static_cast<::std::uint64_t>(p10);
        auto const carry_sum_low = sum_low < static_cast<::std::uint64_t>(p01) ? 1 : 0;
        auto const middle = sum_low + carry_low;
        auto const carry_middle = middle < sum_low ? 1 : 0;
        auto const carry_products = static_cast<::std::uint64_t>(p01 >> 64) + static_cast<::std::uint64_t>(p10 >> 64) +
                                    carry_sum_low + carry_middle;

        auto const sum_high = static_cast<::std::uint64_t>(p02) + static_cast<::std::uint64_t>(p11);
        auto const high = sum_high + static_cast<::std::uint64_t>(p20) + carry_products;
        return UInt192{low, middle, high};
    }

public:
    /// Shifts left, discarding bits that leave the type.
    [[nodiscard]]
    constexpr auto operator<<(this UInt192 const& self, unsigned const count) noexcept -> UInt192 {
        if (count >= 192) {
            return UInt192{};
        }
        if (count == 0) {
            return self;
        }
        auto const limb_shift = count / 64;
        auto const bit_shift = count % 64;
        ::std::uint64_t const source[3]{self.low_limb, self.middle_limb, self.high_limb};
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
        return UInt192{result[0], result[1], result[2]};
    }

    /// Shifts right, discarding the bits shifted out.
    [[nodiscard]]
    constexpr auto operator>>(this UInt192 const& self, unsigned const count) noexcept -> UInt192 {
        if (count >= 192) {
            return UInt192{};
        }
        if (count == 0) {
            return self;
        }
        auto const limb_shift = count / 64;
        auto const bit_shift = count % 64;
        ::std::uint64_t const source[3]{self.low_limb, self.middle_limb, self.high_limb};
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
        return UInt192{result[0], result[1], result[2]};
    }

    /// Bitwise OR, used to assemble multi-limb values from shifted 64-bit parts.
    [[nodiscard]]
    constexpr auto operator|(this UInt192 const& self, UInt192 const& other) noexcept -> UInt192 {
        return UInt192{self.low_limb | other.low_limb, self.middle_limb | other.middle_limb,
                       self.high_limb | other.high_limb};
    }

    /// Bitwise AND.
    [[nodiscard]]
    constexpr auto operator&(this UInt192 const& self, UInt192 const& other) noexcept -> UInt192 {
        return UInt192{self.low_limb & other.low_limb, self.middle_limb & other.middle_limb,
                       self.high_limb & other.high_limb};
    }

    /// Adds an integer operand, sign-extending it when signed.
    template<details::integer_operand T>
    [[nodiscard]]
    constexpr auto operator+(this UInt192 const& self, T const& other) noexcept -> UInt192
        requires (!::std::same_as<T, UInt192>)
    {
        return self + UInt192{other};
    }

    template<details::integer_operand T>
    [[nodiscard]]
    friend constexpr auto operator+(T const& left, UInt192 const& right) noexcept -> UInt192
        requires (!::std::same_as<T, UInt192>)
    {
        return UInt192{left} + right;
    }

    /// Subtracts an integer operand, sign-extending it when signed.
    template<details::integer_operand T>
    [[nodiscard]]
    constexpr auto operator-(this UInt192 const& self, T const& other) noexcept -> UInt192
        requires (!::std::same_as<T, UInt192>)
    {
        return self - UInt192{other};
    }

    template<details::integer_operand T>
    [[nodiscard]]
    friend constexpr auto operator-(T const& left, UInt192 const& right) noexcept -> UInt192
        requires (!::std::same_as<T, UInt192>)
    {
        return UInt192{left} - right;
    }

    /// Multiplies by an integer operand, keeping the low 192 bits.
    template<details::integer_operand T>
    [[nodiscard]]
    constexpr auto operator*(this UInt192 const& self, T const& other) noexcept -> UInt192 {
        if constexpr (::std::same_as<T, UInt192>) {
            if (other.middle_limb == 0 && other.high_limb == 0) {
                return self.multiply_64(other.low_limb);
            }
            return self.multiply_192(other);
        }
        else if constexpr (::std::same_as<T, UInt128>) {
            return self.multiply_128(other);
        }
        else if constexpr (::std::unsigned_integral<T> && sizeof(T) <= 8) {
            return self.multiply_64(static_cast<::std::uint64_t>(other));
        }
        else if constexpr (::std::signed_integral<T> && sizeof(T) <= 8) {
            if (other < 0) {
                return UInt192{} - self.multiply_64(0 - static_cast<::std::uint64_t>(other));
            }
            return self.multiply_64(static_cast<::std::uint64_t>(other));
        }
        else {
            return self.multiply_192(UInt192{other});
        }
    }

    template<details::integer_operand T>
    [[nodiscard]]
    friend constexpr auto operator*(T const& factor, UInt192 const& value) noexcept -> UInt192
        requires (!::std::same_as<T, UInt192>)
    {
        return value * factor;
    }

    /// Bitwise OR with an integer operand.
    template<details::integer_operand T>
    [[nodiscard]]
    constexpr auto operator|(this UInt192 const& self, T const& other) noexcept -> UInt192
        requires (!::std::same_as<T, UInt192>)
    {
        return self | UInt192{other};
    }

    template<details::integer_operand T>
    [[nodiscard]]
    friend constexpr auto operator|(T const& left, UInt192 const& right) noexcept -> UInt192
        requires (!::std::same_as<T, UInt192>)
    {
        return UInt192{left} | right;
    }

    /// Bitwise AND with an integer operand.
    template<details::integer_operand T>
    [[nodiscard]]
    constexpr auto operator&(this UInt192 const& self, T const& other) noexcept -> UInt192
        requires (!::std::same_as<T, UInt192>)
    {
        return self & UInt192{other};
    }

    template<details::integer_operand T>
    [[nodiscard]]
    friend constexpr auto operator&(T const& left, UInt192 const& right) noexcept -> UInt192
        requires (!::std::same_as<T, UInt192>)
    {
        return UInt192{left} & right;
    }

    [[nodiscard]]
    constexpr auto operator==(this UInt192 const&, UInt192 const&) noexcept -> bool = default;

    /// Three-way comparison, most significant limb first.
    [[nodiscard]]
    constexpr auto operator<=>(this UInt192 const& self, UInt192 const& other) noexcept -> ::std::strong_ordering {
        if (self.high_limb != other.high_limb) {
            return self.high_limb < other.high_limb ? ::std::strong_ordering::less : ::std::strong_ordering::greater;
        }
        if (self.middle_limb != other.middle_limb) {
            return self.middle_limb < other.middle_limb ? ::std::strong_ordering::less
                                                        : ::std::strong_ordering::greater;
        }
        if (self.low_limb != other.low_limb) {
            return self.low_limb < other.low_limb ? ::std::strong_ordering::less : ::std::strong_ordering::greater;
        }
        return ::std::strong_ordering::equal;
    }
};

/**
 * @brief Compute 5^exponent as a 192-bit integer.
 */
[[nodiscard]]
constexpr auto pow5(::std::size_t exponent) noexcept -> UInt192 {
    UInt192 result{1};
    for (::std::size_t i{}; i < exponent; ++i) {
        result = result * ::std::uint64_t{5};
    }
    return result;
}

/**
 * @brief Compute 10^exponent as a 192-bit integer.
 */
[[nodiscard]]
constexpr auto pow10(::std::size_t exponent) noexcept -> UInt192 {
    UInt192 result{1};
    for (::std::size_t i{}; i < exponent; ++i) {
        result = result * ::std::uint64_t{10};
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
    auto const bits = numerator.bit_length();
    UInt192 quotient{};
    UInt192 remainder{};
    auto const limbs = (bits + 63) / 64;
    for (unsigned limb_offset{}; limb_offset < limbs; ++limb_offset) {
        auto const limb_index = limbs - 1 - limb_offset;
        auto const limb = static_cast<::std::uint64_t>(numerator >> (limb_index * 64));
        auto const limb_bits = (limb_offset == 0) ? (bits - limb_index * 64) : 64;
        for (unsigned bit = limb_bits; bit-- > 0;) {
            bool const set = ((limb >> bit) & 1) != 0;
            remainder = (remainder << 1) + UInt192{static_cast<::std::uint64_t>(set ? 1u : 0u)};
            if (remainder >= divisor) {
                remainder = remainder - divisor;
                quotient = quotient + (UInt192{1} << (limb_index * 64 + bit));
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
    if (numerator == UInt192{}) {
        return 0.0;
    }
    auto const divisor = ::pltxt2htm::numeric::pow10(scale_digits);
    auto const numerator_bits = numerator.bit_length();
    auto const divisor_bits = divisor.bit_length();
    // Target a quotient of 57-58 bits: the 53-bit significand, one round bit, and slack. With this
    // offset the shifted numerator stays below 2^184 and the shifted divisor below 2^135, so both
    // fit the 192-bit type without truncation.
    auto const shift = static_cast<int>(divisor_bits) + 57 - static_cast<int>(numerator_bits);
    UInt192 const scaled_numerator = shift >= 0 ? numerator << static_cast<unsigned>(shift) : numerator;
    UInt192 const scaled_divisor = shift >= 0 ? divisor : divisor << static_cast<unsigned>(-shift);
    auto const division = ::pltxt2htm::numeric::divide(scaled_numerator, scaled_divisor);

    auto const quotient = division.quotient;
    auto const quotient_bits = quotient.bit_length();
    // Keep 53 bits and one round bit; drop_bits is 4 or 5 for the targeted quotient size, so the
    // round/sticky bits below the mantissa are exact.
    auto const drop_bits = quotient_bits - 53;
    auto const quotient_low = static_cast<::std::uint64_t>(quotient);
    auto mantissa = quotient_low >> drop_bits;
    bool const round_bit = ((quotient_low >> (drop_bits - 1)) & 1) != 0;
    ::std::uint64_t const low_mask = (::std::uint64_t{1} << (drop_bits - 1)) - 1;
    bool const sticky = (quotient_low & low_mask) != 0 || (quotient >> 64) != UInt192{} ||
                        division.remainder != UInt192{} || extra_sticky;
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
