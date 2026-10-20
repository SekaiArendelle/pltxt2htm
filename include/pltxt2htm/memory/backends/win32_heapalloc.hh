/**
 * @file win32_heapalloc.hh
 * @brief Win32 HeapAlloc family allocator, synced from fast_io
 *        `fast_io_core_impl/allocation/win32_heapalloc.h`.
 */

#pragma once

#if (defined(_WIN32) || defined(__CYGWIN__)) && !defined(__WINE__)

    #include <cstddef>
    #include <cstdint>

    #include "msvc_win32_imports.hh"
    #include "nt_rtlheapalloc.hh"

namespace pltxt2htm::memory::win32 {

struct memory_basic_information {
    void* BaseAddress;
    void* AllocationBase;
    ::std::uint_least32_t AllocationProtect;
    #if UINT_LEAST64_MAX <= SIZE_MAX
    ::std::uint_least16_t PartitionId;
    #endif
    ::std::size_t RegionSize;
    ::std::uint_least32_t State;
    ::std::uint_least32_t Protect;
    ::std::uint_least32_t Type;
};

pltxt2htm_dllimport pltxt2htm_gnu_malloc extern void* pltxt2htm_win_stdcall HeapAlloc(void*, ::std::uint_least32_t,
                                                                                      ::std::size_t) noexcept
    pltxt2htm_win_stdcall_name(HeapAlloc, 12);
pltxt2htm_dllimport extern int pltxt2htm_win_stdcall HeapFree(void*, ::std::uint_least32_t, void*) noexcept
    pltxt2htm_win_stdcall_name(HeapFree, 12);
pltxt2htm_dllimport pltxt2htm_gnu_const extern void* pltxt2htm_win_stdcall GetProcessHeap() noexcept
    pltxt2htm_win_stdcall_name(GetProcessHeap, 0);
pltxt2htm_dllimport extern void* pltxt2htm_win_stdcall HeapReAlloc(void*, ::std::uint_least32_t, void*,
                                                                   ::std::size_t) noexcept
    pltxt2htm_win_stdcall_name(HeapReAlloc, 16);
pltxt2htm_dllimport extern ::std::size_t pltxt2htm_win_stdcall HeapSize(void*, ::std::uint_least32_t,
                                                                        void const*) noexcept
    pltxt2htm_win_stdcall_name(HeapSize, 12);

} // namespace pltxt2htm::memory::win32

namespace pltxt2htm::memory::details {

    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
    #endif
inline void* win32_heapalloc_handle_common_impl(void* heaphandle, ::std::size_t n,
                                                ::std::uint_least32_t flag) noexcept {
    if (n == 0) {
        n = 1;
    }
    auto p{::pltxt2htm::memory::win32::HeapAlloc(heaphandle, flag, n)};
    if (p == nullptr) {
        ::pltxt2htm::details::trap();
    }
    return p;
}

    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
    #endif
inline void* win32_heaprealloc_handle_common_impl(void* heaphandle, void* addr, ::std::size_t n,
                                                  ::std::uint_least32_t flag) noexcept {
    if (n == 0) {
        n = 1;
    }
    if (addr == nullptr) [[unlikely]] {
        return win32_heapalloc_handle_common_impl(heaphandle, n, flag);
    }
    auto p{::pltxt2htm::memory::win32::HeapReAlloc(heaphandle, flag, addr, n)};
    if (p == nullptr) {
        ::pltxt2htm::details::trap();
    }
    return p;
}

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
inline void* win32_get_process_heap() noexcept {
    constexpr bool intrinsicssupported{
    #if (defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)) && \
        (defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_AMD64) || defined(__aarch64__) || \
         defined(__arm64ec__) || defined(_M_ARM64) || defined(_M_ARM64EC))
        true
    #endif
    };
    if constexpr (intrinsicssupported) {
        return ::pltxt2htm::memory::win32::nt::rtl_get_process_heap();
    }
    else {
        return ::pltxt2htm::memory::win32::GetProcessHeap();
    }
}

    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
    #endif
