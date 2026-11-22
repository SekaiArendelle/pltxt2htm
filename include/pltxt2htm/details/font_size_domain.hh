/**
 * @file font_size_domain.hh
 * @brief Supported numeric domain for font-size values.
 * @details Both the decimal parser and the double formatter use this domain so that every
 *          accepted value can be emitted again as a shortest fixed-point decimal.
 *
 *          The upper bound is exclusive: `<size=N>` is stored as a double, but the plweb px
 *          mapping emits a 64-bit integer and the parser keeps the integer part in a
 *          `std::uint64_t`, so `N < 2^64` is the widest platform-independent range.
 *          The lower bound only excludes values too small to matter for a font size; it keeps
 *          the fixed-point digit generation within 64-bit-scaled integer arithmetic.
 */

#pragma once

namespace pltxt2htm::details {

/// Smallest non-zero font-size value accepted by the parser and formatter.
inline constexpr double min_font_size_value{1e-18};

/// Exclusive upper bound of the font-size domain; values >= this are rejected.
inline constexpr double max_font_size_value{18446744073709551616.0}; // 2^64

/// Largest value a `<size=N>` tag can carry: the largest double below 2^64.
inline constexpr double max_size_tag_value{18446744073709549568.0}; // 2^64 - 2048

} // namespace pltxt2htm::details