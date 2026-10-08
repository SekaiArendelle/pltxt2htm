#pragma once

#include <array>
#include <span>
#include "capture.hh"

namespace pltxt2htm::details::stacktrace {
/** Owning fixed-capacity snapshot of native addresses. Like std::stacktrace,
 * capture is explicit and inspecting/iterating saved data never unwinds again.
 * Unlike the allocator-aware standard container, capacity is chosen at compile
 * time and truncation is exposed. Entries remain native addresses; resolve()
 * explicitly obtains independently owned symbol data for any saved entry.
 */
template<::std::size_t Capacity = 64>
class Stacktrace {
    ::std::array<void*, Capacity> frames_{};
    CaptureResult captured_{};

public:
    constexpr Stacktrace() noexcept = default;
    constexpr Stacktrace(Stacktrace const&) noexcept = default;
    constexpr Stacktrace(Stacktrace&&) noexcept = default;

    constexpr Stacktrace& operator=(this Stacktrace& self, Stacktrace const& other) noexcept {
        self.frames_ = other.frames_;
        self.captured_ = other.captured_;
        return self;
    }

    constexpr Stacktrace& operator=(this Stacktrace& self, Stacktrace&& other) noexcept {
        self.frames_ = other.frames_;
        self.captured_ = other.captured_;
        return self;
    }

    [[nodiscard]] static Stacktrace current(::std::size_t skip = 0) noexcept {
        Stacktrace result{};
        result.captured_ = ::pltxt2htm::details::stacktrace::capture(result.frames_, skip);
        return result;
    }

    [[nodiscard]] constexpr ::std::span<void* const> addresses(this Stacktrace const& self) noexcept {
        return {self.frames_.data(), self.captured_.size};
    }

    [[nodiscard]] constexpr auto begin(this Stacktrace const& self) noexcept {
        return self.frames_.data();
    }

    [[nodiscard]] constexpr auto end(this Stacktrace const& self) noexcept {
        return self.frames_.data() + self.captured_.size;
    }

    [[nodiscard]] constexpr ::std::size_t size(this Stacktrace const& self) noexcept {
        return self.captured_.size;
    }

    [[nodiscard]] constexpr bool empty(this Stacktrace const& self) noexcept {
        return self.captured_.size == 0;
    }

    [[nodiscard]] constexpr bool possibly_truncated(this Stacktrace const& self) noexcept {
        return self.captured_.possibly_truncated;
    }
};
} // namespace pltxt2htm::details::stacktrace
