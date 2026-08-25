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
 * across push_front and push_back.
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

    static consteval auto calculate_elements_per_block() noexcept -> size_type {
        constexpr size_type target_block_bytes{512};
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

    pointer* blocks{};
    size_type map_capacity{};
    size_type first_block{};
    size_type first_offset{};
    size_type element_count{};

    class ConstructionGuard {
        Deque* owner{};

    public:
        constexpr explicit ConstructionGuard(Deque& owner_) noexcept
            : owner{::std::addressof(owner_)} {
        }

        ConstructionGuard(ConstructionGuard const&) = delete;
        auto operator=(ConstructionGuard const&) -> ConstructionGuard& = delete;

        constexpr ~ConstructionGuard() noexcept {
            if (this->owner != nullptr) {
                this->owner->destroy_storage();
            }
        }

        constexpr void release(this ConstructionGuard& self) noexcept {
            self.owner = nullptr;
        }
    };

    class BlockGuard {
        pointer block{};

    public:
        constexpr explicit BlockGuard(pointer block_) noexcept
            : block{block_} {
        }

        BlockGuard(BlockGuard const&) = delete;
        auto operator=(BlockGuard const&) -> BlockGuard& = delete;

        constexpr ~BlockGuard() noexcept {
            if (this->block != nullptr) {
                element_allocator::deallocate_n(this->block, elements_per_block);
            }
        }

        constexpr void release(this BlockGuard& self) noexcept {
            self.block = nullptr;
        }
    };

    [[nodiscard]]
    constexpr auto allocated_block_count(this Deque const& self) noexcept -> size_type {
        if (self.element_count == 0) {
            return 0;
        }
        return (self.first_offset + self.element_count - 1) / elements_per_block + 1;
    }

    [[nodiscard]]
    constexpr auto pointer_at(this Deque& self, size_type index) noexcept -> pointer {
        size_type const offset{self.first_offset + index};
        return self.blocks[self.first_block + offset / elements_per_block] + offset % elements_per_block;
    }

    [[nodiscard]]
    constexpr auto pointer_at(this Deque const& self, size_type index) noexcept -> const_pointer {
        size_type const offset{self.first_offset + index};
        return self.blocks[self.first_block + offset / elements_per_block] + offset % elements_per_block;
    }

    constexpr void allocate_map(this Deque& self, size_type capacity) noexcept {
        self.blocks = map_allocator::allocate(capacity);
        self.map_capacity = capacity;
        self.first_block = capacity / 2;
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
        for (size_type index{}; index != used_blocks; ++index) {
            new_blocks[new_first_block + index] = self.blocks[self.first_block + index];
        }

        if (self.blocks != nullptr) {
            map_allocator::deallocate_n(self.blocks, self.map_capacity);
        }
        self.blocks = new_blocks;
        self.map_capacity = new_capacity;
        self.first_block = new_first_block;
    }

    constexpr void grow_map(this Deque& self, size_type required_front, size_type required_back) noexcept {
        size_type const used_blocks{self.allocated_block_count()};
        size_type const required_capacity{used_blocks + required_front + required_back};
        size_type new_capacity{self.map_capacity == 0 ? initial_map_capacity : self.map_capacity};
        while (new_capacity < required_capacity || new_capacity == self.map_capacity) {
            if (new_capacity > ::std::numeric_limits<size_type>::max() / 2) [[unlikely]] {
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
        self.relocate_map(new_capacity, new_first_block);
    }

    constexpr void ensure_map(this Deque& self) noexcept {
        if (self.blocks == nullptr) {
            self.allocate_map(initial_map_capacity);
        }
    }

    constexpr void ensure_can_grow(this Deque const& self) noexcept {
        if (self.element_count == self.max_size()) [[unlikely]] {
            ::fast_io::fast_terminate();
        }
    }

    constexpr void ensure_front_slot(this Deque& self) noexcept {
        self.ensure_map();
        if (self.first_block == 0) [[unlikely]] {
            self.grow_map(1, 0);
        }
    }

    constexpr void ensure_back_slot(this Deque& self) noexcept {
        self.ensure_map();
        size_type const used_blocks{self.allocated_block_count()};
        if (self.first_block + used_blocks == self.map_capacity) [[unlikely]] {
            self.grow_map(0, 1);
        }
    }

    constexpr void reset_empty_position(this Deque& self) noexcept {
        self.first_offset = 0;
        self.first_block = self.map_capacity / 2;
    }

    constexpr void release_blocks(this Deque& self) noexcept {
        size_type const used_blocks{self.allocated_block_count()};
        for (size_type index{}; index != used_blocks; ++index) {
            size_type const map_index{self.first_block + index};
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
        self.first_block = 0;
    }

    constexpr void take_storage(this Deque& self, Deque& other) noexcept {
        self.blocks = other.blocks;
        self.map_capacity = other.map_capacity;
        self.first_block = other.first_block;
        self.first_offset = other.first_offset;
        self.element_count = other.element_count;

        other.blocks = nullptr;
        other.map_capacity = 0;
        other.first_block = 0;
        other.first_offset = 0;
        other.element_count = 0;
    }

public:
    template<bool is_const>
    class BasicIterator {
        using owner_type = ::std::conditional_t<is_const, Deque const, Deque>;

        owner_type* owner{};
        size_type position{};

        constexpr BasicIterator(owner_type* owner_, size_type position_) noexcept
            : owner{owner_},
              position{position_} {
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
            : owner{other.owner},
              position{other.position} {
        }

        [[nodiscard]]
        constexpr auto operator*(this BasicIterator const& self) noexcept -> reference {
            return *self.owner->pointer_at(self.position);
        }

        [[nodiscard]]
        constexpr auto operator->(this BasicIterator const& self) noexcept -> pointer {
            return ::std::addressof(*self);
        }

        [[nodiscard]]
        constexpr auto operator[](this BasicIterator const& self, difference_type offset) noexcept -> reference {
            return *self.owner->pointer_at(
                static_cast<size_type>(static_cast<difference_type>(self.position) + offset));
        }

        constexpr auto operator++(this BasicIterator& self) noexcept -> BasicIterator& {
            ++self.position;
            return self;
        }

        constexpr auto operator++(this BasicIterator& self, int) noexcept -> BasicIterator {
            BasicIterator old{self};
            ++self;
            return old;
        }

        constexpr auto operator--(this BasicIterator& self) noexcept -> BasicIterator& {
            --self.position;
            return self;
        }

        constexpr auto operator--(this BasicIterator& self, int) noexcept -> BasicIterator {
            BasicIterator old{self};
            --self;
            return old;
        }

        constexpr auto operator+=(this BasicIterator& self, difference_type offset) noexcept -> BasicIterator& {
            self.position = static_cast<size_type>(static_cast<difference_type>(self.position) + offset);
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
            return static_cast<difference_type>(self.position) - static_cast<difference_type>(other.position);
        }

        template<bool other_const>
        [[nodiscard]]
        constexpr auto operator==(this BasicIterator const& self, BasicIterator<other_const> other) noexcept -> bool {
            return self.owner == other.owner && self.position == other.position;
        }

        template<bool other_const>
        [[nodiscard]]
        constexpr auto operator<=>(this BasicIterator const& self, BasicIterator<other_const> other) noexcept
            -> ::std::strong_ordering {
            return self.position <=> other.position;
        }
    };

    using iterator = BasicIterator<false>;
    using const_iterator = BasicIterator<true>;
    using reverse_iterator = ::std::reverse_iterator<iterator>;
    using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;

    constexpr Deque() noexcept = default;

    constexpr explicit Deque(size_type count)
        requires ::std::default_initializable<value_type>
    {
        ConstructionGuard guard{*this};
        for (size_type index{}; index != count; ++index) {
            this->emplace_back();
        }
        guard.release();
    }

    constexpr Deque(size_type count, const_reference value)
        requires ::std::copy_constructible<value_type>
    {
        ConstructionGuard guard{*this};
        for (size_type index{}; index != count; ++index) {
            this->emplace_back(value);
        }
        guard.release();
    }

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
    constexpr Deque(InputIterator first, Sentinel last) {
        ConstructionGuard guard{*this};
        for (; first != last; ++first) {
            this->emplace_back(*first);
        }
        guard.release();
    }

    constexpr Deque(::std::initializer_list<value_type> values)
        requires ::std::copy_constructible<value_type>
        : Deque(values.begin(), values.end()) {
    }

    constexpr Deque(Deque const& other)
        requires ::std::copy_constructible<value_type>
    {
        ConstructionGuard guard{*this};
        for (const_reference value : other) {
            this->emplace_back(value);
        }
        guard.release();
    }

    constexpr Deque(Deque&& other) noexcept {
        this->take_storage(other);
    }

    constexpr ~Deque() noexcept {
        this->destroy_storage();
    }

    constexpr auto operator=(this Deque& self, Deque const& other) -> Deque&
        requires (::std::copy_constructible<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
        if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
            return self;
        }
        Deque copy{other};
        self.swap(copy);
        return self;
    }

    constexpr auto operator=(this Deque& self, Deque&& other) noexcept -> Deque& {
        if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
            return self;
        }
        self.destroy_storage();
        self.take_storage(other);
        return self;
    }

    constexpr auto operator=(this Deque& self, ::std::initializer_list<value_type> values) -> Deque&
        requires (::std::copy_constructible<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
        Deque copy{values};
        self.swap(copy);
        return self;
    }

    [[nodiscard]]
    constexpr auto begin(this Deque& self) noexcept -> iterator {
        return iterator{::std::addressof(self), 0};
    }

    [[nodiscard]]
    constexpr auto begin(this Deque const& self) noexcept -> const_iterator {
        return const_iterator{::std::addressof(self), 0};
    }

    [[nodiscard]]
    constexpr auto cbegin(this Deque const& self) noexcept -> const_iterator {
        return self.begin();
    }

    [[nodiscard]]
    constexpr auto end(this Deque& self) noexcept -> iterator {
        return iterator{::std::addressof(self), self.element_count};
    }

    [[nodiscard]]
    constexpr auto end(this Deque const& self) noexcept -> const_iterator {
        return const_iterator{::std::addressof(self), self.element_count};
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
    constexpr auto empty(this Deque const& self) noexcept -> bool {
        return self.element_count == 0;
    }

    [[nodiscard]]
    constexpr auto is_empty(this Deque const& self) noexcept -> bool {
        return self.empty();
    }

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

    [[nodiscard]]
    constexpr auto operator[](this Deque& self, size_type index) noexcept -> reference {
        return *self.pointer_at(index);
    }

    [[nodiscard]]
    constexpr auto operator[](this Deque const& self, size_type index) noexcept -> const_reference {
        return *self.pointer_at(index);
    }

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

    [[nodiscard]]
    constexpr auto front_unchecked(this Deque& self) noexcept -> reference {
        return *self.pointer_at(0);
    }

    [[nodiscard]]
    constexpr auto front_unchecked(this Deque const& self) noexcept -> const_reference {
        return *self.pointer_at(0);
    }

    [[nodiscard]]
    constexpr auto front(this Deque& self) noexcept -> reference {
        return self.front_unchecked();
    }

    [[nodiscard]]
    constexpr auto front(this Deque const& self) noexcept -> const_reference {
        return self.front_unchecked();
    }

    [[nodiscard]]
    constexpr auto back_unchecked(this Deque& self) noexcept -> reference {
        return *self.pointer_at(self.element_count - 1);
    }

    [[nodiscard]]
    constexpr auto back_unchecked(this Deque const& self) noexcept -> const_reference {
        return *self.pointer_at(self.element_count - 1);
    }

    [[nodiscard]]
    constexpr auto back(this Deque& self) noexcept -> reference {
        return self.back_unchecked();
    }

    [[nodiscard]]
    constexpr auto back(this Deque const& self) noexcept -> const_reference {
        return self.back_unchecked();
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto front(this Deque& self) noexcept -> reference {
        pltxt2htm_assert(!self.empty(), u8"Accessing front of empty Deque");
        return self.front_unchecked();
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto front(this Deque const& self) noexcept -> const_reference {
        pltxt2htm_assert(!self.empty(), u8"Accessing front of empty Deque");
        return self.front_unchecked();
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto back(this Deque& self) noexcept -> reference {
        pltxt2htm_assert(!self.empty(), u8"Accessing back of empty Deque");
        return self.back_unchecked();
    }

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto back(this Deque const& self) noexcept -> const_reference {
        pltxt2htm_assert(!self.empty(), u8"Accessing back of empty Deque");
        return self.back_unchecked();
    }

    template<typename... Arguments>
        requires ::std::constructible_from<value_type, Arguments...>
    constexpr auto emplace_back(this Deque& self, Arguments&&... arguments) noexcept(
        ::std::is_nothrow_constructible_v<value_type, Arguments...>) -> reference {
        self.ensure_can_grow();
        size_type const insertion_offset{self.first_offset + self.element_count};
        size_type const block_offset{insertion_offset % elements_per_block};
        bool const needs_block{self.element_count == 0 || block_offset == 0};

        if (needs_block) {
            self.ensure_back_slot();
            size_type const map_index{self.first_block + self.allocated_block_count()};
            pointer const new_block{element_allocator::allocate(elements_per_block)};
            BlockGuard guard{new_block};
            pointer const result{::std::construct_at(new_block, ::std::forward<Arguments>(arguments)...)};
            self.blocks[map_index] = new_block;
            ++self.element_count;
            guard.release();
            return *result;
        }

        pointer const result{
            ::std::construct_at(self.pointer_at(self.element_count), ::std::forward<Arguments>(arguments)...)};
        ++self.element_count;
        return *result;
    }

    template<typename... Arguments>
        requires ::std::constructible_from<value_type, Arguments...>
    constexpr auto emplace_front(this Deque& self, Arguments&&... arguments) noexcept(
        ::std::is_nothrow_constructible_v<value_type, Arguments...>) -> reference {
        self.ensure_can_grow();
        if (self.empty()) {
            return self.emplace_back(::std::forward<Arguments>(arguments)...);
        }

        if (self.first_offset != 0) {
            pointer const result{::std::construct_at(self.blocks[self.first_block] + self.first_offset - 1,
                                                     ::std::forward<Arguments>(arguments)...)};
            --self.first_offset;
            ++self.element_count;
            return *result;
        }

        self.ensure_front_slot();
        pointer const new_block{element_allocator::allocate(elements_per_block)};
        BlockGuard guard{new_block};
        pointer const result{
            ::std::construct_at(new_block + elements_per_block - 1, ::std::forward<Arguments>(arguments)...)};
        --self.first_block;
        self.blocks[self.first_block] = new_block;
        self.first_offset = elements_per_block - 1;
        ++self.element_count;
        guard.release();
        return *result;
    }

    constexpr void push_back(this Deque& self, const_reference value)
        requires ::std::copy_constructible<value_type>
    {
        self.emplace_back(value);
    }

    constexpr void push_back(this Deque& self,
                             value_type&& value) noexcept(::std::is_nothrow_move_constructible_v<value_type>) {
        self.emplace_back(::std::move(value));
    }

    constexpr void push_front(this Deque& self, const_reference value)
        requires ::std::copy_constructible<value_type>
    {
        self.emplace_front(value);
    }

    constexpr void push_front(this Deque& self,
                              value_type&& value) noexcept(::std::is_nothrow_move_constructible_v<value_type>) {
        self.emplace_front(::std::move(value));
    }

    constexpr void pop_back_unchecked(this Deque& self) noexcept {
        size_type const erased_index{self.element_count - 1};
        size_type const erased_offset{self.first_offset + erased_index};
        size_type const erased_block{self.first_block + erased_offset / elements_per_block};
        ::std::destroy_at(self.pointer_at(erased_index));
        --self.element_count;

        if (self.element_count == 0) {
            element_allocator::deallocate_n(self.blocks[erased_block], elements_per_block);
            self.blocks[erased_block] = nullptr;
            self.reset_empty_position();
        }
        else if ((self.first_offset + self.element_count) % elements_per_block == 0) {
            element_allocator::deallocate_n(self.blocks[erased_block], elements_per_block);
            self.blocks[erased_block] = nullptr;
        }
    }

    constexpr void pop_front_unchecked(this Deque& self) noexcept {
        size_type const erased_block{self.first_block};
        ::std::destroy_at(self.pointer_at(0));
        --self.element_count;

        if (self.element_count == 0) {
            element_allocator::deallocate_n(self.blocks[erased_block], elements_per_block);
            self.blocks[erased_block] = nullptr;
            self.reset_empty_position();
            return;
        }

        ++self.first_offset;
        if (self.first_offset == elements_per_block) {
            element_allocator::deallocate_n(self.blocks[erased_block], elements_per_block);
            self.blocks[erased_block] = nullptr;
            ++self.first_block;
            self.first_offset = 0;
        }
    }

    constexpr void pop_back(this Deque& self) noexcept {
        self.pop_back_unchecked();
    }

    constexpr void pop_front(this Deque& self) noexcept {
        self.pop_front_unchecked();
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
            self.first_block = 0;
            return;
        }

        size_type const requested_capacity{used_blocks + 2 < initial_map_capacity ? initial_map_capacity
                                                                                  : used_blocks + 2};
        if (requested_capacity < self.map_capacity) {
            self.relocate_map(requested_capacity, 1);
        }
    }

    constexpr void resize(this Deque& self, size_type count)
        requires ::std::default_initializable<value_type>
    {
        while (self.element_count > count) {
            self.pop_back_unchecked();
        }
        while (self.element_count < count) {
            self.emplace_back();
        }
    }

    constexpr void resize(this Deque& self, size_type count, const_reference value)
        requires ::std::copy_constructible<value_type>
    {
        while (self.element_count > count) {
            self.pop_back_unchecked();
        }
        while (self.element_count < count) {
            self.emplace_back(value);
        }
    }

    template<typename... Arguments>
        requires (::std::constructible_from<value_type, Arguments...> && ::std::is_move_constructible_v<value_type> &&
                  ::std::is_move_assignable_v<value_type>)
    constexpr auto emplace(this Deque& self, const_iterator position, Arguments&&... arguments) -> iterator {
        size_type const index{position.position};
        if (index == 0) {
            self.emplace_front(::std::forward<Arguments>(arguments)...);
            return self.begin();
        }
        if (index == self.element_count) {
            self.emplace_back(::std::forward<Arguments>(arguments)...);
            return self.begin() + static_cast<difference_type>(index);
        }

        value_type value{::std::forward<Arguments>(arguments)...};
        size_type const old_size{self.element_count};
        if (index < old_size / 2) {
            self.emplace_front(::std::move(self.front_unchecked()));
            for (size_type current{1}; current != index; ++current) {
                self[current] = ::std::move(self[current + 1]);
            }
        }
        else {
            self.emplace_back(::std::move(self.back_unchecked()));
            for (size_type current{old_size - 1}; current != index; --current) {
                self[current] = ::std::move(self[current - 1]);
            }
        }
        self[index] = ::std::move(value);
        return self.begin() + static_cast<difference_type>(index);
    }

    constexpr auto insert(this Deque& self, const_iterator position, const_reference value) -> iterator
        requires (::std::copy_constructible<value_type> && ::std::is_move_constructible_v<value_type> &&
                  ::std::is_move_assignable_v<value_type>)
    {
        return self.emplace(position, value);
    }

    constexpr auto insert(this Deque& self, const_iterator position, value_type&& value) -> iterator
        requires (::std::is_move_constructible_v<value_type> && ::std::is_move_assignable_v<value_type>)
    {
        return self.emplace(position, ::std::move(value));
    }

    constexpr auto insert(this Deque& self, const_iterator position, size_type count, const_reference value) -> iterator
        requires (::std::copy_constructible<value_type> && ::std::is_move_constructible_v<value_type> &&
                  ::std::is_move_assignable_v<value_type>)
    {
        size_type const index{position.position};
        if (count == 0) {
            return self.begin() + static_cast<difference_type>(index);
        }
        value_type copy{value};
        for (size_type inserted{}; inserted != count; ++inserted) {
            self.emplace(self.cbegin() + static_cast<difference_type>(index + inserted), copy);
        }
        return self.begin() + static_cast<difference_type>(index);
    }

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
        requires (::std::is_move_constructible_v<value_type> && ::std::is_move_assignable_v<value_type>)
    constexpr auto insert(this Deque& self, const_iterator position, InputIterator first, Sentinel last) -> iterator {
        size_type const index{position.position};
        Deque values{first, last};
        size_type inserted{};
        for (reference value : values) {
            self.emplace(self.cbegin() + static_cast<difference_type>(index + inserted), ::std::move(value));
            ++inserted;
        }
        return self.begin() + static_cast<difference_type>(index);
    }

    constexpr auto insert(this Deque& self, const_iterator position, ::std::initializer_list<value_type> values)
        -> iterator
        requires (::std::copy_constructible<value_type> && ::std::is_move_constructible_v<value_type> &&
                  ::std::is_move_assignable_v<value_type>)
    {
        return self.insert(position, values.begin(), values.end());
    }

    constexpr auto erase(this Deque& self, const_iterator position) noexcept -> iterator
        requires (::std::is_move_assignable_v<value_type>)
    {
        return self.erase(position, position + 1);
    }

    constexpr auto erase(this Deque& self, const_iterator first, const_iterator last) noexcept -> iterator
        requires (::std::is_move_assignable_v<value_type>)
    {
        size_type const first_index{first.position};
        size_type const last_index{last.position};
        size_type const erased_count{last_index - first_index};
        if (erased_count == 0) {
            return self.begin() + static_cast<difference_type>(first_index);
        }

        size_type const suffix_size{self.element_count - last_index};
        if (first_index < suffix_size) {
            for (size_type current{first_index}; current != 0; --current) {
                self[current + erased_count - 1] = ::std::move(self[current - 1]);
            }
            for (size_type count{}; count != erased_count; ++count) {
                self.pop_front_unchecked();
            }
        }
        else {
            for (size_type current{first_index}; current + erased_count != self.element_count; ++current) {
                self[current] = ::std::move(self[current + erased_count]);
            }
            for (size_type count{}; count != erased_count; ++count) {
                self.pop_back_unchecked();
            }
        }
        return self.begin() + static_cast<difference_type>(first_index);
    }

    constexpr void assign(this Deque& self, size_type count, const_reference value)
        requires (::std::copy_constructible<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
        Deque replacement{count, value};
        self.swap(replacement);
    }

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
    constexpr void assign(this Deque& self, InputIterator first, Sentinel last)
        requires ::std::is_nothrow_destructible_v<value_type>
    {
        Deque replacement{first, last};
        self.swap(replacement);
    }

    constexpr void assign(this Deque& self, ::std::initializer_list<value_type> values)
        requires (::std::copy_constructible<value_type> && ::std::is_nothrow_destructible_v<value_type>)
    {
        Deque replacement{values};
        self.swap(replacement);
    }

    constexpr void swap(this Deque& self, Deque& other) noexcept {
        if (::std::addressof(self) == ::std::addressof(other)) {
            return;
        }
        ::std::swap(self.blocks, other.blocks);
        ::std::swap(self.map_capacity, other.map_capacity);
        ::std::swap(self.first_block, other.first_block);
        ::std::swap(self.first_offset, other.first_offset);
        ::std::swap(self.element_count, other.element_count);
    }
};

template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
Deque(InputIterator, Sentinel) -> Deque<::std::iter_value_t<InputIterator>>;

template<typename T>
Deque(::std::initializer_list<T>) -> Deque<T>;

template<typename T, typename LeftAllocator, typename RightAllocator>
[[nodiscard]]
constexpr auto operator==(Deque<T, LeftAllocator> const& left, Deque<T, RightAllocator> const& right) noexcept -> bool {
    return left.size() == right.size() && ::std::equal(left.begin(), left.end(), right.begin());
}

template<typename T, typename LeftAllocator, typename RightAllocator>
    requires ::std::three_way_comparable<T>
[[nodiscard]]
constexpr auto operator<=>(Deque<T, LeftAllocator> const& left, Deque<T, RightAllocator> const& right) noexcept(
    noexcept(::std::compare_three_way{}(::std::declval<T const&>(), ::std::declval<T const&>()))) {
    return ::std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end(),
                                                    ::std::compare_three_way{});
}

template<typename T, typename Allocator>
constexpr void swap(Deque<T, Allocator>& left, Deque<T, Allocator>& right) noexcept {
    left.swap(right);
}

template<typename T, typename Allocator, typename U>
constexpr auto erase(Deque<T, Allocator>& container, U const& value) -> typename Deque<T, Allocator>::size_type {
    typename Deque<T, Allocator>::size_type const old_size{container.size()};
    auto iterator{container.begin()};
    while (iterator != container.end()) {
        if (*iterator == value) {
            iterator = container.erase(iterator);
        }
        else {
            ++iterator;
        }
    }
    return old_size - container.size();
}

template<typename T, typename Allocator, typename Predicate>
constexpr auto erase_if(Deque<T, Allocator>& container, Predicate predicate) ->
    typename Deque<T, Allocator>::size_type {
    typename Deque<T, Allocator>::size_type const old_size{container.size()};
    auto first_removed{::std::remove_if(container.begin(), container.end(), predicate)};
    container.erase(first_removed, container.end());
    return old_size - container.size();
}

} // namespace pltxt2htm::container

#include "../details/pop_macro.hh"
