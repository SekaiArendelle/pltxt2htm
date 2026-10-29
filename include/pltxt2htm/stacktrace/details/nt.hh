/**
 * @file nt.hh
 * @brief Windows DbgHelp symbol resolution for the stacktrace implementation.
 * @details The resolver is process-wide and lazily initialized; hosts using
 *          DbgHelp directly must coordinate with it.
 */

#pragma once

#include <cstdint>
#include <cstdlib>
#include <limits>
#include <memory>
#include "../resolved_frame.hh"
#include "../../sys/nt/ntdll.hh"

#include "../../sys/win32/dbghelp.hh"

namespace pltxt2htm::stacktrace::details {
/**
 * @brief One DbgHelp session per linked image.
 * @details This lock only coordinates our own calls; hosts using DbgHelp
 *          elsewhere must coordinate those users, because panic diagnostics
 *          resolve symbols here. A distinct session handle does not make
 *          concurrent DbgHelp calls safe. Source lookup enables the
 *          process-wide LOAD_LINES option; hosts must coordinate option
 *          changes and all other DbgHelp calls.
 */
struct DbgHelpSession {
    ::pltxt2htm::sys_nt::SrwLock lock{};
    void* process{};
    bool attempted{};
    bool ready{};

    [[nodiscard]]
    auto try_initialize(this DbgHelpSession& self) noexcept -> bool {
        if (self.attempted) {
            return self.ready;
        }
        self.attempted = true;
        auto const current_process = reinterpret_cast<void*>(static_cast<::std::intptr_t>(-1));
        // A distinct real handle lets DbgHelp enumerate this process without
        // sharing the GetCurrentProcess() pseudo-handle session with the host.
        if (::pltxt2htm::sys_nt::NtDuplicateObject(current_process, current_process, current_process,
                                                   ::std::addressof(self.process), 0, 0, 2) < 0) {
            return false;
        }
        constexpr ::pltxt2htm::sys_win32::Dword symopt_load_lines = 0x00000010;
        (void)::pltxt2htm::sys_win32::SymSetOptions(::pltxt2htm::sys_win32::SymGetOptions() | symopt_load_lines);
        self.ready = ::pltxt2htm::sys_win32::SymInitialize(self.process, nullptr, 1) != 0;
        return self.ready;
    }

    /**
     * @brief Release the session's handle and DbgHelp state.
     * @details Explicit cleanup avoids invoking DbgHelp while a DLL is
     *          unloading under the loader lock. Call only while holding this
     *          session's lock.
     */
    void reset(this DbgHelpSession& self) noexcept {
        namespace nt = ::pltxt2htm::sys_nt;
        namespace win32 = ::pltxt2htm::sys_win32;
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

/**
 * @brief Access the lazily initialized process-wide DbgHelp session.
 * @return The session singleton.
 * @details Trivial destruction retains the resolver until explicit shutdown or
 *          process exit, so panics during static destruction remain usable.
 */
[[nodiscard]]
inline auto get_symbol_session() noexcept -> DbgHelpSession& {
    static constinit DbgHelpSession session{};
    return session;
}

/**
 * @brief Release the resolver before unloading a consumer DLL.
 * @return false when a lookup is active and cleanup did not run.
 * @details The host must also coordinate external DbgHelp users.
 */
[[nodiscard]]
inline auto shutdown_symbols() noexcept -> bool {
    namespace nt = ::pltxt2htm::sys_nt;
    auto& session = details::get_symbol_session();
    if (!nt::RtlTryAcquireSRWLockExclusive(::std::addressof(session.lock))) {
        return false;
    }
    session.reset();
    nt::RtlReleaseSRWLockExclusive(::std::addressof(session.lock));
    return true;
}

inline void resolve_description(ResolvedFrame& result, DbgHelpSession const& session,
                                ::std::uint64_t native_address) noexcept {
    constexpr auto name_offset = offsetof(::pltxt2htm::sys_win32::SymbolInfo, name);
    ::std::size_t capacity{2000}; // DbgHelp MAX_SYM_NAME
    bool refreshed{};
    for (;;) {
        if (capacity > (::std::numeric_limits<::pltxt2htm::sys_win32::Dword>::max)() ||
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
        auto* info = reinterpret_cast<::pltxt2htm::sys_win32::SymbolInfo*>(storage);
        info->size_of_struct = sizeof(::pltxt2htm::sys_win32::SymbolInfo);
        info->max_name_len = static_cast<::pltxt2htm::sys_win32::Dword>(capacity);
        auto const found = ::pltxt2htm::sys_win32::SymFromAddr(session.process, native_address,
                                                               ::std::addressof(result.displacement), info) != 0;
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
            if (!refreshed && ::pltxt2htm::sys_win32::SymRefreshModuleList(session.process)) {
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
inline auto resolve_nt(void* address) noexcept -> ResolvedFrame {
    ResolvedFrame result{.address = address};
    auto& session = details::get_symbol_session();
    if (!::pltxt2htm::sys_nt::RtlTryAcquireSRWLockExclusive(::std::addressof(session.lock))) {
        return result;
    }
    if (session.try_initialize()) {
        auto const native_address = static_cast<::std::uint64_t>(reinterpret_cast<::std::uintptr_t>(address));
        details::resolve_description(result, session, native_address);
        // File/line lookup is independent of function-name lookup.
        ::pltxt2htm::sys_win32::ImageHlpLine64 line{};
        line.size_of_struct = sizeof(line);
        ::pltxt2htm::sys_win32::Dword line_displacement{};
        if (::pltxt2htm::sys_win32::SymGetLineFromAddr64(session.process, native_address,
                                                         ::std::addressof(line_displacement), ::std::addressof(line)) &&
            line.file_name != nullptr) {
            result.text_truncated = !result.source_file.assign(line.file_name) || result.text_truncated;
            result.source_line = line.line_number;
        }
    }
    ::pltxt2htm::sys_nt::RtlReleaseSRWLockExclusive(::std::addressof(session.lock));
    return result;
}
} // namespace pltxt2htm::stacktrace::details
