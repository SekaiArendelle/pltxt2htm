/**
 * @file deque_decl.hh
 * @brief Declarations for container::Deque.
 * @details Contract-checked members are declared here and defined in
 *          deque_def.inc. Members defined inline that call those members also
 *          need deque_def.inc in the same translation unit; include
 *          container/deque.hh for the complete API.
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
#include <type_traits>
#include <utility>

#include <fast_io/fast_io_core.h>

#include "../../details/contracts.hh"

namespace pltxt2htm::container {

/**
 * @brief A random-access sequence with constant-time insertion and removal at both ends.
 * @tparam T Element type.
 * @tparam Allocator Stateless fast_io allocator adapter used for all storage.
 *
 * Elements are stored in independently allocated fixed-size blocks. Growing the block map
 * moves only block pointers, so references and pointers to existing elements remain valid
 * across push_front and push_back. Element and iterator operations used by Deque must not throw.
 * Iterators to elements follow storage across swap and move. Insertion, shrink_to_fit, clear,
 * and middle erasure invalidate iterators; erasure at an end invalidates only erased elements
 * and the past-the-end iterator.
 */
template<typename T, typename Allocator = ::fast_io::native_global_allocator>
class Deque {
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = ::std::size_t;
    using difference_type = ::std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;

private:
    using element_allocator = ::fast_io::typed_generic_allocator_adapter<allocator_type, value_type>;
    using map_allocator = ::fast_io::typed_generic_allocator_adapter<allocator_type, pointer>;

    static_assert(!element_allocator::has_status,
                  "Deque requires a stateless fast_io allocator; stateful allocators need an external handle");
    static_assert(::std::is_nothrow_default_constructible_v<allocator_type>,
                  "Deque requires a nothrow-default-constructible allocator");
    static_assert(::std::is_nothrow_destructible_v<value_type>, "Deque requires nothrow-destructible elements");

    static consteval auto calculate_elements_per_block() noexcept -> size_type {
        constexpr size_type target_block_bytes{1024};
        if constexpr (sizeof(value_type) >= target_block_bytes) {
            return 1;
        }
        else {
            return target_block_bytes / sizeof(value_type);
        }
    }

public:
    static constexpr size_type elements_per_block{calculate_elements_per_block()};

private:
    static constexpr size_type initial_map_capacity{8};
    // Bound both map_capacity * elements_per_block (element offsets) and
    // map_capacity * sizeof(pointer) (map allocation bytes) to size_type.
    static constexpr size_type max_map_capacity{::std::min(::std::numeric_limits<size_type>::max() / elements_per_block,
                                                           ::std::numeric_limits<size_type>::max() / sizeof(pointer))};

    pointer* blocks{};
    size_type map_capacity{};
    size_type start_offset{};
    size_type element_count{};

    template<typename InputIterator, typename Sentinel>
    [[nodiscard]]
    static consteval auto is_nothrow_input_range() noexcept -> bool {
        if constexpr (!requires(InputIterator& iterator, Sentinel& sentinel) {
                          { *iterator } noexcept;
                          { ++iterator } noexcept;
                          { static_cast<bool>(iterator != sentinel) } noexcept -> ::std::same_as<bool>;
                      }) {
            return false;
        }
        return ::std::is_nothrow_copy_constructible_v<InputIterator> &&
               ::std::is_nothrow_move_constructible_v<InputIterator> &&
               ::std::is_nothrow_copy_constructible_v<Sentinel> && ::std::is_nothrow_move_constructible_v<Sentinel> &&
               ::std::is_nothrow_constructible_v<value_type, ::std::iter_reference_t<InputIterator>>;
    }

    [[nodiscard]]
    constexpr auto allocated_block_count(this Deque const& self) noexcept -> size_type {
        if (self.element_count == 0) {
            return 0;
        }
        return (self.start_offset % elements_per_block + self.element_count - 1) / elements_per_block + 1;
    }

