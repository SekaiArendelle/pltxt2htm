/**
 * @file floating.hh
 * @brief Fixed-point decimal formatting for IEEE-754 binary64 values.
 * @details Emits the shortest fixed-point decimal that round-trips back to the
 *          same double, which is the same conversion `std::to_chars` performs
 *          for `std::chars_format::fixed` on finite values: no exponent
 *          notation, no locale, no allocation in the conversion itself. The
 *          whole finite domain is supported, including subnormal and negative
 *          values. Negative zero is written as "-0"; non-finite values are
 *          written as "nan", "-nan", "inf" and "-inf" according to their sign
 *          bit.
 *
 *          Integral values are emitted as their exact integer expansion, which
 *          is the shortest fixed representation with distance zero (the same
 *          choice std::to_chars makes); other values use the shortest decimal
 *          digits that round-trip. Both cases are computed without runtime
 *          allocation, and the conversion is constexpr-friendly.
 *
 *          The shortest digits come from the vendored Dragonbox implementation;
 *          the exact integer expansion uses a small fixed-size base-2^32
 *          accumulator defined in this header.
 */

#pragma once

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>

#include "../container/array.hh"
#include "../container/string.hh"
#include "integer.hh"

// Configure the vendored Dragonbox before its first inclusion. The vendored
// header leaves the macro defined when the including file supplied it, so it is
// undefined again below.
#define JKJ_NAMESPACE pltxt2htm_dragonbox
#include <dragonbox/dragonbox.h>
#undef JKJ_NAMESPACE

