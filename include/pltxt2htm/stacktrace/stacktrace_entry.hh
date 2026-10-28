/**
 * @file stacktrace_entry.hh
 * @brief Saved native frame address and the text resolved from it.
 */

#pragma once

#include <compare>
#include <cstdint>
#include "resolve.hh"

namespace pltxt2htm::stacktrace {
template<typename Allocator>
class BasicStacktrace;

/** A saved native address. Queries resolve that address without recapturing.
 * Text queries return independently owned `stacktrace::Text` values, like
 * `std::stacktrace_entry`. Use resolved() when several fields are needed, so the
 * address is looked up only once.
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
    auto description(this StacktraceEntry const& self) noexcept -> Text {
        if (!self) {
            return {};
        }
        return ::pltxt2htm::stacktrace::resolve(self.address).description;
    }

    [[nodiscard]]
    auto source_file(this StacktraceEntry const& self) noexcept -> Text {
        if (!self) {
            return {};
        }
        return ::pltxt2htm::stacktrace::resolve(self.address).source_file;
    }

    [[nodiscard]]
    auto source_line(this StacktraceEntry const& self) noexcept -> ::std::uint_least32_t {
        if (!self) {
            return 0;
        }
        return static_cast<::std::uint_least32_t>(::pltxt2htm::stacktrace::resolve(self.address).source_line);
    }

    /** Resolve every field with a single lookup. */
    [[nodiscard]]
    auto resolved(this StacktraceEntry const& self) noexcept -> ResolvedFrame {
        if (!self) {
            return {};
        }
        return ::pltxt2htm::stacktrace::resolve(self.address);
    }

    [[nodiscard]]
    friend constexpr auto operator==(StacktraceEntry const& left, StacktraceEntry const& right) noexcept -> bool {
        return left.address == right.address;
    }

    [[nodiscard]]
    friend constexpr auto operator<=>(StacktraceEntry const& left, StacktraceEntry const& right) noexcept
        -> ::std::strong_ordering {
        return ::std::compare_three_way{}(left.address, right.address);
    }
};
} // namespace pltxt2htm::stacktrace
