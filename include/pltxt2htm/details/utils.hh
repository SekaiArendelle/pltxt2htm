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
#include <type_traits>
#include <fast_io/fast_io_dsal/stack.h>
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include "../container/expected.hh"
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
    return ::pltxt2htm::details::is_ascii_graphic(chr) || chr >= char8_t(0x80);
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
    pltxt2htm_assert(i < pltext.size(), u8"Index of u8string_view out of bound");

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
    pltxt2htm_assert(!vec.empty(), u8"Indexing front but vector is empty");

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
    pltxt2htm_assert(i < vec.size(), u8"Index of vector out of bound");

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
    pltxt2htm_assert(stack.empty() == false, u8"Accessing top but stack is empty");

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
    pltxt2htm_assert(stack.empty() == false, u8"Accessing top but stack is empty");

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
template<::pltxt2htm::Contracts ndebug, U8LiteralString prefix_str>
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

    ::std::ranges::reverse(result);

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
 * @brief Parse an ASCII decimal value.
 * @details Stops at the first non-digit and rejects empty or overflowing values.
 *          Callers that need to skip a prefix should pass `str` pre-subviewed;
 *          the returned `end` is the run length relative to `str`.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_size_t_decimal_value(::fast_io::u8string_view str) noexcept
    -> ::pltxt2htm::container::Optional<TryParseSizeTDecimalValueResult> {
    ::std::size_t const str_size{str.size()};
    ::std::size_t parsed_value{};
    auto pos = ::std::size_t{0};
    for (; pos < str_size; ++pos) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(str, pos);
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
 * @brief Convert a signed integer (::std::ptrdiff_t) to a UTF-8 string.
 * @details Handles negative values with a leading U+002D '-' sign. Mirrors
 *          ::pltxt2htm::details::size_t2str but for signed values.
 */
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__pure__)
[[__gnu__::__pure__]]
#endif
constexpr auto ptrdiff_t2str(::std::ptrdiff_t num) noexcept -> ::fast_io::u8string {
    if (num == 0) {
        return ::fast_io::u8string{u8"0"};
    }

    auto magnitude = static_cast<::std::make_unsigned_t<::std::ptrdiff_t>>(num);
    if (num < 0) {
        magnitude = static_cast<::std::make_unsigned_t<::std::ptrdiff_t>>(0) - magnitude;
    }

    ::fast_io::u8string result{};
    while (magnitude > 0) {
        char8_t const digit = (magnitude % 10) + u8'0';
        result.push_back(digit);
        magnitude /= 10;
    }
    ::std::ranges::reverse(result);

    if (num < 0) {
        result.insert(result.begin(), ::fast_io::u8string_view{u8"-"});
    }

    return result;
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
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_ptrdiff_t_decimal_value(::fast_io::u8string_view str) noexcept
    -> ::pltxt2htm::container::Optional<TryParsePtrdiffTDecimalValueResult> {
    using unsigned_type = ::std::make_unsigned_t<::std::ptrdiff_t>;

    ::std::size_t const str_size{str.size()};
    auto pos = ::std::size_t{0};
    bool const negative = pos < str_size && ::pltxt2htm::details::u8string_view_index<ndebug>(str, pos) == u8'-';
    if (negative) {
        ++pos;
    }

    unsigned_type parsed_value{};
    auto digit_pos = pos;
    for (; digit_pos < str_size; ++digit_pos) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(str, digit_pos);
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
 * @brief Result of parsing a non-negative ASCII decimal value that may be fractional.
 */
struct TryParseDoubleDecimalValueResult {
    ::std::size_t end;
    double value;
};

/**
 * @brief Parse a non-negative ASCII decimal value that may contain a fractional part.
 * @details Accepts `[0-9]+` with an optional `.` followed by one or more `[0-9]`; a bare `.`
 *          without digits is rejected. Stops at the first character that is neither a digit nor
 *          the fraction dot; the returned `end` is the run length relative to `str`. The value is
 *          stored as double so `<size=12.5>` keeps its fractional part exact enough to
 *          round-trip when emitted again.
 */
template<::pltxt2htm::Contracts ndebug>
[[nodiscard]]
constexpr auto try_parse_double_decimal_value(::fast_io::u8string_view str) noexcept
    -> ::pltxt2htm::container::Optional<TryParseDoubleDecimalValueResult> {
    ::std::size_t const str_size{str.size()};
    double parsed_value{};
    auto pos = ::std::size_t{0};
    for (; pos < str_size; ++pos) {
        auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(str, pos);
        if (::pltxt2htm::details::is_ascii_digit(chr) == false) {
            break;
        }
        parsed_value = parsed_value * 10 + static_cast<double>(chr - u8'0');
    }
    if (pos < str_size && ::pltxt2htm::details::u8string_view_index<ndebug>(str, pos) == u8'.') {
        if (pos == 0) {
            return ::pltxt2htm::container::nullopt;
        }
        auto const dot_pos = pos;
        auto frac_pos = dot_pos + 1;
        double scale{10};
        for (; frac_pos < str_size; ++frac_pos) {
            auto const chr = ::pltxt2htm::details::u8string_view_index<ndebug>(str, frac_pos);
            if (::pltxt2htm::details::is_ascii_digit(chr) == false) {
                break;
            }
            parsed_value += static_cast<double>(chr - u8'0') / scale;
            scale *= 10;
        }
        if (frac_pos == dot_pos + 1) {
            return ::pltxt2htm::container::nullopt;
        }
        pos = frac_pos;
    }
    if (pos == 0) {
        return ::pltxt2htm::container::nullopt;
    }
    // Reject values that cannot be represented in ::std::size_t so oversized tags stay literal
    // text (preserving the historical behavior of try_parse_size_t_decimal_value). `>=` is used
    // because double(std::size_t max) rounds up to 2^64, and any double >= 2^64 cannot fit.
    if (parsed_value >= static_cast<double>(::std::numeric_limits<::std::size_t>::max())) {
        return ::pltxt2htm::container::nullopt;
    }
    return TryParseDoubleDecimalValueResult{.end = pos, .value = parsed_value};
}

/**
 * @brief Round a non-negative double up to the nearest integer.
 */
[[nodiscard]]
constexpr auto double_to_size_t_ceil(double value) noexcept -> ::std::size_t {
    auto const truncated = static_cast<::std::size_t>(value);
    return static_cast<double>(truncated) < value ? truncated + 1 : truncated;
}

/**
 * @brief Convert a non-negative double to a UTF-8 decimal string that round-trips.
 * @details Tries fixed-point representations with 0 through 17 fractional digits and keeps the
 *          first one that parses back to exactly `value`. Integral values therefore print without
 *          a decimal point (e.g. 12.0 -> "12"), while fractional values print with the minimal
 *          number of digits (e.g. 12.5 -> "12.5").
 */
[[nodiscard]]
constexpr auto double2str(double value) noexcept -> ::fast_io::u8string {
    constexpr ::std::size_t max_fractional_digits{17};
    constexpr double max_scaled{static_cast<double>(::std::numeric_limits<::std::ptrdiff_t>::max()) + 1};
    ::fast_io::u8string fallback{};
    for (::std::size_t fractional_digits{0}; fractional_digits <= max_fractional_digits; ++fractional_digits) {
        double scale{1};
        for (::std::size_t i{0}; i < fractional_digits; ++i) {
            scale *= 10;
        }
        double const scaled = value * scale;
        if (scaled > max_scaled) {
            continue;
        }
        auto rounded = static_cast<::std::ptrdiff_t>(scaled);
        if (static_cast<double>(rounded) < scaled) {
            ++rounded;
        }
        auto const digit_str = ::pltxt2htm::details::size_t2str(static_cast<::std::size_t>(rounded));
        ::std::size_t const digit_str_size{digit_str.size()};
        ::fast_io::u8string candidate{};
        if (fractional_digits == 0) {
            candidate = digit_str;
        }
        else if (digit_str_size > fractional_digits) {
            auto const frac_start = digit_str_size - fractional_digits;
            candidate.append(::fast_io::u8string_view{digit_str.data(), frac_start});
            candidate.push_back(u8'.');
            candidate.append(::fast_io::u8string_view{digit_str.data() + frac_start, fractional_digits});
        }
        else {
            candidate.append(u8"0.");
            ::std::size_t const padding_size{fractional_digits - digit_str_size};
            for (::std::size_t i{0}; i < padding_size; ++i) {
                candidate.push_back(u8'0');
            }
            candidate.append(digit_str);
        }
        auto opt_reparsed = ::pltxt2htm::details::try_parse_double_decimal_value<::pltxt2htm::Contracts::quick_enforce>(
            ::fast_io::u8string_view{candidate.data(), candidate.size()});
        bool const round_trips =
            opt_reparsed.has_value() && (opt_reparsed.template value<::pltxt2htm::Contracts::ignore>().value == value);
        if (round_trips) {
            return candidate;
        }
        if (fractional_digits == max_fractional_digits) {
            fallback = ::std::move(candidate);
        }
    }
    if (fallback.empty() == false) {
        return fallback;
    }
    // Values too large for a signed 64-bit integer cannot be round-tripped above; emit the
    // truncated integer part instead of failing (the parser guarantees value <= size_t max).
    return ::pltxt2htm::details::size_t2str(static_cast<::std::size_t>(value));
}

} // namespace pltxt2htm::details

#include "pop_macro.hh"
