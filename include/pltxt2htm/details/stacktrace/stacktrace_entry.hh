#pragma once

#include <compare>
#include <functional>
#include "resolved_frame.hh"

namespace pltxt2htm::details::stacktrace {
template<typename Allocator>
class BasicStacktrace;

/** A saved native address. Queries resolve that address without recapturing.
 * Text queries return independently owned strings, like std::stacktrace_entry.
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
        if (!self) {
            return {};
        }
        auto const frame = ::pltxt2htm::details::stacktrace::resolve(self.address);
        return frame.description;
    }

    [[nodiscard]]
    auto source_file(this StacktraceEntry const& self) noexcept -> ::pltxt2htm::container::String {
        if (!self) {
            return {};
        }
        auto const frame = ::pltxt2htm::details::stacktrace::resolve(self.address);
        return frame.source_file;
    }

    [[nodiscard]]
    auto source_line(this StacktraceEntry const& self) noexcept -> ::std::uint_least32_t {
        if (!self) {
            return 0;
        }
        return ::pltxt2htm::details::stacktrace::resolve(self.address).source_line;
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
} // namespace pltxt2htm::details::stacktrace
