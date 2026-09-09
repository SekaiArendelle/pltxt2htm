/**
 * @file optional.hh
 * @brief Lightweight Optional container for pltxt2htm.
 */

#pragma once

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

#include "non_zero.hh"
#include "../details/concepts.hh"
#include "../details/push_macro.hh"

namespace pltxt2htm::container {

struct NulloptType {
    [[nodiscard]]
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

    constexpr auto operator=(this OptionalStorage<T>& self,
                             OptionalStorage<T> const& other) noexcept(::std::is_nothrow_copy_constructible_v<T> &&
                                                                       ::std::is_nothrow_copy_assignable_v<T>)
        -> OptionalStorage<T>&
        requires (::std::is_copy_constructible_v<T> && ::std::is_copy_assignable_v<T>)
    {
        // Copy-and-swap would additionally require T to be move constructible and move assignable,
        // even though ordinary optional copy assignment needs only copying.
        if (other.has_value()) {
            if (self.has_value()) {
                self.value_storage = other.value_storage;
                return self;
            }
            ::std::construct_at(::std::addressof(self.value_storage), other.value_storage);
            self.contains_value = true;
            return self;
        }
        self.reset();
        return self;
    }

    constexpr auto operator=(this OptionalStorage<T>& self, OptionalStorage<T>&& other) noexcept -> OptionalStorage<T>&
        requires (::std::is_move_constructible_v<T> && ::std::is_move_assignable_v<T>)
    {
        self.swap(other);
        return self;
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
class OptionalStorage<T&> {
    T* value_storage;

    template<typename U>
    [[nodiscard]]
    static constexpr auto reference_address(U&& value) noexcept(::std::is_nothrow_constructible_v<T&, U>) -> T* {
        T& reference(::std::forward<U>(value));
        return ::std::addressof(reference);
    }

public:
    constexpr OptionalStorage(NulloptType) noexcept
        : value_storage{nullptr} {
    }

    template<typename U>
        requires (::std::is_constructible_v<T&, U> && !::pltxt2htm::details::reference_constructs_from_temporary<T&, U>)
    constexpr OptionalStorage(U&& value) noexcept(::std::is_nothrow_constructible_v<T&, U>)
        : value_storage{reference_address(::std::forward<U>(value))} {
    }

    constexpr void reset(this OptionalStorage& self) noexcept {
        self.value_storage = nullptr;
    }

    constexpr void swap(this OptionalStorage& self, OptionalStorage& other) noexcept {
        T* const tmp{self.value_storage};
        self.value_storage = other.value_storage;
        other.value_storage = tmp;
    }

    [[nodiscard]]
    constexpr auto has_value(this OptionalStorage const& self) noexcept -> bool {
        return self.value_storage != nullptr;
    }

    [[nodiscard]]
    constexpr auto value(this OptionalStorage const& self) noexcept -> T& {
        return *self.value_storage;
    }
};

template<is_non_zero_unsigned_integer T>
class OptionalStorage<NonZero<T>> {
    NonZero<T> value_storage;

public:
    constexpr OptionalStorage(NulloptType) noexcept
        : value_storage{} {
    }

    constexpr OptionalStorage(NonZero<T> const& value) noexcept
        : value_storage{value} {
    }

    constexpr OptionalStorage(NonZero<T>&& value) noexcept
        : value_storage{::std::move(value)} {
    }

    constexpr OptionalStorage(OptionalStorage const&) noexcept = default;

    constexpr OptionalStorage(OptionalStorage&&) noexcept = default;

    constexpr ~OptionalStorage() noexcept = default;

    constexpr auto operator=(this OptionalStorage& self, OptionalStorage const& other) noexcept -> OptionalStorage& {
        // NonZero stores only a trivially copyable integer, so moving needs no state exchange.
        self.value_storage.value_storage = other.value_storage.value_storage;
        return self;
    }

    template<typename U>
        requires (::std::same_as<::std::remove_cvref_t<U>, NonZero<T>> &&
                  (::std::is_copy_assignable_v<U> || ::std::is_move_assignable_v<U>))
    constexpr void assign(this OptionalStorage& self, U&& value) noexcept {
        self.value_storage.value_storage = value.value_storage;
    }

    constexpr void reset(this OptionalStorage& self) noexcept {
        self.value_storage.value_storage = 0;
    }

    constexpr void swap(this OptionalStorage& self, OptionalStorage& other) noexcept {
        T const tmp{self.value_storage.value_storage};
        self.value_storage.value_storage = other.value_storage.value_storage;
        other.value_storage.value_storage = tmp;
    }

    [[nodiscard]]
    constexpr auto has_value(this OptionalStorage const& self) noexcept -> bool {
        return self.value_storage.value_storage != 0;
    }

    [[nodiscard]]
    constexpr auto value(this auto&& self) noexcept -> decltype(auto) {
        return ::std::forward_like<decltype(self)>(self.value_storage);
    }
};

} // namespace details

template<typename T>
class Optional {
    static_assert(!::std::is_rvalue_reference_v<T>);
    static_assert(!::std::is_function_v<T>);
    // Keep owned storage unqualified: use Optional<T> const for read-only ownership or
    // Optional<T const&> for a read-only reference. A cv-qualified value complicates lifetime reuse
    // and produces an Optional that cannot be assigned or swapped.
    static_assert(::std::is_lvalue_reference_v<T> || ::std::same_as<T, ::std::remove_cv_t<T>>);

public:
    using value_type = ::std::remove_reference_t<T>;
    template<typename U>
    using rebind = ::pltxt2htm::container::Optional<U>;

private:
    using non_reference_value_type = ::std::conditional_t<::std::is_lvalue_reference_v<T>, NulloptType, value_type>;

