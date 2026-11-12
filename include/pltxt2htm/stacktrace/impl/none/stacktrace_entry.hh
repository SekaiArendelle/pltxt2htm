/**
 * @file stacktrace_entry.hh
 * @brief NoneStacktraceEntry and address resolution for platforms without symbol
 *        resolution.
 * @details Without a resolver, resolved() reports the address and no text.
 */

#pragma once

#include <compare>
#include <cstdint>

#include "../resolved_frame.hh"

namespace pltxt2htm::stacktrace::details {

/**
 * @brief A saved native address.
 * @details resolved() looks the saved address up without recapturing; each call
 *          performs a full lookup, so resolve once and reuse the frame. The
 *          text fields are independently owned `container::String` values.
 * @note The class is a standard-layout wrapper around a single native address.
 * @note This is the no-resolver fallback; NoneBackend in impl/none/backend.hh
 *       pairs it with the no-op capture backend, the public front-end aliases
 *       it when no capture-capable backend is selected, and the defunct
 *       NtBackend/StdBackend bundles alias it outside their platforms.
 */
class NoneStacktraceEntry {
public:
    using native_handle_type = ::std::uintptr_t;

private:
    native_handle_type address{};

public:
    constexpr NoneStacktraceEntry() noexcept = default;
    constexpr NoneStacktraceEntry(NoneStacktraceEntry const&) noexcept = default;

    constexpr auto operator=(this NoneStacktraceEntry& self, NoneStacktraceEntry const& other) noexcept
        -> NoneStacktraceEntry& {
        self.address = other.address;
        return self;
    }

#if defined(PLTXT2HTM_INTERNAL_USE)

private:
    constexpr NoneStacktraceEntry(native_handle_type address_) noexcept
        : address{address_} {
    }

public:
    /**
     * @brief Build an entry from a native address for tests.
     * @param handle Native frame address to wrap.
     */
    [[nodiscard]]
    static constexpr auto from_native_handle(native_handle_type handle) noexcept -> NoneStacktraceEntry {
        return NoneStacktraceEntry{handle};
    }
#endif

    [[nodiscard]]
    constexpr auto native_handle(this NoneStacktraceEntry const& self) noexcept -> native_handle_type {
        return self.address;
    }

    [[nodiscard]]
    constexpr explicit operator bool(this NoneStacktraceEntry const& self) noexcept {
        return self.address != 0;
    }

    /**
     * @brief Resolve the saved address into owned text and source location.
     * @return The resolved frame; the address is retained on lookup failure.
     * @details Null addresses resolve to an empty frame; without a resolver
     *          only the address is kept.
     */
    [[nodiscard]]
    auto resolved(this NoneStacktraceEntry const& self) noexcept -> ResolvedFrame {
        if (self.address == 0) {
            return {};
        }
        return {.address = self.address};
    }

    [[nodiscard]]
    constexpr auto operator==(this NoneStacktraceEntry const& self, NoneStacktraceEntry const& other) noexcept -> bool {
        return self.address == other.address;
    }

    [[nodiscard]]
    constexpr auto operator<=>(this NoneStacktraceEntry const& self, NoneStacktraceEntry const& other) noexcept
        -> ::std::strong_ordering {
        return self.address <=> other.address;
    }
};

} // namespace pltxt2htm::stacktrace::details
