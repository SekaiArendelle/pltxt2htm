/**
 * @file capture.hh
 * @brief Windows capture backend using RtlCaptureStackBackTrace.
 */

#pragma once

#include <cstddef>
#include <limits>
#include <memory>
#include <type_traits>
#include "stacktrace_entry.hh"
#include "../../../container/impl/vector_decl.hh"
#include "../../../sys/nt/ntdll.hh"

// windows.h defines min and max as function-like macros, which would expand
// inside std::numeric_limits<...>::max(). Hide them for this header, then
// restore the caller's definitions at the end.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace pltxt2htm::stacktrace::details {

struct NtCaptureBackend {
    static constexpr bool supported = true;
    static constexpr ::std::size_t max_depth = 65535;

    /**
     * @brief Append every captured entry to frames.
     * @param frames Entry storage to append to.
     * @param skip Native frames to skip, starting with this function's frame.
     * @details Grows the entry buffer by doubling until the unwinder writes
     *          fewer entries than the buffer holds or the backend limit is
     *          reached.
     */
    template<typename Allocator>
#if __has_cpp_attribute(__gnu__::__noinline__)
    [[__gnu__::__noinline__]]
#elif defined(_MSC_VER)
    __declspec(noinline)
#endif
    static void capture(
        ::pltxt2htm::container::Vector<NtStacktraceEntry, ::fast_io::generic_allocator_adapter<Allocator>>& frames,
        ::std::size_t skip) noexcept {
        // NtStacktraceEntry is a standard-layout wrapper around exactly one native
        // handle, and this backend may address that handle directly, so the trace
        // storage itself can be handed to the unwinder.
        static_assert(::std::is_standard_layout_v<NtStacktraceEntry>);
        static_assert(sizeof(NtStacktraceEntry) == sizeof(NtStacktraceEntry::native_handle_type));
        static_assert(alignof(NtStacktraceEntry) == alignof(NtStacktraceEntry::native_handle_type));
        // A skip wider than ULONG is unrepresentable, so report no frames
        // instead of letting the cast wrap into a different offset.
        constexpr auto max_skip = ::std::numeric_limits<::pltxt2htm::sys_nt::Ulong>::max();
        if (skip > max_skip) {
            return;
        }
        auto const base = frames.size();
        auto depth = max_depth < 64 ? max_depth : 64;
        while (true) {
            frames.resize(base + depth);
            auto* const handles = ::std::addressof(frames[base].address);
            auto const written = ::pltxt2htm::sys_nt::RtlCaptureStackBackTrace(
                static_cast<::pltxt2htm::sys_nt::Ulong>(skip), static_cast<::pltxt2htm::sys_nt::Ulong>(depth), handles,
                nullptr);
            frames.resize(base + written);
            if (written < depth || depth == max_depth) {
                return;
            }
            depth = depth > max_depth / 2 ? max_depth : depth * 2;
        }
    }
};

} // namespace pltxt2htm::stacktrace::details

#pragma pop_macro("max")
#pragma pop_macro("min")
