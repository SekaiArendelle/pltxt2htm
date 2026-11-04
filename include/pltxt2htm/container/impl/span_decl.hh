/**
 * @file span_decl.hh
 * @brief Declarations for container::Span.
 * @details Contract-checked members are declared here and defined in
 *          span_def.inc. Members defined inline that call those members also
 *          need span_def.inc in the same translation unit; include
 *          container/span.hh for the complete API.
 */

#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <limits>
#include <memory>
#include <ranges>
#include <type_traits>
#include <utility>

#include "../../details/concepts.hh"
#include "../../details/contracts.hh"

// windows.h defines min and max as function-like macros, which would expand
// inside std::numeric_limits<...>::max(). Keep them undefined in this header.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace pltxt2htm::container {

/**
 * @brief Extent value selecting a Span whose element count is known only at run time.
 */
inline constexpr ::std::size_t dynamic_extent = ::std::numeric_limits<::std::size_t>::max();

template<typename T, ::std::size_t Extent = dynamic_extent>
class Span;

namespace details {

template<typename T>
constexpr bool is_span_v = false;

template<typename T, ::std::size_t Extent>
constexpr bool is_span_v<Span<T, Extent>> = true;

} // namespace details

/**
 * @brief Tests whether a type is a container::Span specialization.
 */
template<typename T>
concept is_span = details::is_span_v<::std::remove_cvref_t<T>>;

/**
 * @brief A non-owning view over a contiguous sequence of objects.
 * @tparam T Element type; qualify it as `const` to model a read-only view.
 * @tparam Extent Number of elements, or dynamic_extent when the size is known only at run time.
 *
 * Span deliberately implements only the operations currently needed by pltxt2htm.
 * Unlike the owning containers, const-qualifying a Span does not propagate to the
 * referenced elements: a `Span<T> const` still yields `T&` on element access, which
 * matches `std::span` and keeps a mutable view usable after being handed around as a
 * const object. Use `Span<T const>` when the elements themselves must be read-only.
 */
template<typename T, ::std::size_t Extent>
class Span {
    static_assert(::std::is_object_v<T>, "Span element type must be an object type");

public:
    using element_type = T;
    using value_type = ::std::remove_cv_t<T>;
    using size_type = ::std::size_t;
    using difference_type = ::std::ptrdiff_t;
    using pointer = element_type*;
    using const_pointer = element_type const*;
    using reference = element_type&;
    using const_reference = element_type const&;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = ::std::reverse_iterator<iterator>;
    using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;

    static constexpr size_type extent = Extent;

private:
    pointer data_pointer{};
    size_type current_size{};

public:
    constexpr Span() noexcept
        requires (Extent == 0 || Extent == dynamic_extent)
    = default;

    constexpr explicit(Extent != dynamic_extent) Span(pointer pointer_, size_type size_) noexcept;

    /**
     * @brief Builds a view over the half-open iterator range `[first, last)`.
     * @pre `first` and `last` delimit one contiguous range, and `last - first`
     *      equals Extent when Extent is not dynamic_extent.
     */
    template<::std::contiguous_iterator First, ::std::sized_sentinel_for<First> Last>
        requires (::std::is_convertible_v<::std::remove_reference_t<::std::iter_reference_t<First>> (*)[], T (*)[]>)
    constexpr explicit(Extent != dynamic_extent)
        Span(First first, Last last) noexcept(noexcept(::std::declval<Last>() - ::std::declval<First>()) &&
                                              noexcept(::std::declval<First>() == ::std::declval<Last>()));

    /**
     * @brief Builds a view of `count` elements starting at the contiguous iterator `first`.
     * @pre The range starting at `first` has at least `count` elements, and
     *      `count` equals Extent when Extent is not dynamic_extent.
     */
    template<::std::contiguous_iterator First>
        requires (::std::is_convertible_v<::std::remove_reference_t<::std::iter_reference_t<First>> (*)[], T (*)[]>)
    constexpr explicit(Extent != dynamic_extent) Span(First first, size_type count) noexcept;

    /**
     * @brief Builds a view over a built-in array, including a string literal's terminator.
     */
    template<::std::size_t N>
        requires (Extent == dynamic_extent || Extent == N)
    constexpr Span(element_type (&array)[N]) noexcept
        : data_pointer{array},
          current_size{N} {
    }

    /**
     * @brief Converts a view of a more qualified element type.
     * @details Converting between different statically known extents is rejected;
     *          converting a dynamically sized view to a statically sized one is
     *          explicit and checks the size at run time.
     */
    template<typename U, ::std::size_t OtherExtent>
        requires (::std::is_convertible_v<U (*)[], T (*)[]> &&
                  (Extent == dynamic_extent || OtherExtent == dynamic_extent || Extent == OtherExtent))
    constexpr explicit(Extent != dynamic_extent && OtherExtent == dynamic_extent)
        Span(Span<U, OtherExtent> const& other) noexcept;

    /**
     * @brief Builds a view over an lvalue contiguous sized range.
     * @details Rvalue ranges are rejected so the view cannot outlive its storage;
     *          pass a pointer and a size instead when the referenced storage is
     *          guaranteed to outlive the view.
     */
    template<typename Range>
        requires (
            !is_span<Range> && !::std::is_array_v<::std::remove_cvref_t<Range>> &&
            ::std::ranges::contiguous_range<Range> && ::std::ranges::sized_range<Range> &&
            ::std::is_lvalue_reference_v<Range> &&
            ::std::is_convertible_v<::std::remove_reference_t<::std::ranges::range_reference_t<Range>> (*)[], T (*)[]>)
    constexpr explicit(Extent != dynamic_extent)
        Span(Range&& range) noexcept(noexcept(::std::ranges::data(range)) && noexcept(::std::ranges::size(range)));

