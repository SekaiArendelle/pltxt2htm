// Generic fallback without inline assembly: the libc ::syscall() wrapper is used instead.
// Adapted from fast_io_hosted/platforms/linux/generic.h.
//
// ::syscall() is a C variadic, so every argument is first normalized into a register-width value
// by details::syscall_argument; integers narrower than long would otherwise reach the callee with
// undefined upper bits, and pointers must be reinterpreted before they can cross a variadic call.
template<::std::size_t syscall_number, ::std::signed_integral return_value_type, typename... Args>
    requires (::std::is_trivially_copyable_v<Args> && ...)
[[__gnu__::__always_inline__]]
inline auto system_call(Args... arguments) noexcept -> return_value_type {
    long ret{::syscall(syscall_number, details::syscall_argument(arguments)...)};
    return static_cast<return_value_type>(ret);
}

template<::std::integral I>
[[noreturn]]
inline void fast_exit(I ret) noexcept {
    [[maybe_unused]] auto const exit_code = system_call<__NR_exit, ::std::int64_t>(ret);
    __builtin_unreachable();
}
