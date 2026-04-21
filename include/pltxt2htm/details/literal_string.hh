/**
 * @file literal_string.hh
 * @brief Compile-time string utilities for pltxt2htm
 * @details Provides a compile-time string class that stores string data
 *          as part of the type, enabling efficient compile-time string operations
 * @note This is used for compile-time error messages and static string processing
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>

#include <exception/exception.hh>

namespace pltxt2htm::details {

template<typename, ::std::size_t>
class BasicLiteralString;

template<::std::size_t N>
using LiteralString = ::pltxt2htm::details::BasicLiteralString<char, N>;

template<::std::size_t N>
using U8LiteralString = ::pltxt2htm::details::BasicLiteralString<char8_t, N>;

template<typename CharType, ::std::size_t N>
BasicLiteralString(CharType const (&str)[N]) -> BasicLiteralString<CharType, N - 1>;

namespace details {

template<typename T>
constexpr bool is_literal_string_ = false;

template<typename CharType, ::std::size_t N>
constexpr bool is_literal_string_<::pltxt2htm::details::BasicLiteralString<CharType, N>> = true;

} // namespace details

template<typename T>
concept is_leteral_string = ::pltxt2htm::details::details::is_literal_string_<::std::remove_cvref_t<T>>;

template<typename CharType, ::std::size_t N>
class BasicLiteralString {
public:
    using value_type = CharType;
    using size_type = ::std::size_t;
    using diffrence_type = ::std::ptrdiff_t;
    using iterator = value_type*;
    using const_iterator = value_type const*;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;

    value_type data_[N]{};

    constexpr BasicLiteralString() noexcept = default;

    template<::std::size_t M>
    constexpr BasicLiteralString(CharType const (&str)[M]) noexcept {
        static_assert(N > 0 && N + 1 == M);
        for (::std::size_t i{}; i < N; ++i) {
            this->data_[i] = str[i];
        }
    }

    template<::std::size_t M>
    [[nodiscard]]
    constexpr auto operator==(this ::pltxt2htm::details::BasicLiteralString<value_type, N> const& self,
                              ::pltxt2htm::details::BasicLiteralString<value_type, M> const& other) noexcept {
        if constexpr (N != M) {
            return false;
        }
        else {
            return ::std::equal(self.cbegin(), self.cend(), other.data());
        }
    }

    constexpr auto&& operator[](this ::pltxt2htm::details::is_leteral_string auto&& self,
                                ::std::size_t index) noexcept {
        if (index >= N) [[unlikely]] {
            ::exception::terminate();
        }
        return self.data_[index];
    }

    [[nodiscard]]
    static constexpr ::std::size_t size() noexcept {
        return N;
    }

    [[nodiscard]]
    constexpr auto begin(this ::pltxt2htm::details::is_leteral_string auto&& self) noexcept -> iterator {
        return self.data_;
    }

    [[nodiscard]]
    constexpr auto begin(this ::pltxt2htm::details::BasicLiteralString<value_type, N> const& self) noexcept {
        return const_iterator(self.data_);
    }

    [[nodiscard]]
    constexpr auto cbegin(this ::pltxt2htm::details::BasicLiteralString<value_type, N> const& self) noexcept {
        return const_iterator(self.data_);
    }

    [[nodiscard]]
    constexpr auto end(this ::pltxt2htm::details::is_leteral_string auto&& self) noexcept -> iterator {
        return self.data_ + N;
    }

    [[nodiscard]]
    constexpr auto end(this ::pltxt2htm::details::BasicLiteralString<value_type, N> const& self) noexcept {
        return const_iterator(self.data_ + N);
    }

    [[nodiscard]]
    constexpr auto cend(this ::pltxt2htm::details::BasicLiteralString<value_type, N> const& self) noexcept {
        return const_iterator(self.data_ + N);
    }

    [[nodiscard]]
    constexpr auto data(this ::pltxt2htm::details::is_leteral_string auto&& self) noexcept -> pointer {
        return self.data_;
    }

    [[nodiscard]]
    constexpr auto data(this ::pltxt2htm::details::BasicLiteralString<value_type, N> const& self) noexcept {
        return const_pointer(self.data_);
    }

    [[nodiscard]]
    constexpr auto cdata(this ::pltxt2htm::details::BasicLiteralString<value_type, N> const& self) noexcept {
        return const_pointer(self.data_);
    }
};

template<::pltxt2htm::details::U8LiteralString str, ::std::size_t M = 0>
consteval auto shrink_string_literal_() noexcept {
    if constexpr (M >= str.size()) {
        return str;
    }
    else if constexpr (str[M] == 0) {
        char8_t result[M + 1]{};
        ::std::reverse_copy(str.cbegin(), str.cbegin() + M, result);
        return ::pltxt2htm::details::U8LiteralString{result};
    }
    else {
        return ::pltxt2htm::details::shrink_string_literal_<str, M + 1>();
    }
}

consteval auto uint_to_literal_string_(::std::uint_least32_t number) noexcept {
    using result_type = ::pltxt2htm::details::U8LiteralString<::std::numeric_limits<decltype(number)>::digits10 + 2>;
    auto result = result_type{};
    ::std::size_t index{};
    while (number) {
        result[index++] = static_cast<char8_t>(number % 10 + '0');
        number /= 10;
    }
    return result;
}

template<::std::uint_least32_t number>
consteval auto uint_to_literal_string() noexcept {
    constexpr auto result = ::pltxt2htm::details::uint_to_literal_string_(number);
    return ::pltxt2htm::details::shrink_string_literal_<result>();
}

template<typename result_type>
constexpr void concat_memcpy(::pltxt2htm::details::is_leteral_string auto const& args, ::std::size_t& index,
                             result_type& result) noexcept {
    for (::std::size_t i{}; i < args.size(); ++i) {
        result[i + index] = args[i];
    }
}

/**
 * @brief Concatenate multiple LiteralStrings into one
 * @tparam Args Types of the input strings
 * @param[in] args The strings to concatenate
 * @return A new LiteralString containing all input strings concatenated
 */
template<::pltxt2htm::details::is_leteral_string Arg, ::pltxt2htm::details::is_leteral_string... Args>
    requires (::std::is_same_v<typename Arg::value_type, typename Args::value_type> && ...)
consteval auto concat(Arg const& arg, Args const&... args) noexcept {
    ::pltxt2htm::details::BasicLiteralString<typename Arg::value_type, arg.size() + (args.size() + ...)> result{};
    ::std::size_t index{};
    for (; index < arg.size(); ++index) {
        result[index] = arg[index];
    }
    ((::pltxt2htm::details::concat_memcpy(args, index, result), index += args.size()), ...);

    return result;
}

} // namespace pltxt2htm::details
