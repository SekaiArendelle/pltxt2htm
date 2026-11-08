#pragma once

#include "doctest_config.hh"
#include "details/helpers.hh"

template<typename Backend>
void test_std_stacktrace_backend() {
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
        CHECK(!empty_entry);
        ::check_empty_resolved_frame(empty_entry.resolved());
    }
    {
        // std::stacktrace semantics: entry zero of current(0) is the caller of
        // current(), and skip shifts the trace by exactly one entry; the
        // host-defined naming is asserted on the NT path only.
        using BackendTrace = ::pltxt2htm::stacktrace::details::BasicStacktrace<Backend>;
        auto const from_here = ::trace_from_named_frame<BackendTrace>(0);
        auto const one_skipped = ::trace_from_named_frame<BackendTrace>(1);
        REQUIRE(from_here.size() > 1);
        CHECK(one_skipped.size() + 1 == from_here.size());
    }
    {
        // The std fallback resolves through the standard entry: the saved
        // native handle round-trips, and naming depends on the host resolver,
        // so only the address is asserted here.
        using BackendTrace = ::pltxt2htm::stacktrace::details::BasicStacktrace<Backend>;
        auto const deep = ::deep_trace<BackendTrace>(96);
        REQUIRE(deep.size() > 64);
        auto const deep_order = BackendTrace{} <=> deep;
        CHECK(deep_order == ::std::strong_ordering::less);
        CHECK(!deep.is_empty());
        CHECK(static_cast<bool>(deep[0]));
        auto const first_frame = deep[0].resolved();
        CHECK(first_frame.address != 0);
        CHECK(first_frame.address == reinterpret_cast<::std::uintptr_t>(deep[0].native_handle()));
    }
}

TEST_CASE("std-backend") {
    namespace details = ::pltxt2htm::stacktrace::details;
    if constexpr (details::StdBackend::capture::supported) {
        test_std_stacktrace_backend<details::StdBackend>();
    }
}
