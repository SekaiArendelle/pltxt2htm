/**
 * @file inplace_vector_decl.hh
 * @brief Declarations for container::InplaceVector.
 * @details Contract-checked members are declared here and defined in
 *          inplace_vector_def.inc. Members defined inline that call those members also
 *          need inplace_vector_def.inc in the same translation unit; include
 *          container/inplace_vector.hh for the complete API.
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

#include "../optional.hh"
#include "../../details/concepts.hh"

// windows.h defines min and max as function-like macros, which would expand
// inside std::numeric_limits<...>::max(). Keep them undefined in this header.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace pltxt2htm::container {

namespace details {

template<typename T>
concept is_constexpr_activatable_inplace_vector_element =
    ::std::is_trivially_default_constructible_v<T> && ::std::is_trivially_copyable_v<T> &&
    ::std::is_trivially_assignable_v<T&, T>;

template<typename T, ::std::size_t extent>
union InplaceVectorNonzeroStorage {
    T elements[extent];

    constexpr InplaceVectorNonzeroStorage() noexcept {
        if consteval {
            // Assignment implicitly starts the lifetime of this union array and
            // its implicit-lifetime elements without relying on array construct_at.
            if constexpr (is_constexpr_activatable_inplace_vector_element<T>) {
                for (::std::size_t index{}; index != extent; ++index) {
                    elements[index] = T{};
                }
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

    constexpr void set_size(this InplaceVector& self, size_type new_size) noexcept {
        if constexpr (extent != 0) {
            self.size_storage.value = new_size;
        }
    }

    constexpr void increment_size(this InplaceVector& self) noexcept {
        if constexpr (extent != 0) {
            ++self.size_storage.value;
        }
    }

    constexpr void decrement_size(this InplaceVector& self) noexcept {
        if constexpr (extent != 0) {
            --self.size_storage.value;
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
        if consteval {
            if constexpr (details::is_constexpr_activatable_inplace_vector_element<value_type>) {
                return;
            }
        }
        ::std::destroy_at(position);
    }

    static constexpr void destroy(iterator first, iterator last) noexcept {
        if consteval {
            if constexpr (details::is_constexpr_activatable_inplace_vector_element<value_type>) {
                return;
            }
        }
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
                this->vector->pop_back();
            }
        }

        constexpr void release(this AppendRollbackGuard& self) noexcept {
            self.vector = nullptr;
        }
    };

    [[nodiscard]]
    constexpr auto position_index(this InplaceVector const& self, const_iterator position, bool allow_end) noexcept
        -> size_type;

    template<typename InputIterator, typename Sentinel>
    static consteval auto is_nothrow_range_iteration() noexcept -> bool {
        constexpr bool is_nothrow_iteration =
            ::pltxt2htm::details::nothrow_sentinel_iteration<InputIterator, Sentinel> &&
            ::std::is_nothrow_constructible_v<value_type, ::std::iter_reference_t<InputIterator>> &&
            ::std::is_nothrow_copy_constructible_v<InputIterator> && ::std::is_nothrow_copy_constructible_v<Sentinel>;
        if constexpr (::std::sized_sentinel_for<Sentinel, InputIterator>) {
            return is_nothrow_iteration &&
                   noexcept(static_cast<size_type>(::std::declval<Sentinel&>() - ::std::declval<InputIterator&>()));
        }
        else {
            return is_nothrow_iteration;
        }
    }

    template<typename Range>
    static consteval auto is_nothrow_range_operation() noexcept -> bool {
        return ::pltxt2htm::details::nothrow_range_access<Range> &&
               is_nothrow_range_iteration<::std::ranges::iterator_t<Range>, ::std::ranges::sentinel_t<Range>>();
    }

    static consteval auto is_nothrow_element_rotation() noexcept -> bool {
        return ::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_move_assignable_v<value_type> &&
               ::std::is_nothrow_swappable_v<value_type>;
    }

public:
    constexpr InplaceVector() noexcept = default;

    constexpr explicit InplaceVector(size_type count) noexcept(::std::is_nothrow_default_constructible_v<value_type>)
        requires ::std::default_initializable<value_type>;

    constexpr InplaceVector(size_type count,
                            const_reference value) noexcept(::std::is_nothrow_copy_constructible_v<value_type>)
        requires ::std::copy_constructible<value_type>;

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
        requires ::std::constructible_from<value_type, ::std::iter_reference_t<InputIterator>>
    constexpr InplaceVector(InputIterator first,
                            Sentinel last) noexcept(is_nothrow_range_iteration<InputIterator, Sentinel>()) {
        ConstructionGuard guard{*this};
        this->append(first, last);
        guard.release();
    }

    constexpr InplaceVector(::std::initializer_list<value_type> values) noexcept(
        ::std::is_nothrow_copy_constructible_v<value_type>)
        requires ::std::copy_constructible<value_type>
    {
        ConstructionGuard guard{*this};
        this->append(values.begin(), values.end());
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
            this->emplace_back(value);
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
            this->emplace_back(::std::move(value));
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

    constexpr auto operator=(this InplaceVector& self,
                             InplaceVector const& other) noexcept(::std::is_nothrow_copy_constructible_v<value_type>)
        -> InplaceVector&
        requires (extent != 0 && ::std::is_copy_constructible_v<value_type> &&
                  ::std::is_copy_assignable_v<value_type> &&
                  !(::std::is_trivially_copy_constructible_v<value_type> &&
                    ::std::is_trivially_copy_assignable_v<value_type> &&
                    ::std::is_trivially_destructible_v<value_type>));

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
                    ::std::is_trivially_destructible_v<value_type>));

    constexpr auto operator=(this InplaceVector& self, ::std::initializer_list<value_type> values) noexcept(
        ::std::is_nothrow_copy_constructible_v<value_type>) -> InplaceVector&
        requires (::std::copy_constructible<value_type> && ::std::is_copy_assignable_v<value_type>)
    {
        self.assign(values.begin(), values.end());
        return self;
    }

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
        requires (::std::constructible_from<value_type, ::std::iter_reference_t<InputIterator>> &&
                  ::std::assignable_from<reference, ::std::iter_reference_t<InputIterator>>)
    constexpr void assign(this InplaceVector& self, InputIterator first,
                          Sentinel last) noexcept(is_nothrow_range_iteration<InputIterator, Sentinel>()) {
        self.clear();
        self.append(first, last);
    }

    template<::std::ranges::input_range Range>
        requires ::std::constructible_from<value_type, ::std::ranges::range_reference_t<Range>>
    constexpr void assign_range(this InplaceVector& self, Range&& range) noexcept(is_nothrow_range_operation<Range>()) {
        self.clear();
        self.append_range(::std::forward<Range>(range));
    }

    constexpr void assign(this InplaceVector& self, size_type count,
                          const_reference value) noexcept(::std::is_nothrow_copy_constructible_v<value_type> &&
                                                          ::std::is_nothrow_copy_assignable_v<value_type>)
        requires (::std::copy_constructible<value_type> && ::std::is_copy_assignable_v<value_type>);

    constexpr void assign(this InplaceVector& self, ::std::initializer_list<value_type> values) noexcept(
        ::std::is_nothrow_copy_constructible_v<value_type>)
        requires (::std::copy_constructible<value_type> && ::std::is_copy_assignable_v<value_type>)
    {
        self.assign(values.begin(), values.end());
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
        if (self.is_empty()) {
            return self.data();
        }
        return self.data() + self.size();
    }

    [[nodiscard]]
    constexpr auto end(this InplaceVector const& self) noexcept -> const_iterator {
        if (self.is_empty()) {
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
    constexpr auto is_empty(this InplaceVector const& self) noexcept -> bool {
        return self.size() == 0;
    }

    /**
     * @brief Tests whether the vector has no elements, for downstream users only.
     * @return true when size() is zero.
     */
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto empty(this InplaceVector const&) noexcept -> bool = delete
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
    constexpr auto empty(this InplaceVector const& self) noexcept -> bool {
        return self.is_empty();
    }
