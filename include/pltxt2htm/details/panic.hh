#pragma once

// // libc++ do not support <stacktrace>
// #if __has_include(<stacktrace>)
//     #include <stacktrace>
// #endif
#include <cstdio>
#include <exception/exception.hh>
#include "literal_string.hh"

namespace pltxt2htm::details {

#if defined(__clang__) && (defined(__linux__) || defined(_WIN32))
// To minimize clang generate __clang_call_terminate, only for optimization
int c_fputs(char const* __restrict _Str, FILE* __restrict) noexcept __asm__("fputs");
int c_fflush(FILE* __restrict) noexcept __asm__("fflush");
#endif

template<::pltxt2htm::details::LiteralString expression, ::pltxt2htm::details::LiteralString file_name,
         ::std::uint_least32_t line, ::std::uint_least32_t column, ::pltxt2htm::details::LiteralString msg>
#if __has_cpp_attribute(__gnu__::__cold__)
[[__gnu__::__cold__]]
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
#if defined(__clang__) && (defined(__linux__) || defined(_WIN32))
    ::pltxt2htm::details::c_fputs(to_be_printed.cdata(), stderr);
    ::pltxt2htm::details::c_fflush(stderr);
#else
    ::std::fputs(to_be_printed.cdata(), stderr);
    ::std::fflush(stderr);
#endif
    ::exception::terminate();
}

} // namespace pltxt2htm::details
