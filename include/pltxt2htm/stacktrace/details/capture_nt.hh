/**
 * @file capture_nt.hh
 * @brief Windows capture backend using RtlCaptureStackBackTrace.
 */

#pragma once

#include <cstddef>
#include <limits>
#include <span>
#include "capture_common.hh"
#include "../../sys/nt/ntdll.hh"

namespace pltxt2htm::stacktrace::details {
struct capture_backend {
    static constexpr bool supported = true;
    static constexpr ::std::size_t max_skip = (::std::numeric_limits<::pltxt2htm::sys_nt::Ulong>::max)();
    static constexpr ::std::size_t max_depth = 65535;

    [[nodiscard]]
    static auto capture(::std::span<void*> frames, ::std::size_t skip) noexcept -> CaptureResult {
        if (frames.empty() || skip > max_skip) {
            return {};
        }
        auto const capacity = frames.size() < max_depth ? frames.size() : max_depth;
        auto const count = ::pltxt2htm::sys_nt::RtlCaptureStackBackTrace(
            static_cast<::pltxt2htm::sys_nt::Ulong>(skip), static_cast<::pltxt2htm::sys_nt::Ulong>(capacity),
            frames.data(), nullptr);
        return {.size = count, .possibly_truncated = count == capacity};
    }
};
} // namespace pltxt2htm::stacktrace::details
