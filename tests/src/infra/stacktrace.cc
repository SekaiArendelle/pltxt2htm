#include <pltxt2htm/stacktrace/stacktrace.hh>
// The fallback backend exists on every platform: include it explicitly so the
// test covers the no-resolver entry and backend even on Windows.
#include <pltxt2htm/stacktrace/impl/none/stacktrace_entry.hh>
#include <pltxt2htm/stacktrace/impl/none/capture.hh>

#if defined(pltxt2htm_assert) || defined(pltxt2htm_assume) || defined(pltxt2htm_unreachable)
    #error "stacktrace.hh must not leave the assertion macros defined"
#endif

#if defined(_WIN32)
    #include <pltxt2htm/sys/win32/kernel32.hh>
#endif
#include <fast_io/fast_io_dsal/string.h>
#include "precompile.hh"
#include <limits>
#include <concepts>
#include <iterator>
#include <memory>
#include <string_view>
#include <utility>

// windows.h defines min and max as function-like macros, which would expand
// inside std::numeric_limits<...>::max(). Keep them undefined in this file.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

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

// Entries expose no handle storage to callers: a trace's entries stay
// immutable, and only the capture backend fills them. The check lives in a
// concept because this clang rejects a bare requires-expression on a concrete
// type whose explicit object parameter does not bind.
template<typename Entry>
concept can_access_entry_storage = requires(Entry& entry) { entry.native_handle_storage(); };

// Positive control: the concept must still detect an accessible accessor, so a
// renamed or deleted member cannot make the assertions below vacuous.
struct StorageProbe {
    [[nodiscard]]
    constexpr auto native_handle_storage(this StorageProbe& self) noexcept -> void* {
        return ::std::addressof(self.value);
    }

    void* value{};
};

static_assert(can_access_entry_storage<StorageProbe>);

[[nodiscard]]
#if defined(_MSC_VER)
__declspec(noinline)
#elif defined(__GNUC__)
[[gnu::noinline]]
#endif
::pltxt2htm::stacktrace::Stacktrace deep_trace(::std::size_t depth) noexcept {
    if (depth == 0) {
        return ::pltxt2htm::stacktrace::Stacktrace::current();
    }
    auto result = ::deep_trace(depth - 1);
    // Keep work after recursion so optimized builds cannot use a tail call.
    if (result.is_empty()) {
        return {};
    }
    return result;
}

[[nodiscard]]
#if defined(_MSC_VER)
__declspec(noinline)
#elif defined(__GNUC__)
[[gnu::noinline]]
#endif
::pltxt2htm::stacktrace::Stacktrace trace_from_named_frame(::std::size_t skip) noexcept {
    return ::pltxt2htm::stacktrace::Stacktrace::current(skip);
}

using test_frames =
    ::pltxt2htm::container::Vector<::pltxt2htm::stacktrace::StacktraceEntry,
                                   ::fast_io::generic_allocator_adapter<::fast_io::native_global_allocator>>;

#if defined(_MSC_VER)
__declspec(noinline)
#elif defined(__GNUC__)
[[gnu::noinline]]
#endif
void capture_from_shared_site(test_frames& frames, ::std::size_t skip) {
    ::pltxt2htm::stacktrace::details::CaptureBackend::capture<::fast_io::native_global_allocator>(frames, skip);
}

