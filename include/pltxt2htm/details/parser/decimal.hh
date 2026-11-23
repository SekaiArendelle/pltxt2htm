/**
 * @file decimal.hh
 * @brief Parsing of non-negative ASCII decimal values that may contain a fractional part.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

#include "../../container/optional.hh"
#include "../../container/string_view.hh"
#include "../ascii.hh"
#include "../font_size_domain.hh"
#include "../../numeric/uint192.hh"

namespace pltxt2htm::details {

/**
 * @brief Result of parsing a non-negative ASCII decimal value that may be fractional.
 */
struct TryParseDoubleDecimalValueResult {
    ::std::size_t end;
    double value;
};

namespace parser_details {

/// Exact powers of ten that fit a ::std::uint64_t (10^0 through 10^19).
inline constexpr ::std::uint64_t decimal_powers_of_ten[]{
    1ull,
    10ull,
    100ull,
    1000ull,
    10000ull,
    100000ull,
    1000000ull,
    10000000ull,
    100000000ull,
    1000000000ull,
    10000000000ull,
    100000000000ull,
    1000000000000ull,
    10000000000000ull,
    100000000000000ull,
    1000000000000000ull,
    10000000000000000ull,
    100000000000000000ull,
    1000000000000000000ull,
    10000000000000000000ull,
};

/// The number of exact powers of ten in decimal_powers_of_ten.
inline constexpr ::std::size_t decimal_powers_of_ten_size{sizeof(decimal_powers_of_ten) / sizeof(::std::uint64_t)};

/// 2^53, the largest integer magnitude a double represents exactly.
inline constexpr ::std::uint64_t exact_integer_limit{::std::uint64_t{1} << 53};

/// Number of significant fraction digits retained exactly (fits 10^19 in a uint64).
inline constexpr ::std::size_t max_significant_fraction_digits{decimal_powers_of_ten_size - 1};

/// Largest decimal scale whose exact numerator (integer part * 10^scale + fraction) fits 192 bits.
inline constexpr ::std::size_t max_exact_scale_digits{38};

} // namespace parser_details

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
            if (significant_fraction_digits < parser_details::max_significant_fraction_digits) {
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
    if (significant_fraction_digits == 0) {
        if (integer_part < parser_details::exact_integer_limit) {
            value = static_cast<double>(integer_part);
        }
        else {
            // Round the large integer exactly instead of relying on the implementation-defined
            // direction of the integer-to-double conversion.
            value = ::pltxt2htm::numeric::decimal_ratio_to_double(::pltxt2htm::numeric::UInt192{integer_part}, 0);
        }
    }
    else {
        bool exact{};
        if (scale_digits < parser_details::decimal_powers_of_ten_size) {
            auto const scale = parser_details::decimal_powers_of_ten[scale_digits];
            auto const scale_double = static_cast<double>(scale);
            if (integer_part <= (parser_details::exact_integer_limit - 1) / scale &&
                integer_part * scale + fraction < parser_details::exact_integer_limit) {
                // The whole significand is exactly representable, so a single division produces
                // the correctly rounded value.
                value = static_cast<double>(integer_part * scale + fraction) / scale_double;
                exact = true;
            }
        }
        if (exact == false) {
            if (scale_digits <= parser_details::max_exact_scale_digits) {
                // Exactly rounded: build the full rational numerator and let the shared 192-bit
                // helper handle the rounding, so long fractions never drift by an ulp.
                auto const numerator =
                    (::pltxt2htm::numeric::pow(::pltxt2htm::numeric::UInt192{10}, scale_digits) * integer_part) +
                    ::pltxt2htm::numeric::UInt192{fraction};
                value =
                    ::pltxt2htm::numeric::decimal_ratio_to_double(numerator, scale_digits, has_nonzero_fraction_tail);
            }
            else {
                // The fraction digits beyond the retained significand are below 10^-20, far below
                // the rounding distance of any double. They are still strictly positive, which
                // matters when the integer part sits exactly on a midpoint: the sticky bit makes
                // the tie round up instead of to even.
                value =
                    ::pltxt2htm::numeric::decimal_ratio_to_double(::pltxt2htm::numeric::UInt192{integer_part}, 0, true);
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