#pragma once

#if defined(_WIN32)
    #include <pltxt2htm/sys/win32/kernel32.hh>
#endif

#include "doctest_config.hh"
#include "details/helpers.hh"

template<typename Backend>
void test_nt_stacktrace_backend() {
    using Entry = typename Backend::entry;
    using NativeHandle = typename Entry::native_handle_type;
    ::test_backend_capture_contract<Backend>();
    {
        // The entry keeps the native handle for zero-copy capture.
        static_assert(::std::same_as<NativeHandle, void*>);
        constexpr Entry empty_entry{};
        static_assert(!empty_entry && empty_entry.native_handle() == NativeHandle{});
        // Internal-use entry construction from native handles.
        int first_object{};
        auto const first_handle = reinterpret_cast<NativeHandle>(::std::addressof(first_object));
        auto const first_entry = Entry::from_native_handle(first_handle);
        CHECK(first_entry.native_handle() == first_handle);
        CHECK(static_cast<bool>(first_entry));
        CHECK(!Entry{});
    }
    {
        // Deep capture: entries, comparison and saved addresses.
        using BackendTrace = ::pltxt2htm::stacktrace::details::BasicStacktrace<Backend>;
        auto const deep = ::deep_trace<BackendTrace>(96);
        REQUIRE(deep.size() > 64);
        auto const deep_order = BackendTrace{} <=> deep;
        CHECK(deep_order == ::std::strong_ordering::less);
        CHECK(!deep.is_empty());
        CHECK(static_cast<bool>(deep[0]));
        // Entry queries use the saved address, not a new capture.
        auto const first_frame = deep[0].resolved();
        CHECK(reinterpret_cast<void*>(first_frame.address) == deep[0].native_handle());
        // DbgHelp must reach the image's symbols (PDB or, for GNU-ABI builds,
        // the export table) for frames inside the test executable.
        CHECK(!first_frame.description.is_empty());
        CHECK(deep.max_size() >= deep.size());
    }
    {
        // std::stacktrace semantics: entry zero of current(0) is the caller of
        // current(), and skip shifts the trace by exactly one entry.
        using BackendTrace = ::pltxt2htm::stacktrace::details::BasicStacktrace<Backend>;
        auto const from_here = ::trace_from_named_frame<BackendTrace>(0);
        auto const one_skipped = ::trace_from_named_frame<BackendTrace>(1);
        REQUIRE(from_here.size() > 1);
        CHECK(one_skipped.size() + 1 == from_here.size());
        auto const from_here_name = from_here[0].resolved().description;
        ::std::string_view const from_here_view{from_here_name.data(), from_here_name.size()};
        CHECK(from_here_view.find("trace_from_named_frame") != ::std::string_view::npos);
    }
#if defined(__GXX_ABI_VERSION) && !defined(_MSC_VER) && __has_include(<cxxabi.h>)
    {
        // GNU-ABI builds receive Itanium-mangled export names, which DbgHelp
        // reports without the leading underscore; the demangler must turn
        // deep_trace's name into readable text instead of "Z10deep_trace...".
        using BackendTrace = ::pltxt2htm::stacktrace::details::BasicStacktrace<Backend>;
        auto const deep_trace_handle = reinterpret_cast<NativeHandle>(::std::addressof(::deep_trace<BackendTrace>));
        auto const named = Entry::from_native_handle(deep_trace_handle).resolved();
        REQUIRE(!named.description.is_empty());
        CHECK(named.description.data()[0] != u8'Z');
    }
#endif
}

