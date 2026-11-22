/**
 * @file wide_decimal.hh
 * @brief Exact fixed-point helpers shared by decimal parsing and fixed-point formatting.
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

namespace pltxt2htm::details::wide {

#if defined(__SIZEOF_INT128__)
/// The compiler's native 128-bit unsigned type, accepted without a pedantic warning.
/// The alias is required: GCC rejects every bare `unsigned __int128` spelling under
/// `-Wpedantic`, which the project builds with `-Werror=pedantic`. A future standard `uint128_t`
/// would replace only this line.
__extension__ using UInt128 = unsigned __int128;
#else
/**
 * @brief Portable 128-bit unsigned integer with unsigned (modulo 2^128) arithmetic.
 * @details Used only when the compiler provides no native 128-bit unsigned type (for example
 *          MSVC and 32-bit targets). Supports what the decimal code needs: construction from a
 *          64-bit value, multiplication by a 64-bit factor, logical right shift, and explicit
 *          conversion to an unsigned integer type (the low 64 bits for targets up to 64 bits).
 */
class UInt128 {
    ::std::uint64_t low_limb_{};
    ::std::uint64_t high_limb_{};

    constexpr UInt128(::std::uint64_t const low, ::std::uint64_t const high) noexcept
        : low_limb_{low},
          high_limb_{high} {
    }

    /**
     * @brief Portable 64x64->128 multiplication on 32-bit halves.
     */
    [[nodiscard]]
    static constexpr auto multiply_64x64(::std::uint64_t const left, ::std::uint64_t const right) noexcept -> UInt128 {
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
        return UInt128{low, high};
    }

public:
    constexpr UInt128() noexcept = default;

    /// Constructs a value from a 64-bit integer; the conversion is lossless and implicit.
    constexpr UInt128(::std::uint64_t const value) noexcept
        : low_limb_{value} {
    }

    /// Explicit conversion to an unsigned integer target of at most 64 bits.
    /// @details `bool` reports whether the value is non-zero; other targets keep the low bits and
    /// truncate. A 16-byte target cannot occur: this class is only selected when no native 128-bit
    /// type exists.
    template<::std::unsigned_integral T>
    [[nodiscard]]
    constexpr explicit operator T(this UInt128 const& self) noexcept {
        if constexpr (::std::same_as<T, bool>) {
            return self.low_limb_ != 0 || self.high_limb_ != 0;
        }
        else {
            static_assert(sizeof(T) <= 8, "the portable UInt128 converts to at most 64 bits");
            return static_cast<T>(self.low_limb_);
        }
    }

    /// Multiplies by a 64-bit factor, keeping the low 128 bits.
    [[nodiscard]]
    constexpr auto operator*(this UInt128 const& self, ::std::uint64_t const factor) noexcept -> UInt128 {
        auto const low_product = UInt128::multiply_64x64(self.low_limb_, factor);
        auto const high_product = UInt128::multiply_64x64(self.high_limb_, factor);
        return UInt128{low_product.low_limb_,
                       static_cast<::std::uint64_t>(high_product.low_limb_ + low_product.high_limb_)};
    }

    /// Logical right shift. `count` must be less than 128 to match the native type; the portable
    /// implementation additionally returns zero for larger counts so it never has undefined
    /// behavior.
    [[nodiscard]]
    constexpr auto operator>>(this UInt128 const& self, unsigned const count) noexcept -> UInt128 {
        if (count == 0) {
            return self;
        }
        if (count >= 128) {
            return UInt128{};
        }
        auto const limb_shift = count / 64;
        auto const bit_shift = count % 64;
        if (limb_shift == 0) {
            return UInt128{(self.low_limb_ >> bit_shift) | (self.high_limb_ << (64 - bit_shift)),
                           self.high_limb_ >> bit_shift};
        }
        return UInt128{self.high_limb_ >> bit_shift, 0};
    }
};
#endif

/**
 * @brief Targets the explicit integer conversions accept.
 * @details All standard unsigned integer types, plus the native 128-bit type when it is
 *          available. Some standard libraries do not classify the native 128-bit type as an
 *          integral type, so it is listed explicitly.
 */
#if defined(__SIZEOF_INT128__)
template<typename T>
concept UnsignedConversionTarget = ::std::unsigned_integral<T> || ::std::same_as<T, UInt128>;
#else
template<typename T>
concept UnsignedConversionTarget = ::std::unsigned_integral<T>;
#endif
/**
 * @brief A 192-bit unsigned integer with unsigned (modulo 2^192) arithmetic.
 */
class UInt192 {
    ::std::uint64_t low_limb_{}; ///< Least significant 64-bit limb.
    ::std::uint64_t middle_limb_{}; ///< Middle 64-bit limb.
    ::std::uint64_t high_limb_{}; ///< Most significant 64-bit limb.

