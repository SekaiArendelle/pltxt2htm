/**
 * @file font_size_value.hh
 * @brief Font-size value type definitions for pltxt2htm
 * @details Defines the font-size value and its unit, used by Physics-Lab
 *          &lt;size=N&gt; / &lt;size=N%&gt; tags and &lt;span style="font-size:..."&gt;
 *          so the numeric value and its unit never travel independently.
 */

#pragma once

#include <cstddef>

namespace pltxt2htm {

/**
 * @brief Unit of a font-size value
 * @details Distinguishes absolute sizes from percentages in &lt;size=N&gt; /
 *          &lt;size=N%&gt; (TMP_Text rich text) and &lt;span style="font-size:..."&gt;.
 */
enum class SizeUnit : unsigned {
    px = 0, ///< Absolute size (CSS px; TMP_Text unitless value)
    percent, ///< Percentage of the default font size (TMP_Text / CSS %)
};

/**
 * @brief A font-size value together with its unit.
 * @details Pairs the numeric value with its ::pltxt2htm::SizeUnit so the two
 *          never travel independently. This avoids carrying an invalid/undefined
 *          unit when no font-size is present (the value is then simply absent).
 */
struct FontSizeValue {
    ::std::size_t value; ///< Numeric font-size value
    ::pltxt2htm::SizeUnit unit; ///< Unit of the value (px or %)

    [[nodiscard]]
    constexpr auto operator==(::pltxt2htm::FontSizeValue const& other) const noexcept -> bool = default;
};

} // namespace pltxt2htm
