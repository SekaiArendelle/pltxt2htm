#pragma once

/**
 * @file exception.hh
 * @details https://github.com/SekaiArendelle/exception.git
 */

#include <memory>
#include <utility>
#include <type_traits>
#include <concepts>

#if !defined(__GNUC__) && !defined(__clang__)
    #include <cstdlib>
    #include <exception>
#endif

namespace exception {

/**
 * @brief Terminates the program.
 */
[[noreturn]]
inline void terminate() noexcept {
    // https://llvm.org/doxygen/Compiler_8h_source.html
#if defined(__has_builtin) && __has_builtin(__builtin_trap)
    __builtin_trap();
#else
    ::std::terminate();
#endif
}

/**
 * @brief Unreachable code.
 */
template<bool ndebug = false>
[[noreturn]]
inline void unreachable() noexcept {
    if constexpr (ndebug) {
#if defined(__has_builtin) && __has_builtin(__builtin_unreachable)
        __builtin_unreachable();
#else
        ::std::unreachable();
#endif
    } else {
        ::exception::terminate();
    }
}

template<bool ndebug = false>
constexpr void assert_true(bool cond) noexcept {
    if constexpr (ndebug == false) {
        if (cond == false) [[unlikely]] {
            ::exception::terminate();
        }
    }
}

template<bool ndebug = false>
constexpr void assert_false(bool cond) noexcept {
    if constexpr (ndebug == false) {
        if (cond == true) [[unlikely]] {
            ::exception::terminate();
        }
    }
}

template<typename T>
struct unexpected {
#if __has_cpp_attribute(msvc::no_unique_address)
    [[msvc::no_unique_address]]
#elif __has_cpp_attribute(no_unique_address)
    [[no_unique_address]]
#endif
    T val_{};
};

namespace details {

template<typename T>
constexpr bool is_unexpected_v = false;

template<typename T>
constexpr bool is_unexpected_v<::exception::unexpected<T>> = true;

} // namespace details

template<typename T>
concept is_unexpected = ::exception::details::is_unexpected_v<::std::remove_cvref_t<T>>;

template<typename Ok, typename Fail>
class expected {
    static_assert(!::std::is_reference_v<Ok>);
    static_assert(!::std::is_function_v<Ok>);
    static_assert(!::exception::is_unexpected<Ok>);

public:
    using value_type = ::std::remove_cvref_t<Ok>;
    using error_type = ::std::remove_cvref_t<Fail>;
    using unexpected_type = ::exception::unexpected<Fail>;
    template<typename T>
    using rebind = ::exception::expected<T, Fail>;

private:
    union {
        value_type ok_;
        error_type fail_;
    };

    bool has_value_;

public:
    constexpr expected() noexcept(::std::is_nothrow_default_constructible_v<Ok>)
        requires ::std::is_default_constructible_v<Ok>
        : ok_(),
          has_value_{true} {
    }

    constexpr expected() noexcept = delete;

    constexpr expected(Ok const& ok) noexcept(::std::is_nothrow_copy_constructible_v<Ok>)
        requires (::std::is_copy_constructible_v<Ok>)
        : has_value_{true} {
        ::std::construct_at(::std::addressof(this->ok_), ok);
    }

    constexpr expected(Ok&& ok) noexcept(::std::is_nothrow_move_constructible_v<Ok>)
        requires (::std::is_move_constructible_v<Ok>)
        : has_value_{true} {
        ::std::construct_at(::std::addressof(this->ok_), ::std::move(ok));
    }

    constexpr expected(unexpected<Fail> const& fail) noexcept(::std::is_nothrow_copy_constructible_v<Fail>)
        requires (::std::is_copy_constructible_v<Fail>)
        : has_value_{false} {
        ::std::construct_at(::std::addressof(this->fail_), fail.val_);
    }

    constexpr expected(unexpected<Fail>&& fail) noexcept(::std::is_nothrow_move_constructible_v<Fail>)
        requires (::std::is_move_constructible_v<Fail>)
        : has_value_{false} {
        ::std::construct_at(::std::addressof(this->fail_), ::std::move(fail.val_));
    }

    constexpr expected(expected<Ok, Fail> const& other) noexcept(::std::is_nothrow_copy_constructible_v<Ok> &&
                                                                 ::std::is_nothrow_copy_constructible_v<Fail>)
        : has_value_(other.has_value_) {
        if (this->has_value()) {
            ::std::construct_at(::std::addressof(this->ok_), other.ok_);
        } else {
            ::std::construct_at(::std::addressof(this->fail_), other.fail_);
        }
    }

    constexpr expected(expected<Ok, Fail>&& other) noexcept(::std::is_nothrow_move_constructible_v<Ok> &&
                                                            ::std::is_nothrow_move_constructible_v<Fail>)
        : has_value_(::std::move(other.has_value_)) {
        if (this->has_value()) {
            ::std::construct_at(::std::addressof(this->ok_), ::std::move(other.ok_));
        } else {
            ::std::construct_at(::std::addressof(this->fail_), ::std::move(other.fail_));
        }
    }

    constexpr ~expected() noexcept = default;

    constexpr ~expected() noexcept
        requires ((!::std::is_trivially_destructible_v<Ok>) || (!::std::is_trivially_destructible_v<Fail>))
    {
        if (this->has_value()) {
            ::std::destroy_at(::std::addressof(this->ok_));
        } else {
            ::std::destroy_at(::std::addressof(this->fail_));
        }
    }

