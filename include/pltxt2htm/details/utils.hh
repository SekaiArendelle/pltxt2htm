/**
 * @file utils.hh
 * @brief Utility functions and helpers for pltxt2htm
 * @details Provides various utility functions for string manipulation,
 *          type conversions, and parsing operations
 */

#pragma once

#include <cstddef>
#include <ranges>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "literal_string.hh"

#include "push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Get character at specific index from u8string_view with bounds checking
 * @tparam ndebug Debug mode flag - controls assertion behavior
 * @param[in] pltext The string view to index into
 * @param[in] i The index to access
 * @return The character at the specified index
 * @retval char8_t The UTF-8 character at the specified index
 * @note This function performs bounds checking in debug mode for safety
 */
template<bool ndebug>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto u8string_view_index(::fast_io::u8string_view pltext, ::std::size_t i) noexcept {
    ::std::size_t const pltext_size{pltext.size()};
    pltxt2htm_assert(i < pltext_size, u8"Index of u8string_view out of bound");

    return pltext.index_unchecked(i);
}

/**
 * @brief Get a substring view from u8string_view
 * @tparam ndebug Debug mode flag - controls bounds checking
 * @param[in] pltext The original string view
 * @param[in] i Starting index of the substring
 * @param[in] count Number of characters in the substring (npos for remainder)
 * @return A new string view representing the substring
 * @retval fast_io::u8string_view New string view representing the substring
 * @note In debug mode, performs bounds checking; in release mode, uses unchecked access
 */
template<bool ndebug>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto u8string_view_subview(::fast_io::u8string_view pltext, ::std::size_t i,
                                     ::std::size_t count = ::fast_io::containers::npos) noexcept
    -> ::fast_io::u8string_view {
    if constexpr (ndebug) {
        return pltext.subview_unchecked(i, count);
    }
    else {
        return pltext.subview(i, count);
    }
}

template<bool ndebug, typename T>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto vector_front(::fast_io::vector<T> const& vec) noexcept -> T const& {
    bool const vec_is_not_empty{!vec.empty()};
    pltxt2htm_assert(vec_is_not_empty, u8"Indexing front but vector is empty");

    return vec.front_unchecked();
}

/**
 * @return index of ::fast_io::u8string_view
 */
template<bool ndebug, typename T>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto vector_index(::fast_io::vector<T> const& vec, ::std::size_t i) noexcept -> T const& {
    bool const is_not_out_of_bound{i < vec.size()};
    pltxt2htm_assert(is_not_out_of_bound, u8"Index of vector out of bound");

    return vec.index_unchecked(i);
}

/**
 * @brief Check if a string is a case-insensitive prefix match
 * @details This function performs compile-time prefix matching that is case-insensitive.
 *          It generates efficient if-expressions at compile time for optimal runtime performance.
 * @tparam ndebug Debug mode flag - controls bounds checking
 * @tparam prefix_str The prefix to match (must be lowercase compile-time string)
 * @param[in] str The string to check against
 * @return true if str starts with prefix_str (case-insensitive), false otherwise
 * @retval bool Boolean indicating whether the prefix match succeeded
 * @note prefix_str must contain only lowercase characters due to compile-time constraints
 * @warning This is a compile-time function that generates optimized matching code
 */
template<bool ndebug, ::pltxt2htm::details::LiteralString prefix_str>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr bool is_prefix_match(::fast_io::u8string_view str) noexcept {
    // Ensure prefix_str does not contain uppercase characters.
    constexpr bool has_uppercase = []<::std::size_t... Is>(::std::index_sequence<Is...>) static constexpr noexcept {
        return (((prefix_str[Is] >= 'A') && (prefix_str[Is] <= 'Z')) || ...);
    }(::std::make_index_sequence<prefix_str.size()>{});
    static_assert(!has_uppercase, "prefix_str must not contain uppercase letters");

    // Check whether the index is out of bound.
    if (prefix_str.size() > str.size()) [[unlikely]] {
        return false;
    }

    // Return whether the `prefix_str` is a prefix of the str.
    // libc++ does not implement std::ranges::enumerate yet
#if __cpp_expansion_statements >= 202506L
    template for (constexpr ::std::size_t I : ::std::ranges::views::iota(::std::size_t{}, prefix_str.size())) {
#else
    return [str]<::std::size_t... Is>(::std::index_sequence<Is...>) {
        return ([str]<::std::size_t I>() {
#endif
        constexpr auto expect = prefix_str[I];
        if constexpr ('a' <= expect && expect <= 'z') {
            // ASCII between lowercase and uppercase is 32 (e.g. 'a' - 'A' == 32)
            constexpr ::std::uint8_t diff{32};
            // (expect != str[I] && expect != str[I] + diff) <=> (expect != (str[I] | diff))
            if (expect != (::pltxt2htm::details::u8string_view_index<ndebug>(str, I) | diff)) {
                return false;
            }
            else {
                return true;
            }
        }
        else {
            if (expect != ::pltxt2htm::details::u8string_view_index<ndebug>(str, I)) {
                return false;
            }
            else {
                return true;
            }
        }
#if __cpp_expansion_statements >= 202506L
    }
#else
        }.template operator()<Is>() &&
                ...);
    }(::std::make_index_sequence<prefix_str.size()>{});
#endif
}

/**
 * @brief Convert a std::size_t to a UTF-8 string
 * @param[in] num The number to convert
 * @return A UTF-8 string representation of the number
 * @retval fast_io::u8string UTF-8 string containing the number representation
 * @note This function handles the special case of 0 and builds the string
 *       by extracting digits from least significant to most significant,
 *       then reversing the result
 */
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr ::fast_io::u8string size_t2str(::std::size_t num) noexcept {
    if (num == 0) {
        return ::fast_io::u8string{u8"0"};
    }

    ::fast_io::u8string result{};

    while (num > 0) {
        char8_t digit = (num % 10) + u8'0';
        result.push_back(digit);
        num /= 10;
    }

    ::std::reverse(result.begin(), result.end());

    return result;
}

/**
 * @brief Convert a UTF-8 string to std::size_t
 * @param[in] str The string to convert (must contain only digits)
 * @return Optional containing the converted number, or nullopt if conversion fails
 * @retval exception::optional<std::size_t> Optional containing the converted number
 * @note This function only accepts strings containing ASCII digits (0-9).
 *       Empty strings or strings with non-digit characters return nullopt
 */
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr ::exception::optional<std::size_t> u8str2size_t(::fast_io::u8string_view str) noexcept {
    if (str.empty()) {
        return ::exception::nullopt_t{};
    }

    ::std::size_t result{};
    for (auto&& c : str) {
        if (c < u8'0' || c > u8'9') {
            return ::exception::nullopt_t{};
        }

        result = result * 10 + (c - '0');
    }

    return result;
}

} // namespace pltxt2htm::details

#include "pop_macro.hh"
