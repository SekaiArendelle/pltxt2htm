#pragma once

#include <compare>
#include <functional>
#include "resolve.hh"

namespace pltxt2htm::details::stacktrace {
template<typename Allocator>
class BasicStacktrace;

/** A saved native address. Queries resolve that address without recapturing.
 * Text queries return independently owned strings, like std::stacktrace_entry.
 */
class StacktraceEntry {
    void* address_{};
    template<typename Allocator>
    friend class BasicStacktrace;

public:
    using native_handle_type = void*;

    constexpr StacktraceEntry() noexcept = default;
    constexpr StacktraceEntry(StacktraceEntry const&) noexcept = default;
    constexpr StacktraceEntry(StacktraceEntry&&) noexcept = default;

    constexpr StacktraceEntry& operator=(this StacktraceEntry& self, StacktraceEntry const& other) noexcept {
        self.address_ = other.address_;
        return self;
    }

    constexpr StacktraceEntry& operator=(this StacktraceEntry& self, StacktraceEntry&& other) noexcept {
        self.address_ = other.address_;
        return self;
    }

    [[nodiscard]] constexpr native_handle_type native_handle(this StacktraceEntry const& self) noexcept {
        return self.address_;
    }

    [[nodiscard]] constexpr explicit operator bool(this StacktraceEntry const& self) noexcept {
        return self.address_ != nullptr;
    }

    [[nodiscard]] ::pltxt2htm::container::String description(this StacktraceEntry const& self) noexcept {
        if (!self) {
            return {};
        }
        auto const frame = ::pltxt2htm::details::stacktrace::resolve(self.address_);
        return frame.description;
    }

    [[nodiscard]] ::pltxt2htm::container::String source_file(this StacktraceEntry const& self) noexcept {
        if (!self) {
            return {};
        }
        auto const frame = ::pltxt2htm::details::stacktrace::resolve(self.address_);
        return frame.source_file;
    }

    [[nodiscard]] ::std::uint_least32_t source_line(this StacktraceEntry const& self) noexcept {
        if (!self) {
            return 0;
        }
        return ::pltxt2htm::details::stacktrace::resolve(self.address_).source_line;
    }

    [[nodiscard]] friend constexpr bool operator==(StacktraceEntry const& left, StacktraceEntry const& right) noexcept {
        return left.address_ == right.address_;
    }

    [[nodiscard]] friend constexpr ::std::strong_ordering operator<=>(StacktraceEntry const& left,
                                                                      StacktraceEntry const& right) noexcept {
        return ::std::compare_three_way{}(left.address_, right.address_);
    }
};
} // namespace pltxt2htm::details::stacktrace
