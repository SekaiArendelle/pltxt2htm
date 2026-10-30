/**
 * @file resolved_frame.hh
 * @brief Owning resolved frame data for the stacktrace implementation.
 * @note This header is declaration-only for the text fields. Include
 *       `stacktrace.hh` (or `container/string.hh`) in translation units that
 *       copy or assign them.
 */

#pragma once

#include <cstdint>

#include "../container/impl/string_decl.hh"

namespace pltxt2htm::stacktrace {

struct ResolvedFrame {
    void* address{};
    ::pltxt2htm::container::String description{};
    ::pltxt2htm::container::String source_file{};
    ::std::uint64_t displacement{};
    ::std::uint32_t source_line{};
    bool text_truncated{};
};

} // namespace pltxt2htm::stacktrace
