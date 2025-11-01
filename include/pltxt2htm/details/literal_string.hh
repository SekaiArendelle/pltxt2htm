#pragma once

#include <limits>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace pltxt2htm::details {

/**
 * @brief A compile-time string class that stores string data as part of the type
 * @tparam N The length of the string
 */
template<::std::size_t>
class LiteralString;

/**
 * @brief Deduction guide for char array literals
 * @tparam N The length of the string including null terminator
 * @param str The input string literal
 */
template<::std::size_t N>
LiteralString(char const (&str)[N]) -> LiteralString<N - 1>;

/**
 * @brief Deduction guide for char8_t array literals
 * @tparam N The length of the string including null terminator
 * @param str The input string literal
 */
template<::std::size_t N>
LiteralString(char8_t const (&str)[N]) -> LiteralString<N - 1>;

/**
 * @brief Type trait to check if a type is a LiteralString
 * @tparam T The type to check
 */
template<typename T>
constexpr bool is_literal_string_ = false;

/**
 * @brief Specialization for LiteralString types
 * @tparam N The length of the string
 */
template<::std::size_t N>
constexpr bool is_literal_string_<::pltxt2htm::details::LiteralString<N>> = true;

/**
 * @brief Concept to identify LiteralString types
 * @tparam T The type to check
 */
template<typename T>
concept is_leteral_string = ::pltxt2htm::details::is_literal_string_<::std::remove_cvref_t<T>>;

/**
 * @brief A compile-time string class that stores string data as part of the type
 * @tparam N The length of the string
 */
template<::std::size_t N>
class LiteralString {
public:
    /** @brief Storage for the string characters */
    char data_[N]{};

    /** @brief Type used for size values */
    using size_type = ::std::size_t;

    /** @brief Type used for difference values */
    using diffrence_type = ::std::ptrdiff_t;

    /** @brief Iterator type for const access */
    using const_iterator = char const*;

    /** @brief Default constructor */
    constexpr LiteralString() noexcept = default;

    /**
     * @brief Constructor from char array literal
     * @tparam M The size of the input array (including null terminator)
     * @param str The input string literal
     */
    template<::std::size_t M>
    constexpr LiteralString(char const (&str)[M]) noexcept {
        static_assert(N > 0 && N + 1 == M);
        // Assume the end of the string is '\0'
        for (::std::size_t i{}; i < N; ++i) {
            this->data_[i] = str[i];
        }
    }

    /**
     * @brief Constructor from char8_t array literal
     * @tparam M The size of the input array (including null terminator)
     * @param str The input string literal
     */
    template<::std::size_t M>
    constexpr LiteralString(char8_t const (&str)[M]) noexcept {
        static_assert(N > 0 && N + 1 == M);
        // Assume the end of the string is '\0'
        for (::std::size_t i{}; i < N; ++i) {
            this->data_[i] = str[i];
        }
    }

    /**
     * @brief Equality comparison with another LiteralString
     * @tparam Self The type of this object (deduced)
     * @tparam M The length of the other string
     * @param self This object
     * @param other The other LiteralString to compare with
     * @return true if strings are equal, false otherwise
     */
    template<::pltxt2htm::details::is_leteral_string Self, ::std::size_t M>
    constexpr auto operator==(this Self const& self, ::pltxt2htm::details::LiteralString<M> const& other) noexcept {
        if constexpr (N != M) {
            return false;
        } else {
            return ::std::equal(self.cbegin(), self.cend(), other.data());
        }
    }

    /**
     * @brief Access character at specific index (non-const version)
     * @tparam Self The type of this object (deduced)
     * @param self This object
     * @param index The index of the character to access
     * @return Reference to the character at the specified index
     */
    template<::pltxt2htm::details::is_leteral_string Self>
    constexpr auto operator[](this Self&& self, ::std::size_t index) noexcept -> char& {
        if (index >= N) [[unlikely]] {
            __builtin_trap();
        }
        return self.data_[index];
    }

    /**
     * @brief Access character at specific index (const version)
     * @tparam Self The type of this object (deduced)
     * @param self This object
     * @param index The index of the character to access
     * @return Const reference to the character at the specified index
     */
    template<::pltxt2htm::details::is_leteral_string Self>
    constexpr auto operator[](this Self const& self, ::std::size_t index) noexcept -> char const& {
        if (index >= N) [[unlikely]] {
            __builtin_trap();
        }
        return self.data_[index];
    }

    /** @brief Get the size of the string */
    static constexpr ::std::size_t size() noexcept {
        return N;
    }

    /**
     * @brief Get iterator to the beginning of the string
     * @tparam Self The type of this object (deduced)
     * @param self This object
     * @return Iterator pointing to the first character
     */
    template<::pltxt2htm::details::is_leteral_string Self>
    constexpr auto cbegin(this Self const& self) noexcept {
        return static_cast<const_iterator>(self.data_);
    }

    /**
     * @brief Get iterator to the end of the string
     * @tparam Self The type of this object (deduced)
     * @param self This object
     * @return Iterator pointing past the last character
     */
    template<::pltxt2htm::details::is_leteral_string Self>
    constexpr auto cend(this Self const& self) noexcept {
        return static_cast<const_iterator>(self.data_ + N);
    }

    /**
     * @brief Get pointer to the underlying data (non-const version)
     * @tparam Self The type of this object (deduced)
     * @param self This object
     * @return Pointer to the string data
     */
    template<::pltxt2htm::details::is_leteral_string Self>
    constexpr auto data(this Self&& self) noexcept -> char* {
        return self.data_;
    }

    /**
     * @brief Get pointer to the underlying data (const version)
     * @tparam Self The type of this object (deduced)
     * @param self This object
     * @return Const pointer to the string data
     */
    template<::pltxt2htm::details::is_leteral_string Self>
    constexpr auto data(this Self const& self) noexcept {
        return static_cast<char const*>(self.data_);
    }

    /**
     * @brief Get const pointer to the underlying data
     * @tparam Self The type of this object (deduced)
     * @param self This object
     * @return Const pointer to the string data
     */
    template<::pltxt2htm::details::is_leteral_string Self>
    constexpr auto cdata(this Self&& self) noexcept {
        return static_cast<char const*>(self.data_);
    }
};

/**
 * @brief Remove trailing null characters from a string literal
 * @tparam str The input string literal
 * @tparam M Current position in the string being processed
 * @return A new LiteralString with trailing nulls removed
 */
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

/**
 * @brief Convert an unsigned integer to a LiteralString
 * @param number The number to convert
 * @return A LiteralString representing the number
 */
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

/**
 * @brief Convert an unsigned integer to a LiteralString at compile time
 * @tparam number The number to convert
 * @return A LiteralString representing the number
 */
template<::std::uint_least32_t number>
consteval auto uint_to_literal_string() noexcept {
    constexpr auto result = ::pltxt2htm::details::uint_to_literal_string_(number);
    return ::pltxt2htm::details::shrink_string_literal_<result>();
}

/**
 * @brief Concatenate multiple LiteralStrings into one
 * @tparam Args Types of the input strings
 * @param args The strings to concatenate
 * @return A new LiteralString containing all input strings concatenated
 */
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
