/**
 * @file push_macro.hh
 * @brief Macro definitions for pltxt2htm assertions and debugging
 * @details This header defines assertion macros and debugging utilities.
 *          It should be paired with pop_macro.hh to properly manage macro scope.
 * @note This header may be included multiple times with pop_macro.hh,
 *       therefore, do not use `#pragma once`
 * @warning Always include pop_macro.hh after this file to restore previous macro state
 */

#include <source_location>
#include "panic.hh"

#pragma push_macro("pltxt2htm_assert")
#undef pltxt2htm_assert
/**
 * @brief Assert whether the condition expression is true, if not, print
 *        the message and terminate the program.
 *
 * This macro evaluates the given condition and if it evaluates to false,
 * it will call panic() to terminate the program with diagnostic information.
 * The assertion is only active when ndebug is set to false.
 *
 * @param condition The condition expression to evaluate
 * @param message A descriptive error message to display if the assertion fails
 *
 * @note This macro behaves like a standard assert but uses the pltxt2htm
 *       panic mechanism for error reporting.
 *
 * @see pltxt2htm::details::panic()
 */
#define pltxt2htm_assert(condition, message) \
    do { \
        if constexpr (ndebug == false) { \
            if ((condition) == false) [[unlikely]] { \
                constexpr auto source_location = ::std::source_location::current(); \
                ::pltxt2htm::details::panic< \
                    ::pltxt2htm::details::LiteralString{#condition}, ::pltxt2htm::details::LiteralString{__FILE__}, \
                    source_location.line(), source_location.column(), ::pltxt2htm::details::LiteralString{message}>(); \
            } \
        } else { \
            [[assume(condition)]]; \
        } \
    } while (0)
