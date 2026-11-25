/**
 * @file font_size.hh
 * @brief Font-size value paired with its unit.
 * @details Font sizes are the only non-integral values the AST carries, so they use this
 *          dedicated value type instead of the generic ::pltxt2htm::ValueWithUnit template,
 *          which stays in service for integer quantities. The type also surfaces the numeric
 *          domain the decimal parser accepts. The authoritative limits remain in
 *          details/font_size_domain.hh; the static members here only forward them so callers do
 *          not need to reach into the details namespace.
 */

#pragma once

#include "../details/font_size_domain.hh"
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

    /// Smallest non-zero value the decimal parser accepts.
    static constexpr double min_value{::pltxt2htm::details::min_font_size_value};

    /// Exclusive upper bound of the supported value range; values >= this are rejected.
    static constexpr double max_value{::pltxt2htm::details::max_font_size_value};

    /// Largest value a `<size=N>` tag can carry: the largest double below 2^64.
    static constexpr double max_size_tag_value{::pltxt2htm::details::max_size_tag_value};

    /**
     * @brief Whether value lies inside the supported font-size range.
     * @details Zero is accepted (an inert span); any other value must lie in
     *          [min_value, max_value).
     */
    [[nodiscard]]
    static constexpr auto is_representable(double const value) noexcept -> bool {
        return ::pltxt2htm::details::is_representable(value);
    }

    [[nodiscard]]
    constexpr auto operator==(this FontSize const& self, FontSize const& other) noexcept -> bool = default;
};

} // namespace pltxt2htm
