/**
 * @file asan_status.hh
 * @brief Compile-time AddressSanitizer detection, synced from fast_io
 *        `fast_io_core_impl/allocation/asan_util.h`.
 *
 * Referenced from
 * https://github.com/google/sanitizers/wiki/AddressSanitizerManualPoisoning
 */

#pragma once

#include <cstddef>

namespace pltxt2htm::memory::details::asan {

enum class asan_status {
    none,
    activate,
#if defined(__SANITIZE_ADDRESS__)
    current = activate
#elif defined(__has_feature)
    #if __has_feature(address_sanitizer)
    current = activate
    #else
    current = none
    #endif
#else
    current = none
#endif
};

} // namespace pltxt2htm::memory::details::asan
