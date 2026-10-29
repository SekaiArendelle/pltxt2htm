/**
 * @file capture.hh
 * @brief Native return-address capture for the stacktrace implementation.
 */

#pragma once

#include <cstddef>
#include <limits>
#include <span>

#if defined(_WIN32)
    #include "../../sys/nt/ntdll.hh"
#endif

namespace pltxt2htm::stacktrace::details {
struct CaptureResult {
    ::std::size_t size{};
    bool possibly_truncated{};
};

/**
 * @brief Capture native return addresses into caller-owned storage.
 * @param frames Destination storage; its size limits the returned entries.
 * @param skip Entries to skip in the native trace, including this helper when
 *             it has a physical frame.
 * @return Captured count and whether the capacity limit truncated the trace.
 * @details No exact caller-frame identity is promised under inlining or
 *          tail-call optimization. Not async-signal-safe.
 */
[[nodiscard]]
inline auto capture(::std::span<void*> frames, [[maybe_unused]] ::std::size_t skip = 0) noexcept -> CaptureResult {
    if (frames.empty()) {
        return {};
    }
#if defined(_WIN32)
    if (skip > (::std::numeric_limits<::pltxt2htm::sys_nt::Ulong>::max)()) {
        return {};
    }
    auto const capacity = frames.size() < 65535 ? frames.size() : 65535;
    auto const count = ::pltxt2htm::sys_nt::RtlCaptureStackBackTrace(static_cast<::pltxt2htm::sys_nt::Ulong>(skip),
                                                                     static_cast<::pltxt2htm::sys_nt::Ulong>(capacity),
                                                                     frames.data(), nullptr);
    return {.size = count, .possibly_truncated = count == capacity};
#else
    return {};
#endif
}
} // namespace pltxt2htm::stacktrace::details
