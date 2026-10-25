/**
 * @file nt_rtlheapalloc.hh
 * @brief NT heap (RtlAllocateHeap family) allocator, synced from fast_io
 *        `fast_io_core_impl/allocation/nt_rtlheapalloc.h`; the NT symbols
 *        (RtlAllocateHeap, RtlGetCurrentPeb, Peb, ...) come from the generated
 *        declarations in pltxt2htm::sys_nt.
 * @details The TEB register-access machinery is dropped (project style forbids
 *          `register ... __asm__`), falling back to `RtlGetCurrentPeb()`
 *          instead. The whole result lives in `pltxt2htm::memory::details`:
 *          the backend is only composed by `NativeGlobalAllocator` and is
 *          not public API.
 */

#pragma once

#if defined(_WIN32) && !defined(__WINE__)

    #include <cstddef>
    #include <cstdint>

    #if defined(_MSC_VER)
        #include <intrin.h>
    #endif

    #if defined(_MSC_VER) && !defined(_KERNEL_MODE) && !defined(_WIN32_WINDOWS)
        #pragma comment(lib, "ntdll.lib")
    #endif

    #include "../../sys/nt/ntdll.hh"

namespace pltxt2htm::memory::details {

    #if __has_cpp_attribute(__gnu__::__always_inline__)
[[__gnu__::__always_inline__]]
    #elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
    #endif
    #if __has_cpp_attribute(__gnu__::__artificial__)
[[__gnu__::__artificial__]]
    #endif
    #if __has_cpp_attribute(__gnu__::__const__)
[[__gnu__::__const__]]
    #endif
    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
    #endif
[[nodiscard]]
inline auto nt_get_current_peb() noexcept -> ::pltxt2htm::sys_nt::Peb* {
    #if defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_AMD64)
        #if defined(__GNUC__) || defined(__clang__)
    if constexpr (sizeof(::std::size_t) == sizeof(::std::uint_least64_t)) {
        ::pltxt2htm::sys_nt::Peb* ppeb;
        __asm__("{movq\t%%gs:0x60, %0|mov\t%0, %%gs:[0x60]}" : "=r"(ppeb));
        return ppeb;
    }
    else {
        ::pltxt2htm::sys_nt::Peb* ppeb;
        __asm__("{movl\t%%fs:0x30, %0|mov\t%0, %%fs:[0x30]}" : "=r"(ppeb));
        return ppeb;
    }
        #elif defined(_MSC_VER)
            #if defined(_M_AMD64)
    return reinterpret_cast< ::pltxt2htm::sys_nt::Peb*>(__readgsqword(0x60));
            #else
    return reinterpret_cast< ::pltxt2htm::sys_nt::Peb*>(__readfsdword(0x30));
            #endif
        #else
    return ::pltxt2htm::sys_nt::RtlGetCurrentPeb();
        #endif
    #else
    return ::pltxt2htm::sys_nt::RtlGetCurrentPeb();
    #endif
}

    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
    #endif
[[nodiscard]]
inline auto nt_get_process_heap() noexcept -> void* {
    return nt_get_current_peb()->process_heap;
}

    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
    #endif
inline auto nt_rtlallocate_heap_handle_common_impl(void* heaphandle, ::std::size_t n,
                                                    ::std::uint_least32_t flag) noexcept -> void* {
    if (n == 0) {
        n = 1;
    }
    auto p{::pltxt2htm::sys_nt::RtlAllocateHeap(heaphandle, flag, n)};
    if (p == nullptr) [[unlikely]] {
        ::pltxt2htm::details::trap();
    }
    return p;
}

    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
    #endif
inline auto nt_rtlreallocate_heap_handle_common_impl(void* heaphandle, void* addr, ::std::size_t n,
                                                      ::std::uint_least32_t flag) noexcept -> void* {
    if (n == 0) {
        n = 1;
    }
    if (addr == nullptr) [[unlikely]] {
        return nt_rtlallocate_heap_handle_common_impl(heaphandle, n, flag);
    }
    auto p{::pltxt2htm::sys_nt::RtlReAllocateHeap(heaphandle, flag, addr, n)};
    if (p == nullptr) {
        ::pltxt2htm::details::trap();
    }
    return p;
}

    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
    #endif
inline auto nt_rtlallocate_heap_common_impl(::std::size_t n, ::std::uint_least32_t flag) noexcept -> void* {
    return nt_rtlallocate_heap_handle_common_impl(nt_get_process_heap(), n, flag);
}

    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
    #endif
inline auto nt_rtlreallocate_heap_common_impl(void* addr, ::std::size_t n, ::std::uint_least32_t flag) noexcept -> void* {
    return nt_rtlreallocate_heap_handle_common_impl(nt_get_process_heap(), addr, n, flag);
}

inline auto nt_rtlallocate_heap_least_common_impl(
    ::std::size_t n, ::std::uint_least32_t flag) noexcept -> ::pltxt2htm::memory::AllocationLeastResult {
    auto processheap{nt_get_process_heap()};
    auto ptr{nt_rtlallocate_heap_handle_common_impl(processheap, n, flag)};
    return {.ptr = ptr, .count = ::pltxt2htm::sys_nt::RtlSizeHeap(processheap, 0, ptr)};
}

inline auto nt_rtlreallocate_heap_least_common_impl(
    void* addr, ::std::size_t n, ::std::uint_least32_t flag) noexcept -> ::pltxt2htm::memory::AllocationLeastResult {
    auto processheap{nt_get_process_heap()};
    auto ptr{nt_rtlreallocate_heap_handle_common_impl(processheap, addr, n, flag)};
    return {.ptr = ptr, .count = ::pltxt2htm::sys_nt::RtlSizeHeap(processheap, 0, ptr)};
}

class NtRtlAllocateHeapAllocator {
public:
    #if __has_cpp_attribute(__gnu__::__malloc__)
    [[__gnu__::__malloc__]]
    #endif
    static inline auto allocate_conditional_zero(::std::size_t n, bool zero) noexcept -> void* {
        return details::nt_rtlallocate_heap_common_impl(n, zero ? 0x00000008u : 0u);
    }

    static inline auto reallocate(void* addr, ::std::size_t n) noexcept -> void* {
        return details::nt_rtlreallocate_heap_common_impl(addr, n, 0u);
    }

    static inline auto reallocate_zero(void* addr, ::std::size_t n) noexcept -> void* {
        return details::nt_rtlreallocate_heap_common_impl(addr, n, 0x00000008u);
    }

    static inline void deallocate(void* addr) noexcept {
        if (addr == nullptr) [[unlikely]] {
            return;
        }
        static_cast<void>(::pltxt2htm::sys_nt::RtlFreeHeap(details::nt_get_process_heap(), 0u, addr));
    }
    #if 0
	static inline auto allocate_at_least(::std::size_t n) noexcept -> AllocationLeastResult
	{
		return details::nt_rtlallocate_heap_least_common_impl(n, 0u);
	}
	static inline auto allocate_zero_at_least(::std::size_t n) noexcept -> AllocationLeastResult
	{
		return details::nt_rtlallocate_heap_least_common_impl(n, 0x00000008u);
	}
	static inline auto reallocate_at_least(void* addr, ::std::size_t n) noexcept -> AllocationLeastResult
	{
		return details::nt_rtlreallocate_heap_least_common_impl(addr, n, 0u);
	}
	static inline auto reallocate_zero_at_least(void* addr, ::std::size_t n) noexcept -> AllocationLeastResult
	{
		return details::nt_rtlreallocate_heap_least_common_impl(addr, n, 0x00000008u);
	}
    #endif
};

} // namespace pltxt2htm::memory::details

#endif
