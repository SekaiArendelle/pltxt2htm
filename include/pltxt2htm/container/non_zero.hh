/**
 * @file non_zero.hh
 * @brief Non-zero unsigned integer refinement type for pltxt2htm.
 */

#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>

#include "../details/push_macro.hh"

namespace pltxt2htm::container {

namespace details {

template<typename T>
concept is_non_zero_unsigned_integer =
    ::std::unsigned_integral<T> && ::std::same_as<T, ::std::remove_cv_t<T>> && !::std::same_as<T, bool>;

template<typename T>
class OptionalStorage;

} // namespace details

/**
 * @brief An unsigned integer that is non-zero in every publicly observable state.
 * @tparam T An unqualified unsigned integer type other than bool.
 */
template<::pltxt2htm::container::details::is_non_zero_unsigned_integer T>
class NonZero {
    T value_storage{};

    constexpr NonZero() noexcept = default;

    constexpr explicit NonZero(T value) noexcept
        : value_storage{value} {
    }

    template<typename>
    friend class ::pltxt2htm::container::details::OptionalStorage;

public:
    using value_type = T;

    constexpr NonZero(NonZero const&) noexcept = default;

    constexpr NonZero(NonZero&&) noexcept = default;

    constexpr auto operator=(this NonZero& self, NonZero const&) noexcept -> NonZero& = default;

    constexpr auto operator=(this NonZero& self, NonZero&&) noexcept -> NonZero& = default;

    /**
     * @brief Construct a NonZero after checking that @p value is not zero.
     * @note Passing zero violates the precondition when contracts are ignored.
     */
    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    static constexpr auto from(T value) noexcept -> NonZero {
        pltxt2htm_assert(value != 0, u8"NonZero value must not be zero");
        return NonZero{value};
    }

    /**
     * @brief Return the underlying unsigned integer by value.
     */
    [[nodiscard]]
    constexpr auto get(this NonZero const& self) noexcept -> T {
        return self.value_storage;
    }

    constexpr bool operator==(this NonZero const&, NonZero const&) noexcept = default;
};

using NonZeroSize = ::pltxt2htm::container::NonZero<::std::size_t>;

} // namespace pltxt2htm::container

#include "../details/pop_macro.hh"