    // Live blocks are contiguous in the map. At most one empty allocated block may sit
    // immediately before or after them; an empty Deque uses the same two positions.
    [[nodiscard]]
    constexpr auto has_front_spare(this Deque const& self) noexcept -> bool {
        size_type const first_block{self.start_offset / elements_per_block};
        return first_block != 0 && self.blocks[first_block - 1] != nullptr;
    }

    [[nodiscard]]
    constexpr auto has_back_spare(this Deque const& self) noexcept -> bool {
        size_type const back_block{self.start_offset / elements_per_block + self.allocated_block_count()};
        return back_block < self.map_capacity && self.blocks[back_block] != nullptr;
    }

    [[nodiscard]]
    constexpr auto pointer_at(this Deque& self, size_type index) noexcept -> pointer {
        size_type const offset{self.start_offset + index};
        return self.blocks[offset / elements_per_block] + offset % elements_per_block;
    }

    [[nodiscard]]
    constexpr auto pointer_at(this Deque const& self, size_type index) noexcept -> const_pointer {
        size_type const offset{self.start_offset + index};
        return self.blocks[offset / elements_per_block] + offset % elements_per_block;
    }

    constexpr void reverse_elements(this Deque& self, size_type first, size_type last) noexcept
        requires (::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_move_assignable_v<value_type>)
    {
        while (first < last) {
            --last;
            if (first == last) {
                break;
            }
            value_type displaced{::std::move(*self.pointer_at(first))};
            *self.pointer_at(first) = ::std::move(*self.pointer_at(last));
            *self.pointer_at(last) = ::std::move(displaced);
            ++first;
        }
    }

    constexpr void rotate_elements(this Deque& self, size_type first, size_type middle, size_type last) noexcept
        requires (::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_move_assignable_v<value_type>)
    {
        if (first == middle || middle == last) {
            return;
        }
        // Keep rotation within the noexcept move requirements; iter_swap may call a throwing ADL swap.
        self.reverse_elements(first, middle);
        self.reverse_elements(middle, last);
        self.reverse_elements(first, last);
    }

    constexpr void allocate_map(this Deque& self, size_type capacity) noexcept {
        self.blocks = map_allocator::allocate(capacity);
        self.map_capacity = capacity;
        self.start_offset = capacity / 2 * elements_per_block;
        for (size_type index{}; index != capacity; ++index) {
            ::std::construct_at(self.blocks + index, nullptr);
        }
    }

    constexpr void relocate_map(this Deque& self, size_type new_capacity, size_type new_first_block) noexcept {
        pointer* const new_blocks{map_allocator::allocate(new_capacity)};
        for (size_type index{}; index != new_capacity; ++index) {
            ::std::construct_at(new_blocks + index, nullptr);
        }

        size_type const used_blocks{self.allocated_block_count()};
        size_type const first_block{self.start_offset / elements_per_block};
        size_type const front_spare{self.has_front_spare() ? 1u : 0u};
        size_type const back_spare{self.has_back_spare() ? 1u : 0u};
        size_type const occupied_blocks{used_blocks + front_spare + back_spare};
        for (size_type index{}; index != occupied_blocks; ++index) {
            new_blocks[new_first_block - front_spare + index] = self.blocks[first_block - front_spare + index];
        }

        map_allocator::deallocate_n(self.blocks, self.map_capacity);

        self.blocks = new_blocks;
        self.map_capacity = new_capacity;
        self.start_offset = new_first_block * elements_per_block + self.start_offset % elements_per_block;
    }

    constexpr void grow_map(this Deque& self, size_type required_front, size_type required_back) noexcept;

    constexpr void ensure_map(this Deque& self) noexcept {
        if (self.blocks == nullptr) {
            self.allocate_map(initial_map_capacity);
        }
    }