    template<typename T>
        requires (::std::same_as<::std::remove_cvref_t<T>, Ok> &&
                  (::std::is_copy_assignable_v<T> || ::std::is_move_assignable_v<T>))
    constexpr auto&& operator=(this ::exception::expected<Ok, Fail>& self, T&& ok) noexcept {
        if (self.has_value()) {
            self.ok_ = ::std::forward<T>(ok);
        } else {
            ::std::destroy_at(::std::addressof(self.fail_));
            ::std::construct_at(::std::addressof(self.ok_), ::std::forward<T>(ok));
            self.has_value_ = true;
        }
        return self;
    }

    template<::exception::is_unexpected T>
    constexpr auto&& operator=(this ::exception::expected<Ok, Fail>& self, T const& fail) noexcept {
        self.has_value_ = false;
        self.fail_ = fail.val_;
        return self;
    }

    template<::exception::is_unexpected T>
    constexpr auto&& operator=(this ::exception::expected<Ok, Fail>& self, T&& fail) noexcept {
        self.has_value_ = false;
        self.fail_ = ::std::move(fail.val_);
        return self;
    }

    constexpr auto&& operator=(this ::exception::expected<Ok, Fail>& self,
                               ::exception::expected<Ok, Fail> const& other) noexcept {
        ::exception::expected<Ok, Fail> tmp(other);
        tmp.swap(self);
        return self;
    }

    constexpr auto&& operator=(this expected<Ok, Fail>& self, expected<Ok, Fail>&& other) noexcept {
        self.swap(other);
        return self;
    }

    template<typename T>
        requires (::std::same_as<::std::remove_cvref_t<T>, expected<Ok, Fail>> && ::std::is_move_assignable_v<Ok> &&
                  ::std::is_move_assignable_v<Fail>)
    constexpr void swap(this expected<Ok, Fail>& self, T&& other) noexcept {
        if (self.has_value()) {
            if (other.has_value()) {
                Ok tmp{::std::move(self.ok_)};
                self.ok_ = ::std::move(other.ok_);
                other.ok_ = ::std::move(tmp);
            } else {
                Ok tmp{::std::move(self.ok_)};
                self.fail_ = ::std::move(other.fail_);
                other.ok_ = ::std::move(tmp);
                self.has_value_ = false;
                other.has_value_ = true;
            }
        } else {
            if (other.has_value()) {
                Fail tmp{::std::move(self.fail_)};
                self.ok_ = ::std::move(other.ok_);
                other.fail_ = ::std::move(tmp);
                self.has_value_ = true;
                other.has_value_ = false;
            } else {
                Fail tmp{::std::move(self.fail_)};
                self.fail_ = ::std::move(other.fail_);
                other.fail_ = ::std::move(tmp);
            }
        }
    }

    [[nodiscard]]
    constexpr auto&& has_value(this expected<Ok, Fail> const& self) noexcept {
        return self.has_value_;
    }

    [[nodiscard]]
    constexpr auto&& has_value(this expected<Ok, Fail> const&& self) noexcept {
        return ::std::move(self.has_value_);
    }

    /**
     * @brief get value from optional or expected, if it is not, terminate the program
     * @param self: the optional or expected object
     */
    template<bool ndebug = false>
    [[nodiscard]]
    constexpr auto&& value(this auto&& self) noexcept {
        ::exception::assert_true<ndebug>(self.has_value());
        return ::std::forward_like<decltype(self)>(self.ok_);
    }

    /**
     * @brief get the error value from an expected
     */
    template<bool ndebug = false>
    [[nodiscard]]
    constexpr auto&& error(this auto&& self) noexcept {
        ::exception::assert_false<ndebug>(self.has_value());
        return ::std::forward_like<decltype(self)>(self.fail_);
    }

    /**
     * @brief get value from optional or expected, if it is not, return the value you passed
     * @param self: the optional or expected object
     * @param val: the value you want to return if the optional or expected is not
     * @return: the value
     * @note: implicit conversion of val is not allowed
     */
    template<typename U>
        requires (::std::same_as<U, value_type>)
    [[nodiscard]]
    constexpr auto value_or(U& val) & noexcept -> value_type& {
        if (this->has_value() == false) {
            return val;
        } else {
            return this->ok_;
        }
    }

    template<typename U>
        requires (::std::same_as<U, value_type>)
    [[nodiscard]]
    constexpr auto value_or(U const& val) const& noexcept -> value_type const& {
        if (this->has_value() == false) {
            return val;
        } else {
            return this->ok_;
        }
    }

    template<typename U>
        requires (::std::same_as<U, value_type>)
    [[nodiscard]]
    constexpr auto value_or(U&& val) && noexcept -> value_type&& {
        if (this->has_value() == false) {
            return ::std::move(val);
        } else {
            return ::std::move(this->ok_);
        }
    }

    template<typename U>
        requires (::std::same_as<U, value_type>)
    [[nodiscard]]
    constexpr auto value_or(U const&& val) const&& noexcept -> value_type const&& {
        if (this->has_value() == false) {
            return ::std::move(val);
        } else {
            return ::std::move(this->ok_);
        }
    }
};

namespace details {

struct nullopt_t_ {};

}

template<typename T>
using optional = ::exception::expected<T, ::exception::details::nullopt_t_>;

using nullopt_t = ::exception::unexpected<::exception::details::nullopt_t_>;

namespace details {

template<typename T>
constexpr bool is_expected_ = false;

template<typename Ok, typename Fail>
constexpr bool is_expected_<expected<Ok, Fail>> = true;

template<typename T>
constexpr bool is_optional_ = false;

template<typename T>
constexpr bool is_optional_<optional<T>> = true;

} // namespace details

template<typename T>
concept is_expected = ::exception::details::is_expected_<::std::remove_cvref_t<T>>;

template<typename T>
concept is_optional = ::exception::details::is_optional_<::std::remove_cvref_t<T>>;

} // namespace exception
