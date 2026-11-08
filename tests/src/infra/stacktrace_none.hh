/**
 * @file stacktrace_none.hh
 * @brief Tests for details::NoneBackend, the no-resolver fallback bundle.
 * @details The fallback exists on every platform and is always testable,
 *          including where the public front-end selects another backend.
 */

#pragma once

#include "stacktrace_common.hh"

inline void test_none_stacktrace() noexcept {
    namespace details = ::pltxt2htm::stacktrace::details;
    static_assert(!can_access_entry_storage<details::NoneBackend::entry>);
    ::test_backend_capture_contract<details::NoneBackend>();
    {
        // The fallback entry keeps the address and produces no text.
        constexpr details::NoneBackend::entry fallback{};
        static_assert(::std::same_as<details::NoneBackend::entry::native_handle_type, ::std::uintptr_t>);
        static_assert(!fallback);
        auto const empty_frame = fallback.resolved();
        pltxt2htm_test_assert_true(empty_frame.address == 0 && empty_frame.description.is_empty() &&
                                   empty_frame.source_file.is_empty() && empty_frame.source_line == 0);
        // Entry construction from native addresses resolves to an address-only frame.
        int object{};
        auto const handle = reinterpret_cast<details::NoneBackend::entry::native_handle_type>(::std::addressof(object));
        auto const entry = details::NoneBackend::entry::from_native_handle(handle);
        pltxt2htm_test_assert_true(entry.native_handle() == handle);
        auto const frame = entry.resolved();
        pltxt2htm_test_assert_true(frame.address == handle && frame.description.is_empty() &&
                                   frame.source_file.is_empty() && frame.source_line == 0);
    }
    {
        // Its backend is a declared no-op, so an existing entry is left untouched.
        int object{};
        auto const handle = reinterpret_cast<details::NoneBackend::entry::native_handle_type>(::std::addressof(object));
        auto const entry = details::NoneBackend::entry::from_native_handle(handle);
        ::TraceFrames<details::NoneBackend::entry> fallback_frames{};
        fallback_frames.push_back(entry);
        details::NoneBackend::capture::template capture<::fast_io::native_global_allocator>(fallback_frames, 0);
        pltxt2htm_test_assert_true(fallback_frames.size() == 1 && fallback_frames[0].native_handle() == handle);
    }
}