    constexpr void reset_empty_position(this Deque& self) noexcept {
        size_type const first_block{self.start_offset / elements_per_block};
        pointer const front_spare{self.has_front_spare() ? self.blocks[first_block - 1] : nullptr};
        pointer const back_spare{self.has_back_spare() ? self.blocks[first_block] : nullptr};
        if (front_spare != nullptr) {
            self.blocks[first_block - 1] = nullptr;
        }
        if (back_spare != nullptr) {
            self.blocks[first_block] = nullptr;
        }
        size_type const new_first_block{self.map_capacity / 2};
        if (front_spare != nullptr) {
            self.blocks[new_first_block - 1] = front_spare;
        }
        if (back_spare != nullptr) {
            self.blocks[new_first_block] = back_spare;
        }
        self.start_offset = new_first_block * elements_per_block;
    }

    constexpr void destroy_storage(this Deque& self) noexcept {
        self.clear();
        self.shrink_to_fit();
    }

public:
    template<bool is_const>
    class BasicIterator {
        T** block{};
        size_type block_offset{};

        constexpr BasicIterator(T** block_, size_type offset_) noexcept
            : block{block_},
              block_offset{offset_} {
        }

        friend class Deque;
        template<bool>
        friend class BasicIterator;

    public:
        using iterator_category = ::std::random_access_iterator_tag;
        using iterator_concept = ::std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = ::std::ptrdiff_t;
        using reference = ::std::conditional_t<is_const, T const&, T&>;
        using pointer = ::std::conditional_t<is_const, T const*, T*>;

        constexpr BasicIterator() noexcept = default;

        template<bool other_const>
            requires (is_const && !other_const)
        constexpr BasicIterator(BasicIterator<other_const> const& other) noexcept
            : block{other.block},
              block_offset{other.block_offset} {
        }

        [[nodiscard]]
        constexpr auto operator*(this BasicIterator const& self) noexcept -> reference {
            return (*self.block)[self.block_offset];
        }

        [[nodiscard]]
        constexpr auto operator->(this BasicIterator const& self) noexcept -> pointer {
            return ::std::addressof(*self);
        }

        [[nodiscard]]
        constexpr auto operator[](this BasicIterator const& self, difference_type offset) noexcept -> reference {
            return *(self + offset);
        }

        constexpr auto operator++(this BasicIterator& self) noexcept -> BasicIterator& {
            self += 1;
            return self;
        }

        constexpr auto operator++(this BasicIterator& self, int) noexcept -> BasicIterator {
            BasicIterator old{self};
            ++self;
            return old;
        }

        constexpr auto operator--(this BasicIterator& self) noexcept -> BasicIterator& {
            self -= 1;
            return self;
        }

        constexpr auto operator--(this BasicIterator& self, int) noexcept -> BasicIterator {
            BasicIterator old{self};
            --self;
            return old;
        }

        constexpr auto operator+=(this BasicIterator& self, difference_type offset) noexcept -> BasicIterator& {
            constexpr difference_type block_size{static_cast<difference_type>(elements_per_block)};
            difference_type block_delta{offset / block_size};
            difference_type new_offset{static_cast<difference_type>(self.block_offset) + offset % block_size};
            if (new_offset < 0) {
                new_offset += block_size;
                --block_delta;
            }
            else if (new_offset >= block_size) {
                new_offset -= block_size;
                ++block_delta;
            }
            if (block_delta != 0) {
                self.block += block_delta;
            }
            self.block_offset = static_cast<size_type>(new_offset);
            return self;
        }

        constexpr auto operator-=(this BasicIterator& self, difference_type offset) noexcept -> BasicIterator& {
            return self += -offset;
        }

        [[nodiscard]]
        friend constexpr auto operator+(BasicIterator iterator, difference_type offset) noexcept -> BasicIterator {
            iterator += offset;
            return iterator;
        }

        [[nodiscard]]
        friend constexpr auto operator+(difference_type offset, BasicIterator iterator) noexcept -> BasicIterator {
            iterator += offset;
            return iterator;
        }

