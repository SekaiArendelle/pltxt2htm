/**
 * @file allocator_concepts.hh
 * @brief Concepts classifying allocators usable with `GenericAllocatorAdapter`,
 *        built on the detection traits in `allocator_detect.hh`. The capability
 *        unions mirror the dispatch conditions of the adapter itself
 *        (e.g. `GenericAllocatorAdapter::has_native_allocate`).
 */

#pragma once

#include "allocator_detect.hh"

namespace pltxt2htm::memory::details {

template<typename Alloc>
concept allocator_has_stateless_allocate_ops =
    has_allocate_impl<Alloc> || has_allocate_aligned_impl<Alloc> || has_allocate_zero_impl<Alloc> ||
    has_allocate_aligned_zero_impl<Alloc> || has_allocate_conditional_zero_impl<Alloc> ||
    has_allocate_aligned_conditional_zero_impl<Alloc> || has_allocate_at_least_impl<Alloc> ||
    has_allocate_aligned_at_least_impl<Alloc> || has_allocate_zero_at_least_impl<Alloc> ||
    has_allocate_aligned_zero_at_least_impl<Alloc> || has_allocate_conditional_zero_at_least_impl<Alloc> ||
    has_allocate_aligned_conditional_zero_at_least_impl<Alloc>;

template<typename Alloc>
concept allocator_has_stateless_deallocate_ops = has_deallocate_impl<Alloc> || has_deallocate_aligned_impl<Alloc> ||
                                                 has_deallocate_n_impl<Alloc> || has_deallocate_aligned_n_impl<Alloc>;

} // namespace pltxt2htm::memory::details

namespace pltxt2htm::memory {

/**
 * @brief An allocator whose allocation surface is entirely static (no handle).
 */
template<typename Alloc>
concept stateless_allocator =
    details::allocator_has_stateless_allocate_ops<Alloc> && details::allocator_has_stateless_deallocate_ops<Alloc>;

/**
 * @brief An allocator usable with `GenericAllocatorAdapter`.
 */
template<typename Alloc>
concept allocator = stateless_allocator<Alloc>;

} // namespace pltxt2htm::memory
