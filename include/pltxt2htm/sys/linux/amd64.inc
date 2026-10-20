// Adapted from fast_io_hosted/platforms/linux/amd64.h.
// https://github.com/bminor/glibc/master/sysdeps/unix/sysv/linux/x86_64/sysdep.h
//
// Argument values are normalized through details::syscall_argument into ordinary locals first: a
// narrow 32-bit value written into the low half of a register would zero-extend, so an argument
// must never reach a register in its original static type. rdi/rsi/rdx have constraint letters and
// are bound that way, which loads the normalized values straight into place when the asm statement
// is prepared; r10/r8/r9 have no constraint letters and rely on the GCC/Clang
// local-register-variable extension, pinned consecutively with no intervening statements because
// low optimization levels would otherwise reuse those registers and clobber an earlier pin.

template<::std::uint64_t syscall_number>
[[noreturn]]
inline void system_call_no_return(auto p1) noexcept {
    ::std::uint64_t arg1{details::syscall_argument(p1)};
    ::std::uint64_t ret;
    __asm__ __volatile__("syscall" : "=a"(ret) : "0"(syscall_number), "D"(arg1) : "memory", "cc", "r11", "cx");
    __builtin_unreachable();
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call() noexcept -> return_value_type {
    return_value_type ret;
    __asm__ __volatile__("syscall" : "=a"(ret) : "0"(syscall_number) : "memory", "cc", "r11", "cx");
    return ret;
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1) noexcept -> return_value_type {
    ::std::uint64_t arg1{details::syscall_argument(p1)};
    return_value_type ret;
    __asm__ __volatile__("syscall" : "=a"(ret) : "0"(syscall_number), "D"(arg1) : "memory", "cc", "r11", "cx");
    return ret;
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1, auto p2) noexcept -> return_value_type {
    ::std::uint64_t arg1{details::syscall_argument(p1)};
    ::std::uint64_t arg2{details::syscall_argument(p2)};
    return_value_type ret;
    __asm__ __volatile__("syscall"
                         : "=a"(ret)
                         : "0"(syscall_number), "D"(arg1), "S"(arg2)
                         : "memory", "cc", "r11", "cx");
    return ret;
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1, auto p2, auto p3) noexcept -> return_value_type {
    ::std::uint64_t arg1{details::syscall_argument(p1)};
    ::std::uint64_t arg2{details::syscall_argument(p2)};
    ::std::uint64_t arg3{details::syscall_argument(p3)};
    return_value_type ret;
    __asm__ __volatile__("syscall"
                         : "=a"(ret)
                         : "0"(syscall_number), "D"(arg1), "S"(arg2), "d"(arg3)
                         : "memory", "cc", "r11", "cx");
    return ret;
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1, auto p2, auto p3, auto p4) noexcept -> return_value_type {
    ::std::uint64_t arg1{details::syscall_argument(p1)};
    ::std::uint64_t arg2{details::syscall_argument(p2)};
    ::std::uint64_t arg3{details::syscall_argument(p3)};
    ::std::uint64_t arg4{details::syscall_argument(p4)};
    register ::std::uint64_t r10 __asm__("r10") = arg4;
    return_value_type ret;
    __asm__ __volatile__("syscall"
                         : "=a"(ret)
                         : "0"(syscall_number), "D"(arg1), "S"(arg2), "d"(arg3), "r"(r10)
                         : "memory", "cc", "r11", "cx");
    return ret;
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1, auto p2, auto p3, auto p4, auto p5) noexcept -> return_value_type {
    ::std::uint64_t arg1{details::syscall_argument(p1)};
    ::std::uint64_t arg2{details::syscall_argument(p2)};
    ::std::uint64_t arg3{details::syscall_argument(p3)};
    ::std::uint64_t arg4{details::syscall_argument(p4)};
    ::std::uint64_t arg5{details::syscall_argument(p5)};
    register ::std::uint64_t r10 __asm__("r10") = arg4;
    register ::std::uint64_t r8 __asm__("r8") = arg5;
    return_value_type ret;
    __asm__ __volatile__("syscall"
                         : "=a"(ret)
                         : "0"(syscall_number), "D"(arg1), "S"(arg2), "d"(arg3), "r"(r10), "r"(r8)
                         : "memory", "cc", "r11", "cx");
    return ret;
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1, auto p2, auto p3, auto p4, auto p5, auto p6) noexcept -> return_value_type {
    ::std::uint64_t arg1{details::syscall_argument(p1)};
    ::std::uint64_t arg2{details::syscall_argument(p2)};
    ::std::uint64_t arg3{details::syscall_argument(p3)};
    ::std::uint64_t arg4{details::syscall_argument(p4)};
    ::std::uint64_t arg5{details::syscall_argument(p5)};
    ::std::uint64_t arg6{details::syscall_argument(p6)};
    register ::std::uint64_t r10 __asm__("r10") = arg4;
    register ::std::uint64_t r8 __asm__("r8") = arg5;
    register ::std::uint64_t r9 __asm__("r9") = arg6;
    return_value_type ret;
    __asm__ __volatile__("syscall"
                         : "=a"(ret)
                         : "0"(syscall_number), "D"(arg1), "S"(arg2), "d"(arg3), "r"(r10), "r"(r8), "r"(r9)
                         : "memory", "cc", "r11", "cx");
    return ret;
}

template<::std::integral I>
[[noreturn]]
inline void fast_exit(I ret) noexcept {
    system_call_no_return<__NR_exit>(ret);
}