namespace pltxt2htm::format {

namespace details {

/**
 * @brief Maximum number of characters a finite double needs in fixed notation.
 * @details The decimal exponent of a finite non-zero double is at least -324.
 *          For a negative exponent the text is `0.` followed by `-exponent`
 *          digits, so the worst case is 2 + 324 = 326 characters; a negative
 *          sign adds one more, giving 327.
 */
inline constexpr ::std::size_t max_fixed_point_length{327};

/**
 * @brief Write the decimal digits of an unsigned 64-bit value.
 * @param buffer Destination with room for 20 characters.
 * @return The number of characters written.
 */
[[nodiscard]]
constexpr auto write_uint64_digits(::std::uint64_t value, char8_t* const buffer) noexcept -> ::std::size_t {
    auto reversed = ::pltxt2htm::container::Array<char8_t, 20>{};
    auto* const reversed_data = reversed.data();
    ::std::size_t reversed_length{};
    do {
        reversed_data[reversed_length++] = static_cast<char8_t>(u8'0' + value % 10);
        value /= 10;
    } while (value != 0);
    for (::std::size_t i{}; i < reversed_length; ++i) {
        buffer[i] = reversed_data[reversed_length - 1 - i];
    }
    return reversed_length;
}

/// Base-2^32 limbs, least significant first. 32 limbs cover any integral double.
using ExactIntegerLimbs = ::std::array<::std::uint32_t, 32>;

/**
 * @brief Divide the significant limbs by divisor in place and return the remainder.
 * @param[in,out] limbs The accumulator; only [0, significant_limbs) is read.
 * @param[in,out] significant_limbs Number of limbs above the most significant
 *                zero limb; updated after the division.
 * @param divisor The 32-bit divisor.
 */
[[nodiscard]]
constexpr auto divide_limbs(ExactIntegerLimbs& limbs, ::std::size_t& significant_limbs,
                            ::std::uint32_t const divisor) noexcept -> ::std::uint32_t {
    ::std::uint64_t remainder{};
    for (::std::size_t i{significant_limbs}; i != 0; --i) {
        ::std::uint64_t const current = (remainder << 32) | limbs[i - 1];
        limbs[i - 1] = static_cast<::std::uint32_t>(current / divisor);
        remainder = current % divisor;
    }
    while (significant_limbs != 0 && limbs[significant_limbs - 1] == 0) {
        --significant_limbs;
    }
    return static_cast<::std::uint32_t>(remainder);
}

/**
 * @brief Write the exact decimal digits of a non-negative integral double.
 * @param value A finite, non-negative, integral binary64 value.
 * @param buffer Destination with room for at least 309 characters.
 * @return The number of characters written.
 * @details The exact expansion is the representation `std::to_chars` selects for
 *          `chars_format::fixed`, because it has the minimal length (the number
 *          of integer digits) and distance zero.
 * @note The 1024-bit division loop is only reached for magnitudes at or above
 *       2^64. It is slower than the table-driven std::to_chars path, but keeps
 *       the exact digits without pulling in a big-integer dependency, and the
 *       font-size use case never reaches it.
 */
[[nodiscard]]
constexpr auto exact_integer_digits(double const value, char8_t* const buffer) noexcept -> ::std::size_t {
    auto const bits = ::std::bit_cast<::std::uint64_t>(value);
    if (bits == 0) {
        buffer[0] = u8'0';
        return 1;
    }

    auto const exponent_bits = static_cast<int>((bits >> 52) & 0x7ff);
    auto const unbiased_exponent = exponent_bits - 1023;
    auto const significand = (::std::uint64_t{1} << 52) | (bits & 0x000f'ffff'ffff'ffffull);

    // Values below 2^52 need a right shift; the low bits are zero because the
    // caller guarantees the value is integral.
    if (unbiased_exponent < 52) {
        return ::pltxt2htm::format::details::write_uint64_digits(
            significand >> static_cast<unsigned>(52 - unbiased_exponent), buffer);
    }
    // Values below 2^64 still fit a 64-bit integer.
    if (unbiased_exponent <= 63) {
        return ::pltxt2htm::format::details::write_uint64_digits(
            significand << static_cast<unsigned>(unbiased_exponent - 52), buffer);
    }

    // Larger integral values need up to 1024 bits; accumulate in base 2^32.
    ExactIntegerLimbs limbs{};
    limbs[0] = static_cast<::std::uint32_t>(significand);
    limbs[1] = static_cast<::std::uint32_t>(significand >> 32);

    auto const shift = static_cast<unsigned>(unbiased_exponent - 52);
    auto const limb_shift = shift / 32;
    auto const bit_shift = shift % 32;
    if (limb_shift != 0) {
        for (::std::size_t i{limbs.size()}; i > limb_shift; --i) {
            limbs[i - 1] = limbs[i - 1 - limb_shift];
        }
        for (::std::size_t i{}; i < limb_shift; ++i) {
            limbs[i] = 0;
        }
    }
    if (bit_shift != 0) {
        for (::std::size_t i{limbs.size()}; i != 0; --i) {
            auto const index = i - 1;
            ::std::uint32_t const carry = index == 0 ? 0 : limbs[index - 1] >> (32 - bit_shift);
            limbs[index] = (limbs[index] << bit_shift) | carry;
        }
    }

    // Peel off nine decimal digits per division, least significant chunk first.
    constexpr ::std::size_t max_digits{309};
    auto digits = ::pltxt2htm::container::Array<char8_t, max_digits>{};
    char8_t* cursor = digits.data() + digits.size();
    constexpr ::std::uint32_t chunk_divisor{1'000'000'000};
    ::std::size_t significant_limbs{limbs.size()};
    while (significant_limbs != 0 && limbs[significant_limbs - 1] == 0) {
        --significant_limbs;
    }
    // Both write sites below decrement first and then assign, instead of using
    // `*--cursor = ...`: clang-tidy's misc-const-correctness does not resolve
    // pointee writes through increment/decrement expressions, so it wrongly
    // suggests a const pointee (llvm/llvm-project#215161, fixed by #215285 on
    // main for clang 24; not in the release/23.x branch). Drop this workaround
    // once the CI clang-tidy includes the fix.
    while (significant_limbs != 0) {
        auto remainder = ::pltxt2htm::format::details::divide_limbs(limbs, significant_limbs, chunk_divisor);
        if (significant_limbs == 0) {
            // Most significant chunk: no leading zeros.
            do {
                --cursor;
                *cursor = static_cast<char8_t>(u8'0' + remainder % 10);
                remainder /= 10;
            } while (remainder != 0);
        }
        else {
            for (unsigned i{}; i < 9; ++i) {
                --cursor;
                *cursor = static_cast<char8_t>(u8'0' + remainder % 10);
                remainder /= 10;
            }
        }
    }

    auto const length = static_cast<::std::size_t>(digits.data() + digits.size() - cursor);
    for (::std::size_t i{}; i < length; ++i) {
        buffer[i] = cursor[i];
    }
    return length;
}

/**
 * @brief Whether the magnitude in magnitude_bits has no fractional part.
 * @param magnitude_bits The sign-cleared bit pattern of an IEEE-754 binary64
 *        value.
 */
[[nodiscard]]
constexpr auto is_integral_magnitude(::std::uint64_t const magnitude_bits) noexcept -> bool {
    auto const exponent_bits = static_cast<int>((magnitude_bits >> 52) & 0x7ff);
    if (exponent_bits == 0) {
        return magnitude_bits == 0;
    }
    auto const unbiased_exponent = exponent_bits - 1023;
    if (unbiased_exponent >= 52) {
        // All binary64 values at or above 2^52 are integers.
        return true;
    }
    if (unbiased_exponent < 0) {
        return false;
    }
    auto const fractional_mask = (::std::uint64_t{1} << (52 - unbiased_exponent)) - 1;
    return (magnitude_bits & fractional_mask) == 0;
}

/**
 * @brief Render significand * 10^exponent in fixed notation.
 * @param significand A non-zero decimal significand without trailing zeros.
 * @param exponent The decimal exponent of the significand.
 * @param buffer Destination with room for at most `max_fixed_point_length` characters.
 * @return The number of characters written.
 */
[[nodiscard]]
constexpr auto render_fixed_point(::std::uint64_t const significand, int const exponent, char8_t* const buffer) noexcept
    -> ::std::size_t {
    // A decimal significand has at most 17 digits, so 20 bytes are always enough.
    auto digit_buffer = ::pltxt2htm::container::Array<char8_t, 20>{};
    char8_t const* const begin =
        details::write_decimal_digits_backward(significand, digit_buffer.data() + digit_buffer.size());
    auto const digit_count = static_cast<::std::size_t>(digit_buffer.data() + digit_buffer.size() - begin);

    ::std::size_t length{};
    if (exponent >= 0) {
        for (::std::size_t i{}; i < digit_count; ++i) {
            buffer[length++] = begin[i];
        }
        for (int i{}; i < exponent; ++i) {
            buffer[length++] = u8'0';
        }
        return length;
    }

    auto const fractional_digits = static_cast<::std::size_t>(-exponent);
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
 * @brief Write the fixed-point representation of any double into buffer.
 * @param buffer Destination with room for `max_fixed_point_length` characters.
 * @return The number of characters written.
 */
[[nodiscard]]
constexpr auto format_fixed_point(double const value, char8_t* const buffer) noexcept -> ::std::size_t {
    auto const bits = ::std::bit_cast<::std::uint64_t>(value);
    bool const negative = (bits >> 63) != 0;
    auto const magnitude_bits = bits & 0x7fff'ffff'ffff'ffffull;

    ::std::size_t length{};
    // A NaN has the all-ones exponent with a non-zero significand field.
    if (magnitude_bits > 0x7ff0'0000'0000'0000ull) {
        if (negative) {
            buffer[length++] = u8'-';
        }
        buffer[length++] = u8'n';
        buffer[length++] = u8'a';
        buffer[length++] = u8'n';
        return length;
    }
    if (magnitude_bits == 0x7ff0'0000'0000'0000ull) {
        if (negative) {
            buffer[length++] = u8'-';
        }
        buffer[length++] = u8'i';
        buffer[length++] = u8'n';
        buffer[length++] = u8'f';
        return length;
    }

    if (negative) {
        buffer[length++] = u8'-';
    }
    if (magnitude_bits == 0) {
        buffer[length++] = u8'0';
        return length;
    }

    auto const magnitude = ::std::bit_cast<double>(magnitude_bits);
    // For an integral value, the exact integer expansion is the shortest
    // representation with the smallest distance from the value, which is what
    // std::to_chars selects. Dragonbox's significand only has the shortest
    // number of significant digits, and expanding it with zeros would pick a
    // different (farther) value of the same length.
    if (::pltxt2htm::format::details::is_integral_magnitude(magnitude_bits)) {
        length += ::pltxt2htm::format::details::exact_integer_digits(magnitude, buffer + length);
        return length;
    }

    // Dragonbox requires a finite non-zero input, which the branches above ensure.
    // The compact cache is used deliberately: formatting is not a hot path here,
    // and 584 bytes of static data beats 9904 bytes.
    auto const decimal =
        ::pltxt2htm_dragonbox::dragonbox::to_decimal(magnitude, ::pltxt2htm_dragonbox::dragonbox::policy::sign::ignore,
                                                     ::pltxt2htm_dragonbox::dragonbox::policy::cache::compact);
    length += ::pltxt2htm::format::details::render_fixed_point(decimal.significand, decimal.exponent, buffer + length);
    return length;
}

} // namespace details

/**
 * @brief Append the fixed-point decimal representation of a value to a string.
 * @param[in,out] out Destination string.
 * @param[in] value The value to append; any double is accepted.
 */
constexpr void append_decimal(::pltxt2htm::container::U8String& out, double const value) noexcept {
    auto buffer = ::pltxt2htm::container::Array<char8_t, ::pltxt2htm::format::details::max_fixed_point_length>{};
    auto const length = ::pltxt2htm::format::details::format_fixed_point(value, buffer.data());
    out.append(buffer.data(), buffer.data() + length);
}

} // namespace pltxt2htm::format