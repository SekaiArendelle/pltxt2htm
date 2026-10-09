#include <pltxt2htm/details/stacktrace/stacktrace.hh>
#include <pltxt2htm/details/stacktrace/resolve.hh>
#include <pltxt2htm/details/stacktrace/format.hh>
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
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(__linux__) && __has_include(<execinfo.h>) && __has_include(<cxxabi.h>)
    char symbol[] = "demo(_Z9demo_leafv+0x66) [0x1234]";
    trace::NativeResolvedFrame decoded{};
    trace::resolve_symbol_text(decoded, symbol);
    pltxt2htm_test_assert_true(::std::strcmp(decoded.description.c_str(), "demo_leaf") == 0);
    pltxt2htm_test_assert_true(decoded.displacement == 0x66);
    pltxt2htm_test_assert_true(::fast_io::concat_fast_io(trace::own_frame(decoded)) == "demo_leaf + 0x66 in demo");
    pltxt2htm_test_assert_true(::std::strcmp(symbol, "demo(_Z9demo_leafv+0x66) [0x1234]") == 0);
    char plain[] = "libc(__libc_start_main+0x89) [0x1234]";
    trace::NativeResolvedFrame c_symbol{};
    trace::resolve_symbol_text(c_symbol, plain);
    pltxt2htm_test_assert_true(::fast_io::concat_fast_io(trace::own_frame(c_symbol)) == "__libc_start_main + 0x89 in libc");
    char invalid[] = "demo(_Zinvalid+0x1) [0x1234]";
    trace::NativeResolvedFrame invalid_symbol{};
    trace::resolve_symbol_text(invalid_symbol, invalid);
    pltxt2htm_test_assert_true(::fast_io::concat_fast_io(trace::own_frame(invalid_symbol)) == "_Zinvalid + 0x1 in demo");
    char unknown[] = "libc(+0x27781) [0x1234]";
    trace::NativeResolvedFrame unnamed{.address = reinterpret_cast<void*>(0x1234)};
    trace::resolve_symbol_text(unnamed, unknown);
    pltxt2htm_test_assert_true(::fast_io::concat_fast_io(trace::own_frame(unnamed)) == "0x1234 in libc");
    char malformed[] = "demo(main+0x10000000000000000) [0x1234]";
    trace::resolve_symbol_text(unnamed, malformed);
    pltxt2htm_test_assert_true(::fast_io::concat_fast_io(trace::own_frame(unnamed)) == "0x1234 in demo");
    char address_only[] = "no-symbol-module [0x1234]";
    trace::NativeResolvedFrame address_frame{.address = reinterpret_cast<void*>(0x1234)};
    trace::resolve_symbol_text(address_frame, address_only);
    pltxt2htm_test_assert_true(::fast_io::concat_fast_io(trace::own_frame(address_frame)) == "0x1234 in no-symbol-module");
    pltxt2htm_test_assert_true(::std::strcmp(address_only, "no-symbol-module [0x1234]") == 0);
    char call_operator[] = "Functor::operator()(int (*)(double)) const";
    trace::remove_symbol_parameters(call_operator);
    pltxt2htm_test_assert_true(::std::strcmp(call_operator, "Functor::operator()") == 0);
    char templated[] = "example<void (*)(int)>(double)";
    trace::remove_symbol_parameters(templated);
    pltxt2htm_test_assert_true(::std::strcmp(templated, "example<void (*)(int)>") == 0);
    char pointer_return[] = "void (*make_callback<int>())(double)";
    trace::remove_symbol_parameters(pointer_return);
    pltxt2htm_test_assert_true(::std::strcmp(pointer_return, "void (*make_callback<int>())(double)") == 0);
    char long_symbol[1200]{};
    ::std::memcpy(long_symbol, "demo(", 5);
    ::std::memset(long_symbol + 5, 'a', 1100);
    ::std::memcpy(long_symbol + 1105, "+0x2) [0x1234]", 15);
    trace::NativeResolvedFrame long_frame{};
    trace::resolve_symbol_text(long_frame, long_symbol);
    pltxt2htm_test_assert_true(!long_frame.text_truncated && long_frame.displacement == 2);
    pltxt2htm_test_assert_true(long_frame.description.size() == 1100);
    #if defined(PLTXT2HTM_DETAIL_STACKTRACE_HAS_LIBDWFL)
    auto const dwarf_trace = trace::Stacktrace::current(0, 1);
    auto const source_frame = trace::resolve(dwarf_trace[0].native_handle());
    pltxt2htm_test_assert_true(!source_frame.source_file.is_empty() && source_frame.source_line > 0);
    pltxt2htm_test_assert_true(!source_frame.module_file.is_empty());
    auto const owned_source = dwarf_trace[0].source_file();
    pltxt2htm_test_assert_true(::std::strcmp(owned_source.c_str(), source_frame.source_file.c_str()) == 0);
    #endif
