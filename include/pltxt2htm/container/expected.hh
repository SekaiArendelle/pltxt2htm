/**
 * @file expected.hh
 * @brief Lightweight Expected container for pltxt2htm.
 */

#pragma once

#include <memory>
#include <utility>
#include <type_traits>
#include <concepts>

#include "../details/push_macro.hh"

namespace pltxt2htm::container {

template<typename T>
struct Unexpected {
#if __has_cpp_attribute(msvc::no_unique_address)
    [[msvc::no_unique_address]]
#elif __has_cpp_attribute(no_unique_address)
    [[no_unique_address]]
#endif
    T value{};
};

namespace details {

template<typename T>
constexpr bool is_unexpected_v = false;

template<typename T>
constexpr bool is_unexpected_v<::pltxt2htm::container::Unexpected<T>> = true;

} // namespace details

template<typename T>
concept is_unexpected = details::is_unexpected_v<::std::remove_cvref_t<T>>;

template<typename Ok, typename Fail>
class Expected {
    static_assert(!::std::is_reference_v<Ok>);
    static_assert(!::std::is_function_v<Ok>);
    static_assert(!::pltxt2htm::container::is_unexpected<Ok>);

public:
    using value_type = ::std::remove_cvref_t<Ok>;
    using error_type = ::std::remove_cvref_t<Fail>;
    using unexpected_type = ::pltxt2htm::container::Unexpected<Fail>;
    template<typename T>
    using rebind = ::pltxt2htm::container::Expected<T, Fail>;

private:
    union {
        value_type value_storage;
        error_type error_storage;
    };

    bool contains_value;

public:
    constexpr Expected() noexcept(::std::is_nothrow_default_constructible_v<Ok>)
        requires (::std::is_default_constructible_v<Ok>)
        : value_storage(),
          contains_value{true} {
    }

    constexpr Expected(Ok const& ok) noexcept(::std::is_nothrow_copy_constructible_v<Ok>)
        requires (::std::is_copy_constructible_v<Ok>)
        : contains_value{true} {
        ::std::construct_at(::std::addressof(this->value_storage), ok);
    }

    constexpr Expected(Ok&& ok) noexcept(::std::is_nothrow_move_constructible_v<Ok>)
        requires (::std::is_move_constructible_v<Ok>)
        : contains_value{true} {
        ::std::construct_at(::std::addressof(this->value_storage), ::std::move(ok));
    }

    constexpr Expected(Unexpected<Fail> const& fail) noexcept(::std::is_nothrow_copy_constructible_v<Fail>)
        requires (::std::is_copy_constructible_v<Fail>)
        : contains_value{false} {
        ::std::construct_at(::std::addressof(this->error_storage), fail.value);
    }

    constexpr Expected(Unexpected<Fail>&& fail) noexcept(::std::is_nothrow_move_constructible_v<Fail>)
        requires (::std::is_move_constructible_v<Fail>)
        : contains_value{false} {
        ::std::construct_at(::std::addressof(this->error_storage), ::std::move(fail.value));
    }

    constexpr Expected(Expected<Ok, Fail> const& other) noexcept(::std::is_nothrow_copy_constructible_v<Ok> &&
                                                                 ::std::is_nothrow_copy_constructible_v<Fail>)
        : contains_value(other.contains_value) {
        if (this->has_value()) {
            ::std::construct_at(::std::addressof(this->value_storage), other.value_storage);
        }
        else {
            ::std::construct_at(::std::addressof(this->error_storage), other.error_storage);
        }
    }

    constexpr Expected(Expected<Ok, Fail>&& other) noexcept(::std::is_nothrow_move_constructible_v<Ok> &&
                                                            ::std::is_nothrow_move_constructible_v<Fail>)
        : contains_value(::std::move(other.contains_value)) {
        if (this->has_value()) {
            ::std::construct_at(::std::addressof(this->value_storage), ::std::move(other.value_storage));
        }
        else {
            ::std::construct_at(::std::addressof(this->error_storage), ::std::move(other.error_storage));
        }
    }

    constexpr ~Expected() noexcept = default;

    constexpr ~Expected() noexcept
        requires ((!::std::is_trivially_destructible_v<Ok>) || (!::std::is_trivially_destructible_v<Fail>))
    {
        if (this->has_value()) {
            ::std::destroy_at(::std::addressof(this->value_storage));
        }
        else {
            ::std::destroy_at(::std::addressof(this->error_storage));
        }
    }

    template<typename T>
        requires (::std::same_as<::std::remove_cvref_t<T>, Ok> &&
                  (::std::is_copy_assignable_v<T> || ::std::is_move_assignable_v<T>))
    constexpr auto&& operator=(this Expected<Ok, Fail>& self, T&& ok) noexcept {
        if (self.has_value()) {
            self.value_storage = ::std::forward<T>(ok);
        }
        else {
            ::std::destroy_at(::std::addressof(self.error_storage));
            ::std::construct_at(::std::addressof(self.value_storage), ::std::forward<T>(ok));
            self.contains_value = true;
        }
        return self;
    }

    constexpr auto&& operator=(this Expected<Ok, Fail>& self, unexpected_type const& fail) noexcept
        requires (::std::is_copy_assignable_v<error_type> && ::std::is_copy_constructible_v<error_type>)
    {
        if (self.has_value()) {
            ::std::destroy_at(::std::addressof(self.value_storage));
            ::std::construct_at(::std::addressof(self.error_storage), fail.value);
            self.contains_value = false;
        }
        else {
            self.error_storage = fail.value;
        }
        return self;
    }

