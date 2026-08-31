/**
 * @file optional.hh
 * @brief Lightweight Optional container for pltxt2htm.
 */

#pragma once

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

#include "../details/push_macro.hh"

namespace pltxt2htm::container {

struct NulloptType {
    constexpr bool operator==(this NulloptType const&, NulloptType const&) noexcept = default;
};

inline constexpr auto nullopt = NulloptType{};

namespace details {

template<typename T>
class OptionalStorage {
    union {
        T value_storage;
    };

    bool contains_value;

public:
    constexpr OptionalStorage(NulloptType) noexcept
        : contains_value{false} {
    }

    constexpr OptionalStorage(T const& value) noexcept(::std::is_nothrow_copy_constructible_v<T>)
        requires (::std::is_copy_constructible_v<T>)
        : contains_value{true} {
        ::std::construct_at(::std::addressof(this->value_storage), value);
    }

    constexpr OptionalStorage(T&& value) noexcept(::std::is_nothrow_move_constructible_v<T>)
        requires (::std::is_move_constructible_v<T>)
        : contains_value{true} {
        ::std::construct_at(::std::addressof(this->value_storage), ::std::move(value));
    }

    constexpr OptionalStorage(OptionalStorage<T> const& other) noexcept(::std::is_nothrow_copy_constructible_v<T>)
        : contains_value{other.contains_value} {
        if (this->has_value()) {
            ::std::construct_at(::std::addressof(this->value_storage), other.value_storage);
        }
    }

    constexpr OptionalStorage(OptionalStorage<T>&& other) noexcept(::std::is_nothrow_move_constructible_v<T>)
        : contains_value{other.contains_value} {
        if (this->has_value()) {
            ::std::construct_at(::std::addressof(this->value_storage), ::std::move(other.value_storage));
        }
    }

    constexpr ~OptionalStorage() noexcept = default;

    constexpr ~OptionalStorage() noexcept
        requires (!::std::is_trivially_destructible_v<T>)
    {
        this->reset();
    }

    template<typename U>
        requires (::std::same_as<::std::remove_cvref_t<U>, T> &&
                  (::std::is_copy_assignable_v<U> || ::std::is_move_assignable_v<U>))
    constexpr void assign(this OptionalStorage<T>& self, U&& value) noexcept {
        if (self.has_value()) {
            self.value_storage = ::std::forward<U>(value);
            return;
        }
        ::std::construct_at(::std::addressof(self.value_storage), ::std::forward<U>(value));
        self.contains_value = true;
    }

    constexpr void reset(this OptionalStorage<T>& self) noexcept {
        if (self.has_value() == false) {
            return;
        }
        ::std::destroy_at(::std::addressof(self.value_storage));
        self.contains_value = false;
    }

    constexpr void swap(this OptionalStorage<T>& self, OptionalStorage<T>& other) noexcept
        requires (::std::is_move_assignable_v<T> && ::std::is_move_constructible_v<T>)
    {
        if (::std::addressof(self) == ::std::addressof(other)) {
            return;
        }
        if (self.has_value()) {
            if (other.has_value()) {
                T tmp{::std::move(self.value_storage)};
                self.value_storage = ::std::move(other.value_storage);
                other.value_storage = ::std::move(tmp);
                return;
            }
            ::std::construct_at(::std::addressof(other.value_storage), ::std::move(self.value_storage));
            ::std::destroy_at(::std::addressof(self.value_storage));
            self.contains_value = false;
            other.contains_value = true;
            return;
        }
        if (other.has_value()) {
            ::std::construct_at(::std::addressof(self.value_storage), ::std::move(other.value_storage));
            ::std::destroy_at(::std::addressof(other.value_storage));
            self.contains_value = true;
            other.contains_value = false;
        }
    }

    [[nodiscard]]
    constexpr auto has_value(this OptionalStorage<T> const& self) noexcept -> bool {
        return self.contains_value;
    }

    [[nodiscard]]
    constexpr auto value(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.value_storage);
    }
};

template<typename T>
constexpr bool is_optional_v = false;

} // namespace details