#endif

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

    static constexpr void reserve(size_type requested_capacity) noexcept;

    static constexpr void shrink_to_fit() noexcept {
    }

    constexpr void resize(this InplaceVector& self,
                          size_type new_size) noexcept(::std::is_nothrow_default_constructible_v<value_type>)
        requires ::std::default_initializable<value_type>;

    constexpr void resize(this InplaceVector& self, size_type new_size,
                          const_reference value) noexcept(::std::is_nothrow_copy_constructible_v<value_type>)
        requires ::std::copy_constructible<value_type>;

    /**
     * @brief Indexed element access, bounds-checked according to the
     *        build-wide contract setting.
     * @param position Zero-based element position.
     * @return Mutable reference to the requested element.
     * @pre position is a valid element position.
     */
    [[nodiscard]]
    constexpr auto operator[](this InplaceVector& self, size_type position) noexcept -> reference;

    /**
     * @brief Indexed element access, read-only, bounds-checked according to the
     *        build-wide contract setting.
     * @param position Zero-based element position.
     * @return Read-only reference to the requested element.
     * @pre position is a valid element position.
     */
    [[nodiscard]]
    constexpr auto operator[](this InplaceVector const& self, size_type position) noexcept -> const_reference;

    [[nodiscard]]
    constexpr auto front(this InplaceVector& self) noexcept -> reference;

    [[nodiscard]]
    constexpr auto front(this InplaceVector const& self) noexcept -> const_reference;

    [[nodiscard]]
    constexpr auto back(this InplaceVector& self) noexcept -> reference;

    [[nodiscard]]
    constexpr auto back(this InplaceVector const& self) noexcept -> const_reference;

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
    constexpr auto try_emplace_back(this InplaceVector& self,
                                    Args&&... args) noexcept(::std::is_nothrow_constructible_v<value_type, Args...>)
        -> Optional<reference> {
        if (self.size() == extent) {
            return nullopt;
        }
        return Optional<reference>{self.emplace_back(::std::forward<Args>(args)...)};
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

    /**
     * @note Intentionally not marked `[[nodiscard]]`: callers commonly need only the insertion side effect.
     */
    template<typename... Args>
    constexpr auto emplace_back(this InplaceVector& self,
                                Args&&... args) noexcept(::std::is_nothrow_constructible_v<value_type, Args...>)
        -> reference
        requires ::std::constructible_from<value_type, Args...>;

    /**
     * @note Intentionally not marked `[[nodiscard]]`: callers commonly need only the insertion side effect.
     */
    constexpr auto push_back(this InplaceVector& self,
                             const_reference value) noexcept(::std::is_nothrow_copy_constructible_v<value_type>)
        -> reference
        requires ::std::copy_constructible<value_type>
    {
        return self.emplace_back(value);
    }

    /**
     * @note Intentionally not marked `[[nodiscard]]`: callers commonly need only the insertion side effect.
     */
    constexpr auto push_back(this InplaceVector& self,
                             value_type&& value) noexcept(::std::is_nothrow_move_constructible_v<value_type>)
        -> reference
        requires ::std::move_constructible<value_type>
    {
        return self.emplace_back(::std::move(value));
    }

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
    constexpr void append(this InplaceVector& self, InputIterator first,
                          Sentinel last) noexcept(is_nothrow_range_iteration<InputIterator, Sentinel>())
        requires ::std::constructible_from<value_type, ::std::iter_reference_t<InputIterator>>;

    template<::std::ranges::input_range Range>
        requires ::std::constructible_from<value_type, ::std::ranges::range_reference_t<Range>>
    constexpr void append_range(this InplaceVector& self, Range&& range) noexcept(is_nothrow_range_operation<Range>()) {
        self.append(::std::ranges::begin(range), ::std::ranges::end(range));
    }

    constexpr void pop_back(this InplaceVector& self) noexcept;

    template<typename... Args>
        requires (::std::constructible_from<value_type, Args...> && ::std::permutable<iterator>)
    [[nodiscard]]
    constexpr auto emplace(this InplaceVector& self, const_iterator position,
                           Args&&... args) noexcept(::std::is_nothrow_constructible_v<value_type, Args...> &&
                                                    is_nothrow_element_rotation()) -> iterator {
        auto const index = self.position_index(position, true);
        self.emplace_back(::std::forward<Args>(args)...);
        auto const result = self.iterator_at(index);
        ::std::rotate(result, self.end() - 1, self.end());
        return result;
    }

    [[nodiscard]]
    constexpr auto insert(this InplaceVector& self, const_iterator position,
                          const_reference value) noexcept(::std::is_nothrow_copy_constructible_v<value_type> &&
                                                          is_nothrow_element_rotation()) -> iterator
        requires (::std::copy_constructible<value_type> && ::std::permutable<iterator>)
    {
        return self.emplace(position, value);
    }

    [[nodiscard]]
    constexpr auto insert(this InplaceVector& self, const_iterator position,
                          value_type&& value) noexcept(::std::is_nothrow_move_constructible_v<value_type> &&
                                                       is_nothrow_element_rotation()) -> iterator
        requires (::std::move_constructible<value_type> && ::std::permutable<iterator>)
    {
        return self.emplace(position, ::std::move(value));
    }

    [[nodiscard]]
    constexpr auto insert(this InplaceVector& self, const_iterator position, size_type count,
                          const_reference value) noexcept(::std::is_nothrow_copy_constructible_v<value_type> &&
                                                          is_nothrow_element_rotation()) -> iterator
        requires (::std::copy_constructible<value_type> && ::std::permutable<iterator>);

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
    [[nodiscard]]
    constexpr auto insert(this InplaceVector& self, const_iterator position, InputIterator first,
                          Sentinel last) noexcept(is_nothrow_range_iteration<InputIterator, Sentinel>() &&
                                                  is_nothrow_element_rotation()) -> iterator
        requires (::std::constructible_from<value_type, ::std::iter_reference_t<InputIterator>> &&
                  ::std::permutable<iterator>);

    template<::std::ranges::input_range Range>
        requires (::std::constructible_from<value_type, ::std::ranges::range_reference_t<Range>> &&
                  ::std::permutable<iterator>)
    [[nodiscard]]
    constexpr auto insert_range(this InplaceVector& self, const_iterator position,
                                Range&& range) noexcept(is_nothrow_range_operation<Range>() &&
                                                        is_nothrow_element_rotation()) -> iterator {
        return self.insert(position, ::std::ranges::begin(range), ::std::ranges::end(range));
    }

    [[nodiscard]]
    constexpr auto insert(
        this InplaceVector& self, const_iterator position,
        ::std::initializer_list<value_type> values) noexcept(::std::is_nothrow_copy_constructible_v<value_type> &&
                                                             is_nothrow_element_rotation()) -> iterator
        requires (::std::copy_constructible<value_type> && ::std::permutable<iterator>)
    {
        return self.insert(position, values.begin(), values.end());
    }

    [[nodiscard]]
    constexpr auto erase(this InplaceVector& self,
                         const_iterator position) noexcept(::std::is_nothrow_move_assignable_v<value_type>) -> iterator
        requires ::std::is_move_assignable_v<value_type>
    {
        auto const index = self.position_index(position, false);
        auto const result = self.iterator_at(index);
        ::std::move(result + 1, self.end(), result);
        self.pop_back();
        return result;
    }

    [[nodiscard]]
    constexpr auto erase(this InplaceVector& self, const_iterator first,
                         const_iterator last) noexcept(::std::is_nothrow_move_assignable_v<value_type>) -> iterator
        requires ::std::is_move_assignable_v<value_type>;

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
                    self.emplace_back(::std::move(other[index]));
                }
                other.destroy(other.iterator_at(common_size), other.end());
                other.set_size(common_size);
                return;
            }
            auto const self_size = self.size();
            while (other.size() < self_size) {
                auto const index = other.size();
                other.emplace_back(::std::move(self[index]));
            }
            self.destroy(self.iterator_at(common_size), self.end());
            self.set_size(common_size);
        }
    }

    template<::std::size_t right_extent>
        requires ::std::equality_comparable<value_type>
    [[nodiscard]]
    constexpr auto
    operator==(this InplaceVector const& self, InplaceVector<value_type, right_extent> const& other) noexcept(
        extent == 0 || right_extent == 0 || ::pltxt2htm::details::nothrow_equality_comparable<value_type>) -> bool {
        return ::std::equal(self.begin(), self.end(), other.begin(), other.end());
    }

    template<::std::size_t right_extent>
        requires ::std::three_way_comparable<value_type>
    [[nodiscard]]
    constexpr auto operator<=>(this InplaceVector const& self,
                               InplaceVector<value_type, right_extent> const&
                                   other) noexcept(extent == 0 || right_extent == 0 ||
                                                   ::pltxt2htm::details::nothrow_three_way_comparable<value_type>) {
        return ::std::lexicographical_compare_three_way(self.begin(), self.end(), other.begin(), other.end(),
                                                        ::std::compare_three_way{});
    }
};

template<typename T, ::std::size_t extent>
    requires (extent == 0 || (::std::swappable<T> && ::std::move_constructible<T>))
constexpr void swap(InplaceVector<T, extent>& left,
                    InplaceVector<T, extent>& right) noexcept(noexcept(left.swap(right))) {
    left.swap(right);
}

} // namespace pltxt2htm::container

#pragma pop_macro("max")
#pragma pop_macro("min")
