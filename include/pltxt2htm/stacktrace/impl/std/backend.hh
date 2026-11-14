/**
 * @file backend.hh
 * @brief StdBackend: policy bundle of the C++23-standard capture backend.
 * @details The struct exists on every platform so code can name the pair
 *          uniformly; where the standard library does not provide usable
 *          <stacktrace> support its members alias the no-resolver fallback,
 *          and `StdBackend::capture::supported` reports availability.
 */

#pragma once

// libstdc++ defines __cpp_lib_stacktrace although its <stacktrace> support is
// still experimental: usable only when GCC was configured with backtrace
// support and the extra archive is linked. Mask that library.
#if !defined(__GLIBCXX__) && __has_include(<stacktrace>)
    #include <stacktrace>
    #if defined(__cpp_lib_stacktrace) && (__cpp_lib_stacktrace >= 202011L)
        #include "stacktrace_entry.hh"
        #include "capture.hh"
    #endif
#endif

// Defunct aliases keep the struct referenceable where <stacktrace> is missing
// or unusable; the no-resolver headers are includable everywhere.
#include "../none/stacktrace_entry.hh"
#include "../none/capture.hh"

//
// NOTE: the condition below must mirror the pairing at the top of this file;
// keep the two spellings in sync when the mask changes.
namespace pltxt2htm::stacktrace::details {

#if !defined(__GLIBCXX__) && __has_include(<stacktrace>) && defined(__cpp_lib_stacktrace) \
    && (__cpp_lib_stacktrace >= 202011L)
/// Pairing of StdStacktraceEntry with the std::stacktrace backend.
struct StdBackend {
    using entry = StdStacktraceEntry;
    using capture = StdCaptureBackend;
};
#else
/// Defunct pairing on standard libraries without usable <stacktrace>.
struct StdBackend {
    using entry = NoneStacktraceEntry;
    using capture = NoneCaptureBackend;
};
#endif

} // namespace pltxt2htm::stacktrace::details
