/**
 * @file uint128.hh
 * @brief 128-bit unsigned integer used by the exact decimal helpers.
 * @details `UInt128` has the same class interface on every target. It uses the compiler's native
 *          128-bit unsigned type internally when available and two 64-bit limbs otherwise.
 *          It implements the integer construction, arithmetic, shifts, and explicit unsigned
 *          conversions needed by the exact decimal code.
 */

#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>

#include "../details/push_macro.hh"

namespace pltxt2htm::numeric {

namespace details {

#if defined(__SIZEOF_INT128__)
// Name the compiler extensions once without pedantic warnings. Some standard libraries do not
// classify these types as integral, so the operand concepts include them explicitly.
__extension__ using NativeUInt128 = unsigned __int128;
__extension__ using NativeInt128 = __int128;

template<typename T>
concept unsigned_integer = ::std::unsigned_integral<T> || ::std::same_as<T, NativeUInt128>;

template<typename T>
concept signed_integer = ::std::signed_integral<T> || ::std::same_as<T, NativeInt128>;
#else
template<typename T>
concept unsigned_integer = ::std::unsigned_integral<T>;

template<typename T>
concept signed_integer = ::std::signed_integral<T>;
#endif

template<typename T>
concept integer = unsigned_integer<T> || signed_integer<T>;

} // namespace details

/**
 * @brief A 128-bit unsigned integer with unsigned (modulo 2^128) arithmetic.
 * @details Unsigned construction is implicit, signed construction is explicit and sign-extending,
 *          and conversions to unsigned integers are explicit. Default construction produces zero;
 *          shifts by 128 bits or more also produce zero, independently of the storage backend.
 */
class UInt128 {
#if defined(__SIZEOF_INT128__)
    details::NativeUInt128 native_value{};
#else
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
#endif

public:
    constexpr UInt128() noexcept = default;

    /// Constructs a value from an unsigned integer; the conversion is lossless and implicit.
    template<details::unsigned_integer T>
    constexpr UInt128(T const input) noexcept {
#if defined(__SIZEOF_INT128__)
        native_value = static_cast<details::NativeUInt128>(input);
#else
        low_limb = static_cast<::std::uint64_t>(input);
        if constexpr (sizeof(T) == 16) {
            high_limb = static_cast<::std::uint64_t>(input >> 64);
        }
#endif
    }

    /// Constructs a value from a signed integer, sign-extending it to 128 bits.
    template<details::signed_integer T>
    explicit constexpr UInt128(T const input) noexcept {
#if defined(__SIZEOF_INT128__)
        native_value = static_cast<details::NativeUInt128>(input);
#else
        low_limb = static_cast<::std::uint64_t>(input);
        if constexpr (sizeof(T) == 16) {
            high_limb = static_cast<::std::uint64_t>(input >> 64);
        }
        else if (input < 0) {
            high_limb = 0xFFFFFFFFFFFFFFFFull;
        }
#endif
    }

    /// Explicit conversion to an unsigned integer target.
    /// @details `bool` reports whether the value is non-zero; other targets keep the low bits and
    /// truncate. Conversion to the native unsigned 128-bit type is lossless when it is available.
    template<details::unsigned_integer T>
    [[nodiscard]]
    constexpr explicit operator T(this UInt128 const& self) noexcept {
#if defined(__SIZEOF_INT128__)
        return static_cast<T>(self.native_value);
#else
        if constexpr (::std::same_as<T, bool>) {
            return self.low_limb != 0 || self.high_limb != 0;
        }
        else {
            static_assert(sizeof(T) <= 8, "the portable UInt128 converts to at most 64 bits");
            return static_cast<T>(self.low_limb);
        }
#endif
    }

private:
    /// Tests one bit without materializing a shifted UInt128 value.
    [[nodiscard]]
    friend constexpr auto bittest_impl(UInt128 const& self, ::std::size_t const index) noexcept -> bool {
        pltxt2htm_assert(index < 128, u8"bittest index is out of range");
#if defined(__SIZEOF_INT128__)
        return ((self.native_value >> index) & details::NativeUInt128{1}) != 0;
#else
        if (index < 64) {
            return ((self.low_limb >> index) & ::std::uint64_t{1}) != 0;
        }
        return ((self.high_limb >> (index - 64)) & ::std::uint64_t{1}) != 0;
#endif
    }

    /// Adds two values modulo 2^128.
    [[nodiscard]]
    friend constexpr auto operator+(UInt128 const& self, UInt128 const& other) noexcept -> UInt128 {
#if defined(__SIZEOF_INT128__)
        return UInt128{self.native_value + other.native_value};
#else
        auto const low = self.low_limb + other.low_limb;
        ::std::uint64_t const carry = low < self.low_limb ? 1 : 0;
        auto const high = self.high_limb + other.high_limb + carry;
        return UInt128{low, high};
#endif
    }

