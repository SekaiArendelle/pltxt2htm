/**
 * @file nt_rtlheapalloc.hh
 * @brief NT heap (RtlAllocateHeap family) allocator, synced from fast_io
 *        `fast_io_core_impl/allocation/nt_preliminary_definition.h` and
 *        `nt_rtlheapalloc.h`.
 * @details Only the PEB (up to `ProcessHeap`) is ported; the TEB register-access
 *          machinery is dropped (project style forbids `register ... __asm__`),
 *          falling back to `RtlGetCurrentPeb()` instead.
 */

#pragma once

#if (defined(_WIN32) || defined(__CYGWIN__)) && !defined(__WINE__)

    #include <cstddef>
    #include <cstdint>

    #if defined(_MSC_VER)
        #include <intrin.h>
    #endif

    #if defined(_MSC_VER) && !defined(_KERNEL_MODE) && !defined(_WIN32_WINDOWS)
        #pragma comment(lib, "ntdll.lib")
    #endif

    #include "msvc_win32_imports.hh"

namespace pltxt2htm::memory::win32::nt {

struct peb_ldr_data;
struct rtl_user_process_parameters;
using pps_post_process_init_routine = void(pltxt2htm_win_stdcall*)(void) noexcept;

struct peb {
    char unsigned InheritedAddressSpace;
    char unsigned ReadImageFileExecOptions;
    char unsigned BeingDebugged;
    char unsigned SpareBool;
    void* Mutant;
    void* ImageBaseAddress;
    peb_ldr_data* Ldr;
    rtl_user_process_parameters* ProcessParameters; // PRTL_USER_PROCESS_PARAMETERS
    void* SubSystemData;
    void* ProcessHeap;
};

pltxt2htm_dllimport pltxt2htm_gnu_malloc void* pltxt2htm_win_stdcall RtlAllocateHeap(void*, ::std::uint_least32_t,
                                                                                     ::std::size_t) noexcept
    pltxt2htm_win_stdcall_name(RtlAllocateHeap, 12);
pltxt2htm_dllimport char unsigned pltxt2htm_win_stdcall RtlFreeHeap(void*, ::std::uint_least32_t, void*) noexcept
    pltxt2htm_win_stdcall_name(RtlFreeHeap, 12);
pltxt2htm_dllimport peb* pltxt2htm_win_stdcall RtlGetCurrentPeb() noexcept
    pltxt2htm_win_stdcall_name(RtlGetCurrentPeb, 0);
pltxt2htm_dllimport void* pltxt2htm_win_stdcall RtlReAllocateHeap(void*, ::std::uint_least32_t, void*,
                                                                  ::std::size_t) noexcept
    pltxt2htm_win_stdcall_name(RtlReAllocateHeap, 16);
pltxt2htm_dllimport::std::size_t pltxt2htm_win_stdcall RtlSizeHeap(void*, ::std::uint_least32_t, void*) noexcept
    pltxt2htm_win_stdcall_name(RtlSizeHeap, 12);

    #if defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_AMD64)
        #define PLTXT2HTM_MEMORY_NT_PEB_INTRINSIC 1
    #else
        #define PLTXT2HTM_MEMORY_NT_PEB_INTRINSIC 0
    #endif

[[nodiscard]] pltxt2htm_always_inline_artificial_const inline peb* nt_get_current_peb() noexcept {
    #if PLTXT2HTM_MEMORY_NT_PEB_INTRINSIC
        #if (defined(__GNUC__) || defined(__clang__))
    if constexpr (sizeof(::std::size_t) == sizeof(::std::uint_least64_t)) {
        peb* ppeb;
        __asm__("{movq\t%%gs:0x60, %0|mov\t%0, %%gs:[0x60]}" : "=r"(ppeb));
        return ppeb;
    }
    else {
        peb* ppeb;
        __asm__("{movl\t%%fs:0x30, %0|mov\t%0, %%fs:[0x30]}" : "=r"(ppeb));
        return ppeb;
    }
        #elif defined(_MSC_VER)
            #if defined(_M_AMD64)
    return reinterpret_cast<peb*>(__readgsqword(0x60));
            #else
    return reinterpret_cast<peb*>(__readfsdword(0x30));
            #endif
        #else
    return RtlGetCurrentPeb();
        #endif
    #else
    return RtlGetCurrentPeb();
    #endif
}

    #undef PLTXT2HTM_MEMORY_NT_PEB_INTRINSIC

[[nodiscard]] pltxt2htm_always_inline_artificial_const inline void* rtl_get_process_heap() noexcept {
    return nt_get_current_peb()->ProcessHeap;
}

} // namespace pltxt2htm::memory::win32::nt

