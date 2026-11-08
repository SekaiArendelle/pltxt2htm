/**
 * @file backend.hh
 * @brief NoneBackend: policy bundle of the no-resolver fallback backend.
 * @details One struct per backend pairs the entry type with its capture
 *          backend and makes the pair nameable everywhere; this fallback is
 *          always available.
 */

#pragma once

#include "stacktrace_entry.hh"
#include "capture.hh"

namespace pltxt2htm::stacktrace::details {

/// Pairing of NoneStacktraceEntry with the no-op NoneCaptureBackend.
struct NoneBackend {
    using entry = NoneStacktraceEntry;
    using capture = NoneCaptureBackend;
};

} // namespace pltxt2htm::stacktrace::details
