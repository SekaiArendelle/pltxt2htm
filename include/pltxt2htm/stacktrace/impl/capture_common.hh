/**
 * @file capture_common.hh
 * @brief Types shared by the stacktrace capture backends.
 * @details Each backend header defines `pltxt2htm::stacktrace::details::CaptureBackend`
 *          with `supported`, `max_skip`, `max_depth`, and
 *          `capture(::std::span<void*>, ::std::size_t)`. A supported backend
 *          must use a positive `max_depth`; `max_skip` is the largest skip the
 *          backend can honor and may be smaller than the widest integer.
 */

#pragma once

#include <cstddef>

namespace pltxt2htm::stacktrace::details {
struct CaptureResult {
    ::std::size_t size{};
    bool possibly_truncated{};
};
} // namespace pltxt2htm::stacktrace::details
