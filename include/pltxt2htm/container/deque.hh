/**
 * @file deque.hh
 * @brief Segmented double-ended queue for pltxt2htm.
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

#include "../contracts.hh"
#include "../details/push_macro.hh"

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
        for (size_type index{}; index != used_blocks; ++index) {
            new_blocks[new_first_block + index] = self.blocks[first_block + index];
        }

        if (self.blocks != nullptr) {
            map_allocator::deallocate_n(self.blocks, self.map_capacity);
        }
        self.blocks = new_blocks;
        self.map_capacity = new_capacity;
        self.start_offset = new_first_block * elements_per_block + self.start_offset % elements_per_block;
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void grow_map(this Deque& self, size_type required_front, size_type required_back) noexcept {
        size_type const used_blocks{self.allocated_block_count()};
        size_type const required_capacity{used_blocks + required_front + required_back};
        pltxt2htm_assert(required_capacity <= max_map_capacity, u8"Deque map capacity exceeds max_map_capacity");
        size_type new_capacity{self.map_capacity == 0 ? initial_map_capacity : self.map_capacity};
        // Leave enough slack to amortize map movement when the queue is nearly full.
        while (new_capacity < required_capacity || used_blocks > new_capacity / 2) {
            if (new_capacity > max_map_capacity / 2) {
                new_capacity = required_capacity;
                break;
            }
            new_capacity *= 2;
        }

        size_type new_first_block{(new_capacity - used_blocks) / 2};
        if (new_first_block < required_front) {
            new_first_block = required_front;
        }
        if (new_capacity - new_first_block - used_blocks < required_back) {
            new_first_block = new_capacity - used_blocks - required_back;
        }
        if (new_capacity != self.map_capacity) {
            self.relocate_map(new_capacity, new_first_block);
            return;
        }

        size_type const first_block{self.start_offset / elements_per_block};
        if (new_first_block < first_block) {
            for (size_type index{}; index != used_blocks; ++index) {
                self.blocks[new_first_block + index] = self.blocks[first_block + index];
            }
        }
        else {
            for (size_type index{used_blocks}; index != 0; --index) {
                self.blocks[new_first_block + index - 1] = self.blocks[first_block + index - 1];
            }
        }
        for (size_type index{}; index != self.map_capacity; ++index) {
            if (index < new_first_block || index >= new_first_block + used_blocks) {
                self.blocks[index] = nullptr;
            }
        }
        self.start_offset = new_first_block * elements_per_block + self.start_offset % elements_per_block;
    }

    constexpr void ensure_map(this Deque& self) noexcept {
        if (self.blocks == nullptr) {
            self.allocate_map(initial_map_capacity);
        }
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void ensure_can_grow(this Deque const& self) noexcept {
        pltxt2htm_assert(self.element_count < self.max_size(), u8"Deque size exceeds max_size");
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void ensure_front_slot(this Deque& self) noexcept {
        self.ensure_map();
        if (self.start_offset / elements_per_block == 0) {
            self.template grow_map<ndebug>(1, 0);
        }
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void ensure_back_slot(this Deque& self) noexcept {
        self.ensure_map();
        size_type const used_blocks{self.allocated_block_count()};
        if (self.start_offset / elements_per_block + used_blocks == self.map_capacity) {
            self.template grow_map<ndebug>(0, 1);
        }
    }

    constexpr void reset_empty_position(this Deque& self) noexcept {
        self.start_offset = self.map_capacity / 2 * elements_per_block;
    }

    constexpr void release_blocks(this Deque& self) noexcept {
        size_type const used_blocks{self.allocated_block_count()};
        size_type const first_block{self.start_offset / elements_per_block};
        for (size_type index{}; index != used_blocks; ++index) {
            size_type const map_index{first_block + index};
            element_allocator::deallocate_n(self.blocks[map_index], elements_per_block);
            self.blocks[map_index] = nullptr;
        }
    }

    constexpr void destroy_storage(this Deque& self) noexcept {
        self.clear();
        if (self.blocks != nullptr) {
            map_allocator::deallocate_n(self.blocks, self.map_capacity);
        }
        self.blocks = nullptr;
        self.map_capacity = 0;
        self.start_offset = 0;
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
#ifdef NDEBUG
        constexpr auto ndebug = ::pltxt2htm::Contracts::ignore;
#else
        constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;
#endif
        for (size_type index{}; index != count; ++index) {
            this->template emplace_back<ndebug>();
        }
    }

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
        requires (is_nothrow_input_range<InputIterator, Sentinel>())
    constexpr Deque(InputIterator first, Sentinel last) noexcept {
#ifdef NDEBUG
        constexpr auto ndebug = ::pltxt2htm::Contracts::ignore;
#else
        constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;
#endif
        for (; first != last; ++first) {
            this->template emplace_back<ndebug>(*first);
        }
    }

    constexpr Deque(::std::initializer_list<value_type> values) noexcept
        requires ::std::is_nothrow_copy_constructible_v<value_type>
        : Deque(values.begin(), values.end()) {
    }

    constexpr Deque(Deque const& other) noexcept
        requires ::std::is_nothrow_copy_constructible_v<value_type>
    {
#ifdef NDEBUG
        constexpr auto ndebug = ::pltxt2htm::Contracts::ignore;
#else
        constexpr auto ndebug = ::pltxt2htm::Contracts::quick_enforce;
#endif
        for (const_reference value : other) {
            this->template emplace_back<ndebug>(value);
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

    constexpr auto operator=(this Deque& self, Deque const& other) noexcept -> Deque&
        requires ::std::is_nothrow_copy_constructible_v<value_type>
    {
        if (::std::addressof(self) == ::std::addressof(other)) {
            return self;
        }
        Deque copy{other};
        self.swap(copy);
        return self;
    }

    constexpr auto operator=(this Deque& self, Deque&& other) noexcept -> Deque& {
        if (::std::addressof(self) == ::std::addressof(other)) {
            return self;
        }
        self.swap(other);
        return self;
    }

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
     * @brief Unchecked element access, for downstream users only.
     * @pre index < size().
     */
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator[](this Deque&, size_type) noexcept -> reference = delete
    #if __cpp_deleted_function >= 202403L
        #if defined __clang__
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wc++26-extensions"
        #endif
        ("operator[] is external-only; use index<ndebug>() inside pltxt2htm")
        #if defined __clang__
            #pragma clang diagnostic pop
        #endif
    #endif
        ;
