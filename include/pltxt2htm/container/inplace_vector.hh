/**
 * @file inplace_vector.hh
 * @brief Fixed-capacity contiguous vectors for pltxt2htm.
 */

#pragma once

#include <algorithm>
#include <compare>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <ranges>
#include <type_traits>
#include <utility>

#include "optional.hh"
#include "../contracts.hh"
#include "../details/push_macro.hh"

namespace pltxt2htm::container {

namespace details {

template<typename T, ::std::size_t extent>
union InplaceVectorNonzeroStorage {
    T elements[extent];

    constexpr InplaceVectorNonzeroStorage() noexcept {
        if consteval {
            // C++23 cannot portably activate this union array for non-trivial T.
            if constexpr (::std::is_trivial_v<T> && ::std::is_default_constructible_v<T>) {
                ::std::construct_at(::std::addressof(elements));
            }
        }
    }

    constexpr InplaceVectorNonzeroStorage(InplaceVectorNonzeroStorage const&) = default;

    constexpr InplaceVectorNonzeroStorage(InplaceVectorNonzeroStorage&&) = default;

    constexpr auto operator=(this InplaceVectorNonzeroStorage&, InplaceVectorNonzeroStorage const&)
        -> InplaceVectorNonzeroStorage& = default;

    constexpr auto operator=(this InplaceVectorNonzeroStorage&, InplaceVectorNonzeroStorage&&)
        -> InplaceVectorNonzeroStorage& = default;

    constexpr ~InplaceVectorNonzeroStorage() noexcept = default;

    constexpr ~InplaceVectorNonzeroStorage() noexcept
        requires (!::std::is_trivially_destructible_v<T>)
    {
    }
};

template<typename T>
struct EmptyInplaceVectorStorage {};

template<typename T, ::std::size_t extent>
using InplaceVectorStorage =
    ::std::conditional_t<extent == 0, EmptyInplaceVectorStorage<T>, InplaceVectorNonzeroStorage<T, extent>>;

template<::std::size_t extent>
struct InplaceVectorSize {
    ::std::size_t value{};
};

template<>
struct InplaceVectorSize<0> {
    static constexpr ::std::size_t value{};
};

} // namespace details

/**
 * @brief A contiguous vector whose elements are stored inside the vector object.
 * @tparam T Element type.
 * @tparam extent Maximum number of elements.
 *
 * Capacity never changes and no dynamic allocation is performed. Operations that
 * can exceed the fixed capacity use the project's contract policy. The `try_*`
 * operations report a full vector with an empty Optional instead.
 */
template<typename T, ::std::size_t extent>
class InplaceVector {
    static_assert(::std::is_object_v<T>);
    static_assert(::std::same_as<T, ::std::remove_cv_t<T>>);
    static_assert(extent <= ::std::numeric_limits<::std::size_t>::max() / sizeof(T),
                  "inplace vector capacity is too large");

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

private:
#if __has_cpp_attribute(msvc::no_unique_address)
    [[msvc::no_unique_address]]
#else
    [[no_unique_address]]
#endif
    details::InplaceVectorStorage<value_type, extent> storage;

#if __has_cpp_attribute(msvc::no_unique_address)
    [[msvc::no_unique_address]]
#else
    [[no_unique_address]]
#endif
    details::InplaceVectorSize<extent> size_storage;

    constexpr void set_size(size_type new_size) noexcept {
        if constexpr (extent != 0) {
            this->size_storage.value = new_size;
        }
    }

    constexpr void increment_size() noexcept {
        if constexpr (extent != 0) {
            ++this->size_storage.value;
        }
    }

    constexpr void decrement_size() noexcept {
        if constexpr (extent != 0) {
            --this->size_storage.value;
        }
    }

    [[nodiscard]]
    constexpr auto iterator_at(this InplaceVector& self, size_type index) noexcept -> iterator {
        if constexpr (extent == 0) {
            return nullptr;
        }
        else {
            return self.data() + index;
        }
    }

    [[nodiscard]]
    constexpr auto iterator_at(this InplaceVector const& self, size_type index) noexcept -> const_iterator {
        if constexpr (extent == 0) {
            return nullptr;
        }
        else {
            return self.data() + index;
        }
    }

    static constexpr void destroy_at(pointer position) noexcept {
        ::std::destroy_at(position);
    }

