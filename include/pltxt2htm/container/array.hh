/**
 * @file array.hh
 * @brief Fixed-size contiguous arrays for pltxt2htm.
 */

#pragma once

#include <algorithm>
#include <compare>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>

#include "../contracts.hh"
#include "../details/push_macro.hh"

namespace pltxt2htm::container {

/**
 * @brief A fixed-size contiguous array.
 * @tparam T Element type.
 * @tparam extent Number of elements. Zero-length arrays are not supported.
 */
template<typename T, ::std::size_t extent>
    requires (extent != 0)
class Array {
    static_assert(extent <= ::std::numeric_limits<::std::size_t>::max() / sizeof(T), "array size is too large");

public:
    using value_type = T;
    using size_type = ::std::size_t;
    using difference_type = ::std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = ::std::reverse_iterator<iterator>;
    using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;

    value_type element[extent];

    [[nodiscard]]
    static constexpr auto size() noexcept -> size_type {
        return extent;
    }

    [[nodiscard]]
    static constexpr auto size_bytes() noexcept -> size_type {
        return extent * sizeof(value_type);
    }

    [[nodiscard]]
    static constexpr auto max_size() noexcept -> size_type {
        return extent;
    }

    [[nodiscard]]
    static constexpr auto max_size_bytes() noexcept -> size_type {
        return extent * sizeof(value_type);
    }

    [[nodiscard]]
    constexpr auto data(this Array& self) noexcept -> pointer {
        return self.element;
    }

    [[nodiscard]]
    constexpr auto data(this Array const& self) noexcept -> const_pointer {
        return self.element;
    }

    [[nodiscard]]
    constexpr auto begin(this Array& self) noexcept -> iterator {
        return self.element;
    }

    [[nodiscard]]
    constexpr auto begin(this Array const& self) noexcept -> const_iterator {
        return self.element;
    }

    [[nodiscard]]
    constexpr auto cbegin(this Array const& self) noexcept -> const_iterator {
        return self.element;
    }

    [[nodiscard]]
    constexpr auto end(this Array& self) noexcept -> iterator {
        return self.element + extent;
    }

    [[nodiscard]]
    constexpr auto end(this Array const& self) noexcept -> const_iterator {
        return self.element + extent;
    }

    [[nodiscard]]
    constexpr auto cend(this Array const& self) noexcept -> const_iterator {
        return self.element + extent;
    }

    [[nodiscard]]
    constexpr auto rbegin(this Array& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto rbegin(this Array const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto crbegin(this Array const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto rend(this Array& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto rend(this Array const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto crend(this Array const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.begin()};
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto index(this Array& self, size_type position) noexcept -> reference {
        pltxt2htm_assert(position < extent, u8"Index of array out of bound");
        return self.element[position];
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto index(this Array const& self, size_type position) noexcept -> const_reference {
        pltxt2htm_assert(position < extent, u8"Index of array out of bound");
        return self.element[position];
    }

    constexpr auto operator[](this Array&, size_type) noexcept -> reference = delete
#if __cpp_deleted_function >= 202403L
        ("operator[] is deleted; use index() instead for bounds-checked access")
#endif
        ;

    constexpr auto operator[](this Array const&, size_type) noexcept -> const_reference = delete
#if __cpp_deleted_function >= 202403L
        ("operator[] is deleted; use index() instead for bounds-checked access")
#endif
        ;

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto front(this Array& self) noexcept -> reference {
        pltxt2htm_assert(extent != 0, u8"front() called on an empty array");
        return self.element[0];
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto front(this Array const& self) noexcept -> const_reference {
        pltxt2htm_assert(extent != 0, u8"front() called on an empty array");
        return self.element[0];
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto back(this Array& self) noexcept -> reference {
        pltxt2htm_assert(extent != 0, u8"back() called on an empty array");
        return self.element[extent - 1];
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto back(this Array const& self) noexcept -> const_reference {
        pltxt2htm_assert(extent != 0, u8"back() called on an empty array");
        return self.element[extent - 1];
    }

    constexpr void fill(this Array& self, const_reference value) noexcept(::std::is_nothrow_copy_assignable_v<T>)
        requires ::std::is_copy_assignable_v<T>
    {
        ::std::fill_n(self.element, extent, value);
    }

    constexpr void swap(this Array& self, Array& other) noexcept(::std::is_nothrow_swappable_v<T>)
        requires ::std::is_swappable_v<T>
    {
        ::std::swap_ranges(self.begin(), self.end(), other.begin());
    }
};

template<typename T, typename... U>
    requires (::std::constructible_from<T, U> && ...)
Array(T, U...) -> Array<T, 1 + sizeof...(U)>;

template<typename T, ::std::size_t left_extent, ::std::size_t right_extent>
    requires (::std::equality_comparable<T> && left_extent != 0 && right_extent != 0)
[[nodiscard]]
constexpr auto operator==(Array<T, left_extent> const& left, Array<T, right_extent> const& right) -> bool {
    if constexpr (left_extent != right_extent) {
        return false;
    }
    else {
        return ::std::equal(left.begin(), left.end(), right.begin());
    }
}

template<typename T, ::std::size_t left_extent, ::std::size_t right_extent>
    requires (::std::three_way_comparable<T> && left_extent != 0 && right_extent != 0)
[[nodiscard]]
constexpr auto operator<=>(Array<T, left_extent> const& left, Array<T, right_extent> const& right) {
    return ::std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end(),
                                                    ::std::compare_three_way{});
}

template<typename T, ::std::size_t extent>
    requires (extent != 0 && ::std::is_swappable_v<T>)
constexpr void swap(Array<T, extent>& left, Array<T, extent>& right) noexcept(noexcept(left.swap(right))) {
    left.swap(right);
}

namespace details {

template<typename T, ::std::size_t extent, ::std::size_t... indexes>
constexpr auto to_array_lvalue_impl(T (&source)[extent], ::std::index_sequence<indexes...>)
    -> Array<::std::remove_cv_t<T>, extent> {
    return {{source[indexes]...}};
}

template<typename T, ::std::size_t extent, ::std::size_t... indexes>
constexpr auto to_array_rvalue_impl(T (&&source)[extent], ::std::index_sequence<indexes...>)
    -> Array<::std::remove_cv_t<T>, extent> {
    return {{::std::move(source[indexes])...}};
}

} // namespace details

template<typename T, ::std::size_t extent>
    requires (!::std::is_array_v<T>)
[[nodiscard]]
constexpr auto to_array(T (&source)[extent]) noexcept(::std::is_nothrow_copy_constructible_v<T>)
    -> Array<::std::remove_cv_t<T>, extent> {
    return ::pltxt2htm::container::details::to_array_lvalue_impl(source, ::std::make_index_sequence<extent>{});
}

template<typename T, ::std::size_t extent>
    requires (!::std::is_array_v<T>)
[[nodiscard]]
constexpr auto to_array(T (&&source)[extent]) noexcept(::std::is_nothrow_move_constructible_v<T>)
    -> Array<::std::remove_cv_t<T>, extent> {
    return ::pltxt2htm::container::details::to_array_rvalue_impl(::std::move(source),
                                                                 ::std::make_index_sequence<extent>{});
}

} // namespace pltxt2htm::container

#include "../details/pop_macro.hh"
