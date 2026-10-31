/**
 * @file resolved_frame.hh
 * @brief Owning resolved frame data for the stacktrace implementation.
 * @note Internal declaration header. Include `stacktrace.hh` for the complete
 *       public API and the `container::String` definitions.
 */

#pragma once

#include <cstdint>

#include "../../container/impl/string_decl.hh"

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
