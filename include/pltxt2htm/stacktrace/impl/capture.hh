/**
 * @file capture.hh
 * @brief Platform-independent native return-address capture.
 * @details Selects the capture backend for the current platform and grows
 *          storage until the stack is complete or the backend limit is reached.
 */

#pragma once

#include <cstddef>
#include <span>
#include "capture_common.hh"
#include "../../container/impl/vector_decl.hh"

#if defined(_WIN32)
    #include "capture_nt.hh"
#else
    #include "capture_null.hh"
#endif

namespace pltxt2htm::stacktrace::details {
static_assert(!CaptureBackend::supported || CaptureBackend::max_depth >= 1,
              "supported capture backends need a positive max_depth");

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
inline auto capture(::std::span<void*> frames, ::std::size_t skip = 0) noexcept -> CaptureResult {
    return CaptureBackend::capture(frames, skip);
}

template<typename Allocator>
struct CapturedFrames {
    ::pltxt2htm::container::Vector<void*, ::fast_io::generic_allocator_adapter<Allocator>> addresses{};
    CaptureResult result{};
};

/**
 * @brief Capture the complete native stack into growable storage.
 * @param skip Native frames to skip, including the capture machinery's own
 *             frames.
 * @return Captured addresses and the capture result; empty when the platform
 *         has no backend or skip exceeds the backend's maximum.
 * @details Grows the buffer by doubling until the backend reports that the
 *          stack is complete or the backend limit is reached.
 */
template<typename Allocator>
[[nodiscard]]
auto capture_all(::std::size_t skip) noexcept -> CapturedFrames<Allocator> {
    if constexpr (!CaptureBackend::supported) {
        return {};
    }
    else {
        if (skip > CaptureBackend::max_skip) {
            return {};
        }
        CapturedFrames<Allocator> frames{};
        auto depth = CaptureBackend::max_depth < 64 ? CaptureBackend::max_depth : 64;
        for (;;) {
            frames.addresses.reserve(depth);
            while (frames.addresses.size() < depth) {
                frames.addresses.emplace_back(nullptr);
            }
            frames.result = CaptureBackend::capture({frames.addresses.data(), depth}, skip);
            if (!frames.result.possibly_truncated || depth == CaptureBackend::max_depth) {
                break;
            }
            depth = depth > CaptureBackend::max_depth / 2 ? CaptureBackend::max_depth : depth * 2;
        }
        return frames;
    }
}
} // namespace pltxt2htm::stacktrace::details
