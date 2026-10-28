/**
 * @file allocator_adapter.hh
 * @brief Generic and typed allocator adapters, synced from fast_io
 *          `fast_io_core_impl/allocation/adapters.h`.
 * @details Class, method, and concept names are kept identical to upstream so
 *          stateless fast_io-style allocators work unchanged; only the namespace differs
 *          (`pltxt2htm::memory` instead of `fast_io`). Dependencies were remapped:
 *          `fast_terminate` -> `pltxt2htm::details::trap`, `fast_io::freestanding`
 *          byte/string helpers -> local `details` equivalents, and the consteval
 *          allocator path now uses a local `consteval_allocator` mirroring
 *          `fast_io::freestanding::allocator`.
 *          Deviating from upstream, some `*_at_least` dispatchers gained a final
 *          else fallback (e.g. `reallocate_zero_at_least` synthesizes zeroing by
 *          delegating to `reallocate_at_least` and clearing the whole block);
 *          upstream leaves the template chain dangling there and emits a trap
 *          for allocators that reallocate but implement no zero variant.
 *
 * To make a constexpr allocator, we need ::std::allocator. Because only new expression and
 * ::std::allocator<T>::allocate are allowed in constexpr functions. See https://github.com/microsoft/STL/issues/1532
 * https://github.com/microsoft/STL/issues/4002 gcc and clang provide constexpr new, but still won't compile.
 * ::std::allocator<T> is NOT freestanding.
 */

#pragma once

#include <cstddef>
#include <limits>
#include <memory>
#include <new>

#include "../../details/trap.hh"
#include "allocation_least_result.hh"
#include "allocator_detect.hh"
#include "byte_access.hh"

namespace pltxt2htm::memory {
namespace details {

template<typename alloc>
concept has_default_alignment_impl = requires(::std::size_t n) { alloc::default_alignment; };

template<typename alloc>
constexpr auto calculate_default_alignment() noexcept -> ::std::size_t {
    if constexpr (has_default_alignment_impl<alloc>) {
        return alloc::default_alignment;
    }
    else {
        return __STDCPP_DEFAULT_NEW_ALIGNMENT__;
    }
}

constexpr auto allocator_compute_aligned_total_size_impl(::std::size_t alignment, ::std::size_t n) noexcept
    -> ::std::size_t {
    constexpr ::std::size_t mxn{::std::numeric_limits<::std::size_t>::max()}, sizeofptr{sizeof(void*)},
        mxmptr{mxn - sizeofptr};
    if (alignment < sizeofptr) {
        alignment = sizeofptr;
    }
    if (alignment > mxmptr) {
        ::pltxt2htm::details::trap();
    }
    ::std::size_t const total_extra_space{alignment + sizeofptr};
    ::std::size_t const upperlimit{static_cast<::std::size_t>(mxn - total_extra_space)};
    if (n > upperlimit) {
        ::pltxt2htm::details::trap();
    }
    return n + total_extra_space;
}

inline auto allocator_adjust_ptr_to_aligned_impl(void* p, ::std::size_t alignment) noexcept -> void* {
    void* aligned_ptr{reinterpret_cast<void*>((reinterpret_cast<::std::uintptr_t>(p) + alignment) & (0 - alignment))};
    reinterpret_cast<void**>(aligned_ptr)[-1] = p;
    return aligned_ptr;
}

template<typename>
constexpr auto allocator_pointer_aligned_impl(::std::size_t, ::std::size_t, bool) noexcept -> void*;

template<typename>
constexpr auto allocator_pointer_aligned_at_least_impl(::std::size_t, ::std::size_t, bool) noexcept
    -> ::pltxt2htm::memory::AllocationLeastResult;
} // namespace details

// Upstream `allocator_adapter_flags.h` (compile-time adapter feature flags) was not ported.

template<typename alloc>
class GenericAllocatorAdapter {
public:
    using allocator_type = alloc;
    static constexpr ::std::size_t default_alignment{
        ::pltxt2htm::memory::details::calculate_default_alignment<allocator_type>()};
    static constexpr bool has_native_allocate{
        ::pltxt2htm::memory::details::has_allocate_at_least_impl<alloc> ||
        ::pltxt2htm::memory::details::has_allocate_aligned_at_least_impl<alloc> ||
        ::pltxt2htm::memory::details::has_allocate_zero_at_least_impl<alloc> ||
        ::pltxt2htm::memory::details::has_allocate_aligned_zero_at_least_impl<alloc> ||
        ::pltxt2htm::memory::details::has_allocate_impl<alloc> ||
        ::pltxt2htm::memory::details::has_allocate_aligned_impl<alloc> ||
        ::pltxt2htm::memory::details::has_allocate_zero_impl<alloc> ||
        ::pltxt2htm::memory::details::has_allocate_aligned_zero_impl<alloc> ||
        ::pltxt2htm::memory::details::has_allocate_conditional_zero_impl<alloc> ||
        ::pltxt2htm::memory::details::has_allocate_aligned_conditional_zero_impl<alloc> ||
        ::pltxt2htm::memory::details::has_allocate_conditional_zero_at_least_impl<alloc> ||
        ::pltxt2htm::memory::details::has_allocate_aligned_conditional_zero_at_least_impl<alloc>};

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto allocate_conditional_zero(::std::size_t n, bool zero) noexcept -> void* {
        if consteval {
            auto p{
#if defined(__has_builtin) && __has_builtin(__builtin_operator_new)
                __builtin_operator_new(n)
#else
                ::operator new(n)
#endif
            };
            if (zero) {
                ::pltxt2htm::memory::details::bytes_clear_n(static_cast<::std::byte*>(p), n);
            }
            return p;
        }
        else {
            if constexpr (::pltxt2htm::memory::details::has_allocate_conditional_zero_impl<alloc>) {
                return allocator_type::allocate_conditional_zero(n, zero);
            }
            else if constexpr (::pltxt2htm::memory::details::has_allocate_conditional_zero_at_least_impl<alloc>) {
                return allocator_type::allocate_conditional_zero_at_least(n, zero).ptr;
            }
            else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_conditional_zero_impl<alloc>) {
                return allocator_type::allocate_aligned_conditional_zero(default_alignment, n, zero);
            }
            else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_conditional_zero_at_least_impl<
                                   alloc>) {
                return allocator_type::allocate_aligned_conditional_zero_at_least(default_alignment, n, zero).ptr;
            }
            else {
                constexpr bool has_none_zero_ops{
                    ::pltxt2htm::memory::details::native_allocate_has_none_zero_ops<alloc>};
                constexpr bool has_zero_ops{::pltxt2htm::memory::details::native_allocate_has_zero_ops<alloc>};
                if constexpr (!has_none_zero_ops && !has_zero_ops) {
                    ::pltxt2htm::details::trap();
#if __has_cpp_attribute(unreachable)
                    [[unreachable]];
#endif
                }
                else if constexpr (!has_none_zero_ops && has_zero_ops) {
                    if constexpr (::pltxt2htm::memory::details::has_allocate_zero_impl<alloc>) {
                        return allocator_type::allocate_zero(n);
                    }
                    else if constexpr (::pltxt2htm::memory::details::has_allocate_zero_at_least_impl<alloc>) {
                        return allocator_type::allocate_zero_at_least(n).ptr;
                    }
                    else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_zero_impl<alloc>) {
                        return allocator_type::allocate_aligned_zero(default_alignment, n);
                    }
                    else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_zero_at_least_impl<alloc>) {
                        return allocator_type::allocate_aligned_zero_at_least(default_alignment, n).ptr;
                    }
                }
                else if constexpr (has_none_zero_ops && !has_zero_ops) {
                    void* ptr;
                    if constexpr (::pltxt2htm::memory::details::has_allocate_impl<alloc>) {
                        ptr = allocator_type::allocate(n);
                    }
                    else if constexpr (::pltxt2htm::memory::details::has_allocate_at_least_impl<alloc>) {
                        ptr = allocator_type::allocate_at_least(n).ptr;
                    }
                    else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_impl<alloc>) {
                        ptr = allocator_type::allocate_aligned(default_alignment, n);
                    }
                    else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_at_least_impl<alloc>) {
                        ptr = allocator_type::allocate_aligned_at_least(default_alignment, n).ptr;
                    }
                    if (zero) {
                        ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(ptr), n);
                    }
                    return ptr;
                }
                else {
                    void* ptr;
                    if (zero) {
                        if constexpr (::pltxt2htm::memory::details::has_allocate_zero_impl<alloc>) {
                            ptr = allocator_type::allocate_zero(n);
                        }
                        else if constexpr (::pltxt2htm::memory::details::has_allocate_zero_at_least_impl<alloc>) {
                            ptr = allocator_type::allocate_zero_at_least(n).ptr;
                        }
                        else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_zero_impl<alloc>) {
                            ptr = allocator_type::allocate_aligned_zero(default_alignment, n);
                        }
                        else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_zero_at_least_impl<
                                               alloc>) {
                            ptr = allocator_type::allocate_aligned_zero_at_least(default_alignment, n).ptr;
                        }
                        else {
                            ::pltxt2htm::details::trap();
                        }
                    }
                    else {
                        if constexpr (::pltxt2htm::memory::details::has_allocate_impl<alloc>) {
                            ptr = allocator_type::allocate(n);
                        }
                        else if constexpr (::pltxt2htm::memory::details::has_allocate_at_least_impl<alloc>) {
                            ptr = allocator_type::allocate_at_least(n).ptr;
                        }
                        else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_impl<alloc>) {
                            ptr = allocator_type::allocate_aligned(default_alignment, n);
                        }
                        else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_at_least_impl<alloc>) {
                            ptr = allocator_type::allocate_aligned_at_least(default_alignment, n).ptr;
                        }
                        else {
                            ::pltxt2htm::details::trap();
                        }
                    }
                    return ptr;
                }
            }
        }
    }
