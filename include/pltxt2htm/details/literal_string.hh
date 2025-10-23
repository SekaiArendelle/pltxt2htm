#pragma once

#include <limits>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace pltxt2htm::details {

template<::std::size_t>
class LiteralString;

template<::std::size_t N>
LiteralString(char const (&str)[N]) -> LiteralString<N - 1>;

template<::std::size_t N>
LiteralString(char8_t const (&str)[N]) -> LiteralString<N - 1>;

template<typename T>
constexpr bool is_literal_string_ = false;

template<::std::size_t N>
constexpr bool is_literal_string_<LiteralString<N>> = true;

template<typename T>
concept is_leteral_string = is_literal_string_<::std::remove_cvref_t<T>>;

template<::std::size_t N>
class LiteralString {
public:
    char data_[N]{};

    using size_type = ::std::size_t;
    using diffrence_type = ::std::ptrdiff_t;
    using const_iterator = char const*;

    constexpr LiteralString() noexcept = default;

    template<::std::size_t M>
    constexpr LiteralString(char const (&str)[M]) noexcept {
        static_assert(N > 0 && N + 1 == M);
        // Assume the end of the string is '\0'
        for (::std::size_t i{}; i < N; ++i) {
            this->data_[i] = str[i];
        }
    }

    template<::std::size_t M>
    constexpr LiteralString(char8_t const (&str)[M]) noexcept {
        static_assert(N > 0 && N + 1 == M);
        // Assume the end of the string is '\0'
        for (::std::size_t i{}; i < N; ++i) {
            this->data_[i] = str[i];
        }
    }

    template<::pltxt2htm::details::is_leteral_string Self, ::std::size_t M>
    constexpr auto operator==(this Self const& self, ::pltxt2htm::details::LiteralString<M> const& other) noexcept {
        if constexpr (N != M) {
            return false;
        } else {
            return ::std::equal(self.cbegin(), self.cend(), other.data());
        }
    }

    template<::pltxt2htm::details::is_leteral_string Self>
    constexpr auto operator[](this Self&& self, ::std::size_t index) noexcept -> char& {
        if (index >= N) [[unlikely]] {
            __builtin_trap();
        }
        return self.data_[index];
    }

    template<::pltxt2htm::details::is_leteral_string Self>
    constexpr auto operator[](this Self const& self, ::std::size_t index) noexcept -> char const& {
        if (index >= N) [[unlikely]] {
            __builtin_trap();
        }
        return self.data_[index];
    }

    static constexpr ::std::size_t size() noexcept {
        return N;
    }

    template<::pltxt2htm::details::is_leteral_string Self>
    constexpr auto cbegin(this Self const& self) noexcept {
        return static_cast<const_iterator>(self.data_);
    }

    template<::pltxt2htm::details::is_leteral_string Self>
    constexpr auto cend(this Self const& self) noexcept {
        return static_cast<const_iterator>(self.data_ + N);
    }

    template<::pltxt2htm::details::is_leteral_string Self>
    constexpr auto data(this Self&& self) noexcept -> char* {
        return self.data_;
    }

    template<::pltxt2htm::details::is_leteral_string Self>
    constexpr auto data(this Self const& self) noexcept {
        return static_cast<char const*>(self.data_);
    }

    template<::pltxt2htm::details::is_leteral_string Self>
    constexpr auto cdata(this Self&& self) noexcept {
        return static_cast<char const*>(self.data_);
    }
};

template<::pltxt2htm::details::LiteralString str, ::std::size_t M = 0>
consteval auto shrink_string_literal_() noexcept {
    if constexpr (M >= str.size()) {
        return str;
    } else if constexpr (str[M] == 0) {
        char result[M + 1]{};
        ::std::reverse_copy(str.cbegin(), str.cbegin() + M, result);
        return ::pltxt2htm::details::LiteralString{result};
    } else {
        return ::pltxt2htm::details::shrink_string_literal_<str, M + 1>();
    }
}

consteval auto uint_to_literal_string_(::std::uint_least32_t number) noexcept {
    using result_type = ::pltxt2htm::details::LiteralString<::std::numeric_limits<decltype(number)>::digits10 + 2>;
    auto result = result_type{};
    ::std::size_t index{};
    while (number) {
        result[index++] = static_cast<char>(number % 10 + '0');
        number /= 10;
    }
    return result;
}

template<::std::uint_least32_t number>
consteval auto uint_to_literal_string() noexcept {
    constexpr auto result = ::pltxt2htm::details::uint_to_literal_string_(number);
    return ::pltxt2htm::details::shrink_string_literal_<result>();
}

template<is_leteral_string... Args>
consteval auto concat(Args const&... args) noexcept {
    ::pltxt2htm::details::LiteralString<(args.size() + ...)> result{};
    ::std::size_t index{};
    // TODO impl template-for version
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

} // namespace pltxt2htm::details
