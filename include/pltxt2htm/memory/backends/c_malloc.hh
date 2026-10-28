/**
 * @file c_malloc.hh
 * @brief C malloc/calloc/realloc/free based allocator, synced from fast_io
 *        `fast_io_core_impl/allocation/c_malloc.h`. Used as the fallback backend
 *        and as the backend under AddressSanitizer.
 * @details The whole result lives in `pltxt2htm::memory::details`: the backend
 *          is only composed by `NativeGlobalAllocator` and is not public API.
 */

#pragma once

#if __has_include(<malloc.h>)
    #include <malloc.h>
#elif __has_include(<malloc_np.h>)
    #include <malloc_np.h>
#endif
#include <cstdlib>

#include "../details/allocation_least_result.hh"
#include "../../details/trap.hh"

namespace pltxt2htm::memory::details {

#if (__has_include(<malloc.h>) || __has_include(<malloc_np.h>)) && !defined(__MSDOS__) && !defined(__LLVM_LIBC__)

inline auto c_malloc_usable_size_impl(void* p) noexcept -> ::std::size_t {
    #if defined(_WIN32) && !defined(__WINE__) && !defined(__CYGWIN__)
    return ::_msize(p);
    #else
    return ::malloc_usable_size(p);
    #endif
}

#endif // (__has_include(<malloc.h>) || __has_include(<malloc_np.h>)) && !defined(__MSDOS__) && !defined(__LLVM_LIBC__)

class CMallocAllocator {
public:
#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto allocate(::std::size_t n) noexcept -> void* {
        if (n == 0) {
            n = 1;
        }
        void* p =
#if defined(__has_builtin) && __has_builtin(__builtin_malloc)
            __builtin_malloc(n)
#else
            ::std::malloc(n)
#endif
            ;
        if (p == nullptr) {
            ::pltxt2htm::details::trap();
        }
        return p;
    }
#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto reallocate(void* p, ::std::size_t n) noexcept -> void* {
        if (n == 0) {
            n = 1;
        }
        ::std::size_t const to_allocate{n};
        p =
#if defined(__has_builtin) && __has_builtin(__builtin_realloc)
            __builtin_realloc
#else
            ::std::realloc
#endif

            (p, to_allocate);
        if (p == nullptr) {
            ::pltxt2htm::details::trap();
        }
        return p;
    }
#if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
#endif
    static inline auto allocate_zero(::std::size_t n) noexcept -> void* {
        if (n == 0) {
            n = 1;
        }
        void* p =
#if defined(__has_builtin) && __has_builtin(__builtin_calloc)
            __builtin_calloc
#else
            ::std::calloc
#endif

            (1, n);
        if (p == nullptr) {
            ::pltxt2htm::details::trap();
        }
        return p;
    }
#if (__has_include(<malloc.h>) || __has_include(<malloc_np.h>)) && !defined(__MSDOS__) && !defined(__LLVM_LIBC__)
    static inline auto allocate_at_least(::std::size_t n) noexcept -> AllocationLeastResult {
        auto p{CMallocAllocator::allocate(n)};
        return {.ptr = p, .count = details::c_malloc_usable_size_impl(p)};
    }

    static inline auto allocate_zero_at_least(::std::size_t n) noexcept -> AllocationLeastResult {
        auto p{CMallocAllocator::allocate_zero(n)};
        return {.ptr = p, .count = details::c_malloc_usable_size_impl(p)};
    }

    static inline auto reallocate_at_least(void* oldp, ::std::size_t n) noexcept -> AllocationLeastResult {
        auto p{CMallocAllocator::reallocate(oldp, n)};
        return {.ptr = p, .count = details::c_malloc_usable_size_impl(p)};
    }
#endif

#if defined(_WIN32) && !defined(__WINE__) && !defined(__CYGWIN__)
    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
    #endif
    static inline auto allocate_aligned(::std::size_t alignment, ::std::size_t n) noexcept -> void* {
        if (n == 0) {
            n = 1;
        }
        void* p;
        if (alignment <= __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
            p =
    #if defined(__has_builtin) && __has_builtin(__builtin_malloc)
                __builtin_malloc
    #else
                ::std::malloc
    #endif
                (n);
        }
        else {
            p = ::_aligned_malloc(n, alignment);
        }
        if (p == nullptr) {
            ::pltxt2htm::details::trap();
        }
        return p;
    }
    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
    [[__gnu__::__returns_nonnull__]]
    #endif
    static inline auto reallocate_aligned(void* p, ::std::size_t alignment, ::std::size_t n) noexcept -> void* {
        if (n == 0) {
            n = 1;
        }
        if (alignment <= __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
            p =
    #if defined(__has_builtin) && __has_builtin(__builtin_realloc)
                __builtin_realloc
    #else
                ::std::realloc
    #endif

                (p, n);
        }
        else {
            p = ::_aligned_realloc(p, n, alignment);
        }
        if (p == nullptr) {
            ::pltxt2htm::details::trap();
        }
        return p;
    }

    static inline void deallocate_aligned(void* p, ::std::size_t alignment) noexcept {
        if (p == nullptr) {
            return;
        }
        if (alignment <= __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
    #if defined(__has_builtin) && __has_builtin(__builtin_free)
            __builtin_free
    #else
            ::std::free
    #endif
                (p);
        }
        else {
            ::_aligned_free(p);
        }
    }
#endif
    static inline void deallocate(void* p) noexcept {
        if (p == nullptr) {
            return;
        }

#if defined(__has_builtin) && __has_builtin(__builtin_free)
        __builtin_free
#else
        ::std::free
#endif

            (p);
    }
};

} // namespace pltxt2htm::memory::details
