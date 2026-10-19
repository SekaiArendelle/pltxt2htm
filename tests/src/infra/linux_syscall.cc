#if defined(__linux__)
    #include <asm/unistd.h>
    #include <sys/syscall.h>
    #include <sys/mman.h>
    #include <sys/utsname.h>
    #include <sys/wait.h>
    #include <unistd.h>

    #include <cerrno>
    #include <concepts>
    #include <cstdint>
    #include <type_traits>

    #include <pltxt2htm/sys/linux/system_call.hh>
    #include "precompile.hh"

namespace linux_sys = ::pltxt2htm::sys_linux;

static_assert(linux_sys::system_call_fails(0) == false);
static_assert(linux_sys::system_call_fails(1) == false);
static_assert(linux_sys::system_call_fails(4095) == false);
static_assert(linux_sys::system_call_fails(-1) == true);
static_assert(linux_sys::system_call_fails(-4095) == true);
static_assert(linux_sys::system_call_fails(-4096) == false);
static_assert(linux_sys::system_call_fails(0u) == false);
static_assert(linux_sys::system_call_fails(4095u) == false);
static_assert(linux_sys::system_call_fails(4096u) == false);
static_assert(linux_sys::system_call_fails(-1l) == true);
static_assert(linux_sys::system_call_fails(-4096l) == false);

namespace {

void test_zero_and_one_argument_syscalls() noexcept {
    pltxt2htm_test_assert_true(linux_sys::system_call<__NR_getpid, long>() == static_cast<long>(::getpid()));

    struct utsname uts_buffer{};
    pltxt2htm_test_assert_true(linux_sys::system_call<__NR_uname, long>(::std::addressof(uts_buffer)) == 0);
    pltxt2htm_test_assert_true(uts_buffer.sysname[0] != '\0');
}

void test_two_and_three_argument_syscalls() noexcept {
    int pipe_fds[2]{};
    pltxt2htm_test_assert_true(linux_sys::system_call<__NR_pipe2, long>(pipe_fds, 0) == 0);

    // write() and read() roundtrip through the pipe above covers the three-argument path.
    char const message[]{"pltxt2htm"};
    auto written = linux_sys::system_call<__NR_write, long>(pipe_fds[1], message, 9);
    pltxt2htm_test_assert_true(written == 9);

    char drained[16]{};
    auto received = linux_sys::system_call<__NR_read, long>(pipe_fds[0], drained, sizeof(drained));
    pltxt2htm_test_assert_true(received == 9);

    // Closing the same descriptor twice exercises a real failure return value (-EBADF).
    pltxt2htm_test_assert_true(linux_sys::system_call<__NR_close, long>(pipe_fds[0]) == 0);
    auto second_close = linux_sys::system_call<__NR_close, long>(pipe_fds[0]);
    pltxt2htm_test_assert_true(linux_sys::system_call_fails(second_close));
    pltxt2htm_test_assert_true(second_close == -EBADF);
    pltxt2htm_test_assert_true(linux_sys::system_call<__NR_close, long>(pipe_fds[1]) == 0);
}

void test_six_argument_and_inline_syscalls() noexcept {
    // mmap() exists only in the kernel tables of the inline-assembly architectures above.
    #if defined(__x86_64__) || defined(__aarch64__) || (defined(__riscv) && (__SIZEOF_SIZE_T__ == 8)) || \
        (defined(__loongarch__) && (__SIZEOF_SIZE_T__ == 8))
    auto mapping =
        linux_sys::system_call<__NR_mmap, long>(nullptr, 4096, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    pltxt2htm_test_assert_true(linux_sys::system_call_fails(mapping) == false);
    pltxt2htm_test_assert_true(linux_sys::inline_syscall<__NR_munmap, long>(mapping, 4096) == 0);
    #endif
}

void test_must_succeed_traps_on_failure() noexcept {
    auto child_pid = ::fork();
    pltxt2htm_test_assert_true(child_pid != -1);
    if (child_pid == 0) {
        // The child must terminate inside system_call_must_succeed and never reach ::_exit().
        linux_sys::system_call_must_succeed(static_cast<long>(-1));
        ::_exit(0);
    }

    int wait_status{};
    ::waitpid(child_pid, ::std::addressof(wait_status), 0);
    pltxt2htm_test_assert_true(WIFSIGNALED(wait_status));
}

void test_must_succeed_success_path_and_fast_exit() noexcept {
    // A non-failing value must not terminate: the child reaches its ::_exit() below.
    auto child_pid = ::fork();
    pltxt2htm_test_assert_true(child_pid != -1);
    if (child_pid == 0) {
        linux_sys::system_call_must_succeed(0l);
        ::_exit(7);
    }

    int wait_status{};
    ::waitpid(child_pid, ::std::addressof(wait_status), 0);
    pltxt2htm_test_assert_true(WIFEXITED(wait_status) && WEXITSTATUS(wait_status) == 7);

    // fast_exit() is a [[noreturn]] raw __NR_exit; the child must leave with the given code.
    child_pid = ::fork();
    pltxt2htm_test_assert_true(child_pid != -1);
    if (child_pid == 0) {
        linux_sys::fast_exit(42);
    }

    wait_status = 0;
    ::waitpid(child_pid, ::std::addressof(wait_status), 0);
    pltxt2htm_test_assert_true(WIFEXITED(wait_status) && WEXITSTATUS(wait_status) == 42);
}

} // namespace

int main() noexcept {
    test_zero_and_one_argument_syscalls();
    test_two_and_three_argument_syscalls();
    test_six_argument_and_inline_syscalls();
    test_must_succeed_traps_on_failure();
    test_must_succeed_success_path_and_fast_exit();
    return 0;
}

#else

int main() noexcept {
    return 0;
}

#endif