    details::OptionalStorage<T> storage;

public:
    constexpr Optional(T const& value) noexcept(::std::is_nothrow_copy_constructible_v<T>)
        requires (!::std::is_reference_v<T> && ::std::is_copy_constructible_v<T>)
        : storage{value} {
    }

    constexpr Optional(T&& value) noexcept(::std::is_nothrow_move_constructible_v<T>)
        requires (!::std::is_reference_v<T> && ::std::is_move_constructible_v<T>)
        : storage{::std::move(value)} {
    }

    template<typename U>
        requires (::std::is_lvalue_reference_v<T> && !::std::same_as<::std::remove_cvref_t<U>, Optional<T>> &&
                  !::std::same_as<::std::remove_cvref_t<U>, NulloptType> && ::std::is_constructible_v<T, U> &&
                  !::pltxt2htm::details::reference_constructs_from_temporary<T, U>)
    constexpr explicit(!::std::is_convertible_v<U, T>)
        Optional(U&& value) noexcept(::std::is_nothrow_constructible_v<T, U>)
        : storage{::std::forward<U>(value)} {
    }

    template<typename U>
        requires (::std::is_lvalue_reference_v<T> && !::std::same_as<::std::remove_cvref_t<U>, Optional<T>> &&
                  !::std::same_as<::std::remove_cvref_t<U>, NulloptType> && ::std::is_constructible_v<T, U> &&
                  ::pltxt2htm::details::reference_constructs_from_temporary<T, U>)
    constexpr explicit(!::std::is_convertible_v<U, T>)
        Optional(U&&) noexcept(::std::is_nothrow_constructible_v<T, U>) = delete
#if __cpp_deleted_function >= 202403L
        ("binding a temporary to Optional<T&> would create a dangling reference")
#endif
            ;

    constexpr Optional(NulloptType nullopt_value) noexcept
        : storage{nullopt_value} {
    }

    template<typename U>
        requires (!::std::is_reference_v<T> && ::std::same_as<::std::remove_cvref_t<U>, T> &&
                  (::std::is_copy_assignable_v<U> || ::std::is_move_assignable_v<U>))
    constexpr auto&& operator=(this Optional<T>& self, U&& value) noexcept {
        self.storage.assign(::std::forward<U>(value));
        return self;
    }

    constexpr auto&& operator=(this Optional<T>& self, NulloptType) noexcept {
        self.storage.reset();
        return self;
    }

    template<typename U>
    constexpr auto operator=(this Optional<T>&&, U&&) noexcept -> Optional<T>& = delete;

