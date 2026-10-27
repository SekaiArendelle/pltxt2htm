#pragma once

/**
 * @file trap.hh
 * @brief Low-level facilities for trapping and unreachable code paths.
 */

#include <utility>

#if defined(_MSC_VER) && !defined(__clang__)
    #include <intrin.h>
#endif

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
 * @brief Marks a code path as unreachable for the optimizer.
 * @details Pure optimizer hint with no diagnostic behavior in any contract mode.
 *          Prefer the pltxt2htm_unreachable macro (details/push_macro.hh), which
 *          panics with a message in enforce mode and only falls back to this
 *          function in ignore mode. Only use this function directly at call sites
 *          whose unreachability is proven, or when the optimizer hint is the goal.
 */
[[noreturn]]
inline void unreachable() noexcept {
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

} // namespace pltxt2htm::details
