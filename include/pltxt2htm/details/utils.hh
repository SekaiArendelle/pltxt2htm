/**
 * @file utils.hh
 * @brief Utility functions and helpers for pltxt2htm
 * @details Provides various utility functions for string manipulation,
 *          type conversions, and parsing operations
 */

#pragma once

#include <limits>
#include <cstddef>
#include <ranges>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>
#include "../contracts.hh"
#include "literal_string.hh"

#include "push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief Check if a character is an ASCII letter.
 * @param chr The character to test.
 * @return true if chr is in [a-zA-Z].
 */
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr bool is_ascii_alpha(char8_t const chr) noexcept {
    return (u8'a' <= chr && chr <= u8'z') || (u8'A' <= chr && chr <= u8'Z');
}

/**
 * @brief Check if a character is an ASCII digit.
 * @param chr The character to test.
 * @return true if chr is in [0-9].
 */
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr bool is_ascii_digit(char8_t const chr) noexcept {
    return u8'0' <= chr && chr <= u8'9';
}

/**
 * @brief Check if a character is an ASCII hexadecimal digit.
 * @param chr The character to test.
 * @return true if chr is in [0-9a-fA-F].
 */
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr bool is_ascii_hexdigit(char8_t const chr) noexcept {
    return ::pltxt2htm::details::is_ascii_digit(chr) || (u8'a' <= chr && chr <= u8'f') ||
           (u8'A' <= chr && chr <= u8'F');
}

/**
 * @brief Check if a character is an ASCII lowercase letter or digit.
 */
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr bool is_ascii_lowercase_alphanumeric(char8_t const chr) noexcept {
    return (u8'a' <= chr && chr <= u8'z') || ::pltxt2htm::details::is_ascii_digit(chr);
}

/**
 * @brief Check if a character is printable ASCII excluding space.
 */
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr bool is_ascii_graphic(char8_t const chr) noexcept {
    return u8'!' <= chr && chr <= u8'~';
}

/**
 * @brief Check if a character is allowed in a URL-valued tag attribute.
 * @details Printable ASCII plus any non-ASCII byte (e.g. UTF-8 CJK); non-ASCII bytes are
 *          percent-encoded later by make_try_parse_url_result.
 */
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr bool is_url_value_char(char8_t const chr) noexcept {
    return ::pltxt2htm::details::is_ascii_graphic(chr) || chr >= u8'\x80';
}

/**
 * @brief Get character at specific index from u8string_view with bounds checking
 * @tparam ndebug Contract checking mode controlling assertion behavior.
 * @param[in] pltext The string view to index into
 * @param[in] i The index to access
 * @return The character at the specified index
 * @retval char8_t The UTF-8 character at the specified index
 * @note This function performs bounds checking in debug mode for safety
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto u8string_view_index(::fast_io::u8string_view pltext, ::std::size_t i) noexcept -> char8_t {
    ::std::size_t const pltext_size{pltext.size()};
    pltxt2htm_assert(i < pltext_size, u8"Index of u8string_view out of bound");

    return pltext.index_unchecked(i);
}

/**
 * @brief Get a substring view from u8string_view
 * @tparam ndebug Contract checking mode controlling bounds-check behavior.
 * @param[in] pltext The original string view
 * @param[in] i Starting index of the substring
 * @param[in] count Number of characters in the substring (npos for remainder)
 * @return A new string view representing the substring
 * @retval fast_io::u8string_view New string view representing the substring
 * @note In debug mode, performs bounds checking; in release mode, uses unchecked access
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto u8string_view_subview(::fast_io::u8string_view pltext, ::std::size_t i,
                                     ::std::size_t count = ::fast_io::containers::npos) noexcept
    -> ::fast_io::u8string_view {
    if constexpr (ndebug == ::pltxt2htm::Contracts::ignore) {
        return pltext.subview_unchecked(i, count);
    }
    else {
        return pltext.subview(i, count);
    }
}

/**
 * @brief Variable-template helper for the is_fast_io_vector concept.
 */
template<typename T>
constexpr bool is_fast_io_vector_impl = false;

template<typename T, typename Alloc>
constexpr bool is_fast_io_vector_impl<::fast_io::vector<T, Alloc>> = true;

/**
 * @brief Concept matching ::fast_io::vector<T, Alloc> (any T, any Alloc).
 */
template<typename T>
concept is_fast_io_vector = is_fast_io_vector_impl<::std::remove_cvref_t<T>>;

/**
 * @brief Access the first element of a mutable vector (checked).
 * @tparam ndebug Contract checking mode.
 * @tparam T Element type.
 * @param vec The vector.
 * @return Reference to the first element.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto vector_front(is_fast_io_vector auto&& vec) noexcept -> decltype(auto) {
    static_assert(::std::is_lvalue_reference_v<decltype(vec)>, "vector_front requires an lvalue reference");
    bool const vec_is_not_empty{!vec.empty()};
    pltxt2htm_assert(vec_is_not_empty, u8"Indexing front but vector is empty");

    return ::std::forward_like<decltype(vec)>(vec.front_unchecked());
}

/**
 * @brief Index into a fast_io::vector with bounds checking.
 * @tparam ndebug Contract checking mode.
 * @param vec The vector.
 * @param i Index.
 * @return Reference to the element at index @p i.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto vector_index(is_fast_io_vector auto&& vec, ::std::size_t i) noexcept -> decltype(auto) {
    static_assert(::std::is_lvalue_reference_v<decltype(vec)>, "vector_index requires an lvalue reference");
    bool const is_not_out_of_bound{i < vec.size()};
    pltxt2htm_assert(is_not_out_of_bound, u8"Index of vector out of bound");

    return ::std::forward_like<decltype(vec)>(vec.index_unchecked(i));
}

/**
 * @brief Access the top element of a mutable stack (checked).
 * @tparam ndebug Contract checking mode.
 * @tparam T Element type.
 * @param stack The stack.
 * @return Reference to the top element.
 */