#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static constexpr auto allocate(::std::size_t n) noexcept -> void* {
        if consteval {
            return GenericAllocatorAdapter::allocate_conditional_zero(n, false);
        }
        else {
            if constexpr (::pltxt2htm::memory::details::has_allocate_impl<alloc>) {
                return allocator_type::allocate(n);
            }
            else {
                return GenericAllocatorAdapter::allocate_conditional_zero(n, false);
            }
        }
    }

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto allocate_zero(::std::size_t n) noexcept -> void* {
        if consteval {
            return GenericAllocatorAdapter::allocate_conditional_zero(n, true);
        }
        else {
            if constexpr (::pltxt2htm::memory::details::has_allocate_zero_impl<alloc>) {
                return allocator_type::allocate_zero(n);
            }
            else {
                return GenericAllocatorAdapter::allocate_conditional_zero(n, true);
            }
        }
    }

    static constexpr bool has_reallocate =
        (::pltxt2htm::memory::details::has_reallocate_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_at_least_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_aligned_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_aligned_at_least_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_zero_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_zero_at_least_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_aligned_zero_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_conditional_zero_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_aligned_conditional_zero_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_conditional_zero_at_least_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_aligned_conditional_zero_at_least_impl<alloc>);

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto reallocate_conditional_zero(void* p, ::std::size_t n, bool zero) noexcept -> void*
        requires (has_reallocate)
    {
        if constexpr (::pltxt2htm::memory::details::has_reallocate_conditional_zero_impl<alloc>) {
            return allocator_type::reallocate_conditional_zero(p, n, zero);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_conditional_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_conditional_zero_at_least(p, n, zero).ptr;
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_conditional_zero_impl<alloc>) {
            return allocator_type::reallocate_aligned_conditional_zero(p, default_alignment, n, zero);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_conditional_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_conditional_zero_at_least(p, default_alignment, n, zero).ptr;
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_impl<alloc> ||
                           ::pltxt2htm::memory::details::has_reallocate_at_least_impl<alloc> ||
                           ::pltxt2htm::memory::details::has_reallocate_aligned_impl<alloc> ||
                           ::pltxt2htm::memory::details::has_reallocate_aligned_at_least_impl<alloc>) {
            // Non-zero APIs exist - need runtime branch
            if (zero) {
                if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_impl<alloc>) {
                    return allocator_type::reallocate_zero(p, n);
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_at_least_impl<alloc>) {
                    return allocator_type::reallocate_zero_at_least(p, n).ptr;
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_impl<alloc>) {
                    return allocator_type::reallocate_aligned_zero(p, default_alignment, n);
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>) {
                    return allocator_type::reallocate_aligned_zero_at_least(p, default_alignment, n).ptr;
                }
                else {
                    auto newptr{GenericAllocatorAdapter::reallocate(p, n)};
                    ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(newptr), n);
                    return newptr;
                }
            }
            else {
                if constexpr (::pltxt2htm::memory::details::has_reallocate_impl<alloc>) {
                    return allocator_type::reallocate(p, n);
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_at_least_impl<alloc>) {
                    return allocator_type::reallocate_at_least(p, n).ptr;
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_impl<alloc>) {
                    return allocator_type::reallocate_aligned(p, default_alignment, n);
                }
                else {
                    return allocator_type::reallocate_aligned_at_least(p, default_alignment, n).ptr;
                }
            }
        }
        else {
            // Only zero APIs exist - use zero API for both cases
            if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_impl<alloc>) {
                return allocator_type::reallocate_zero(p, n);
            }
            else if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_at_least_impl<alloc>) {
                return allocator_type::reallocate_zero_at_least(p, n).ptr;
            }
            else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_impl<alloc>) {
                return allocator_type::reallocate_aligned_zero(p, default_alignment, n);
            }
            else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>) {
                return allocator_type::reallocate_aligned_zero_at_least(p, default_alignment, n).ptr;
            }
            else {
                ::pltxt2htm::details::trap();
            }
        }
    }

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto reallocate(void* p, ::std::size_t n) noexcept -> void*
        requires (has_reallocate)
    {
        if constexpr (::pltxt2htm::memory::details::has_reallocate_impl<alloc>) {
            return allocator_type::reallocate(p, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_at_least_impl<alloc>) {
            return allocator_type::reallocate_at_least(p, n).ptr;
        }
        else {
            return GenericAllocatorAdapter::reallocate_conditional_zero(p, n, false);
        }
    }

    static constexpr bool has_reallocate_zero =
        (::pltxt2htm::memory::details::has_reallocate_zero_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_zero_at_least_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_aligned_zero_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>);

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto reallocate_zero(void* p, ::std::size_t n) noexcept -> void*
        requires (has_reallocate_zero)
    {
        if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_impl<alloc>) {
            return allocator_type::reallocate_zero(p, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_zero_at_least(p, n).ptr;
        }
        else {
            return GenericAllocatorAdapter::reallocate_conditional_zero(p, n, true);
        }
    }

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto reallocate_n_conditional_zero(void* p, ::std::size_t oldn, ::std::size_t n, bool zero) noexcept
        -> void* {
        if (p != nullptr && oldn == n) {
            return p;
        }
        if consteval {
            auto newptr{::operator new(n)};
            if (p != nullptr) {
                if (n) {
                    ::std::size_t const copyn{oldn < n ? oldn : n};
                    ::pltxt2htm::memory::details::nonoverlapped_bytes_copy_n(
                        reinterpret_cast<::std::byte const*>(p), copyn, reinterpret_cast<::std::byte*>(newptr));
                }
                ::operator delete(p);
            }
            if (zero && oldn < n) {
                ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(newptr) + oldn, n - oldn);
            }
            return newptr;
        }
        else {
            if constexpr (::pltxt2htm::memory::details::has_reallocate_n_conditional_zero_impl<alloc>) {
                return allocator_type::reallocate_n_conditional_zero(p, oldn, n, zero);
            }
            else if constexpr (::pltxt2htm::memory::details::has_reallocate_n_conditional_zero_at_least_impl<alloc>) {
                return allocator_type::reallocate_n_conditional_zero_at_least(p, oldn, n, zero).ptr;
            }
            else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_n_conditional_zero_impl<alloc>) {
                return allocator_type::reallocate_aligned_n_conditional_zero(p, oldn, default_alignment, n, zero);
            }
            else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_n_conditional_zero_at_least_impl<
                                   alloc>) {
                return allocator_type::reallocate_aligned_n_conditional_zero_at_least(p, oldn, default_alignment, n,
                                                                                      zero)
                    .ptr;
            }
            else if (zero) {
                if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_n_impl<alloc>) {
                    return allocator_type::reallocate_zero_n(p, oldn, n);
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_n_at_least_impl<alloc>) {
                    return allocator_type::reallocate_zero_n_at_least(p, oldn, n).ptr;
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_impl<alloc>) {
                    return allocator_type::reallocate_aligned_zero_n(p, oldn, default_alignment, n);
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_at_least_impl<alloc>) {
                    return allocator_type::reallocate_aligned_zero_n_at_least(p, oldn, default_alignment, n).ptr;
                }
                else {
                    auto newptr{GenericAllocatorAdapter::reallocate_n(p, oldn, n)};
                    if (oldn < n) {
                        ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(newptr) + oldn,
                                                                    n - oldn);
                    }
                    return newptr;
                }
            }
            else {
                if constexpr (::pltxt2htm::memory::details::has_reallocate_n_impl<alloc>) {
                    return allocator_type::reallocate_n(p, oldn, n);
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_n_at_least_impl<alloc>) {
                    return allocator_type::reallocate_n_at_least(p, oldn, n).ptr;
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_n_impl<alloc>) {
                    return allocator_type::reallocate_aligned_n(p, oldn, default_alignment, n);
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_n_at_least_impl<alloc>) {
                    return allocator_type::reallocate_aligned_n_at_least(p, oldn, default_alignment, n).ptr;
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_impl<alloc>) {
                    return allocator_type::reallocate_aligned_zero_n(p, oldn, default_alignment, n);
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_at_least_impl<alloc>) {
                    return allocator_type::reallocate_aligned_zero_n_at_least(p, oldn, default_alignment, n).ptr;
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_n_impl<alloc>) {
                    return allocator_type::reallocate_zero_n(p, oldn, n);
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_n_at_least_impl<alloc>) {
                    return allocator_type::reallocate_zero_n_at_least(p, oldn, n).ptr;
                }
                else {
                    auto newptr{GenericAllocatorAdapter::allocate(n)};
                    if (p != nullptr) {
                        if (n) {
                            ::std::size_t const copyn{oldn < n ? oldn : n};
                            ::pltxt2htm::memory::details::nonoverlapped_bytes_copy_n(
                                reinterpret_cast<::std::byte const*>(p), copyn, reinterpret_cast<::std::byte*>(newptr));
                        }
                        GenericAllocatorAdapter::deallocate_n(p, oldn);
                    }
                    return newptr;
                }
            }
        }
    }

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto reallocate_n(void* p, ::std::size_t oldn, ::std::size_t n) noexcept -> void* {
        if (p != nullptr && oldn == n) {
            return p;
        }
        if constexpr (::pltxt2htm::memory::details::has_reallocate_n_impl<alloc>) {
            return allocator_type::reallocate_n(p, oldn, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_n_at_least_impl<alloc>) {
            return allocator_type::reallocate_n_at_least(p, oldn, n).ptr;
        }
        else {
            return GenericAllocatorAdapter::reallocate_n_conditional_zero(p, oldn, n, false);
        }
    }

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto reallocate_zero_n(void* p, ::std::size_t oldn, ::std::size_t n) noexcept -> void* {
        if (p != nullptr && oldn == n) {
            return p;
        }
        if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_n_impl<alloc>) {
            return allocator_type::reallocate_zero_n(p, oldn, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_n_at_least_impl<alloc>) {
            return allocator_type::reallocate_zero_n_at_least(p, oldn, n).ptr;
        }
        else {
            return GenericAllocatorAdapter::reallocate_n_conditional_zero(p, oldn, n, true);
        }
    }

    static constexpr bool has_deallocate = (::pltxt2htm::memory::details::has_deallocate_impl<alloc> ||
                                                   ::pltxt2htm::memory::details::has_deallocate_aligned_impl<alloc>);

    static constexpr void deallocate(void* p) noexcept
        requires (has_deallocate)
    {
        if consteval {
#if defined(__has_builtin) && __has_builtin(__builtin_operator_delete)
            __builtin_operator_delete(p);
#else
            ::operator delete(p);
#endif
        }
        else {
            if constexpr (::pltxt2htm::memory::details::has_deallocate_impl<alloc>) {
                allocator_type::deallocate(p);
            }
            else if constexpr (::pltxt2htm::memory::details::has_deallocate_aligned_impl<alloc>) {
                allocator_type::deallocate_aligned(p, default_alignment);
            }
        }
    }

    static constexpr void deallocate_n(void* p, ::std::size_t n) noexcept {
        if consteval {
#if defined(__has_builtin) && __has_builtin(__builtin_operator_delete)
            __builtin_operator_delete(p);
#else
            ::operator delete(p);
#endif
        }
        else {
            if constexpr (::pltxt2htm::memory::details::has_deallocate_n_impl<alloc>) {
                allocator_type::deallocate_n(p, n);
            }
            else if constexpr (::pltxt2htm::memory::details::has_deallocate_aligned_n_impl<alloc>) {
                allocator_type::deallocate_aligned_n(p, default_alignment, n);
            }
            else if constexpr (::pltxt2htm::memory::details::has_deallocate_impl<alloc>) {
                allocator_type::deallocate(p);
            }
            else if constexpr (::pltxt2htm::memory::details::has_deallocate_aligned_impl<alloc>) {
                allocator_type::deallocate_aligned(p, default_alignment);
            }
            else {
                static_assert(::pltxt2htm::memory::details::has_deallocate_n_impl<alloc>);
            }
        }
    }

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static constexpr auto allocate_aligned_conditional_zero(::std::size_t alignment, ::std::size_t n,
                                                            bool zero) noexcept -> void* {
        if consteval {
            auto p{
#if defined(__has_builtin) && __has_builtin(__builtin_operator_new)
                __builtin_operator_new(n)
#else
                ::operator new(n)
#endif
            };
            if (zero) {
                ::pltxt2htm::memory::details::bytes_clear_n(static_cast<::std::byte*>(p), n);
            }
            return p;
        }
        if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_conditional_zero_impl<alloc>) {
            return allocator_type::allocate_aligned_conditional_zero(alignment, n, zero);
        }
        else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_conditional_zero_at_least_impl<alloc>) {
            return allocator_type::allocate_aligned_conditional_zero_at_least(alignment, n, zero).ptr;
        }
        else if constexpr (::pltxt2htm::memory::details::native_allocate_aligned_has_ops<alloc>) {
            constexpr bool has_none_zero_ops{
                ::pltxt2htm::memory::details::native_allocate_aligned_has_none_zero_ops<alloc>};
            constexpr bool has_zero_ops{::pltxt2htm::memory::details::native_allocate_aligned_has_zero_ops<alloc>};
            if constexpr (!has_none_zero_ops && has_zero_ops) {
                if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_zero_impl<alloc>) {
                    return allocator_type::allocate_aligned_zero(alignment, n);
                }
                else {
                    return allocator_type::allocate_aligned_zero_at_least(alignment, n).ptr;
                }
            }
            else if constexpr (has_none_zero_ops && !has_zero_ops) {
                void* ptr;
                if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_impl<alloc>) {
                    ptr = allocator_type::allocate_aligned(alignment, n);
                }
                else {
                    ptr = allocator_type::allocate_aligned_at_least(alignment, n).ptr;
                }
                if (zero) {
                    ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(ptr), n);
                }
                return ptr;
            }
            else if constexpr (has_none_zero_ops && has_zero_ops) {
                void* ptr;
                if (zero) {
                    if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_zero_impl<alloc>) {
                        ptr = allocator_type::allocate_aligned_zero(alignment, n);
                    }
                    else {
                        ptr = allocator_type::allocate_aligned_at_least(alignment, n).ptr;
                    }
                }
                else {
                    if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_impl<alloc>) {
                        ptr = allocator_type::allocate_aligned_zero(alignment, n);
                    }
                    else {
                        ptr = allocator_type::allocate_aligned_at_least(alignment, n).ptr;
                    }
                }
                return ptr;
            }
            else {
                ::pltxt2htm::details::trap();
            }
        }
        else {
            return ::pltxt2htm::memory::details::allocator_pointer_aligned_impl<alloc>(alignment, n, zero);
        }
    }

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static constexpr auto allocate_aligned(::std::size_t alignment, ::std::size_t n) noexcept -> void* {
        if consteval {
            return
#if defined(__has_builtin) && __has_builtin(__builtin_operator_new)
                __builtin_operator_new(n)
#else
                ::operator new(n)
#endif
                    ;
        }
        if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_impl<alloc>) {
            return allocator_type::allocate_aligned(alignment, n);
        }
        else {
            return GenericAllocatorAdapter::allocate_aligned_conditional_zero(alignment, n, false);
        }
    }

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static constexpr auto allocate_aligned_zero(::std::size_t alignment, ::std::size_t n) noexcept -> void* {
        if consteval {
            return
#if defined(__has_builtin) && __has_builtin(__builtin_operator_new)
                __builtin_operator_new(n)
#else
                ::operator new(n)
#endif
                    ;
        }
        if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_zero_impl<alloc>) {
            return allocator_type::allocate_aligned_zero(alignment, n);
        }
        else {
            return GenericAllocatorAdapter::allocate_aligned_conditional_zero(alignment, n, true);
        }
    }

    static constexpr bool has_native_allocate_at_least{
        ::pltxt2htm::memory::details::has_allocate_at_least_impl<alloc> ||
        ::pltxt2htm::memory::details::has_allocate_aligned_at_least_impl<alloc> ||
        ::pltxt2htm::memory::details::has_allocate_zero_at_least_impl<alloc> ||
        ::pltxt2htm::memory::details::has_allocate_aligned_zero_at_least_impl<alloc>};

    static constexpr auto allocate_at_least(::std::size_t n) noexcept -> AllocationLeastResult {
        if consteval {
            return {.ptr =
#if defined(__has_builtin) && __has_builtin(__builtin_operator_new)
                        __builtin_operator_new(n)
#else
                        ::operator new(n)
#endif
                        ,
                    .count = n};
        }
        else {
            if constexpr (::pltxt2htm::memory::details::has_allocate_at_least_impl<alloc>) {
                return allocator_type::allocate_at_least(n);
            }
            else {
                return {.ptr = GenericAllocatorAdapter::allocate(n), .count = n};
            }
        }
    }

    static constexpr auto allocate_zero_at_least(::std::size_t n) noexcept -> AllocationLeastResult {
        if consteval {
            return {.ptr =
#if defined(__has_builtin) && __has_builtin(__builtin_operator_new)
                        __builtin_operator_new(n)
#else
                        ::operator new(n)
#endif
                        ,
                    .count = n};
        }
        else {
            if constexpr (::pltxt2htm::memory::details::has_allocate_zero_at_least_impl<alloc>) {
                return allocator_type::allocate_zero_at_least(n);
            }
            else {
                return GenericAllocatorAdapter::allocate_conditional_zero_at_least(n, true);
            }
        }
    }

    static constexpr auto allocate_conditional_zero_at_least(::std::size_t n, bool zero) noexcept
        -> AllocationLeastResult {
        if consteval {
            auto p{::operator new(n)};
            if (zero) {
                ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(p), n);
            }
            return {.ptr = p, .count = n};
        }
        else {
            if constexpr (::pltxt2htm::memory::details::has_allocate_conditional_zero_at_least_impl<alloc>) {
                return allocator_type::allocate_conditional_zero_at_least(n, zero);
            }
            if constexpr (::pltxt2htm::memory::details::has_allocate_conditional_zero_impl<alloc>) {
                return {.ptr = allocator_type::allocate_conditional_zero(n, zero), .count = n};
            }
            else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_conditional_zero_at_least_impl<
                                   alloc>) {
                return allocator_type::allocate_aligned_conditional_zero_at_least(default_alignment, n, zero);
            }
            else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_conditional_zero_impl<alloc>) {
                return {.ptr = allocator_type::allocate_aligned_conditional_zero(default_alignment, n, zero),
                        .count = n};
            }
            else {
                constexpr bool has_none_zero_ops{
                    ::pltxt2htm::memory::details::native_allocate_has_none_zero_ops<alloc>};
                constexpr bool has_zero_ops{::pltxt2htm::memory::details::native_allocate_has_zero_ops<alloc>};
                if constexpr (!has_none_zero_ops && !has_zero_ops) {
                    ::pltxt2htm::details::trap();
#if __has_cpp_attribute(unreachable)
                    [[unreachable]];
#endif
                }
                else if constexpr (!has_none_zero_ops && has_zero_ops) {
                    if constexpr (::pltxt2htm::memory::details::has_allocate_zero_at_least_impl<alloc>) {
                        return allocator_type::allocate_zero_at_least(n);
                    }
                    else if constexpr (::pltxt2htm::memory::details::has_allocate_zero_impl<alloc>) {
                        return {.ptr = allocator_type::allocate_zero(n), .count = n};
                    }
                    else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_zero_at_least_impl<alloc>) {
                        return allocator_type::allocate_aligned_zero_at_least(default_alignment, n);
                    }
                    else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_zero_impl<alloc>) {
                        return {.ptr = allocator_type::allocate_aligned_zero(default_alignment, n), .count = n};
                    }
                }
                else if constexpr (has_none_zero_ops && !has_zero_ops) {
                    ::pltxt2htm::memory::AllocationLeastResult res;
                    if constexpr (::pltxt2htm::memory::details::has_allocate_at_least_impl<alloc>) {
                        res = allocator_type::allocate_at_least(n);
                    }
                    else if constexpr (::pltxt2htm::memory::details::has_allocate_impl<alloc>) {
                        res = {allocator_type::allocate(n), n};
                    }
                    else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_at_least_impl<alloc>) {
                        res = allocator_type::allocate_aligned_at_least(default_alignment, n);
                    }
                    else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_impl<alloc>) {
                        res = {allocator_type::allocate_aligned(default_alignment, n), n};
                    }
                    if (zero) {
                        ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(res.ptr), res.count);
                    }
                    return res;
                }
                else {
                    ::pltxt2htm::memory::AllocationLeastResult res;
                    if (zero) {
                        if constexpr (::pltxt2htm::memory::details::has_allocate_at_least_impl<alloc>) {
                            res = allocator_type::allocate_at_least(n);
                        }
                        else if constexpr (::pltxt2htm::memory::details::has_allocate_impl<alloc>) {
                            res = {allocator_type::allocate(n), n};
                        }
                        else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_at_least_impl<alloc>) {
                            res = allocator_type::allocate_aligned_at_least(default_alignment, n);
                        }
                        else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_impl<alloc>) {
                            res = {allocator_type::allocate_aligned(default_alignment, n), n};
                        }
                        else {
                            ::pltxt2htm::details::trap();
                        }
                    }
                    else {
                        if constexpr (::pltxt2htm::memory::details::has_allocate_zero_at_least_impl<alloc>) {
                            res = allocator_type::allocate_zero_at_least(n);
                        }
                        else if constexpr (::pltxt2htm::memory::details::has_allocate_zero_impl<alloc>) {
                            res = {allocator_type::allocate_zero(n), n};
                        }
                        else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_zero_at_least_impl<
                                               alloc>) {
                            res = allocator_type::allocate_aligned_zero_at_least(default_alignment, n);
                        }
                        else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_zero_impl<alloc>) {
                            res = {allocator_type::allocate_aligned_zero(default_alignment, n), n};
                        }
                        else {
                            ::pltxt2htm::details::trap();
                        }
                    }
                    return res;
                }
            }
        }
    }

    static constexpr auto allocate_aligned_at_least(::std::size_t alignment, ::std::size_t n) noexcept
        -> AllocationLeastResult {
        if consteval {
            return GenericAllocatorAdapter::allocate_aligned_conditional_zero_at_least(alignment, n, false);
        }
        else {
            if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_at_least_impl<alloc>) {
                return allocator_type::allocate_aligned_at_least(alignment, n);
            }
            else {
                return GenericAllocatorAdapter::allocate_aligned_conditional_zero_at_least(alignment, n, false);
            }
        }
    }

    static constexpr auto allocate_aligned_zero_at_least(::std::size_t alignment, ::std::size_t n) noexcept
        -> AllocationLeastResult {
        if consteval {
            return GenericAllocatorAdapter::allocate_aligned_conditional_zero_at_least(alignment, n, true);
        }
        else {
            if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_zero_at_least_impl<alloc>) {
                return allocator_type::allocate_aligned_zero_at_least(alignment, n);
            }
            else {
                return GenericAllocatorAdapter::allocate_aligned_conditional_zero_at_least(alignment, n, true);
            }
        }
    }

    static constexpr auto allocate_aligned_conditional_zero_at_least(::std::size_t alignment, ::std::size_t n,
                                                                     bool zero) noexcept -> AllocationLeastResult {
        if consteval {
            auto p{::operator new(n)};
            if (zero) {
                ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(p), n);
            }
            return {.ptr = p, .count = n};
        }
        if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_conditional_zero_at_least_impl<alloc>) {
            return allocator_type::allocate_aligned_conditional_zero_at_least(alignment, n, zero);
        }
        else if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_conditional_zero_impl<alloc>) {
            return {.ptr = allocator_type::allocate_aligned_conditional_zero(alignment, n, zero), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::native_allocate_aligned_has_ops<alloc>) {
            constexpr bool has_none_zero_ops{
                ::pltxt2htm::memory::details::native_allocate_aligned_has_none_zero_ops<alloc>};
            constexpr bool has_zero_ops{::pltxt2htm::memory::details::native_allocate_aligned_has_zero_ops<alloc>};
            if constexpr (!has_none_zero_ops && has_zero_ops) {
                if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_zero_at_least_impl<alloc>) {
                    return allocator_type::allocate_aligned_zero_at_least(alignment, n);
                }
                else {
                    return {.ptr = allocator_type::allocate_aligned_zero(alignment, n), .count = n};
                }
            }
            else if constexpr (has_none_zero_ops && !has_zero_ops) {
                ::pltxt2htm::memory::AllocationLeastResult res;
                if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_at_least_impl<alloc>) {
                    res = allocator_type::allocate_aligned_at_least(alignment, n);
                }
                else {
                    res = {allocator_type::allocate_aligned(alignment, n), n};
                }
                if (zero) {
                    ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(res.ptr), res.count);
                }
                return res;
            }
            else if constexpr (has_none_zero_ops && has_zero_ops) {
                ::pltxt2htm::memory::AllocationLeastResult res;
                if (zero) {
                    if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_zero_at_least_impl<alloc>) {
                        res = allocator_type::allocate_aligned_zero_at_least(alignment, n);
                    }
                    else {
                        res = {allocator_type::allocate_aligned_zero(alignment, n), n};
                    }
                }
                else {
                    if constexpr (::pltxt2htm::memory::details::has_allocate_aligned_at_least_impl<alloc>) {
                        res = allocator_type::allocate_aligned_at_least(alignment, n);
                    }
                    else {
                        res = {allocator_type::allocate_aligned(alignment, n), n};
                    }
                }
                return res;
            }
            else {
                ::pltxt2htm::details::trap();
            }
        }
        else {
            return ::pltxt2htm::memory::details::allocator_pointer_aligned_at_least_impl<alloc>(alignment, n, zero);
        }
    }

    static constexpr bool has_reallocate_aligned =
        (::pltxt2htm::memory::details::has_reallocate_aligned_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_aligned_at_least_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_aligned_zero_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_aligned_conditional_zero_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_aligned_conditional_zero_at_least_impl<alloc>);

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto reallocate_aligned_conditional_zero(void* p, ::std::size_t alignment, ::std::size_t n,
                                                           bool zero) noexcept -> void*
        requires (has_reallocate_aligned)
    {
        if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_conditional_zero_impl<alloc>) {
            return allocator_type::reallocate_aligned_conditional_zero(p, alignment, n, zero);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_conditional_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_conditional_zero_at_least(p, alignment, n, zero).ptr;
        }
        else if (zero) {
            if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_impl<alloc>) {
                return allocator_type::reallocate_aligned_zero(p, alignment, n);
            }
            else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>) {
                return allocator_type::reallocate_aligned_zero_at_least(p, alignment, n).ptr;
            }
            else {
                ::pltxt2htm::details::trap();
            }
        }
        else {
            if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_impl<alloc>) {
                return allocator_type::reallocate_aligned(p, alignment, n);
            }
            else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_at_least_impl<alloc>) {
                return allocator_type::reallocate_aligned_at_least(p, alignment, n).ptr;
            }
            else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_impl<alloc>) {
                return allocator_type::reallocate_aligned_zero(p, alignment, n);
            }
            else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>) {
                return allocator_type::reallocate_aligned_zero_at_least(p, alignment, n).ptr;
            }
            else {
                ::pltxt2htm::details::trap();
            }
        }
    }

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto reallocate_aligned(void* p, ::std::size_t alignment, ::std::size_t n) noexcept -> void*
        requires (has_reallocate_aligned)
    {
        if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_impl<alloc>) {
            return allocator_type::reallocate_aligned(p, alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_at_least(p, alignment, n).ptr;
        }
        else {
            return GenericAllocatorAdapter::reallocate_aligned_conditional_zero(p, alignment, n, false);
        }
    }

    static constexpr bool has_reallocate_aligned_zero =
        (::pltxt2htm::memory::details::has_reallocate_aligned_zero_impl<alloc> ||
         ::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>);
