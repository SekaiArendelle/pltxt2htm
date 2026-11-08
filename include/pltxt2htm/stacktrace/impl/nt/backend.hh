/**
 * @file backend.hh
 * @brief NtBackend: policy bundle of the Windows NT capture backend.
 * @details The struct exists on every platform so code can name the pair
 *          uniformly; outside Windows its members alias the no-resolver
 *          fallback, and `NtBackend::capture::supported` reports availability.
 */

#pragma once

// Defunct aliases keep the struct referenceable where the NT unwinder does not
// exist; the no-resolver headers are includable everywhere.
#include "../none/stacktrace_entry.hh"
#include "../none/capture.hh"

#if defined(_WIN32)
    #include "stacktrace_entry.hh"
    #include "capture.hh"
#endif

namespace pltxt2htm::stacktrace::details {

#if defined(_WIN32)
/// Pairing of NtStacktraceEntry with the RtlCaptureStackBackTrace backend.
struct NtBackend {
    using entry = NtStacktraceEntry;
    using capture = NtCaptureBackend;
};
#else
/// Defunct pairing on platforms without the NT unwinder.
struct NtBackend {
    using entry = NoneStacktraceEntry;
    using capture = NoneCaptureBackend;
};
#endif

} // namespace pltxt2htm::stacktrace::details
