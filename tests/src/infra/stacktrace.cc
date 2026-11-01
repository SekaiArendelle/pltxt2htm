#include <pltxt2htm/stacktrace/stacktrace.hh>

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
    if (result.empty()) {
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

int main() {
    namespace trace = ::pltxt2htm::stacktrace;
    [[maybe_unused]] void* storage[8]{};
#if defined(_WIN32)
    // Backend contract: capacity-bounded writes, skip handling, written count.
    pltxt2htm_test_assert_true(trace::details::CaptureBackend::capture({}, 0) == 0);
    pltxt2htm_test_assert_true(
        trace::details::CaptureBackend::capture(storage, (::std::numeric_limits<::std::size_t>::max)()) == 0);
    void* guarded[3]{nullptr, nullptr, reinterpret_cast<void*>(1)};
    pltxt2htm_test_assert_true(trace::details::CaptureBackend::capture({guarded, 1}, 0) == 1);
    pltxt2htm_test_assert_true(guarded[0] != nullptr && guarded[1] == nullptr);
    pltxt2htm_test_assert_true(guarded[2] == reinterpret_cast<void*>(1));
    // Capacity is the number retained after skipping.
    pltxt2htm_test_assert_true(trace::details::CaptureBackend::capture({guarded, 1}, 1) == 1);
    pltxt2htm_test_assert_true(guarded[1] == nullptr);
    auto const written = trace::details::CaptureBackend::capture(storage, 0);
    pltxt2htm_test_assert_true(written > 0 && written <= 8);
    // The front-end stores exactly the addresses the backend reported.
    auto const grown = trace::details::capture_all<::fast_io::native_global_allocator>(0);
    pltxt2htm_test_assert_true(!grown.addresses.is_empty());
    for (void* address : grown.addresses) {
        pltxt2htm_test_assert_true(address != nullptr);
    }
#else
    auto const unsupported = trace::details::capture_all<::fast_io::native_global_allocator>(0);
    pltxt2htm_test_assert_true(unsupported.addresses.is_empty() && !unsupported.possibly_truncated);
#endif
    static_assert(::std::random_access_iterator<trace::Stacktrace::const_iterator>);
    static_assert(
        ::std::same_as<decltype(::std::declval<trace::Stacktrace const&>()[0]), trace::StacktraceEntry const&>);
    constexpr trace::StacktraceEntry empty_entry{};
    static_assert(!empty_entry && empty_entry.native_handle() == nullptr);
    {
        auto const empty_frame = empty_entry.resolved();
        pltxt2htm_test_assert_true(empty_frame.address == nullptr && empty_frame.description.is_empty() &&
                                   empty_frame.source_file.is_empty() && empty_frame.source_line == 0 &&
                                   !empty_frame.text_truncated && empty_frame.displacement == 0);
    }
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
    pltxt2htm_test_assert_true(trace::Stacktrace::current((::std::numeric_limits<::std::size_t>::max)()).empty());
    auto moved_source = snapshot;
    auto moved = ::std::move(moved_source);
    pltxt2htm_test_assert_true(moved == snapshot && moved_source.empty());
    moved_source = ::std::move(moved);
    pltxt2htm_test_assert_true(moved_source == snapshot && moved.empty());
    moved_source.swap(moved);
    pltxt2htm_test_assert_true(moved == snapshot && moved_source.empty());
    moved_source = snapshot;
    pltxt2htm_test_assert_true(moved_source == snapshot);
    trace::BasicStacktrace<::fast_io::native_thread_local_allocator> alternate{};
    pltxt2htm_test_assert_true(alternate == trace::Stacktrace{});
    (void)alternate.get_allocator();
    {
        TrackingAllocator const alloc{};
        using TrackedTrace = trace::BasicStacktrace<TrackingAllocator>;
        auto tracked = TrackedTrace::current(0);
        TrackedTrace copy{tracked, alloc};
        TrackedTrace moved_copy{::std::move(copy), alloc};
        pltxt2htm_test_assert_true(tracked == moved_copy && copy.empty());
        // Self-assignment and self-move are contract violations; testing them
        // requires a trap death harness, which this file does not have.
    }
    pltxt2htm_test_assert_true(TrackingAllocator::outstanding == 0);
#if defined(_WIN32)
    pltxt2htm_test_assert_true(TrackingAllocator::allocations > 0);
    auto const deep = ::deep_trace(96);
    pltxt2htm_test_assert_true(deep.size() > 64 && !deep.possibly_truncated());
    pltxt2htm_test_assert_true((trace::Stacktrace{} <=> deep) < 0);
    pltxt2htm_test_assert_true(!deep.empty() && deep[0]);
    // Entry queries use the saved address, not a new capture.
    auto const first_frame = deep[0].resolved();
    auto const expected_frame = trace::resolve(deep[0].native_handle());
    pltxt2htm_test_assert_true(
        first_frame.address == deep[0].native_handle() && first_frame.description == expected_frame.description &&
        first_frame.source_file == expected_frame.source_file && first_frame.source_line == expected_frame.source_line);
    // DbgHelp must reach the image's symbols (PDB or, for GNU-ABI builds, the
    // export table) for frames inside the test executable.
    pltxt2htm_test_assert_true(!first_frame.description.is_empty());
    // std::stacktrace semantics: entry zero of current(0) is the caller of
    // current(), and skip shifts the trace by exactly one entry.
    auto const from_here = ::trace_from_named_frame(0);
    auto const one_skipped = ::trace_from_named_frame(1);
    pltxt2htm_test_assert_true(from_here.size() > 1);
    pltxt2htm_test_assert_true(one_skipped.size() + 1 == from_here.size());
    auto const from_here_name = from_here[0].resolved().description;
    auto const from_here_view = ::std::string_view{from_here_name.data(), from_here_name.size()};
    pltxt2htm_test_assert_true(from_here_view.find("trace_from_named_frame") != ::std::string_view::npos);
    #if defined(__GXX_ABI_VERSION) && !defined(_MSC_VER) && __has_include(<cxxabi.h>)
    // GNU-ABI builds receive Itanium-mangled export names, which DbgHelp reports
    // without the leading underscore; the demangler must turn deep_trace's name
    // into readable text instead of the raw "Z10deep_trace..." spelling.
    auto const named = trace::resolve(reinterpret_cast<void*>(::std::addressof(::deep_trace)));
    pltxt2htm_test_assert_true(!named.description.is_empty());
    pltxt2htm_test_assert_true(named.description.data()[0] != u8'Z');
    #endif
    pltxt2htm_test_assert_true(deep.max_size() >= deep.size());
#endif
#if defined(_WIN32)
    namespace win32 = ::pltxt2htm::sys_win32;
    auto& session = trace::details::get_symbol_session();
    pltxt2htm_test_assert_true(session.is_ready());
    pltxt2htm_test_assert_true((win32::SymGetOptions() & 0x00000010) != 0);
    // Resolve a module loaded after DbgHelp initialized its module list.
    auto* late_module = win32::LoadLibraryExW(L"winmm.dll", nullptr, 0x00000800);
    pltxt2htm_test_assert_true(late_module != nullptr);
    auto const late_function = win32::GetProcAddress(late_module, "PlaySoundW");
    pltxt2htm_test_assert_true(late_function != nullptr);
    auto const late_frame = trace::resolve(reinterpret_cast<void*>(late_function));
    pltxt2htm_test_assert_true(!late_frame.description.is_empty());
    (void)win32::FreeLibrary(late_module);
    // Exercise contention fallback without depending on another thread's timing.
    {
        auto held = trace::details::DbgHelpLock::try_acquire(session.lock);
        pltxt2htm_test_assert_true(static_cast<bool>(held));
        auto const busy = trace::resolve(storage[0]);
        pltxt2htm_test_assert_true(busy.address == storage[0] && busy.description.is_empty());
        pltxt2htm_test_assert_true(!trace::details::shutdown_symbols());
        pltxt2htm_test_assert_true(session.is_ready() && session.process != nullptr);
    }
    auto const invalid = trace::resolve(nullptr);
    pltxt2htm_test_assert_true(invalid.address == nullptr && invalid.description.is_empty());
    pltxt2htm_test_assert_true(trace::details::shutdown_symbols());
    pltxt2htm_test_assert_true(!session.is_ready() && session.process == nullptr);
    auto const again = trace::resolve(storage[0]);
    pltxt2htm_test_assert_true(again.address == storage[0] && session.is_ready());
    pltxt2htm_test_assert_true(trace::details::shutdown_symbols());
    // A recorded initialization failure must not be retried by a later lookup.
    session.state = trace::details::DbgHelpSession::State::unavailable;
    auto const unavailable = trace::resolve(storage[0]);
    pltxt2htm_test_assert_true(unavailable.address == storage[0] && unavailable.description.is_empty() &&
                               session.process == nullptr && !session.is_ready());
    pltxt2htm_test_assert_true(trace::details::shutdown_symbols());
#endif
}
