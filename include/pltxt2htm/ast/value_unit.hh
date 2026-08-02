/**
 * @file value_unit.hh
 * @brief Value+unit pair type definitions for pltxt2htm
 * @details Defines a generic numeric value paired with its unit, so the value
 *          and its unit never travel independently. Used by Physics-Lab
 *          &lt;size=N&gt; / &lt;size=N%&gt; tags and &lt;span style="font-size:..."&gt; /
 *          &lt;span style="vertical-align:..."&gt; lengths.
 */

#pragma once

#include <cstddef>

namespace pltxt2htm {

/**
 * @brief Unit of a value
 * @details Distinguishes absolute sizes from percentages in &lt;size=N&gt; /
 *          &lt;size=N%&gt; (TMP_Text rich text) and &lt;span style="font-size:..."&gt;.
 */
enum class Unit : unsigned {
    px = 0, ///< Absolute size (CSS px; TMP_Text unitless value)
    percent, ///< Percentage of the default font size (TMP_Text / CSS %)
};

/**
 * @brief A numeric value together with its unit.
 * @details Pairs the numeric value with its ::pltxt2htm::Unit so the two
 *          never travel independently. This avoids carrying an invalid/undefined
 *          unit when no value is present (the value is then simply absent).
 */
struct ValueWithUnit {
    ::std::size_t value; ///< Numeric value
    ::pltxt2htm::Unit unit; ///< Unit of the value (px or %)

    [[nodiscard]]
    constexpr auto operator==(::pltxt2htm::ValueWithUnit const& other) const noexcept -> bool = default;
};

} // namespace pltxt2htm
