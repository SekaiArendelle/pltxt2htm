/**
 * @file resolve.hh
 * @brief StacktraceEntry and address resolution for platforms without symbol
 *        resolution.
 * @details Without a resolver, resolved() reports the address and no text.
 */

#pragma once

#include <compare>

#include "../resolved_frame.hh"

namespace pltxt2htm::stacktrace {

/**
 * @brief A saved native address.
 * @details resolved() looks the saved address up without recapturing; each call
 *          performs a full lookup (see resolve()), so resolve once and reuse the
 *          frame. The text fields are independently owned `container::String`
 *          values.
 */
class StacktraceEntry {
public:
    using native_handle_type = void*;

private:
    native_handle_type address{};

    template<typename Allocator>
    friend class BasicStacktrace;

    constexpr StacktraceEntry(native_handle_type address_) noexcept
        : address{address_} {
    }

public:
    constexpr StacktraceEntry() noexcept = default;
    constexpr StacktraceEntry(StacktraceEntry const&) noexcept = default;
    constexpr StacktraceEntry(StacktraceEntry&&) noexcept = default;

    constexpr auto operator=(this StacktraceEntry& self, StacktraceEntry const& other) noexcept -> StacktraceEntry& {
        self.address = other.address;
        return self;
    }

#if defined(PLTXT2HTM_INTERNAL_USE)
    /**
     * @brief Build an entry from a native handle for tests.
     * @param handle Native frame handle to wrap.
     */
    [[nodiscard]]
    static constexpr auto from_native_handle(native_handle_type handle) noexcept -> StacktraceEntry {
        return StacktraceEntry{handle};
    }
#endif

    [[nodiscard]]
    constexpr auto native_handle(this StacktraceEntry const& self) noexcept -> native_handle_type {
        return self.address;
    }

    [[nodiscard]]
    constexpr explicit operator bool(this StacktraceEntry const& self) noexcept {
        return self.address != native_handle_type{};
    }

    /**
     * @brief Resolve the saved address into owned text and source location.
     * @return The resolved frame; the address is retained on lookup failure.
     */
    [[nodiscard]]
    auto resolved(this StacktraceEntry const& self) noexcept -> ResolvedFrame;

    [[nodiscard]]
    constexpr auto operator==(this StacktraceEntry const& self, StacktraceEntry const& other) noexcept -> bool {
        return self.address == other.address;
    }

    [[nodiscard]]
    constexpr auto operator<=>(this StacktraceEntry const& self, StacktraceEntry const& other) noexcept
        -> ::std::strong_ordering {
        return self.address <=> other.address;
    }
};

/**
 * @brief Resolve an already captured address without unwinding or printing.
 * @param address Native frame handle; null resolves to an empty frame.
 * @return Resolved symbol data; without a resolver only the address is kept.
 */
[[nodiscard]]
inline auto resolve(StacktraceEntry::native_handle_type address) noexcept -> ResolvedFrame {
    if (address == StacktraceEntry::native_handle_type{}) {
        return {};
    }
    return {.address = address};
}

inline auto StacktraceEntry::resolved(this StacktraceEntry const& self) noexcept -> ResolvedFrame {
    return ::pltxt2htm::stacktrace::resolve(self.address);
}

} // namespace pltxt2htm::stacktrace
