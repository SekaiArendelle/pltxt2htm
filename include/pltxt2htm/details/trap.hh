#pragma once

/**
 * @file trap.hh
 * @brief Low-level facilities for trapping and unreachable code paths.
 */

#include <exception>
#include <utility>

#include "../contracts.hh"

namespace pltxt2htm::details {

/**
 * @brief Terminates the program immediately.
 */
[[noreturn]]
inline void terminate() noexcept {
    // https://llvm.org/doxygen/Compiler_8h_source.html
#if defined(__has_builtin)
    #if __has_builtin(__builtin_trap)
    __builtin_trap();
    #else
    ::std::terminate();
    #endif
#else
    ::std::terminate();
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
        ::pltxt2htm::details::terminate();
    }
}

} // namespace pltxt2htm::details
