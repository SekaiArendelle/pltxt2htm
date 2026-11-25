/**
 * @file utils.hh
 * @brief Utility functions and helpers for pltxt2htm
 * @details Provides various utility functions for string manipulation,
 *          type conversions, and parsing operations
 */

#pragma once

#include <limits>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <type_traits>
#include "../container/string.hh"
#include "../container/optional.hh"
#include "../container/string_view.hh"
#include "contracts.hh"
#include "literal_string.hh"
#include "ascii.hh"
#include "../format/floating.hh"
#include "../format/integer.hh"
#include "parser/decimal.hh"

#include "push_macro.hh"

// windows.h defines min and max as function-like macros, which would expand
// inside std::numeric_limits<...>::max(). Keep them undefined in this header.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace pltxt2htm::details {

/**
 * @brief Check if a string is a case-insensitive prefix match
 * @details This function performs compile-time prefix matching that is case-insensitive.
 *          It generates efficient if-expressions at compile time for optimal runtime performance.
 * Contract mode follows PLTXT2HTM_CONTRACTS.
 * @tparam prefix_str The prefix to match (must be lowercase compile-time string)
 * @param[in] str The string to check against
 * @return true if str starts with prefix_str (case-insensitive), false otherwise
 * @retval bool Boolean indicating whether the prefix match succeeded
 * @note prefix_str must contain only lowercase characters due to compile-time constraints
 * @warning This is a compile-time function that generates optimized matching code
 */
template<U8LiteralString prefix_str>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr bool is_prefix_match(::pltxt2htm::container::U8StringView str) noexcept {
    // Ensure prefix_str does not contain uppercase characters.
    constexpr bool has_uppercase = []<::std::size_t... Is>(::std::index_sequence<Is...>) static constexpr noexcept {
        return (((prefix_str[Is] >= 'A') && (prefix_str[Is] <= 'Z')) || ...);
    }(::std::make_index_sequence<prefix_str.size()>{});
    static_assert(!has_uppercase, "prefix_str must not contain uppercase letters");

    // Check whether the index is out of bound.
    if (prefix_str.size() > str.size()) {
        return false;
    }

#if __cpp_expansion_statements >= 202506L
    // TODO use `template for (constexpr auto [I, expect] : prefix_str | ::std::views::enumerate)` instead
    template for (constexpr ::std::size_t I : ::std::ranges::views::iota(::std::size_t{}, prefix_str.size())) {
        constexpr auto expect = prefix_str[I];
        if constexpr ('a' <= expect && expect <= 'z') {
            // ASCII between lowercase and uppercase is 32 (e.g. 'a' - 'A' == 32)
            constexpr ::std::uint8_t diff{32};
            // (expect != str[I] && expect != str[I] + diff) <=> (expect != (str[I] | diff))
            if (expect != (str[I] | diff)) {
                return false;
            }
        }
        else {
            if (expect != str[I]) {
                return false;
            }
        }
    }
    return true;
#else
    return [str]<::std::size_t... Is>(::std::index_sequence<Is...>) {
        return ([str]<::std::size_t I>() {
            constexpr auto expect = prefix_str[I];
            if constexpr ('a' <= expect && expect <= 'z') {
                // ASCII between lowercase and uppercase is 32 (e.g. 'a' - 'A' == 32)
                constexpr ::std::uint8_t diff{32};
                // (expect != str[I] && expect != str[I] + diff) <=> (expect != (str[I] | diff))
                return expect == (str[I] | diff);
            }
            else {
                return expect == str[I];
            }
        }.template operator()<Is>() &&
                ...);
    }(::std::make_index_sequence<prefix_str.size()>{});
#endif
}

/**
 * @brief Result of parsing an ASCII decimal value.
 */
struct TryParseSizeTDecimalValueResult {
    ::std::size_t end;
    ::std::size_t value;
};

/**
 * @brief Parse an ASCII decimal value.
 * @details Stops at the first non-digit and rejects empty or overflowing values.
 *          Callers that need to skip a prefix should pass `str` pre-subviewed;
 *          the returned `end` is the run length relative to `str`.
 */
