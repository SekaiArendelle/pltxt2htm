/**
 * @file resolved_frame.hh
 * @brief Owning resolved frame data for the stacktrace implementation.
 */

#pragma once

#include <cstdint>
#include "text.hh"

namespace pltxt2htm::stacktrace {

/** Owning symbol data exposed by the pltxt2htm stacktrace API. */
struct ResolvedFrame {
    void* address{};
    Text description{};
    Text source_file{};
    ::std::uint64_t displacement{};
    unsigned long source_line{};
    bool text_truncated{};
};

} // namespace pltxt2htm::stacktrace
