#pragma once

#include <cstddef>
#if __cpp_expansion_statements >= 202506L
    #include <ranges>
#endif
#include <fast_io/fast_io_dsal/vector.h>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <exception/exception.hh>

#include "push_macro.hh"

namespace pltxt2htm::details {

/**
 * @return index of ::fast_io::u8string_view
 */
template<bool ndebug>
#if __has_cpp_attribute(__gnu__::always_inline)
[[__gnu__::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
[[nodiscard]]
constexpr auto u8string_view_index(::fast_io::u8string_view pltext, ::std::size_t i) noexcept {
    ::std::size_t const pltext_size{pltext.size()};
    pltxt2htm_assert(i < pltext_size, u8"Index of u8string_view out of bound");

    return pltext.index_unchecked(i);
}

/**
 * @brief Get the slice of u8string_view
 */
template<bool ndebug>
#if __has_cpp_attribute(__gnu__::always_inline)
[[__gnu__::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
[[nodiscard]]
constexpr auto u8string_view_subview(::fast_io::u8string_view pltext, ::std::size_t i,
                                     ::std::size_t count = ::fast_io::containers::npos) noexcept
    -> ::fast_io::u8string_view {
    if constexpr (ndebug) {
        return pltext.subview_unchecked(i, count);
    } else {
        return pltext.subview(i, count);
    }
}

template<bool ndebug, typename T>
#if __has_cpp_attribute(__gnu__::always_inline)
[[__gnu__::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
[[nodiscard]]
constexpr auto vector_front(::fast_io::vector<T> const& vec) noexcept -> T const& {
    pltxt2htm_assert(!vec.empty(), u8"Indexing front but vector is empty");

    return vec.front_unchecked();
}

/**
 * @return index of ::fast_io::u8string_view
 */
template<bool ndebug, typename T>
#if __has_cpp_attribute(__gnu__::always_inline)
[[__gnu__::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
[[nodiscard]]
constexpr auto vector_index(::fast_io::vector<T> const& vec, ::std::size_t i) noexcept -> T const& {
    pltxt2htm_assert(i < vec.size(), u8"Index of vector out of bound");

    return vec.index_unchecked(i);
}

/**
 * @brief Get the index-th char8_t from the string.
 * @return The char8_t at index I of str.
 */
template<::std::size_t I, char8_t first_char, char8_t... str>
    requires (I <= sizeof...(str))
[[nodiscard]]
consteval char8_t pack_indexing_char8_t() noexcept {
    // https://en.cppreference.com/w/cpp/language/pack_indexing.html
    if constexpr (I == 0) {
        return first_char;
    } else {
#if __cpp_pack_indexing >= 202311L
    #if defined(__clang__)
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wc++26-extensions"
    #endif
        return str...[I - 1];
    #if defined(__clang__)
        #pragma clang diagnostic pop
    #endif
#else
        return pack_indexing_char8_t<I - 1, str...>();
#endif
    }
}

/**
 * @brief Check whether the string is a prefix (without distinguishing
 *        between uppercase and lowercase) of the pl-text.
 *        This is a magic funciton that will generate if-expression in compile time.
 * @tparam prefix_str: The prefix string.
 * @param str: The string to be checked.
 * @return Whether the string is a prefix of the pl-text.
 */
template<bool ndebug, char8_t... prefix_str>
    requires ((prefix_str < 'A' || prefix_str > 'Z') && ...)
#if __has_cpp_attribute(__gnu__::always_inline)
[[__gnu__::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
[[nodiscard]]
constexpr bool is_prefix_match(::fast_io::u8string_view str) noexcept {
    // Check whether the index is out of bound.
    if (sizeof...(prefix_str) > str.size()) [[unlikely]] {
        return false;
    }

    // Return whether the `prefix_str` is a prefix of the str.
#if __cpp_expansion_statements >= 202506L
    template for (constexpr ::std::size_t I : ::std::ranges::views::iota(::std::size_t{}, sizeof...(prefix_str))) {
#else
    return [str]<::std::size_t... Is>(::std::index_sequence<Is...>) {
        return ([str]<::std::size_t I>() {
#endif
        constexpr auto expect = ::pltxt2htm::details::pack_indexing_char8_t<I, prefix_str...>();
        if constexpr ('a' <= expect && expect <= 'z') {
            // ASCII between lowercase and uppercase is 32 (e.g. 'a' - 'A' == 32)
            constexpr ::std::uint8_t diff{32};
            // (expect != str[I] && expect != str[I] + diff) <=> (expect != (str[I] | diff))
            if (expect != (::pltxt2htm::details::u8string_view_index<ndebug>(str, I) | diff)) {
                return false;
            } else {
                return true;
            }
        } else {
            if (expect != ::pltxt2htm::details::u8string_view_index<ndebug>(str, I)) {
                return false;
            } else {
                return true;
            }
        }
#if __cpp_expansion_statements >= 202506L
    }
#else
        }.template operator()<Is>() &&
                ...);
    }(::std::make_index_sequence<sizeof...(prefix_str)>{});
#endif
}

/**
 * @brief Convert a ::std::size_t to a string.
 */
[[nodiscard]]
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
 * @brief Convert a string to a ::std::size_t.
 */
[[nodiscard]]
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