inline void* win32_heapalloc_common_impl(::std::size_t n, ::std::uint_least32_t flag) noexcept {
    return win32_heapalloc_handle_common_impl(win32_get_process_heap(), n, flag);
}

    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
    #endif
inline void* win32_heaprealloc_common_impl(void* addr, ::std::size_t n, ::std::uint_least32_t flag) noexcept {
    return win32_heaprealloc_handle_common_impl(win32_get_process_heap(), addr, n, flag);
}

inline ::pltxt2htm::memory::allocation_least_result win32_heapalloc_least_common_impl(
    ::std::size_t n, ::std::uint_least32_t flag) noexcept {
    auto processheap{win32_get_process_heap()};
    auto ptr{win32_heapalloc_handle_common_impl(processheap, n, flag)};
    return {ptr, ::pltxt2htm::memory::win32::HeapSize(processheap, 0, ptr)};
}

inline ::pltxt2htm::memory::allocation_least_result win32_heaprealloc_least_common_impl(
    void* addr, ::std::size_t n, ::std::uint_least32_t flag) noexcept {
    auto processheap{win32_get_process_heap()};
    auto ptr{win32_heaprealloc_handle_common_impl(processheap, addr, n, flag)};
    return {ptr, ::pltxt2htm::memory::win32::HeapSize(processheap, 0, ptr)};
}

} // namespace pltxt2htm::memory::details

namespace pltxt2htm::memory {

class win32_heapalloc_allocator {
public:
    #if __has_cpp_attribute(__gnu__::__malloc__)
    [[__gnu__::__malloc__]]
    #endif
    static inline void* allocate_conditional_zero(::std::size_t n, bool zeroing) noexcept {
        return details::win32_heapalloc_common_impl(n, zeroing ? 0x00000008u : 0u);
    }

    static inline void* reallocate(void* addr, ::std::size_t n) noexcept {
        return details::win32_heaprealloc_common_impl(addr, n, 0u);
    }

    static inline void* reallocate_zero(void* addr, ::std::size_t n) noexcept {
        return details::win32_heaprealloc_common_impl(addr, n, 0x00000008u);
    }

    static inline void* reallocate_conditional_zero(void* addr, ::std::size_t n, bool zeroing) noexcept {
        return details::win32_heaprealloc_common_impl(addr, n, zeroing ? 0x00000008u : 0u);
    }

    static inline void deallocate(void* addr) noexcept {
        if (addr == nullptr) [[unlikely]] {
            return;
        }
        ::pltxt2htm::memory::win32::HeapFree(details::win32_get_process_heap(), 0u, addr);
    }
    #if 0
	static inline allocation_least_result allocate_at_least(::std::size_t n) noexcept
	{
		return details::win32_heapalloc_least_common_impl(n, 0u);
	}
	static inline allocation_least_result allocate_zero_at_least(::std::size_t n) noexcept
	{
		return details::win32_heapalloc_least_common_impl(n, 0x00000008u);
	}
	static inline allocation_least_result reallocate_at_least(void* addr, ::std::size_t n) noexcept
	{
		return details::win32_heaprealloc_least_common_impl(addr, n, 0u);
	}
	static inline allocation_least_result reallocate_zero_at_least(void* addr, ::std::size_t n) noexcept
	{
		return details::win32_heaprealloc_least_common_impl(addr, n, 0x00000008u);
	}
    #endif
};

} // namespace pltxt2htm::memory

    #pragma pop_macro("pltxt2htm_artificial")
    #pragma pop_macro("pltxt2htm_always_inline")
    #pragma pop_macro("pltxt2htm_gnu_const")
    #pragma pop_macro("pltxt2htm_gnu_malloc")
    #pragma pop_macro("pltxt2htm_win_stdcall_name")
    #pragma pop_macro("pltxt2htm_win_stdcall")
    #pragma pop_macro("pltxt2htm_dllimport")

#endif