    /// Constructs a value from little-endian limbs. Callers inside the class and its hidden
    /// friends use this; external callers assemble multi-limb values with shifts and `operator|`.
    constexpr UInt192(::std::uint64_t const low, ::std::uint64_t const middle, ::std::uint64_t const high) noexcept
        : low_limb_{low},
          middle_limb_{middle},
          high_limb_{high} {
    }

public:
    constexpr UInt192() noexcept = default;

    /// Constructs a value from a 64-bit integer; the conversion is lossless and implicit.
    constexpr UInt192(::std::uint64_t const value) noexcept
        : low_limb_{value} {
    }

    /// Explicit conversion to an unsigned integer-like target.
    /// @details `bool` reports whether the value is non-zero. Targets up to 64 bits keep the low
    /// bits and truncate; a 128-bit target keeps the low 128 bits. Conversions are explicit, so
    /// narrowing never happens implicitly.
    template<UnsignedConversionTarget T>
    [[nodiscard]]
    constexpr explicit operator T(this UInt192 const& self) noexcept {
        if constexpr (::std::same_as<T, bool>) {
            return self != UInt192{};
        }
        else if constexpr (sizeof(T) <= 8) {
            return static_cast<T>(self.low_limb_);
        }
        else {
            static_assert(sizeof(T) == 16, "UInt192 converts to at most 128 bits");
            return (static_cast<T>(self.middle_limb_) << 64) | static_cast<T>(self.low_limb_);
        }
    }

    /// Number of significant bits, or 0 for zero.
    [[nodiscard]]
    constexpr auto bit_length() const noexcept -> unsigned {
        if (high_limb_ != 0) {
            return static_cast<unsigned>(128 + ::std::bit_width(high_limb_));
        }
        if (middle_limb_ != 0) {
            return static_cast<unsigned>(64 + ::std::bit_width(middle_limb_));
        }
        return static_cast<unsigned>(::std::bit_width(low_limb_));
    }

    /// Returns the bit at index, or false when index >= 192.
    [[nodiscard]]
    constexpr auto bit(unsigned const index) const noexcept -> bool {
        if (index >= 192) {
            return false;
        }
        auto const limb = index < 64 ? low_limb_ : (index < 128 ? middle_limb_ : high_limb_);
        return ((limb >> (index % 64)) & 1) != 0;
    }

    /// True when the value fits a ::std::uint64_t.
    [[nodiscard]]
    constexpr auto fits_uint64() const noexcept -> bool {
        return middle_limb_ == 0 && high_limb_ == 0;
    }

    /// Adds two values modulo 2^192.
    [[nodiscard]]
    friend constexpr auto operator+(UInt192 const& left, UInt192 const& right) noexcept -> UInt192 {
        auto const low = left.low_limb_ + right.low_limb_;
        ::std::uint64_t const carry_low = low < left.low_limb_ ? 1 : 0;
        auto const middle = left.middle_limb_ + right.middle_limb_;
        ::std::uint64_t const carry_middle = middle < left.middle_limb_ ? 1 : 0;
        auto const middle_adjusted = middle + carry_low;
        ::std::uint64_t const carry_adjusted = middle_adjusted < carry_low ? 1 : 0;
        auto const high = left.high_limb_ + right.high_limb_ + carry_middle + carry_adjusted;
        return UInt192{low, middle_adjusted, high};
    }

    /// Subtracts two values modulo 2^192.
    [[nodiscard]]
    friend constexpr auto operator-(UInt192 const& left, UInt192 const& right) noexcept -> UInt192 {
        auto const low = left.low_limb_ - right.low_limb_;
        ::std::uint64_t const borrow_low = left.low_limb_ < right.low_limb_ ? 1 : 0;
        auto const middle = left.middle_limb_ - right.middle_limb_;
        ::std::uint64_t const borrow_middle = left.middle_limb_ < right.middle_limb_ ? 1 : 0;
        auto const middle_adjusted = middle - borrow_low;
        ::std::uint64_t const borrow_adjusted = middle < borrow_low ? 1 : 0;
        auto const high = left.high_limb_ - right.high_limb_ - borrow_middle - borrow_adjusted;
        return UInt192{low, middle_adjusted, high};
    }

