#pragma once

#include <cstdlib>

#include <pltxt2htm/details/demangle.hh>

#include "doctest_config.hh"
#include "details/helpers.hh"

#if defined(pltxt2htm_assert) || defined(pltxt2htm_assume) || defined(pltxt2htm_unreachable)
    #error "stacktrace.hh must not leave the assertion macros defined"
#endif

/// A pairing whose entry is not a native stacktrace entry, for the concept's
/// negative case.
struct TestNonconformingBackend {
    using entry = int;
    using capture = int;
};

/// An otherwise conforming entry whose copy constructor throws, isolating the
/// no-throw trait half of the entry concept's negative case.
struct TestThrowingCopyEntry {
    TestThrowingCopyEntry() noexcept = default;

    TestThrowingCopyEntry(TestThrowingCopyEntry const&) noexcept(false) {
    }

    TestThrowingCopyEntry(TestThrowingCopyEntry&&) noexcept = default;
    ~TestThrowingCopyEntry() = default;

    [[nodiscard]]
    constexpr auto operator<=>(TestThrowingCopyEntry const&) const noexcept -> ::std::strong_ordering = default;
    [[nodiscard]]
    constexpr auto operator==(TestThrowingCopyEntry const&) const noexcept -> bool = default;

    [[nodiscard]]
    constexpr auto native_handle() const noexcept -> int {
        return 0;
    }

    [[nodiscard]]
    constexpr auto resolved() const noexcept -> ::pltxt2htm::stacktrace::ResolvedFrame {
        return {};
    }

    constexpr explicit operator bool() const noexcept {
        return false;
    }

    constexpr auto operator=(TestThrowingCopyEntry const&) noexcept -> TestThrowingCopyEntry& = default;
};

/// A capture backend with a wrongly typed `supported` flag, isolating the
/// static-member shape half of the capture concept's negative case.
struct TestBadSupportedCapture {
    inline static int supported{};
    static constexpr ::std::size_t max_depth{};

    template<typename Allocator>
    static void capture(::pltxt2htm::container::Vector<::pltxt2htm::stacktrace::details::NoneStacktraceEntry,
                                                       ::fast_io::generic_allocator_adapter<Allocator>>&,
                        ::std::size_t) noexcept {
    }
};

TEST_CASE("front-end-compile-surface") {
    namespace trace = ::pltxt2htm::stacktrace;
    static_assert(::std::random_access_iterator<trace::Stacktrace::const_iterator>);
    static_assert(
        ::std::same_as<decltype(::std::declval<trace::Stacktrace const&>()[0]), trace::StacktraceEntry const&>);
}

TEST_CASE("front-end-default-entry-empty-frame") {
    namespace trace = ::pltxt2htm::stacktrace;
    // A value-constructed entry resolves to an empty frame on every backend.
    auto const empty_entry = trace::StacktraceEntry{};
    ::check_empty_resolved_frame(empty_entry.resolved());
}

TEST_CASE("front-end-capture-semantics") {
    namespace trace = ::pltxt2htm::stacktrace;
    if constexpr (trace::details::DefaultBackend::capture::supported) {
        auto const grown = trace::Stacktrace::current(0);
        REQUIRE(!grown.is_empty());
        for (auto const& entry : grown) {
            CHECK(entry.native_handle() != trace::StacktraceEntry::native_handle_type{});
        }
    }
    else {
        // Without a usable capture backend the front-end reports an empty trace.
        CHECK(trace::Stacktrace::current(0).is_empty());
    }
}

TEST_CASE("front-end-snapshot-copy-comparison-iteration") {
    namespace trace = ::pltxt2htm::stacktrace;
    auto const snapshot = trace::Stacktrace::current(0);
    auto const copied_snapshot = snapshot;
    CHECK(snapshot.size() == copied_snapshot.size());
    for (::std::size_t i = 0; i < snapshot.size(); ++i) {
        CHECK(snapshot[i].native_handle() == copied_snapshot[i].native_handle());
        CHECK(snapshot[i] == copied_snapshot[i]);
    }
    auto const snapshot_order = snapshot <=> copied_snapshot;
    CHECK(snapshot == copied_snapshot);
    CHECK(snapshot_order == ::std::strong_ordering::equal);
    CHECK(snapshot.cbegin() == snapshot.begin());
    CHECK(snapshot.cend() == snapshot.end());
    CHECK(snapshot.crbegin() == snapshot.rbegin());
    CHECK(snapshot.crend() == snapshot.rend());
    auto reverse = snapshot.rbegin();
    for (::std::size_t i = snapshot.size(); i != 0; --i, ++reverse) {
        CHECK(*reverse == snapshot[i - 1]);
    }
    CHECK(reverse == snapshot.rend());
}

TEST_CASE("front-end-skip-wider-than-stack") {
    namespace trace = ::pltxt2htm::stacktrace;
    CHECK(trace::Stacktrace::current((::std::numeric_limits<::std::size_t>::max)()).is_empty());
}

