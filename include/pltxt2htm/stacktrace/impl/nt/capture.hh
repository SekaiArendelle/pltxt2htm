/**
 * @file capture.hh
 * @brief Windows capture backend using RtlCaptureStackBackTrace.
 */

#pragma once

#include <cstddef>
#include <limits>
#include <span>
#include <type_traits>
#include "stacktrace_entry.hh"
#include "../../../sys/nt/ntdll.hh"

// windows.h defines min and max as function-like macros, which would expand
// inside std::numeric_limits<...>::max(). Hide them for this header, then
// restore the caller's definitions at the end.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace pltxt2htm::stacktrace::details {

struct CaptureBackend {
    static constexpr bool supported = true;
    static constexpr ::std::size_t max_depth = 65535;

    /**
     * @brief Write up to frames.size() captured entries into frames.
     * @param skip Native frames to skip, starting with this function's frame.
     * @return Number of entries written; a full buffer may have more frames.
     */
    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__noinline__)
    [[__gnu__::__noinline__]]
#elif defined(_MSC_VER)
    __declspec(noinline)
#endif
    static auto capture(::std::span<StacktraceEntry> frames, ::std::size_t skip) noexcept -> ::std::size_t {
        // StacktraceEntry is a standard-layout wrapper around exactly one native
        // handle, so the trace storage itself can be handed to the unwinder.
        static_assert(::std::is_standard_layout_v<StacktraceEntry>);
        static_assert(sizeof(StacktraceEntry) == sizeof(StacktraceEntry::native_handle_type));
        static_assert(alignof(StacktraceEntry) == alignof(StacktraceEntry::native_handle_type));
        // A skip wider than ULONG is unrepresentable, so report no frames
        // instead of letting the cast wrap into a different offset.
        constexpr auto max_skip = ::std::numeric_limits<::pltxt2htm::sys_nt::Ulong>::max();
        if (frames.empty() || skip > max_skip) {
            return 0;
        }
        auto const capacity = frames.size() < max_depth ? frames.size() : max_depth;
        auto* const handles = reinterpret_cast<StacktraceEntry::native_handle_type*>(frames.data());
        return ::pltxt2htm::sys_nt::RtlCaptureStackBackTrace(static_cast<::pltxt2htm::sys_nt::Ulong>(skip),
                                                             static_cast<::pltxt2htm::sys_nt::Ulong>(capacity), handles,
                                                             nullptr);
    }
};

} // namespace pltxt2htm::stacktrace::details

#pragma pop_macro("max")
#pragma pop_macro("min")
