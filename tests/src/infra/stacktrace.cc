#include <pltxt2htm/details/stacktrace/stacktrace.hh>
#include <pltxt2htm/details/stacktrace/resolve.hh>
#include <pltxt2htm/details/stacktrace/format.hh>
#if defined(_WIN32) && defined(PLTXT2HTM_ENABLE_STACKTRACE)
    #include <pltxt2htm/details/symbols/win32/kernel32.hh>
#endif
#include <fast_io/fast_io_dsal/string.h>
#include <cstring>
#include "precompile.hh"
#include <limits>
#include <memory>
#include <concepts>
#include <iterator>
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
::pltxt2htm::details::stacktrace::Stacktrace deep_trace(::std::size_t depth) noexcept {
    if (depth == 0) {
        return ::pltxt2htm::details::stacktrace::Stacktrace::current();
    }
    auto result = ::deep_trace(depth - 1);
    // Keep work after recursion so optimized builds cannot use a tail call.
    if (result.empty()) {
        return {};
    }
    return result;
}

int main() {
    namespace trace = ::pltxt2htm::details::stacktrace;
    void* storage[8]{};
    auto const empty = trace::capture({});
    pltxt2htm_test_assert_true(empty.size == 0 && !empty.possibly_truncated);
    auto const skipped = trace::capture(storage, (::std::numeric_limits<::std::size_t>::max)());
    pltxt2htm_test_assert_true(skipped.size == 0);
    void* guarded[3]{nullptr, nullptr, reinterpret_cast<void*>(1)};
    auto const single = trace::capture({guarded, 1});
    pltxt2htm_test_assert_true(guarded[1] == nullptr && guarded[2] == reinterpret_cast<void*>(1));
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(_WIN32)
    pltxt2htm_test_assert_true(single.size == 1 && single.possibly_truncated);
    pltxt2htm_test_assert_true(guarded[0] != nullptr);
    // Capacity is the number retained after skipping.
    auto const after_skip = trace::capture({guarded, 1}, 1);
    pltxt2htm_test_assert_true(after_skip.size == 1 && guarded[1] == nullptr);
    auto const full = trace::capture(storage);
    pltxt2htm_test_assert_true(full.size > 0 && full.size <= 8);
#else
    pltxt2htm_test_assert_true(single.size == 0 && !single.possibly_truncated);
#endif
    static_assert(::std::random_access_iterator<trace::Stacktrace::const_iterator>);
    static_assert(::std::same_as<decltype(::std::declval<trace::Stacktrace const&>()[0]), trace::StacktraceEntry const&>);
    constexpr trace::StacktraceEntry empty_entry{};
    static_assert(!empty_entry && empty_entry.native_handle() == nullptr);
    pltxt2htm_test_assert_true(empty_entry.description().is_empty());
    pltxt2htm_test_assert_true(empty_entry.source_file().is_empty() && empty_entry.source_line() == 0);
    auto const snapshot = trace::Stacktrace::current(0, 8);
    auto const copied_snapshot = snapshot;
    pltxt2htm_test_assert_true(snapshot.size() == copied_snapshot.size());
    for (::std::size_t i = 0; i < snapshot.size(); ++i) {
        pltxt2htm_test_assert_true(snapshot[i].native_handle() == copied_snapshot[i].native_handle());
        pltxt2htm_test_assert_true(snapshot.at(i) == copied_snapshot[i]);
    }
    pltxt2htm_test_assert_true(snapshot == copied_snapshot && (snapshot <=> copied_snapshot) == 0);
    pltxt2htm_test_assert_true(snapshot.cbegin() == snapshot.begin() && snapshot.cend() == snapshot.end());
    pltxt2htm_test_assert_true(snapshot.crbegin() == snapshot.rbegin() && snapshot.crend() == snapshot.rend());
    auto reverse = snapshot.rbegin();
    for (::std::size_t i = snapshot.size(); i != 0; --i, ++reverse) {
        pltxt2htm_test_assert_true(*reverse == snapshot[i - 1]);
    }
    pltxt2htm_test_assert_true(reverse == snapshot.rend());
    pltxt2htm_test_assert_true(trace::Stacktrace::current(0, 0).empty());
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
        auto tracked = TrackedTrace::current(0, 8, alloc);
        TrackedTrace copy{tracked, alloc};
        TrackedTrace moved_copy{::std::move(copy), alloc};
        pltxt2htm_test_assert_true(tracked == moved_copy && copy.empty());
        tracked = tracked;
        pltxt2htm_test_assert_true(tracked == moved_copy);
        tracked = ::std::move(tracked);
        pltxt2htm_test_assert_true(tracked == moved_copy);
    }
    pltxt2htm_test_assert_true(TrackingAllocator::outstanding == 0);
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(_WIN32)
    pltxt2htm_test_assert_true(TrackingAllocator::allocations > 0);
    auto const deep = ::deep_trace(96);
    pltxt2htm_test_assert_true(deep.size() > 64 && !deep.possibly_truncated());
    auto const one_entry = trace::Stacktrace::current(0, 1);
    pltxt2htm_test_assert_true(one_entry.size() == 1 && one_entry.possibly_truncated() && one_entry[0]);
    pltxt2htm_test_assert_true((one_entry <=> deep) < 0);
    // Entry queries and formatting use the saved address, not a new capture.
    auto const description = one_entry[0].description();
    auto const file = one_entry[0].source_file();
    (void)one_entry[0].source_line();
    auto const entry_text = ::fast_io::concat_fast_io(one_entry[0]);
    auto const resolved_text = ::fast_io::concat_fast_io(trace::resolve(one_entry[0].native_handle()));
    pltxt2htm_test_assert_true(entry_text == resolved_text);
    pltxt2htm_test_assert_true(one_entry.max_size() >= one_entry.size());
