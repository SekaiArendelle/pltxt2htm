/**
 * @file stacktrace_entry.hh
 * @brief Saved native frame address and the text resolved from it.
 * @note Internal declaration header. Include `stacktrace.hh` for the complete
 *       public API and the `container::String` definitions.
 */

#pragma once

#include <cstdint>

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
 * @details Queries resolve the address without recapturing. Text queries return
 *          independently owned `container::String` values, like
 *          `std::stacktrace_entry`. Use resolved() when several fields are
 *          needed, so the address is looked up only once.
 */
class StacktraceEntry {
    void* address{};
    template<typename Allocator>
    friend class BasicStacktrace;

public:
    using native_handle_type = void*;

    constexpr StacktraceEntry() noexcept = default;
    constexpr StacktraceEntry(StacktraceEntry const&) noexcept = default;
    constexpr StacktraceEntry(StacktraceEntry&&) noexcept = default;

    constexpr StacktraceEntry& operator=(this StacktraceEntry& self, StacktraceEntry const& other) noexcept {
        self.address = other.address;
        return self;
    }

    constexpr StacktraceEntry& operator=(this StacktraceEntry& self, StacktraceEntry&& other) noexcept {
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

    [[nodiscard]]
    auto description(this StacktraceEntry const& self) noexcept -> ::pltxt2htm::container::String {
        return ::pltxt2htm::stacktrace::resolve(self.address).description;
    }

    [[nodiscard]]
    auto source_file(this StacktraceEntry const& self) noexcept -> ::pltxt2htm::container::String {
        return ::pltxt2htm::stacktrace::resolve(self.address).source_file;
    }

    [[nodiscard]]
    auto source_line(this StacktraceEntry const& self) noexcept -> ::std::uint32_t {
        return ::pltxt2htm::stacktrace::resolve(self.address).source_line;
    }

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