    /// Subtracts two values modulo 2^128.
    [[nodiscard]]
    friend constexpr auto operator-(UInt128 const& self, UInt128 const& other) noexcept -> UInt128 {
#if defined(__SIZEOF_INT128__)
        return UInt128{self.native_value - other.native_value};
#else
        auto const low = self.low_limb - other.low_limb;
        ::std::uint64_t const borrow = self.low_limb < other.low_limb ? 1 : 0;
        auto const high = self.high_limb - other.high_limb - borrow;
        return UInt128{low, high};
#endif
    }

#if !defined(__SIZEOF_INT128__)
    /// Multiplies by a 64-bit factor, keeping the low 128 bits.
    [[nodiscard]]
    constexpr auto multiply_64(this UInt128 const& self, ::std::uint64_t const factor) noexcept -> UInt128 {
        auto const low_product = UInt128::multiply_64x64(self.low_limb, factor);
        auto const high = self.high_limb * factor + low_product.high_limb;
        return UInt128{low_product.low_limb, high};
    }

    /// Multiplies by a full 128-bit factor, keeping the low 128 bits.
    [[nodiscard]]
    constexpr auto multiply_128(this UInt128 const& self, UInt128 const& other) noexcept -> UInt128 {
        auto const low_product = UInt128::multiply_64x64(self.low_limb, other.low_limb);
        auto const high = low_product.high_limb + self.low_limb * other.high_limb + self.high_limb * other.low_limb;
        return UInt128{low_product.low_limb, high};
    }
#endif

    /// Multiplies by an integer operand modulo 2^128, sign-extending signed operands.
    template<typename T>
    [[nodiscard]]
    friend constexpr auto operator*(UInt128 const& self, T const& other) noexcept -> UInt128
        requires (details::integer<T> || ::std::same_as<T, UInt128>)
    {
#if defined(__SIZEOF_INT128__)
        return UInt128{self.native_value * UInt128{other}.native_value};
#else
        if constexpr (::std::same_as<T, UInt128>) {
            if (other.high_limb == 0) {
                return self.multiply_64(other.low_limb);
            }
            return self.multiply_128(other);
        }
        else if constexpr (details::unsigned_integer<T> && sizeof(T) <= 8) {
            return self.multiply_64(static_cast<::std::uint64_t>(other));
        }
        else if constexpr (details::signed_integer<T> && sizeof(T) <= 8) {
            if (other < 0) {
                return UInt128{} - self.multiply_64(0 - static_cast<::std::uint64_t>(other));
            }
            return self.multiply_64(static_cast<::std::uint64_t>(other));
        }
        else {
            return self.multiply_128(UInt128{other});
        }
#endif
    }

    template<details::integer T, typename R>
    [[nodiscard]]
    friend constexpr auto operator*(T const& factor, R const& value) noexcept -> UInt128
        requires (::std::same_as<R, UInt128>)
    {
        return value * factor;
    }

public:
    /// Shifts left, discarding the bits shifted out. Counts of 128 or more produce zero.
    [[nodiscard]]
    constexpr auto operator<<(this UInt128 const& self, unsigned const count) noexcept -> UInt128 {
        if (count == 0) {
            return self;
        }
        if (count >= 128) {
            return UInt128{};
        }
#if defined(__SIZEOF_INT128__)
        return UInt128{self.native_value << count};
#else
        if (count < 64) {
            return UInt128{self.low_limb << count, (self.high_limb << count) | (self.low_limb >> (64 - count))};
        }
        return UInt128{0, self.low_limb << (count - 64)};
#endif
    }

    /// Logical right shift. Counts of 128 or more produce zero.
    [[nodiscard]]
    constexpr auto operator>>(this UInt128 const& self, unsigned const count) noexcept -> UInt128 {
        if (count == 0) {
            return self;
        }
        if (count >= 128) {
            return UInt128{};
        }
#if defined(__SIZEOF_INT128__)
        return UInt128{self.native_value >> count};
#else
        auto const limb_shift = count / 64;
        auto const bit_shift = count % 64;
        if (limb_shift == 0) {
            return UInt128{(self.low_limb >> bit_shift) | (self.high_limb << (64 - bit_shift)),
                           self.high_limb >> bit_shift};
        }
        return UInt128{self.high_limb >> bit_shift, 0};
#endif
    }
};

} // namespace pltxt2htm::numeric

#include "../details/pop_macro.hh"
