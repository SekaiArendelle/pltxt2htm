#pragma once

// https://github.com/bminor/glibc/blob/master/sysdeps/unix/sysv/linux/x86_64/sysdep.h
//
// Argument values are normalized through details::syscall_argument into ordinary locals first: a
// narrow 32-bit value written into the low half of a register would zero-extend, so an argument
// must never reach a register in its original static type.

template<::std::uint64_t syscall_number, details::syscall_return return_value_type>
[[__gnu__::__always_inline__]]
inline auto system_call() noexcept -> return_value_type {
    ::std::uint64_t ret;
    __asm__ __volatile__("syscall" : "=a"(ret) : "0"(syscall_number) : "memory", "cc", "r11", "cx");
    return static_cast<return_value_type>(ret);
}

template<::std::uint64_t syscall_number, details::syscall_return return_value_type>
[[__gnu__::__always_inline__]]
inline auto system_call(auto p1) noexcept -> return_value_type {
    ::std::uint64_t arg1{details::syscall_argument(p1)};
    ::std::uint64_t ret;
    __asm__ __volatile__("syscall" : "=a"(ret) : "0"(syscall_number), "D"(arg1) : "memory", "cc", "r11", "cx");
    return static_cast<return_value_type>(ret);
}

template<::std::uint64_t syscall_number, details::syscall_return return_value_type>
[[__gnu__::__always_inline__]]
inline auto system_call(auto p1, auto p2) noexcept -> return_value_type {
    ::std::uint64_t arg1{details::syscall_argument(p1)};
    ::std::uint64_t arg2{details::syscall_argument(p2)};
    ::std::uint64_t ret;
    __asm__ __volatile__("syscall"
                         : "=a"(ret)
                         : "0"(syscall_number), "D"(arg1), "S"(arg2)
                         : "memory", "cc", "r11", "cx");
    return static_cast<return_value_type>(ret);
}

template<::std::uint64_t syscall_number, details::syscall_return return_value_type>
[[__gnu__::__always_inline__]]
inline auto system_call(auto p1, auto p2, auto p3) noexcept -> return_value_type {
    ::std::uint64_t arg1{details::syscall_argument(p1)};
    ::std::uint64_t arg2{details::syscall_argument(p2)};
    ::std::uint64_t arg3{details::syscall_argument(p3)};
    ::std::uint64_t ret;
    __asm__ __volatile__("syscall"
                         : "=a"(ret)
                         : "0"(syscall_number), "D"(arg1), "S"(arg2), "d"(arg3)
                         : "memory", "cc", "r11", "cx");
    return static_cast<return_value_type>(ret);
}

template<::std::uint64_t syscall_number, details::syscall_return return_value_type>
[[__gnu__::__always_inline__]]
inline auto system_call(auto p1, auto p2, auto p3, auto p4) noexcept -> return_value_type {
    ::std::uint64_t arg1{details::syscall_argument(p1)};
    ::std::uint64_t arg2{details::syscall_argument(p2)};
    ::std::uint64_t arg3{details::syscall_argument(p3)};
    ::std::uint64_t arg4{details::syscall_argument(p4)};
    register ::std::uint64_t r10 __asm__("r10") = arg4;
    ::std::uint64_t ret;
    __asm__ __volatile__("syscall"
                         : "=a"(ret)
                         : "0"(syscall_number), "D"(arg1), "S"(arg2), "d"(arg3), "r"(r10)
                         : "memory", "cc", "r11", "cx");
    return static_cast<return_value_type>(ret);
}

template<::std::uint64_t syscall_number, details::syscall_return return_value_type>
[[__gnu__::__always_inline__]]
inline auto system_call(auto p1, auto p2, auto p3, auto p4, auto p5) noexcept -> return_value_type {
    ::std::uint64_t arg1{details::syscall_argument(p1)};
    ::std::uint64_t arg2{details::syscall_argument(p2)};
    ::std::uint64_t arg3{details::syscall_argument(p3)};
    ::std::uint64_t arg4{details::syscall_argument(p4)};
    ::std::uint64_t arg5{details::syscall_argument(p5)};
    register ::std::uint64_t r10 __asm__("r10") = arg4;
    register ::std::uint64_t r8 __asm__("r8") = arg5;
    ::std::uint64_t ret;
    __asm__ __volatile__("syscall"
                         : "=a"(ret)
                         : "0"(syscall_number), "D"(arg1), "S"(arg2), "d"(arg3), "r"(r10), "r"(r8)
                         : "memory", "cc", "r11", "cx");
    return static_cast<return_value_type>(ret);
}

template<::std::uint64_t syscall_number, details::syscall_return return_value_type>
[[__gnu__::__always_inline__]]
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
    ::std::uint64_t ret;
    __asm__ __volatile__("syscall"
                         : "=a"(ret)
                         : "0"(syscall_number), "D"(arg1), "S"(arg2), "d"(arg3), "r"(r10), "r"(r8), "r"(r9)
                         : "memory", "cc", "r11", "cx");
    return static_cast<return_value_type>(ret);
}
