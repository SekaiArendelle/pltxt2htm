/**
 * @file string.hh
 * @brief Dynamically allocated strings for pltxt2htm.
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <utility>

#include <fast_io/fast_io_dsal/string.h>

#include "../contracts.hh"
#include "../details/concepts.hh"
#include "../details/literal_string.hh"
#include "../details/push_macro.hh"
#include "string_view.hh"

namespace pltxt2htm::container {

/**
 * @brief An owning, null-terminated contiguous character sequence.
 * @tparam CharType Character type stored by the string.
 * @tparam Allocator fast_io-compatible allocator used for dynamic storage.
 *
 * BasicString stores three pointers. `begin_pointer` addresses the first character,
 * `current_pointer` addresses the trailing null character, and `end_pointer`
 * addresses the last allocation slot reserved for that terminator. Consequently,
 * `[begin_pointer, current_pointer)` is the logical string and
 * `[begin_pointer, end_pointer)` is the character capacity.
 *
 * The following invariants hold for every live object:
 *
 * - `begin_pointer <= current_pointer <= end_pointer` within one allocation;
 * - `*current_pointer == value_type{}`;
 * - capacity excludes the trailing-null slot;
 * - a runtime empty string uses fast_io's shared static null character and owns no
 *   allocation.
 *
 * Operations accepting BasicStringView support views into the same BasicString.
 * Growth paths finish reading the old allocation before releasing it, while
 * in-place paths use overlap-safe copying where necessary.
 */
template<::pltxt2htm::details::is_char_type CharType, typename Allocator = ::fast_io::native_global_allocator>
class BasicString {
public:
    using allocator_type = Allocator; ///< Allocator used for owned storage.
    using value_type = CharType; ///< Character type stored by the string.
    using char_type = value_type; ///< fast_io-compatible character alias.
    using size_type = ::std::size_t; ///< Unsigned size and offset type.
    using difference_type = ::std::ptrdiff_t; ///< Signed iterator difference type.
    using reference = value_type&; ///< Mutable character reference.
    using const_reference = value_type const&; ///< Read-only character reference.
    using pointer = value_type*; ///< Mutable contiguous-storage pointer.
    using const_pointer = value_type const*; ///< Read-only contiguous-storage pointer.
    using iterator = pointer; ///< Mutable contiguous iterator.
    using const_iterator = const_pointer; ///< Read-only contiguous iterator.
    using reverse_iterator = ::std::reverse_iterator<iterator>; ///< Mutable reverse iterator.
    using const_reverse_iterator = ::std::reverse_iterator<const_iterator>; ///< Read-only reverse iterator.
    using string_view_type = BasicStringView<value_type>; ///< Non-owning view type.

private:
    using untyped_allocator_type =
        ::fast_io::generic_allocator_adapter<allocator_type>; ///< Normalized fast_io allocator adapter.
    using typed_allocator_type =
        ::fast_io::typed_generic_allocator_adapter<untyped_allocator_type,
                                                   value_type>; ///< Character-counted allocator adapter.

    pointer begin_pointer; ///< First character, or the shared runtime empty character.
    pointer current_pointer; ///< Trailing null character and logical end iterator.
    pointer end_pointer; ///< Last allocation slot, reserved for a trailing null.

    /**
     * @brief Starts character lifetimes in allocator-provided storage during constant evaluation.
     * @param storage First slot in the newly allocated storage.
     * @param count Number of allocated character slots.
     *
     * At runtime every supported CharType is a trivial implicit-lifetime type. The
     * C allocator-backed storage and subsequent character writes therefore start
     * the required lifetimes without a per-element construction loop. Performing
     * `construct_at` there would only add redundant work.
     *
     * During constant evaluation, fast_io's typed allocator is backed by
     * `std::allocator::allocate`. The constant evaluator tracks object lifetimes
     * strictly, so assignments through the returned pointer are only valid after
     * each character object has been explicitly created. The `if consteval` branch
     * supplies those `construct_at` calls while being completely absent from the
     * runtime code path.
     */
    static constexpr void start_lifetime(pointer storage, size_type count) noexcept {
        if consteval {
            for (size_type index{}; index < count; ++index) {
                ::std::construct_at(storage + index);
            }
        }
    }

    /**
     * @brief Restores the empty-string representation without preserving contents.
     *
     * Runtime objects point at fast_io's shared static null character and own no
     * allocation. Constant evaluation instead obtains writable storage whose
     * character lifetime is visible to the constant evaluator.
     */
    constexpr void reset(this BasicString& self) noexcept {
        if consteval {
            auto [new_pointer, allocated_size] = typed_allocator_type::allocate_at_least(2);
            self.start_lifetime(new_pointer, allocated_size);
            self.begin_pointer = new_pointer;
            self.current_pointer = new_pointer;
            self.end_pointer = new_pointer + static_cast<size_type>(allocated_size - 1);
            *new_pointer = value_type{};
        }
        else {
            pointer const empty_string{const_cast<pointer>(::fast_io::null_terminated_c_str_v<value_type>)};
            self.begin_pointer = empty_string;
            self.current_pointer = empty_string;
            self.end_pointer = empty_string;
        }
    }

    /**
     * @brief Releases owned storage, if any.
     *
     * The shared runtime empty representation has zero capacity and is never
     * passed to the allocator.
     */
    constexpr void destroy(this BasicString& self) noexcept {
        if (self.begin_pointer != self.end_pointer) {
            typed_allocator_type::deallocate_n(self.begin_pointer, self.capacity() + 1);
        }
    }

