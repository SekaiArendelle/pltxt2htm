/**
 * @file allocator_detect.hh
 * @brief Detection traits for fast_io-style allocators.
 * @details Adapted from fast_io `fast_io_core_impl/allocation/has_methods_detect.h`.
 *          Detection is limited to the stateless allocator operations supported here.
 */

#pragma once

#include "allocation_least_result.hh"

namespace pltxt2htm::memory::details {

template<typename alloc>
concept has_allocate_impl = requires(::std::size_t n) {
    { alloc::allocate(n) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_allocate_aligned_impl = requires(::std::size_t n) {
    { alloc::allocate_aligned(n, n) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_allocate_zero_impl = requires(::std::size_t n) {
    { alloc::allocate_zero(n) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_allocate_aligned_zero_impl = requires(::std::size_t n) {
    { alloc::allocate_aligned_zero(n, n) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_allocate_conditional_zero_impl = requires(::std::size_t n, bool zero) {
    { alloc::allocate_conditional_zero(n, zero) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_allocate_aligned_conditional_zero_impl = requires(::std::size_t n, bool zero) {
    { alloc::allocate_aligned_conditional_zero(n, n, zero) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_reallocate_impl = requires(void* p, ::std::size_t n) {
    { alloc::reallocate(p, n) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_reallocate_aligned_impl = requires(void* p, ::std::size_t n) {
    { alloc::reallocate_aligned(p, n, n) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_reallocate_zero_impl = requires(void* p, ::std::size_t n) {
    { alloc::reallocate_zero(p, n) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_reallocate_aligned_zero_impl = requires(void* p, ::std::size_t n) {
    { alloc::reallocate_aligned_zero(p, n, n) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_reallocate_conditional_zero_impl = requires(void* p, ::std::size_t n, bool zero) {
    { alloc::reallocate_conditional_zero(p, n, zero) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_reallocate_aligned_conditional_zero_impl = requires(void* p, ::std::size_t n, bool zero) {
    { alloc::reallocate_aligned_conditional_zero(p, n, n, zero) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_reallocate_n_impl = requires(void* p, ::std::size_t n) {
    { alloc::reallocate_n(p, n, n) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_reallocate_aligned_n_impl = requires(void* p, ::std::size_t n) {
    { alloc::reallocate_aligned_n(p, n, n, n) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_reallocate_zero_n_impl = requires(void* p, ::std::size_t n) {
    { alloc::reallocate_zero_n(p, n, n) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_reallocate_aligned_zero_n_impl = requires(void* p, ::std::size_t n) {
    { alloc::reallocate_aligned_zero_n(p, n, n, n) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_reallocate_n_conditional_zero_impl = requires(void* p, ::std::size_t n, bool zero) {
    { alloc::reallocate_n_conditional_zero(p, n, n, zero) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_reallocate_aligned_n_conditional_zero_impl = requires(void* p, ::std::size_t n, bool zero) {
    { alloc::reallocate_aligned_n_conditional_zero(p, n, n, n, zero) } -> ::std::same_as<void*>;
};

template<typename alloc>
concept has_deallocate_impl = requires(void* p) {
    { alloc::deallocate(p) } -> ::std::same_as<void>;
};

template<typename alloc>
concept has_deallocate_aligned_impl = requires(void* p, ::std::size_t n) {
    { alloc::deallocate_aligned(p, n) } -> ::std::same_as<void>;
};

template<typename alloc>
concept has_deallocate_n_impl = requires(void* p, ::std::size_t n) {
    { alloc::deallocate_n(p, n) } -> ::std::same_as<void>;
};

template<typename alloc>
concept has_deallocate_aligned_n_impl = requires(void* p, ::std::size_t n) {
    { alloc::deallocate_aligned_n(p, n, n) } -> ::std::same_as<void>;
};

template<typename alloc>
concept has_allocate_at_least_impl = requires(::std::size_t n) {
    { alloc::allocate_at_least(n) } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_allocate_aligned_at_least_impl = requires(::std::size_t n) {
    { alloc::allocate_aligned_at_least(n, n) } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_allocate_zero_at_least_impl = requires(::std::size_t n) {
    { alloc::allocate_zero_at_least(n) } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_allocate_aligned_zero_at_least_impl = requires(::std::size_t n) {
    { alloc::allocate_aligned_zero_at_least(n, n) } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_allocate_conditional_zero_at_least_impl = requires(::std::size_t n, bool zero) {
    {
        alloc::allocate_conditional_zero_at_least(n, zero)
    } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_allocate_aligned_conditional_zero_at_least_impl = requires(::std::size_t n, bool zero) {
    {
        alloc::allocate_aligned_conditional_zero_at_least(n, n, zero)
    } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_reallocate_at_least_impl = requires(void* p, ::std::size_t n) {
    { alloc::reallocate_at_least(p, n) } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_reallocate_aligned_at_least_impl = requires(void* p, ::std::size_t n) {
    { alloc::reallocate_aligned_at_least(p, n, n) } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_reallocate_zero_at_least_impl = requires(void* p, ::std::size_t n) {
    { alloc::reallocate_zero_at_least(p, n) } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_reallocate_aligned_zero_at_least_impl = requires(void* p, ::std::size_t n) {
    { alloc::reallocate_aligned_zero_at_least(p, n, n) } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_reallocate_conditional_zero_at_least_impl = requires(void* p, ::std::size_t n, bool zero) {
    {
        alloc::reallocate_conditional_zero_at_least(p, n, zero)
    } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_reallocate_aligned_conditional_zero_at_least_impl = requires(void* p, ::std::size_t n, bool zero) {
    {
        alloc::reallocate_aligned_conditional_zero_at_least(p, n, n, zero)
    } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_reallocate_n_at_least_impl = requires(void* p, ::std::size_t n) {
    { alloc::reallocate_n_at_least(p, n, n) } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_reallocate_aligned_n_at_least_impl = requires(void* p, ::std::size_t n) {
    { alloc::reallocate_aligned_n_at_least(p, n, n, n) } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_reallocate_zero_n_at_least_impl = requires(void* p, ::std::size_t n) {
    { alloc::reallocate_zero_n_at_least(p, n, n) } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_reallocate_aligned_zero_n_at_least_impl = requires(void* p, ::std::size_t n) {
    {
        alloc::reallocate_aligned_zero_n_at_least(p, n, n, n)
    } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_reallocate_n_conditional_zero_at_least_impl = requires(void* p, ::std::size_t n, bool zero) {
    {
        alloc::reallocate_n_conditional_zero_at_least(p, n, n, zero)
    } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept has_reallocate_aligned_n_conditional_zero_at_least_impl = requires(void* p, ::std::size_t n, bool zero) {
    {
        alloc::reallocate_aligned_n_conditional_zero_at_least(p, n, n, n, zero)
    } -> ::std::same_as<::pltxt2htm::memory::AllocationLeastResult>;
};

template<typename alloc>
concept native_allocate_aligned_has_none_zero_ops =
    ::pltxt2htm::memory::details::has_allocate_aligned_impl<alloc> ||
    ::pltxt2htm::memory::details::has_allocate_aligned_at_least_impl<alloc>;
template<typename alloc>
concept native_allocate_aligned_has_zero_ops =
    ::pltxt2htm::memory::details::has_allocate_aligned_zero_impl<alloc> ||
    ::pltxt2htm::memory::details::has_allocate_aligned_zero_at_least_impl<alloc>;

template<typename alloc>
concept native_allocate_aligned_has_ops =
    ::pltxt2htm::memory::details::native_allocate_aligned_has_none_zero_ops<alloc> ||
    ::pltxt2htm::memory::details::native_allocate_aligned_has_zero_ops<alloc>;

template<typename alloc>
concept native_allocate_has_none_zero_ops =
    ::pltxt2htm::memory::details::has_allocate_impl<alloc> ||
    ::pltxt2htm::memory::details::has_allocate_at_least_impl<alloc> ||
    ::pltxt2htm::memory::details::native_allocate_aligned_has_none_zero_ops<alloc>;
template<typename alloc>
concept native_allocate_has_zero_ops = ::pltxt2htm::memory::details::has_allocate_zero_impl<alloc> ||
                                       ::pltxt2htm::memory::details::has_allocate_zero_at_least_impl<alloc> ||
                                       ::pltxt2htm::memory::details::native_allocate_aligned_has_zero_ops<alloc>;

} // namespace pltxt2htm::memory::details
