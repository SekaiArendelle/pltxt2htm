/**
 * @file capture.hh
 * @brief Capture backend for platforms without a supported unwinder.
 */

#pragma once

#include <cstddef>
#include <span>

namespace pltxt2htm::stacktrace::details {

struct CaptureBackend {
    static constexpr bool supported = false;
    static constexpr ::std::size_t max_depth = 0;

    /**
     * @brief Unused backend entry point, kept so the shared front-end compiles.
     */
    [[nodiscard]]
    static auto capture(::std::span<StacktraceEntry::native_handle_type>, ::std::size_t) noexcept -> ::std::size_t {
        return 0;
    }
};

} // namespace pltxt2htm::stacktrace::details
