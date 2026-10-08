#include <pltxt2htm/details/stacktrace/stacktrace.hh>
#include <pltxt2htm/details/stacktrace/resolve.hh>
#include <pltxt2htm/details/stacktrace/format.hh>
#include <fast_io/fast_io_dsal/string.h>
#include <cstring>
#include "precompile.hh"
#include <limits>
#include <memory>

int main() {
    namespace trace = ::pltxt2htm::details::stacktrace;
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(__linux__) && __has_include(<execinfo.h>) && __has_include(<cxxabi.h>)
    char symbol[] = "demo(_Z9demo_leafv+0x66) [0x1234]";
    trace::ResolvedFrame decoded{};
    trace::resolve_symbol_text(decoded, symbol);
    pltxt2htm_test_assert_true(::std::strcmp(decoded.description, "demo_leaf") == 0);
    pltxt2htm_test_assert_true(decoded.displacement == 0x66);
    pltxt2htm_test_assert_true(::fast_io::concat_fast_io(decoded) == "demo_leaf + 0x66");
    pltxt2htm_test_assert_true(::std::strcmp(symbol, "demo(_Z9demo_leafv+0x66) [0x1234]") == 0);
    char plain[] = "libc(__libc_start_main+0x89) [0x1234]";
    trace::ResolvedFrame c_symbol{};
    trace::resolve_symbol_text(c_symbol, plain);
    pltxt2htm_test_assert_true(::fast_io::concat_fast_io(c_symbol) == "__libc_start_main + 0x89");
    char invalid[] = "demo(_Zinvalid+0x1) [0x1234]";
    trace::ResolvedFrame invalid_symbol{};
    trace::resolve_symbol_text(invalid_symbol, invalid);
    pltxt2htm_test_assert_true(::fast_io::concat_fast_io(invalid_symbol) == "_Zinvalid + 0x1");
    char unknown[] = "libc(+0x27781) [0x1234]";
    trace::ResolvedFrame unnamed{.address = reinterpret_cast<void*>(0x1234)};
    trace::resolve_symbol_text(unnamed, unknown);
    pltxt2htm_test_assert_true(::fast_io::concat_fast_io(unnamed) == "0x1234");
    char malformed[] = "demo(main+0x10000000000000000) [0x1234]";
    trace::resolve_symbol_text(unnamed, malformed);
    pltxt2htm_test_assert_true(::fast_io::concat_fast_io(unnamed) == "0x1234");
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
    trace::ResolvedFrame long_frame{};
    trace::resolve_symbol_text(long_frame, long_symbol);
    pltxt2htm_test_assert_true(long_frame.text_truncated && long_frame.displacement == 2);
    pltxt2htm_test_assert_true(::std::strlen(long_frame.description) == sizeof(long_frame.description) - 1);
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
    auto const snapshot = trace::Stacktrace<8>::current();
    auto const copied_snapshot = snapshot;
    pltxt2htm_test_assert_true(snapshot.size() == copied_snapshot.size());
    for (::std::size_t i = 0; i < snapshot.size(); ++i) {
        pltxt2htm_test_assert_true(snapshot.addresses()[i] == copied_snapshot.addresses()[i]);
    }
    pltxt2htm_test_assert_true(trace::Stacktrace<0>::current().empty());
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
    pltxt2htm_test_assert_true(late_frame.description[0] != '\0');
    (void)nt::pltxt2htm_nt_free_library(late_module);
    // Exercise contention fallback without depending on another thread's timing.
    nt::pltxt2htm_nt_acquire_srw_lock_exclusive(::std::addressof(session.lock));
    auto const busy = trace::resolve(storage[0]);
    pltxt2htm_test_assert_true(busy.address == storage[0] && busy.description[0] == '\0');
    pltxt2htm_test_assert_true(!trace::shutdown_symbols());
    nt::pltxt2htm_nt_release_srw_lock_exclusive(::std::addressof(session.lock));
    auto const invalid = trace::resolve(nullptr);
    pltxt2htm_test_assert_true(invalid.address == nullptr && invalid.description[0] == '\0');
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
    pltxt2htm_test_assert_true(!trace::copy_text(synthetic.description, "saved_function"));
    pltxt2htm_test_assert_true(!trace::copy_text(synthetic.source_file, "saved.cc"));
    char bounded[4]{};
    pltxt2htm_test_assert_true(trace::copy_text(bounded, "abcdef"));
    pltxt2htm_test_assert_true(::std::strcmp(bounded, "abc") == 0);
    trace::ResolvedFrame unterminated{};
    for (auto& ch : unterminated.description) {
        ch = 'x';
    }
    auto const bounded_format = ::fast_io::concat_fast_io(unterminated);
    pltxt2htm_test_assert_true(bounded_format.size() == sizeof(unterminated.description));
    auto const formatted = ::fast_io::concat_fast_io("[3] ", synthetic, "\n");
    constexpr char expected[] = "[3] saved_function + 0x2a at saved.cc:7\n";
    pltxt2htm_test_assert_true(::std::strcmp(formatted.c_str(), expected) == 0);
}
