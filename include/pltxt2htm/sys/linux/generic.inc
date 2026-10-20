#pragma once

// Generic fallback without inline assembly: the libc ::syscall() wrapper is used instead.
//
// ::syscall() is a C variadic, so every argument is first normalized into a register-width value
// by details::syscall_argument; integers narrower than long would otherwise reach the callee with
// undefined upper bits, and pointers must be reinterpreted before they can cross a variadic call.
// Its failure reporting also differs from the raw ABI: a raw syscall writes -errno into the
// return register while ::syscall collapses every failure to -1 and moves the identity into
// errno, so it is translated back to keep the same convention as the asm bodies.

#include <cerrno>
#include <unistd.h>

template<::std::size_t syscall_number, details::syscall_return return_value_type, typename... Args>
    requires (::std::is_trivially_copyable_v<Args> && ...)
[[__gnu__::__always_inline__]]
inline auto system_call(Args... arguments) noexcept -> return_value_type {
    ::std::uint64_t ret{
        static_cast<::std::uint64_t>(::syscall(syscall_number, details::syscall_argument(arguments)...))};
    if (static_cast<::std::int64_t>(ret) == -1) {
        ret = static_cast<::std::uint64_t>(0) - static_cast<::std::uint64_t>(errno);
    }
    return static_cast<return_value_type>(ret);
}
