/**
 * @file capture_null.hh
 * @brief Capture backend for platforms without a supported unwinder.
 */

#pragma once

#include <cstddef>
#include <span>
#include "capture_common.hh"

namespace pltxt2htm::stacktrace::details {
struct capture_backend {
    static constexpr bool supported = false;
    static constexpr ::std::size_t max_skip = 0;
    static constexpr ::std::size_t max_depth = 0;

    [[nodiscard]]
    static auto capture(::std::span<void*>, ::std::size_t) noexcept -> CaptureResult {
        return {};
    }
};
} // namespace pltxt2htm::stacktrace::details