    [[nodiscard]]
    constexpr auto data(this Span const& self) noexcept -> pointer {
        return self.data_pointer;
    }

    [[nodiscard]]
    constexpr auto size(this Span const& self) noexcept -> size_type {
        return self.current_size;
    }

    [[nodiscard]]
    constexpr auto size_bytes(this Span const& self) noexcept -> size_type {
        return self.current_size * sizeof(element_type);
    }

    [[nodiscard]]
    constexpr auto is_empty(this Span const& self) noexcept -> bool {
        return self.current_size == 0;
    }

    /**
     * @brief Tests whether the view has no elements, for downstream users only.
     * @return true when size() is zero.
     */
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto empty(this Span const&) noexcept -> bool = delete
    #if __cpp_deleted_function >= 202403L
        #if defined __clang__
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wc++26-extensions"
        #endif
        ("empty() is external-only; use is_empty() inside pltxt2htm")
        #if defined __clang__
            #pragma clang diagnostic pop
        #endif
    #endif
        ;
#else
    [[nodiscard]]
    constexpr auto empty(this Span const& self) noexcept -> bool {
        return self.is_empty();
    }
#endif

    [[nodiscard]]
    constexpr auto begin(this Span const& self) noexcept -> iterator {
        return self.data_pointer;
    }

    [[nodiscard]]
    constexpr auto cbegin(this Span const& self) noexcept -> const_iterator {
        return self.data_pointer;
    }

    [[nodiscard]]
    constexpr auto end(this Span const& self) noexcept -> iterator {
        if (self.data_pointer == nullptr) {
            return nullptr;
        }
        return self.data_pointer + self.current_size;
    }

    [[nodiscard]]
    constexpr auto cend(this Span const& self) noexcept -> const_iterator {
        return self.end();
    }

    [[nodiscard]]
    constexpr auto rbegin(this Span const& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto crbegin(this Span const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.cend()};
    }

    [[nodiscard]]
    constexpr auto rend(this Span const& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto crend(this Span const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.cbegin()};
    }

    /**
     * @brief Indexed element access, bounds-checked according to the build-wide
     * contract setting.
     * @param position Zero-based element position.
     * @return Reference to the requested element; a const Span still yields a
     *         mutable reference when T is not const-qualified.
     * @pre position is a valid element position.
     */
    [[nodiscard]]
    constexpr auto operator[](this Span const& self, size_type position) noexcept -> reference;

    /**
     * @return Reference to the first element.
     * @pre The view is not empty.
     */
    [[nodiscard]]
    constexpr auto front(this Span const& self) noexcept -> reference;

    /**
     * @return Reference to the last element.
     * @pre The view is not empty.
     */
    [[nodiscard]]
    constexpr auto back(this Span const& self) noexcept -> reference;

    /**
     * @brief Returns the subview of `count` elements starting at `position`.
     * @param position Offset of the first element of the subview.
     * @param count Number of elements, or dynamic_extent for the remainder.
     * @pre position <= size() and count <= size() - position.
     */
    [[nodiscard]]
    constexpr auto subspan(this Span const& self, size_type position, size_type count = dynamic_extent) noexcept
        -> Span<T, dynamic_extent>;

    /**
     * @brief Returns the first `count` elements.
     * @pre count <= size().
     */
    [[nodiscard]]
    constexpr auto first(this Span const& self, size_type count) noexcept -> Span<T, dynamic_extent>;

    /**
     * @brief Returns the last `count` elements.
     * @pre count <= size().
     */
    [[nodiscard]]
    constexpr auto last(this Span const& self, size_type count) noexcept -> Span<T, dynamic_extent>;

    /**
     * @brief Compares two views element by element.
     * @details Views of different statically known extents compare unequal without
     *          examining any element.
     */
    template<typename U, ::std::size_t RightExtent>
        requires (::std::same_as<::std::remove_cv_t<U>, value_type> && ::std::equality_comparable<value_type>)
    [[nodiscard]]
    constexpr auto operator==(this Span const& self, Span<U, RightExtent> const& right) noexcept(
        ::pltxt2htm::details::nothrow_equality_comparable<value_type>) -> bool {
        if constexpr (Extent != dynamic_extent && RightExtent != dynamic_extent && Extent != RightExtent) {
            return false;
        }
        else {
            return self.size() == right.size() && ::std::equal(self.begin(), self.end(), right.begin());
        }
    }
};

/**
 * @brief Deduces a dynamically sized view from an iterator range, or from a
 *        pointer and a size, keeping the element qualification.
 */
template<::std::contiguous_iterator First, typename Last>
Span(First, Last) -> Span<::std::remove_reference_t<::std::iter_reference_t<First>>>;

/**
 * @brief Deduces a statically sized view from a built-in array, including a
 *        string literal's terminator.
 */
template<typename T, ::std::size_t N>
Span(T (&)[N]) -> Span<T, N>;

/**
 * @brief Deduces a dynamically sized view from an lvalue contiguous sized range.
 */
template<typename Range>
    requires (!is_span<Range> && !::std::is_array_v<::std::remove_cvref_t<Range>> &&
              ::std::ranges::contiguous_range<Range> && ::std::ranges::sized_range<Range> &&
              ::std::is_lvalue_reference_v<Range>)
Span(Range&&) -> Span<::std::remove_reference_t<::std::ranges::range_reference_t<Range>>>;

} // namespace pltxt2htm::container

#pragma pop_macro("max")
#pragma pop_macro("min")