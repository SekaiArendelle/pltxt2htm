#if defined(__linux__) && defined(PLTXT2HTM_ENABLE_STACKTRACE) && __has_include(<execinfo.h>)
    #include <cerrno>
    #include <cstring>
    #include <sys/wait.h>
    #include <unistd.h>
    #include <pltxt2htm/details/panic.hh>
    #include "precompile.hh"

int main() {
    int descriptors[2]{};
    pltxt2htm_test_assert_true(::pipe(descriptors) == 0);
    auto const pid = ::fork();
    pltxt2htm_test_assert_true(pid >= 0);
    if (pid == 0) {
        (void)::close(descriptors[0]);
        pltxt2htm_test_assert_true(::dup2(descriptors[1], STDERR_FILENO) != -1);
        (void)::close(descriptors[1]);
        ::pltxt2htm::details::panic<::pltxt2htm::details::U8LiteralString{u8"test_expression"},
                                    ::pltxt2htm::details::U8LiteralString{u8"test_file.cc"}, 42, 7,
                                    ::pltxt2htm::details::U8LiteralString{u8"test message"}>();
    }
    (void)::close(descriptors[1]);
    // Drain before waiting: the resolver can produce more than a pipe buffer.
    ::pltxt2htm::container::String output{};
    char buffer[4096]{};
    for (;;) {
        auto const count = ::read(descriptors[0], buffer, sizeof(buffer));
        if (count < 0 && errno == EINTR) {
            continue;
        }
        pltxt2htm_test_assert_true(count >= 0);
        if (count == 0) {
            break;
        }
        output.append<::pltxt2htm::Contracts::quick_enforce>(
            ::pltxt2htm::container::StringView{buffer, static_cast<::std::size_t>(count)});
    }
    (void)::close(descriptors[0]);
    int status{};
    auto waited = ::waitpid(pid, ::std::addressof(status), 0);
    while (waited == -1 && errno == EINTR) {
        waited = ::waitpid(pid, ::std::addressof(status), 0);
    }
    pltxt2htm_test_assert_true(waited == pid && WIFSIGNALED(status));
    pltxt2htm_test_assert_true(::std::strstr(output.c_str(), "* stack trace:\n") != nullptr);
    auto const* first = ::std::strstr(output.c_str(), "[0] ");
    pltxt2htm_test_assert_true(first != nullptr && ::std::strstr(first + 4, "[0] ") == nullptr);
    // Unnamed runtime frames must retain their module diagnostic too.
    pltxt2htm_test_assert_true(::std::strstr(output.c_str(), " in ") != nullptr ||
                               ::std::strstr(output.c_str(), " at ") != nullptr);
}
#else
int main() {
}
#endif