    /**
     * @brief Allocates storage for at least `capacity` characters plus a terminator.
     * @param capacity Requested logical character capacity, excluding the terminator.
     *
     * Initializes the object as an empty string backed by the new allocation.
     * Constructors use the NDEBUG-selected contract workaround because callers
     * cannot explicitly provide a Contracts template argument to a constructor.
     */
    constexpr void allocate(this BasicString& self, size_type capacity) noexcept {
        // Work around constructors being unable to receive an explicitly selected Contracts template argument.
#ifndef NDEBUG
        constexpr auto ndebug{::pltxt2htm::Contracts::quick_enforce};
        pltxt2htm_assert(capacity < self.max_size(), u8"BasicString capacity is too large");
#endif
        auto [new_pointer, allocated_size] = typed_allocator_type::allocate_at_least(capacity + 1);
        self.start_lifetime(new_pointer, allocated_size);
        self.begin_pointer = new_pointer;
        self.current_pointer = new_pointer;
        self.end_pointer = new_pointer + static_cast<size_type>(allocated_size - 1);
        *new_pointer = value_type{};
    }

    /**
     * @brief Allocates zero-initialized storage for exactly `capacity` characters plus a terminator.
     * @param capacity Requested logical character capacity, excluding the terminator.
     *
     * At runtime this preserves allocator-specific zero-allocation optimizations,
     * such as calloc-backed lazy zero pages. The exact-size API avoids relying on
     * the allocator adapter's conditional `allocate_zero_at_least` fallback.
     * During constant evaluation, start_lifetime value-initializes every allocated
     * character instead.
     */
    constexpr void allocate_zero(this BasicString& self, size_type capacity) noexcept {
        // Work around constructors being unable to receive an explicitly selected Contracts template argument.
#ifndef NDEBUG
        constexpr auto ndebug{::pltxt2htm::Contracts::quick_enforce};
        pltxt2htm_assert(capacity < self.max_size(), u8"BasicString capacity is too large");
#endif
        if consteval {
            auto [new_pointer, allocated_size] = typed_allocator_type::allocate_at_least(capacity + 1);
            self.start_lifetime(new_pointer, allocated_size);
            self.begin_pointer = new_pointer;
            self.current_pointer = new_pointer;
            self.end_pointer = new_pointer + static_cast<size_type>(allocated_size - 1);
        }
        else {
            pointer const new_pointer{typed_allocator_type::allocate_zero(capacity + 1)};
            self.begin_pointer = new_pointer;
            self.current_pointer = new_pointer;
            self.end_pointer = new_pointer + capacity;
        }
    }

    /**
     * @brief Constructs the string from a counted character sequence.
     * @param first First source character.
     * @param count Number of characters to copy, excluding any source terminator.
     */
    constexpr void construct(this BasicString& self, const_pointer first, size_type count) noexcept {
        if (count == 0) {
            self.reset();
            return;
        }
        self.allocate(count);
        ::std::copy_n(first, count, self.begin_pointer);
        self.current_pointer = self.begin_pointer + count;
        *self.current_pointer = value_type{};
    }

    /**
     * @brief Finds an in-place insertion source within this string.
     * @param source Candidate source pointer.
     * @return Offset from begin_pointer, or npos when source is external.
     *
     * Runtime pointer ordering uses std::less, whose pointer specialization gives a
     * strict total order even for unrelated pointers, making this check O(1).
     * Relational ordering of unrelated pointers is not generally a constant
     * expression, so constant evaluation uses equality-only scanning. This helper
     * is only needed for in-place insertion; append and assignment avoid source
     * classification entirely.
     */
    [[nodiscard]]
    constexpr auto source_offset(this BasicString const& self, const_pointer source) noexcept -> size_type {
        if consteval {
            for (size_type index{}; index <= self.size(); ++index) {
                if (source == self.begin_pointer + index) {
                    return index;
                }
            }
            return self.npos;
        }
        else {
            ::std::less<const_pointer> const less{};
            if (!less(source, self.begin_pointer) && !less(self.current_pointer, source)) {
                return static_cast<size_type>(source - self.begin_pointer);
            }
            return self.npos;
        }
    }

    /**
     * @brief Computes the geometric-growth capacity for a required size.
     * @param required_capacity Minimum logical capacity required by the caller.
     * @return A capacity at least as large as required_capacity without overflow.
     */
    [[nodiscard]]
    constexpr auto grown_capacity(this BasicString const& self, size_type required_capacity) noexcept -> size_type {
        size_type new_capacity{self.capacity()};
        if (new_capacity == 0) {
            new_capacity = 1;
        }
        while (new_capacity < required_capacity) {
            size_type const remaining{self.max_size() - new_capacity};
            if (new_capacity > remaining) {
                return required_capacity;
            }
            new_capacity *= 2;
        }
        return new_capacity;
    }

    /**
     * @brief Appends through a newly allocated buffer while preserving source aliases.
     * @tparam ndebug Contract policy used for capacity validation.
     * @param source First source character; it may point into self.
     * @param count Number of characters to append.
     *
     * Both the existing prefix and source are copied before destroy releases the
     * old allocation. This ordering keeps self-append valid without a preliminary
     * linear alias scan.
     */
    template<::pltxt2htm::Contracts ndebug>
    constexpr void append_reallocate(this BasicString& self, const_pointer source, size_type count) noexcept {
        size_type const old_size{self.size()};
        pltxt2htm_assert(count < self.max_size() - old_size, u8"BasicString capacity is too large");
        size_type const new_size{old_size + count};
        size_type const new_capacity{self.grown_capacity(new_size)};
        auto [new_pointer, allocated_size] = typed_allocator_type::allocate_at_least(new_capacity + 1);
        self.start_lifetime(new_pointer, allocated_size);
        ::std::copy(self.begin_pointer, self.current_pointer, new_pointer);
        ::std::copy_n(source, count, new_pointer + old_size);
        new_pointer[new_size] = value_type{};
        self.destroy();
        self.begin_pointer = new_pointer;
        self.current_pointer = new_pointer + new_size;
        self.end_pointer = new_pointer + static_cast<size_type>(allocated_size - 1);
    }

