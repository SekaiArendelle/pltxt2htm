/**
 * @file resolve.hh
 * @brief Symbol resolution for an already captured native address.
 */

#pragma once

#include "resolved_frame.hh"
#if defined(_WIN32)
    #include "windows.hh"
#endif

namespace pltxt2htm::stacktrace {
/** Resolve an already captured address without unwinding or printing.
 * Windows DbgHelp resolution benefits from matching PDBs. Resolution may
 * allocate, load libraries, and perform file I/O, so it is not suitable for
 * signal/loader callbacks or reliable diagnostics after heap corruption.
 * Failure retains the address.
 */
[[nodiscard]]
inline auto resolve(void* address) noexcept -> ResolvedFrame {
#if defined(_WIN32)
    return ::pltxt2htm::stacktrace::resolve_native(address);
#else
    return {.address = address};
#endif
}
} // namespace pltxt2htm::stacktrace
