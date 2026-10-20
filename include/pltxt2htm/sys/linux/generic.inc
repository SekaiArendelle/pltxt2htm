// Generic fallback without inline assembly: the libc ::syscall() wrapper is used instead.
// Adapted from fast_io_hosted/platforms/linux/generic.h.
//
// ::syscall() is a C variadic, so every argument is first normalized into a register-width value
// by details::syscall_argument; integers narrower than long would otherwise reach the callee with
// undefined upper bits, and pointers must be reinterpreted before they can cross a variadic call.
template<::std::size_t syscall_number, details::syscall_return return_value_type, typename... Args>
    requires (::std::is_trivially_copyable_v<Args> && ...)
[[__gnu__::__always_inline__]]
inline auto system_call(Args... arguments) noexcept -> return_value_type {
    ::std::uint64_t ret{
        static_cast<::std::uint64_t>(::syscall(syscall_number, details::syscall_argument(arguments)...))};
    return static_cast<return_value_type>(ret);
}