    constexpr auto&& operator=(this Expected<Ok, Fail>& self, unexpected_type&& fail) noexcept
        requires (::std::is_move_assignable_v<error_type> && ::std::is_move_constructible_v<error_type>)
    {
        if (self.has_value()) {
            ::std::destroy_at(::std::addressof(self.value_storage));
            ::std::construct_at(::std::addressof(self.error_storage), ::std::move(fail.value));
            self.contains_value = false;
        }
        else {
            self.error_storage = ::std::move(fail.value);
        }
        return self;
    }

    constexpr auto&& operator=(this Expected<Ok, Fail>& self,
                               ::pltxt2htm::container::Expected<Ok, Fail> const& other) noexcept {
        ::pltxt2htm::container::Expected<Ok, Fail> tmp(other);
        tmp.swap(self);
        return self;
    }

    constexpr auto&& operator=(this Expected<Ok, Fail>& self, Expected<Ok, Fail>&& other) noexcept {
        self.swap(other);
        return self;
    }

    constexpr void swap(this Expected<Ok, Fail>& self, Expected<Ok, Fail>& other) noexcept
        requires (::std::is_move_assignable_v<value_type> && ::std::is_move_assignable_v<error_type> &&
                  ::std::is_move_constructible_v<value_type> && ::std::is_move_constructible_v<error_type>)
    {
        if (::std::addressof(self) == ::std::addressof(other)) {
            return;
        }
        if (self.has_value()) {
            if (other.has_value()) {
                Ok tmp{::std::move(self.value_storage)};
                self.value_storage = ::std::move(other.value_storage);
                other.value_storage = ::std::move(tmp);
            }
            else {
                Ok tmp{::std::move(self.value_storage)};
                ::std::destroy_at(::std::addressof(self.value_storage));
                ::std::construct_at(::std::addressof(self.error_storage), ::std::move(other.error_storage));
                ::std::destroy_at(::std::addressof(other.error_storage));
                ::std::construct_at(::std::addressof(other.value_storage), ::std::move(tmp));
                self.contains_value = false;
                other.contains_value = true;
            }
        }
        else {
            if (other.has_value()) {
                Fail tmp{::std::move(self.error_storage)};
                ::std::destroy_at(::std::addressof(self.error_storage));
                ::std::construct_at(::std::addressof(self.value_storage), ::std::move(other.value_storage));
                ::std::destroy_at(::std::addressof(other.value_storage));
                ::std::construct_at(::std::addressof(other.error_storage), ::std::move(tmp));
                self.contains_value = true;
                other.contains_value = false;
            }
            else {
                Fail tmp{::std::move(self.error_storage)};
                self.error_storage = ::std::move(other.error_storage);
                other.error_storage = ::std::move(tmp);
            }
        }
    }

    [[nodiscard]]
    constexpr auto has_value(this Expected<Ok, Fail> const& self) noexcept -> bool {
        return self.contains_value;
    }

    /**
     * @brief Get the contained value, terminating when the Expected contains an error.
     */
    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto value(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.has_value(), u8"expected does not contain a value");
        return ::std::forward_like<decltype(self)>(self.value_storage);
    }

    /**
     * @brief get the error value from an Expected
     */
    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto error(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.has_value() == false, u8"expected does not contain an error");
        return ::std::forward_like<decltype(self)>(self.error_storage);
    }

    /**
     * @brief Get the contained value, or the supplied fallback when the Expected contains an error.
     * @note Implicit conversion of @p val is not allowed.
     */
    template<typename U>
        requires (::std::same_as<U, value_type>)
    [[nodiscard]]
    constexpr auto value_or(this Expected<Ok, Fail>& self, U& val) noexcept -> value_type& {
        if (self.has_value() == false) {
            return val;
        }
        return self.value_storage;
    }

    template<typename U>
        requires (::std::same_as<U, value_type>)
    [[nodiscard]]
    constexpr auto value_or(this Expected<Ok, Fail> const& self, U const& val) noexcept -> value_type const& {
        if (self.has_value() == false) {
            return val;
        }
        return self.value_storage;
    }

    template<typename U>
        requires (::std::same_as<U, value_type>)
    [[nodiscard]]
    constexpr auto value_or(this Expected<Ok, Fail>&& self, U&& val) noexcept -> value_type&& {
        if (self.has_value() == false) {
            return ::std::move(val);
        }
        return ::std::move(self.value_storage);
    }

    [[nodiscard]]
    constexpr bool operator==(this Expected const& self, Expected const& rhs) noexcept
        requires (::std::equality_comparable<Ok> && ::std::equality_comparable<Fail>)
    {
        if (self.has_value() != rhs.has_value()) {
            return false;
        }
        if (self.has_value()) {
            return self.value_storage == rhs.value_storage;
        }
        return self.error_storage == rhs.error_storage;
    }

    [[nodiscard]]
    constexpr bool operator==(this Expected const& self, value_type const& rhs) noexcept
        requires ::std::equality_comparable<Ok>
    {
        return self.has_value() && self.value_storage == rhs;
    }

    [[nodiscard]]
    constexpr bool operator==(this Expected const& self, Unexpected<Fail> const& rhs) noexcept
        requires ::std::equality_comparable<Fail>
    {
        return !self.has_value() && self.error_storage == rhs.value;
    }
};

namespace details {

template<typename T>
constexpr bool is_expected_v = false;

template<typename Ok, typename Fail>
constexpr bool is_expected_v<Expected<Ok, Fail>> = true;

} // namespace details

template<typename T>
concept is_expected = details::is_expected_v<::std::remove_cvref_t<T>>;

} // namespace pltxt2htm::container

#include "../details/pop_macro.hh"