[[nodiscard]]
constexpr auto try_parse_size_t_decimal_value(::pltxt2htm::container::U8StringView str) noexcept
    -> ::pltxt2htm::container::Optional<TryParseSizeTDecimalValueResult> {
    ::std::size_t const str_size{str.size()};
    ::std::size_t parsed_value{};
    auto pos = ::std::size_t{0};
    for (; pos < str_size; ++pos) {
        auto const chr = str[pos];
        if (::pltxt2htm::details::is_ascii_digit(chr) == false) {
            break;
        }
        auto const digit = static_cast<::std::size_t>(chr - u8'0');
        if (parsed_value > (::std::numeric_limits<::std::size_t>::max() - digit) / 10) {
            return ::pltxt2htm::container::nullopt;
        }
        parsed_value = parsed_value * 10 + digit;
    }
    if (pos == 0) {
        return ::pltxt2htm::container::nullopt;
    }
    return TryParseSizeTDecimalValueResult{.end = pos, .value = parsed_value};
}

/**
 * @brief Result of parsing a signed ASCII decimal value.
 */
struct TryParsePtrdiffTDecimalValueResult {
    ::std::size_t end;
    ::std::ptrdiff_t value;
};

/**
 * @brief Parse a signed ASCII decimal value.
 * @details Accepts an optional leading U+002D '-' sign (negative values), stops at
 *          the first non-digit, and returns nullopt for empty or overflowing values.
 *          Callers that need to skip a prefix should pass `str` pre-subviewed.
 */
[[nodiscard]]
constexpr auto try_parse_ptrdiff_t_decimal_value(::pltxt2htm::container::U8StringView str) noexcept
    -> ::pltxt2htm::container::Optional<TryParsePtrdiffTDecimalValueResult> {
    using unsigned_type = ::std::make_unsigned_t<::std::ptrdiff_t>;

    ::std::size_t const str_size{str.size()};
    auto pos = ::std::size_t{0};
    bool const negative = pos < str_size && str[pos] == u8'-';
    if (negative) {
        ++pos;
    }

    unsigned_type parsed_value{};
    auto digit_pos = pos;
    for (; digit_pos < str_size; ++digit_pos) {
        auto const chr = str[digit_pos];
        if (::pltxt2htm::details::is_ascii_digit(chr) == false) {
            break;
        }
        auto const digit = static_cast<unsigned_type>(chr - u8'0');
        if (parsed_value > (::std::numeric_limits<unsigned_type>::max() - digit) / 10) {
            return ::pltxt2htm::container::nullopt;
        }
        parsed_value = parsed_value * 10 + digit;
    }
    if (digit_pos == pos) {
        return ::pltxt2htm::container::nullopt;
    }

    auto const min_magnitude = static_cast<unsigned_type>(::std::numeric_limits<::std::ptrdiff_t>::max()) + 1;
    if (parsed_value >
        (negative ? min_magnitude : static_cast<unsigned_type>(::std::numeric_limits<::std::ptrdiff_t>::max()))) {
        return ::pltxt2htm::container::nullopt;
    }

    ::std::ptrdiff_t value{};
    if (negative) {
        value = parsed_value == min_magnitude ? ::std::numeric_limits<::std::ptrdiff_t>::min()
                                              : -static_cast<::std::ptrdiff_t>(parsed_value);
    }
    else {
        value = static_cast<::std::ptrdiff_t>(parsed_value);
    }

    return TryParsePtrdiffTDecimalValueResult{.end = digit_pos, .value = value};
}

/**
 * @brief Round a non-negative double up to the nearest integer.
 * @pre value is in [0, 2^64), so the conversion to `::std::uint64_t` is defined.
 */
[[nodiscard]]
constexpr auto double_to_uint64_ceil(double const value) noexcept -> ::std::uint64_t {
    auto const truncated = static_cast<::std::uint64_t>(value);
    return static_cast<double>(truncated) < value ? truncated + 1 : truncated;
}

} // namespace pltxt2htm::details

#include "pop_macro.hh"

#pragma pop_macro("max")
#pragma pop_macro("min")
