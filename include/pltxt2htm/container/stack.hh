/**
 * @file stack.hh
 * @brief Minimal LIFO container using pltxt2htm contract checks.
 */

#pragma once

#include <concepts>
#include <type_traits>
#include <utility>

#include <fast_io/fast_io_dsal/deque.h>

#include "../contracts.hh"
#include "../details/push_macro.hh"

namespace pltxt2htm::container {

/**
 * @brief Last-in, first-out container used by pltxt2htm traversal code.
 * @tparam T Nothrow-movable element type.
 */
template<typename T>
    requires ::std::is_nothrow_move_constructible_v<T>
class Stack {
public:
    using value_type = T;
    using size_type = typename ::fast_io::deque<value_type>::size_type;
    using reference = value_type&;
    using const_reference = value_type const&;

private:
    ::fast_io::deque<value_type> container{};

public:
    constexpr Stack() noexcept = default;

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto top(this Stack& self) noexcept -> reference {
        pltxt2htm_assert(self.empty() == false, u8"Accessing top but Stack is empty");
        return self.container.back_unchecked();
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto top(this Stack const& self) noexcept -> const_reference {
        pltxt2htm_assert(self.empty() == false, u8"Accessing top but Stack is empty");
        return self.container.back_unchecked();
    }

    [[nodiscard]]
    constexpr auto empty(this Stack const& self) noexcept -> bool {
        return self.container.is_empty();
    }

    [[nodiscard]]
    constexpr auto size(this Stack const& self) noexcept -> size_type {
        return self.container.size();
    }

    constexpr void push(this Stack& self, value_type&& value) noexcept {
        self.container.push_back(::std::move(value));
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void pop(this Stack& self) noexcept {
        pltxt2htm_assert(self.empty() == false, u8"Popping but Stack is empty");
        self.container.pop_back_unchecked();
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto pop_element(this Stack& self) noexcept -> value_type {
        pltxt2htm_assert(self.empty() == false, u8"Popping but Stack is empty");
        value_type element{::std::move(self.container.back_unchecked())};
        self.container.pop_back_unchecked();
        return element;
    }

    template<typename Predicate>
        requires requires(Predicate& predicate, const_reference value) {
            { predicate(value) } noexcept -> ::std::convertible_to<bool>;
        }
    [[nodiscard]]
    constexpr auto contains_if(this Stack const& self, Predicate&& predicate) noexcept -> bool {
        for (auto const& value : self.container) {
            if (predicate(value)) {
                return true;
            }
        }
        return false;
    }
};

} // namespace pltxt2htm::container

#include "../details/pop_macro.hh"
