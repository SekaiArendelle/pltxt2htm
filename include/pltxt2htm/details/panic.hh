/**
 * @file panic.hh
 * @brief Panic and error handling utilities for pltxt2htm
 * @details Provides panic functionality for assertion failures and critical errors
 */

#pragma once

// Stack trace support is currently disabled as libc++ doesn't support <stacktrace>
// #if __has_include(<stacktrace>)
//     #include <stacktrace>
// #endif
#include <cstdio>
#include <exception/exception.hh>
#include "literal_string.hh"

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
template<::pltxt2htm::details::LiteralString expression, ::pltxt2htm::details::LiteralString file_name,
         ::std::uint_least32_t line, ::std::uint_least32_t column, ::pltxt2htm::details::LiteralString msg>
#if __has_cpp_attribute(__gnu__::__cold__)
[[__gnu__::__cold__]] // Mark as cold path for compiler optimization
#endif
[[noreturn]]
inline void panic() noexcept {
    auto to_be_printed = ::pltxt2htm::details::concat(
        ::pltxt2htm::details::LiteralString{"Program panicked because \"assert("}, expression,
        ::pltxt2htm::details::LiteralString{
            ")\" failed, please file a bug at \"https://github.com/SekaiArendelle/pltxt2htm/issues\" and attach the "
            "crash info along with the source text\n"
            "* in file: "},
        file_name,
        ::pltxt2htm::details::LiteralString{"\n"
                                            "* in line: "},
        ::pltxt2htm::details::uint_to_literal_string<line>(),
        ::pltxt2htm::details::LiteralString{"\n"
                                            "* in column: "},
        ::pltxt2htm::details::uint_to_literal_string<column>(),
        ::pltxt2htm::details::LiteralString{"\n"
                                            "* with message: \""},
        msg, ::pltxt2htm::details::LiteralString{"\"\n\0"});

    ::std::fputs(to_be_printed.cdata(), stderr);
    ::std::fflush(stderr);

    ::exception::terminate();
}

} // namespace pltxt2htm::details