#endif
    auto const captured = trace::capture(storage);
    auto const resolved = trace::resolve(captured.size != 0 ? storage[0] : nullptr);
#if defined(_WIN32) && defined(PLTXT2HTM_ENABLE_STACKTRACE)
    namespace nt = ::pltxt2htm::details::symbols::nt;
    namespace win32 = ::pltxt2htm::details::symbols::win32;
    auto& session = trace::get_symbol_session();
    pltxt2htm_test_assert_true(session.ready);
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
    nt::RtlAcquireSRWLockExclusive(::std::addressof(session.lock));
    auto const busy = trace::resolve(storage[0]);
    pltxt2htm_test_assert_true(busy.address == storage[0] && busy.description.is_empty());
    pltxt2htm_test_assert_true(!trace::shutdown_symbols());
    nt::RtlReleaseSRWLockExclusive(::std::addressof(session.lock));
    auto const invalid = trace::resolve(nullptr);
    pltxt2htm_test_assert_true(invalid.address == nullptr && invalid.description.is_empty());
    pltxt2htm_test_assert_true(trace::shutdown_symbols());
    pltxt2htm_test_assert_true(!session.ready && session.process == nullptr);
    // Saved data survives resolver teardown; printing must not reinitialize it.
    auto const saved_text = ::fast_io::concat_fast_io(resolved);
    auto const repeated = ::fast_io::concat_fast_io(resolved);
    pltxt2htm_test_assert_true(saved_text == repeated);
    pltxt2htm_test_assert_true(!session.attempted);
    auto const again = trace::resolve(storage[0]);
    pltxt2htm_test_assert_true(again.address == storage[0] && session.ready);
    pltxt2htm_test_assert_true(trace::shutdown_symbols());
#endif
    // Formatting consumes the exact supplied data, not the current call stack.
    trace::ResolvedFrame synthetic{};
    synthetic.address = reinterpret_cast<void*>(1);
    synthetic.displacement = 42;
    synthetic.source_line = 7;
    synthetic.description = ::pltxt2htm::container::String{"saved_function"};
    synthetic.source_file = ::pltxt2htm::container::String{"saved.cc"};
    auto const formatted = ::fast_io::concat_fast_io("[3] ", synthetic, "\n");
    constexpr char expected[] = "[3] saved_function + 0x2a at saved.cc:7\n";
    pltxt2htm_test_assert_true(::std::strcmp(formatted.c_str(), expected) == 0);
}
