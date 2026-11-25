/**
 * @file decimal.hh
 * @brief Parsing of non-negative ASCII decimal values that may contain a fractional part.
 */

#pragma once

#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>

#include "../../container/array.hh"
#include "../../container/optional.hh"
#include "../../container/string_view.hh"
#include "../ascii.hh"
#include "../font_size_domain.hh"
#include "../../numeric/uint192.hh"
#include "../push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Round a positive rational to the nearest double with ties to even.
 * @param numerator The significand.
 * @param scale_digits The number of decimal places: the value is numerator / 10^scale_digits.
 * @param extra_sticky Set when the true value is slightly above numerator / 10^scale_digits
 *        (for example when a fraction tail was dropped); it forces the sticky bit for ties.
 * @pre `scale_digits <= 38`; the parser guarantees this, and the shifted operands below stay
 *      below 192 bits under that bound.
 * @details The quotient is computed with 57 or 58 significant bits, so the round bit and the
 *          sticky bits are exact.
 */
[[nodiscard]]
constexpr auto decimal_ratio_to_double(::pltxt2htm::numeric::UInt192 const numerator, ::std::size_t const scale_digits,
                                       bool const extra_sticky = false) noexcept -> double {
    pltxt2htm_assert(scale_digits <= 38, u8"scale_digits must not exceed 38");
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

/**
 * @brief Result of parsing a non-negative ASCII decimal value that may be fractional.
 */
struct TryParseDoubleDecimalValueResult {
    ::std::size_t end;
    double value;
};

/**
 * @brief Parse a non-negative ASCII decimal value that may contain a fractional part.
 * @details Accepts `[0-9]+` with an optional `.` followed by one or more `[0-9]`; a bare `.`
 *          without digits is rejected. Stops at the first character that is neither a digit nor
 *          the fraction dot; the returned `end` is the run length relative to `str`.
 *
 *          The integer part is accumulated exactly in `::std::uint64_t`, and the first 19
 *          *significant* fraction digits are accumulated exactly as well (leading zeros only
 *          advance the decimal scale). The value is then formed with a single division and at
 *          most one addition instead of accumulating each digit in a double, which would drift
 *          by an ulp on long runs. When the whole significand fits 2^53, both division operands
 *          are exact and the result is the correctly rounded value; longer significands are
 *          rounded once after the exact integer accumulation.
 *
 *          Values outside the font-size domain (see font_size_domain.hh) are rejected, and the
 *          integer part must fit a `::std::uint64_t`.
 */
[[nodiscard]]
constexpr auto try_parse_double_decimal_value(::pltxt2htm::container::U8StringView str) noexcept
    -> ::pltxt2htm::container::Optional<TryParseDoubleDecimalValueResult> {
    /// Exact powers of ten that fit a ::std::uint64_t (10^0 through 10^19).
    constexpr auto decimal_powers_of_ten = ::pltxt2htm::container::Array{
        ::std::uint64_t{1ull},
        ::std::uint64_t{10ull},
        ::std::uint64_t{100ull},
        ::std::uint64_t{1000ull},
        ::std::uint64_t{10000ull},
        ::std::uint64_t{100000ull},
        ::std::uint64_t{1000000ull},
        ::std::uint64_t{10000000ull},
        ::std::uint64_t{100000000ull},
        ::std::uint64_t{1000000000ull},
        ::std::uint64_t{10000000000ull},
        ::std::uint64_t{100000000000ull},
        ::std::uint64_t{1000000000000ull},
        ::std::uint64_t{10000000000000ull},
        ::std::uint64_t{100000000000000ull},
        ::std::uint64_t{1000000000000000ull},
        ::std::uint64_t{10000000000000000ull},
        ::std::uint64_t{100000000000000000ull},
        ::std::uint64_t{1000000000000000000ull},
        ::std::uint64_t{10000000000000000000ull},
    };

    ::std::size_t const str_size{str.size()};
    ::std::uint64_t integer_part{};
    auto pos = ::std::size_t{0};
    for (; pos < str_size; ++pos) {
        auto const chr = str[pos];
        if (::pltxt2htm::details::is_ascii_digit(chr) == false) {
            break;
        }
        auto const digit = static_cast<::std::uint64_t>(chr - u8'0');
        if (integer_part > (::std::numeric_limits<::std::uint64_t>::max() - digit) / 10) {
            return ::pltxt2htm::container::nullopt;
        }
        integer_part = integer_part * 10 + digit;
    }
    if (pos == 0) {
        return ::pltxt2htm::container::nullopt;
    }

    ::std::uint64_t fraction{};
    auto significant_fraction_digits = ::std::size_t{0};
    auto leading_zero_digits = ::std::size_t{0};
    bool seen_significant_digit{};
    bool has_nonzero_fraction_tail{};
    if (pos < str_size && str[pos] == u8'.') {
        auto frac_pos = pos + 1;
        for (; frac_pos < str_size; ++frac_pos) {
            auto const chr = str[frac_pos];
            if (::pltxt2htm::details::is_ascii_digit(chr) == false) {
                break;
            }
            auto const digit = static_cast<::std::uint64_t>(chr - u8'0');
            if (seen_significant_digit == false) {
                if (digit == 0) {
                    ++leading_zero_digits;
                    continue;
                }
                // With no integer part, 18 leading zeros already put the first significant
                // digit below the domain minimum (1e-18).
                if (integer_part == 0 && leading_zero_digits >= 18) {
                    return ::pltxt2htm::container::nullopt;
                }
                seen_significant_digit = true;
            }
            if (significant_fraction_digits < decimal_powers_of_ten.size() - 1) {
                fraction = fraction * 10 + digit;
                ++significant_fraction_digits;
            }
            else if (digit != 0) {
                has_nonzero_fraction_tail = true;
            }
        }
        if (frac_pos == pos + 1) {
            return ::pltxt2htm::container::nullopt;
        }
        pos = frac_pos;
    }

    // Number of fraction digits covered by the retained significand; leading zeros are part of
    // the scale but not of the accumulated integer.
    auto const scale_digits = leading_zero_digits + significant_fraction_digits;

    double value{};
    /// 2^53, the largest integer magnitude a double represents exactly.
    constexpr ::std::uint64_t exact_integer_limit{::std::uint64_t{1} << 53};
    if (significant_fraction_digits == 0) {
        if (integer_part < exact_integer_limit) {
            value = static_cast<double>(integer_part);
        }
        else {
            // Round the large integer exactly instead of relying on the implementation-defined
            // direction of the integer-to-double conversion.
            value = ::pltxt2htm::details::decimal_ratio_to_double(::pltxt2htm::numeric::UInt192{integer_part}, 0);
        }
    }
    else {
        bool exact{};
        if (scale_digits < decimal_powers_of_ten.size()) {
            auto const scale = decimal_powers_of_ten[scale_digits];
            auto const scale_double = static_cast<double>(scale);
            if (integer_part <= (exact_integer_limit - 1) / scale &&
                integer_part * scale + fraction < exact_integer_limit) {
                // The whole significand is exactly representable, so a single division produces
                // the correctly rounded value.
                value = static_cast<double>(integer_part * scale + fraction) / scale_double;
                exact = true;
            }
        }
        if (exact == false) {
            /// Largest decimal scale whose exact numerator (integer part * 10^scale + fraction) fits 192 bits.
            constexpr ::std::size_t max_exact_scale_digits{38};
            if (scale_digits <= max_exact_scale_digits) {
                // Exactly rounded: build the full rational numerator and let the shared 192-bit
                // helper handle the rounding, so long fractions never drift by an ulp.
                auto const numerator =
                    (::pltxt2htm::numeric::pow(::pltxt2htm::numeric::UInt192{10}, scale_digits) * integer_part) +
                    ::pltxt2htm::numeric::UInt192{fraction};
                value =
                    ::pltxt2htm::details::decimal_ratio_to_double(numerator, scale_digits, has_nonzero_fraction_tail);
            }
            else {
                // The fraction digits beyond the retained significand are below 10^-20, far below
                // the rounding distance of any double. They are still strictly positive, which
                // matters when the integer part sits exactly on a midpoint: the sticky bit makes
                // the tie round up instead of to even.
                value =
                    ::pltxt2htm::details::decimal_ratio_to_double(::pltxt2htm::numeric::UInt192{integer_part}, 0, true);
            }
        }
    }

    // Keep every accepted value inside the domain the formatter supports. Zero is allowed: a
    // zero font size is meaningful as an inert span, and it round-trips exactly.
    if (value == 0) {
        // A non-zero digit that did not survive the union of the retained digits means the true
        // value is a tiny non-zero number below the domain, not zero.
        if (fraction != 0 || has_nonzero_fraction_tail) {
            return ::pltxt2htm::container::nullopt;
        }
    }
    else if (value < ::pltxt2htm::details::min_font_size_value) {
        return ::pltxt2htm::container::nullopt;
    }
    if (value >= ::pltxt2htm::details::max_font_size_value) {
        return ::pltxt2htm::container::nullopt;
    }
    return TryParseDoubleDecimalValueResult{.end = pos, .value = value};
}

} // namespace pltxt2htm::details

#include "../pop_macro.hh"