#if defined(_WIN32)
template<typename Backend>
void test_nt_symbol_machinery() {
    namespace win32 = ::pltxt2htm::sys_win32;
    namespace details = ::pltxt2htm::stacktrace::details;
    using Entry = typename Backend::entry;
    using NativeHandle = typename Entry::native_handle_type;
    ::TraceFrames<Entry> frames{};
    Backend::capture::template capture<::fast_io::native_global_allocator>(frames, 0);
    REQUIRE(!frames.is_empty());
    auto const sample_handle = frames[0].native_handle();
    {
        // Symbol session is ready with line information enabled.
        auto& session = details::get_symbol_session();
        REQUIRE(session.is_ready());
        CHECK((win32::SymGetOptions() & 0x00000010) != 0);
        // A module loaded after DbgHelp initialized its module list resolves.
        auto late_module = win32::LoadLibraryExW(L"winmm.dll", win32::Handle{}, 0x00000800);
        REQUIRE(!late_module.is_null());
        auto const late_function = win32::GetProcAddress(late_module, "PlaySoundW");
        REQUIRE(late_function != nullptr);
        auto const late_frame = Entry::from_native_handle(reinterpret_cast<NativeHandle>(late_function)).resolved();
        CHECK(!late_frame.description.is_empty());
        (void)win32::FreeLibrary(late_module);
    }

    {
        // A real lookup leaves the input fields the storage relies on untouched,
        // so a reused record never needs them re-seeded before being freed.
        auto& session = details::get_symbol_session();
        auto held = details::DbgHelpLock::try_acquire();
        REQUIRE(static_cast<bool>(held));
        details::SymbolInfoStorage looked_up{};
        ::std::uint64_t displacement{};
        [[maybe_unused]] auto const found = ::pltxt2htm::sys_win32::SymFromAddr(
            session.process_handle(), reinterpret_cast<::pltxt2htm::sys_win32::Dword64>(sample_handle),
            ::std::addressof(displacement), looked_up.symbol_info());
        auto* const looked_up_info = looked_up.symbol_info();
        CHECK(looked_up_info->size_of_struct == sizeof(::pltxt2htm::sys_win32::SymbolInfo));
        CHECK(looked_up_info->max_name_len == details::SymbolInfoStorage::default_name_capacity);
        // A failed lookup reports no displacement, so a frame without a symbol
        // never prints a stale offset.
        auto const unresolved = Entry::from_native_handle(reinterpret_cast<NativeHandle>(1)).resolved();
        CHECK(unresolved.description.is_empty());
        CHECK(unresolved.displacement == 0);
    }

    {
        // A held process-level DbgHelp lock makes resolution fall back instead
        // of blocking, and the same gate denies shutdown and re-acquisition.
        auto& session = details::get_symbol_session();
        auto held = details::DbgHelpLock::try_acquire();
        REQUIRE(static_cast<bool>(held));
        auto const busy = Entry::from_native_handle(sample_handle).resolved();
        CHECK(reinterpret_cast<void*>(busy.address) == sample_handle);
        CHECK(busy.description.is_empty());
        CHECK(!session.try_shutdown());
        CHECK(session.is_ready());
        CHECK(!session.process_handle().is_null());
    }

    {
        // Invalid addresses stay unresolved; shutdown is followed by a lazy
        // re-initialization on the next lookup.
        auto& session = details::get_symbol_session();
        auto const invalid = Entry{}.resolved();
        CHECK(invalid.address == 0);
        CHECK(invalid.description.is_empty());
        CHECK(session.try_shutdown());
        CHECK(!session.is_ready());
        CHECK(session.process_handle().is_null());
        auto const again = Entry::from_native_handle(sample_handle).resolved();
        CHECK(reinterpret_cast<void*>(again.address) == sample_handle);
        CHECK(session.is_ready());
        CHECK(session.try_shutdown());
    }

    {
        // A recorded initialization failure is not retried by later lookups.
        auto& session = details::get_symbol_session();
        session.force_unavailable();
        auto const unavailable = Entry::from_native_handle(sample_handle).resolved();
        CHECK(reinterpret_cast<void*>(unavailable.address) == sample_handle);
        CHECK(unavailable.description.is_empty());
        CHECK(session.process_handle().is_null());
        CHECK(!session.is_ready());
        CHECK(session.try_shutdown());
    }

    {
        // The storage owns SYMBOL_INFO's trailing one-character array: the name
        // buffer is zero-filled and terminated, and the fields DbgHelp reads stay
        // in sync.
        details::SymbolInfoStorage storage{};
        CHECK(storage.is_valid());
        CHECK(storage.name_capacity() == details::SymbolInfoStorage::default_name_capacity);
        auto* const info = storage.symbol_info();
        CHECK(info->size_of_struct == sizeof(::pltxt2htm::sys_win32::SymbolInfo));
        CHECK(info->name_len == 0);
        CHECK(storage.name().is_empty());
        // A lookup may fill the buffer without terminating it; the zero-filled
        // allocation supplies the byte after the reported length.
        constexpr char sample[] = "deep_trace";
        constexpr auto sample_length = sizeof(sample) - 1;
        auto* const written_name = static_cast<char*>(static_cast<void*>(info->name));
        for (::std::size_t index{}; index != sample_length; ++index) {
            written_name[index] = sample[index];
        }
        info->name_len = static_cast<decltype(info->name_len)>(sample_length);
        auto const written = storage.name();
        CHECK(written.size() == sample_length);
        CHECK(::std::string_view{written.data(), written.size()} == "deep_trace");
        CHECK(written.c_str()[sample_length] == '\0');
        // A name filling every reserved byte but the terminator still fits.
        info->name_len = static_cast<decltype(info->name_len)>(storage.name_capacity() - 1);
        CHECK(!storage.is_name_truncated());
        // A full buffer is truncated and grows, doubling the capacity.
        info->name_len = static_cast<decltype(info->name_len)>(storage.name_capacity());
        CHECK(storage.is_name_truncated());
        CHECK(storage.grow());
        CHECK(storage.name_capacity() == details::SymbolInfoStorage::default_name_capacity * 2);
        CHECK(storage.symbol_info()->name_len == 0);
        CHECK(storage.name().is_empty());
        CHECK(!storage.is_name_truncated());
        // Growth takes the reported length when it exceeds the doubled capacity.
        details::SymbolInfoStorage small{4};
        small.symbol_info()->name_len = 100;
        CHECK(small.grow());
        CHECK(small.name_capacity() == 101);
        // A growth that cannot be represented keeps the current record.
        auto* const kept = small.symbol_info();
        small.symbol_info()->name_len = (::std::numeric_limits<::pltxt2htm::sys_win32::Ulong>::max)();
        CHECK(!small.grow());
        CHECK(small.symbol_info() == kept);
        CHECK(small.name_capacity() == 101);
        // Each lookup starts from an empty reported name length again.
        small.reset_for_lookup();
        CHECK(small.symbol_info()->name_len == 0);
        // A capacity DbgHelp's length field cannot represent leaves the storage
        // invalid instead of allocating.
        if constexpr (sizeof(::std::size_t) > sizeof(::pltxt2htm::sys_win32::Ulong)) {
            details::SymbolInfoStorage unrepresentable{
                static_cast<::std::size_t>((::std::numeric_limits<::pltxt2htm::sys_win32::Ulong>::max)()) + 1};
            CHECK(!unrepresentable.is_valid());
            CHECK(!unrepresentable.grow());
        }
    }
}
#endif

TEST_CASE("nt-backend-capture-and-entry") {
    namespace details = ::pltxt2htm::stacktrace::details;
    if constexpr (details::NtBackend::capture::supported) {
        test_nt_stacktrace_backend<details::NtBackend>();
    }
}

#if defined(_WIN32)
TEST_CASE("nt-symbol-machinery") {
    namespace details = ::pltxt2htm::stacktrace::details;
    if constexpr (details::NtBackend::capture::supported) {
        test_nt_symbol_machinery<details::NtBackend>();
    }
}
#endif
