/**
 * @file deque_def.inc
 * @brief Out-of-line contract-checked definitions for container::Deque.
 */

#pragma once

#include "deque_decl.hh"
#include "../../details/push_macro.hh"

namespace pltxt2htm::container {

template<typename T, typename Allocator>
constexpr void Deque<T, Allocator>::grow_map(this Deque& self, size_type required_front,
                                             size_type required_back) noexcept {
    size_type const used_blocks{self.allocated_block_count()};
    size_type const front_spare{self.has_front_spare() ? 1u : 0u};
    size_type const back_spare{self.has_back_spare() ? 1u : 0u};
    size_type const occupied_blocks{used_blocks + front_spare + back_spare};
    size_type const required_capacity{occupied_blocks + required_front + required_back};
    pltxt2htm_assert(required_capacity <= max_map_capacity, u8"Deque map capacity exceeds max_map_capacity");
    size_type new_capacity{self.map_capacity == 0 ? initial_map_capacity : self.map_capacity};
    // Leave enough slack to amortize map movement when the queue is nearly full.
    while (new_capacity < required_capacity || occupied_blocks > new_capacity / 2) {
        if (new_capacity > max_map_capacity / 2) {
            new_capacity = required_capacity;
            break;
        }
        new_capacity *= 2;
    }

    size_type new_first_occupied{(new_capacity - occupied_blocks) / 2};
    if (new_first_occupied < required_front) {
        new_first_occupied = required_front;
    }
    if (new_capacity - new_first_occupied - occupied_blocks < required_back) {
        new_first_occupied = new_capacity - occupied_blocks - required_back;
    }
    size_type const new_first_block{new_first_occupied + front_spare};
    if (new_capacity != self.map_capacity) {
        self.relocate_map(new_capacity, new_first_block);
        return;
    }

    size_type const first_block{self.start_offset / elements_per_block};
    size_type const first_occupied{first_block - front_spare};
    if (new_first_occupied < first_occupied) {
        ::std::copy(self.blocks + first_occupied, self.blocks + first_occupied + occupied_blocks,
                    self.blocks + new_first_occupied);
    }
    else if (new_first_occupied > first_occupied) {
        ::std::copy_backward(self.blocks + first_occupied, self.blocks + first_occupied + occupied_blocks,
                             self.blocks + new_first_occupied + occupied_blocks);
    }
    for (size_type index{}; index != self.map_capacity; ++index) {
        if (index < new_first_occupied || index >= new_first_occupied + occupied_blocks) {
            self.blocks[index] = nullptr;
        }
    }
    self.start_offset = new_first_block * elements_per_block + self.start_offset % elements_per_block;
}

template<typename T, typename Allocator>
constexpr auto Deque<T, Allocator>::operator=(this Deque& self, Deque const& other) noexcept -> Deque&
    requires ::std::is_nothrow_copy_constructible_v<value_type>
{
#if defined(PLTXT2HTM_INTERNAL_USE)
    pltxt2htm_assert(::std::addressof(self) != ::std::addressof(other), u8"Internal self-assignment is not allowed");
#else
    if (::std::addressof(self) == ::std::addressof(other)) {
        return self;
    }
#endif
    Deque copy{other};
    self.swap(copy);
    return self;
}

template<typename T, typename Allocator>
constexpr auto Deque<T, Allocator>::operator=(this Deque& self, Deque&& other) noexcept -> Deque& {
#if defined(PLTXT2HTM_INTERNAL_USE)
    pltxt2htm_assert(::std::addressof(self) != ::std::addressof(other), u8"Internal self-assignment is not allowed");
#else
    if (::std::addressof(self) == ::std::addressof(other)) {
        return self;
    }
#endif
    self.swap(other);
    return self;
}

template<typename T, typename Allocator>
constexpr auto Deque<T, Allocator>::operator[](this Deque& self, size_type position) noexcept -> reference {
    pltxt2htm_assert(position < self.element_count, u8"Index of Deque out of bound");
    return *self.pointer_at(position);
}

template<typename T, typename Allocator>
constexpr auto Deque<T, Allocator>::operator[](this Deque const& self, size_type position) noexcept -> const_reference {
    pltxt2htm_assert(position < self.element_count, u8"Index of Deque out of bound");
    return *self.pointer_at(position);
}

template<typename T, typename Allocator>
constexpr auto Deque<T, Allocator>::front(this Deque& self) noexcept -> reference {
    pltxt2htm_assert(!self.is_empty(), u8"Accessing front of empty Deque");
    return *self.pointer_at(0);
}

template<typename T, typename Allocator>
constexpr auto Deque<T, Allocator>::front(this Deque const& self) noexcept -> const_reference {
    pltxt2htm_assert(!self.is_empty(), u8"Accessing front of empty Deque");
    return *self.pointer_at(0);
}

template<typename T, typename Allocator>
constexpr auto Deque<T, Allocator>::back(this Deque& self) noexcept -> reference {
    pltxt2htm_assert(!self.is_empty(), u8"Accessing back of empty Deque");
    return *self.pointer_at(self.element_count - 1);
}

template<typename T, typename Allocator>
constexpr auto Deque<T, Allocator>::back(this Deque const& self) noexcept -> const_reference {
    pltxt2htm_assert(!self.is_empty(), u8"Accessing back of empty Deque");
    return *self.pointer_at(self.element_count - 1);
}

template<typename T, typename Allocator>
template<typename... Args>
constexpr auto Deque<T, Allocator>::emplace_back(this Deque& self, Args&&... args) noexcept -> reference
    requires ::std::is_nothrow_constructible_v<value_type, Args...>
{
    pltxt2htm_assert(self.element_count < self.max_size(), u8"Deque size exceeds max_size");
    size_type const insertion_offset{self.start_offset + self.element_count};
    size_type const block_offset{insertion_offset % elements_per_block};
    bool const needs_block{self.is_empty() || block_offset == 0};

    if (needs_block) {
        self.ensure_map();
        size_type const used_blocks{self.allocated_block_count()};
        size_type const first_block{self.start_offset / elements_per_block};
        size_type map_index{first_block + used_blocks};
        pointer new_block{map_index < self.map_capacity ? self.blocks[map_index] : nullptr};
        if (new_block == nullptr && self.has_front_spare()) {
            new_block = self.blocks[first_block - 1];
            self.blocks[first_block - 1] = nullptr;
        }
        if (map_index == self.map_capacity) {
            if (new_block != nullptr) {
                ::std::copy(self.blocks + first_block, self.blocks + self.map_capacity, self.blocks + first_block - 1);
                self.start_offset -= elements_per_block;
                --map_index;
            }
            else {
                self.grow_map(0, 1);
                map_index = self.start_offset / elements_per_block + used_blocks;
            }
        }
        if (new_block == nullptr) {
            new_block = element_allocator::allocate(elements_per_block);
        }
        pointer const result{::std::construct_at(new_block, ::std::forward<Args>(args)...)};
        self.blocks[map_index] = new_block;
        ++self.element_count;
        return *result;
    }

    pointer const result{::std::construct_at(self.pointer_at(self.element_count), ::std::forward<Args>(args)...)};
    ++self.element_count;
    return *result;
}

template<typename T, typename Allocator>
template<typename... Args>
constexpr auto Deque<T, Allocator>::emplace_front(this Deque& self, Args&&... args) noexcept -> reference
    requires ::std::is_nothrow_constructible_v<value_type, Args...>
{
    pltxt2htm_assert(self.element_count < self.max_size(), u8"Deque size exceeds max_size");
    if (self.start_offset % elements_per_block != 0) {
        pointer const result{::std::construct_at(
            self.blocks[self.start_offset / elements_per_block] + self.start_offset % elements_per_block - 1,
            ::std::forward<Args>(args)...)};
        --self.start_offset;
        ++self.element_count;
        return *result;
    }

    self.ensure_map();
    size_type const used_blocks{self.allocated_block_count()};
    size_type const first_block{self.start_offset / elements_per_block};
    pointer new_block{first_block != 0 ? self.blocks[first_block - 1] : nullptr};
    if (new_block == nullptr && self.has_back_spare()) {
        size_type const back_block{first_block + used_blocks};
        new_block = self.blocks[back_block];
        self.blocks[back_block] = nullptr;
    }
    if (first_block == 0) {
        if (new_block != nullptr) {
            ::std::copy_backward(self.blocks, self.blocks + used_blocks, self.blocks + used_blocks + 1);
            self.start_offset += elements_per_block;
        }
        else {
            self.grow_map(1, 0);
        }
    }
    if (new_block == nullptr) {
        new_block = element_allocator::allocate(elements_per_block);
    }
    pointer const result{::std::construct_at(new_block + elements_per_block - 1, ::std::forward<Args>(args)...)};
    --self.start_offset;
    self.blocks[self.start_offset / elements_per_block] = new_block;
    ++self.element_count;
    return *result;
}

template<typename T, typename Allocator>
constexpr void Deque<T, Allocator>::pop_back(this Deque& self) noexcept {
    pltxt2htm_assert(!self.is_empty(), u8"Popping back of empty Deque");
    size_type const erased_index{self.element_count - 1};
    size_type const erased_offset{self.start_offset + erased_index};
    size_type const erased_block{erased_offset / elements_per_block};
    ::std::destroy_at(self.pointer_at(erased_index));
    --self.element_count;

    if (self.element_count == 0 || (self.start_offset + self.element_count) % elements_per_block == 0) {
        if (erased_block + 1 < self.map_capacity && self.blocks[erased_block + 1] != nullptr) {
            element_allocator::deallocate_n(self.blocks[erased_block + 1], elements_per_block);
            self.blocks[erased_block + 1] = nullptr;
        }
        if (self.element_count == 0) {
            self.reset_empty_position();
        }
    }
}

template<typename T, typename Allocator>
constexpr void Deque<T, Allocator>::pop_front(this Deque& self) noexcept {
    pltxt2htm_assert(!self.is_empty(), u8"Popping front of empty Deque");
    size_type const erased_block{self.start_offset / elements_per_block};
    ::std::destroy_at(self.pointer_at(0));
    --self.element_count;

    if (self.element_count == 0) {
        if (erased_block != 0 && self.blocks[erased_block - 1] != nullptr) {
            element_allocator::deallocate_n(self.blocks[erased_block - 1], elements_per_block);
            self.blocks[erased_block - 1] = nullptr;
        }
        self.start_offset = (erased_block + 1) * elements_per_block;
        self.reset_empty_position();
        return;
    }

    ++self.start_offset;
    if (self.start_offset % elements_per_block == 0) {
        if (erased_block != 0 && self.blocks[erased_block - 1] != nullptr) {
            element_allocator::deallocate_n(self.blocks[erased_block - 1], elements_per_block);
            self.blocks[erased_block - 1] = nullptr;
        }
    }
}

template<typename T, typename Allocator>
template<typename... Args>
constexpr auto Deque<T, Allocator>::emplace(this Deque& self, const_iterator position, Args&&... args) noexcept
    -> iterator
    requires (::std::is_nothrow_constructible_v<value_type, Args...> &&
              ::std::is_nothrow_move_constructible_v<value_type> && ::std::is_nothrow_move_assignable_v<value_type>)
{
    size_type const index{static_cast<size_type>(position - self.cbegin())};
    pltxt2htm_assert(index <= self.element_count, u8"Insertion position of Deque out of bound");
    if (index == 0) {
        self.emplace_front(::std::forward<Args>(args)...);
        return self.begin();
    }
    if (index == self.element_count) {
        self.emplace_back(::std::forward<Args>(args)...);
        return self.begin() + static_cast<difference_type>(index);
    }

    value_type value{::std::forward<Args>(args)...};
    size_type const old_size{self.element_count};
    if (index < old_size / 2) {
        self.emplace_front(::std::move(self.front()));
        for (size_type current{1}; current != index; ++current) {
            self[current] = ::std::move(self[current + 1]);
        }
    }
    else {
        self.emplace_back(::std::move(self.back()));
        for (size_type current{old_size - 1}; current != index; --current) {
            self[current] = ::std::move(self[current - 1]);
        }
    }
    self[index] = ::std::move(value);
    return self.begin() + static_cast<difference_type>(index);
}

template<typename T, typename Allocator>
template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
constexpr auto Deque<T, Allocator>::insert(this Deque& self, const_iterator position, InputIterator first,
                                           Sentinel last) noexcept -> iterator
    requires (is_nothrow_input_range<InputIterator, Sentinel>() && ::std::is_nothrow_move_constructible_v<value_type> &&
              ::std::is_nothrow_move_assignable_v<value_type>)
{
    size_type const index{static_cast<size_type>(position - self.cbegin())};
    pltxt2htm_assert(index <= self.element_count, u8"Insertion position of Deque out of bound");
    Deque values{first, last};
    size_type const inserted{values.size()};
    size_type const old_size{self.element_count};
    pltxt2htm_assert(inserted <= self.max_size() - old_size, u8"Deque size exceeds max_size");
    if (inserted == 0) {
        return self.begin() + static_cast<difference_type>(index);
    }
    if (index < old_size - index) {
        for (size_type remaining{inserted}; remaining != 0; --remaining) {
            self.emplace_front(::std::move(values[remaining - 1]));
        }
        self.rotate_elements(0, inserted, inserted + index);
    }
    else {
        for (reference value : values) {
            self.emplace_back(::std::move(value));
        }
        self.rotate_elements(index, old_size, old_size + inserted);
    }
    return self.begin() + static_cast<difference_type>(index);
}

template<typename T, typename Allocator>
constexpr auto Deque<T, Allocator>::erase(this Deque& self, const_iterator position) noexcept -> iterator
    requires ::std::is_nothrow_move_assignable_v<value_type>
{
    pltxt2htm_assert(position != self.cend(), u8"Erasing past the end of Deque");
    return self.erase(position, position + 1);
}

template<typename T, typename Allocator>
constexpr auto Deque<T, Allocator>::erase(this Deque& self, const_iterator first, const_iterator last) noexcept
    -> iterator
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
            self[current + erased_count - 1] = ::std::move(self[current - 1]);
        }
        for (size_type count{}; count != erased_count; ++count) {
            self.pop_front();
        }
    }
    else {
        for (size_type current{first_index}; current + erased_count != self.element_count; ++current) {
            self[current] = ::std::move(self[current + erased_count]);
        }
        for (size_type count{}; count != erased_count; ++count) {
            self.pop_back();
        }
    }
    return self.begin() + static_cast<difference_type>(first_index);
}

} // namespace pltxt2htm::container

#include "../../details/pop_macro.hh"
