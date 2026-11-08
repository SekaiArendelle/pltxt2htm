#include "stacktrace_common.hh"
#include "stacktrace_none.hh"
#include "stacktrace_nt.hh"
#include "stacktrace_std.hh"
// Exercised on every ABI that provides a demangler, not only on Windows.
#include <pltxt2htm/details/demangle.hh>

#include <cstdlib>

#if defined(pltxt2htm_assert) || defined(pltxt2htm_assume) || defined(pltxt2htm_unreachable)
    #error "stacktrace.hh must not leave the assertion macros defined"
#endif

// windows.h defines min and max as function-like macros, which would expand
// inside std::numeric_limits<...>::max(). Keep them undefined in this file.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

int main() {
    namespace trace = ::pltxt2htm::stacktrace;

    {
        // Compile-time surface of the public front-end: iterator category and
        // indexing.
        static_assert(::std::random_access_iterator<trace::Stacktrace::const_iterator>);
        static_assert(
            ::std::same_as<decltype(::std::declval<trace::Stacktrace const&>()[0]), trace::StacktraceEntry const&>);
        // A value-constructed entry resolves to an empty frame on every backend.
        auto const empty_entry = trace::StacktraceEntry{};
        auto const empty_frame = empty_entry.resolved();
        pltxt2htm_test_assert_true(empty_frame.address == 0 && empty_frame.description.is_empty() &&
                                   empty_frame.source_file.is_empty() && empty_frame.source_line == 0 &&
                                   !empty_frame.text_truncated && empty_frame.displacement == 0);
    }

    // Every backend runs its own tests where its bundle is supported.
    ::test_none_stacktrace();
    ::test_nt_stacktrace();
    ::test_std_stacktrace();

    {
        // Front-end capture semantics for the selected default bundle.
        if constexpr (trace::details::DefaultBackend::capture::supported) {
            auto const grown = trace::Stacktrace::current(0);
            pltxt2htm_test_assert_true(!grown.is_empty());
            for (auto const& entry : grown) {
                pltxt2htm_test_assert_true(entry.native_handle() != trace::StacktraceEntry::native_handle_type{});
            }
        }
        else {
            // Without a usable capture backend the front-end reports an empty trace.
            pltxt2htm_test_assert_true(trace::Stacktrace::current(0).is_empty());
        }
    }

    {
        // Copying, comparison and iteration order of a captured snapshot.
        auto const snapshot = trace::Stacktrace::current(0);
        auto const copied_snapshot = snapshot;
        pltxt2htm_test_assert_true(snapshot.size() == copied_snapshot.size());
        for (::std::size_t i = 0; i < snapshot.size(); ++i) {
            pltxt2htm_test_assert_true(snapshot[i].native_handle() == copied_snapshot[i].native_handle());
            pltxt2htm_test_assert_true(snapshot[i] == copied_snapshot[i]);
        }
        pltxt2htm_test_assert_true(snapshot == copied_snapshot && (snapshot <=> copied_snapshot) == 0);
        pltxt2htm_test_assert_true(snapshot.cbegin() == snapshot.begin() && snapshot.cend() == snapshot.end());
        pltxt2htm_test_assert_true(snapshot.crbegin() == snapshot.rbegin() && snapshot.crend() == snapshot.rend());
        auto reverse = snapshot.rbegin();
        for (::std::size_t i = snapshot.size(); i != 0; --i, ++reverse) {
            pltxt2htm_test_assert_true(*reverse == snapshot[i - 1]);
        }
        pltxt2htm_test_assert_true(reverse == snapshot.rend());
    }

    {
        // A skip wider than the stack yields an empty trace.
        pltxt2htm_test_assert_true(
            trace::Stacktrace::current((::std::numeric_limits<::std::size_t>::max)()).is_empty());
    }
    {
        // Move construction, move assignment and swap preserve the value.
        auto const snapshot = trace::Stacktrace::current(0);
        auto moved_source = snapshot;
        auto moved = ::std::move(moved_source);
        pltxt2htm_test_assert_true(moved == snapshot && moved_source.is_empty());
        moved_source = ::std::move(moved);
        pltxt2htm_test_assert_true(moved_source == snapshot && moved.is_empty());
        moved_source.swap(moved);
        pltxt2htm_test_assert_true(moved == snapshot && moved_source.is_empty());
        moved_source = snapshot;
        pltxt2htm_test_assert_true(moved_source == snapshot);
    }

    {
        // Another allocator instance still yields an interchangeable empty trace,
        // spelling both template parameters of the public alias explicitly.
        using AlternateTrace =
            trace::BasicStacktrace<trace::details::DefaultBackend, ::fast_io::native_thread_local_allocator>;
        AlternateTrace alternate{};
        static_assert(
            ::std::same_as<AlternateTrace, trace::details::BasicStacktrace<trace::details::DefaultBackend,
                                                                           ::fast_io::native_thread_local_allocator>>);
        static_assert(::std::same_as<trace::BasicStacktrace<>, trace::Stacktrace>);
        pltxt2htm_test_assert_true(alternate == trace::Stacktrace{});
        // The comparison is symmetric, so either side may name the allocator.
        pltxt2htm_test_assert_true(trace::Stacktrace{} == alternate);
        pltxt2htm_test_assert_true((alternate <=> trace::Stacktrace{}) == 0);
        pltxt2htm_test_assert_true((trace::Stacktrace{} <=> alternate) == 0);
        (void)alternate.get_allocator();
    }

    {
        // Tracked construction and moves release every allocation.
        using TrackedTrace = trace::details::BasicStacktrace<trace::details::DefaultBackend, TrackingAllocator>;
        auto tracked = TrackedTrace::current(0);
        TrackedTrace copy{tracked};
        TrackedTrace moved_copy{::std::move(copy)};
        pltxt2htm_test_assert_true(tracked == moved_copy && copy.is_empty());
        // Self-assignment and self-move are contract violations; testing them
        // requires a trap death harness, which this file does not have.
    }
    {
        // Tracked allocations are balanced, and captures allocate.
        if constexpr (trace::details::DefaultBackend::capture::supported) {
            pltxt2htm_test_assert_true(TrackingAllocator::allocations > 0);
        }
        pltxt2htm_test_assert_true(TrackingAllocator::outstanding == 0);
    }

#if defined(__GXX_ABI_VERSION) && !defined(_MSC_VER) && __has_include(<cxxabi.h>)
    {
        // Symbol producers that strip the leading underscore are retried with
        // it: the demangler must turn the plain spelling into readable text.
        auto* const demangled = ::pltxt2htm::details::demangle_symbol_name("Z10deep_tracei");
        pltxt2htm_test_assert_true(demangled != nullptr);
        if (demangled != nullptr) {
            pltxt2htm_test_assert_true(::std::string_view{demangled} == "deep_trace(int)");
            ::std::free(demangled);
        }
    }
#endif
}

#pragma pop_macro("max")
#pragma pop_macro("min")