namespace pltxt2htm::memory::details {

    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
    #endif
inline void* nt_rtlallocate_heap_handle_common_impl(void* heaphandle, ::std::size_t n,
                                                    ::std::uint_least32_t flag) noexcept {
    if (n == 0) {
        n = 1;
    }
    auto p{::pltxt2htm::memory::win32::nt::RtlAllocateHeap(heaphandle, flag, n)};
    if (p == nullptr) [[unlikely]] {
        ::pltxt2htm::details::trap();
    }
    return p;
}

    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
    #endif
inline void* nt_rtlreallocate_heap_handle_common_impl(void* heaphandle, void* addr, ::std::size_t n,
                                                      ::std::uint_least32_t flag) noexcept {
    if (n == 0) {
        n = 1;
    }
    if (addr == nullptr) [[unlikely]] {
        return nt_rtlallocate_heap_handle_common_impl(heaphandle, n, flag);
    }
    auto p{::pltxt2htm::memory::win32::nt::RtlReAllocateHeap(heaphandle, flag, addr, n)};
    if (p == nullptr) {
        ::pltxt2htm::details::trap();
    }
    return p;
}

    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
    #endif
inline void* nt_rtlallocate_heap_common_impl(::std::size_t n, ::std::uint_least32_t flag) noexcept {
    return nt_rtlallocate_heap_handle_common_impl(::pltxt2htm::memory::win32::nt::rtl_get_process_heap(), n, flag);
}

    #if __has_cpp_attribute(__gnu__::__returns_nonnull__)
[[__gnu__::__returns_nonnull__]]
    #endif
inline void* nt_rtlreallocate_heap_common_impl(void* addr, ::std::size_t n, ::std::uint_least32_t flag) noexcept {
    return nt_rtlreallocate_heap_handle_common_impl(::pltxt2htm::memory::win32::nt::rtl_get_process_heap(), addr, n,
                                                    flag);
}

inline ::pltxt2htm::memory::allocation_least_result nt_rtlallocate_heap_least_common_impl(
    ::std::size_t n, ::std::uint_least32_t flag) noexcept {
    auto processheap{::pltxt2htm::memory::win32::nt::rtl_get_process_heap()};
    auto ptr{nt_rtlallocate_heap_handle_common_impl(processheap, n, flag)};
    return {ptr, ::pltxt2htm::memory::win32::nt::RtlSizeHeap(processheap, 0, ptr)};
}

inline ::pltxt2htm::memory::allocation_least_result nt_rtlreallocate_heap_least_common_impl(
    void* addr, ::std::size_t n, ::std::uint_least32_t flag) noexcept {
    auto processheap{::pltxt2htm::memory::win32::nt::rtl_get_process_heap()};
    auto ptr{nt_rtlreallocate_heap_handle_common_impl(processheap, addr, n, flag)};
    return {ptr, ::pltxt2htm::memory::win32::nt::RtlSizeHeap(processheap, 0, ptr)};
}

} // namespace pltxt2htm::memory::details

namespace pltxt2htm::memory {

class nt_rtlallocateheap_allocator {
public:
    #if __has_cpp_attribute(__gnu__::__malloc__)
    [[__gnu__::__malloc__]]
    #endif
    static inline void* allocate_conditional_zero(::std::size_t n, bool zero) noexcept {
        return details::nt_rtlallocate_heap_common_impl(n, zero ? 0x00000008u : 0u);
    }

    static inline void* reallocate(void* addr, ::std::size_t n) noexcept {
        return details::nt_rtlreallocate_heap_common_impl(addr, n, 0u);
    }

    static inline void* reallocate_zero(void* addr, ::std::size_t n) noexcept {
        return details::nt_rtlreallocate_heap_common_impl(addr, n, 0x00000008u);
    }

    static inline void deallocate(void* addr) noexcept {
        if (addr == nullptr) [[unlikely]] {
            return;
        }
        ::pltxt2htm::memory::win32::nt::RtlFreeHeap(::pltxt2htm::memory::win32::nt::rtl_get_process_heap(), 0u, addr);
    }
    #if 0
	static inline allocation_least_result allocate_at_least(::std::size_t n) noexcept
	{
		return details::nt_rtlallocate_heap_least_common_impl(n, 0u);
	}
	static inline allocation_least_result allocate_zero_at_least(::std::size_t n) noexcept
	{
		return details::nt_rtlallocate_heap_least_common_impl(n, 0x00000008u);
	}
	static inline allocation_least_result reallocate_at_least(void* addr, ::std::size_t n) noexcept
	{
		return details::nt_rtlreallocate_heap_least_common_impl(addr, n, 0u);
	}
	static inline allocation_least_result reallocate_zero_at_least(void* addr, ::std::size_t n) noexcept
	{
		return details::nt_rtlreallocate_heap_least_common_impl(addr, n, 0x00000008u);
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
