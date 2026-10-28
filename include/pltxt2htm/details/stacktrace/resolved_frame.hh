#pragma once

#include <cstdint>
#include "../../container/string.hh"
#include "native_frame.hh"
#if defined(_WIN32)
    #include "windows.hh"
#endif

namespace pltxt2htm::details::stacktrace {
/** Owning symbol data exposed by the pltxt2htm stacktrace API. */
struct ResolvedFrame {
    void* address{};
    ::pltxt2htm::container::String description{};
    ::pltxt2htm::container::String source_file{};
    ::std::uint64_t displacement{};
    unsigned long source_line{};
    bool text_truncated{};
};

[[nodiscard]]
constexpr auto own_frame(NativeResolvedFrame const& frame) noexcept -> ResolvedFrame {
    return {.address = frame.address,
            .description = ::pltxt2htm::container::String{frame.description.data(),
                                                          frame.description.data() + frame.description.size()},
            .source_file = ::pltxt2htm::container::String{frame.source_file.data(),
                                                          frame.source_file.data() + frame.source_file.size()},
            .displacement = frame.displacement,
            .source_line = frame.source_line,
            .text_truncated = frame.text_truncated};
}

/** Resolve an already captured address without unwinding or printing.
 * Windows DbgHelp resolution benefits from matching PDBs. Resolution may
 * allocate, load libraries, and perform file I/O, so it is not suitable for
 * signal/loader callbacks or reliable diagnostics after heap corruption.
 * Failure retains the address.
 */
[[nodiscard]]
inline auto resolve(void* address) noexcept -> ResolvedFrame {
#if defined(_WIN32)
    return ::pltxt2htm::details::stacktrace::own_frame(::pltxt2htm::details::stacktrace::resolve_native(address));
#else
    return {.address = address};
#endif
}
} // namespace pltxt2htm::details::stacktrace