    /**
     * @brief Inserts through a newly allocated buffer while preserving source aliases.
     * @tparam ndebug Contract policy used for capacity validation.
     * @param position_index Offset at which to insert.
     * @param string Characters to insert; the view may refer to self.
     * @return Iterator to the first inserted character.
     *
     * Prefix, inserted sequence, and suffix are assembled before the old allocation
     * is released, so no temporary BasicString is required for self-insertion.
     */
    template<::pltxt2htm::Contracts ndebug>
    constexpr auto insert_reallocate(this BasicString& self, size_type position_index, string_view_type string) noexcept
        -> iterator {
        size_type const old_size{self.size()};
        pltxt2htm_assert(string.size() < self.max_size() - old_size, u8"BasicString capacity is too large");
        size_type const new_size{old_size + string.size()};
        size_type const new_capacity{self.grown_capacity(new_size)};
        auto [new_pointer, allocated_size] = typed_allocator_type::allocate_at_least(new_capacity + 1);
        self.start_lifetime(new_pointer, allocated_size);
        ::std::copy_n(self.begin_pointer, position_index, new_pointer);
        ::std::copy(string.begin(), string.end(), new_pointer + position_index);
        ::std::copy(self.begin_pointer + position_index, self.current_pointer,
                    new_pointer + position_index + string.size());
        new_pointer[new_size] = value_type{};
        self.destroy();
        self.begin_pointer = new_pointer;
        self.current_pointer = new_pointer + new_size;
        self.end_pointer = new_pointer + static_cast<size_type>(allocated_size - 1);
        return new_pointer + position_index;
    }

    /**
     * @brief Ensures that at least required_capacity characters can be stored.
     * @tparam ndebug Contract policy used for capacity validation.
     * @param required_capacity Minimum logical capacity, excluding the terminator.
     */
    template<::pltxt2htm::Contracts ndebug>
    constexpr void ensure_capacity(this BasicString& self, size_type required_capacity) noexcept {
        if (required_capacity <= self.capacity()) {
            return;
        }
        pltxt2htm_assert(required_capacity < self.max_size(), u8"BasicString capacity is too large");

        self.reserve<ndebug>(self.grown_capacity(required_capacity));
    }

    /**
     * @brief Replaces the contents with a counted character sequence.
     * @param source First source character; it may point into self.
     * @param count Number of characters to assign.
     *
     * A valid source inside self can never require capacity growth because its
     * length is bounded by self.size(). Once capacity is sufficient, the runtime
     * path uses overlap-safe copying and therefore needs no alias scan.
     */
    constexpr void assign_impl(this BasicString& self, const_pointer source, size_type count) noexcept {
        if (count == 0) {
            self.clear();
            return;
        }
        if (count > self.capacity()) {
            self.reserve<::pltxt2htm::Contracts::quick_enforce>(count);
        }

        if consteval {
            ::std::copy_n(source, count, self.begin_pointer);
        }
        else {
            ::fast_io::freestanding::overlapped_copy_n(source, count, self.begin_pointer);
        }
        self.current_pointer = self.begin_pointer + count;
        *self.current_pointer = value_type{};
    }

public:
    /// Sentinel used by APIs and internal helpers to represent no position.
    static constexpr size_type npos{::std::numeric_limits<size_type>::max()};

    /**
     * @brief Constructs an empty string without allocating at runtime.
     */
    constexpr BasicString() noexcept {
        this->reset();
    }

    /**
     * @brief Constructs a string containing `count` null characters.
     * @param count Number of logical characters, excluding the trailing terminator.
     *
     * The logical null characters are part of the string; an additional null
     * character is retained at data()[size()] as the terminator.
     */
    constexpr explicit BasicString(size_type count) noexcept {
        if (count == 0) {
            this->reset();
            return;
        }
        this->allocate_zero(count);
        this->current_pointer = this->begin_pointer + count;
    }

    /**
     * @brief Constructs `count` copies of a character.
     * @param count Number of logical characters.
     * @param character Character value to copy.
     */
    constexpr explicit BasicString(size_type count, value_type character) noexcept {
        if (count == 0) {
            this->reset();
            return;
        }
        this->allocate(count);
        ::std::fill_n(this->begin_pointer, count, character);
        this->current_pointer = this->begin_pointer + count;
        *this->current_pointer = value_type{};
    }

    /**
     * @brief Constructs a string from the half-open range `[first, last)`.
     * @param first First character to copy.
     * @param last One past the last character to copy.
     * @pre first and last describe one valid contiguous range, with first <= last.
     */
    constexpr explicit BasicString(const_pointer first, const_pointer last) noexcept {
        this->construct(first, static_cast<size_type>(last - first));
    }

    /**
     * @brief Constructs a string by copying a BasicStringView.
     * @param string Source view; its characters need not be null-terminated.
     */
    constexpr explicit BasicString(string_view_type string) noexcept {
        this->construct(string.data(), string.size());
    }

    /**
     * @brief Constructs a string by copying a fast_io string view.
     * @param string Source view; its characters need not be null-terminated.
     */
    constexpr explicit BasicString(::fast_io::basic_string_view<value_type> string) noexcept
        : BasicString{string_view_type{string}} {
    }

    /**
     * @brief Constructs a string from a null-terminated character array.
     * @tparam size_with_null Array extent including the final null character.
     * @param string Source array.
     *
     * Exactly `size_with_null - 1` characters are copied. Embedded null characters
     * before the final array element remain part of the string.
     */
    template<::std::size_t size_with_null>
    constexpr explicit BasicString(value_type const (&string)[size_with_null]) noexcept {
        static_assert(size_with_null != 0);
        this->construct(string, size_with_null - 1);
    }

    /**
     * @brief Constructs a string from a compile-time literal string wrapper.
     * @tparam size Logical number of characters in the literal wrapper.
     * @param string Source literal string.
     */
    template<::std::size_t size>
    constexpr explicit BasicString(::pltxt2htm::details::BasicLiteralString<value_type, size> const& string) noexcept {
        this->construct(string.data(), string.size());
    }

    /**
     * @brief Constructs a string by copying a fast_io owning string.
     * @tparam OtherAllocator Allocator used by the source string.
     * @param string Source owning string.
     */
    template<typename OtherAllocator>
    constexpr explicit BasicString(
        ::fast_io::containers::basic_string<value_type, OtherAllocator> const& string) noexcept {
        this->construct(string.data(), string.size());
    }

