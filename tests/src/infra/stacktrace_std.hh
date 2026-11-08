/**
 * @file stacktrace_std.hh
 * @brief Tests for details::StdBackend, the C++23-standard capture bundle.
 * @details The contract tests instantiate only where
 *          `StdBackend::capture::supported` holds; assert shapes that depend on
 *          the standard entry stay expressed through the bundle's aliases, so
 *          this header compiles everywhere.
 */

#pragma once

#include "stacktrace_common.hh"

template<typename Backend>
inline void test_std_stacktrace_backend() noexcept {
    using Entry = typename Backend::entry;
    ::test_backend_capture_contract<Backend>();
    {
        // The standard bundle never hands its handle-rewrapping machinery to
        // callers: entries cannot be built from a bare handle.
        static_assert(!::std::is_constructible_v<Entry, typename Entry::native_handle_type>);
    }
    {
        // The standard entry is not guaranteed constant-evaluable, so keep the
        // empty-frame check at runtime.
        Entry const empty_entry{};
        pltxt2htm_test_assert_true(!empty_entry);
        auto const empty_frame = empty_entry.resolved();
        pltxt2htm_test_assert_true(empty_frame.address == 0 && empty_frame.description.is_empty() &&
                                   empty_frame.source_file.is_empty() && empty_frame.source_line == 0 &&
                                   !empty_frame.text_truncated && empty_frame.displacement == 0);
    }
    {
        // std::stacktrace semantics: entry zero of current(0) is the caller of
        // current(), and skip shifts the trace by exactly one entry; the
        // host-defined naming is asserted on the NT path only.
        using BackendTrace = ::pltxt2htm::stacktrace::details::BasicStacktrace<Backend>;
        auto const from_here = ::trace_from_named_frame<BackendTrace>(0);
        auto const one_skipped = ::trace_from_named_frame<BackendTrace>(1);
        pltxt2htm_test_assert_true(from_here.size() > 1);
        pltxt2htm_test_assert_true(one_skipped.size() + 1 == from_here.size());
    }
    {
        // The std fallback resolves through the standard entry: the saved
        // native handle round-trips, and naming depends on the host resolver,
        // so only the address is asserted here.
        using BackendTrace = ::pltxt2htm::stacktrace::details::BasicStacktrace<Backend>;
        auto const deep = ::deep_trace<BackendTrace>(96);
        pltxt2htm_test_assert_true(deep.size() > 64);
        pltxt2htm_test_assert_true((BackendTrace{} <=> deep) < 0);
        pltxt2htm_test_assert_true(!deep.is_empty() && static_cast<bool>(deep[0]));
        auto const first_frame = deep[0].resolved();
        pltxt2htm_test_assert_true(first_frame.address != 0 &&
                                   first_frame.address == reinterpret_cast<::std::uintptr_t>(deep[0].native_handle()));
    }
}

inline void test_std_stacktrace() noexcept {
    namespace details = ::pltxt2htm::stacktrace::details;
    if constexpr (details::StdBackend::capture::supported) {
        test_std_stacktrace_backend<details::StdBackend>();
    }
}