    constexpr void swap(this Optional<T>& self, Optional<T>& other) noexcept
        requires (::std::is_lvalue_reference_v<T> ||
                  (::std::is_move_assignable_v<value_type> && ::std::is_move_constructible_v<value_type>))
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
        requires (!::std::is_reference_v<T>)
    [[nodiscard]]
    constexpr auto value(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.storage.has_value(), u8"optional does not contain a value");
        return ::std::forward_like<decltype(self)>(self.storage).value();
    }

    template<::pltxt2htm::Contracts ndebug>
        requires (::std::is_lvalue_reference_v<T>)
    [[nodiscard]]
    constexpr auto value(this Optional<T> const& self) noexcept -> T {
        pltxt2htm_assert(self.storage.has_value(), u8"optional does not contain a value");
        return self.storage.value();
    }

    template<typename U = non_reference_value_type>
        requires (!::std::is_reference_v<T> && ::std::is_copy_constructible_v<non_reference_value_type> &&
                  ::std::is_convertible_v<U &&, non_reference_value_type>)
    [[nodiscard]]
    constexpr auto value_or(this Optional<T> const& self, U&& value) noexcept(
        noexcept(static_cast<non_reference_value_type>(self.storage.value())) &&
        noexcept(static_cast<non_reference_value_type>(::std::forward<U>(value)))) -> non_reference_value_type {
        if (self.storage.has_value() == false) {
            return static_cast<non_reference_value_type>(::std::forward<U>(value));
        }
        return static_cast<non_reference_value_type>(self.storage.value());
    }

    template<typename U = non_reference_value_type>
        requires (!::std::is_reference_v<T> && ::std::is_move_constructible_v<non_reference_value_type> &&
                  ::std::is_convertible_v<U &&, non_reference_value_type>)
    [[nodiscard]]
    constexpr auto value_or(this Optional<T>&& self, U&& value) noexcept(
        noexcept(static_cast<non_reference_value_type>(::std::move(self.storage).value())) &&
        noexcept(static_cast<non_reference_value_type>(::std::forward<U>(value)))) -> non_reference_value_type {
        if (self.storage.has_value() == false) {
            return static_cast<non_reference_value_type>(::std::forward<U>(value));
        }
        return static_cast<non_reference_value_type>(::std::move(self.storage).value());
    }

    // Prevent rvalue calls from falling back to the const lvalue overload when moving is unsupported.
    template<typename U = non_reference_value_type>
        requires (!::std::is_reference_v<T> && !::std::is_move_constructible_v<non_reference_value_type>)
    constexpr auto value_or(this Optional<T>&&, U&&) -> non_reference_value_type = delete;

    template<typename U = ::std::remove_cv_t<value_type>>
        requires (::std::is_lvalue_reference_v<T> && ::std::is_object_v<value_type> && !::std::is_array_v<value_type> &&
                  ::std::is_constructible_v<::std::remove_cv_t<value_type>, T> &&
                  ::std::is_convertible_v<U, ::std::remove_cv_t<value_type>>)
    [[nodiscard]]
    constexpr auto value_or(this Optional<T> const& self, U&& value) {
        using result_type = ::std::remove_cv_t<value_type>;
        if (self.has_value()) {
            return static_cast<result_type>(self.storage.value());
        }
        return static_cast<result_type>(::std::forward<U>(value));
    }

    [[nodiscard]]
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

    [[nodiscard]]
    constexpr bool operator==(this Optional<T> const& self, value_type const& rhs) noexcept
        requires ::std::equality_comparable<T>
    {
        return self.has_value() && self.storage.value() == rhs;
    }

    [[nodiscard]]
    constexpr bool operator==(this Optional<T> const& self, NulloptType) noexcept {
        return self.has_value() == false;
    }
};

namespace details {

template<typename T>
constexpr bool is_optional_v = false;

template<typename T>
constexpr bool is_optional_v<Optional<T>> = true;

} // namespace details

template<typename T>
concept is_optional = details::is_optional_v<::std::remove_cvref_t<T>>;

} // namespace pltxt2htm::container

#include "../details/pop_macro.hh"
