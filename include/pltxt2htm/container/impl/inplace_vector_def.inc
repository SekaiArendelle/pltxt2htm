/**
 * @file inplace_vector_def.inc
 * @brief Out-of-line contract-checked definitions for container::InplaceVector.
 */

#pragma once

// windows.h defines min and max as function-like macros, which would expand
// inside std::min(...) in the definitions below. Keep them undefined here.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

#include "../../details/push_macro.hh"

namespace pltxt2htm::container {

template<typename T, ::std::size_t extent>
constexpr auto InplaceVector<T, extent>::position_index(this InplaceVector const& self, const_iterator position,
                                                        bool allow_end) noexcept -> size_type {
    if constexpr (extent == 0) {
        pltxt2htm_assert(position == nullptr && allow_end, u8"Invalid InplaceVector iterator");
        return 0;
    }
    auto const offset = position - self.cbegin();
    pltxt2htm_assert(offset >= 0, u8"Invalid InplaceVector iterator");
    auto const index = static_cast<size_type>(offset);
    if (allow_end) {
        pltxt2htm_assert(index <= self.size(), u8"Invalid InplaceVector iterator");
    }
    else {
        pltxt2htm_assert(index < self.size(), u8"Invalid InplaceVector iterator");
    }
    return index;
}

template<typename T, ::std::size_t extent>
constexpr InplaceVector<T, extent>::InplaceVector(size_type count) noexcept(
    ::std::is_nothrow_default_constructible_v<value_type>)
    requires ::std::default_initializable<value_type>
{
    pltxt2htm_assert(count <= extent, u8"InplaceVector capacity exceeded");
    ConstructionGuard guard{*this};
    while (this->size() < count) {
        this->emplace_back();
    }
    guard.release();
}

template<typename T, ::std::size_t extent>
constexpr InplaceVector<T, extent>::InplaceVector(size_type count, const_reference value) noexcept(
    ::std::is_nothrow_copy_constructible_v<value_type>)
    requires ::std::copy_constructible<value_type>
{
    pltxt2htm_assert(count <= extent, u8"InplaceVector capacity exceeded");
    ConstructionGuard guard{*this};
    while (this->size() < count) {
        this->emplace_back(value);
    }
    guard.release();
}

template<typename T, ::std::size_t extent>
constexpr auto InplaceVector<T, extent>::operator=(this InplaceVector& self, InplaceVector const& other) noexcept(
    ::std::is_nothrow_copy_constructible_v<value_type>) -> InplaceVector&
    requires (extent != 0 && ::std::is_copy_constructible_v<value_type> && ::std::is_copy_assignable_v<value_type> &&
              !(::std::is_trivially_copy_constructible_v<value_type> &&
                ::std::is_trivially_copy_assignable_v<value_type> && ::std::is_trivially_destructible_v<value_type>))
{
#if defined(PLTXT2HTM_INTERNAL_USE)
    pltxt2htm_assert(::std::addressof(self) != ::std::addressof(other), u8"Self assignment of InplaceVector");
#else
    if (::std::addressof(self) == ::std::addressof(other)) {
        return self;
    }
#endif

    self.assign(other.begin(), other.end());
    return self;
}

template<typename T, ::std::size_t extent>
constexpr auto InplaceVector<T, extent>::operator=(this InplaceVector& self, InplaceVector&& other) noexcept(
    ::std::is_nothrow_move_assignable_v<value_type> && ::std::is_nothrow_move_constructible_v<value_type>)
    -> InplaceVector&
    requires (extent != 0 && ::std::is_move_constructible_v<value_type> && ::std::is_move_assignable_v<value_type> &&
              !(::std::is_trivially_move_constructible_v<value_type> &&
                ::std::is_trivially_move_assignable_v<value_type> && ::std::is_trivially_destructible_v<value_type>))
{
#if defined(PLTXT2HTM_INTERNAL_USE)
    pltxt2htm_assert(::std::addressof(self) != ::std::addressof(other), u8"Self assignment of InplaceVector");
#else
    if (::std::addressof(self) == ::std::addressof(other)) {
        return self;
    }
#endif

    self.clear();
    for (auto& value : other) {
        self.emplace_back(::std::move(value));
    }
    return self;
}

template<typename T, ::std::size_t extent>
constexpr void InplaceVector<T, extent>::assign(
    this InplaceVector& self, size_type count,
    const_reference value) noexcept(::std::is_nothrow_copy_constructible_v<value_type> &&
                                    ::std::is_nothrow_copy_assignable_v<value_type>)
    requires (::std::copy_constructible<value_type> && ::std::is_copy_assignable_v<value_type>)
{
    pltxt2htm_assert(count <= extent, u8"InplaceVector capacity exceeded");
    auto const common_size = ::std::min(self.size(), count);
    ::std::fill_n(self.begin(), common_size, value);
    if (count < self.size()) {
        self.destroy(self.iterator_at(count), self.end());
        self.set_size(count);
        return;
    }
    while (self.size() < count) {
        self.emplace_back(value);
    }
}

template<typename T, ::std::size_t extent>
constexpr void InplaceVector<T, extent>::reserve(size_type requested_capacity) noexcept {
    pltxt2htm_assert(requested_capacity <= extent, u8"InplaceVector capacity exceeded");
}

template<typename T, ::std::size_t extent>
constexpr void InplaceVector<T, extent>::resize(this InplaceVector& self, size_type new_size) noexcept(
    ::std::is_nothrow_default_constructible_v<value_type>)
    requires ::std::default_initializable<value_type>
{
    pltxt2htm_assert(new_size <= extent, u8"InplaceVector capacity exceeded");
    AppendRollbackGuard guard{self};
    while (self.size() > new_size) {
        self.pop_back();
    }
    while (self.size() < new_size) {
        self.emplace_back();
    }
    guard.release();
}

template<typename T, ::std::size_t extent>
constexpr void InplaceVector<T, extent>::resize(
    this InplaceVector& self, size_type new_size,
    const_reference value) noexcept(::std::is_nothrow_copy_constructible_v<value_type>)
    requires ::std::copy_constructible<value_type>
{
    pltxt2htm_assert(new_size <= extent, u8"InplaceVector capacity exceeded");
    if (new_size <= self.size()) {
        while (self.size() > new_size) {
            self.pop_back();
        }
        return;
    }
    AppendRollbackGuard guard{self};
    while (self.size() < new_size) {
        self.emplace_back(value);
    }
    guard.release();
}

template<typename T, ::std::size_t extent>
constexpr auto InplaceVector<T, extent>::operator[](this InplaceVector& self, size_type position) noexcept
    -> reference {
    pltxt2htm_assert(position < self.size(), u8"Index of InplaceVector out of bound");
    return self.data()[position];
}

template<typename T, ::std::size_t extent>
constexpr auto InplaceVector<T, extent>::operator[](this InplaceVector const& self, size_type position) noexcept
    -> const_reference {
    pltxt2htm_assert(position < self.size(), u8"Index of InplaceVector out of bound");
    return self.data()[position];
}

template<typename T, ::std::size_t extent>
constexpr auto InplaceVector<T, extent>::front(this InplaceVector& self) noexcept -> reference {
    pltxt2htm_assert(!self.is_empty(), u8"front() called on an empty InplaceVector");
    return self[0];
}

template<typename T, ::std::size_t extent>
constexpr auto InplaceVector<T, extent>::front(this InplaceVector const& self) noexcept -> const_reference {
    pltxt2htm_assert(!self.is_empty(), u8"front() called on an empty InplaceVector");
    return self[0];
}

template<typename T, ::std::size_t extent>
constexpr auto InplaceVector<T, extent>::back(this InplaceVector& self) noexcept -> reference {
    pltxt2htm_assert(!self.is_empty(), u8"back() called on an empty InplaceVector");
    return self[self.size() - 1];
}

template<typename T, ::std::size_t extent>
constexpr auto InplaceVector<T, extent>::back(this InplaceVector const& self) noexcept -> const_reference {
    pltxt2htm_assert(!self.is_empty(), u8"back() called on an empty InplaceVector");
    return self[self.size() - 1];
}

template<typename T, ::std::size_t extent>
template<typename... Args>
constexpr auto InplaceVector<T, extent>::emplace_back(this InplaceVector& self, Args&&... args) noexcept(
    ::std::is_nothrow_constructible_v<value_type, Args...>) -> reference
    requires ::std::constructible_from<value_type, Args...>
{
    pltxt2htm_assert(self.size() < extent, u8"InplaceVector capacity exceeded");
    if constexpr (extent == 0) {
        pltxt2htm_unreachable(u8"emplace_back on zero-capacity InplaceVector");
    }
    else {
        auto* const result = ::std::construct_at(self.iterator_at(self.size()), ::std::forward<Args>(args)...);
        self.increment_size();
        return *result;
    }
}

template<typename T, ::std::size_t extent>
template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
constexpr void InplaceVector<T, extent>::append(this InplaceVector& self, InputIterator first, Sentinel last) noexcept(
    is_nothrow_range_iteration<InputIterator, Sentinel>())
    requires ::std::constructible_from<value_type, ::std::iter_reference_t<InputIterator>>
{
    if constexpr (::std::sized_sentinel_for<Sentinel, InputIterator>) {
        auto const count = static_cast<size_type>(last - first);
        pltxt2htm_assert(count <= extent - self.size(), u8"InplaceVector capacity exceeded");
    }
    while (first != last) {
        self.emplace_back(*first);
        ++first;
    }
}

template<typename T, ::std::size_t extent>
constexpr void InplaceVector<T, extent>::pop_back(this InplaceVector& self) noexcept {
    pltxt2htm_assert(!self.is_empty(), u8"pop_back() called on an empty InplaceVector");
    self.decrement_size();
    self.destroy_at(self.iterator_at(self.size()));
}

template<typename T, ::std::size_t extent>
constexpr auto InplaceVector<T, extent>::insert(
    this InplaceVector& self, const_iterator position, size_type count,
    const_reference value) noexcept(::std::is_nothrow_copy_constructible_v<value_type> && is_nothrow_element_rotation())
    -> iterator
    requires (::std::copy_constructible<value_type> && ::std::permutable<iterator>)
{
    auto const index = self.position_index(position, true);
    pltxt2htm_assert(count <= extent - self.size(), u8"InplaceVector capacity exceeded");
    if (count == 0) {
        return self.iterator_at(index);
    }
    auto const old_size = self.size();
    while (self.size() < old_size + count) {
        self.emplace_back(value);
    }
    auto const result = self.iterator_at(index);
    ::std::rotate(result, self.iterator_at(old_size), self.end());
    return result;
}

template<typename T, ::std::size_t extent>
template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
constexpr auto InplaceVector<T, extent>::insert(
    this InplaceVector& self, const_iterator position, InputIterator first,
    Sentinel last) noexcept(is_nothrow_range_iteration<InputIterator, Sentinel>() && is_nothrow_element_rotation())
    -> iterator
    requires (::std::constructible_from<value_type, ::std::iter_reference_t<InputIterator>> &&
              ::std::permutable<iterator>)
{
    auto const index = self.position_index(position, true);
    auto const old_size = self.size();
    self.append(first, last);
    auto const result = self.iterator_at(index);
    if (self.size() == old_size) {
        return result;
    }
    ::std::rotate(result, self.iterator_at(old_size), self.end());
    return result;
}

template<typename T, ::std::size_t extent>
constexpr auto InplaceVector<T, extent>::erase(
    this InplaceVector& self, const_iterator first,
    const_iterator last) noexcept(::std::is_nothrow_move_assignable_v<value_type>) -> iterator
    requires ::std::is_move_assignable_v<value_type>
{
    auto const first_index = self.position_index(first, true);
    auto const last_index = self.position_index(last, true);
    pltxt2htm_assert(first_index <= last_index, u8"Invalid InplaceVector erase range");
    auto const result = self.iterator_at(first_index);
    auto const erased_count = last_index - first_index;
    if (erased_count == 0) {
        return result;
    }
    auto const new_end = ::std::move(self.iterator_at(last_index), self.end(), result);
    self.destroy(new_end, self.end());
    self.set_size(self.size() - erased_count);
    return result;
}

} // namespace pltxt2htm::container

#include "../../details/pop_macro.hh"

#pragma pop_macro("max")
#pragma pop_macro("min")