    /// Multiplies by a 64-bit factor, keeping the low 192 bits.
    [[nodiscard]]
    friend constexpr auto operator*(UInt192 const& value, ::std::uint64_t const factor) noexcept -> UInt192 {
        auto const low_product = UInt128{value.low_limb_} * factor;
        auto const middle_product = UInt128{value.middle_limb_} * factor;
        auto const high_product = UInt128{value.high_limb_} * factor;

        auto const low_high = static_cast<::std::uint64_t>(low_product >> 64);
        auto const middle = static_cast<::std::uint64_t>(middle_product) + low_high;
        ::std::uint64_t const middle_carry = middle < low_high ? 1 : 0;
        auto const high = static_cast<::std::uint64_t>(high_product) +
                          static_cast<::std::uint64_t>(middle_product >> 64) + middle_carry;
        return UInt192{static_cast<::std::uint64_t>(low_product), middle, high};
    }

    /// Shifts left, discarding bits that leave the type.
    [[nodiscard]]
    friend constexpr auto operator<<(UInt192 const& value, unsigned const count) noexcept -> UInt192 {
        if (count >= 192) {
            return UInt192{};
        }
        if (count == 0) {
            return value;
        }
        auto const limb_shift = count / 64;
        auto const bit_shift = count % 64;
        ::std::uint64_t const source[3]{value.low_limb_, value.middle_limb_, value.high_limb_};
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
    friend constexpr auto operator>>(UInt192 const& value, unsigned const count) noexcept -> UInt192 {
        if (count >= 192) {
            return UInt192{};
        }
        if (count == 0) {
            return value;
        }
        auto const limb_shift = count / 64;
        auto const bit_shift = count % 64;
        ::std::uint64_t const source[3]{value.low_limb_, value.middle_limb_, value.high_limb_};
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
    friend constexpr auto operator|(UInt192 const& left, UInt192 const& right) noexcept -> UInt192 {
        return UInt192{left.low_limb_ | right.low_limb_, left.middle_limb_ | right.middle_limb_,
                       left.high_limb_ | right.high_limb_};
    }

    [[nodiscard]]
    constexpr auto operator==(this UInt192 const&, UInt192 const&) noexcept -> bool = default;

    /// Three-way comparison, most significant limb first.
    [[nodiscard]]
    constexpr auto operator<=>(this UInt192 const& self, UInt192 const& other) noexcept -> ::std::strong_ordering {
        if (self.high_limb_ != other.high_limb_) {
            return self.high_limb_ < other.high_limb_ ? ::std::strong_ordering::less : ::std::strong_ordering::greater;
        }
        if (self.middle_limb_ != other.middle_limb_) {
            return self.middle_limb_ < other.middle_limb_ ? ::std::strong_ordering::less
                                                          : ::std::strong_ordering::greater;
        }
        if (self.low_limb_ != other.low_limb_) {
            return self.low_limb_ < other.low_limb_ ? ::std::strong_ordering::less : ::std::strong_ordering::greater;
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
        result = result * 5;
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
        result = result * 10;
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
    for (unsigned index = bits; index-- > 0;) {
        remainder = (remainder << 1) + UInt192{static_cast<::std::uint64_t>(numerator.bit(index) ? 1u : 0u)};
        if (remainder >= divisor) {
            remainder = remainder - divisor;
            quotient = quotient + (UInt192{1} << index);
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
    auto const divisor = ::pltxt2htm::details::wide::pow10(scale_digits);
    auto const numerator_bits = numerator.bit_length();
    auto const divisor_bits = divisor.bit_length();
    // Target a quotient of 57-58 bits: the 53-bit significand, one round bit, and slack. With this
    // offset the shifted numerator stays below 2^184 and the shifted divisor below 2^135, so both
    // fit the 192-bit type without truncation.
    auto const shift = static_cast<int>(divisor_bits) + 57 - static_cast<int>(numerator_bits);
    UInt192 const scaled_numerator = shift >= 0 ? numerator << static_cast<unsigned>(shift) : numerator;
    UInt192 const scaled_divisor = shift >= 0 ? divisor : divisor << static_cast<unsigned>(-shift);
    auto const division = ::pltxt2htm::details::wide::divide(scaled_numerator, scaled_divisor);

    auto const quotient = division.quotient;
    auto const quotient_bits = quotient.bit_length();
    // Keep 53 bits and one round bit; drop_bits is 4 or 5 for the targeted quotient size, so the
    // round/sticky bits below the mantissa are exact.
    auto const drop_bits = quotient_bits - 53;
    auto const quotient_low = static_cast<::std::uint64_t>(quotient);
    auto mantissa = quotient_low >> drop_bits;
    bool const round_bit = ((quotient_low >> (drop_bits - 1)) & 1) != 0;
    ::std::uint64_t const low_mask = (::std::uint64_t{1} << (drop_bits - 1)) - 1;
    bool const sticky = (quotient_low & low_mask) != 0 || quotient.fits_uint64() == false ||
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

} // namespace pltxt2htm::details::wide