    /**
     * @brief Copy-constructs a string with independent storage.
     * @param other String to copy.
     */
    constexpr BasicString(BasicString const& other) noexcept {
        this->construct(other.data(), other.size());
    }

    /**
     * @brief Move-constructs by transferring the source allocation.
     * @param other String whose storage is transferred.
     *
     * After the operation, other is a valid empty string.
     */
    constexpr BasicString(BasicString&& other) noexcept {
        this->begin_pointer = other.begin_pointer;
        this->current_pointer = other.current_pointer;
        this->end_pointer = other.end_pointer;
        other.reset();
    }

    /**
     * @brief Releases owned storage.
     */
    constexpr ~BasicString() {
        this->destroy();
    }

    /**
     * @brief Copy-assigns the contents of another BasicString.
     * @param other String to copy.
     * @return self.
     *
     * Existing capacity is reused when sufficient. Self-assignment has no effect.
     */
    constexpr auto operator=(this BasicString& self, BasicString const& other) noexcept -> BasicString& {
        if (::std::addressof(self) != ::std::addressof(other)) {
            self.assign_impl(other.data(), other.size());
        }
        return self;
    }

    /**
     * @brief Move-assigns by transferring another string's allocation.
     * @param other String whose storage is transferred.
     * @return self.
     *
     * Existing owned storage is released. After the operation, other is a valid
     * empty string. Self-move-assignment has no effect.
     */
    constexpr auto operator=(this BasicString& self, BasicString&& other) noexcept -> BasicString& {
        if (::std::addressof(self) == ::std::addressof(other)) {
            return self;
        }
        self.destroy();
        self.begin_pointer = other.begin_pointer;
        self.current_pointer = other.current_pointer;
        self.end_pointer = other.end_pointer;
        other.reset();
        return self;
    }

    /**
     * @brief Assigns characters from a non-owning view.
     * @param string Source view, which may refer to self.
     * @return self.
     */
    constexpr auto operator=(this BasicString& self, string_view_type string) noexcept -> BasicString& {
        self.assign_impl(string.data(), string.size());
        return self;
    }

    /**
     * @brief Returns a mutable pointer to the contiguous character storage.
     * @return Pointer to the first logical character, or to the trailing null when empty.
     *
     * The returned allocation always has a null character at data()[size()]. It may
     * be invalidated by any operation that grows or replaces the allocation.
     */
    [[nodiscard]]
    constexpr auto data(this BasicString& self) noexcept -> pointer {
        return self.begin_pointer;
    }

    /**
     * @brief Returns a read-only pointer to the contiguous character storage.
     * @return Pointer to the first logical character, or to the trailing null when empty.
     */
    [[nodiscard]]
    constexpr auto data(this BasicString const& self) noexcept -> const_pointer {
        return self.begin_pointer;
    }

    /**
     * @brief Returns the null-terminated character sequence.
     * @return A pointer p for which p[size()] is value_type{}.
     */
    [[nodiscard]]
    constexpr auto c_str(this BasicString const& self) noexcept -> const_pointer {
        return self.begin_pointer;
    }

    /**
     * @brief Returns the number of logical characters.
     * @return Character count, excluding the trailing null.
     */
    [[nodiscard]]
    constexpr auto size(this BasicString const& self) noexcept -> size_type {
        return static_cast<size_type>(self.current_pointer - self.begin_pointer);
    }

    /**
     * @brief Returns the number of bytes occupied by logical characters.
     * @return size() multiplied by sizeof(value_type), excluding the terminator.
     */
    [[nodiscard]]
    constexpr auto size_bytes(this BasicString const& self) noexcept -> size_type {
        return self.size() * sizeof(value_type);
    }

    /**
     * @brief Returns the number of characters that fit without allocation growth.
     * @return Logical capacity, excluding the trailing-null slot.
     */
    [[nodiscard]]
    constexpr auto capacity(this BasicString const& self) noexcept -> size_type {
        return static_cast<size_type>(self.end_pointer - self.begin_pointer);
    }

    /**
     * @brief Returns the byte capacity available for logical characters.
     * @return capacity() multiplied by sizeof(value_type), excluding the terminator.
     */
    [[nodiscard]]
    constexpr auto capacity_bytes(this BasicString const& self) noexcept -> size_type {
        return self.capacity() * sizeof(value_type);
    }

    /**
     * @brief Returns the implementation's theoretical character-count limit.
     * @return Exclusive upper bound enforced by capacity checks.
     *
     * Actual allocations may fail at substantially smaller sizes.
     */
    [[nodiscard]]
    static constexpr auto max_size() noexcept -> size_type {
        return ::std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    /**
     * @brief Tests whether the string has no logical characters.
     * @return true when size() is zero.
     */
    [[nodiscard]]
    constexpr auto empty(this BasicString const& self) noexcept -> bool {
        return self.begin_pointer == self.current_pointer;
    }

    /**
     * @brief Compatibility spelling for empty().
     * @return true when size() is zero.
     */
    [[nodiscard]]
    constexpr auto is_empty(this BasicString const& self) noexcept -> bool {
        return self.empty();
    }

    /**
     * @brief Returns a mutable iterator to the first character.
     * @return data().
     */
    [[nodiscard]]
    constexpr auto begin(this BasicString& self) noexcept -> iterator {
        return self.begin_pointer;
    }

    /**
     * @brief Returns a read-only iterator to the first character.
     * @return data().
     */
    [[nodiscard]]
    constexpr auto begin(this BasicString const& self) noexcept -> const_iterator {
        return self.begin_pointer;
    }

    /**
     * @brief Returns a read-only iterator to the first character.
     * @return data().
     */
    [[nodiscard]]
    constexpr auto cbegin(this BasicString const& self) noexcept -> const_iterator {
        return self.begin_pointer;
    }

    /**
     * @brief Returns a mutable iterator one past the last logical character.
     * @return Pointer to the trailing null character.
     */
    [[nodiscard]]
    constexpr auto end(this BasicString& self) noexcept -> iterator {
        return self.current_pointer;
    }

    /**
     * @brief Returns a read-only iterator one past the last logical character.
     * @return Pointer to the trailing null character.
     */
    [[nodiscard]]
    constexpr auto end(this BasicString const& self) noexcept -> const_iterator {
        return self.current_pointer;
    }

    /**
     * @brief Returns a read-only iterator one past the last logical character.
     * @return Pointer to the trailing null character.
     */
    [[nodiscard]]
    constexpr auto cend(this BasicString const& self) noexcept -> const_iterator {
        return self.current_pointer;
    }

    /**
     * @brief Returns a mutable reverse iterator to the last character.
     * @return Reverse iterator constructed from end().
     */
    [[nodiscard]]
    constexpr auto rbegin(this BasicString& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.end()};
    }

