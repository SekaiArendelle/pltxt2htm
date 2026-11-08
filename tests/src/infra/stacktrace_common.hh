/**
 * @file stacktrace_common.hh
 * @brief Shared helper layer for the stacktrace test headers.
 * @details Each backend is tested by instantiating `details::BasicStacktrace`
 *          and its backend bundle directly (see `stacktrace_none.hh`,
 *          `stacktrace_nt.hh`, `stacktrace_std.hh`); this header supplies the
 *          tooling those tests share.
 */

#pragma once

#include <concepts>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <limits>
#include <memory>
#include <string_view>
#include <type_traits>
#include <utility>

#include <fast_io/fast_io_dsal/string.h>

#include "precompile.hh"
#include <pltxt2htm/stacktrace/stacktrace.hh>

/// Counts each allocation serviced by the native global allocator.
struct TrackingAllocator {
    static inline ::std::size_t allocations{};
    static inline ::std::size_t outstanding{};

    [[nodiscard]] static void* allocate(::std::size_t bytes) noexcept {
        ++allocations;
        ++outstanding;
        return ::fast_io::native_global_allocator::allocate(bytes);
    }

    static void deallocate(void* pointer) noexcept {
        if (pointer != nullptr) {
            --outstanding;
        }
        ::fast_io::native_global_allocator::deallocate(pointer);
    }
};

template<typename Trace>
[[nodiscard]]
#if defined(_MSC_VER)
__declspec(noinline)
#elif defined(__GNUC__)
[[gnu::noinline]]
#endif
auto deep_trace(::std::size_t depth) noexcept -> Trace {
    if (depth == 0) {
        return Trace::current();
    }
    auto result = ::deep_trace<Trace>(depth - 1);
    // Keep work after recursion so optimized builds cannot use a tail call.
    if (result.is_empty()) {
        return {};
    }
    return result;
}

template<typename Trace>
[[nodiscard]]
#if defined(_MSC_VER)
__declspec(noinline)
#elif defined(__GNUC__)
[[gnu::noinline]]
#endif
auto trace_from_named_frame(::std::size_t skip) noexcept -> Trace {
    return Trace::current(skip);
}

/// Frame storage that the capture backends append to.
template<typename Entry>
using TraceFrames =
    ::pltxt2htm::container::Vector<Entry, ::fast_io::generic_allocator_adapter<::fast_io::native_global_allocator>>;

/// Appends the capture taken from a shared noinline call site, so a test can
/// pair a zero-skip capture with a one-skip capture from the same frame.
template<typename CaptureBackend, typename Entry>
#if defined(_MSC_VER)
__declspec(noinline)
#elif defined(__GNUC__)
[[gnu::noinline]]
#endif
void capture_from_shared_site(TraceFrames<Entry>& frames, ::std::size_t skip) noexcept {
    CaptureBackend::template capture<::fast_io::native_global_allocator>(frames, skip);
}

// Only the capture backend may rewrite a saved handle; entries must not hand
// their backing storage to ordinary callers.
template<typename Entry>
concept can_access_entry_storage = requires(Entry& entry) { entry.native_handle_storage(); };

static_assert(!can_access_entry_storage<::pltxt2htm::stacktrace::StacktraceEntry>);

/// Backend contract tests shared by every backend bundle.
template<typename Backend>
inline void test_backend_capture_contract() noexcept {
    using Entry = typename Backend::entry;
    using NativeHandle = typename Entry::native_handle_type;
    using Capture = typename Backend::capture;
    if constexpr (Capture::supported) {
        static_assert(Capture::max_depth >= 1, "supported capture backends need a positive max_depth");
        static_assert(!can_access_entry_storage<Entry>);
        TraceFrames<Entry> frames{};
        Capture::template capture<::fast_io::native_global_allocator>(frames,
                                                                      (::std::numeric_limits<::std::size_t>::max)());
        pltxt2htm_test_assert_true(frames.is_empty());
        Capture::template capture<::fast_io::native_global_allocator>(frames, 0);
        pltxt2htm_test_assert_true(!frames.is_empty());
        for (auto const& entry : frames) {
            pltxt2htm_test_assert_true(entry.native_handle() != NativeHandle{});
        }
        // skip shifts the capture by one frame from the same call site.
        TraceFrames<Entry> unskipped{};
        TraceFrames<Entry> skipped{};
        ::capture_from_shared_site<Capture, Entry>(unskipped, 0);
        ::capture_from_shared_site<Capture, Entry>(skipped, 1);
        pltxt2htm_test_assert_true(unskipped.size() > 1);
        pltxt2htm_test_assert_true(skipped[0].native_handle() == unskipped[1].native_handle());
        // The front-end of this backend stores exactly what the backend reported.
        using BackendTrace = ::pltxt2htm::stacktrace::details::BasicStacktrace<Backend>;
        auto const grown = BackendTrace::current(0);
        pltxt2htm_test_assert_true(!grown.is_empty());
        for (auto const& entry : grown) {
            pltxt2htm_test_assert_true(entry.native_handle() != NativeHandle{});
        }
        // Skips wider than the stack or over the backend limit report no frames.
        pltxt2htm_test_assert_true(BackendTrace::current((::std::numeric_limits<::std::size_t>::max)()).is_empty());
        pltxt2htm_test_assert_true(BackendTrace::current(100000).is_empty());
    }
    else {
        static_assert(Capture::max_depth == 0, "unsupported capture backends advertise no capacity");
        TraceFrames<Entry> frames{};
        Capture::template capture<::fast_io::native_global_allocator>(frames, 0);
        pltxt2htm_test_assert_true(frames.is_empty());
    }
}