int main() {
    namespace trace = ::pltxt2htm::stacktrace;
    [[maybe_unused]] trace::StacktraceEntry::native_handle_type sample_handle{};

    {
        // Backend contract: append every captured entry, honor skip.
#if defined(_WIN32)
        test_frames frames{};
        trace::details::CaptureBackend::capture<::fast_io::native_global_allocator>(
            frames, (::std::numeric_limits<::std::size_t>::max)());
        pltxt2htm_test_assert_true(frames.is_empty());
        trace::details::CaptureBackend::capture<::fast_io::native_global_allocator>(frames, 0);
        pltxt2htm_test_assert_true(!frames.is_empty());
        for (auto const& entry : frames) {
            pltxt2htm_test_assert_true(entry.native_handle() != trace::StacktraceEntry::native_handle_type{});
        }
        sample_handle = frames[0].native_handle();
        // skip shifts the capture by one frame from the same call site.
        test_frames unskipped{};
        test_frames skipped{};
        capture_from_shared_site(unskipped, 0);
        capture_from_shared_site(skipped, 1);
        pltxt2htm_test_assert_true(unskipped.size() > 1);
        pltxt2htm_test_assert_true(skipped[0].native_handle() == unskipped[1].native_handle());
        // The front-end stores exactly the handles the backend reported.
        auto const grown = trace::Stacktrace::current(0);
        pltxt2htm_test_assert_true(!grown.is_empty());
        for (auto const& entry : grown) {
            pltxt2htm_test_assert_true(entry.native_handle() != trace::StacktraceEntry::native_handle_type{});
        }
#else
        // Without a capture backend the front-end reports an empty trace.
        pltxt2htm_test_assert_true(trace::Stacktrace::current(0).is_empty());
#endif
    }

    {
        // Compile-time surface: iterator category, indexing and empty entry.
        static_assert(::std::random_access_iterator<trace::Stacktrace::const_iterator>);
        static_assert(
            ::std::same_as<decltype(::std::declval<trace::Stacktrace const&>()[0]), trace::StacktraceEntry const&>);
        // Entries expose no handle storage to callers: only the capture backend
        // may fill it.
        static_assert(!can_access_entry_storage<trace::StacktraceEntry>);
        constexpr trace::StacktraceEntry empty_entry{};
        static_assert(!empty_entry && empty_entry.native_handle() == nullptr);
        auto const empty_frame = empty_entry.resolved();
        pltxt2htm_test_assert_true(empty_frame.address == nullptr && empty_frame.description.is_empty() &&
                                   empty_frame.source_file.is_empty() && empty_frame.source_line == 0 &&
                                   !empty_frame.text_truncated && empty_frame.displacement == 0);
    }

    {
        // Internal-use entry construction from native handles.
        int first_object{};
        auto const first_handle =
            reinterpret_cast<trace::StacktraceEntry::native_handle_type>(::std::addressof(first_object));
        auto const first_entry = trace::StacktraceEntry::from_native_handle(first_handle);
        pltxt2htm_test_assert_true(first_entry.native_handle() == first_handle);
        pltxt2htm_test_assert_true(static_cast<bool>(first_entry));
        pltxt2htm_test_assert_true(!trace::StacktraceEntry{});
    }

    {
        // The fallback entry resolves everywhere: without a symbol backend it
        // keeps the address and produces no text.
        constexpr trace::details::NoneStacktraceEntry fallback{};
        static_assert(!fallback);
        auto const empty = fallback.resolved();
        pltxt2htm_test_assert_true(empty.address == nullptr && empty.description.is_empty() &&
                                   empty.source_file.is_empty() && empty.source_line == 0);
        int object{};
        auto const handle =
            reinterpret_cast<trace::details::NoneStacktraceEntry::native_handle_type>(::std::addressof(object));
        auto const entry = trace::details::NoneStacktraceEntry::from_native_handle(handle);
        pltxt2htm_test_assert_true(entry.native_handle() == handle);
        auto const frame = entry.resolved();
        pltxt2htm_test_assert_true(frame.address == handle && frame.description.is_empty() &&
                                   frame.source_file.is_empty() && frame.source_line == 0);
        // Its backend is a declared no-op, so the front-end compiles against it
        // and an existing entry is left untouched.
        static_assert(!trace::details::NoneCaptureBackend::supported);
        static_assert(trace::details::NoneCaptureBackend::max_depth == 0);
        ::pltxt2htm::container::Vector<trace::details::NoneStacktraceEntry,
                                       ::fast_io::generic_allocator_adapter<::fast_io::native_global_allocator>>
            fallback_frames{};
        fallback_frames.push_back(entry);
        trace::details::NoneCaptureBackend::capture<::fast_io::native_global_allocator>(fallback_frames, 0);
        pltxt2htm_test_assert_true(fallback_frames.size() == 1 && fallback_frames[0].native_handle() == handle);
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
        pltxt2htm_test_assert_true(trace::Stacktrace::current(::std::numeric_limits<::std::size_t>::max()).is_empty());
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
        // Another allocator instance still yields an interchangeable empty trace.
        trace::BasicStacktrace<::fast_io::native_thread_local_allocator> alternate{};
        pltxt2htm_test_assert_true(alternate == trace::Stacktrace{});
        // The comparison is symmetric, so either side may name the allocator.
        pltxt2htm_test_assert_true(trace::Stacktrace{} == alternate);
        pltxt2htm_test_assert_true((alternate <=> trace::Stacktrace{}) == 0);
        pltxt2htm_test_assert_true((trace::Stacktrace{} <=> alternate) == 0);
        (void)alternate.get_allocator();
    }

    {
        // Tracked construction and moves release every allocation.
        TrackingAllocator const alloc{};
        using TrackedTrace = trace::BasicStacktrace<TrackingAllocator>;
        auto tracked = TrackedTrace::current(0);
        TrackedTrace copy{tracked, alloc};
        TrackedTrace moved_copy{::std::move(copy), alloc};
        pltxt2htm_test_assert_true(tracked == moved_copy && copy.is_empty());
        // Self-assignment and self-move are contract violations; testing them
        // requires a trap death harness, which this file does not have.
    }
    {
        // Tracked allocations are balanced, and Windows captures allocate.
#if defined(_WIN32)
        pltxt2htm_test_assert_true(TrackingAllocator::allocations > 0);
#endif
        pltxt2htm_test_assert_true(TrackingAllocator::outstanding == 0);
    }

#if defined(_WIN32)
    {
        // Deep capture: entries, comparison and saved addresses.
        auto const deep = ::deep_trace(96);
        pltxt2htm_test_assert_true(deep.size() > 64);
        pltxt2htm_test_assert_true((trace::Stacktrace{} <=> deep) < 0);
        pltxt2htm_test_assert_true(!deep.is_empty() && deep[0]);
        // Entry queries use the saved address, not a new capture.
        auto const first_frame = deep[0].resolved();
        pltxt2htm_test_assert_true(first_frame.address == deep[0].native_handle());
        // DbgHelp must reach the image's symbols (PDB or, for GNU-ABI builds,
        // the export table) for frames inside the test executable.
        pltxt2htm_test_assert_true(!first_frame.description.is_empty());
        pltxt2htm_test_assert_true(deep.max_size() >= deep.size());
    }

    {
        // std::stacktrace semantics: entry zero of current(0) is the caller of
        // current(), and skip shifts the trace by exactly one entry.
        auto const from_here = ::trace_from_named_frame(0);
        auto const one_skipped = ::trace_from_named_frame(1);
        pltxt2htm_test_assert_true(from_here.size() > 1);
        pltxt2htm_test_assert_true(one_skipped.size() + 1 == from_here.size());
        auto const from_here_name = from_here[0].resolved().description;
        auto const from_here_view = ::std::string_view{from_here_name.data(), from_here_name.size()};
        pltxt2htm_test_assert_true(from_here_view.find("trace_from_named_frame") != ::std::string_view::npos);
    }

    #if defined(__GXX_ABI_VERSION) && !defined(_MSC_VER) && __has_include(<cxxabi.h>)
    {
        // GNU-ABI builds receive Itanium-mangled export names, which DbgHelp
        // reports without the leading underscore; the demangler must turn
        // deep_trace's name into readable text instead of "Z10deep_trace...".
        auto const deep_trace_handle =
            reinterpret_cast<trace::StacktraceEntry::native_handle_type>(::std::addressof(::deep_trace));
        auto const named = trace::StacktraceEntry::from_native_handle(deep_trace_handle).resolved();
        pltxt2htm_test_assert_true(!named.description.is_empty());
        pltxt2htm_test_assert_true(named.description.data()[0] != u8'Z');
    }
    #endif
#endif

#if defined(_WIN32)
    {
        // Symbol session is ready with line information enabled.
        namespace win32 = ::pltxt2htm::sys_win32;
        auto& session = trace::details::get_symbol_session();
        pltxt2htm_test_assert_true(session.is_ready());
        pltxt2htm_test_assert_true((win32::SymGetOptions() & 0x00000010) != 0);
        // A module loaded after DbgHelp initialized its module list resolves.
        auto late_module = win32::LoadLibraryExW(L"winmm.dll", win32::Handle{}, 0x00000800);
        pltxt2htm_test_assert_true(!late_module.is_null());
        auto const late_function = win32::GetProcAddress(late_module, "PlaySoundW");
        pltxt2htm_test_assert_true(late_function != nullptr);
        auto const late_frame = trace::StacktraceEntry::from_native_handle(
                                    reinterpret_cast<trace::StacktraceEntry::native_handle_type>(late_function))
                                    .resolved();
        pltxt2htm_test_assert_true(!late_frame.description.is_empty());
        (void)win32::FreeLibrary(late_module);
    }

    {
        // A held lock makes resolution fall back instead of blocking.
        auto& session = trace::details::get_symbol_session();
        auto held = trace::details::DbgHelpLock::try_acquire(session.lock);
        pltxt2htm_test_assert_true(static_cast<bool>(held));
        auto const busy = trace::StacktraceEntry::from_native_handle(sample_handle).resolved();
        pltxt2htm_test_assert_true(busy.address == sample_handle && busy.description.is_empty());
        pltxt2htm_test_assert_true(!trace::details::shutdown_symbols());
        pltxt2htm_test_assert_true(session.is_ready() && !session.process.is_null());
    }

    {
        // Invalid addresses stay unresolved; shutdown is followed by a lazy
        // re-initialization on the next lookup.
        auto& session = trace::details::get_symbol_session();
        auto const invalid = trace::StacktraceEntry{}.resolved();
        pltxt2htm_test_assert_true(invalid.address == nullptr && invalid.description.is_empty());
        pltxt2htm_test_assert_true(trace::details::shutdown_symbols());
        pltxt2htm_test_assert_true(!session.is_ready() && session.process.is_null());
        auto const again = trace::StacktraceEntry::from_native_handle(sample_handle).resolved();
        pltxt2htm_test_assert_true(again.address == sample_handle && session.is_ready());
        pltxt2htm_test_assert_true(trace::details::shutdown_symbols());
    }

    {
        // A recorded initialization failure is not retried by later lookups.
        auto& session = trace::details::get_symbol_session();
        session.state = trace::details::DbgHelpSession::State::unavailable;
        auto const unavailable = trace::StacktraceEntry::from_native_handle(sample_handle).resolved();
        pltxt2htm_test_assert_true(unavailable.address == sample_handle && unavailable.description.is_empty() &&
                                   session.process.is_null() && !session.is_ready());
        pltxt2htm_test_assert_true(trace::details::shutdown_symbols());
    }
#endif
}

#pragma pop_macro("max")
#pragma pop_macro("min")