    /**
     * @brief Returns a read-only reverse iterator to the last character.
     * @return Reverse iterator constructed from end().
     */
    [[nodiscard]]
    constexpr auto rbegin(this BasicString const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.end()};
    }

    /**
     * @brief Returns a mutable reverse iterator past the first character.
     * @return Reverse iterator constructed from begin().
     */
    [[nodiscard]]
    constexpr auto rend(this BasicString& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.begin()};
    }

    /**
     * @brief Returns a read-only reverse iterator past the first character.
     * @return Reverse iterator constructed from begin().
     */
    [[nodiscard]]
    constexpr auto rend(this BasicString const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.begin()};
    }

    /**
     * @brief Returns a mutable character with contract-controlled bounds checking.
     * @tparam ndebug Contract policy used when position is out of bounds.
     * @param position Zero-based character position.
     * @return Reference to the requested character.
     * @pre position < size().
     */
    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto index(this BasicString& self, size_type position) noexcept -> reference {
        pltxt2htm_assert(position < self.size(), u8"Index of BasicString out of bound");
        return self.begin_pointer[position];
    }

    /**
     * @brief Returns a read-only character with contract-controlled bounds checking.
     * @tparam ndebug Contract policy used when position is out of bounds.
     * @param position Zero-based character position.
     * @return Read-only reference to the requested character.
     * @pre position < size().
     */
    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto index(this BasicString const& self, size_type position) noexcept -> const_reference {
        pltxt2htm_assert(position < self.size(), u8"Index of BasicString out of bound");
        return self.begin_pointer[position];
    }

    /**
     * @brief Disabled to require an explicit Contracts policy through index().
     *
     * A templated operator[] would require the awkward explicit spelling
     * `string.template operator[]<policy>(position)`. Keeping operator[] deleted
     * makes the contract-bearing index() API explicit and consistent with
     * BasicStringView.
     */
    constexpr auto operator[](this BasicString& self, size_type position) noexcept -> reference = delete
#if __cpp_deleted_function >= 202403L
        ("operator[] is deleted; use index() instead for bounds-checked access")
