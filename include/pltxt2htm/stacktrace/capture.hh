/**
 * @file capture.hh
 * @brief Native return-address capture for the stacktrace implementation.
 */

#pragma once

#include <cstddef>
#include <limits>
#include <span>

#if defined(_WIN32)
    #include "../sys/nt/ntdll.hh"
#endif

namespace pltxt2htm::stacktrace::details {
struct CaptureResult {
    ::std::size_t size{};
    bool possibly_truncated{};
};

/** Capture native return addresses into caller-owned storage.
 * skip counts entries in the captured native trace, including this helper when
 * it has a physical frame. No exact caller-frame identity is promised under
 * inlining/tail-call optimization. Capacity limits returned entries after skip.
 * Not async-signal-safe.
 */
[[nodiscard]]
inline auto capture(::std::span<void*> frames, ::std::size_t skip = 0) noexcept -> CaptureResult {
    if (frames.empty()) {
        return {};
    }
#if defined(_WIN32)
    if (skip > (::std::numeric_limits<unsigned long>::max)()) {
        return {};
    }
    auto const capacity = frames.size() < 65535 ? frames.size() : 65535;
    auto const count = ::pltxt2htm::sys_nt::RtlCaptureStackBackTrace(
        static_cast<unsigned long>(skip), static_cast<unsigned long>(capacity), frames.data(), nullptr);
    return {.size = count, .possibly_truncated = count == capacity};
#else
    (void)skip;
    return {};
#endif
}
} // namespace pltxt2htm::stacktrace::details
