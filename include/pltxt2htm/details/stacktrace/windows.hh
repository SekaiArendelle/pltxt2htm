#pragma once

#include <cstdint>
#include <memory>
#include "native_frame.hh"

#include "../symbols/nt/ntdll.hh"
#include "../symbols/nt/kernel32.hh"
#include "../symbols/nt/dbghelp.hh"

namespace pltxt2htm::details::stacktrace {
// One session per linked image. This lock only coordinates our own calls;
// Hosts using DbgHelp elsewhere must coordinate those users when enabling
// PLTXT2HTM_ENABLE_STACKTRACE. A distinct session handle does not make concurrent
// DbgHelp calls safe. No process-wide symbol options are changed; source lines
// depend on debug information and the existing DbgHelp options.
struct alignas(void*) DbgHelpSession {
    void* lock{};
    void* library{};
    void* process{};
    ::pltxt2htm::details::symbols::nt::SymInitialize initialize{};
    ::pltxt2htm::details::symbols::nt::SymCleanup cleanup{};
    ::pltxt2htm::details::symbols::nt::SymRefreshModuleList refresh{};
    ::pltxt2htm::details::symbols::nt::SymFromAddr from_addr{};
    ::pltxt2htm::details::symbols::nt::SymGetLineFromAddr64 get_line{};
    bool attempted{};
    bool ready{};

    [[nodiscard]] bool try_initialize(this DbgHelpSession& self) noexcept {
        namespace nt = ::pltxt2htm::details::symbols::nt;
        if (self.attempted) {
            return self.ready;
        }
        self.attempted = true;
        self.library = nt::pltxt2htm_nt_load_library_ex_w(L"dbghelp.dll", nullptr, 0x00000800); // SYSTEM32
        if (self.library == nullptr) {
            return false;
        }
        self.initialize =
            reinterpret_cast<nt::SymInitialize>(nt::pltxt2htm_nt_get_proc_address(self.library, "SymInitialize"));
        self.cleanup = reinterpret_cast<nt::SymCleanup>(nt::pltxt2htm_nt_get_proc_address(self.library, "SymCleanup"));
        self.refresh = reinterpret_cast<nt::SymRefreshModuleList>(
            nt::pltxt2htm_nt_get_proc_address(self.library, "SymRefreshModuleList"));
        self.from_addr =
            reinterpret_cast<nt::SymFromAddr>(nt::pltxt2htm_nt_get_proc_address(self.library, "SymFromAddr"));
        self.get_line = reinterpret_cast<nt::SymGetLineFromAddr64>(
            nt::pltxt2htm_nt_get_proc_address(self.library, "SymGetLineFromAddr64"));
        if (self.initialize == nullptr || self.cleanup == nullptr || self.from_addr == nullptr) {
            return false;
        }
        auto const current_process = reinterpret_cast<void*>(static_cast<::std::intptr_t>(-1));
        // A distinct real handle lets DbgHelp enumerate this process without
        // sharing the GetCurrentProcess() pseudo-handle session with the host.
        if (nt::pltxt2htm_nt_duplicate_object(current_process, current_process, current_process,
                                              ::std::addressof(self.process), 0, 0, 2) < 0) {
            return false;
        }
        self.ready = self.initialize(self.process, nullptr, 1) != 0;
        return self.ready;
    }

    // Explicit cleanup avoids invoking DbgHelp while a DLL is unloading under
    // the loader lock. Call only while holding this session's lock.
    void reset(this DbgHelpSession& self) noexcept {
        namespace nt = ::pltxt2htm::details::symbols::nt;
        if (self.ready) {
            (void)self.cleanup(self.process);
        }
        if (self.process != nullptr) {
            (void)nt::pltxt2htm_nt_close(self.process);
        }
        if (self.library != nullptr) {
            (void)nt::pltxt2htm_nt_free_library(self.library);
        }
        self.library = nullptr;
        self.process = nullptr;
        self.initialize = nullptr;
        self.cleanup = nullptr;
        self.refresh = nullptr;
        self.from_addr = nullptr;
        self.get_line = nullptr;
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
    if (!nt::pltxt2htm_nt_try_acquire_srw_lock_exclusive(::std::addressof(session.lock))) {
        return false;
    }
    session.reset();
    nt::pltxt2htm_nt_release_srw_lock_exclusive(::std::addressof(session.lock));
    return true;
}

inline void resolve_description(NativeResolvedFrame& result, DbgHelpSession& session,
                                ::std::uint64_t native_address) noexcept {
    namespace nt = ::pltxt2htm::details::symbols::nt;
    constexpr auto name_offset = offsetof(nt::SymbolInfo, name);
    ::std::size_t capacity{nt::max_symbol_name_length};
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
        auto* info = reinterpret_cast<nt::SymbolInfo*>(storage);
        info->size_of_struct = sizeof(nt::SymbolInfo);
        info->max_name_len = static_cast<unsigned long>(capacity);
        auto const found = session.from_addr(session.process, native_address, ::std::addressof(result.displacement),
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
            if (!refreshed && session.refresh != nullptr && session.refresh(session.process)) {
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
    if (!nt::pltxt2htm_nt_try_acquire_srw_lock_exclusive(::std::addressof(session.lock))) {
        return result;
    }
    if (session.try_initialize()) {
        auto const native_address = static_cast<::std::uint64_t>(reinterpret_cast<::std::uintptr_t>(address));
        ::pltxt2htm::details::stacktrace::resolve_description(result, session, native_address);
        // File/line lookup is independent of function-name lookup.
        nt::ImageHlpLine64 line{};
        line.size_of_struct = sizeof(line);
        unsigned long line_displacement{};
        if (session.get_line != nullptr &&
            session.get_line(session.process, native_address, ::std::addressof(line_displacement),
                             ::std::addressof(line)) &&
            line.file_name != nullptr) {
            result.text_truncated = !::pltxt2htm::details::stacktrace::assign_text(result.source_file, line.file_name) ||
                                    result.text_truncated;
            result.source_line = line.line_number;
        }
    }
    nt::pltxt2htm_nt_release_srw_lock_exclusive(::std::addressof(session.lock));
    return result;
}
} // namespace pltxt2htm::details::stacktrace