#endif
        ;

    /**
     * @brief Returns the first character.
     * @tparam ndebug Contract policy used for the empty-string check.
     * @return Mutable reference to the first character.
     * @pre The string is not empty.
     */
    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto front(this BasicString& self) noexcept -> reference {
        return self.index<ndebug>(0);
    }

    /**
     * @brief Returns the first character.
     * @tparam ndebug Contract policy used for the empty-string check.
     * @return Read-only reference to the first character.
     * @pre The string is not empty.
     */
    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto front(this BasicString const& self) noexcept -> const_reference {
        return self.index<ndebug>(0);
    }

    /**
     * @brief Returns the last character.
     * @tparam ndebug Contract policy used for the empty-string check.
     * @return Mutable reference to the last character.
     * @pre The string is not empty.
     */
    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto back(this BasicString& self) noexcept -> reference {
        pltxt2htm_assert(!self.empty(), u8"back() called on empty BasicString");
        return self.current_pointer[-1];
    }

    /**
     * @brief Returns the last character.
     * @tparam ndebug Contract policy used for the empty-string check.
     * @return Read-only reference to the last character.
     * @pre The string is not empty.
     */
    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto back(this BasicString const& self) noexcept -> const_reference {
        pltxt2htm_assert(!self.empty(), u8"back() called on empty BasicString");
        return self.current_pointer[-1];
    }

    /**
     * @brief Removes all logical characters while retaining allocated capacity.
     *
     * The first storage slot becomes the trailing null character. This operation
     * performs no allocation.
     */
    constexpr void clear(this BasicString& self) noexcept {
        if (self.begin_pointer == self.end_pointer) {
            return;
        }
        self.current_pointer = self.begin_pointer;
        *self.current_pointer = value_type{};
    }

    /**
     * @brief Ensures storage for at least requested_capacity logical characters.
     * @tparam ndebug Contract policy used to reject an unrepresentable capacity.
     * @param requested_capacity Minimum capacity, excluding the trailing-null slot.
     *
     * Has no effect when requested_capacity <= capacity(). At runtime the allocator's
     * native reallocation operation is used when available; otherwise its counted
     * reallocation fallback is used. Callers must assume all pointers, references,
     * and iterators are invalidated whenever capacity grows, even if the allocator
     * happens to extend the allocation in place.
     */
    template<::pltxt2htm::Contracts ndebug>
    constexpr void reserve(this BasicString& self, size_type requested_capacity) noexcept {
        if (requested_capacity <= self.capacity()) {
            return;
        }
        pltxt2htm_assert(requested_capacity < self.max_size(), u8"BasicString capacity is too large");

        size_type const old_size{self.size()};
        size_type const old_capacity{self.capacity()};
        pointer new_pointer{};
        size_type allocated_size{};
        if consteval {
            auto allocation = typed_allocator_type::allocate_at_least(requested_capacity + 1);
            new_pointer = allocation.ptr;
            allocated_size = allocation.count;
            self.start_lifetime(new_pointer, allocated_size);
            ::std::copy(self.begin_pointer, self.current_pointer, new_pointer);
            if (old_capacity != 0) {
                typed_allocator_type::deallocate_n(self.begin_pointer, old_capacity + 1);
            }
        }
        else {
            pointer const allocation_pointer{old_capacity == 0 ? nullptr : self.begin_pointer};
            if constexpr (typed_allocator_type::has_reallocate) {
                auto allocation = typed_allocator_type::reallocate_at_least(allocation_pointer, requested_capacity + 1);
                new_pointer = allocation.ptr;
                allocated_size = allocation.count;
            }
            else {
                auto allocation = typed_allocator_type::reallocate_n_at_least(
                    allocation_pointer, old_capacity == 0 ? 0 : old_capacity + 1, requested_capacity + 1);
                new_pointer = allocation.ptr;
                allocated_size = allocation.count;
            }
        }
        self.begin_pointer = new_pointer;
        self.current_pointer = new_pointer + old_size;
        self.end_pointer = new_pointer + static_cast<size_type>(allocated_size - 1);
        *self.current_pointer = value_type{};
    }

    /**
     * @brief Replaces the contents with a view.
     * @param string Source view, which may refer to self.
     *
     * Existing capacity is retained and reused when sufficient.
     */
    constexpr void assign(this BasicString& self, string_view_type string) noexcept {
        self.assign_impl(string.data(), string.size());
    }

    /**
     * @brief Replaces the contents with the half-open range `[first, last)`.
     * @param first First source character.
     * @param last One past the last source character.
     * @pre first and last describe one valid contiguous range, with first <= last.
     */
    constexpr void assign(this BasicString& self, const_pointer first, const_pointer last) noexcept {
        self.assign_impl(first, static_cast<size_type>(last - first));
    }

    /**
     * @brief Replaces the contents with repeated copies of one character.
     * @param count Number of logical characters to store.
     * @param character Character value; defaults to the null character.
     *
     * Existing capacity is retained and reused when sufficient. A zero count is
     * equivalent to clear().
     */
    constexpr void assign_characters(this BasicString& self, size_type count,
                                     value_type character = value_type{}) noexcept {
        if (count == 0) {
            self.clear();
            return;
        }
        self.ensure_capacity<::pltxt2htm::Contracts::quick_enforce>(count);
        ::std::fill_n(self.begin_pointer, count, character);
        self.current_pointer = self.begin_pointer + count;
        *self.current_pointer = value_type{};
    }

    /**
     * @brief Replaces the contents with a single character.
     * @param character Character value to store.
     */
    constexpr void assign_with_character(this BasicString& self, value_type character) noexcept {
        self.assign_characters(1, character);
    }

    /**
     * @brief Appends one character.
     * @param character Character value to append.
     *
     * The hot path only advances current_pointer and rewrites the terminator.
     * Pointers, references, and iterators remain valid unless capacity growth occurs.
     */
    constexpr void push_back(this BasicString& self, value_type character) noexcept {
        if (self.current_pointer == self.end_pointer) [[unlikely]] {
            self.ensure_capacity<::pltxt2htm::Contracts::quick_enforce>(self.size() + 1);
        }
        *self.current_pointer++ = character;
        *self.current_pointer = value_type{};
    }

    /**
     * @brief Removes the final character.
     * @tparam ndebug Contract policy used for the empty-string check.
     * @pre The string is not empty.
     */
    template<::pltxt2htm::Contracts ndebug>
    constexpr void pop_back(this BasicString& self) noexcept {
        pltxt2htm_assert(!self.empty(), u8"pop_back() called on empty BasicString");
        --self.current_pointer;
        *self.current_pointer = value_type{};
    }

    /**
     * @brief Appends the half-open range `[first, last)`.
     * @param first First source character.
     * @param last One past the last source character.
     * @pre first and last describe one valid contiguous range, with first <= last.
     *
     * The source range may refer to self. Capacity-preserving appends use
     * overlap-safe copying; growth appends consume the old source before releasing
     * the old allocation. Pointers, references, and iterators are invalidated when
     * growth occurs.
     */
    constexpr void append(this BasicString& self, const_pointer first, const_pointer last) noexcept {
        size_type const count{static_cast<size_type>(last - first)};
        if (count == 0) {
            return;
        }
        size_type const old_size{self.size()};
        if (count > static_cast<size_type>(self.end_pointer - self.current_pointer)) [[unlikely]] {
            self.append_reallocate<::pltxt2htm::Contracts::quick_enforce>(first, count);
            return;
        }
        if consteval {
            ::std::copy_n(first, count, self.current_pointer);
        }
        else {
            ::fast_io::freestanding::overlapped_copy_n(first, count, self.current_pointer);
        }
        self.current_pointer = self.begin_pointer + old_size + count;
        *self.current_pointer = value_type{};
    }

    /**
     * @brief Appends a counted character sequence.
     * @param first First source character.
     * @param count Number of characters to append.
     * @pre `[first, first + count)` is a valid contiguous range.
     */
    constexpr void append(this BasicString& self, const_pointer first, size_type count) noexcept {
        self.append(first, first + count);
    }

    /**
     * @brief Appends a BasicStringView.
     * @param string Source view, which may refer to self.
     */
    constexpr void append(this BasicString& self, string_view_type string) noexcept {
        self.append(string.data(), string.data() + string.size());
    }

    /**
     * @brief Appends another BasicString.
     * @param string Source string. Passing self is supported.
     */
    constexpr void append(this BasicString& self, BasicString const& string) noexcept {
        self.append(string.data(), string.data() + string.size());
    }

    /**
     * @brief Appends a null-terminated character array without its final null.
     * @tparam size_with_null Array extent including the final null character.
     * @param string Source array.
     */
    template<::std::size_t size_with_null>
    constexpr void append(this BasicString& self, value_type const (&string)[size_with_null]) noexcept {
        static_assert(size_with_null != 0);
        self.append(string, string + size_with_null - 1);
    }

    /**
     * @brief Appends a compile-time literal string wrapper.
     * @tparam size Logical number of characters in the literal wrapper.
     * @param string Source literal string.
     */
    template<::std::size_t size>
    constexpr void append(this BasicString& self,
                          ::pltxt2htm::details::BasicLiteralString<value_type, size> const& string) noexcept {
        self.append(string.data(), string.data() + string.size());
    }

    /**
     * @brief Inserts a view before position.
     * @tparam ndebug Contract policy used to validate position and capacity.
     * @param position Iterator identifying the insertion point.
     * @param string Characters to insert; the view may refer to self.
     * @return Iterator to the first inserted character, or position when string is empty.
     * @pre position is in the closed range `[begin(), end()]`.
     *
     * When capacity is sufficient, the suffix is moved in place and internal
     * source offsets are adjusted to their post-move positions. When growth is
     * required, prefix, source, and suffix are assembled in a new allocation before
     * the old one is released. Any growth invalidates all previous iterators.
     */
    template<::pltxt2htm::Contracts ndebug>
    constexpr auto insert(this BasicString& self, const_iterator position, string_view_type string) noexcept
        -> iterator {
        pltxt2htm_assert(position >= self.begin_pointer && position <= self.current_pointer,
                         u8"BasicString insert position out of bound");
        size_type const position_index{static_cast<size_type>(position - self.begin_pointer)};
        if (string.empty()) {
            return self.begin_pointer + position_index;
        }

        size_type const old_size{self.size()};
        if (string.size() > static_cast<size_type>(self.end_pointer - self.current_pointer)) [[unlikely]] {
            return self.insert_reallocate<ndebug>(position_index, string);
        }
        size_type const source_index{self.source_offset(string.data())};
        pointer const insertion_pointer{self.begin_pointer + position_index};
        ::std::move_backward(insertion_pointer, self.begin_pointer + old_size,
                             self.begin_pointer + old_size + string.size());
        const_pointer source{string.data()};
        if (source_index != self.npos && source_index >= position_index) {
            source = self.begin_pointer + source_index + string.size();
        }
        if consteval {
            if (source_index != self.npos && source_index < position_index &&
                position_index < source_index + string.size()) {
                ::std::copy_backward(source, source + string.size(), insertion_pointer + string.size());
            }
            else {
                ::std::copy_n(source, string.size(), insertion_pointer);
            }
        }
        else {
            ::fast_io::freestanding::overlapped_copy_n(source, string.size(), insertion_pointer);
        }
        self.current_pointer = self.begin_pointer + old_size + string.size();
        *self.current_pointer = value_type{};
        return insertion_pointer;
    }

    /**
     * @brief Creates a non-owning BasicStringView of this lvalue string.
     * @return View spanning `[data(), data() + size())`.
     *
     * The view is invalidated by allocation growth, replacement, destruction, or
     * any other operation that invalidates the referenced characters.
     */
    [[nodiscard]]
    constexpr operator string_view_type(this BasicString const& self) noexcept {
        return string_view_type{self.begin_pointer, self.size()};
    }

    /// Prevents creating a dangling BasicStringView from a temporary string.
    constexpr operator string_view_type(this BasicString&&) = delete;

    /// Prevents creating a dangling BasicStringView from a const temporary string.
    constexpr operator string_view_type(this BasicString const&&) = delete;

    /**
     * @brief Creates a fast_io string view of this string.
     * @return Non-owning fast_io view of the logical characters.
     *
     * As with data(), allocation-changing operations invalidate the returned view.
     * Converting a temporary BasicString would produce a dangling view and must be
     * avoided by the caller.
     */
    [[nodiscard]]
    constexpr operator ::fast_io::basic_string_view<value_type>() const noexcept {
        return ::fast_io::basic_string_view<value_type>{this->begin_pointer, this->size()};
    }

    /**
     * @brief Compares two strings by size and character values.
     * @param other Right-hand string.
     * @return true when both strings contain the same character sequence.
     */
    [[nodiscard]]
    constexpr auto operator==(this BasicString const& self, BasicString const& other) noexcept -> bool {
        return string_view_type{self} == string_view_type{other};
    }

    /**
     * @brief Compares a string with a non-owning view.
     * @param other Right-hand view.
     * @return true when both sequences have equal size and characters.
     */
    [[nodiscard]]
    constexpr auto operator==(this BasicString const& self, string_view_type other) noexcept -> bool {
        return string_view_type{self} == other;
    }
};

