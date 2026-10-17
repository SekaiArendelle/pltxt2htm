#pragma once

#include <cstdint>
#include <memory>
#include "native_frame.hh"

#include "../symbols/nt/ntdll.hh"
#include "../symbols/win32/dbghelp.hh"

namespace pltxt2htm::details::stacktrace {
// One session per linked image. This lock only coordinates our own calls;
// Hosts using DbgHelp elsewhere must coordinate those users when enabling
// PLTXT2HTM_ENABLE_STACKTRACE. A distinct session handle does not make concurrent
// DbgHelp calls safe. No process-wide symbol options are changed; source lines
// depend on debug information and the existing DbgHelp options.
struct alignas(void*) DbgHelpSession {
    ::pltxt2htm::details::symbols::nt::SrwLock lock{};
    void* process{};
    bool attempted{};
    bool ready{};

    [[nodiscard]] bool try_initialize(this DbgHelpSession& self) noexcept {
        namespace nt = ::pltxt2htm::details::symbols::nt;
        namespace win32 = ::pltxt2htm::details::symbols::win32;
        if (self.attempted) {
            return self.ready;
        }
        self.attempted = true;
        auto const current_process = reinterpret_cast<void*>(static_cast<::std::intptr_t>(-1));
        // A distinct real handle lets DbgHelp enumerate this process without
        // sharing the GetCurrentProcess() pseudo-handle session with the host.
        if (nt::NtDuplicateObject(current_process, current_process, current_process,
                                              ::std::addressof(self.process), 0, 0, 2) < 0) {
            return false;
        }
        self.ready = win32::SymInitialize(self.process, nullptr, 1) != 0;
        return self.ready;
    }

    // Explicit cleanup avoids invoking DbgHelp while a DLL is unloading under
    // the loader lock. Call only while holding this session's lock.
    void reset(this DbgHelpSession& self) noexcept {
        namespace nt = ::pltxt2htm::details::symbols::nt;
        namespace win32 = ::pltxt2htm::details::symbols::win32;
        if (self.ready) {
            (void)win32::SymCleanup(self.process);
        }
        if (self.process != nullptr) {
            (void)nt::NtClose(self.process);
        }
        self.process = nullptr;
        self.ready = false;
        self.attempted = false;
    }
};

// Trivial destruction: retain the resolver until explicit shutdown or process
// exit, so panic during static destruction remains usable.
[[nodiscard]]
inline DbgHelpSession& get_symbol_session() noexcept {
    static constinit DbgHelpSession session{};
    return session;
}

/** Release the resolver before unloading a consumer DLL. The host
 * must also coordinate external DbgHelp users. False means a lookup is active.
 */
[[nodiscard]]
inline bool shutdown_symbols() noexcept {
    namespace nt = ::pltxt2htm::details::symbols::nt;
    auto& session = ::pltxt2htm::details::stacktrace::get_symbol_session();
    if (!nt::RtlTryAcquireSRWLockExclusive(::std::addressof(session.lock))) {
        return false;
    }
    session.reset();
    nt::RtlReleaseSRWLockExclusive(::std::addressof(session.lock));
    return true;
}

inline void resolve_description(NativeResolvedFrame& result, DbgHelpSession& session,
                                ::std::uint64_t native_address) noexcept {
    namespace win32 = ::pltxt2htm::details::symbols::win32;
    constexpr auto name_offset = offsetof(win32::SymbolInfo, name);
    ::std::size_t capacity{2000}; // DbgHelp MAX_SYM_NAME
    bool refreshed{};
    for (;;) {
        if (capacity > (::std::numeric_limits<unsigned long>::max)() ||
            capacity > (::std::numeric_limits<::std::size_t>::max)() - name_offset) {
            result.text_truncated = true;
            return;
        }
        auto const storage_size = name_offset + capacity;
        auto* storage = static_cast<unsigned char*>(::std::calloc(storage_size, 1));
        if (storage == nullptr) {
            result.text_truncated = true;
            return;
        }
        auto* info = reinterpret_cast<win32::SymbolInfo*>(storage);
        info->size_of_struct = sizeof(win32::SymbolInfo);
        info->max_name_len = static_cast<unsigned long>(capacity);
        auto const found = win32::SymFromAddr(session.process, native_address, ::std::addressof(result.displacement),
                                             info) != 0;
        auto const name_size = static_cast<::std::size_t>(info->name_len);
        if (name_size >= capacity) {
            ::std::free(storage);
            auto const max_size = (::std::numeric_limits<::std::size_t>::max)();
            auto const required = name_size == max_size ? name_size : name_size + 1;
            auto const doubled = capacity > max_size / 2 ? max_size : capacity * 2;
            auto const new_capacity = required > doubled ? required : doubled;
            if (new_capacity <= capacity) {
                result.text_truncated = true;
                return;
            }
            capacity = new_capacity;
            continue;
        }
        if (!found) {
            ::std::free(storage);
            if (!refreshed && win32::SymRefreshModuleList(session.process)) {
                refreshed = true;
                continue;
            }
            return;
        }
        if (!result.description.assign(reinterpret_cast<char const*>(storage + name_offset), name_size)) {
            result.text_truncated = true;
        }
        ::std::free(storage);
        return;
    }
}

[[nodiscard]]
inline NativeResolvedFrame resolve_native(void* address) noexcept {
    namespace nt = ::pltxt2htm::details::symbols::nt;
    NativeResolvedFrame result{.address = address};
    auto& session = ::pltxt2htm::details::stacktrace::get_symbol_session();
    if (!nt::RtlTryAcquireSRWLockExclusive(::std::addressof(session.lock))) {
        return result;
    }
    if (session.try_initialize()) {
        auto const native_address = static_cast<::std::uint64_t>(reinterpret_cast<::std::uintptr_t>(address));
        ::pltxt2htm::details::stacktrace::resolve_description(result, session, native_address);
        // File/line lookup is independent of function-name lookup.
        ::pltxt2htm::details::symbols::win32::ImageHlpLine64 line{};
        line.size_of_struct = sizeof(line);
        unsigned long line_displacement{};
        if (::pltxt2htm::details::symbols::win32::SymGetLineFromAddr64(
                session.process, native_address, ::std::addressof(line_displacement), ::std::addressof(line)) &&
            line.file_name != nullptr) {
            result.text_truncated = !::pltxt2htm::details::stacktrace::assign_text(result.source_file, line.file_name) ||
                                    result.text_truncated;
            result.source_line = line.line_number;
        }
    }
    nt::RtlReleaseSRWLockExclusive(::std::addressof(session.lock));
    return result;
}
} // namespace pltxt2htm::details::stacktrace
