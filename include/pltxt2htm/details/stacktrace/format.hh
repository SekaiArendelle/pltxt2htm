#pragma once

#include <fast_io/fast_io.h>
#include "entry.hh"

namespace pltxt2htm::details::stacktrace {
/** fast_io formatting customization. Only reads caller-owned data; it never
 * captures addresses or invokes a resolver. The caller chooses the output sink.
 */
template<typename Output>
constexpr void print_define(::fast_io::io_reserve_type_t<char, ResolvedFrame>, Output output,
                            ResolvedFrame const& frame) {
    if (frame.description[0] != '\0') {
        auto const text = ::pltxt2htm::details::stacktrace::text_view(frame.description);
        ::fast_io::io::print(output, ::fast_io::basic_io_scatter_t<char>{.base = text.data(), .len = text.size()});
    }
    else {
        ::fast_io::io::print(output, ::fast_io::mnp::hex0x(reinterpret_cast<::std::uintptr_t>(frame.address)));
    }
    if (frame.displacement != 0) {
        ::fast_io::io::print(output, " + ", ::fast_io::mnp::hex0x(frame.displacement));
    }
    if (frame.source_file[0] != '\0') {
        auto const text = ::pltxt2htm::details::stacktrace::text_view(frame.source_file);
        ::fast_io::io::print(output, " at ",
                             ::fast_io::basic_io_scatter_t<char>{.base = text.data(), .len = text.size()});
        if (frame.source_line != 0) {
            ::fast_io::io::print(output, ":", frame.source_line);
        }
    }
    if (frame.text_truncated) {
        ::fast_io::io::print(output, " <text truncated>");
    }
}
} // namespace pltxt2htm::details::stacktrace
