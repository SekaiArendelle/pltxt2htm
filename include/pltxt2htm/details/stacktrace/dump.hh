#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <fast_io/fast_io.h>
#include "capture.hh"
#include "native_frame.hh"
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(_WIN32)
    #include "windows.hh"
#elif defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(__linux__) && __has_include(<execinfo.h>)
    #include "linux.hh"
#endif

namespace pltxt2htm::details::stacktrace {
template<typename Output>
constexpr void print_native_frame(Output output, NativeResolvedFrame const& frame) noexcept {
    if (!frame.description.empty()) {
        ::fast_io::io::print(output, ::fast_io::basic_io_scatter_t<char>{.base = frame.description.data(),
                                                                         .len = frame.description.size()});
    }
    else {
        ::fast_io::io::print(output, ::fast_io::mnp::hex0x(reinterpret_cast<::std::uintptr_t>(frame.address)));
    }
    if (frame.displacement != 0) {
        ::fast_io::io::print(output, " + ", ::fast_io::mnp::hex0x(frame.displacement));
    }
    if (!frame.source_file.empty()) {
        ::fast_io::io::print(
            output, " at ",
            ::fast_io::basic_io_scatter_t<char>{.base = frame.source_file.data(), .len = frame.source_file.size()});
        if (frame.source_line != 0) {
            ::fast_io::io::print(output, ":", frame.source_line);
        }
    }
    else if (!frame.module_file.empty()) {
        ::fast_io::io::print(
            output, " in ",
            ::fast_io::basic_io_scatter_t<char>{.base = frame.module_file.data(), .len = frame.module_file.size()});
    }
    if (frame.text_truncated) {
        ::fast_io::io::print(output, " <text truncated>");
    }
}

/** Capture, resolve, and print the current trace without using pltxt2htm containers. */
inline void dump_current_stacktrace() noexcept {
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && (defined(_WIN32) || (defined(__linux__) && __has_include(<execinfo.h>)))
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
    ::fast_io::io::perr("* stack trace:\n");
    for (::std::size_t i = 0; i < captured.size; ++i) {
        auto const frame = ::pltxt2htm::details::stacktrace::resolve_native(addresses[i]);
        ::fast_io::io::perr("[", i, "] ");
        ::pltxt2htm::details::stacktrace::print_native_frame(::fast_io::native_stderr(), frame);
        ::fast_io::io::perr("\n");
    }
    ::std::free(addresses);
#endif
}
} // namespace pltxt2htm::details::stacktrace