        [[nodiscard]]
        friend constexpr auto operator-(BasicIterator iterator, difference_type offset) noexcept -> BasicIterator {
            iterator -= offset;
            return iterator;
        }

        template<bool other_const>
        [[nodiscard]]
        constexpr auto operator-(this BasicIterator const& self, BasicIterator<other_const> other) noexcept
            -> difference_type {
            difference_type const offset_delta{static_cast<difference_type>(self.block_offset) -
                                               static_cast<difference_type>(other.block_offset)};
            if (self.block == other.block) {
                return offset_delta;
            }
            constexpr difference_type block_size{static_cast<difference_type>(elements_per_block)};
            difference_type const block_delta{self.block - other.block};
            // Normalize the partial block before multiplying, so an otherwise representable
            // distance near max_size() does not overflow in the intermediate product.
            if (block_delta > 0 && offset_delta < 0) {
                return (block_delta - 1) * block_size + (block_size + offset_delta);
            }
            if (block_delta < 0 && offset_delta > 0) {
                return (block_delta + 1) * block_size - (block_size - offset_delta);
            }
            return block_delta * block_size + offset_delta;
        }

        template<bool other_const>
        [[nodiscard]]
        constexpr auto operator==(this BasicIterator const& self, BasicIterator<other_const> other) noexcept -> bool {
            return self.block == other.block && self.block_offset == other.block_offset;
        }

        template<bool other_const>
        [[nodiscard]]
        constexpr auto operator<=>(this BasicIterator const& self, BasicIterator<other_const> other) noexcept
            -> ::std::strong_ordering {
            if (auto const order = self.block <=> other.block; order != 0) {
                return order;
            }
            return self.block_offset <=> other.block_offset;
        }
    };

    using iterator = BasicIterator<false>;
    using const_iterator = BasicIterator<true>;
    using reverse_iterator = ::std::reverse_iterator<iterator>;
    using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;

    constexpr Deque() noexcept = default;

    constexpr explicit Deque(size_type count) noexcept
        requires ::std::is_nothrow_default_constructible_v<value_type>
    {
        for (size_type index{}; index != count; ++index) {
            this->emplace_back();
        }
    }

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
        requires (is_nothrow_input_range<InputIterator, Sentinel>())
    constexpr Deque(InputIterator first, Sentinel last) noexcept {
        for (; first != last; ++first) {
            this->emplace_back(*first);
        }
    }

    constexpr Deque(::std::initializer_list<value_type> values) noexcept
        requires ::std::is_nothrow_copy_constructible_v<value_type>
        : Deque(values.begin(), values.end()) {
    }

    constexpr Deque(Deque const& other) noexcept
        requires ::std::is_nothrow_copy_constructible_v<value_type>
    {
        for (const_reference value : other) {
            this->emplace_back(value);
        }
    }

    constexpr Deque(Deque&& other) noexcept
        : blocks{::std::exchange(other.blocks, nullptr)},
          map_capacity{::std::exchange(other.map_capacity, 0)},
          start_offset{::std::exchange(other.start_offset, 0)},
          element_count{::std::exchange(other.element_count, 0)} {
    }

    constexpr ~Deque() noexcept {
        this->destroy_storage();
    }

    /// @pre Internal callers must not assign the Deque to itself.
    constexpr auto operator=(this Deque& self, Deque const& other) noexcept -> Deque&
        requires ::std::is_nothrow_copy_constructible_v<value_type>;

    /// @pre Internal callers must not assign the Deque to itself.
    constexpr auto operator=(this Deque& self, Deque&& other) noexcept -> Deque&;

    constexpr auto operator=(this Deque& self, ::std::initializer_list<value_type> values) noexcept -> Deque&
        requires ::std::is_nothrow_copy_constructible_v<value_type>
    {
        Deque copy{values};
        self.swap(copy);
        return self;
    }

