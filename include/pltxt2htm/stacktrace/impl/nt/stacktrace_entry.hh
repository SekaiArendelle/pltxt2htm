/**
 * @file stacktrace_entry.hh
 * @brief Windows NtStacktraceEntry and DbgHelp symbol resolution.
 * @details The resolver is process-wide and lazily initialized; hosts using
 *          DbgHelp directly must coordinate with it.
 */

#pragma once

#include <cstdint>
#include <cstdlib>
#include <memory>

#include <fast_io/fast_io_core.h>
#include <fast_io/fast_io_dsal/string_view.h>

#include "../resolved_frame.hh"
#include "../../../details/demangle.hh"
#include "../../../sys/nt/ntdll.hh"
#include "../../../sys/win32/dbghelp.hh"
#include "symbol_info_storage.hh"

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
class DbgHelpSession {
public:
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

private:
    ::pltxt2htm::sys_nt::SrwLock lock{};
    ::pltxt2htm::sys_nt::Handle process{};
    State state{State::uninitialized};

public:
    [[nodiscard]]
    constexpr auto is_ready(this DbgHelpSession const& self) noexcept -> bool {
        return self.state == State::ready;
    }

    /**
     * @brief Stored duplicate process handle used for DbgHelp calls.
     * @return The handle value; callers pass it to DbgHelp lookup functions and
     *         must not close it, since the session cleans up in reset().
     * @details DbgHelp calls outside the class (and tests checking the session
     *          lifecycle) use this accessor instead of touching the member.
     */
    [[nodiscard]]
    constexpr auto process_handle(this DbgHelpSession const& self) noexcept -> ::pltxt2htm::sys_nt::Handle {
        return self.process;
    }

    /**
     * @brief The session's single coordination lock.
     * @return Reference to the session lock; callers serialize DbgHelp use by
     *         acquiring it exclusively via DbgHelpLock::try_acquire().
     * @details Resolution paths outside the class (and tests simulating a busy
     *          session) use this accessor instead of touching the member.
     */
    [[nodiscard]]
    constexpr auto srw_lock(this DbgHelpSession& self) noexcept -> ::pltxt2htm::sys_nt::SrwLock& {
        return self.lock;
    }

#if defined(PLTXT2HTM_INTERNAL_USE)

    /**
     * @brief Record a failed initialization attempt for tests.
     * @details Lets tests reproduce the unavailable state, in which later
     *          lookups give up without calling DbgHelp, without a real
     *          initialization failure.
     */
    constexpr auto force_unavailable(this DbgHelpSession& self) noexcept -> void {
        self.state = State::unavailable;
    }
#endif

