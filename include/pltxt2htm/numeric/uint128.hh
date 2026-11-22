/**
 * @file uint128.hh
 * @brief 128-bit unsigned integer used by the exact decimal helpers.
 * @details `UInt128` is the compiler's native 128-bit unsigned type when available; otherwise it
 *          is a portable two-limb class implementing only the operations the decimal code needs:
 *          construction from a 64-bit value, multiplication by a 64-bit factor, logical right
 *          shift, and explicit conversion to an unsigned integer type.
 */

#pragma once

#include <concepts>
#include <cstdint>

namespace pltxt2htm::numeric {

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
    ::std::uint64_t low_limb{};
    ::std::uint64_t high_limb{};

    constexpr UInt128(::std::uint64_t const low, ::std::uint64_t const high) noexcept
        : low_limb{low},
          high_limb{high} {
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
        : low_limb{value} {
    }

    /// Explicit conversion to an unsigned integer target of at most 64 bits.
    /// @details `bool` reports whether the value is non-zero; other targets keep the low bits and
    /// truncate. A 16-byte target cannot occur: this class is only selected when no native 128-bit
    /// type exists.
    template<::std::unsigned_integral T>
    [[nodiscard]]
    constexpr explicit operator T(this UInt128 const& self) noexcept {
        if constexpr (::std::same_as<T, bool>) {
            return self.low_limb != 0 || self.high_limb != 0;
        }
        else {
            static_assert(sizeof(T) <= 8, "the portable UInt128 converts to at most 64 bits");
            return static_cast<T>(self.low_limb);
        }
    }

    /// Adds two values modulo 2^128.
    [[nodiscard]]
    constexpr auto operator+(this UInt128 const& self, UInt128 const& other) noexcept -> UInt128 {
        auto const low = self.low_limb + other.low_limb;
        ::std::uint64_t const carry = low < self.low_limb ? 1 : 0;
        auto const high = self.high_limb + other.high_limb + carry;
        return UInt128{low, high};
    }

    /// Subtracts two values modulo 2^128.
    [[nodiscard]]
    constexpr auto operator-(this UInt128 const& self, UInt128 const& other) noexcept -> UInt128 {
        auto const low = self.low_limb - other.low_limb;
        ::std::uint64_t const borrow = self.low_limb < other.low_limb ? 1 : 0;
        auto const high = self.high_limb - other.high_limb - borrow;
        return UInt128{low, high};
    }

    /// Multiplies by a 64-bit factor, keeping the low 128 bits.
    [[nodiscard]]
    constexpr auto operator*(this UInt128 const& self, ::std::uint64_t const factor) noexcept -> UInt128 {
        auto const low_product = UInt128::multiply_64x64(self.low_limb, factor);
        auto const high_product = UInt128::multiply_64x64(self.high_limb, factor);
        return UInt128{low_product.low_limb,
                       static_cast<::std::uint64_t>(high_product.low_limb + low_product.high_limb)};
    }

    /// Shifts left, discarding the bits shifted out. `count` must be less than 128 to match the
    /// native type; the portable implementation returns zero for larger counts.
    [[nodiscard]]
    constexpr auto operator<<(this UInt128 const& self, unsigned const count) noexcept -> UInt128 {
        if (count == 0) {
            return self;
        }
        if (count >= 128) {
            return UInt128{};
        }
        if (count < 64) {
            return UInt128{self.low_limb << count, (self.high_limb << count) | (self.low_limb >> (64 - count))};
        }
        return UInt128{0, self.low_limb << (count - 64)};
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
            return UInt128{(self.low_limb >> bit_shift) | (self.high_limb << (64 - bit_shift)),
                           self.high_limb >> bit_shift};
        }
        return UInt128{self.high_limb >> bit_shift, 0};
    }
};
#endif

} // namespace pltxt2htm::numeric
