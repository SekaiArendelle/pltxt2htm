/**
 * @file capture.hh
 * @brief Capture backend for platforms without a supported unwinder.
 */

#pragma once

#include <cstddef>
#include "stacktrace_entry.hh"
#include "../../../container/impl/vector_decl.hh"

namespace pltxt2htm::stacktrace::details {

struct NoneCaptureBackend {
    static constexpr bool supported{false};
    static constexpr ::std::size_t max_depth{};

    /**
     * @brief Unused backend entry point, kept so the shared front-end compiles.
     */
    template<typename Allocator>
    static void capture(
        ::pltxt2htm::container::Vector<NoneStacktraceEntry, ::fast_io::generic_allocator_adapter<Allocator>>&,
        ::std::size_t) noexcept {
    }
};

} // namespace pltxt2htm::stacktrace::details
