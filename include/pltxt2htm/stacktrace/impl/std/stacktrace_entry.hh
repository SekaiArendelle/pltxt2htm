/**
 * @file stacktrace_entry.hh
 * @brief StdStacktraceEntry wrapped around the C++23 std::stacktrace_entry
 *        fallback.
 * @details Resolution redistributes description(), source_file() and
 *          source_line() of the standard entry into a ResolvedFrame; demangling
 *          mirrors the resolver backends for standard libraries that report
 *          mangled names as the description.
 */

#pragma once

#include <compare>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <stacktrace>

#include <fast_io/fast_io_core.h>
#include <fast_io/fast_io_dsal/string_view.h>

#include "../resolved_frame.hh"
#include "../../../details/demangle.hh"

namespace pltxt2htm::stacktrace::details {

/**
 * @brief One saved frame held by the C++23 standard library.
 * @details The standard entry keeps the native frame state, so capture stores
 *          entries by value and resolution consults the standard library.
 * @note This is the C++23-standard fallback; StdBackend in impl/std/backend.hh
 *       pairs it with the capture backend, and the public front-end aliases it
 *       when that bundle is selected.
 */
class StdStacktraceEntry {
public:
    using native_handle_type = ::std::stacktrace_entry::native_handle_type;

private:
    friend struct StdCaptureBackend;

    ::std::stacktrace_entry entry{};

public:
    constexpr StdStacktraceEntry() noexcept = default;
    constexpr StdStacktraceEntry(StdStacktraceEntry const&) noexcept = default;

    constexpr auto operator=(this StdStacktraceEntry& self, StdStacktraceEntry const& other) noexcept
        -> StdStacktraceEntry& {
        self.entry = other.entry;
        return self;
    }

private:
    constexpr explicit StdStacktraceEntry(::std::stacktrace_entry entry_) noexcept
        : entry{entry_} {
    }

public:
    [[nodiscard]]
    constexpr auto native_handle(this StdStacktraceEntry const& self) noexcept -> native_handle_type {
        return self.entry.native_handle();
    }

    [[nodiscard]]
    constexpr explicit operator bool(this StdStacktraceEntry const& self) noexcept {
        return static_cast<bool>(self.entry);
    }

    /**
     * @brief Resolve the saved frame into owned text and source location.
     * @return The resolved frame; a failed lookup keeps the address and
     *         reports no text for that field.
     * @details Null entries resolve to an empty frame without touching the
     *          standard machinery. Per the standard, description(),
     *          source_file() and source_line() report every failure except
     *          allocation failure as no-information-available, so allocation
     *          failure is the only thing that can escape this noexcept
     *          function; unwinding out of it then terminates, matching the
     *          library's allocation-failure handling elsewhere.
     */
    [[nodiscard]]
    auto resolved(this StdStacktraceEntry const& self) noexcept -> ResolvedFrame {
        if (static_cast<bool>(self.entry) == false) {
            return {};
        }
        ResolvedFrame frame{.address = reinterpret_cast<::std::uintptr_t>(self.entry.native_handle())};
        auto const description = self.entry.description();
        if (auto* const demangled =
                ::pltxt2htm::details::demangle_symbol_name(::fast_io::mnp::os_c_str(description.c_str()))) {
            frame.description.assign(demangled, demangled + ::fast_io::cstr_len(demangled));
            ::std::free(demangled);
        }
        else {
            frame.description.assign(description.data(), description.data() + description.size());
        }
        auto const file = self.entry.source_file();
        frame.source_file.assign(file.data(), file.data() + file.size());
        frame.source_line = static_cast<::std::size_t>(self.entry.source_line());
        return frame;
    }

    [[nodiscard]]
    constexpr auto operator==(this StdStacktraceEntry const& self, StdStacktraceEntry const& other) noexcept -> bool {
        return self.entry == other.entry;
    }

    [[nodiscard]]
    constexpr auto operator<=>(this StdStacktraceEntry const& self, StdStacktraceEntry const& other) noexcept
        -> ::std::strong_ordering {
        return self.entry <=> other.entry;
    }
};

} // namespace pltxt2htm::stacktrace::details
