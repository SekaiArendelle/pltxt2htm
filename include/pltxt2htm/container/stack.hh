/**
 * @file stack.hh
 * @brief LIFO container adapter using pltxt2htm contract checks.
 */

#pragma once

#include <compare>
#include <concepts>
#include <ranges>
#include <utility>

#include <fast_io/fast_io_dsal/deque.h>

#include "../contracts.hh"
#include "../details/push_macro.hh"

namespace pltxt2htm::container {

/**
 * @brief Last-in, first-out container adapter.
 * @tparam T Element type.
 * @tparam Container Underlying sequence container.
 */
template<typename T, typename Container = ::fast_io::deque<T>>
class Stack {
public:
    using container_type = Container;
    using value_type = typename container_type::value_type;
    using size_type = typename container_type::size_type;
    using reference = typename container_type::reference;
    using const_reference = typename container_type::const_reference;

    static_assert(::std::same_as<T, value_type>);

    container_type container{};

private:
    constexpr void remove_top(this Stack& self) noexcept {
        if constexpr (requires { self.container.pop_back(); }) {
            if constexpr (requires { self.container.pop_back_unchecked(); }) {
                self.container.pop_back_unchecked();
            }
            else {
                self.container.pop_back();
            }
        }
        else if constexpr (requires { self.container.pop_front_unchecked(); }) {
            self.container.pop_front_unchecked();
        }
        else {
            self.container.pop_front();
        }
    }

public:
    constexpr Stack() noexcept = default;

    template<::std::ranges::range Range>
    constexpr explicit Stack(::fast_io::freestanding::from_range_t, Range&& range)
        : container(::fast_io::freestanding::from_range, ::std::forward<Range>(range)) {
    }

    [[nodiscard]]
    constexpr auto get_container(this Stack& self) noexcept -> container_type& {
        return self.container;
    }

    [[nodiscard]]
    constexpr auto get_container(this Stack const& self) noexcept -> container_type const& {
        return self.container;
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto top(this Stack& self) noexcept -> reference {
        pltxt2htm_assert(self.empty() == false, u8"Accessing top but Stack is empty");
        if constexpr (requires {
                          { self.container.back_unchecked() } -> ::std::same_as<reference>;
                      }) {
            return self.container.back_unchecked();
        }
        else if constexpr (requires {
                               { self.container.back() } -> ::std::same_as<reference>;
                           }) {
            return self.container.back();
        }
        else if constexpr (requires {
                               { self.container.front_unchecked() } -> ::std::same_as<reference>;
                           }) {
            return self.container.front_unchecked();
        }
        else {
            return self.container.front();
        }
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto top(this Stack const& self) noexcept -> const_reference {
        pltxt2htm_assert(self.empty() == false, u8"Accessing top but Stack is empty");
        if constexpr (requires {
                          { self.container.back_unchecked() } -> ::std::same_as<const_reference>;
                      }) {
            return self.container.back_unchecked();
        }
        else if constexpr (requires {
                               { self.container.back() } -> ::std::same_as<const_reference>;
                           }) {
            return self.container.back();
        }
        else if constexpr (requires {
                               { self.container.front_unchecked() } -> ::std::same_as<const_reference>;
                           }) {
            return self.container.front_unchecked();
        }
        else {
            return self.container.front();
        }
    }

    [[nodiscard]]
    constexpr auto empty(this Stack const& self) noexcept -> bool {
        if constexpr (requires {
                          { self.container.is_empty() } -> ::std::convertible_to<bool>;
                      }) {
            return self.container.is_empty();
        }
        else {
            return self.container.empty();
        }
    }

    [[nodiscard]]
    constexpr auto is_empty(this Stack const& self) noexcept -> bool {
        return self.empty();
    }

    [[nodiscard]]
    constexpr auto size(this Stack const& self) noexcept -> size_type
        requires requires { self.container.size(); }
    {
        return self.container.size();
    }

    constexpr void push(this Stack& self, value_type const& value) {
        if constexpr (requires {
                          self.container.push_back(value);
                          { self.container.back() } -> ::std::same_as<reference>;
                      }) {
            self.container.push_back(value);
        }
        else {
            self.container.push_front(value);
        }
    }

    constexpr void push(this Stack& self, value_type&& value) {
        if constexpr (requires {
                          self.container.push_back(::std::move(value));
                          { self.container.back() } -> ::std::same_as<reference>;
                      }) {
            self.container.push_back(::std::move(value));
        }
        else {
            self.container.push_front(::std::move(value));
        }
    }

    constexpr void reserve(this Stack& self, size_type new_capacity) noexcept
        requires requires { self.container.reserve(new_capacity); }
    {
        self.container.reserve(new_capacity);
    }

    template<typename... Args>
        requires ::std::constructible_from<value_type, Args...>
    constexpr auto emplace(this Stack& self, Args&&... args) noexcept -> reference {
        if constexpr (requires {
                          self.container.emplace_back(::std::forward<Args>(args)...);
                          { self.container.back() } -> ::std::same_as<reference>;
                      }) {
            return self.container.emplace_back(::std::forward<Args>(args)...);
        }
        else {
            return self.container.emplace_front(::std::forward<Args>(args)...);
        }
    }

    template<::std::ranges::range Range>
    constexpr void push_range(this Stack& self, Range&& range) {
        if constexpr (requires { self.container.append_range(::std::forward<Range>(range)); }) {
            self.container.append_range(::std::forward<Range>(range));
        }
        else {
            self.container.prepend_range(::std::forward<Range>(range));
        }
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void pop(this Stack& self) noexcept {
        pltxt2htm_assert(self.empty() == false, u8"Popping but Stack is empty");
        self.remove_top();
    }

    template<::pltxt2htm::Contracts ndebug>
        requires ::std::move_constructible<value_type>
    constexpr auto pop_element(this Stack& self) noexcept -> value_type {
        value_type element{::std::move(self.template top<ndebug>())};
        self.remove_top();
        return element;
    }

    constexpr void clear(this Stack& self) noexcept {
        self.container.clear();
    }

    constexpr void clear_destroy(this Stack& self) noexcept
        requires requires { self.container.clear_destroy(); }
    {
        self.container.clear_destroy();
    }

    constexpr void swap(this Stack& self, Stack& other) noexcept {
        self.container.swap(other.container);
    }

    [[nodiscard]]
    constexpr auto operator==(this Stack const& self, Stack const& other) noexcept -> bool {
        return self.container == other.container;
    }

    [[nodiscard]]
    constexpr auto operator<=>(this Stack const& self, Stack const& other) noexcept {
        return self.container <=> other.container;
    }
};

template<typename T, typename Container>
constexpr void swap(Stack<T, Container>& left, Stack<T, Container>& right) noexcept {
    left.swap(right);
}

} // namespace pltxt2htm::container

#include "../details/pop_macro.hh"