TEST_CASE("front-end-move-swap-preserves-value") {
    namespace trace = ::pltxt2htm::stacktrace;
    auto const snapshot = trace::Stacktrace::current(0);
    auto moved_source = snapshot;
    auto moved = ::std::move(moved_source);
    CHECK(moved == snapshot);
    CHECK(moved_source.is_empty());
    moved_source = ::std::move(moved);
    CHECK(moved_source == snapshot);
    CHECK(moved.is_empty());
    moved_source.swap(moved);
    CHECK(moved == snapshot);
    CHECK(moved_source.is_empty());
    moved_source = snapshot;
    CHECK(moved_source == snapshot);
    // Self-assignment and self-move are contract violations; testing them
    // requires a trap death harness, which this runner does not have.
}

TEST_CASE("front-end-alternate-allocator-comparison") {
    namespace trace = ::pltxt2htm::stacktrace;
    // Another allocator instance still yields an interchangeable empty trace,
    // spelling both template parameters of the public alias explicitly.
    using AlternateTrace =
        trace::BasicStacktrace<trace::details::DefaultBackend, ::fast_io::native_thread_local_allocator>;
    AlternateTrace alternate{};
    static_assert(
        ::std::same_as<AlternateTrace, trace::details::BasicStacktrace<trace::details::DefaultBackend,
                                                                       ::fast_io::native_thread_local_allocator>>);
    static_assert(::std::same_as<trace::BasicStacktrace<>, trace::Stacktrace>);
    CHECK(alternate == trace::Stacktrace{});
    // The comparison is symmetric, so either side may name the allocator.
    CHECK(trace::Stacktrace{} == alternate);
    auto const alternate_order = alternate <=> trace::Stacktrace{};
    CHECK(alternate_order == ::std::strong_ordering::equal);
    auto const reverse_alternate_order = trace::Stacktrace{} <=> alternate;
    CHECK(reverse_alternate_order == ::std::strong_ordering::equal);
    (void)alternate.get_allocator();
}

TEST_CASE("front-end-tracked-allocator-ownership") {
    namespace trace = ::pltxt2htm::stacktrace;
    using TrackedTrace = trace::details::BasicStacktrace<trace::details::DefaultBackend, TrackingAllocator>;
    {
        TrackedTrace tracked = TrackedTrace::current(0);
        TrackedTrace copy{tracked};
        TrackedTrace moved_copy{::std::move(copy)};
        CHECK(tracked == moved_copy);
        CHECK(copy.is_empty());
        // Self-assignment and self-move are contract violations; testing them
        // requires a trap death harness, which this runner does not have.
    }
    // The balance check runs after the locals above released their allocations.
    if constexpr (trace::details::DefaultBackend::capture::supported) {
        CHECK(TrackingAllocator::allocations > 0);
    }
    CHECK(TrackingAllocator::outstanding == 0);
}

TEST_CASE("concept-describes-the-bundles") {
    namespace details = ::pltxt2htm::stacktrace::details;
    static_assert(details::is_stacktrace_backend<details::NoneBackend>);
    static_assert(details::is_stacktrace_backend<details::NtBackend>);
    static_assert(details::is_stacktrace_backend<details::StdBackend>);
    static_assert(details::is_stacktrace_backend<details::DefaultBackend>);
    static_assert(!details::is_stacktrace_backend<TestNonconformingBackend>);
}

TEST_CASE("concept-halves-describe-entries-and-captures") {
    namespace details = ::pltxt2htm::stacktrace::details;
    static_assert(details::is_stacktrace_entry<details::NoneStacktraceEntry>);
    static_assert(details::is_stacktrace_capture_backend<details::NoneCaptureBackend, details::NoneStacktraceEntry>);
    static_assert(!details::is_stacktrace_entry<int>);
    static_assert(!details::is_stacktrace_entry<TestThrowingCopyEntry>);
    static_assert(!details::is_stacktrace_capture_backend<int, details::NoneStacktraceEntry>);
    // A capture backend is tied to its entry type through the storage it fills.
    static_assert(!details::is_stacktrace_capture_backend<details::NoneCaptureBackend, int>);
    static_assert(!details::is_stacktrace_capture_backend<TestBadSupportedCapture, details::NoneStacktraceEntry>);
#if defined(_WIN32)
    static_assert(details::is_stacktrace_entry<details::NtStacktraceEntry>);
    static_assert(details::is_stacktrace_capture_backend<details::NtCaptureBackend, details::NtStacktraceEntry>);
    static_assert(!details::is_stacktrace_capture_backend<details::NtCaptureBackend, details::NoneStacktraceEntry>);
#endif
#if !defined(__GLIBCXX__) && __has_include(<stacktrace>) && defined(__cpp_lib_stacktrace) \
    && (__cpp_lib_stacktrace >= 202011L)
    static_assert(details::is_stacktrace_entry<details::StdStacktraceEntry>);
    static_assert(details::is_stacktrace_capture_backend<details::StdCaptureBackend, details::StdStacktraceEntry>);
#endif
}

#if defined(__GXX_ABI_VERSION) && !defined(_MSC_VER) && __has_include(<cxxabi.h>)
TEST_CASE("demangle-retry-on-package-stripped-underscore") {
    // Symbol producers that strip the leading underscore are retried with
    // it: the demangler must turn the plain spelling into readable text.
    auto* const demangled = ::pltxt2htm::details::demangle_symbol_name("Z10deep_tracei");
    REQUIRE(demangled != nullptr);
    CHECK(::std::string_view{demangled} == "deep_trace(int)");
    ::std::free(demangled);
}
#endif
