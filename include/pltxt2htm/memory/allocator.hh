/**
 * @file allocator.hh
 * @brief Entry point for `pltxt2htm::memory` — the vendored fast_io allocator
 *        stack (generic/typed adapters, detection concepts, Win32/NT and C
 *        malloc backends) under the pltxt2htm namespace.
 * @details Synced from fast_io `fast_io_core_impl/allocation/impl.h`. mimalloc,
 *          the Linux-kernel allocator, MSVC debug-CRT (`_DEBUG`) allocators,
 *          and the user-customization hooks (`custom.h`) are intentionally not
 *          ported; freestanding toolchains are rejected at compile time.
 */

#pragma once

#include <type_traits>

#include "backends/c_malloc.hh"
#if (defined(_WIN32) || defined(__CYGWIN__)) && !defined(__WINE__)
    #include "backends/nt_rtlheapalloc.hh"
    #include "backends/win32_heapalloc.hh"
#endif
#include "details/allocation_least_result.hh"
#include "details/allocator_adapter.hh"
#include "details/allocator_concepts.hh"
#include "details/asan_status.hh"

#if ((__STDC_HOSTED__ == 1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED == 1) && !defined(_LIBCPP_FREESTANDING)))
#else
    #error "pltxt2htm::memory requires a hosted toolchain"
#endif

namespace pltxt2htm::memory {

using native_global_allocator = generic_allocator_adapter<
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__WINE__) && !defined(PLTXT2HTM_MEMORY_USE_C_MALLOC)
    ::std::conditional_t<::pltxt2htm::memory::asan::asan_status::current ==
                             ::pltxt2htm::memory::asan::asan_status::none,
    #ifdef _WIN32_WINDOWS
                         win32_heapalloc_allocator
    #else
                         nt_rtlallocateheap_allocator
    #endif
                         ,
                         c_malloc_allocator>
#else
    c_malloc_allocator
#endif
    >;

template<typename T>
using native_typed_global_allocator = typed_generic_allocator_adapter<native_global_allocator, T>;

using native_thread_local_allocator = generic_allocator_adapter<native_global_allocator>;

template<typename T>
using native_typed_thread_local_allocator = typed_generic_allocator_adapter<native_thread_local_allocator, T>;

} // namespace pltxt2htm::memory
