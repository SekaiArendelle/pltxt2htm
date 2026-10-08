#pragma once

#include <cstddef>
#include <limits>
#include <span>

#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(_WIN32)
    #include "../symbols/nt/ntdll.hh"
#elif defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(__linux__) && __has_include(<execinfo.h>)
    #include <execinfo.h>
    #include <cstdlib>
#endif

namespace pltxt2htm::details::stacktrace {
struct CaptureResult {
    ::std::size_t size{};
    bool possibly_truncated{};
};

/** Capture native return addresses into caller-owned storage.
 * skip counts entries in the captured native trace, including this helper when
 * it has a physical frame. No exact caller-frame identity is promised under
 * inlining/tail-call optimization. Capacity limits returned entries after skip.
 * Linux uses temporary storage when skip is nonzero; allocation failure returns
 * an empty result. Native unwinders may also allocate. Not async-signal-safe.
 */
[[nodiscard]]
inline CaptureResult capture(::std::span<void*> frames, ::std::size_t skip = 0) noexcept {
    if (frames.empty()) {
        return {};
    }
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(_WIN32)
    if (skip > (::std::numeric_limits<unsigned long>::max)()) {
        return {};
    }
    auto const capacity = frames.size() < 65535 ? frames.size() : 65535;
    auto const count = ::pltxt2htm::details::symbols::nt::pltxt2htm_nt_capture_stack_back_trace(
        static_cast<unsigned long>(skip), static_cast<unsigned long>(capacity), frames.data(), nullptr);
    return {.size = count, .possibly_truncated = count == capacity};
#elif defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(__linux__) && __has_include(<execinfo.h>)
    auto const max_capacity = static_cast< ::std::size_t>((::std::numeric_limits<int>::max)());
    auto const capacity = frames.size() < max_capacity ? frames.size() : max_capacity;
    if (skip > max_capacity - capacity) {
        return {};
    }
    auto const requested = capacity + skip;
    if (requested > (::std::numeric_limits< ::std::size_t>::max)() / sizeof(void*)) {
        return {};
    }
    void** buffer = frames.data();
    if (skip != 0) {
        buffer = static_cast<void**>(::std::malloc(requested * sizeof(void*)));
        if (buffer == nullptr) {
            return {};
        }
    }
    auto const count = ::backtrace(buffer, static_cast<int>(requested));
    auto const size = count > 0 ? static_cast< ::std::size_t>(count) : 0;
    auto const kept = size > skip ? size - skip : 0;
    if (skip != 0) {
        for (::std::size_t i = 0; i < kept; ++i) {
            frames[i] = buffer[i + skip];
        }
        ::std::free(buffer);
    }
    return {.size = kept, .possibly_truncated = size == requested};
#else
    (void)skip;
    return {};
#endif
}
} // namespace pltxt2htm::details::stacktrace
