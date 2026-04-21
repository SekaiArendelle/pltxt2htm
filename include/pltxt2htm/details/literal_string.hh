/**
 * @file literal_string.hh
 * @brief Compile-time string utilities for pltxt2htm
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>
#include <tuple>

#include <exception/exception.hh>

namespace pltxt2htm {

template<typename ch_type, ::std::size_t>
class BasicLiteralString;

template<::std::size_t N>
using LiteralString = ::pltxt2htm::BasicLiteralString<char, N>;

template<::std::size_t N>
using U8LiteralString = ::pltxt2htm::BasicLiteralString<char8_t, N>;

template<typename ch_type, ::std::size_t N>
BasicLiteralString(ch_type const (&str)[N]) -> BasicLiteralString<ch_type, N - 1>;

namespace details {

template<typename T>
constexpr bool is_literal_string_ = false;

template<typename ch_type, ::std::size_t N>
constexpr bool is_literal_string_<::pltxt2htm::BasicLiteralString<ch_type, N>> = true;

template<typename T>
concept is_leteral_string = ::pltxt2htm::details::is_literal_string_<::std::remove_cvref_t<T>>;

} // namespace details

template<typename ch_type, ::std::size_t N>
class BasicLiteralString {
public:
    using value_type = ch_type;
    using size_type = ::std::size_t;
    using diffrence_type = ::std::ptrdiff_t;
    using iterator = value_type*;
    using const_iterator = value_type const*;

    value_type data_[N]{};

    constexpr BasicLiteralString() noexcept = default;

    template<typename source_char_type, ::std::size_t M>
    constexpr BasicLiteralString(source_char_type const (&str)[M]) noexcept {
        static_assert(N > 0 && N + 1 == M);
        for (::std::size_t i{}; i < N; ++i) {
            this->data_[i] = static_cast<value_type>(str[i]);
        }
    }

    template<::pltxt2htm::details::is_leteral_string Self, ::std::size_t M>
    [[nodiscard]]
    constexpr auto operator==(this Self const& self,
                              ::pltxt2htm::BasicLiteralString<typename Self::value_type, M> const& other) noexcept {
        if constexpr (N != M) {
            return false;
        }
        else {
            return ::std::equal(self.cbegin(), self.cend(), other.data());
        }
    }

    template<::pltxt2htm::details::is_leteral_string Self>
    constexpr auto operator[](this Self&& self, ::std::size_t index) noexcept -> value_type& {
        if (index >= N) [[unlikely]] {
            ::exception::terminate();
        }
        return self.data_[index];
    }

    template<::pltxt2htm::details::is_leteral_string Self>
    [[nodiscard]]
    constexpr auto operator[](this Self const& self, ::std::size_t index) noexcept -> value_type const& {
        if (index >= N) [[unlikely]] {
            ::exception::terminate();
        }
        return self.data_[index];
    }

    [[nodiscard]]
    static constexpr ::std::size_t size() noexcept {
        return N;
    }

    template<::pltxt2htm::details::is_leteral_string Self>
    [[nodiscard]]
    constexpr auto begin(this Self&& self) noexcept -> iterator {
        return self.data_;
    }

    [[nodiscard]]
    constexpr auto begin(this ::pltxt2htm::BasicLiteralString<value_type, N> const& self) noexcept {
        return const_iterator(self.data_);
    }

    [[nodiscard]]
    constexpr auto cbegin(this ::pltxt2htm::BasicLiteralString<value_type, N> const& self) noexcept {
        return const_iterator(self.data_);
    }

    template<::pltxt2htm::details::is_leteral_string Self>
    [[nodiscard]]
    constexpr auto end(this Self&& self) noexcept -> iterator {
        return self.data_ + N;
    }

    [[nodiscard]]
    constexpr auto end(this ::pltxt2htm::BasicLiteralString<value_type, N> const& self) noexcept {
        return const_iterator(self.data_ + N);
    }

    [[nodiscard]]
    constexpr auto cend(this ::pltxt2htm::BasicLiteralString<value_type, N> const& self) noexcept {
        return const_iterator(self.data_ + N);
    }

    [[nodiscard]]
    constexpr auto data(this ::pltxt2htm::BasicLiteralString<value_type, N>& self) noexcept {
        return self.data_;
    }

    [[nodiscard]]
    constexpr auto data(this ::pltxt2htm::BasicLiteralString<value_type, N> const& self) noexcept {
        return ::std::as_const(self.data_);
    }

    [[nodiscard]]
    constexpr auto data(this ::pltxt2htm::BasicLiteralString<value_type, N>&& self) noexcept {
        return ::std::move(self.data_);
    }

    template<::pltxt2htm::details::is_leteral_string Self>
    [[nodiscard]]
    constexpr auto cdata(this Self&& self) noexcept {
        return static_cast<value_type const*>(self.data_);
    }
};

namespace details {

template<::pltxt2htm::U8LiteralString str, ::std::size_t M = 0>
consteval auto shrink_string_literal_() noexcept {
    if constexpr (M >= str.size()) {
        return str;
    }
    else if constexpr (str[M] == 0) {
        char8_t result[M + 1]{};
        ::std::reverse_copy(str.cbegin(), str.cbegin() + M, result);
        return ::pltxt2htm::U8LiteralString{result};
    }
    else {
        return ::pltxt2htm::details::shrink_string_literal_<str, M + 1>();
    }
}

consteval auto uint_to_literal_string_(::std::uint_least32_t number) noexcept {
    using result_type = ::pltxt2htm::U8LiteralString<::std::numeric_limits<decltype(number)>::digits10 + 2>;
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

template<::pltxt2htm::details::is_leteral_string... Args>
consteval auto concat(Args const&... args) noexcept {
    using ch_type = typename ::std::tuple_element_t<0, ::std::tuple<Args...>>::value_type;
    ::pltxt2htm::BasicLiteralString<ch_type, (args.size() + ...)> result{};
    ::std::size_t index{};
    (((
         [args, index, &result]() constexpr noexcept {
             for (::std::size_t i{}; i < args.size(); ++i) {
                 result[i + index] = args[i];
             }
         }(),
         index += args.size())),
     ...);

    return result;
}

} // namespace details

} // namespace pltxt2htm
