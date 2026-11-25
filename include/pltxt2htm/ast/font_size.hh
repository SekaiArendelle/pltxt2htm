/**
 * @file font_size.hh
 * @brief Font-size value paired with its unit.
 * @details Font sizes are the only non-integral values the AST carries, so they use this
 *          dedicated value type instead of the generic ::pltxt2htm::ValueWithUnit template,
 *          which stays in service for integer quantities. The class owns the numeric domain and
 *          can only be constructed with a value inside it, either through the checked factory
 *          `try_make` or through the contract-checked constructor.
 */

#pragma once

#include "../container/optional.hh"
#include "value_unit.hh"
#include "../details/push_macro.hh"

namespace pltxt2htm {

/**
 * @brief A font-size value together with its unit.
 * @details The value is a non-negative double; ::pltxt2htm::Unit selects whether it is an
 *          absolute px size, a percentage, or an em-relative size.
 */
class FontSize {
    double value{};
    ::pltxt2htm::Unit unit{::pltxt2htm::Unit::px};

public:
    /**
     * @brief Largest value a font size may carry; larger values are rejected.
     * @details A product bound, not a representation limit: it is far above any real font size
     *          and far below the parser's numeric range, so backend arithmetic (doubling for the
     *          plunity px mapping, halving for the plweb px mapping) stays trivial. The bound is
     *          inclusive; larger tags stay literal text.
     */
    static constexpr double max_value{1000000.0};

    /**
     * @brief Smallest value a font size may carry; zero is an inert size.
     * @details The parser still cannot represent non-zero values below about 1e-18 because of
     *          its exact-decimal implementation, but that is a parser limit, not a FontSize
     *          invariant.
     */
    static constexpr double min_value{0.0};

    /**
     * @brief Whether value lies inside the supported font-size range.
     * @details The range is [min_value, max_value]; NaN and infinities are rejected because the
     *          comparisons are false.
     */
    [[nodiscard]]
    static constexpr auto is_representable(double const value) noexcept -> bool {
        return value >= min_value && value <= max_value;
    }

    /// A zero-size px value. Zero is an inert size and lies inside the domain.
    constexpr FontSize() noexcept = default;

    /**
     * @brief Construct a font size whose value is already known to be valid.
     * @param[in] value_ The numeric font-size value.
     * @param[in] unit_ The unit of the value.
     * @pre `is_representable(value_)`; enforced by the contract system.
     * @details Use `try_make` when the value comes from untrusted input.
     */
    constexpr FontSize(double const value_, ::pltxt2htm::Unit const unit_) noexcept
        : value(value_),
          unit(unit_) {
        pltxt2htm_assert(FontSize::is_representable(value), u8"font size is out of range");
    }

    /**
     * @brief Validate a value and create a font size.
     * @param[in] value The numeric font-size value.
     * @param[in] unit The unit of the value.
     * @return The font size, or nullopt when value is outside [min_value, max_value].
     */
    [[nodiscard]]
    static constexpr auto try_make(double const value, ::pltxt2htm::Unit const unit) noexcept
        -> ::pltxt2htm::container::Optional<FontSize> {
        if (FontSize::is_representable(value) == false) {
            return ::pltxt2htm::container::nullopt;
        }
        return FontSize{value, unit};
    }

    [[nodiscard]]
    constexpr auto get_value(this FontSize const& self) noexcept -> double {
        return self.value;
    }

    [[nodiscard]]
    constexpr auto get_unit(this FontSize const& self) noexcept -> ::pltxt2htm::Unit {
        return self.unit;
    }

    [[nodiscard]]
    constexpr auto operator==(this FontSize const& self, FontSize const& other) noexcept -> bool = default;
};

} // namespace pltxt2htm

#include "../details/pop_macro.hh"