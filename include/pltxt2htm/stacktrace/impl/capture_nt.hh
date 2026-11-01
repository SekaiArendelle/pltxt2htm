/**
 * @file capture_nt.hh
 * @brief Windows capture backend using RtlCaptureStackBackTrace.
 */

#pragma once

#include <cstddef>
#include <limits>
#include <span>
#include "../../sys/nt/ntdll.hh"

namespace pltxt2htm::stacktrace::details {

struct CaptureBackend {
    static constexpr bool supported = true;
    static constexpr ::std::size_t max_depth = 65535;

    /**
     * @brief Write up to frames.size() return addresses into frames.
     * @param skip Native frames to skip, starting with this function's frame.
     * @return Number of addresses written; a full buffer may have more frames.
     */
    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__noinline__)
    [[__gnu__::__noinline__]]
#elif defined(_MSC_VER)
    __declspec(noinline)
#endif
    static auto capture(::std::span<void*> frames, ::std::size_t skip) noexcept -> ::std::size_t {
        // A skip wider than ULONG is unrepresentable, so report no frames
        // instead of letting the cast wrap into a different offset.
        constexpr auto max_skip = (::std::numeric_limits<::pltxt2htm::sys_nt::Ulong>::max)();
        if (frames.empty() || skip > max_skip) {
            return 0;
        }
        auto const capacity = frames.size() < max_depth ? frames.size() : max_depth;
        return ::pltxt2htm::sys_nt::RtlCaptureStackBackTrace(static_cast<::pltxt2htm::sys_nt::Ulong>(skip),
                                                             static_cast<::pltxt2htm::sys_nt::Ulong>(capacity),
                                                             frames.data(), nullptr);
    }
};

} // namespace pltxt2htm::stacktrace::details
