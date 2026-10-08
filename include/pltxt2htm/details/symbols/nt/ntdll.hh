#pragma once

#include <cstdint>

// These declarations deliberately avoid Windows SDK headers. The calling
// convention matters on x86; x64 and ARM64 use the platform convention.
#if defined(_MSC_VER)
    #define PLTXT2HTM_NT_IMPORT __declspec(dllimport)
    #define PLTXT2HTM_NT_CALL __stdcall
#elif defined(__i386__)
    #define PLTXT2HTM_NT_IMPORT __attribute__((dllimport))
    #define PLTXT2HTM_NT_CALL __attribute__((stdcall))
#else
    #define PLTXT2HTM_NT_IMPORT __attribute__((dllimport))
    #define PLTXT2HTM_NT_CALL
#endif

#if defined(__clang__) || defined(__GNUC__)
    #if defined(__i386__) && defined(__clang__)
        #define PLTXT2HTM_NT_SYMBOL(name, size) __asm__("_" #name "@" #size)
    #elif defined(__i386__)
        #define PLTXT2HTM_NT_SYMBOL(name, size) __asm__(#name "@" #size)
    #else
        #define PLTXT2HTM_NT_SYMBOL(name, size) __asm__(#name)
    #endif
#else
    #define PLTXT2HTM_NT_SYMBOL(name, size)
#endif

namespace pltxt2htm::details::symbols::nt {
extern "C" {
[[nodiscard]]
PLTXT2HTM_NT_IMPORT unsigned short PLTXT2HTM_NT_CALL pltxt2htm_nt_capture_stack_back_trace(unsigned long, unsigned long,
                                                                                           void**,
                                                                                           unsigned long*) noexcept
    PLTXT2HTM_NT_SYMBOL(RtlCaptureStackBackTrace, 16);
[[nodiscard]]
PLTXT2HTM_NT_IMPORT unsigned char PLTXT2HTM_NT_CALL pltxt2htm_nt_try_acquire_srw_lock_exclusive(void*) noexcept
    PLTXT2HTM_NT_SYMBOL(RtlTryAcquireSRWLockExclusive, 4);
PLTXT2HTM_NT_IMPORT void PLTXT2HTM_NT_CALL pltxt2htm_nt_acquire_srw_lock_exclusive(void*) noexcept
    PLTXT2HTM_NT_SYMBOL(RtlAcquireSRWLockExclusive, 4);
PLTXT2HTM_NT_IMPORT void PLTXT2HTM_NT_CALL pltxt2htm_nt_release_srw_lock_exclusive(void*) noexcept
    PLTXT2HTM_NT_SYMBOL(RtlReleaseSRWLockExclusive, 4);
[[nodiscard]]
PLTXT2HTM_NT_IMPORT ::std::int32_t PLTXT2HTM_NT_CALL pltxt2htm_nt_duplicate_object(void*, void*, void*, void**,
                                                                                   unsigned long, unsigned long,
                                                                                   unsigned long) noexcept
    PLTXT2HTM_NT_SYMBOL(NtDuplicateObject, 28);
[[nodiscard]]
PLTXT2HTM_NT_IMPORT ::std::int32_t PLTXT2HTM_NT_CALL pltxt2htm_nt_close(void*) noexcept PLTXT2HTM_NT_SYMBOL(NtClose, 4);
}
} // namespace pltxt2htm::details::symbols::nt

#undef PLTXT2HTM_NT_IMPORT
#undef PLTXT2HTM_NT_CALL

#undef PLTXT2HTM_NT_SYMBOL

#if defined(_MSC_VER) && !defined(__clang__)
    #if defined(_M_IX86)
        #pragma comment( \
            linker, \
            "/alternatename:__imp__pltxt2htm_nt_capture_stack_back_trace@16=__imp__RtlCaptureStackBackTrace@16")
        #pragma comment( \
            linker, \
            "/alternatename:__imp__pltxt2htm_nt_try_acquire_srw_lock_exclusive@4=__imp__RtlTryAcquireSRWLockExclusive@4")
        #pragma comment( \
            linker, \
            "/alternatename:__imp__pltxt2htm_nt_acquire_srw_lock_exclusive@4=__imp__RtlAcquireSRWLockExclusive@4")
        #pragma comment( \
            linker, \
            "/alternatename:__imp__pltxt2htm_nt_release_srw_lock_exclusive@4=__imp__RtlReleaseSRWLockExclusive@4")
        #pragma comment(linker, "/alternatename:__imp__pltxt2htm_nt_duplicate_object@28=__imp__NtDuplicateObject@28")
        #pragma comment(linker, "/alternatename:__imp__pltxt2htm_nt_close@4=__imp__NtClose@4")
    #else
        #pragma comment(linker, \
                        "/alternatename:__imp_pltxt2htm_nt_capture_stack_back_trace=__imp_RtlCaptureStackBackTrace")
        #pragma comment( \
            linker, \
            "/alternatename:__imp_pltxt2htm_nt_try_acquire_srw_lock_exclusive=__imp_RtlTryAcquireSRWLockExclusive")
        #pragma comment( \
            linker, "/alternatename:__imp_pltxt2htm_nt_acquire_srw_lock_exclusive=__imp_RtlAcquireSRWLockExclusive")
        #pragma comment( \
            linker, "/alternatename:__imp_pltxt2htm_nt_release_srw_lock_exclusive=__imp_RtlReleaseSRWLockExclusive")
        #pragma comment(linker, "/alternatename:__imp_pltxt2htm_nt_duplicate_object=__imp_NtDuplicateObject")
        #pragma comment(linker, "/alternatename:__imp_pltxt2htm_nt_close=__imp_NtClose")
    #endif
#endif
