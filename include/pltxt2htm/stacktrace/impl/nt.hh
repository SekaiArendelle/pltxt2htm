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

#include <fast_io/fast_io_core.h>

#include "resolved_frame.hh"
#include "../../sys/nt/ntdll.hh"

#include "../../sys/win32/dbghelp.hh"

namespace pltxt2htm::stacktrace::details {
/**
 * @brief Scoped exclusive ownership of a session's SRW lock.
 * @details The guard releases the lock when it leaves scope, so early returns
 *          cannot unbalance the acquire/release pair. An unheld guard is the
 *          failure result of try_acquire().
 */
class DbgHelpLock {
    ::pltxt2htm::sys_nt::SrwLock* lock{};

    constexpr explicit DbgHelpLock(::pltxt2htm::sys_nt::SrwLock* lock_) noexcept
        : lock{lock_} {
    }

public:
    constexpr DbgHelpLock(DbgHelpLock const&) noexcept = delete;
    constexpr DbgHelpLock(DbgHelpLock&&) noexcept = delete;
    constexpr auto operator=(DbgHelpLock const&) noexcept -> DbgHelpLock& = delete;
    constexpr auto operator=(DbgHelpLock&&) noexcept -> DbgHelpLock& = delete;

    ~DbgHelpLock() noexcept {
        if (lock != nullptr) {
            ::pltxt2htm::sys_nt::RtlReleaseSRWLockExclusive(lock);
        }
    }

    [[nodiscard]]
    static auto try_acquire(::pltxt2htm::sys_nt::SrwLock& lock_) noexcept -> DbgHelpLock {
        if (::pltxt2htm::sys_nt::RtlTryAcquireSRWLockExclusive(::std::addressof(lock_)) == 0) {
            return DbgHelpLock{nullptr};
        }
        return DbgHelpLock{::std::addressof(lock_)};
    }

    [[nodiscard]]
    constexpr explicit operator bool(this DbgHelpLock const& self) noexcept {
        return self.lock != nullptr;
    }
};

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
    /**
     * @brief Initialization state of the session.
     * @details `uninitialized` allows another attempt; `unavailable` records a
     *          failed attempt so it is not repeated; `ready` means the duplicate
     *          handle is initialized and must be cleaned up before release.
     */
    enum class State : unsigned {
        uninitialized = 0,
        unavailable,
        ready,
    };

    ::pltxt2htm::sys_nt::SrwLock lock{};
    void* process{};
    State state{State::uninitialized};

    [[nodiscard]]
    constexpr auto is_ready(this DbgHelpSession const& self) noexcept -> bool {
        return self.state == State::ready;
    }

    [[nodiscard]]
    auto try_initialize(this DbgHelpSession& self) noexcept -> bool {
        if (self.state != State::uninitialized) {
            return self.state == State::ready;
        }
        self.state = State::unavailable;
        auto const current_process = reinterpret_cast<void*>(static_cast<::std::intptr_t>(-1));
        // A distinct real handle lets DbgHelp enumerate this process without
        // sharing the GetCurrentProcess() pseudo-handle session with the host.
        if (::pltxt2htm::sys_nt::NtDuplicateObject(current_process, current_process, current_process,
                                                   ::std::addressof(self.process), 0, 0, 2) < 0) {
            return false;
        }
        constexpr ::pltxt2htm::sys_win32::Dword symopt_load_lines = 0x00000010;
        (void)::pltxt2htm::sys_win32::SymSetOptions(::pltxt2htm::sys_win32::SymGetOptions() | symopt_load_lines);
        if (::pltxt2htm::sys_win32::SymInitialize(self.process, nullptr, 1) == 0) {
            return false;
        }
        self.state = State::ready;
        return true;
    }

    /**
     * @brief Release the session's handle and DbgHelp state.
     * @details Explicit cleanup avoids invoking DbgHelp while a DLL is
     *          unloading under the loader lock. Call only while holding this
     *          session's lock.
     */
    void reset(this DbgHelpSession& self) noexcept {
        if (self.state == State::ready) {
            (void)::pltxt2htm::sys_win32::SymCleanup(self.process);
        }
        if (self.process != nullptr) {
            (void)::pltxt2htm::sys_nt::NtClose(self.process);
        }
        self.process = nullptr;
        self.state = State::uninitialized;
    }

    /**
     * @brief Acquire the session's lock exclusively, then release the session.
     * @return false when a lookup is active, so nothing was released.
     */
    [[nodiscard]]
    auto try_shutdown(this DbgHelpSession& self) noexcept -> bool {
        auto held = DbgHelpLock::try_acquire(self.lock);
        if (!held) {
            return false;
        }
        self.reset();
        return true;
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
    return details::get_symbol_session().try_shutdown();
}

inline void resolve_description(ResolvedFrame& result, DbgHelpSession const& session,
                                ::std::uint64_t native_address) noexcept {
    constexpr auto name_offset = offsetof(::pltxt2htm::sys_win32::SymbolInfo, name);
    ::std::size_t capacity{2000}; // DbgHelp MAX_SYM_NAME
    bool refreshed{};
    while (true) {
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
        auto const* const name = reinterpret_cast<char const*>(storage + name_offset);
        result.description.assign(name, name + name_size);
        ::std::free(storage);
        return;
    }
}

[[nodiscard]]
inline auto resolve_nt(void* address) noexcept -> ResolvedFrame {
    ResolvedFrame result{.address = address};
    auto& session = details::get_symbol_session();
    auto held = DbgHelpLock::try_acquire(session.lock);
    if (!held) {
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
            auto const* const file_name = line.file_name;
            result.source_file.assign(file_name, file_name + ::fast_io::cstr_len(file_name));
            result.source_line = line.line_number;
        }
    }
    return result;
}
} // namespace pltxt2htm::stacktrace::details