#endif
    void* storage[8]{};
    auto const empty = trace::capture({});
    pltxt2htm_test_assert_true(empty.size == 0 && !empty.possibly_truncated);
    auto const skipped = trace::capture(storage, (::std::numeric_limits<::std::size_t>::max)());
    pltxt2htm_test_assert_true(skipped.size == 0);
    void* guarded[3]{nullptr, nullptr, reinterpret_cast<void*>(1)};
    auto const single = trace::capture({guarded, 1});
    pltxt2htm_test_assert_true(guarded[1] == nullptr && guarded[2] == reinterpret_cast<void*>(1));
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && (defined(_WIN32) || (defined(__linux__) && __has_include(<execinfo.h>)))
    pltxt2htm_test_assert_true(single.size == 1 && single.possibly_truncated);
    pltxt2htm_test_assert_true(guarded[0] != nullptr);
    // Capacity is the number retained after skipping, on every native backend.
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
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && (defined(_WIN32) || (defined(__linux__) && __has_include(<execinfo.h>)))
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
    auto& session = trace::get_symbol_session();
    pltxt2htm_test_assert_true(session.ready);
    // Resolve a module loaded after DbgHelp initialized its module list.
    auto* late_module = nt::pltxt2htm_nt_load_library_ex_w(L"winmm.dll", nullptr, 0x00000800);
    pltxt2htm_test_assert_true(late_module != nullptr);
    auto const late_function = nt::pltxt2htm_nt_get_proc_address(late_module, "PlaySoundW");
    pltxt2htm_test_assert_true(late_function != nullptr);
    auto const late_frame = trace::resolve(reinterpret_cast<void*>(late_function));
    pltxt2htm_test_assert_true(!late_frame.description.is_empty());
    (void)nt::pltxt2htm_nt_free_library(late_module);
    // Exercise contention fallback without depending on another thread's timing.
    nt::pltxt2htm_nt_acquire_srw_lock_exclusive(::std::addressof(session.lock));
    auto const busy = trace::resolve(storage[0]);
    pltxt2htm_test_assert_true(busy.address == storage[0] && busy.description.is_empty());
    pltxt2htm_test_assert_true(!trace::shutdown_symbols());
    nt::pltxt2htm_nt_release_srw_lock_exclusive(::std::addressof(session.lock));
    auto const invalid = trace::resolve(nullptr);
    pltxt2htm_test_assert_true(invalid.address == nullptr && invalid.description.is_empty());
    pltxt2htm_test_assert_true(trace::shutdown_symbols());
    pltxt2htm_test_assert_true(!session.ready && session.process == nullptr && session.library == nullptr);
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
    synthetic.module_file = ::pltxt2htm::container::String{"demo.so"};
    pltxt2htm_test_assert_true(::fast_io::concat_fast_io("[3] ", synthetic, "\n") == formatted);
    trace::NativeResolvedFrame source{};
    trace::assign_source_location(source, "tests/demo.cc", "/workspace", 42);
    pltxt2htm_test_assert_true(::std::strcmp(source.source_file.c_str(), "/workspace/tests/demo.cc") == 0 &&
                               source.source_line == 42);
    trace::assign_source_location(source, "demo.cc", "/workspace/", 0);
    pltxt2htm_test_assert_true(::std::strcmp(source.source_file.c_str(), "/workspace/demo.cc") == 0 && source.source_line == 0);
    trace::assign_source_location(source, "/other/demo.cc", "/workspace", -1);
    pltxt2htm_test_assert_true(::std::strcmp(source.source_file.c_str(), "/other/demo.cc") == 0 && source.source_line == 0);
    char long_directory[2100]{};
    ::std::memset(long_directory, 'x', sizeof(long_directory) - 1);
    trace::assign_source_location(source, "demo.cc", long_directory, 7);
    pltxt2htm_test_assert_true(!source.text_truncated && source.source_file.size() == 2107);
}
