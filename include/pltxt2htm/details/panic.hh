#pragma once

// // libc++ do not support <stacktrace>
// #if __has_include(<stacktrace>)
//     #include <stacktrace>
// #endif
#include <cstdio>
#include "literal_string.hh"

namespace pltxt2htm::details {

int c_fputs(const char* __restrict _Str, FILE* __restrict) noexcept __asm__("fputs");
int c_fflush(FILE* __restrict) noexcept __asm__("fflush");

template<::pltxt2htm::details::LiteralString expression, ::pltxt2htm::details::LiteralString file_name,
         ::std::uint_least32_t line, ::std::uint_least32_t column, ::pltxt2htm::details::LiteralString msg>
#if __has_cpp_attribute(__gnu__::__cold__)
[[__gnu__::__cold__]]
#endif
inline void panic_print() noexcept {
    auto to_be_printed = ::pltxt2htm::details::concat(
        ::pltxt2htm::details::LiteralString{"Program panicked because \"assert("}, expression,
        ::pltxt2htm::details::LiteralString{")\" failed\n"
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
        msg, ::pltxt2htm::details::LiteralString{"\"\0"});
    ::pltxt2htm::details::c_fputs(to_be_printed.cdata(), stderr);
    ::pltxt2htm::details::c_fflush(stderr);
}

} // namespace pltxt2htm::details