template<typename T>
class Optional {
    static_assert(!::std::is_reference_v<T>);
    static_assert(!::std::is_function_v<T>);

public:
    using value_type = ::std::remove_cvref_t<T>;
    template<typename U>
    using rebind = ::pltxt2htm::container::Optional<U>;

private:
    details::OptionalStorage<value_type> storage;

public:
    constexpr Optional(T const& value) noexcept(::std::is_nothrow_copy_constructible_v<T>)
        requires (::std::is_copy_constructible_v<T>)
        : storage{value} {
    }

    constexpr Optional(T&& value) noexcept(::std::is_nothrow_move_constructible_v<T>)
        requires (::std::is_move_constructible_v<T>)
        : storage{::std::move(value)} {
    }

    constexpr Optional(NulloptType nullopt_value) noexcept
        : storage{nullopt_value} {
    }

    constexpr Optional(Optional<T> const&) noexcept(::std::is_nothrow_copy_constructible_v<T>) = default;

    constexpr Optional(Optional<T>&&) noexcept(::std::is_nothrow_move_constructible_v<T>) = default;

    constexpr ~Optional() noexcept = default;

    template<typename U>
        requires (::std::same_as<::std::remove_cvref_t<U>, T> &&
                  (::std::is_copy_assignable_v<U> || ::std::is_move_assignable_v<U>))
    constexpr auto&& operator=(this Optional<T>& self, U&& value) noexcept {
        self.storage.assign(::std::forward<U>(value));
        return self;
    }

    constexpr auto&& operator=(this Optional<T>& self, NulloptType) noexcept {
        self.storage.reset();
        return self;
    }

    constexpr auto&& operator=(this Optional<T>& self, Optional<T> const& other) noexcept {
        Optional<T> tmp{other};
        tmp.swap(self);
        return self;
    }

    constexpr auto&& operator=(this Optional<T>& self, Optional<T>&& other) noexcept {
        self.swap(other);
        return self;
    }

    constexpr void swap(this Optional<T>& self, Optional<T>& other) noexcept
        requires (::std::is_move_assignable_v<value_type> && ::std::is_move_constructible_v<value_type>)
    {
        self.storage.swap(other.storage);
    }

    [[nodiscard]]
    constexpr auto has_value(this Optional<T> const& self) noexcept -> bool {
        return self.storage.has_value();
    }

    /**
     * @brief Get the contained value, terminating when the Optional is empty.
     */
    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto value(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.has_value(), u8"optional does not contain a value");
        return ::std::forward_like<decltype(self)>(self.storage).value();
    }

    template<typename U>
        requires (::std::same_as<U, value_type>)
    [[nodiscard]]
    constexpr auto value_or(this Optional<T>& self, U& value) noexcept -> value_type& {
        if (self.has_value() == false) {
            return value;
        }
        return self.storage.value();
    }

    template<typename U>
        requires (::std::same_as<U, value_type>)
    [[nodiscard]]
    constexpr auto value_or(this Optional<T> const& self, U const& value) noexcept -> value_type const& {
        if (self.has_value() == false) {
            return value;
        }
        return self.storage.value();
    }

    template<typename U>
        requires (::std::same_as<U, value_type>)
    [[nodiscard]]
    constexpr auto value_or(this Optional<T>&& self, U&& value) noexcept -> value_type&& {
        if (self.has_value() == false) {
            return ::std::move(value);
        }
        return ::std::move(self.storage).value();
    }

    constexpr bool operator==(this Optional<T> const& self, Optional<T> const& rhs) noexcept
        requires ::std::equality_comparable<T>
    {
        if (self.has_value() != rhs.has_value()) {
            return false;
        }
        if (self.has_value() == false) {
            return true;
        }
        return self.storage.value() == rhs.storage.value();
    }

    constexpr bool operator==(this Optional<T> const& self, value_type const& rhs) noexcept
        requires ::std::equality_comparable<T>
    {
        return self.has_value() && self.storage.value() == rhs;
    }

    constexpr bool operator==(this Optional<T> const& self, NulloptType) noexcept {
        return self.has_value() == false;
    }
};

namespace details {

template<typename T>
constexpr bool is_optional_v<Optional<T>> = true;

} // namespace details

template<typename T>
concept is_optional = details::is_optional_v<::std::remove_cvref_t<T>>;

} // namespace pltxt2htm::container

#include "../details/pop_macro.hh"
