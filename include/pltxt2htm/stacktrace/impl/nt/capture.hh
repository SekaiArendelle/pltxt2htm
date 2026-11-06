/**
 * @file capture.hh
 * @brief Windows capture backend using RtlCaptureStackBackTrace.
 */

#pragma once

#include <array>
#include <cstddef>
#include <limits>
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
     * @details Grows the probe buffer by doubling until the unwinder writes
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
        // A skip wider than ULONG is unrepresentable, so report no frames
        // instead of letting the cast wrap into a different offset.
        constexpr auto max_skip = ::std::numeric_limits<::pltxt2htm::sys_nt::Ulong>::max();
        if (skip > max_skip) {
            return;
        }
        auto const base = frames.size();
        // The unwinder reports native frames as pointers, while entries keep
        // their addresses as integers. Probe into pointer storage and convert
        // each reported frame; a stack buffer covers the common shallow trace
        // without allocating one, and deeper traces grow a heap buffer.
        constexpr ::std::size_t initial_depth = 64;
        ::std::array<void*, initial_depth> shallow_probe{};
        ::pltxt2htm::container::Vector<void*, ::fast_io::generic_allocator_adapter<Allocator>> deep_probe{};
        auto depth = max_depth < initial_depth ? max_depth : initial_depth;
        while (true) {
            auto* probe = shallow_probe.data();
            if (depth > initial_depth) {
                deep_probe.resize(depth);
                probe = deep_probe.data();
            }
            auto const written = ::pltxt2htm::sys_nt::RtlCaptureStackBackTrace(
                static_cast<::pltxt2htm::sys_nt::Ulong>(skip), static_cast<::pltxt2htm::sys_nt::Ulong>(depth), probe,
                nullptr);
            frames.resize(base + written);
            for (::std::size_t i = 0; i != written; ++i) {
                frames[base + i].address = reinterpret_cast<NtStacktraceEntry::native_handle_type>(probe[i]);
            }
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