    [[nodiscard]]
    constexpr auto begin(this Deque& self) noexcept -> iterator {
        return iterator{self.blocks == nullptr ? nullptr : self.blocks + self.start_offset / elements_per_block,
                        self.start_offset % elements_per_block};
    }

    [[nodiscard]]
    constexpr auto begin(this Deque const& self) noexcept -> const_iterator {
        return const_iterator{self.blocks == nullptr ? nullptr : self.blocks + self.start_offset / elements_per_block,
                              self.start_offset % elements_per_block};
    }

    [[nodiscard]]
    constexpr auto cbegin(this Deque const& self) noexcept -> const_iterator {
        return self.begin();
    }

    [[nodiscard]]
    constexpr auto end(this Deque& self) noexcept -> iterator {
        return self.begin() + static_cast<difference_type>(self.element_count);
    }

    [[nodiscard]]
    constexpr auto end(this Deque const& self) noexcept -> const_iterator {
        return self.begin() + static_cast<difference_type>(self.element_count);
    }

    [[nodiscard]]
    constexpr auto cend(this Deque const& self) noexcept -> const_iterator {
        return self.end();
    }

    [[nodiscard]]
    constexpr auto rbegin(this Deque& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto rbegin(this Deque const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto crbegin(this Deque const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto rend(this Deque& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto rend(this Deque const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto crend(this Deque const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto is_empty(this Deque const& self) noexcept -> bool {
        return self.element_count == 0;
    }

    /**
     * @brief Tests whether the deque has no elements, for downstream users only.
     */
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto empty(this Deque const&) noexcept -> bool = delete
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
    constexpr auto empty(this Deque const& self) noexcept -> bool {
        return self.is_empty();
    }
#endif

    [[nodiscard]]
    constexpr auto size(this Deque const& self) noexcept -> size_type {
        return self.element_count;
    }

    [[nodiscard]]
    static constexpr auto max_size() noexcept -> size_type {
        constexpr size_type allocation_limit{::std::numeric_limits<size_type>::max() / sizeof(value_type)};
        constexpr size_type iterator_limit{static_cast<size_type>(::std::numeric_limits<difference_type>::max())};
        return allocation_limit < iterator_limit ? allocation_limit : iterator_limit;
    }

    [[nodiscard]]
    constexpr auto get_allocator(this Deque const&) noexcept -> allocator_type {
        return {};
    }

    /**
     * @brief Indexed element access, bounds-checked according to the
     * build-wide contract setting.
     * @param position Zero-based element position.
     * @return Mutable reference to the requested element.
     * @pre position is a valid element position.
     */
    [[nodiscard]]
    constexpr auto operator[](this Deque& self, size_type position) noexcept -> reference;

    /**
     * @brief Indexed element access, read-only, bounds-checked according to the
     * build-wide contract setting.
     * @param position Zero-based element position.
     * @return Read-only reference to the requested element.
     * @pre position is a valid element position.
     */
    [[nodiscard]]
    constexpr auto operator[](this Deque const& self, size_type position) noexcept -> const_reference;

    [[nodiscard]]
    constexpr auto front(this Deque& self) noexcept -> reference;

    [[nodiscard]]
    constexpr auto front(this Deque const& self) noexcept -> const_reference;

    [[nodiscard]]
    constexpr auto back(this Deque& self) noexcept -> reference;

    [[nodiscard]]
    constexpr auto back(this Deque const& self) noexcept -> const_reference;

    template<typename... Args>
    constexpr auto emplace_back(this Deque& self, Args&&... args) noexcept -> reference
        requires ::std::is_nothrow_constructible_v<value_type, Args...>;

    template<typename... Args>
    constexpr auto emplace_front(this Deque& self, Args&&... args) noexcept -> reference
        requires ::std::is_nothrow_constructible_v<value_type, Args...>;

    constexpr void push_back(this Deque& self, const_reference value) noexcept
        requires ::std::is_nothrow_copy_constructible_v<value_type>
    {
        self.emplace_back(value);
    }

    constexpr void push_back(this Deque& self, value_type&& value) noexcept
        requires ::std::is_nothrow_move_constructible_v<value_type>
    {
        self.emplace_back(::std::move(value));
    }

    constexpr void push_front(this Deque& self, const_reference value) noexcept
        requires ::std::is_nothrow_copy_constructible_v<value_type>
    {
        self.emplace_front(value);
    }

    constexpr void push_front(this Deque& self, value_type&& value) noexcept
        requires ::std::is_nothrow_move_constructible_v<value_type>
    {
        self.emplace_front(::std::move(value));
    }

    constexpr void pop_back(this Deque& self) noexcept;

    constexpr void pop_front(this Deque& self) noexcept;

    constexpr void clear(this Deque& self) noexcept {
        size_type const old_size{self.element_count};
        for (size_type index{}; index != old_size; ++index) {
            ::std::destroy_at(self.pointer_at(index));
        }
        size_type const used_blocks{self.allocated_block_count()};
        size_type const first_block{self.start_offset / elements_per_block};
        size_type const front_spare{self.has_front_spare() ? 1u : 0u};
        size_type const back_spare{self.has_back_spare() ? 1u : 0u};
        size_type const first_occupied{first_block - front_spare};
        size_type const occupied_blocks{used_blocks + front_spare + back_spare};
        pointer const first_retained{occupied_blocks > 1 ? self.blocks[first_occupied] : nullptr};
        pointer const last_retained{occupied_blocks != 0 ? self.blocks[first_occupied + occupied_blocks - 1] : nullptr};
        for (size_type index{}; index != occupied_blocks; ++index) {
            size_type const map_index{first_occupied + index};
            if (index != 0 && index + 1 != occupied_blocks) {
                element_allocator::deallocate_n(self.blocks[map_index], elements_per_block);
            }
            self.blocks[map_index] = nullptr;
        }
        self.element_count = 0;
        size_type const center{self.map_capacity / 2};
        if (first_retained != nullptr) {
            self.blocks[center - 1] = first_retained;
        }
        if (last_retained != nullptr) {
            self.blocks[center] = last_retained;
        }
        self.start_offset = center * elements_per_block;
    }

    constexpr void shrink_to_fit(this Deque& self) noexcept {
        size_type const used_blocks{self.allocated_block_count()};
        size_type const first_block{self.start_offset / elements_per_block};
        if (self.has_front_spare()) {
            element_allocator::deallocate_n(self.blocks[first_block - 1], elements_per_block);
            self.blocks[first_block - 1] = nullptr;
        }
        if (self.has_back_spare()) {
            element_allocator::deallocate_n(self.blocks[first_block + used_blocks], elements_per_block);
            self.blocks[first_block + used_blocks] = nullptr;
        }
        if (used_blocks == 0) {
            if (self.blocks != nullptr) {
                map_allocator::deallocate_n(self.blocks, self.map_capacity);
            }
            self.blocks = nullptr;
            self.map_capacity = 0;
            self.start_offset = 0;
            return;
        }

        size_type const requested_capacity{used_blocks + 2 < initial_map_capacity ? initial_map_capacity
                                                                                  : used_blocks + 2};
        if (requested_capacity < self.map_capacity) {
            self.relocate_map(requested_capacity, 1);
        }
    }

    constexpr void resize(this Deque& self, size_type count) noexcept
        requires ::std::is_nothrow_default_constructible_v<value_type>
    {
        while (self.element_count > count) {
            self.pop_back();
        }
        while (self.element_count < count) {
            self.emplace_back();
        }
    }

    template<typename... Args>
    constexpr auto emplace(this Deque& self, const_iterator position, Args&&... args) noexcept -> iterator
        requires (::std::is_nothrow_constructible_v<value_type, Args...> &&
                  ::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_move_assignable_v<value_type>)
    ;

    constexpr auto insert(this Deque& self, const_iterator position, const_reference value) noexcept -> iterator
        requires (::std::is_nothrow_copy_constructible_v<value_type> &&
                  ::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_move_assignable_v<value_type>)
    {
        return self.emplace(position, value);
    }

    constexpr auto insert(this Deque& self, const_iterator position, value_type&& value) noexcept -> iterator
        requires (::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_move_assignable_v<value_type>)
    {
        return self.emplace(position, ::std::move(value));
    }

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
    constexpr auto insert(this Deque& self, const_iterator position, InputIterator first, Sentinel last) noexcept
        -> iterator
        requires (is_nothrow_input_range<InputIterator, Sentinel>() &&
                  ::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_move_assignable_v<value_type>)
    ;

    constexpr auto insert(this Deque& self, const_iterator position,
                          ::std::initializer_list<value_type> values) noexcept -> iterator
        requires (::std::is_nothrow_copy_constructible_v<value_type> &&
                  ::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_move_assignable_v<value_type>)
    {
        return self.insert(position, values.begin(), values.end());
    }

    constexpr auto erase(this Deque& self, const_iterator position) noexcept -> iterator
        requires ::std::is_nothrow_move_assignable_v<value_type>;

    constexpr auto erase(this Deque& self, const_iterator first, const_iterator last) noexcept -> iterator
        requires ::std::is_nothrow_move_assignable_v<value_type>;

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
        requires (is_nothrow_input_range<InputIterator, Sentinel>())
    constexpr void assign(this Deque& self, InputIterator first, Sentinel last) noexcept {
        Deque replacement{first, last};
        self.swap(replacement);
    }

    constexpr void assign(this Deque& self, ::std::initializer_list<value_type> values) noexcept
        requires ::std::is_nothrow_copy_constructible_v<value_type>
    {
        Deque replacement{values};
        self.swap(replacement);
    }

    constexpr void swap(this Deque& self, Deque& other) noexcept {
        auto tmp_blocks = self.blocks;
        auto tmp_map_capacity = self.map_capacity;
        auto tmp_start_offset = self.start_offset;
        auto tmp_element_count = self.element_count;
        self.blocks = other.blocks;
        self.map_capacity = other.map_capacity;
        self.start_offset = other.start_offset;
        self.element_count = other.element_count;
        other.blocks = tmp_blocks;
        other.map_capacity = tmp_map_capacity;
        other.start_offset = tmp_start_offset;
        other.element_count = tmp_element_count;
    }

    template<typename RightAllocator>
        requires requires(T const& left, T const& right) {
            { static_cast<bool>(left == right) } noexcept -> ::std::same_as<bool>;
        }
    [[nodiscard]]
    constexpr auto operator==(this Deque const& self, Deque<T, RightAllocator> const& right) noexcept -> bool {
        return self.size() == right.size() && ::std::equal(self.begin(), self.end(), right.begin());
    }

    template<typename RightAllocator>
        requires (::std::three_way_comparable<T> &&
                  requires(T const& left, T const& right) {
                      { ::std::compare_three_way{}(left, right) } noexcept;
                  })
    [[nodiscard]]
    constexpr auto operator<=>(this Deque const& self, Deque<T, RightAllocator> const& right) noexcept {
        return ::std::lexicographical_compare_three_way(self.begin(), self.end(), right.begin(), right.end(),
                                                        ::std::compare_three_way{});
    }
};

template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
Deque(InputIterator, Sentinel) -> Deque<::std::iter_value_t<InputIterator>>;

template<typename T>
Deque(::std::initializer_list<T>) -> Deque<T>;

template<typename T, typename Allocator>
constexpr void swap(Deque<T, Allocator>& left, Deque<T, Allocator>& right) noexcept {
    left.swap(right);
}

} // namespace pltxt2htm::container
