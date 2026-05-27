#if !defined(_WIN32) && __has_include(<unistd.h>) && __has_include(<sys/wait.h>)

    #include <unistd.h>
    #include <sys/wait.h>

    #include <cstdio>

    // Ensure stacktrace is disabled: stacktrace output is dynamic (file/line/function names
    // depend on the runtime environment), which would break the exact string comparison below.
    #ifdef PLTXT2HTM_EXPERIMENTAL_ENABLE_STACKTRACE
        #undef PLTXT2HTM_EXPERIMENTAL_ENABLE_STACKTRACE
    #endif

    #include <pltxt2htm/details/panic.hh>
    #include "precompile.hh"

namespace {

void test_panic_basic() noexcept {
    int pipe_fds[2]{};
    if (::pipe(pipe_fds) == -1) [[unlikely]] {
        ::std::fprintf(stderr, "pipe() failed\n");
        ::exception::terminate();
    }

    auto pid = ::fork();
    if (pid == -1) [[unlikely]] {
        ::std::fprintf(stderr, "fork() failed\n");
        ::exception::terminate();
    }
    else if (pid == 0) {
        ::close(pipe_fds[0]);
        ::dup2(pipe_fds[1], STDERR_FILENO);
        ::close(pipe_fds[1]);

        ::pltxt2htm::details::panic<::pltxt2htm::details::U8LiteralString{u8"test_expression"},
                                    ::pltxt2htm::details::U8LiteralString{u8"test_file.cc"}, 42, 7,
                                    ::pltxt2htm::details::U8LiteralString{u8"test message"}>();
    }

    ::close(pipe_fds[1]);

    int wait_status{};
    ::waitpid(pid, ::std::addressof(wait_status), 0);

    ::pltxt2htm_test::assert_true(WIFSIGNALED(wait_status));

    char8_t buffer[4096]{};
    ::std::size_t total_read{};
    while (total_read < sizeof(buffer) - 1) {
        auto n = ::read(pipe_fds[0], buffer + total_read, sizeof(buffer) - 1 - total_read);
        if (n <= 0) {
            break;
        }
        total_read += static_cast<::std::size_t>(n);
    }
    ::close(pipe_fds[0]);

    ::pltxt2htm_test::assert_true(total_read > 0);

    constexpr auto expected_ls = ::pltxt2htm::details::concat(
        ::pltxt2htm::details::U8LiteralString{u8"Program panicked because \"assert("},
        ::pltxt2htm::details::U8LiteralString{u8"test_expression"},
        ::pltxt2htm::details::U8LiteralString{
            u8")\" failed, please file a bug at \"https://github.com/SekaiArendelle/pltxt2htm/issues\" and attach the "
            u8"crash info along with the source text\n* in file: "},
        ::pltxt2htm::details::U8LiteralString{u8"test_file.cc"},
        ::pltxt2htm::details::U8LiteralString{u8"\n"
                                              "* in line: "},
        ::pltxt2htm::details::uint_to_literal_string<42>(),
        ::pltxt2htm::details::U8LiteralString{u8"\n"
                                              "* in column: "},
        ::pltxt2htm::details::uint_to_literal_string<7>(),
        ::pltxt2htm::details::U8LiteralString{u8"\n"
                                              "* with message: \""},
        ::pltxt2htm::details::U8LiteralString{u8"test message"}, ::pltxt2htm::details::U8LiteralString{u8"\"\n\0"});
    auto output = ::fast_io::u8string_view{buffer, total_read};
    auto expected = ::fast_io::u8string_view{expected_ls.cdata(), expected_ls.size()};
    ::pltxt2htm_test::assert_true(output == expected);
}

} // namespace

int main() noexcept {
    test_panic_basic();
    return 0;
}

#else

    #warning "Skipping test_panic.cc because the platform does not support fork and pipe"

int main() noexcept {
    return 0;
}

#endif
