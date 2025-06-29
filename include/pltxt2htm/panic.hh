#pragma once

#if __cpp_exceptions < 199711L
    #error "print crash info requiring enable C++ exceptions"
#endif

// // libc++ do not support <stacktrace>
// #if __has_include(<stacktrace>)
//     #include <stacktrace>
// #endif
#include <cstdio>
#include <source_location>
#include <fast_io/fast_io.h>
#include <fast_io/fast_io_dsal/string_view.h>

namespace pltxt2htm::details {

constexpr void panic_print(::std::source_location const& location, char const* const expression,
                           ::fast_io::u8cstring_view msg) {
    ::fast_io::println("Program panicked because \"assert(", ::fast_io::mnp::os_c_str(expression),
                       ")\" failed\n"
                       "* in file: ",
                       ::fast_io::mnp::os_c_str(location.file_name()),
                       "\n"
                       "* in line: ",
                       location.line(),
                       "\n"
                       "* in column: ",
                       location.column(),
                       "\n"
                       "* with message: \"",
                       ::fast_io::mnp::os_c_str(reinterpret_cast<char const*>(msg.data())), "\"");
    ::std::fflush(stdout);
}

} // namespace pltxt2htm::details
