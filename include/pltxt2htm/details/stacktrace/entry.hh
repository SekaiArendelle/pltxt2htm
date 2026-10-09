#pragma once

#include <cstdint>
#include "../../container/string.hh"
#include "native_frame.hh"

namespace pltxt2htm::details::stacktrace {
/** Owning symbol data exposed by the pltxt2htm stacktrace API. */
struct ResolvedFrame {
    void* address{};
    ::pltxt2htm::container::String description{};
    ::pltxt2htm::container::String source_file{};
    ::pltxt2htm::container::String module_file{};
    ::std::uint64_t displacement{};
    unsigned long source_line{};
    bool text_truncated{};
};

[[nodiscard]]
constexpr auto own_frame(NativeResolvedFrame const& frame) noexcept -> ResolvedFrame {
    return {.address = frame.address,
            .description = ::pltxt2htm::container::String{frame.description.data(), frame.description.data() + frame.description.size()},
            .source_file = ::pltxt2htm::container::String{frame.source_file.data(), frame.source_file.data() + frame.source_file.size()},
            .module_file = ::pltxt2htm::container::String{frame.module_file.data(), frame.module_file.data() + frame.module_file.size()},
            .displacement = frame.displacement,
            .source_line = frame.source_line,
            .text_truncated = frame.text_truncated};
}
} // namespace pltxt2htm::details::stacktrace