template<::pltxt2htm::Contracts ndebug, typename T>
[[nodiscard]]
constexpr auto& stack_top(::fast_io::containers::stack<T>& stack) noexcept {
    bool const is_not_empty{stack.empty() == false};
    pltxt2htm_assert(is_not_empty, u8"Accessing top but stack is empty");

    return stack.top_unchecked();
}

/**
 * @brief Access the top element of a const stack (checked).
 * @tparam ndebug Contract checking mode.
 * @tparam T Element type.
 * @param stack The stack.
 * @return Const reference to the top element.
 */
template<::pltxt2htm::Contracts ndebug, typename T>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto const& stack_top(::fast_io::containers::stack<T> const& stack) noexcept {
    bool const is_not_empty{stack.empty() == false};
    pltxt2htm_assert(is_not_empty, u8"Accessing top but stack is empty");

    return stack.top_unchecked();
}

/**
 * @brief Check if a string is a case-insensitive prefix match
 * @details This function performs compile-time prefix matching that is case-insensitive.
 *          It generates efficient if-expressions at compile time for optimal runtime performance.
 * @tparam ndebug Contract checking mode controlling bounds-check behavior.
 * @tparam prefix_str The prefix to match (must be lowercase compile-time string)
 * @param[in] str The string to check against
 * @return true if str starts with prefix_str (case-insensitive), false otherwise
 * @retval bool Boolean indicating whether the prefix match succeeded
 * @note prefix_str must contain only lowercase characters due to compile-time constraints
 * @warning This is a compile-time function that generates optimized matching code
 */
template<::pltxt2htm::Contracts ndebug, ::pltxt2htm::details::U8LiteralString prefix_str>
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
            if (expect != (::pltxt2htm::details::u8string_view_index<ndebug>(str, I) | diff)) {
                return false;
            }
        }
        else {
            if (expect != ::pltxt2htm::details::u8string_view_index<ndebug>(str, I)) {
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
                return expect == (::pltxt2htm::details::u8string_view_index<ndebug>(str, I) | diff);
            }
            else {
                return expect == ::pltxt2htm::details::u8string_view_index<ndebug>(str, I);
            }
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
constexpr auto size_t2str(::std::size_t num) noexcept -> ::fast_io::u8string {
    if (num == 0) {
        return ::fast_io::u8string{u8"0"};
    }

    ::fast_io::u8string result{};

    while (num > 0) {
        char8_t const digit = (num % 10) + u8'0';
        result.push_back(digit);
        num /= 10;
    }

    ::std::reverse(result.begin(), result.end());

    return result;
}

/**
 * @brief Result of parsing an ASCII decimal value.
 */
struct TryParseSizeTDecimalValueResult {
    ::std::size_t end;
    ::std::size_t value;
};

/**
 * @brief Parse an ASCII decimal value starting at a given position.
 * @details Stops at the first non-digit and rejects empty or overflowing values.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_size_t_decimal_value(::fast_io::u8string_view str, ::std::size_t const start) noexcept
    -> ::exception::optional<::pltxt2htm::details::TryParseSizeTDecimalValueResult> {
    ::std::size_t parsed_value{};
    auto pos = start;
    for (; pos < str.size(); ++pos) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(str, pos);
        if (::pltxt2htm::details::is_ascii_digit(chr) == false) {
            break;
        }
        auto const digit = static_cast<::std::size_t>(chr - u8'0');
        if (parsed_value > (::std::numeric_limits<::std::size_t>::max() - digit) / 10) {
            return ::exception::nullopt;
        }
        parsed_value = parsed_value * 10 + digit;
    }
    if (pos == start) {
        return ::exception::nullopt;
    }
    return ::pltxt2htm::details::TryParseSizeTDecimalValueResult{.end = pos, .value = parsed_value};
}

/**
 * @brief Convert a UTF-8 string to std::size_t
 * @tparam ndebug Contract checking mode controlling assertion behavior.
 * @param[in] str The string to convert (must contain only digits)
 * @return Optional containing the converted number, or nullopt if conversion fails
 * @retval exception::optional<std::size_t> Optional containing the converted number
 * @note This function only accepts strings containing ASCII digits (0-9).
 *       Empty strings or strings with non-digit characters return nullopt
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto u8str2size_t(::fast_io::u8string_view str) noexcept -> ::exception::optional<std::size_t> {
    auto result = ::pltxt2htm::details::try_parse_size_t_decimal_value<ndebug>(str, 0);
    if (result.has_value() == false) {
        return ::exception::nullopt;
    }
    auto const parsed = result.template value<false>();
    if (parsed.end != str.size()) {
        return ::exception::nullopt;
    }
    return parsed.value;
}

} // namespace pltxt2htm::details

#include "pop_macro.hh"
