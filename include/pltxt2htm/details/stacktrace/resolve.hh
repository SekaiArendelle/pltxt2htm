#pragma once

#include "entry.hh"
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(_WIN32)
    #include "windows.hh"
#endif

namespace pltxt2htm::details::stacktrace {
/** Resolve an already captured address without unwinding or printing.
 * PLTXT2HTM_ENABLE_STACKTRACE enables Windows DbgHelp resolution and benefits
 * from matching PDBs.
 * Resolution may allocate, load libraries,
 * and perform file I/O, so it is not suitable for signal/loader callbacks or
 * reliable diagnostics after heap corruption. Failure retains the address.
 */
[[nodiscard]]
inline ResolvedFrame resolve(void* address) noexcept {
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(_WIN32)
    return ::pltxt2htm::details::stacktrace::own_frame(
        ::pltxt2htm::details::stacktrace::resolve_native(address));
#else
    return {.address = address};
#endif
}
} // namespace pltxt2htm::details::stacktrace
