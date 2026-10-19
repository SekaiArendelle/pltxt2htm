// Generic fallback without inline assembly: the libc ::syscall() wrapper is used instead.
// Adapted from fast_io_hosted/platforms/linux/generic.h.

template<::std::size_t syscall_number, ::std::signed_integral return_value_type, typename... Args>
    requires (::std::is_trivially_copyable_v<Args> && ...)
inline auto system_call(Args... arguments) noexcept -> return_value_type {
    long ret{::syscall(syscall_number, arguments...)};
    return static_cast<return_value_type>(ret);
}

template<::std::size_t syscall_number>
[[noreturn]]
inline void system_call_no_return(auto p1) noexcept {
    ::syscall(syscall_number, p1);
    __builtin_unreachable();
}

template<::std::integral I>
[[noreturn]]
inline void fast_exit(I ret) noexcept {
    system_call_no_return<__NR_exit>(ret);
}
