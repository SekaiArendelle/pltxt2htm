/**
 * @file resolved_frame.hh
 * @brief Owning resolved frame data for the stacktrace implementation.
 */

#pragma once

#include <cstdint>
#include "text.hh"

namespace pltxt2htm::stacktrace {

struct ResolvedFrame {
    void* address{};
    Text description{};
    Text source_file{};
    ::std::uint64_t displacement{};
    ::std::uint32_t source_line{};
    bool text_truncated{};
};

} // namespace pltxt2htm::stacktrace
