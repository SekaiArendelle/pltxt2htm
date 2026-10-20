// Adapted from fast_io_hosted/platforms/linux/aarch64.h.
//
// Argument values are normalized through details::syscall_argument into ordinary locals first, and
// only then are the ABI registers bound consecutively with no intervening statements: at low
// optimization levels the compiler reuses those same registers (x0 above all) for the
// normalization calls of later arguments, which would silently clobber an earlier pin.

template<::std::uint64_t syscall_number>
[[noreturn]]
inline void system_call_no_return(auto p1) noexcept {
    ::std::uint64_t arg0 = details::syscall_argument(p1);
    register ::std::uint64_t x8 __asm__("x8") = syscall_number;
    register ::std::uint64_t x0 __asm__("x0") = arg0;
    __asm__ __volatile__("svc 0" : "=r"(x0) : "r"(x8), "0"(x0) : "memory", "cc");
    __builtin_unreachable();
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call() noexcept -> return_value_type {
    register ::std::uint64_t x8 __asm__("x8") = syscall_number;
    register ::std::uint64_t x0 __asm__("x0");
    __asm__ __volatile__("svc 0" : "=r"(x0) : "r"(x8) : "memory", "cc");
    return static_cast<return_value_type>(x0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1) noexcept -> return_value_type {
    ::std::uint64_t arg0 = details::syscall_argument(p1);
    register ::std::uint64_t x8 __asm__("x8") = syscall_number;
    register ::std::uint64_t x0 __asm__("x0") = arg0;
    __asm__ __volatile__("svc 0" : "=r"(x0) : "r"(x8), "0"(x0) : "memory", "cc");
    return static_cast<return_value_type>(x0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1, auto p2) noexcept -> return_value_type {
    ::std::uint64_t arg0 = details::syscall_argument(p1);
    ::std::uint64_t arg1 = details::syscall_argument(p2);
    register ::std::uint64_t x8 __asm__("x8") = syscall_number;
    register ::std::uint64_t x0 __asm__("x0") = arg0;
    register ::std::uint64_t x1 __asm__("x1") = arg1;
    __asm__ __volatile__("svc 0" : "=r"(x0) : "r"(x8), "0"(x0), "r"(x1) : "memory", "cc");
    return static_cast<return_value_type>(x0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1, auto p2, auto p3) noexcept -> return_value_type {
    ::std::uint64_t arg0 = details::syscall_argument(p1);
    ::std::uint64_t arg1 = details::syscall_argument(p2);
    ::std::uint64_t arg2 = details::syscall_argument(p3);
    register ::std::uint64_t x8 __asm__("x8") = syscall_number;
    register ::std::uint64_t x0 __asm__("x0") = arg0;
    register ::std::uint64_t x1 __asm__("x1") = arg1;
    register ::std::uint64_t x2 __asm__("x2") = arg2;
    __asm__ __volatile__("svc 0" : "=r"(x0) : "r"(x8), "0"(x0), "r"(x1), "r"(x2) : "memory", "cc");
    return static_cast<return_value_type>(x0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1, auto p2, auto p3, auto p4) noexcept -> return_value_type {
    ::std::uint64_t arg0 = details::syscall_argument(p1);
    ::std::uint64_t arg1 = details::syscall_argument(p2);
    ::std::uint64_t arg2 = details::syscall_argument(p3);
    ::std::uint64_t arg3 = details::syscall_argument(p4);
    register ::std::uint64_t x8 __asm__("x8") = syscall_number;
    register ::std::uint64_t x0 __asm__("x0") = arg0;
    register ::std::uint64_t x1 __asm__("x1") = arg1;
    register ::std::uint64_t x2 __asm__("x2") = arg2;
    register ::std::uint64_t x3 __asm__("x3") = arg3;
    __asm__ __volatile__("svc 0" : "=r"(x0) : "r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3) : "memory", "cc");
    return static_cast<return_value_type>(x0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1, auto p2, auto p3, auto p4, auto p5) noexcept -> return_value_type {
    ::std::uint64_t arg0 = details::syscall_argument(p1);
    ::std::uint64_t arg1 = details::syscall_argument(p2);
    ::std::uint64_t arg2 = details::syscall_argument(p3);
    ::std::uint64_t arg3 = details::syscall_argument(p4);
    ::std::uint64_t arg4 = details::syscall_argument(p5);
    register ::std::uint64_t x8 __asm__("x8") = syscall_number;
    register ::std::uint64_t x0 __asm__("x0") = arg0;
    register ::std::uint64_t x1 __asm__("x1") = arg1;
    register ::std::uint64_t x2 __asm__("x2") = arg2;
    register ::std::uint64_t x3 __asm__("x3") = arg3;
    register ::std::uint64_t x4 __asm__("x4") = arg4;
    __asm__ __volatile__("svc 0" : "=r"(x0) : "r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4) : "memory", "cc");
    return static_cast<return_value_type>(x0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1, auto p2, auto p3, auto p4, auto p5, auto p6) noexcept -> return_value_type {
    ::std::uint64_t arg0 = details::syscall_argument(p1);
    ::std::uint64_t arg1 = details::syscall_argument(p2);
    ::std::uint64_t arg2 = details::syscall_argument(p3);
    ::std::uint64_t arg3 = details::syscall_argument(p4);
    ::std::uint64_t arg4 = details::syscall_argument(p5);
    ::std::uint64_t arg5 = details::syscall_argument(p6);
    register ::std::uint64_t x8 __asm__("x8") = syscall_number;
    register ::std::uint64_t x0 __asm__("x0") = arg0;
    register ::std::uint64_t x1 __asm__("x1") = arg1;
    register ::std::uint64_t x2 __asm__("x2") = arg2;
    register ::std::uint64_t x3 __asm__("x3") = arg3;
    register ::std::uint64_t x4 __asm__("x4") = arg4;
    register ::std::uint64_t x5 __asm__("x5") = arg5;
    __asm__ __volatile__("svc 0"
                         : "=r"(x0)
                         : "r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5)
                         : "memory", "cc");
    return static_cast<return_value_type>(x0);
}

template<::std::integral I>
[[noreturn]]
inline void fast_exit(I ret) noexcept {
    system_call_no_return<__NR_exit>(ret);
}
