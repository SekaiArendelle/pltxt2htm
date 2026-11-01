/**
 * @file capture.hh
 * @brief Platform-independent native return-address capture.
 * @details Selects the capture backend for the current platform and grows
 *          storage until the backend stops filling the probe buffer or the
 *          backend limit is reached. A backend provides `supported`,
 *          `max_depth`, and `capture(::std::span<void*>, ::std::size_t)`, which
 *          writes up to `frames.size()` addresses and returns how many it wrote;
 *          a filled buffer is treated as possibly truncated. A backend must
 *          return 0 for a skip it cannot represent.
 */

#pragma once

#include <cstddef>
#include <span>
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
 * @brief Captured native addresses and whether the backend limit cut them off.
 */
template<typename Allocator>
struct CapturedFrames {
    ::pltxt2htm::container::Vector<void*, ::fast_io::generic_allocator_adapter<Allocator>> addresses{};
    bool possibly_truncated{};
};

/**
 * @brief Capture the complete native stack into growable storage.
 * @param skip Native frames to skip, starting with the backend capture frame.
 * @return Captured addresses and whether the backend limit truncated them;
 *         empty when the platform has no backend or skip is unrepresentable.
 * @details Grows the buffer by doubling until the backend writes fewer
 *          addresses than the buffer holds or the backend limit is reached.
 */
template<typename Allocator>
[[nodiscard]]
#if __has_cpp_attribute(__gnu__::__noinline__)
[[__gnu__::__noinline__]]
#elif defined(_MSC_VER)
__declspec(noinline)
#endif
auto capture_all(::std::size_t skip) noexcept -> CapturedFrames<Allocator> {
    if constexpr (!CaptureBackend::supported) {
        return {};
    }
    else {
        CapturedFrames<Allocator> frames{};
        auto depth = CaptureBackend::max_depth < 64 ? CaptureBackend::max_depth : 64;
        while (true) {
            frames.addresses.resize(depth);
            auto const written = CaptureBackend::capture({frames.addresses.data(), depth}, skip);
            frames.addresses.resize(written);
            frames.possibly_truncated = written == depth;
            if (!frames.possibly_truncated || depth == CaptureBackend::max_depth) {
                return frames;
            }
            depth = depth > CaptureBackend::max_depth / 2 ? CaptureBackend::max_depth : depth * 2;
        }
    }
}
} // namespace pltxt2htm::stacktrace::details