    [[nodiscard]]
    auto try_initialize(this DbgHelpSession& self) noexcept -> bool {
        if (self.state != State::uninitialized) {
            return self.state == State::ready;
        }
        self.state = State::unavailable;
        constexpr auto current_process = ::pltxt2htm::sys_nt::nt_current_process();
        // A distinct real handle lets DbgHelp enumerate this process without
        // sharing the GetCurrentProcess() pseudo-handle session with the host.
        if (::pltxt2htm::sys_nt::NtDuplicateObject(current_process, current_process, current_process,
                                                   ::std::addressof(self.process), 0, 0, 2) < 0) {
            return false;
        }
        constexpr ::pltxt2htm::sys_win32::Dword symopt_load_lines{0x00000010};
        [[maybe_unused]] auto const previous =
            ::pltxt2htm::sys_win32::SymSetOptions(::pltxt2htm::sys_win32::SymGetOptions() | symopt_load_lines);
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
     * @details Cleanup is best-effort: at shutdown a failure is unrecoverable,
     *          there is no retry or error path to take, and process exit reclaims
     *          the resources regardless. The return values are therefore
     *          deliberately uninspected.
     */
    void reset(this DbgHelpSession& self) noexcept {
        if (self.state == State::ready) {
            [[maybe_unused]] auto const cleanup_ok = ::pltxt2htm::sys_win32::SymCleanup(self.process);
        }
        if (self.process.is_null() == false) {
            [[maybe_unused]] auto const close_status = ::pltxt2htm::sys_nt::NtClose(self.process);
        }
        self.process = {};
        self.state = State::uninitialized;
    }

    /**
     * @brief Acquire the session's lock exclusively, then release the session.
     * @return false when a lookup is active, so nothing was released.
     */
    [[nodiscard]]
    auto try_shutdown(this DbgHelpSession& self) noexcept -> bool {
        auto const held = DbgHelpLock::try_acquire(self.lock);
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
 * @brief A saved native address.
 * @details resolved() looks the saved address up without recapturing; each call
 *          performs a full lookup, so resolve once and reuse the frame. The
 *          text fields are independently owned `container::String` values.
 * @note The class is a standard-layout wrapper around a single native handle;
 *       the capture backend relies on that to fill trace storage directly.
 * @note This is the Windows implementation; the public `StacktraceEntry` name
 *       in stacktrace.hh aliases it.
 */
class NtStacktraceEntry {
public:
    using native_handle_type = void*;

private:
    friend struct NtCaptureBackend;

    native_handle_type address{};

public:
    constexpr NtStacktraceEntry() noexcept = default;
    constexpr NtStacktraceEntry(NtStacktraceEntry const&) noexcept = default;
    constexpr NtStacktraceEntry(NtStacktraceEntry&&) noexcept = default;

    constexpr auto operator=(this NtStacktraceEntry& self, NtStacktraceEntry const& other) noexcept
        -> NtStacktraceEntry& {
        self.address = other.address;
        return self;
    }

#if defined(PLTXT2HTM_INTERNAL_USE)

private:
    constexpr NtStacktraceEntry(native_handle_type address_) noexcept
        : address{address_} {
    }

public:
    /**
     * @brief Build an entry from a native handle for tests.
     * @param handle Native frame handle to wrap.
     */
    [[nodiscard]]
    static constexpr auto from_native_handle(native_handle_type handle) noexcept -> NtStacktraceEntry {
        return NtStacktraceEntry{handle};
    }
#endif

    [[nodiscard]]
    constexpr auto native_handle(this NtStacktraceEntry const& self) noexcept -> native_handle_type {
        return self.address;
    }

    [[nodiscard]]
    constexpr explicit operator bool(this NtStacktraceEntry const& self) noexcept {
        return self.address != nullptr;
    }

    /**
     * @brief Resolve the saved address into owned text and source location.
     * @return The resolved frame; the address is retained on lookup failure.
     * @details Null addresses resolve to an empty frame without touching the
     *          backend. DbgHelp resolution benefits from matching PDBs and may
     *          allocate, load libraries, and perform file I/O, so it is not
     *          suitable for signal/loader callbacks or reliable diagnostics
     *          after heap corruption.
     */
    [[nodiscard]]
    auto resolved(this NtStacktraceEntry const& self) noexcept -> ResolvedFrame;

    [[nodiscard]]
    constexpr auto operator==(this NtStacktraceEntry const& self, NtStacktraceEntry const& other) noexcept -> bool {
        return self.address == other.address;
    }

    [[nodiscard]]
    constexpr auto operator<=>(this NtStacktraceEntry const& self, NtStacktraceEntry const& other) noexcept
        -> ::std::strong_ordering {
        // Compare the addresses as integers: ordering pointers to unrelated
        // objects is unspecified.
        return reinterpret_cast<::std::uintptr_t>(self.address) <=> reinterpret_cast<::std::uintptr_t>(other.address);
    }
};

inline void resolve_description(ResolvedFrame& result, DbgHelpSession const& session,
                                ::std::uintptr_t native_address) noexcept {
    SymbolInfoStorage storage{};
    if (storage.is_valid() == false) {
        result.text_truncated = true;
        return;
    }
    bool refreshed{};
    auto const process_handle = session.process_handle();
    while (true) {
        storage.reset_for_lookup();
        // DbgHelp always reports a 64-bit displacement, even in 32-bit
        // processes; the difference of two addresses fits the address width, so
        // the frame stores it as uintptr_t.
        ::pltxt2htm::sys_win32::Dword64 displacement{};
        auto const found =
            ::pltxt2htm::sys_win32::SymFromAddr(process_handle, native_address, ::std::addressof(displacement),
                                                storage.symbol_info()) != 0;
        if (storage.is_name_truncated()) {
            if (!storage.grow()) {
                result.text_truncated = true;
                return;
            }
            continue;
        }
        if (found == false) {
            if (refreshed == false && ::pltxt2htm::sys_win32::SymRefreshModuleList(process_handle)) {
                refreshed = true;
                continue;
            }
            return;
        }
        result.displacement = static_cast<::std::uintptr_t>(displacement);
        auto const name = storage.name();
        if (auto* const demangled = ::pltxt2htm::details::demangle_symbol_name(name)) {
            result.description.assign(demangled, demangled + ::fast_io::cstr_len(demangled));
            ::std::free(demangled);
            return;
        }
        result.description.assign(name.data(), name.data() + name.size());
        return;
    }
}

inline auto NtStacktraceEntry::resolved(this NtStacktraceEntry const& self) noexcept -> ResolvedFrame {
    if (self.address == nullptr) {
        return {};
    }
    ResolvedFrame result{.address = reinterpret_cast<::std::uintptr_t>(self.address)};
    auto& session = ::pltxt2htm::stacktrace::details::get_symbol_session();
    auto const held = DbgHelpLock::try_acquire(session.srw_lock());
    if (!held) {
        return result;
    }
    if (session.try_initialize()) {
        auto const native_address = reinterpret_cast<::std::uintptr_t>(self.address);
        ::pltxt2htm::stacktrace::details::resolve_description(result, session, native_address);
        // File/line lookup is independent of function-name lookup.
        ::pltxt2htm::sys_win32::ImageHlpLine64 line{};
        line.size_of_struct = sizeof(line);
        ::pltxt2htm::sys_win32::Dword line_displacement{};
        if (::pltxt2htm::sys_win32::SymGetLineFromAddr64(session.process_handle(), native_address,
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