#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto reallocate_aligned_zero(void* p, ::std::size_t alignment, ::std::size_t n) noexcept -> void*
        requires (has_reallocate_aligned_zero)
    {
        if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_impl<alloc>) {
            return allocator_type::reallocate_aligned_zero(p, alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_zero_at_least(p, alignment, n).ptr;
        }
        else {
            return GenericAllocatorAdapter::reallocate_aligned_conditional_zero(p, alignment, n, true);
        }
    }

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto reallocate_aligned_n_conditional_zero(void* p, ::std::size_t oldn, ::std::size_t alignment,
                                                             ::std::size_t n, bool zero) noexcept -> void* {
        if (p != nullptr && oldn == n) {
            return p;
        }
        if consteval {
            auto newptr{::operator new(n)};
            if (p != nullptr) {
                if (n) {
                    ::std::size_t const copyn{oldn < n ? oldn : n};
                    ::pltxt2htm::memory::details::nonoverlapped_bytes_copy_n(
                        reinterpret_cast<::std::byte const*>(p), copyn, reinterpret_cast<::std::byte*>(newptr));
                }
                ::operator delete(p);
            }
            if (zero && oldn < n) {
                ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(newptr) + oldn, n - oldn);
            }
            return newptr;
        }
        else {
            if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_n_conditional_zero_impl<alloc>) {
                return allocator_type::reallocate_aligned_n_conditional_zero(p, oldn, alignment, n, zero);
            }
            else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_n_conditional_zero_at_least_impl<
                                   alloc>) {
                return allocator_type::reallocate_aligned_n_conditional_zero_at_least(p, oldn, alignment, n, zero).ptr;
            }
            else if (zero) {
                if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_impl<alloc>) {
                    return allocator_type::reallocate_aligned_zero_n(p, oldn, alignment, n);
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_at_least_impl<alloc>) {
                    return allocator_type::reallocate_aligned_zero_n_at_least(p, oldn, alignment, n).ptr;
                }
                else {
                    auto newptr{GenericAllocatorAdapter::reallocate_aligned_n(p, oldn, alignment, n)};
                    if (oldn < n) {
                        ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(newptr) + oldn,
                                                                    n - oldn);
                    }
                    return newptr;
                }
            }
            else {
                if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_n_impl<alloc>) {
                    return allocator_type::reallocate_aligned_n(p, oldn, alignment, n);
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_n_at_least_impl<alloc>) {
                    return allocator_type::reallocate_aligned_n_at_least(p, oldn, alignment, n).ptr;
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_impl<alloc>) {
                    return allocator_type::reallocate_aligned(p, alignment, n);
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_at_least_impl<alloc>) {
                    return allocator_type::reallocate_aligned_at_least(p, alignment, n).ptr;
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_impl<alloc>) {
                    return allocator_type::reallocate_aligned_zero_n(p, oldn, alignment, n);
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_at_least_impl<alloc>) {
                    return allocator_type::reallocate_aligned_zero_n_at_least(p, oldn, alignment, n).ptr;
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_impl<alloc>) {
                    return allocator_type::reallocate_aligned_zero(p, alignment, n);
                }
                else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>) {
                    return allocator_type::reallocate_aligned_zero_at_least(p, alignment, n).ptr;
                }
                else {
                    if constexpr (::pltxt2htm::memory::details::has_reallocate_n_impl<alloc> ||
                                  ::pltxt2htm::memory::details::has_reallocate_zero_n_impl<alloc> ||
                                  ::pltxt2htm::memory::details::has_reallocate_impl<alloc> ||
                                  ::pltxt2htm::memory::details::has_reallocate_zero_impl<alloc>) {
                        if (alignment <= default_alignment) {
                            return GenericAllocatorAdapter::reallocate_n(p, oldn, n);
                        }
                    }
                    auto newptr{
                        ::pltxt2htm::memory::details::allocator_pointer_aligned_impl<alloc>(alignment, n, false)};
                    if (p != nullptr) {
                        if (n) {
                            bool const moren{oldn < n};
                            ::std::size_t const copyn{moren ? oldn : n};
                            ::pltxt2htm::memory::details::nonoverlapped_bytes_copy_n(
                                reinterpret_cast<::std::byte const*>(p), copyn, reinterpret_cast<::std::byte*>(newptr));
                        }
                        GenericAllocatorAdapter::deallocate_aligned_n(p, alignment, oldn);
                    }
                    return newptr;
                }
            }
        }
    }

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto reallocate_aligned_n(void* p, ::std::size_t oldn, ::std::size_t alignment,
                                            ::std::size_t n) noexcept -> void* {
        if (p != nullptr && oldn == n) {
            return p;
        }
        if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_n_impl<alloc>) {
            return allocator_type::reallocate_aligned_n(p, oldn, alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_n_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_n_at_least(p, oldn, alignment, n).ptr;
        }
        else {
            return GenericAllocatorAdapter::reallocate_aligned_n_conditional_zero(p, oldn, alignment, n, false);
        }
    }

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto reallocate_aligned_zero_n(void* p, ::std::size_t oldn, ::std::size_t alignment,
                                                 ::std::size_t n) noexcept -> void* {
        if (p != nullptr && oldn == n) {
            return p;
        }
        if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_impl<alloc>) {
            return allocator_type::reallocate_aligned_zero_n(p, oldn, alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_zero_n_at_least(p, oldn, alignment, n).ptr;
        }
        else {
            auto newptr{GenericAllocatorAdapter::reallocate_aligned_n(p, oldn, alignment, n)};
            if (oldn < n) {
                ::std::size_t const to_zero_bytes{static_cast<::std::size_t>(n - oldn)};
                ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(newptr) + oldn,
                                                            to_zero_bytes);
            }
            return newptr;
        }
    }

    static constexpr bool has_native_reallocate_at_least =
        (has_reallocate && (::pltxt2htm::memory::details::has_reallocate_aligned_at_least_impl<alloc> ||
                            ::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>));

    static inline auto reallocate_at_least(void* p, ::std::size_t n) noexcept
        -> ::pltxt2htm::memory::AllocationLeastResult
        requires (has_reallocate)
    {
        if constexpr (::pltxt2htm::memory::details::has_reallocate_at_least_impl<alloc>) {
            return allocator_type::reallocate_at_least(p, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_at_least(p, default_alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_zero_at_least(p, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_zero_at_least(p, default_alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_impl<alloc>) {
            return {.ptr = allocator_type::reallocate(p, n), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_aligned(p, default_alignment, n), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_zero(p, n), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_aligned_zero(p, default_alignment, n), .count = n};
        }
    }

    static constexpr bool has_native_reallocate_zero_at_least =
        (has_reallocate_zero && (::pltxt2htm::memory::details::has_reallocate_zero_at_least_impl<alloc> ||
                                 ::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>));

    static inline auto reallocate_zero_at_least(void* p, ::std::size_t n) noexcept
        -> ::pltxt2htm::memory::AllocationLeastResult
        requires (has_reallocate)
    {
        if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_zero_at_least(p, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_zero_at_least(p, default_alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_zero(p, n), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_impl<alloc>) {
            auto newptr{allocator_type::reallocate_aligned(p, default_alignment, n)};
            ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(newptr), n);
            return {.ptr = newptr, .count = n};
        }
        else {
            auto newres{GenericAllocatorAdapter::reallocate_at_least(p, n)};
            ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(newres.ptr), newres.count);
            return newres;
        }
    }

    static inline auto reallocate_n_at_least(void* p, ::std::size_t oldn, ::std::size_t n) noexcept
        -> ::pltxt2htm::memory::AllocationLeastResult {
        if constexpr (::pltxt2htm::memory::details::has_reallocate_n_at_least_impl<alloc>) {
            return allocator_type::reallocate_n_at_least(p, oldn, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_n_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_n_at_least(p, oldn, default_alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_n_at_least_impl<alloc>) {
            return allocator_type::reallocate_zero_n_at_least(p, oldn, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_zero_n_at_least(p, oldn, default_alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_at_least_impl<alloc>) {
            return allocator_type::reallocate_at_least(p, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_at_least(p, default_alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_zero_at_least(p, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_zero_at_least(p, default_alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_n_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_n(p, oldn, n), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_n_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_aligned_n(p, oldn, default_alignment, n), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_n_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_zero_n(p, oldn, n), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_aligned_zero_n(p, oldn, default_alignment, n), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_impl<alloc>) {
            return {.ptr = allocator_type::reallocate(p, n), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_aligned(p, default_alignment, n), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_zero(p, n), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_aligned_zero(p, default_alignment, n), .count = n};
        }
        else {
            auto newres{GenericAllocatorAdapter::allocate_at_least(n)};
            auto newptr{newres.ptr};
            if (p != nullptr) {
                if (n) {
                    if (oldn < n) {
                        n = oldn;
                    }
                    ::pltxt2htm::memory::details::nonoverlapped_bytes_copy_n(reinterpret_cast<::std::byte const*>(p), n,
                                                                             reinterpret_cast<::std::byte*>(newptr));
                }
                GenericAllocatorAdapter::deallocate_n(p, oldn);
            }
            return newres;
        }
    }

    static inline auto reallocate_zero_n_at_least(void* p, ::std::size_t oldn, ::std::size_t n) noexcept
        -> ::pltxt2htm::memory::AllocationLeastResult {
        if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_n_at_least_impl<alloc>) {
            return allocator_type::reallocate_zero_n_at_least(p, oldn, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_zero_n_at_least(p, oldn, default_alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_zero_at_least(p, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_zero_at_least(p, default_alignment, n);
        }
        else {
            auto newres{GenericAllocatorAdapter::reallocate_n_at_least(p, oldn, n)};
            auto newptr{newres.ptr};
            n = newres.count;
            if (oldn < n) {
                ::std::size_t const to_zero_bytes{static_cast<::std::size_t>(n - oldn)};
                ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(newptr) + oldn,
                                                            to_zero_bytes);
            }
            return newres;
        }
    }

    static constexpr bool has_native_reallocate_aligned_at_least =
        (has_reallocate_aligned && (::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc> ||
                                    ::pltxt2htm::memory::details::has_reallocate_aligned_at_least_impl<alloc>));

    static inline auto reallocate_aligned_at_least(void* p, ::std::size_t alignment, ::std::size_t n) noexcept
        -> ::pltxt2htm::memory::AllocationLeastResult
        requires (has_reallocate_aligned_zero)
    {
        if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_at_least(p, alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_zero_at_least(p, alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_aligned(p, alignment, n), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_aligned_zero(p, alignment, n), .count = n};
        }
    }

    static constexpr bool has_native_reallocate_aligned_zero_at_least =
        (has_reallocate_aligned_zero && ::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>);

    static inline auto reallocate_aligned_zero_at_least(void* p, ::std::size_t alignment, ::std::size_t n) noexcept
        -> ::pltxt2htm::memory::AllocationLeastResult
        requires (has_reallocate_aligned_zero)
    {
        if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_zero_at_least(p, alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_aligned_zero(p, alignment, n), .count = n};
        }
    }

    static constexpr bool has_native_reallocate_aligned_n_at_least =
        (has_reallocate_aligned && (::pltxt2htm::memory::details::has_reallocate_aligned_n_at_least_impl<alloc> ||
                                    ::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_at_least_impl<alloc>));

    static inline auto reallocate_aligned_n_at_least(void* p, ::std::size_t oldn, ::std::size_t alignment,
                                                     ::std::size_t n) noexcept
        -> ::pltxt2htm::memory::AllocationLeastResult {
        if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_n_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_n_at_least(p, oldn, alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_at_least(p, alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_zero_n_at_least(p, oldn, alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_zero_at_least(p, alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_aligned_n(p, oldn, alignment, n), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_aligned(p, alignment, n), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_aligned_zero_n(p, oldn, alignment, n), .count = n};
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_aligned_zero(p, alignment, n), .count = n};
        }
        else {
            if constexpr (::pltxt2htm::memory::details::has_reallocate_at_least_impl<alloc> ||
                          ::pltxt2htm::memory::details::has_reallocate_zero_at_least_impl<alloc> ||
                          ::pltxt2htm::memory::details::has_reallocate_n_at_least_impl<alloc> ||
                          ::pltxt2htm::memory::details::has_reallocate_zero_n_at_least_impl<alloc>) {
                if (alignment <= default_alignment) {
                    return GenericAllocatorAdapter::reallocate_n_at_least(p, oldn, n);
                }
            }
            auto newres{
                ::pltxt2htm::memory::details::allocator_pointer_aligned_at_least_impl<alloc>(alignment, n, false)};
            auto newptr{newres.ptr};
            if (p != nullptr) {
                if (n) {
                    if (oldn < n) {
                        n = oldn;
                    }
                    ::pltxt2htm::memory::details::nonoverlapped_bytes_copy_n(reinterpret_cast<::std::byte const*>(p), n,
                                                                             reinterpret_cast<::std::byte*>(newptr));
                }
                GenericAllocatorAdapter::deallocate_aligned_n(p, alignment, oldn);
            }
            return newres;
        }
    }

    static constexpr bool has_native_reallocate_aligned_zero_n_at_least =
        (has_reallocate_aligned_zero && ::pltxt2htm::memory::details::has_reallocate_aligned_zero_at_least_impl<alloc>);

    static inline auto reallocate_aligned_zero_n_at_least(void* p, ::std::size_t oldn, ::std::size_t alignment,
                                                          ::std::size_t n) noexcept
        -> ::pltxt2htm::memory::AllocationLeastResult {
        if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_at_least_impl<alloc>) {
            return allocator_type::reallocate_aligned_zero_n_at_least(p, oldn, alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_reallocate_aligned_zero_n_impl<alloc>) {
            return {.ptr = allocator_type::reallocate_aligned_zero_n(p, oldn, alignment, n), .count = n};
        }
        else {
            auto newres = GenericAllocatorAdapter::reallocate_aligned_n_at_least(p, oldn, alignment, n);
            auto newptr{newres.ptr};
            n = newres.count;
            if (oldn < n) {
                ::std::size_t const to_zero_bytes{static_cast<::std::size_t>(n - oldn)};
                ::pltxt2htm::memory::details::bytes_clear_n(reinterpret_cast<::std::byte*>(newptr) + oldn,
                                                            to_zero_bytes);
            }
            return newres;
        }
    }

    static constexpr bool has_deallocate_aligned =
        (::pltxt2htm::memory::details::has_deallocate_aligned_impl<alloc> ||
         ::pltxt2htm::memory::details::has_deallocate_impl<alloc>);

    static inline void deallocate_aligned(void* p, ::std::size_t alignment) noexcept
        requires (has_deallocate_aligned)
    {
        if constexpr (::pltxt2htm::memory::details::has_deallocate_aligned_impl<alloc>) {
            allocator_type::deallocate_aligned(p, alignment);
        }
        else {
            if (p == nullptr) {
                return;
            }
            if (default_alignment < alignment) {
                p = reinterpret_cast<void**>(p)[-1];
            }
            allocator_type::deallocate(p);
        }
    }

    static inline void deallocate_aligned_n(void* p, ::std::size_t alignment, ::std::size_t n) noexcept {
        if constexpr (::pltxt2htm::memory::details::has_deallocate_aligned_n_impl<alloc>) {
            allocator_type::deallocate_aligned_n(p, alignment, n);
        }
        else if constexpr (::pltxt2htm::memory::details::has_deallocate_aligned_impl<alloc>) {
            allocator_type::deallocate_aligned(p, alignment);
        }
        else {
            if (p == nullptr) {
                return;
            }
            if (default_alignment < alignment) {
                auto start{reinterpret_cast<void**>(p)[-1]};
                n += static_cast<::std::size_t>(reinterpret_cast<char unsigned*>(p) -
                                                reinterpret_cast<char unsigned*>(start));
                p = start;
            }
            if constexpr (::pltxt2htm::memory::details::has_deallocate_impl<alloc>) {
                allocator_type::deallocate(p);
            }
            else {
                allocator_type::deallocate_n(p, n);
            }
        }
    }


};

template<typename alloc, typename T>
class TypedGenericAllocatorAdapter {
public:
    using allocator_adaptor = alloc;
#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static constexpr auto allocate(::std::size_t n) noexcept -> T* {
        if consteval {
            return ::std::allocator<T>{}.allocate(n);
        }
        constexpr ::std::size_t mxn{::std::numeric_limits<::std::size_t>::max() / sizeof(T)};
        if (n > mxn) {
            ::pltxt2htm::details::trap();
        }
        if constexpr (alignof(T) <= alloc::default_alignment) {
            return static_cast<T*>(alloc::allocate(n * sizeof(T)));
        }
        else {
            return static_cast<T*>(alloc::allocate_aligned(alignof(T), n * sizeof(T)));
        }
    }

    static constexpr

        auto
        allocate_at_least(::std::size_t n) noexcept -> details::BasicAllocationLeastResult<T*> {
        if consteval {
            return {.ptr = ::std::allocator<T>{}.allocate(n), .count = n};
        }
        constexpr ::std::size_t mxn{::std::numeric_limits<::std::size_t>::max() / sizeof(T)};
        if (n > mxn) {
            ::pltxt2htm::details::trap();
        }
        if constexpr (alignof(T) <= alloc::default_alignment) {
            auto newres{alloc::allocate_at_least(n * sizeof(T))};
            return {.ptr = reinterpret_cast<T*>(newres.ptr), .count = newres.count / sizeof(T)};
        }
        else {
            auto newres{alloc::allocate_aligned_at_least(alignof(T), n * sizeof(T))};
            return {.ptr = reinterpret_cast<T*>(newres.ptr), .count = newres.count / sizeof(T)};
        }
    }

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static constexpr

    auto allocate_zero(::std::size_t n) noexcept -> T* {
        constexpr ::std::size_t mxn{::std::numeric_limits<::std::size_t>::max() / sizeof(T)};
        if (n > mxn) {
            ::pltxt2htm::details::trap();
        }
        if constexpr (alignof(T) <= alloc::default_alignment) {
            return static_cast<T*>(alloc::allocate_zero(n * sizeof(T)));
        }
        else {
            return static_cast<T*>(alloc::allocate_aligned_zero(alignof(T), n * sizeof(T)));
        }
    }

    static constexpr auto allocate_zero_at_least(::std::size_t n) noexcept
        -> details::BasicAllocationLeastResult<T*> {
        if consteval {
            return {.ptr = ::std::allocator<T>{}.allocate(n), .count = n};
        }
        constexpr ::std::size_t mxn{::std::numeric_limits<::std::size_t>::max() / sizeof(T)};
        if (n > mxn) {
            ::pltxt2htm::details::trap();
        }
        if constexpr (alignof(T) <= alloc::default_alignment) {
            auto newres{alloc::allocate_zero_at_least(n * sizeof(T))};
            return {.ptr = reinterpret_cast<T*>(newres.ptr), .count = newres.count / sizeof(T)};
        }
        else {
            auto newres{alloc::allocate_aligned_zero_at_least(alignof(T), n * sizeof(T))};
            return {.ptr = reinterpret_cast<T*>(newres.ptr), .count = newres.count / sizeof(T)};
        }
    }

    static constexpr bool has_reallocate = allocator_adaptor::has_reallocate;

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static constexpr auto reallocate(T* ptr, ::std::size_t n) noexcept -> T*
        requires (has_reallocate)
    {
        constexpr ::std::size_t mxn{::std::numeric_limits<::std::size_t>::max() / sizeof(T)};
        if (n > mxn) {
            ::pltxt2htm::details::trap();
        }
        if constexpr (alignof(T) <= alloc::default_alignment) {
            return static_cast<T*>(alloc::reallocate(ptr, n * sizeof(T)));
        }
        else {
            return static_cast<T*>(alloc::reallocate_aligned(ptr, alignof(T), n * sizeof(T)));
        }
    }

    static constexpr auto reallocate_at_least(T* ptr, ::std::size_t n) noexcept
        -> details::BasicAllocationLeastResult<T*>
        requires (has_reallocate)
    {
        constexpr ::std::size_t mxn{::std::numeric_limits<::std::size_t>::max() / sizeof(T)};
        if (n > mxn) {
            ::pltxt2htm::details::trap();
        }
        if constexpr (alignof(T) <= alloc::default_alignment) {
            auto newres{alloc::reallocate_at_least(ptr, n * sizeof(T))};
            return {.ptr = reinterpret_cast<T*>(newres.ptr), .count = newres.count / sizeof(T)};
        }
        else {
            auto newres{alloc::reallocate_aligned_at_least(ptr, alignof(T), n * sizeof(T))};
            return {.ptr = reinterpret_cast<T*>(newres.ptr), .count = newres.count / sizeof(T)};
        }
    }

    static constexpr bool has_reallocate_zero = allocator_adaptor::has_reallocate_zero;
#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static constexpr auto reallocate_zero(T* ptr, ::std::size_t n) noexcept -> T*
        requires (has_reallocate_zero)
    {
        constexpr ::std::size_t mxn{::std::numeric_limits<::std::size_t>::max() / sizeof(T)};
        if (n > mxn) {
            ::pltxt2htm::details::trap();
        }
        if constexpr (alignof(T) <= alloc::default_alignment) {
            return static_cast<T*>(alloc::reallocate_zero(ptr, n * sizeof(T)));
        }
        else {
            return static_cast<T*>(alloc::reallocate_aligned_zero(ptr, alignof(T), n * sizeof(T)));
        }
    }

    static constexpr auto reallocate_zero_at_least(T* ptr, ::std::size_t n) noexcept
        -> details::BasicAllocationLeastResult<T*>
        requires (has_reallocate_zero)
    {
        constexpr ::std::size_t mxn{::std::numeric_limits<::std::size_t>::max() / sizeof(T)};
        if (n > mxn) {
            ::pltxt2htm::details::trap();
        }
        if constexpr (alignof(T) <= alloc::default_alignment) {
            auto newres{alloc::reallocate_zero_at_least(ptr, n * sizeof(T))};
            return {.ptr = reinterpret_cast<T*>(newres.ptr), .count = newres.count / sizeof(T)};
        }
        else {
            auto newres{alloc::reallocate_aligned_zero_at_least(ptr, alignof(T), n * sizeof(T))};
            return {.ptr = reinterpret_cast<T*>(newres.ptr), .count = newres.count / sizeof(T)};
        }
    }

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static constexpr auto reallocate_n(T* ptr, ::std::size_t oldn, ::std::size_t n) noexcept -> T* {
        constexpr ::std::size_t mxn{::std::numeric_limits<::std::size_t>::max() / sizeof(T)};
        if (n > mxn) {
            ::pltxt2htm::details::trap();
        }
        if constexpr (alignof(T) <= alloc::default_alignment) {
            return static_cast<T*>(alloc::reallocate_n(ptr, oldn * sizeof(T), n * sizeof(T)));
        }
        else {
            return static_cast<T*>(alloc::reallocate_aligned_n(ptr, oldn * sizeof(T), alignof(T), n * sizeof(T)));
        }
    }

    static constexpr auto reallocate_n_at_least(T* ptr, ::std::size_t oldn, ::std::size_t n) noexcept
        -> details::BasicAllocationLeastResult<T*> {
        constexpr ::std::size_t mxn{::std::numeric_limits<::std::size_t>::max() / sizeof(T)};
        if (n > mxn) {
            ::pltxt2htm::details::trap();
        }
        if constexpr (alignof(T) <= alloc::default_alignment) {
            auto newres{alloc::reallocate_n_at_least(ptr, oldn * sizeof(T), n * sizeof(T))};
            return {.ptr = reinterpret_cast<T*>(newres.ptr), .count = newres.count / sizeof(T)};
        }
        else {
            auto newres{alloc::reallocate_aligned_n_at_least(ptr, oldn * sizeof(T), alignof(T), n * sizeof(T))};
            return {.ptr = reinterpret_cast<T*>(newres.ptr), .count = newres.count / sizeof(T)};
        }
    }

#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static constexpr auto reallocate_zero_n(T* ptr, ::std::size_t oldn, ::std::size_t n) noexcept -> T* {
        constexpr ::std::size_t mxn{::std::numeric_limits<::std::size_t>::max() / sizeof(T)};
        if (n > mxn) {
            ::pltxt2htm::details::trap();
        }
        if constexpr (alignof(T) <= alloc::default_alignment) {
            return static_cast<T*>(alloc::reallocate_zero_n(ptr, oldn * sizeof(T), n * sizeof(T)));
        }
        else {
            return static_cast<T*>(alloc::reallocate_aligned_zero_n(ptr, oldn * sizeof(T), alignof(T), n * sizeof(T)));
        }
    }

    static constexpr auto reallocate_zero_n_at_least(T* ptr, ::std::size_t oldn, ::std::size_t n) noexcept
        -> details::BasicAllocationLeastResult<T*> {
        constexpr ::std::size_t mxn{::std::numeric_limits<::std::size_t>::max() / sizeof(T)};
        if (n > mxn) {
            ::pltxt2htm::details::trap();
        }
        if constexpr (alignof(T) <= alloc::default_alignment) {
            auto newres{alloc::reallocate_zero_n_at_least(ptr, oldn * sizeof(T), n * sizeof(T))};
            return {.ptr = reinterpret_cast<T*>(newres.ptr), .count = newres.count / sizeof(T)};
        }
        else {
            auto newres{alloc::reallocate_aligned_zero_n_at_least(ptr, oldn * sizeof(T), alignof(T), n * sizeof(T))};
            return {.ptr = reinterpret_cast<T*>(newres.ptr), .count = newres.count / sizeof(T)};
        }
    }

    static constexpr bool has_deallocate = allocator_adaptor::has_deallocate;

    static constexpr void deallocate(T* ptr) noexcept
        requires (has_deallocate)
    {
        if consteval {
            if (ptr) {
                return ::std::allocator<T>{}.deallocate(ptr, 1);
            }
            return;
        }
        if constexpr (alignof(T) <= alloc::default_alignment) {
            return alloc::deallocate(ptr);
        }
        else {
            return alloc::deallocate_aligned(ptr, alignof(T));
        }
    }

    static constexpr void deallocate_n(T* ptr, ::std::size_t n) noexcept {
        if consteval {
            if (ptr) {
                return ::std::allocator<T>{}.deallocate(ptr, n);
            }
            return;
        }
        if constexpr (alignof(T) <= alloc::default_alignment) {
            alloc::deallocate_n(ptr, n * sizeof(T));
        }
        else {
            alloc::deallocate_aligned_n(ptr, alignof(T), n * sizeof(T));
        }
    }
};

namespace details {

template<typename alloc>
constexpr auto allocator_pointer_aligned_impl(::std::size_t alignment, ::std::size_t n, bool zero) noexcept -> void* {
    static_assert(::pltxt2htm::memory::GenericAllocatorAdapter<alloc>::has_native_allocate);

    constexpr ::std::size_t defaultalignment{::pltxt2htm::memory::details::calculate_default_alignment<alloc>()};
    bool const alignedadjustment{defaultalignment < alignment};
    if (alignedadjustment) {
        n = ::pltxt2htm::memory::details::allocator_compute_aligned_total_size_impl(alignment, n);
    }
    void* p = ::pltxt2htm::memory::GenericAllocatorAdapter<alloc>::allocate_conditional_zero(n, zero);
    if (alignedadjustment) {
        p = ::pltxt2htm::memory::details::allocator_adjust_ptr_to_aligned_impl(p, alignment);
    }
    return p;
}

template<typename alloc>
constexpr auto allocator_pointer_aligned_at_least_impl(::std::size_t alignment, ::std::size_t n, bool zero) noexcept
    -> ::pltxt2htm::memory::AllocationLeastResult {
    static_assert(::pltxt2htm::memory::GenericAllocatorAdapter<alloc>::has_native_allocate);

    constexpr ::std::size_t defaultalignment{::pltxt2htm::memory::details::calculate_default_alignment<alloc>()};
    bool const alignedadjustment{defaultalignment < alignment};
    if (alignedadjustment) {
        n = ::pltxt2htm::memory::details::allocator_compute_aligned_total_size_impl(alignment, n);
    }
    ::pltxt2htm::memory::AllocationLeastResult res =
        ::pltxt2htm::memory::GenericAllocatorAdapter<alloc>::allocate_conditional_zero_at_least(n, zero);
    if (alignedadjustment) {
        auto resptr{res.ptr};
        auto aligned_ptr = ::pltxt2htm::memory::details::allocator_adjust_ptr_to_aligned_impl(resptr, alignment);
        res = {.ptr = aligned_ptr,
               .count = res.count - static_cast<::std::size_t>(reinterpret_cast<char unsigned*>(aligned_ptr) -
                                                               reinterpret_cast<char unsigned*>(resptr))};
    }
    return res;
}

} // namespace details

} // namespace pltxt2htm::memory