#else
    [[nodiscard]]
    constexpr auto operator[](this Deque& self, size_type index) noexcept -> reference {
        return *self.pointer_at(index);
    }
#endif

    /**
     * @brief Unchecked read-only element access, for downstream users only.
     * @pre index < size().
     */
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto operator[](this Deque const&, size_type) noexcept -> const_reference = delete
    #if __cpp_deleted_function >= 202403L
        #if defined __clang__
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wc++26-extensions"
        #endif
        ("operator[] is external-only; use index<ndebug>() inside pltxt2htm")
        #if defined __clang__
            #pragma clang diagnostic pop
        #endif
    #endif
        ;
#else
    [[nodiscard]]
    constexpr auto operator[](this Deque const& self, size_type index) noexcept -> const_reference {
        return *self.pointer_at(index);
    }
#endif

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto index(this Deque& self, size_type position) noexcept -> reference {
        pltxt2htm_assert(position < self.element_count, u8"Index of Deque out of bound");
        return *self.pointer_at(position);
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto index(this Deque const& self, size_type position) noexcept -> const_reference {
        pltxt2htm_assert(position < self.element_count, u8"Index of Deque out of bound");
        return *self.pointer_at(position);
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto front(this Deque& self) noexcept -> reference {
        pltxt2htm_assert(!self.is_empty(), u8"Accessing front of empty Deque");
        return *self.pointer_at(0);
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto front(this Deque const& self) noexcept -> const_reference {
        pltxt2htm_assert(!self.is_empty(), u8"Accessing front of empty Deque");
        return *self.pointer_at(0);
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto back(this Deque& self) noexcept -> reference {
        pltxt2htm_assert(!self.is_empty(), u8"Accessing back of empty Deque");
        return *self.pointer_at(self.element_count - 1);
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto back(this Deque const& self) noexcept -> const_reference {
        pltxt2htm_assert(!self.is_empty(), u8"Accessing back of empty Deque");
        return *self.pointer_at(self.element_count - 1);
    }

    template<::pltxt2htm::Contracts ndebug, typename... Arguments>
        requires ::std::is_nothrow_constructible_v<value_type, Arguments...>
    constexpr auto emplace_back(this Deque& self, Arguments&&... arguments) noexcept -> reference {
        self.template ensure_can_grow<ndebug>();
        size_type const insertion_offset{self.start_offset + self.element_count};
        size_type const block_offset{insertion_offset % elements_per_block};
        bool const needs_block{self.element_count == 0 || block_offset == 0};

        if (needs_block) {
            self.template ensure_back_slot<ndebug>();
            size_type const map_index{self.start_offset / elements_per_block + self.allocated_block_count()};
            pointer const new_block{element_allocator::allocate(elements_per_block)};
            pointer const result{::std::construct_at(new_block, ::std::forward<Arguments>(arguments)...)};
            self.blocks[map_index] = new_block;
            ++self.element_count;
            return *result;
        }

        pointer const result{
            ::std::construct_at(self.pointer_at(self.element_count), ::std::forward<Arguments>(arguments)...)};
        ++self.element_count;
        return *result;
    }

    template<::pltxt2htm::Contracts ndebug, typename... Arguments>
        requires ::std::is_nothrow_constructible_v<value_type, Arguments...>
    constexpr auto emplace_front(this Deque& self, Arguments&&... arguments) noexcept -> reference {
        self.template ensure_can_grow<ndebug>();
        if (self.is_empty()) {
            return self.template emplace_back<ndebug>(::std::forward<Arguments>(arguments)...);
        }

        if (self.start_offset % elements_per_block != 0) {
            pointer const result{::std::construct_at(
                self.blocks[self.start_offset / elements_per_block] + self.start_offset % elements_per_block - 1,
                ::std::forward<Arguments>(arguments)...)};
            --self.start_offset;
            ++self.element_count;
            return *result;
        }

        self.template ensure_front_slot<ndebug>();
        pointer const new_block{element_allocator::allocate(elements_per_block)};
        pointer const result{
            ::std::construct_at(new_block + elements_per_block - 1, ::std::forward<Arguments>(arguments)...)};
        --self.start_offset;
        self.blocks[self.start_offset / elements_per_block] = new_block;
        ++self.element_count;
        return *result;
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void push_back(this Deque& self, const_reference value) noexcept
        requires ::std::is_nothrow_copy_constructible_v<value_type>
    {
        self.template emplace_back<ndebug>(value);
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void push_back(this Deque& self, value_type&& value) noexcept
        requires ::std::is_nothrow_move_constructible_v<value_type>
    {
        self.template emplace_back<ndebug>(::std::move(value));
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void push_front(this Deque& self, const_reference value) noexcept
        requires ::std::is_nothrow_copy_constructible_v<value_type>
    {
        self.template emplace_front<ndebug>(value);
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void push_front(this Deque& self, value_type&& value) noexcept
        requires ::std::is_nothrow_move_constructible_v<value_type>
    {
        self.template emplace_front<ndebug>(::std::move(value));
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void pop_back(this Deque& self) noexcept {
        pltxt2htm_assert(!self.is_empty(), u8"Popping back of empty Deque");
        size_type const erased_index{self.element_count - 1};
        size_type const erased_offset{self.start_offset + erased_index};
        size_type const erased_block{erased_offset / elements_per_block};
        ::std::destroy_at(self.pointer_at(erased_index));
        --self.element_count;

        if (self.element_count == 0) {
            element_allocator::deallocate_n(self.blocks[erased_block], elements_per_block);
            self.blocks[erased_block] = nullptr;
            self.reset_empty_position();
        }
        else if ((self.start_offset + self.element_count) % elements_per_block == 0) {
            element_allocator::deallocate_n(self.blocks[erased_block], elements_per_block);
            self.blocks[erased_block] = nullptr;
        }
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr void pop_front(this Deque& self) noexcept {
        pltxt2htm_assert(!self.is_empty(), u8"Popping front of empty Deque");
        size_type const erased_block{self.start_offset / elements_per_block};
        ::std::destroy_at(self.pointer_at(0));
        --self.element_count;

        if (self.element_count == 0) {
            element_allocator::deallocate_n(self.blocks[erased_block], elements_per_block);
            self.blocks[erased_block] = nullptr;
            self.reset_empty_position();
            return;
        }

        ++self.start_offset;
        if (self.start_offset % elements_per_block == 0) {
            element_allocator::deallocate_n(self.blocks[erased_block], elements_per_block);
            self.blocks[erased_block] = nullptr;
        }
    }

    constexpr void clear(this Deque& self) noexcept {
        size_type const old_size{self.element_count};
        for (size_type index{}; index != old_size; ++index) {
            ::std::destroy_at(self.pointer_at(index));
        }
        self.release_blocks();
        self.element_count = 0;
        self.reset_empty_position();
    }

    constexpr void shrink_to_fit(this Deque& self) noexcept {
        size_type const used_blocks{self.allocated_block_count()};
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

    template<::pltxt2htm::Contracts ndebug>
    constexpr void resize(this Deque& self, size_type count) noexcept
        requires ::std::is_nothrow_default_constructible_v<value_type>
    {
        while (self.element_count > count) {
            self.template pop_back<ndebug>();
        }
        while (self.element_count < count) {
            self.template emplace_back<ndebug>();
        }
    }

    template<::pltxt2htm::Contracts ndebug, typename... Arguments>
        requires (::std::is_nothrow_constructible_v<value_type, Arguments...> &&
                  ::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_move_assignable_v<value_type>)
    constexpr auto emplace(this Deque& self, const_iterator position, Arguments&&... arguments) noexcept -> iterator {
        size_type const index{static_cast<size_type>(position - self.cbegin())};
        pltxt2htm_assert(index <= self.element_count, u8"Insertion position of Deque out of bound");
        if (index == 0) {
            self.template emplace_front<ndebug>(::std::forward<Arguments>(arguments)...);
            return self.begin();
        }
        if (index == self.element_count) {
            self.template emplace_back<ndebug>(::std::forward<Arguments>(arguments)...);
            return self.begin() + static_cast<difference_type>(index);
        }

        value_type value{::std::forward<Arguments>(arguments)...};
        size_type const old_size{self.element_count};
        if (index < old_size / 2) {
            self.template emplace_front<ndebug>(::std::move(self.template front<ndebug>()));
            for (size_type current{1}; current != index; ++current) {
                self.template index<ndebug>(current) = ::std::move(self.template index<ndebug>(current + 1));
            }
        }
        else {
            self.template emplace_back<ndebug>(::std::move(self.template back<ndebug>()));
            for (size_type current{old_size - 1}; current != index; --current) {
                self.template index<ndebug>(current) = ::std::move(self.template index<ndebug>(current - 1));
            }
        }
        self.template index<ndebug>(index) = ::std::move(value);
        return self.begin() + static_cast<difference_type>(index);
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr auto insert(this Deque& self, const_iterator position, const_reference value) noexcept -> iterator
        requires (::std::is_nothrow_copy_constructible_v<value_type> &&
                  ::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_move_assignable_v<value_type>)
    {
        return self.template emplace<ndebug>(position, value);
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr auto insert(this Deque& self, const_iterator position, value_type&& value) noexcept -> iterator
        requires (::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_move_assignable_v<value_type>)
    {
        return self.template emplace<ndebug>(position, ::std::move(value));
    }

    template<::pltxt2htm::Contracts ndebug, ::std::input_iterator InputIterator,
             ::std::sentinel_for<InputIterator> Sentinel>
        requires (is_nothrow_input_range<InputIterator, Sentinel>() &&
                  ::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_move_assignable_v<value_type>)
    constexpr auto insert(this Deque& self, const_iterator position, InputIterator first, Sentinel last) noexcept
        -> iterator {
        size_type const index{static_cast<size_type>(position - self.cbegin())};
        pltxt2htm_assert(index <= self.element_count, u8"Insertion position of Deque out of bound");
        Deque values{first, last};
        size_type inserted{};
        for (reference value : values) {
            self.template emplace<ndebug>(self.cbegin() + static_cast<difference_type>(index + inserted),
                                          ::std::move(value));
            ++inserted;
        }
        return self.begin() + static_cast<difference_type>(index);
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr auto insert(this Deque& self, const_iterator position,
                          ::std::initializer_list<value_type> values) noexcept -> iterator
        requires (::std::is_nothrow_copy_constructible_v<value_type> &&
                  ::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_move_assignable_v<value_type>)
    {
        return self.template insert<ndebug>(position, values.begin(), values.end());
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr auto erase(this Deque& self, const_iterator position) noexcept -> iterator
        requires ::std::is_nothrow_move_assignable_v<value_type>
    {
        pltxt2htm_assert(position != self.cend(), u8"Erasing past the end of Deque");
        return self.template erase<ndebug>(position, position + 1);
    }

    template<::pltxt2htm::Contracts ndebug>
    constexpr auto erase(this Deque& self, const_iterator first, const_iterator last) noexcept -> iterator
        requires ::std::is_nothrow_move_assignable_v<value_type>
    {
        size_type const first_index{static_cast<size_type>(first - self.cbegin())};
        size_type const last_index{static_cast<size_type>(last - self.cbegin())};
        pltxt2htm_assert(first_index <= last_index && last_index <= self.element_count,
                         u8"Erasure range of Deque out of bound");
        size_type const erased_count{last_index - first_index};
        if (erased_count == 0) {
            return self.begin() + static_cast<difference_type>(first_index);
        }

        size_type const suffix_size{self.element_count - last_index};
        if (first_index < suffix_size) {
            for (size_type current{first_index}; current != 0; --current) {
                self.template index<ndebug>(current + erased_count - 1) =
                    ::std::move(self.template index<ndebug>(current - 1));
            }
            for (size_type count{}; count != erased_count; ++count) {
                self.template pop_front<ndebug>();
            }
        }
        else {
            for (size_type current{first_index}; current + erased_count != self.element_count; ++current) {
                self.template index<ndebug>(current) = ::std::move(self.template index<ndebug>(current + erased_count));
            }
            for (size_type count{}; count != erased_count; ++count) {
                self.template pop_back<ndebug>();
            }
        }
        return self.begin() + static_cast<difference_type>(first_index);
    }

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
        ::std::swap(self.blocks, other.blocks);
        ::std::swap(self.map_capacity, other.map_capacity);
        ::std::swap(self.start_offset, other.start_offset);
        ::std::swap(self.element_count, other.element_count);
    }
};

template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
Deque(InputIterator, Sentinel) -> Deque<::std::iter_value_t<InputIterator>>;

template<typename T>
Deque(::std::initializer_list<T>) -> Deque<T>;

template<typename T, typename LeftAllocator, typename RightAllocator>
    requires requires(T const& left, T const& right) {
        { static_cast<bool>(left == right) } noexcept -> ::std::same_as<bool>;
    }
[[nodiscard]]
constexpr auto operator==(Deque<T, LeftAllocator> const& left, Deque<T, RightAllocator> const& right) noexcept -> bool {
    return left.size() == right.size() && ::std::equal(left.begin(), left.end(), right.begin());
}

template<typename T, typename LeftAllocator, typename RightAllocator>
    requires (::std::three_way_comparable<T> &&
              requires(T const& left, T const& right) {
                  { ::std::compare_three_way{}(left, right) } noexcept;
              })
[[nodiscard]]
constexpr auto operator<=>(Deque<T, LeftAllocator> const& left, Deque<T, RightAllocator> const& right) noexcept {
    return ::std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end(),
                                                    ::std::compare_three_way{});
}

template<typename T, typename Allocator>
constexpr void swap(Deque<T, Allocator>& left, Deque<T, Allocator>& right) noexcept {
    left.swap(right);
}

} // namespace pltxt2htm::container

#include "../details/pop_macro.hh"
