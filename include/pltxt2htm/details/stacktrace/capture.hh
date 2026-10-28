#pragma once

#include <cstddef>
#include <cstdlib>
#include <limits>
#include <span>
#include <utility>

#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(_WIN32)
    #include "../../sys/nt/ntdll.hh"
#endif

namespace pltxt2htm::details::stacktrace {
struct CaptureResult {
    ::std::size_t size{};
    bool possibly_truncated{};
};

/** Capture native return addresses into caller-owned storage.
 * skip counts entries in the captured native trace, including this helper when
 * it has a physical frame. No exact caller-frame identity is promised under
 * inlining/tail-call optimization. Capacity limits returned entries after skip.
 * Not async-signal-safe.
 */
[[nodiscard]]
inline auto capture(::std::span<void*> frames, ::std::size_t skip = 0) noexcept -> CaptureResult {
    if (frames.empty()) {
        return {};
    }
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(_WIN32)
    if (skip > (::std::numeric_limits<unsigned long>::max)()) {
        return {};
    }
    auto const capacity = frames.size() < 65535 ? frames.size() : 65535;
    auto const count = ::pltxt2htm::sys_nt::RtlCaptureStackBackTrace(
        static_cast<unsigned long>(skip), static_cast<unsigned long>(capacity), frames.data(), nullptr);
    return {.size = count, .possibly_truncated = count == capacity};
#else
    (void)skip;
    return {};
#endif
}

/** Malloc-owned native addresses captured for panic-path dumping. It
 * deliberately has no contract checks and avoids pltxt2htm containers, so
 * panic printing does not depend on the pltxt2htm container layer. Printing
 * itself lives in the panic path, not here. */
struct CapturedAddresses {
    void** addresses{};
    ::std::size_t count{};
    bool truncated{};

    constexpr CapturedAddresses() noexcept = default;
    CapturedAddresses(CapturedAddresses const&) = delete;

    constexpr CapturedAddresses(CapturedAddresses&& other) noexcept
        : addresses{::std::exchange(other.addresses, nullptr)},
          count{::std::exchange(other.count, 0)},
          truncated{::std::exchange(other.truncated, false)} {
    }

    CapturedAddresses& operator=(CapturedAddresses const&) = delete;

    constexpr auto operator=(this CapturedAddresses& self, CapturedAddresses&& other) noexcept -> CapturedAddresses& {
        if (::std::addressof(self) == ::std::addressof(other)) {
            return self;
        }
        ::std::free(self.addresses);
        self.addresses = ::std::exchange(other.addresses, nullptr);
        self.count = ::std::exchange(other.count, 0);
        self.truncated = ::std::exchange(other.truncated, false);
        return self;
    }

    constexpr ~CapturedAddresses() {
        ::std::free(addresses);
    }

    [[nodiscard]]
    constexpr auto size(this CapturedAddresses const& self) noexcept -> ::std::size_t {
        return self.count;
    }

    [[nodiscard]]
    constexpr auto possibly_truncated(this CapturedAddresses const& self) noexcept -> bool {
        return self.truncated;
    }

    [[nodiscard]]
    constexpr auto operator[](this CapturedAddresses const& self, ::std::size_t index) noexcept -> void* {
        return self.addresses[index];
    }
};

/** Capture the current native trace into self-owned storage, growing the
 * buffer while the capture reports possible truncation. Skips this helper's
 * own frame. Not async-signal-safe. */
[[nodiscard]]
inline auto capture_current_trace() noexcept -> CapturedAddresses {
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(_WIN32)
    ::std::size_t depth{64};
    void** addresses{};
    CaptureResult captured{};
    for (;;) {
        if (depth > (::std::numeric_limits<::std::size_t>::max)() / sizeof(void*)) {
            break;
        }
        auto* allocation = static_cast<void**>(::std::realloc(addresses, depth * sizeof(void*)));
        if (allocation == nullptr) {
            break;
        }
        addresses = allocation;
        captured = ::pltxt2htm::details::stacktrace::capture({addresses, depth}, 1);
        if (!captured.possibly_truncated || depth > (::std::numeric_limits<::std::size_t>::max)() / 2) {
            break;
        }
        depth *= 2;
    }
    auto result = CapturedAddresses{};
    result.addresses = addresses;
    result.count = captured.size;
    result.truncated = captured.possibly_truncated;
    return result;
#else
    return {};
#endif
}
} // namespace pltxt2htm::details::stacktrace
