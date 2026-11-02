/**
 * @file stacktrace_entry.hh
 * @brief Saved native frame address and the text resolved from it.
 * @note Internal declaration header. Include `stacktrace.hh` for the complete
 *       public API and the `container::String` definitions.
 */

#pragma once

#include "resolved_frame.hh"

#if defined(_WIN32)
    #include "nt.hh"
#endif

namespace pltxt2htm::stacktrace {
/**
 * @brief Resolve an already captured address without unwinding or printing.
 * @param address Native frame address; null resolves to an empty frame without
 *                touching the backend.
 * @return Resolved symbol data; the address is retained on failure.
 * @details Windows DbgHelp resolution benefits from matching PDBs. Resolution
 *          may allocate, load libraries, and perform file I/O, so it is not
 *          suitable for signal/loader callbacks or reliable diagnostics after
 *          heap corruption.
 */
[[nodiscard]]
inline auto resolve(void* address) noexcept -> ResolvedFrame {
    if (address == nullptr) {
        return {};
    }
#if defined(_WIN32)
    return details::resolve_nt(address);
#else
    return {.address = address};
#endif
}

/**
 * @brief A saved native address.
 * @details resolved() looks the saved address up without recapturing; each call
 *          performs a full lookup (see resolve()), so resolve once and reuse the
 *          frame. The text fields are independently owned `container::String`
 *          values.
 */
class StacktraceEntry {
    void* address{};

    template<typename Allocator>
    friend class BasicStacktrace;

    constexpr StacktraceEntry(void* address_) noexcept
    : address{address_} {
    }

public:
    using native_handle_type = void*;

    constexpr StacktraceEntry() noexcept = default;
    constexpr StacktraceEntry(StacktraceEntry const&) noexcept = default;
    constexpr StacktraceEntry(StacktraceEntry&&) noexcept = default;

    constexpr auto operator=(this StacktraceEntry& self, StacktraceEntry const& other) noexcept -> StacktraceEntry& {
        self.address = other.address;
        return self;
    }

    [[nodiscard]]
    constexpr auto native_handle(this StacktraceEntry const& self) noexcept -> native_handle_type {
        return self.address;
    }

    [[nodiscard]]
    constexpr explicit operator bool(this StacktraceEntry const& self) noexcept {
        return self.address != nullptr;
    }

    /**
     * @brief Resolve the saved address into owned text and source location.
     * @return The resolved frame; the address is retained on lookup failure.
     */
    [[nodiscard]]
    auto resolved(this StacktraceEntry const& self) noexcept -> ResolvedFrame {
        return ::pltxt2htm::stacktrace::resolve(self.address);
    }

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
} // namespace pltxt2htm::stacktrace
