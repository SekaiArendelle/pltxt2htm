/**
 * @file font_size.hh
 * @brief Font-size value paired with its unit.
 * @details Font sizes are the only non-integral values the AST carries, so they use this
 *          dedicated value type instead of the generic ::pltxt2htm::ValueWithUnit template,
 *          which stays in service for integer quantities. The class also owns the numeric
 *          domain the decimal parser accepts, so callers do not need a separate details header.
 */

#pragma once

#include "value_unit.hh"

namespace pltxt2htm {

/**
 * @brief A font-size value together with its unit.
 * @details The value is a non-negative double; ::pltxt2htm::Unit selects whether it is an
 *          absolute px size, a percentage, or an em-relative size.
 */
class FontSize {
public:
    double value{}; ///< Numeric font-size value.
    ::pltxt2htm::Unit unit{::pltxt2htm::Unit::px}; ///< Unit of the value.

    /**
     * @brief Smallest non-zero value the decimal parser accepts.
     * @details The parser's exact path keeps its decimal scale within 192 bits and rejects
     *          fractional values with 18 or more leading zeros, so smaller values are not
     *          representable; the bound is also far below any meaningful font size.
     */
    static constexpr double min_value{1e-18};

    /**
     * @brief Exclusive upper bound of the accepted range; values >= this are rejected.
     * @details The plweb px mapping emits a 64-bit integer and the parser keeps the integer
     *          part in a `std::uint64_t`, so `N < 2^64` is the widest platform-independent
     *          range. A decimal just below 2^64 still rounds up to 2^64, so the check happens
     *          after rounding.
     */
    static constexpr double max_value{18446744073709551616.0}; // 2^64

    /// Largest value a `<size=N>` tag can carry: the largest double below 2^64.
    static constexpr double max_size_tag_value{18446744073709549568.0}; // 2^64 - 2048

    /**
     * @brief Whether value lies inside the supported font-size range.
     * @details Zero is accepted (an inert span); any other value must lie in
     *          [min_value, max_value).
     */
    [[nodiscard]]
    static constexpr auto is_representable(double const value) noexcept -> bool {
        return value == 0 || (value >= min_value && value < max_value);
    }

    [[nodiscard]]
    constexpr auto operator==(this FontSize const& self, FontSize const& other) noexcept -> bool = default;
};

} // namespace pltxt2htm
