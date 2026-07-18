/**
 * @file call_stack.hh
 * @brief Fixed-capacity stack used by parser / optimizer / backend state machines.
 */

#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>
#include "../contracts.hh"
#include "push_macro.hh"

namespace pltxt2htm::details {

template<typename T, ::std::size_t Capacity = 1024>
class CallStack {
    static_assert(Capacity != 0);

public:
    using value_type = T;
    using size_type = ::std::size_t;

private:
    alignas(T) ::std::byte storage_[sizeof(T) * Capacity];
    size_type size_{};

public:
    constexpr CallStack() noexcept = default;
    constexpr CallStack(::pltxt2htm::details::CallStack<T, Capacity> const&) noexcept = delete;
    constexpr CallStack(::pltxt2htm::details::CallStack<T, Capacity>&&) noexcept = delete;

    constexpr ~CallStack() noexcept {
        while (size_ != 0) {
            this->destroy_back();
        }
    }

    constexpr auto operator=(::pltxt2htm::details::CallStack<T, Capacity> const&) noexcept
        -> ::pltxt2htm::details::CallStack<T, Capacity>& = delete;
    constexpr auto operator=(::pltxt2htm::details::CallStack<T, Capacity>&&) noexcept
        -> ::pltxt2htm::details::CallStack<T, Capacity>& = delete;

    [[nodiscard]]
    constexpr auto is_empty(this CallStack<T, Capacity> const& self) noexcept -> bool {
        return self.size_ == 0;
    }

    [[nodiscard]]
    constexpr auto empty(this CallStack<T, Capacity> const& self) noexcept -> bool {
        return self.is_empty();
    }

    [[nodiscard]]
    constexpr auto is_full(this CallStack<T, Capacity> const& self) noexcept -> bool {
        return self.size_ == Capacity;
    }

    [[nodiscard]]
    constexpr auto size(this CallStack<T, Capacity> const& self) noexcept -> size_type {
        return self.size_;
    }

    [[nodiscard]]
    static constexpr auto capacity() noexcept -> size_type {
        return Capacity;
    }

    [[nodiscard]]
    constexpr auto begin(this auto&& self) noexcept {
        return self.ptr_at(0);
    }

    [[nodiscard]]
    constexpr auto end(this auto&& self) noexcept {
        return self.ptr_at(self.size_);
    }

    [[nodiscard]]
    constexpr auto rbegin(this auto&& self) noexcept {
        return ::std::reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto rend(this auto&& self) noexcept {
        return ::std::reverse_iterator{self.begin()};
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto top(this auto&& self) noexcept -> decltype(auto) {
        pltxt2htm_assert(self.size_ != 0, u8"call stack is empty");
        return *self.ptr_at(self.size_ - 1);
    }

    template<typename... Args>
    constexpr auto try_emplace(Args&&... args) noexcept(::std::is_nothrow_constructible_v<T, Args...>) -> bool {
        if (size_ == Capacity) {
            return false;
        }
        ::std::construct_at(ptr_at(size_), ::std::forward<Args>(args)...);
        ++size_;
        return true;
    }

    template<::pltxt2htm::Contracts ndebug, typename... Args>
    constexpr void emplace(Args&&... args) noexcept(::std::is_nothrow_constructible_v<T, Args...>) {
        bool const pushed{this->try_emplace(::std::forward<Args>(args)...)};
        pltxt2htm_assert(pushed, u8"call stack capacity exceeded");
    }

    constexpr auto try_push(T&& value) noexcept(::std::is_nothrow_move_constructible_v<T>) -> bool {
        return this->try_emplace(::std::move(value));
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void push(T&& value) noexcept(::std::is_nothrow_move_constructible_v<T>) {
        this->template emplace<ndebug>(::std::move(value));
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void pop() noexcept {
        pltxt2htm_assert(size_ != 0, u8"call stack is empty");
        this->destroy_back();
    }

private:
    [[nodiscard]]
    constexpr auto ptr_at(this auto&& self, size_type const index) noexcept -> decltype(auto) {
        using Self = ::std::remove_reference_t<decltype(self)>;
        using MaybeConstT = ::std::conditional_t<::std::is_const_v<Self>, T const, T>;
        return reinterpret_cast<MaybeConstT*>(self.storage_ + sizeof(T) * index);
    }

    constexpr void destroy_back() noexcept {
        --size_;
        ::std::destroy_at(ptr_at(size_));
    }
};

} // namespace pltxt2htm::details

#include "pop_macro.hh"
