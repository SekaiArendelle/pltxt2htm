/**
 * @file concepts.hh
 * @brief Shared concepts for pltxt2htm implementation details.
 */

#pragma once

#include <concepts>

namespace pltxt2htm::details {

template<typename T>
concept is_char_type = ::std::same_as<T, char> || ::std::same_as<T, wchar_t> || ::std::same_as<T, char8_t> ||
                       ::std::same_as<T, char16_t> || ::std::same_as<T, char32_t>;

/**
 * @brief Equality that reports its result without throwing.
 * @details The `static_cast<bool>` also requires the result-to-bool conversion
 * to be non-throwing, because standard algorithms consume the comparison
 * result through a boolean test.
 */
template<typename T>
concept nothrow_equality_comparable = requires(T const& left, T const& right) {
    { static_cast<bool>(left == right) } noexcept -> ::std::same_as<bool>;
};

/**
 * @brief Three-way comparison that completes without throwing.
 */
template<typename T>
concept nothrow_three_way_comparable = requires(T const& left, T const& right) {
    { left <=> right } noexcept;
};

template<typename T, typename U>
concept reference_constructs_from_temporary =
#if __cpp_lib_reference_from_temporary >= 202202L
    ::std::reference_constructs_from_temporary_v<T, U>;
#elif defined(__has_builtin)
    #if __has_builtin(__reference_constructs_from_temporary)
    __reference_constructs_from_temporary(T, U);
    #else
        #error "compiler too old for __reference_constructs_from_temporary"
    #endif
#else
    #error "__has_builtin is not supported; compiler too old for reference_constructs_from_temporary_v"
#endif

} // namespace pltxt2htm::details
