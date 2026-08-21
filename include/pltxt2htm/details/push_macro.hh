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
#include "../contracts.hh"
#include "panic.hh"
#include "trap.hh"

#pragma push_macro("pltxt2htm_assume")
#undef pltxt2htm_assume
#if defined(__clang__)
    // Clang diagnoses function calls in an assume expression as having side effects.
    #define pltxt2htm_assume(condition)
#else
    #define pltxt2htm_assume(condition) [[assume(condition)]]
#endif

/**
 * @brief Assert whether the condition expression is true, if not, print
 *        the message and terminate the program.
 *
 * This macro evaluates the given condition and if it evaluates to false,
 * it will call panic() to terminate the program with diagnostic information.
 * The assertion is active for contract modes other than
 * `::pltxt2htm::Contracts::ignore` (for example, `quick_enforce`) and is
 * disabled when `ndebug == ::pltxt2htm::Contracts::ignore`.
 *
 * @param condition The condition expression to evaluate
 * @param message A descriptive error message to display if the assertion fails
 *
 * @note This macro behaves like a standard assert but uses the pltxt2htm
 *       panic mechanism for error reporting.
 *
 * @see pltxt2htm::details::panic()
 */
#pragma push_macro("pltxt2htm_assert")
#undef pltxt2htm_assert
#define pltxt2htm_assert(condition, message) \
    do { \
        if constexpr (ndebug != ::pltxt2htm::Contracts::ignore) { \
            if ((condition) == false) [[unlikely]] { \
                constexpr auto source_location = ::std::source_location::current(); \
                ::pltxt2htm::details::panic< \
                    ::pltxt2htm::details::U8LiteralString{u8"\"assert(" #condition ")\" failed"}, \
                    ::pltxt2htm::details::U8LiteralString{u8"" __FILE__}, source_location.line(), \
                    source_location.column(), pltxt2htm::details::U8LiteralString{message}>(); \
            } \
        } \
        else { \
            pltxt2htm_assume(condition); \
        } \
    } while (0)

/**
 * @brief Mark a code path as unreachable; calls panic or unreachable builtin.
 * @param message Diagnostic message string literal.
 * @note In quick_enforce mode invokes ::pltxt2htm::details::panic.
 *       In ignore mode uses ::pltxt2htm::details::unreachable<::pltxt2htm::Contracts::ignore>().
 */
#pragma push_macro("pltxt2htm_unreachable")
#undef pltxt2htm_unreachable
#define pltxt2htm_unreachable(message) \
    do { \
        if constexpr (ndebug != ::pltxt2htm::Contracts::ignore) { \
            constexpr auto source_location = ::std::source_location::current(); \
            ::pltxt2htm::details::panic<::pltxt2htm::details::U8LiteralString{u8"unreachable code reached"}, \
                                        ::pltxt2htm::details::U8LiteralString{u8"" __FILE__}, source_location.line(), \
                                        source_location.column(), ::pltxt2htm::details::U8LiteralString{message}>(); \
        } \
        else { \
            ::pltxt2htm::details::unreachable<::pltxt2htm::Contracts::ignore>(); \
        } \
    } while (0)
