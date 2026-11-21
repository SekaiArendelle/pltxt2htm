/**
 * @file capture.hh
 * @brief Capture backend built on the C++23 std::stacktrace fallback.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include "stacktrace_entry.hh"
#include "../../../container/impl/vector_decl.hh"

// windows.h defines min and max as function-like macros, which would expand
// inside std::numeric_limits<...>::max(). Keep them undefined in this header.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace pltxt2htm::stacktrace::details {

struct StdCaptureBackend {
    static constexpr bool supported{true};
    static constexpr ::std::size_t max_depth{::std::numeric_limits<::std::size_t>::max()};

    /**
     * @brief Append every captured entry to frames.
     * @param frames Entry storage to append to.
     * @param skip Native frames to skip, starting with this function's frame.
     * @details std::stacktrace::current(skip) skips `skip` entries starting
     *          with the caller of current(); as this function is that caller,
     *          the count the shared front-end passes already covers the capture
     *          machinery. A skip beyond the stack reports no frames. Some host
     *          implementations truncate too-wide skips into valid small values
     *          and report the whole stack instead; since a skip over any real
     *          stack always reports no frames anyway, wider than the signed
     *          32-bit range is treated as unrepresentable and reports no
     *          frames, as for the NT backend.
     */
    template<typename Allocator>
#if __has_cpp_attribute(__gnu__::__noinline__)
    [[__gnu__::__noinline__]]
#elif defined(_MSC_VER)
    __declspec(noinline)
#endif
    static void capture(
        ::pltxt2htm::container::Vector<StdStacktraceEntry, ::fast_io::generic_allocator_adapter<Allocator>>& frames,
        ::std::size_t skip) noexcept {
        // A skip that no real stack can cover is either outside the host
        // counters' signed 32-bit range or, on 32-bit targets, truncated by
        // them into small offsets; report no frames in both cases.
        constexpr auto max_skip = ::std::numeric_limits<::std::int32_t>::max();
        if (skip > max_skip) {
            return;
        }
        auto const base = frames.size();
        auto const captured = ::std::stacktrace::current(skip);
        auto const captured_size = captured.size();
        frames.resize(base + captured_size);
        for (::std::size_t i = 0; i < captured_size; ++i) {
            frames[base + i] = StdStacktraceEntry{captured[i]};
        }
    }
};

} // namespace pltxt2htm::stacktrace::details

#pragma pop_macro("max")
#pragma pop_macro("min")
