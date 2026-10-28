/**
 * @file panic.hh
 * @brief Panic and error handling utilities for pltxt2htm
 * @details Provides panic functionality for assertion failures and critical errors
 */

#pragma once

#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(_WIN32)
    #include <fast_io/fast_io.h>
    #include "stacktrace/capture.hh"
    #include "stacktrace/windows.hh"
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

#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && defined(_WIN32)
    ::std::fflush(stderr);
    ::fast_io::io::perr("* stack trace:\n");
    auto captured = ::pltxt2htm::details::stacktrace::capture_current_trace();
    for (::std::size_t i = 0; i < captured.size(); ++i) {
        auto const frame = ::pltxt2htm::details::stacktrace::resolve_native(captured[i]);
        ::fast_io::io::perr("[", i, "] ");
        if (!frame.description.empty()) {
            ::fast_io::io::perr(
                ::fast_io::basic_io_scatter_t<char>{.base = frame.description.data(), .len = frame.description.size()});
        }
        else {
            ::fast_io::io::perr(::fast_io::mnp::hex0x(reinterpret_cast<::std::uintptr_t>(frame.address)));
        }
        if (frame.displacement != 0) {
            ::fast_io::io::perr(" + ", ::fast_io::mnp::hex0x(frame.displacement));
        }
        if (!frame.source_file.empty()) {
            ::fast_io::io::perr(" at ", ::fast_io::basic_io_scatter_t<char>{.base = frame.source_file.data(),
                                                                            .len = frame.source_file.size()});
            if (frame.source_line != 0) {
                ::fast_io::io::perr(":", frame.source_line);
            }
        }
        if (frame.text_truncated) {
            ::fast_io::io::perr(" <text truncated>");
        }
        ::fast_io::io::perr("\n");
    }
#endif
    ::std::fflush(stderr);

    ::pltxt2htm::details::trap();
}

} // namespace pltxt2htm::details
