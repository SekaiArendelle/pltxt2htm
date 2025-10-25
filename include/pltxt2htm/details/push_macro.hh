/**
 * @brief Macro defined by pltxt2htm
 * @note This header may be included multiple times with pop_macro.hh,
 *       therefore, do not `#pragma once`
 */

#include <source_location>
#include "panic.hh"

#pragma push_macro("pltxt2htm_assert")
#undef pltxt2htm_assert
/**
 * @brief Assert whether the condition expression is true, if not, print
 *         the message and terminate the program.
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
        } \
    } while (0)
