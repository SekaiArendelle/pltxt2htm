#include <pltxt2htm/stacktrace/stacktrace.hh>
#include <pltxt2htm/stacktrace/stacktrace_entry.hh>
#if defined(_WIN32)
    #include <pltxt2htm/sys/win32/kernel32.hh>
#endif
#include <fast_io/fast_io_dsal/string.h>
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

int main() {
    namespace trace = ::pltxt2htm::stacktrace;
    void* storage[8]{};
    auto const empty = trace::details::capture({});
    pltxt2htm_test_assert_true(empty.size == 0 && !empty.possibly_truncated);
    auto const skipped = trace::details::capture(storage, (::std::numeric_limits<::std::size_t>::max)());
    pltxt2htm_test_assert_true(skipped.size == 0);
    void* guarded[3]{nullptr, nullptr, reinterpret_cast<void*>(1)};
    auto const single = trace::details::capture({guarded, 1});
    pltxt2htm_test_assert_true(guarded[1] == nullptr && guarded[2] == reinterpret_cast<void*>(1));
#if defined(_WIN32)
    pltxt2htm_test_assert_true(single.size == 1 && single.possibly_truncated);
    pltxt2htm_test_assert_true(guarded[0] != nullptr);
    // Capacity is the number retained after skipping.
    auto const after_skip = trace::details::capture({guarded, 1}, 1);
    pltxt2htm_test_assert_true(after_skip.size == 1 && guarded[1] == nullptr);
    auto const full = trace::details::capture(storage);
    pltxt2htm_test_assert_true(full.size > 0 && full.size <= 8);
#else
    pltxt2htm_test_assert_true(single.size == 0 && !single.possibly_truncated);
#endif
    static_assert(::std::random_access_iterator<trace::Stacktrace::const_iterator>);
    static_assert(
        ::std::same_as<decltype(::std::declval<trace::Stacktrace const&>()[0]), trace::StacktraceEntry const&>);
    constexpr trace::StacktraceEntry empty_entry{};
    static_assert(!empty_entry && empty_entry.native_handle() == nullptr);
    pltxt2htm_test_assert_true(empty_entry.description().empty());
    pltxt2htm_test_assert_true(empty_entry.source_file().empty() && empty_entry.source_line() == 0);
    {
        auto const empty_frame = empty_entry.resolved();
        pltxt2htm_test_assert_true(empty_frame.address == nullptr && empty_frame.description.empty() &&
                                   empty_frame.source_file.empty() && empty_frame.source_line == 0 &&
                                   !empty_frame.text_truncated && empty_frame.displacement == 0);
    }
    {
        trace::Text text{};
        pltxt2htm_test_assert_true(text.empty() && text.size() == 0 && text.data()[0] == '\0');
        pltxt2htm_test_assert_true(text.assign("alpha", 5) && text.size() == 5 && text.data()[0] == 'a');
        text.clear();
        pltxt2htm_test_assert_true(text.empty() && text.data()[0] == '\0');
        pltxt2htm_test_assert_true(text.assign(nullptr) && text.empty());
        pltxt2htm_test_assert_true(text.assign("a\0b", 3) && text.size() == 3 && text.data()[1] == '\0');
        // Assigning from the text's own storage must not read freed memory.
        pltxt2htm_test_assert_true(text.assign(text.data(), 3) && text.size() == 3 && text.data()[0] == 'a');
        auto moved = ::std::move(text);
        pltxt2htm_test_assert_true(text.empty() && moved.size() == 3);
        text = ::std::move(moved);
        pltxt2htm_test_assert_true(moved.empty() && text.size() == 3);
        pltxt2htm_test_assert_true(text.size() == 3);
    }
    auto const snapshot = trace::Stacktrace::current(0, 8);
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
        auto tracked = TrackedTrace::current(0, 8);
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
    auto const one_entry = trace::Stacktrace::current(0, 1);
    pltxt2htm_test_assert_true(one_entry.size() == 1 && one_entry.possibly_truncated() && one_entry[0]);
    pltxt2htm_test_assert_true((one_entry <=> deep) < 0);
    // Entry queries use the saved address, not a new capture.
    auto const description = one_entry[0].description();
    auto const file = one_entry[0].source_file();
    (void)one_entry[0].source_line();
    pltxt2htm_test_assert_true(one_entry.max_size() >= one_entry.size());
#endif
#if defined(_WIN32)
    namespace nt = ::pltxt2htm::sys_nt;
    namespace win32 = ::pltxt2htm::sys_win32;
    auto& session = trace::details::get_symbol_session();
    pltxt2htm_test_assert_true(session.ready);
    pltxt2htm_test_assert_true((win32::SymGetOptions() & 0x00000010) != 0);
    // Resolve a module loaded after DbgHelp initialized its module list.
    auto* late_module = win32::LoadLibraryExW(L"winmm.dll", nullptr, 0x00000800);
    pltxt2htm_test_assert_true(late_module != nullptr);
    auto const late_function = win32::GetProcAddress(late_module, "PlaySoundW");
    pltxt2htm_test_assert_true(late_function != nullptr);
    auto const late_frame = trace::resolve(reinterpret_cast<void*>(late_function));
    pltxt2htm_test_assert_true(!late_frame.description.empty());
    (void)win32::FreeLibrary(late_module);
    // Exercise contention fallback without depending on another thread's timing.
    nt::RtlAcquireSRWLockExclusive(::std::addressof(session.lock));
    auto const busy = trace::resolve(storage[0]);
    pltxt2htm_test_assert_true(busy.address == storage[0] && busy.description.empty());
    pltxt2htm_test_assert_true(!trace::details::shutdown_symbols());
    nt::RtlReleaseSRWLockExclusive(::std::addressof(session.lock));
    auto const invalid = trace::resolve(nullptr);
    pltxt2htm_test_assert_true(invalid.address == nullptr && invalid.description.empty());
    pltxt2htm_test_assert_true(trace::details::shutdown_symbols());
    pltxt2htm_test_assert_true(!session.ready && session.process == nullptr);
    auto const again = trace::resolve(storage[0]);
    pltxt2htm_test_assert_true(again.address == storage[0] && session.ready);
    pltxt2htm_test_assert_true(trace::details::shutdown_symbols());
#endif
}