/** @name BasicString deduction guides
 * @{
 */
template<::pltxt2htm::details::is_char_type CharType, ::std::size_t size_with_null>
BasicString(CharType const (&)[size_with_null]) -> BasicString<CharType>;

template<::pltxt2htm::details::is_char_type CharType>
BasicString(CharType const*, CharType const*) -> BasicString<CharType>;

template<::pltxt2htm::details::is_char_type CharType>
BasicString(BasicStringView<CharType>) -> BasicString<CharType>;

template<::pltxt2htm::details::is_char_type CharType>
BasicString(::fast_io::basic_string_view<CharType>) -> BasicString<CharType>;

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
BasicString(::fast_io::containers::basic_string<CharType, Allocator> const&) -> BasicString<CharType>;

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t size>
BasicString(::pltxt2htm::details::BasicLiteralString<CharType, size> const&) -> BasicString<CharType>;
/** @} */

using String = BasicString<char>; ///< Narrow-character owning string.
using WString = BasicString<wchar_t>; ///< Wide-character owning string.
using U8String = BasicString<char8_t>; ///< UTF-8 code-unit owning string.
using U16String = BasicString<char16_t>; ///< UTF-16 code-unit owning string.
using U32String = BasicString<char32_t>; ///< UTF-32 code-unit owning string.

/**
 * @brief Exposes BasicString as a fast_io scatter for zero-copy output.
 * @tparam CharType Character type stored by the string.
 * @tparam Allocator Allocator used by the string.
 * @param string_ String to expose.
 * @return Scatter containing data() and size(), excluding the trailing null.
 */
template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto print_alias_define(::fast_io::io_alias_t, BasicString<CharType, Allocator> const& string_) noexcept
    -> ::fast_io::basic_io_scatter_t<CharType> {
    return {string_.data(), string_.size()};
}

