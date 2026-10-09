/**
 * @file panic.hh
 * @brief Panic and error handling utilities for pltxt2htm
 * @details Provides panic functionality for assertion failures and critical errors
 */

#pragma once

#if defined(PLTXT2HTM_ENABLE_STACKTRACE)
    #include "stacktrace/dump.hh"
#endif
#include <cstdio>
#include "literal_string.hh"
#include "trap.hh"

namespace pltxt2htm::details {

/**
 * @brief Panic function that reports assertion failures and terminates the program
 * @tparam expression The assertion expression that failed
 * @tparam file_name The source file where the assertion failed
 * @tparam line The line number where the assertion failed
 * @tparam column The column number where the assertion failed
 * @tparam msg The error message to display
 * @note This function is marked as [[noreturn]] - it never returns and always terminates the program
 * @warning This function should only be called when a critical assertion failure occurs
 */
template<U8LiteralString expression, U8LiteralString file_name, unsigned line, unsigned column,
         U8LiteralString msg>
#if __has_cpp_attribute(__gnu__::__cold__)
[[__gnu__::__cold__]] // Mark as cold path for compiler optimization
#endif
[[noreturn]]
inline void panic() noexcept {
    constexpr auto to_be_printed = ::pltxt2htm::details::concat(
        U8LiteralString{u8"Program panicked because "}, expression,
        U8LiteralString{
            u8", please file a bug at \"https://github.com/SekaiArendelle/pltxt2htm/issues\" and attach the "
            u8"crash info along with the source text\n* in file: "},
        file_name,
        U8LiteralString{u8"\n"
                        "* in line: "},
        ::pltxt2htm::details::uint_to_literal_string<line>(),
        U8LiteralString{u8"\n"
                        "* in column: "},
        ::pltxt2htm::details::uint_to_literal_string<column>(),
        U8LiteralString{u8"\n"
                        "* with message: \""},
        msg, U8LiteralString{u8"\"\n"});

    ::std::fwrite(to_be_printed.cdata(), sizeof(typename decltype(to_be_printed)::value_type), to_be_printed.size(),
                  stderr);

#if defined(PLTXT2HTM_ENABLE_STACKTRACE)
    ::std::fflush(stderr);
    ::pltxt2htm::details::stacktrace::dump_current_stacktrace();
#endif
    ::std::fflush(stderr);

    ::pltxt2htm::details::trap();
}

} // namespace pltxt2htm::details
