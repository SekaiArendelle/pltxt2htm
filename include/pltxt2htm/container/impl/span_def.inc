/**
 * @file span_def.inc
 * @brief Out-of-line contract-checked definitions for container::Span.
 */

#pragma once

#include "../../details/push_macro.hh"

namespace pltxt2htm::container {

template<typename T, ::std::size_t Extent>
constexpr Span<T, Extent>::Span(pointer pointer_, size_type size_) noexcept
    : data_pointer{pointer_},
      current_size{size_} {
    if constexpr (Extent != dynamic_extent) {
        pltxt2htm_assert(size_ == Extent, u8"Span size does not match its static extent");
    }
}

template<typename T, ::std::size_t Extent>
template<::std::contiguous_iterator First, ::std::sized_sentinel_for<First> Last>
    requires (::std::is_convertible_v<::std::remove_reference_t<::std::iter_reference_t<First>> (*)[], T (*)[]>)
constexpr Span<T, Extent>::Span(First first,
                                Last last) noexcept(noexcept(::std::declval<Last>() - ::std::declval<First>()) &&
                                                    noexcept(::std::declval<First>() == ::std::declval<Last>()))
    : data_pointer{::std::to_address(first)},
      current_size{first == last ? size_type{} : static_cast<size_type>(last - first)} {
    if constexpr (Extent != dynamic_extent) {
        pltxt2htm_assert(this->current_size == Extent, u8"Span size does not match its static extent");
    }
}

template<typename T, ::std::size_t Extent>
template<typename U, ::std::size_t OtherExtent>
    requires (::std::is_convertible_v<U (*)[], T (*)[]> &&
              (Extent == dynamic_extent || OtherExtent == dynamic_extent || Extent == OtherExtent))
constexpr Span<T, Extent>::Span(Span<U, OtherExtent> const& other) noexcept
    : data_pointer{other.data()},
      current_size{other.size()} {
    if constexpr (Extent != dynamic_extent && OtherExtent == dynamic_extent) {
        pltxt2htm_assert(other.size() == Extent, u8"Span size does not match its static extent");
    }
}

template<typename T, ::std::size_t Extent>
template<typename Range>
    requires (!is_span<Range> && !::std::is_array_v<::std::remove_cvref_t<Range>> &&
              ::std::ranges::contiguous_range<Range> && ::std::ranges::sized_range<Range> &&
              ::std::is_lvalue_reference_v<Range> &&
              ::std::is_convertible_v<::std::remove_reference_t<::std::ranges::range_reference_t<Range>> (*)[],
                                      T (*)[]>)
constexpr Span<T, Extent>::Span(Range&& range) noexcept(noexcept(::std::ranges::data(range)) &&
                                                        noexcept(::std::ranges::size(range)))
    : data_pointer{::std::ranges::data(range)},
      current_size{static_cast<size_type>(::std::ranges::size(range))} {
    if constexpr (Extent != dynamic_extent) {
        pltxt2htm_assert(this->current_size == Extent, u8"Span size does not match its static extent");
    }
}

template<typename T, ::std::size_t Extent>
constexpr auto Span<T, Extent>::operator[](this Span const& self, size_type position) noexcept -> reference {
    pltxt2htm_assert(position < self.current_size, u8"Index of Span out of bound");
    return self.data_pointer[position];
}

template<typename T, ::std::size_t Extent>
constexpr auto Span<T, Extent>::front(this Span const& self) noexcept -> reference {
    pltxt2htm_assert(self.is_empty() == false, u8"front() called on an empty Span");
    return self.data_pointer[0];
}

template<typename T, ::std::size_t Extent>
constexpr auto Span<T, Extent>::back(this Span const& self) noexcept -> reference {
    pltxt2htm_assert(self.is_empty() == false, u8"back() called on an empty Span");
    return self.data_pointer[self.current_size - 1];
}

template<typename T, ::std::size_t Extent>
constexpr auto Span<T, Extent>::subspan(this Span const& self, size_type position, size_type count) noexcept
    -> Span<T, dynamic_extent> {
    pltxt2htm_assert(position <= self.current_size, u8"Subview position of Span out of bound");

    size_type const remaining_size{self.current_size - position};
    if (count == dynamic_extent) {
        count = remaining_size;
    }
    else {
        pltxt2htm_assert(count <= remaining_size, u8"Subview size of Span out of bound");
    }

    return Span<T, dynamic_extent>{self.data_pointer + position, count};
}

template<typename T, ::std::size_t Extent>
constexpr auto Span<T, Extent>::first(this Span const& self, size_type count) noexcept -> Span<T, dynamic_extent> {
    pltxt2htm_assert(count <= self.current_size, u8"first() count exceeds Span size");
    return Span<T, dynamic_extent>{self.data_pointer, count};
}

template<typename T, ::std::size_t Extent>
constexpr auto Span<T, Extent>::last(this Span const& self, size_type count) noexcept -> Span<T, dynamic_extent> {
    pltxt2htm_assert(count <= self.current_size, u8"last() count exceeds Span size");
    return Span<T, dynamic_extent>{self.data_pointer + (self.current_size - count), count};
}

} // namespace pltxt2htm::container

#include "../../details/pop_macro.hh"