    static constexpr void destroy(iterator first, iterator last) noexcept {
        ::std::destroy(first, last);
    }

    class ConstructionGuard {
        InplaceVector* vector;

    public:
        constexpr explicit ConstructionGuard(InplaceVector& vector_) noexcept
            : vector{::std::addressof(vector_)} {
        }

        constexpr ~ConstructionGuard() noexcept {
            if (this->vector != nullptr) {
                this->vector->clear();
            }
        }

        constexpr void release(this ConstructionGuard& self) noexcept {
            self.vector = nullptr;
        }
    };

    class AppendRollbackGuard {
        InplaceVector* vector;
        size_type original_size;

    public:
        constexpr explicit AppendRollbackGuard(InplaceVector& vector_) noexcept
            : vector{::std::addressof(vector_)},
              original_size{vector_.size()} {
        }

        constexpr ~AppendRollbackGuard() noexcept {
            if (this->vector == nullptr) {
                return;
            }
            while (this->vector->size() > this->original_size) {
                this->vector->template pop_back<::pltxt2htm::Contracts::ignore>();
            }
        }

        constexpr void release(this AppendRollbackGuard& self) noexcept {
            self.vector = nullptr;
        }
    };

    template<::pltxt2htm::Contracts ndebug>
    constexpr void check_new_size(size_type new_size) const noexcept {
        pltxt2htm_assert(new_size <= extent, u8"InplaceVector capacity exceeded");
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void check_additional_size(size_type additional_size) const noexcept {
        pltxt2htm_assert(additional_size <= extent - this->size(), u8"InplaceVector capacity exceeded");
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto position_index(const_iterator position, bool allow_end) const noexcept -> size_type {
        if constexpr (extent == 0) {
            pltxt2htm_assert(position == nullptr && allow_end, u8"Invalid InplaceVector iterator");
            return 0;
        }
        auto const offset = position - this->cbegin();
        pltxt2htm_assert(offset >= 0, u8"Invalid InplaceVector iterator");
        auto const index = static_cast<size_type>(offset);
        if (allow_end) {
            pltxt2htm_assert(index <= this->size(), u8"Invalid InplaceVector iterator");
        }
        else {
            pltxt2htm_assert(index < this->size(), u8"Invalid InplaceVector iterator");
        }
        return index;
    }

public:
    constexpr InplaceVector() noexcept = default;

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    constexpr explicit InplaceVector(size_type count)
        requires ::std::default_initializable<value_type>
    {
        ConstructionGuard guard{*this};
        this->template check_new_size<ndebug>(count);
        while (this->size() < count) {
            (void)this->unchecked_emplace_back();
        }
        guard.release();
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    constexpr InplaceVector(size_type count, const_reference value)
        requires ::std::copy_constructible<value_type>
    {
        ConstructionGuard guard{*this};
        this->template check_new_size<ndebug>(count);
        while (this->size() < count) {
            (void)this->unchecked_emplace_back(value);
        }
        guard.release();
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, ::std::input_iterator InputIterator,
             ::std::sentinel_for<InputIterator> Sentinel>
        requires ::std::constructible_from<value_type, ::std::iter_reference_t<InputIterator>>
    constexpr InplaceVector(InputIterator first, Sentinel last) {
        ConstructionGuard guard{*this};
        this->template append<ndebug>(first, last);
        guard.release();
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    constexpr InplaceVector(::std::initializer_list<value_type> values)
        requires ::std::copy_constructible<value_type>
    {
        ConstructionGuard guard{*this};
        this->template append<ndebug>(values.begin(), values.end());
        guard.release();
    }

    constexpr InplaceVector(InplaceVector const&)
        requires (extent == 0 || ::std::is_trivially_copy_constructible_v<value_type>)
    = default;

    constexpr InplaceVector(InplaceVector const& other) noexcept(::std::is_nothrow_copy_constructible_v<value_type>)
        requires (extent != 0 && !::std::is_trivially_copy_constructible_v<value_type> &&
                  ::std::is_copy_constructible_v<value_type>)
    {
        ConstructionGuard guard{*this};
        for (auto const& value : other) {
            (void)this->unchecked_emplace_back(value);
        }
        guard.release();
    }

    constexpr InplaceVector(InplaceVector&&)
        requires (extent == 0 || ::std::is_trivially_move_constructible_v<value_type>)
    = default;

    constexpr InplaceVector(InplaceVector&& other) noexcept(::std::is_nothrow_move_constructible_v<value_type>)
        requires (extent != 0 && !::std::is_trivially_move_constructible_v<value_type> &&
                  ::std::is_move_constructible_v<value_type>)
    {
        ConstructionGuard guard{*this};
        for (auto& value : other) {
            (void)this->unchecked_emplace_back(::std::move(value));
        }
        guard.release();
    }

    constexpr ~InplaceVector() noexcept = default;

    constexpr ~InplaceVector() noexcept
        requires (extent != 0 && !::std::is_trivially_destructible_v<value_type>)
    {
        this->clear();
    }

    constexpr auto operator=(this InplaceVector&, InplaceVector const&) -> InplaceVector&
        requires (extent == 0 ||
                  (::std::is_trivially_copy_constructible_v<value_type> &&
                   ::std::is_trivially_copy_assignable_v<value_type> && ::std::is_trivially_destructible_v<value_type>))
    = default;

    constexpr auto operator=(this InplaceVector& self, InplaceVector const& other) -> InplaceVector&
        requires (extent != 0 && ::std::is_copy_constructible_v<value_type> &&
                  ::std::is_copy_assignable_v<value_type> &&
                  !(::std::is_trivially_copy_constructible_v<value_type> &&
                    ::std::is_trivially_copy_assignable_v<value_type> &&
                    ::std::is_trivially_destructible_v<value_type>))
    {
        if (::std::addressof(self) != ::std::addressof(other)) {
            self.assign(other.begin(), other.end());
        }
        return self;
    }

    constexpr auto operator=(this InplaceVector&, InplaceVector&&) noexcept -> InplaceVector&
        requires (extent == 0 ||
                  (::std::is_trivially_move_constructible_v<value_type> &&
                   ::std::is_trivially_move_assignable_v<value_type> && ::std::is_trivially_destructible_v<value_type>))
    = default;

    constexpr auto operator=(this InplaceVector& self,
                             InplaceVector&& other) noexcept(::std::is_nothrow_move_assignable_v<value_type> &&
                                                             ::std::is_nothrow_move_constructible_v<value_type>)
        -> InplaceVector&
        requires (extent != 0 && ::std::is_move_constructible_v<value_type> &&
                  ::std::is_move_assignable_v<value_type> &&
                  !(::std::is_trivially_move_constructible_v<value_type> &&
                    ::std::is_trivially_move_assignable_v<value_type> &&
                    ::std::is_trivially_destructible_v<value_type>))
    {
        if (::std::addressof(self) != ::std::addressof(other)) {
            self.assign(::std::make_move_iterator(other.begin()), ::std::make_move_iterator(other.end()));
        }
        return self;
    }

    constexpr auto operator=(this InplaceVector& self, ::std::initializer_list<value_type> values) -> InplaceVector&
        requires (::std::copy_constructible<value_type> && ::std::is_copy_assignable_v<value_type>)
    {
        self.assign(values.begin(), values.end());
        return self;
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, ::std::input_iterator InputIterator,
             ::std::sentinel_for<InputIterator> Sentinel>
        requires (::std::constructible_from<value_type, ::std::iter_reference_t<InputIterator>> &&
                  ::std::assignable_from<reference, ::std::iter_reference_t<InputIterator>>)
    constexpr void assign(this InplaceVector& self, InputIterator first, Sentinel last) {
        self.clear();
        self.template append<ndebug>(first, last);
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, ::std::ranges::input_range Range>
        requires ::std::constructible_from<value_type, ::std::ranges::range_reference_t<Range>>
    constexpr void assign_range(this InplaceVector& self, Range&& range) {
        self.clear();
        self.template append_range<ndebug>(::std::forward<Range>(range));
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void assign(this InplaceVector& self, size_type count, const_reference value)
        requires (::std::copy_constructible<value_type> && ::std::is_copy_assignable_v<value_type>)
    {
        self.template check_new_size<ndebug>(count);
        auto const common_size = ::std::min(self.size(), count);
        ::std::fill_n(self.begin(), common_size, value);
        if (count < self.size()) {
            self.destroy(self.iterator_at(count), self.end());
            self.set_size(count);
            return;
        }
        while (self.size() < count) {
            (void)self.unchecked_emplace_back(value);
        }
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void assign(this InplaceVector& self, ::std::initializer_list<value_type> values)
        requires (::std::copy_constructible<value_type> && ::std::is_copy_assignable_v<value_type>)
    {
        self.template assign<ndebug>(values.begin(), values.end());
    }

    [[nodiscard]]
    constexpr auto begin(this InplaceVector& self) noexcept -> iterator {
        return self.data();
    }

    [[nodiscard]]
    constexpr auto begin(this InplaceVector const& self) noexcept -> const_iterator {
        return self.data();
    }

    [[nodiscard]]
    constexpr auto cbegin(this InplaceVector const& self) noexcept -> const_iterator {
        return self.data();
    }

    [[nodiscard]]
    constexpr auto end(this InplaceVector& self) noexcept -> iterator {
        if (self.empty()) {
            return self.data();
        }
        return self.data() + self.size();
    }

    [[nodiscard]]
    constexpr auto end(this InplaceVector const& self) noexcept -> const_iterator {
        if (self.empty()) {
            return self.data();
        }
        return self.data() + self.size();
    }

    [[nodiscard]]
    constexpr auto cend(this InplaceVector const& self) noexcept -> const_iterator {
        return self.end();
    }

    [[nodiscard]]
    constexpr auto rbegin(this InplaceVector& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto rbegin(this InplaceVector const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto crbegin(this InplaceVector const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto rend(this InplaceVector& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto rend(this InplaceVector const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto crend(this InplaceVector const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto empty(this InplaceVector const& self) noexcept -> bool {
        return self.size() == 0;
    }

    [[nodiscard]]
    constexpr auto size(this InplaceVector const& self) noexcept -> size_type {
        return self.size_storage.value;
    }

    [[nodiscard]]
    constexpr auto size_bytes(this InplaceVector const& self) noexcept -> size_type {
        return self.size() * sizeof(value_type);
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
    static constexpr auto capacity() noexcept -> size_type {
        return extent;
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    static constexpr void reserve(size_type requested_capacity) noexcept {
        pltxt2htm_assert(requested_capacity <= extent, u8"InplaceVector capacity exceeded");
    }

    static constexpr void shrink_to_fit() noexcept {
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void resize(this InplaceVector& self, size_type new_size)
        requires ::std::default_initializable<value_type>
    {
        self.template check_new_size<ndebug>(new_size);
        AppendRollbackGuard guard{self};
        while (self.size() > new_size) {
            self.template pop_back<::pltxt2htm::Contracts::ignore>();
        }
        while (self.size() < new_size) {
            (void)self.unchecked_emplace_back();
        }
        guard.release();
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void resize(this InplaceVector& self, size_type new_size, const_reference value)
        requires ::std::copy_constructible<value_type>
    {
        self.template check_new_size<ndebug>(new_size);
        AppendRollbackGuard guard{self};
        while (self.size() > new_size) {
            self.template pop_back<::pltxt2htm::Contracts::ignore>();
        }
        while (self.size() < new_size) {
            (void)self.unchecked_emplace_back(value);
        }
        guard.release();
    }

    [[nodiscard]]
    constexpr auto operator[](this InplaceVector& self, size_type position) noexcept -> reference {
        return self.data()[position];
    }

    [[nodiscard]]
    constexpr auto operator[](this InplaceVector const& self, size_type position) noexcept -> const_reference {
        return self.data()[position];
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto index(this InplaceVector& self, size_type position) noexcept -> reference {
        pltxt2htm_assert(position < self.size(), u8"Index of InplaceVector out of bound");
        return self[position];
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto index(this InplaceVector const& self, size_type position) noexcept -> const_reference {
        pltxt2htm_assert(position < self.size(), u8"Index of InplaceVector out of bound");
        return self[position];
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    [[nodiscard]]
    constexpr auto at(this InplaceVector& self, size_type position) noexcept -> reference {
        return self.template index<ndebug>(position);
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    [[nodiscard]]
    constexpr auto at(this InplaceVector const& self, size_type position) noexcept -> const_reference {
        return self.template index<ndebug>(position);
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    [[nodiscard]]
    constexpr auto front(this InplaceVector& self) noexcept -> reference {
        pltxt2htm_assert(!self.empty(), u8"front() called on an empty InplaceVector");
        return self[0];
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    [[nodiscard]]
    constexpr auto front(this InplaceVector const& self) noexcept -> const_reference {
        pltxt2htm_assert(!self.empty(), u8"front() called on an empty InplaceVector");
        return self[0];
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    [[nodiscard]]
    constexpr auto back(this InplaceVector& self) noexcept -> reference {
        pltxt2htm_assert(!self.empty(), u8"back() called on an empty InplaceVector");
        return self[self.size() - 1];
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    [[nodiscard]]
    constexpr auto back(this InplaceVector const& self) noexcept -> const_reference {
        pltxt2htm_assert(!self.empty(), u8"back() called on an empty InplaceVector");
        return self[self.size() - 1];
    }

    [[nodiscard]]
    constexpr auto data(this InplaceVector& self) noexcept -> pointer {
        if constexpr (extent == 0) {
            return nullptr;
        }
        else {
            return self.storage.elements;
        }
    }

    [[nodiscard]]
    constexpr auto data(this InplaceVector const& self) noexcept -> const_pointer {
        if constexpr (extent == 0) {
            return nullptr;
        }
        else {
            return self.storage.elements;
        }
    }

    template<typename... Args>
        requires ::std::constructible_from<value_type, Args...>
    [[nodiscard]]
    constexpr auto unchecked_emplace_back(this InplaceVector& self, Args&&... args) noexcept(
        ::std::is_nothrow_constructible_v<value_type, Args...>) -> reference {
        auto* const result = ::std::construct_at(self.iterator_at(self.size()), ::std::forward<Args>(args)...);
        self.increment_size();
        return *result;
    }

    [[nodiscard]]
    constexpr auto unchecked_push_back(this InplaceVector& self, const_reference value) noexcept(
        ::std::is_nothrow_copy_constructible_v<value_type>) -> reference
        requires ::std::copy_constructible<value_type>
    {
        return self.unchecked_emplace_back(value);
    }

    [[nodiscard]]
    constexpr auto unchecked_push_back(this InplaceVector& self,
                                       value_type&& value) noexcept(::std::is_nothrow_move_constructible_v<value_type>)
        -> reference
        requires ::std::move_constructible<value_type>
    {
        return self.unchecked_emplace_back(::std::move(value));
    }

    template<typename... Args>
        requires ::std::constructible_from<value_type, Args...>
    [[nodiscard]]
    constexpr auto try_emplace_back(this InplaceVector& self,
                                    Args&&... args) noexcept(::std::is_nothrow_constructible_v<value_type, Args...>)
        -> Optional<reference> {
        if (self.size() == extent) {
            return nullopt;
        }
        return Optional<reference>{self.unchecked_emplace_back(::std::forward<Args>(args)...)};
    }

    [[nodiscard]]
    constexpr auto try_push_back(this InplaceVector& self,
                                 const_reference value) noexcept(::std::is_nothrow_copy_constructible_v<value_type>)
        -> Optional<reference>
        requires ::std::copy_constructible<value_type>
    {
        return self.try_emplace_back(value);
    }

    [[nodiscard]]
    constexpr auto try_push_back(this InplaceVector& self,
                                 value_type&& value) noexcept(::std::is_nothrow_move_constructible_v<value_type>)
        -> Optional<reference>
        requires ::std::move_constructible<value_type>
    {
        return self.try_emplace_back(::std::move(value));
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, typename... Args>
        requires ::std::constructible_from<value_type, Args...>
    [[nodiscard]]
    constexpr auto emplace_back(this InplaceVector& self,
                                Args&&... args) noexcept(::std::is_nothrow_constructible_v<value_type, Args...>)
        -> reference {
        self.template check_additional_size<ndebug>(1);
        return self.unchecked_emplace_back(::std::forward<Args>(args)...);
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    [[nodiscard]]
    constexpr auto push_back(this InplaceVector& self,
                             const_reference value) noexcept(::std::is_nothrow_copy_constructible_v<value_type>)
        -> reference
        requires ::std::copy_constructible<value_type>
    {
        return self.template emplace_back<ndebug>(value);
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    [[nodiscard]]
    constexpr auto push_back(this InplaceVector& self,
                             value_type&& value) noexcept(::std::is_nothrow_move_constructible_v<value_type>)
        -> reference
        requires ::std::move_constructible<value_type>
    {
        return self.template emplace_back<ndebug>(::std::move(value));
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, ::std::input_iterator InputIterator,
             ::std::sentinel_for<InputIterator> Sentinel>
        requires ::std::constructible_from<value_type, ::std::iter_reference_t<InputIterator>>
    constexpr void append(this InplaceVector& self, InputIterator first, Sentinel last) {
        if constexpr (::std::sized_sentinel_for<Sentinel, InputIterator>) {
            auto const count = static_cast<size_type>(last - first);
            self.template check_additional_size<ndebug>(count);
        }
        while (first != last) {
            (void)self.template emplace_back<ndebug>(*first);
            ++first;
        }
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, ::std::ranges::input_range Range>
        requires ::std::constructible_from<value_type, ::std::ranges::range_reference_t<Range>>
    constexpr void append_range(this InplaceVector& self, Range&& range) {
        self.template append<ndebug>(::std::ranges::begin(range), ::std::ranges::end(range));
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    constexpr void pop_back(this InplaceVector& self) noexcept {
        pltxt2htm_assert(!self.empty(), u8"pop_back() called on an empty InplaceVector");
        self.decrement_size();
        self.destroy_at(self.iterator_at(self.size()));
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, typename... Args>
        requires (::std::constructible_from<value_type, Args...> && ::std::permutable<iterator>)
    [[nodiscard]]
    constexpr auto emplace(this InplaceVector& self, const_iterator position, Args&&... args) -> iterator {
        auto const index = self.template position_index<ndebug>(position, true);
        (void)self.template emplace_back<ndebug>(::std::forward<Args>(args)...);
        auto const result = self.iterator_at(index);
        ::std::rotate(result, self.end() - 1, self.end());
        return result;
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    [[nodiscard]]
    constexpr auto insert(this InplaceVector& self, const_iterator position, const_reference value) -> iterator
        requires (::std::copy_constructible<value_type> && ::std::permutable<iterator>)
    {
        return self.template emplace<ndebug>(position, value);
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    [[nodiscard]]
    constexpr auto insert(this InplaceVector& self, const_iterator position, value_type&& value) -> iterator
        requires (::std::move_constructible<value_type> && ::std::permutable<iterator>)
    {
        return self.template emplace<ndebug>(position, ::std::move(value));
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    [[nodiscard]]
    constexpr auto insert(this InplaceVector& self, const_iterator position, size_type count, const_reference value)
        -> iterator
        requires (::std::copy_constructible<value_type> && ::std::permutable<iterator>)
    {
        auto const index = self.template position_index<ndebug>(position, true);
        self.template check_additional_size<ndebug>(count);
        if (count == 0) {
            return self.iterator_at(index);
        }
        auto const old_size = self.size();
        while (self.size() < old_size + count) {
            (void)self.unchecked_emplace_back(value);
        }
        auto const result = self.iterator_at(index);
        ::std::rotate(result, self.iterator_at(old_size), self.end());
        return result;
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, ::std::input_iterator InputIterator,
             ::std::sentinel_for<InputIterator> Sentinel>
        requires (::std::constructible_from<value_type, ::std::iter_reference_t<InputIterator>> &&
                  ::std::permutable<iterator>)
    [[nodiscard]]
    constexpr auto insert(this InplaceVector& self, const_iterator position, InputIterator first, Sentinel last)
        -> iterator {
        auto const index = self.template position_index<ndebug>(position, true);
        auto const old_size = self.size();
        self.template append<ndebug>(first, last);
        auto const result = self.iterator_at(index);
        if (self.size() == old_size) {
            return result;
        }
        ::std::rotate(result, self.iterator_at(old_size), self.end());
        return result;
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce, ::std::ranges::input_range Range>
        requires (::std::constructible_from<value_type, ::std::ranges::range_reference_t<Range>> &&
                  ::std::permutable<iterator>)
    [[nodiscard]]
    constexpr auto insert_range(this InplaceVector& self, const_iterator position, Range&& range) -> iterator {
        return self.template insert<ndebug>(position, ::std::ranges::begin(range), ::std::ranges::end(range));
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    [[nodiscard]]
    constexpr auto insert(this InplaceVector& self, const_iterator position, ::std::initializer_list<value_type> values)
        -> iterator
        requires (::std::copy_constructible<value_type> && ::std::permutable<iterator>)
    {
        return self.template insert<ndebug>(position, values.begin(), values.end());
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    [[nodiscard]]
    constexpr auto erase(this InplaceVector& self, const_iterator position) -> iterator
        requires ::std::is_move_assignable_v<value_type>
    {
        auto const index = self.template position_index<ndebug>(position, false);
        auto const result = self.iterator_at(index);
        ::std::move(result + 1, self.end(), result);
        self.template pop_back<::pltxt2htm::Contracts::ignore>();
        return result;
    }

    template<::pltxt2htm::Contracts ndebug = ::pltxt2htm::Contracts::quick_enforce>
    [[nodiscard]]
    constexpr auto erase(this InplaceVector& self, const_iterator first, const_iterator last) -> iterator
        requires ::std::is_move_assignable_v<value_type>
    {
        auto const first_index = self.template position_index<ndebug>(first, true);
        auto const last_index = self.template position_index<ndebug>(last, true);
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

    constexpr void clear(this InplaceVector& self) noexcept {
        self.destroy(self.begin(), self.end());
        self.set_size(0);
    }

    constexpr void swap(this InplaceVector& self,
                        InplaceVector& other) noexcept(extent == 0 ||
                                                       (::std::is_nothrow_swappable_v<value_type> &&
                                                        ::std::is_nothrow_move_constructible_v<value_type>))
        requires (extent == 0 || (::std::swappable<value_type> && ::std::move_constructible<value_type>))
    {
        if constexpr (extent == 0) {
            return;
        }
        else {
            if (::std::addressof(self) == ::std::addressof(other)) {
                return;
            }
            auto const common_size = ::std::min(self.size(), other.size());
            for (size_type index{}; index < common_size; ++index) {
                ::std::ranges::swap(self[index], other[index]);
            }
            if (self.size() < other.size()) {
                auto const other_size = other.size();
                while (self.size() < other_size) {
                    auto const index = self.size();
                    (void)self.unchecked_emplace_back(::std::move(other[index]));
                }
                other.destroy(other.iterator_at(common_size), other.end());
                other.set_size(common_size);
                return;
            }
            auto const self_size = self.size();
            while (other.size() < self_size) {
                auto const index = other.size();
                (void)other.unchecked_emplace_back(::std::move(self[index]));
            }
            self.destroy(self.iterator_at(common_size), self.end());
            self.set_size(common_size);
        }
    }
};

template<typename T, ::std::size_t left_extent, ::std::size_t right_extent>
    requires ::std::equality_comparable<T>
[[nodiscard]]
constexpr auto operator==(InplaceVector<T, left_extent> const& left, InplaceVector<T, right_extent> const& right)
    -> bool {
    return ::std::equal(left.begin(), left.end(), right.begin(), right.end());
}

template<typename T, ::std::size_t left_extent, ::std::size_t right_extent>
    requires ::std::three_way_comparable<T>
[[nodiscard]]
constexpr auto operator<=>(InplaceVector<T, left_extent> const& left, InplaceVector<T, right_extent> const& right) {
    return ::std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end(),
                                                    ::std::compare_three_way{});
}

template<typename T, ::std::size_t extent>
    requires (extent == 0 || (::std::swappable<T> && ::std::move_constructible<T>))
constexpr void swap(InplaceVector<T, extent>& left,
                    InplaceVector<T, extent>& right) noexcept(noexcept(left.swap(right))) {
    left.swap(right);
}

template<typename T, ::std::size_t extent, typename U = T>
    requires ::std::equality_comparable_with<T, U>
[[nodiscard]]
constexpr auto erase(InplaceVector<T, extent>& vector, U const& value) -> typename InplaceVector<T, extent>::size_type {
    auto const old_size = vector.size();
    auto const new_end = ::std::remove(vector.begin(), vector.end(), value);
    (void)vector.erase(new_end, vector.end());
    return old_size - vector.size();
}

template<typename T, ::std::size_t extent, typename Predicate>
    requires ::std::predicate<Predicate&, T const&>
[[nodiscard]]
constexpr auto erase_if(InplaceVector<T, extent>& vector, Predicate predicate) ->
    typename InplaceVector<T, extent>::size_type {
    auto const old_size = vector.size();
    auto const new_end = ::std::remove_if(vector.begin(), vector.end(), predicate);
    (void)vector.erase(new_end, vector.end());
    return old_size - vector.size();
}

} // namespace pltxt2htm::container

#include "../details/pop_macro.hh"
