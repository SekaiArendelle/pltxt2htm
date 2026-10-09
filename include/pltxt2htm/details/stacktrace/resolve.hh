#pragma once

#include "entry.hh"
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(_WIN32)
    #include "windows.hh"
#elif defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(__linux__) && __has_include(<execinfo.h>)
    #include "linux.hh"
#endif

namespace pltxt2htm::details::stacktrace {
/** Resolve an already captured address without unwinding or printing.
 * PLTXT2HTM_ENABLE_STACKTRACE enables all available native resolution. Windows
 * loads DbgHelp from System32 and benefits from matching PDBs; Linux optionally
 * loads libdw.so.1 to resolve DWARF source locations and local symbols. Without
 * libdw or debug information, Linux retains libc symbols and the module path.
 * Resolution may allocate, load libraries,
 * and perform file I/O, so it is not suitable for signal/loader callbacks or
 * reliable diagnostics after heap corruption. Failure retains the address.
 */
[[nodiscard]]
inline ResolvedFrame resolve(void* address) noexcept {
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && (defined(_WIN32) || (defined(__linux__) && __has_include(<execinfo.h>)))
    return ::pltxt2htm::details::stacktrace::resolve_native(address);
#else
    return {.address = address};
#endif
}
} // namespace pltxt2htm::details::stacktrace
