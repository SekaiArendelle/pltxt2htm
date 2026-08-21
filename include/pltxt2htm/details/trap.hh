#pragma once

/**
 * @file trap.hh
 * @brief Low-level facilities for trapping and unreachable code paths.
 */

#include <utility>

#if defined(_MSC_VER) && !defined(__clang__)
    #include <intrin.h>
#endif

#include "../contracts.hh"

namespace pltxt2htm::details {

/**
 * @brief Terminates the program immediately.
 */
[[noreturn]]
inline void trap() noexcept {
#if defined(__GNUC__) || defined(__clang__)
    __builtin_trap();
#elif defined(_MSC_VER)
    __fastfail(7); // FAST_FAIL_FATAL_APP_EXIT
#else
    #error "pltxt2htm requires GCC, Clang, or MSVC"
#endif
}

/**
 * @brief Marks a code path as unreachable.
 */
template<::pltxt2htm::Contracts ndebug>
[[noreturn]]
inline void unreachable() noexcept {
    if constexpr (ndebug == ::pltxt2htm::Contracts::ignore) {
#if defined(__has_builtin)
    #if __has_builtin(__builtin_unreachable)
        __builtin_unreachable();
    #else
        ::std::unreachable();
    #endif
#else
        ::std::unreachable();
#endif
    }
    else {
        ::pltxt2htm::details::trap();
    }
}

} // namespace pltxt2htm::details
