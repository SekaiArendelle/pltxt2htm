/**
 * @file stacktrace_entry.hh
 * @brief Selects the platform implementation of StacktraceEntry.
 * @note Internal declaration header. Include `stacktrace.hh` for the complete
 *       public API and the `container::String` definitions.
 */

#pragma once

#include "resolved_frame.hh"

#if defined(_WIN32)
    #include "nt/resolve.hh"
#else
#endif
