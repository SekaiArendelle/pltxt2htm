#pragma once

#include "doctest_config.hh"
#include "details/helpers.hh"

void test_none_entry_resolution() {
    namespace details = ::pltxt2htm::stacktrace::details;
    // The fallback entry keeps the address and produces no text.
    constexpr details::NoneBackend::entry fallback{};
    static_assert(::std::same_as<details::NoneBackend::entry::native_handle_type, ::std::uintptr_t>);
    static_assert(!fallback);
    ::check_empty_resolved_frame(fallback.resolved());
    // Entry construction from native addresses resolves to an address-only frame.
    int object{};
    auto const handle = reinterpret_cast<details::NoneBackend::entry::native_handle_type>(::std::addressof(object));
    auto const entry = details::NoneBackend::entry::from_native_handle(handle);
    CHECK(entry.native_handle() == handle);
    auto const frame = entry.resolved();
    CHECK(frame.address == handle);
    CHECK(frame.description.is_empty());
    CHECK(frame.source_file.is_empty());
    CHECK(frame.source_line == 0);
}

TEST_CASE("none-backend-capture-contract") {
    namespace details = ::pltxt2htm::stacktrace::details;
    static_assert(!can_access_entry_storage<details::NoneBackend::entry>);
    ::test_backend_capture_contract<details::NoneBackend>();
}

TEST_CASE("none-entry-resolution") {
    ::test_none_entry_resolution();
}

TEST_CASE("none-backend-noop") {
    namespace details = ::pltxt2htm::stacktrace::details;
    // Its backend is a declared no-op, so an existing entry is left untouched.
    int object{};
    auto const handle = reinterpret_cast<details::NoneBackend::entry::native_handle_type>(::std::addressof(object));
    auto const entry = details::NoneBackend::entry::from_native_handle(handle);
    ::TraceFrames<details::NoneBackend::entry> fallback_frames{};
    fallback_frames.push_back(entry);
    details::NoneBackend::capture::template capture<::fast_io::native_global_allocator>(fallback_frames, 0);
    REQUIRE(fallback_frames.size() == 1);
    CHECK(fallback_frames[0].native_handle() == handle);
}