namespace details {

struct BasicStringScanContext {
    bool copying{};
};

template<bool noskipws, bool line>
constexpr ::fast_io::manipulators::scalar_flags basic_string_default_scalar_flags{.noskipws = noskipws, .line = line};

template<bool noskipws, bool line, ::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto scan_context_define_basic_string(bool& copying, CharType const* first, CharType const* last,
                                                BasicString<CharType, Allocator>& string_) noexcept
    -> ::fast_io::parse_result<CharType const*> {
    auto iterator{first};
    if constexpr (!noskipws && !line) {
        if (!copying) {
            iterator = ::fast_io::find_none_c_space(iterator, last);
            if (iterator == last) {
                return {iterator, ::fast_io::parse_code::partial};
            }
            copying = true;
            string_.clear();
        }
    }

    auto end_iterator{iterator};
    if constexpr (line) {
        end_iterator = ::fast_io::find_lf(end_iterator, last);
    }
    else {
        end_iterator = ::fast_io::find_c_space(end_iterator, last);
    }

    if constexpr (noskipws || line) {
        if (!copying) {
            string_.assign(iterator, end_iterator);
            copying = true;
        }
        else {
            string_.append(iterator, end_iterator);
        }
    }
    else {
        string_.append(iterator, end_iterator);
    }

    if (end_iterator == last) {
        return {end_iterator, ::fast_io::parse_code::partial};
    }
    if constexpr (line) {
        ++end_iterator;
    }
    return {end_iterator, ::fast_io::parse_code::ok};
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto scan_context_define_whole_basic_string(bool& copying, CharType const* first, CharType const* last,
                                                      BasicString<CharType, Allocator>& string_) noexcept
    -> ::fast_io::parse_result<CharType const*> {
    if (!copying) {
        string_.assign(first, last);
        copying = true;
    }
    else {
        string_.append(first, last);
    }
    return {last, ::fast_io::parse_code::partial};
}

} // namespace details

template<::pltxt2htm::details::is_char_type CharType, ::fast_io::manipulators::scalar_flags flags, typename Allocator>
constexpr auto scan_context_type(
    ::fast_io::io_reserve_type_t<
        CharType, ::fast_io::manipulators::scalar_manip_t<flags, BasicString<CharType, Allocator>&>>) noexcept
    -> ::fast_io::io_type_t<::pltxt2htm::container::details::BasicStringScanContext> {
    return {};
}

template<::pltxt2htm::details::is_char_type CharType, ::fast_io::manipulators::scalar_flags flags, typename Allocator>
constexpr auto scan_context_define(
    ::fast_io::io_reserve_type_t<CharType,
                                 ::fast_io::manipulators::scalar_manip_t<flags, BasicString<CharType, Allocator>&>>,
    ::pltxt2htm::container::details::BasicStringScanContext& context, CharType const* first, CharType const* last,
    ::fast_io::manipulators::scalar_manip_t<flags, BasicString<CharType, Allocator>&> string_) noexcept
    -> ::fast_io::parse_result<CharType const*> {
    return ::pltxt2htm::container::details::scan_context_define_basic_string<flags.noskipws, flags.line>(
        context.copying, first, last, string_.reference);
}

template<::fast_io::manipulators::scalar_flags flags, ::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto scan_context_eof_define(
    ::fast_io::io_reserve_type_t<CharType,
                                 ::fast_io::manipulators::scalar_manip_t<flags, BasicString<CharType, Allocator>&>>,
    ::pltxt2htm::container::details::BasicStringScanContext context,
    ::fast_io::manipulators::scalar_manip_t<flags, BasicString<CharType, Allocator>&>) noexcept
    -> ::fast_io::parse_code {
    return context.copying ? ::fast_io::parse_code::ok : ::fast_io::parse_code::end_of_file;
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto scan_alias_define(::fast_io::io_alias_t, BasicString<CharType, Allocator>& string_) noexcept
    -> ::fast_io::manipulators::scalar_manip_t<
        ::pltxt2htm::container::details::basic_string_default_scalar_flags<false, false>,
        BasicString<CharType, Allocator>&> {
    return {string_};
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto scan_context_type(
    ::fast_io::io_reserve_type_t<CharType,
                                 ::fast_io::manipulators::whole_get_t<BasicString<CharType, Allocator>&>>) noexcept
    -> ::fast_io::io_type_t<::pltxt2htm::container::details::BasicStringScanContext> {
    return {};
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto scan_context_define(
    ::fast_io::io_reserve_type_t<CharType, ::fast_io::manipulators::whole_get_t<BasicString<CharType, Allocator>&>>,
    ::pltxt2htm::container::details::BasicStringScanContext& context, CharType const* first, CharType const* last,
    ::fast_io::manipulators::whole_get_t<BasicString<CharType, Allocator>&> string_) noexcept
    -> ::fast_io::parse_result<CharType const*> {
    return ::pltxt2htm::container::details::scan_context_define_whole_basic_string(context.copying, first, last,
                                                                                   string_.reference);
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto scan_context_eof_define(
    ::fast_io::io_reserve_type_t<CharType, ::fast_io::manipulators::whole_get_t<BasicString<CharType, Allocator>&>>,
    ::pltxt2htm::container::details::BasicStringScanContext context,
    ::fast_io::manipulators::whole_get_t<BasicString<CharType, Allocator>&> string_) noexcept -> ::fast_io::parse_code {
    if (!context.copying) {
        string_.reference.clear();
    }
    return ::fast_io::parse_code::ok;
}

} // namespace pltxt2htm::container

namespace fast_io::manipulators {

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto line_get(::pltxt2htm::container::BasicString<CharType, Allocator>& string_) noexcept
    -> ::fast_io::manipulators::scalar_manip_t<
        ::pltxt2htm::container::details::basic_string_default_scalar_flags<false, true>,
        ::pltxt2htm::container::BasicString<CharType, Allocator>&> {
    return {string_};
}

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
constexpr auto whole_get(::pltxt2htm::container::BasicString<CharType, Allocator>& string_) noexcept
    -> ::fast_io::manipulators::whole_get_t<::pltxt2htm::container::BasicString<CharType, Allocator>&> {
    return {string_};
}

} // namespace fast_io::manipulators

#include "../details/pop_macro.hh"
