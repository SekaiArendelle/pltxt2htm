/*
https://github.com/riscvarchive/riscv-musl/blob/develop/arch/riscv64/syscall_arch.h
Adapted from fast_io_hosted/platforms/linux/riscv64.h.
*/

template<::std::uint64_t syscall_number>
[[noreturn]]
inline void system_call_no_return(auto p1) noexcept {
    register ::std::uint64_t a7 __asm__("a7") = syscall_number;
    register ::std::uint64_t a0 __asm__("a0") = details::syscall_argument(p1);
    __asm__ __volatile__("ecall" : "+r"(a0) : "r"(a7) : "memory");
    __builtin_unreachable();
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call() noexcept -> return_value_type {
    register ::std::uint64_t a7 __asm__("a7") = syscall_number;
    register ::std::uint64_t a0 __asm__("a0");
    __asm__ __volatile__("ecall" : "+r"(a0) : "r"(a7) : "memory");
    return static_cast<return_value_type>(a0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1) noexcept -> return_value_type {
    register ::std::uint64_t a7 __asm__("a7") = syscall_number;
    register ::std::uint64_t a0 __asm__("a0") = details::syscall_argument(p1);
    __asm__ __volatile__("ecall" : "+r"(a0) : "r"(a7) : "memory");
    return static_cast<return_value_type>(a0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1, auto p2) noexcept -> return_value_type {
    register ::std::uint64_t a7 __asm__("a7") = syscall_number;
    register ::std::uint64_t a0 __asm__("a0") = details::syscall_argument(p1);
    register ::std::uint64_t a1 __asm__("a1") = details::syscall_argument(p2);
    __asm__ __volatile__("ecall" : "+r"(a0) : "r"(a7), "r"(a1) : "memory");
    return static_cast<return_value_type>(a0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1, auto p2, auto p3) noexcept -> return_value_type {
    register ::std::uint64_t a7 __asm__("a7") = syscall_number;
    register ::std::uint64_t a0 __asm__("a0") = details::syscall_argument(p1);
    register ::std::uint64_t a1 __asm__("a1") = details::syscall_argument(p2);
    register ::std::uint64_t a2 __asm__("a2") = details::syscall_argument(p3);
    __asm__ __volatile__("ecall" : "+r"(a0) : "r"(a7), "r"(a1), "r"(a2) : "memory");
    return static_cast<return_value_type>(a0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1, auto p2, auto p3, auto p4) noexcept -> return_value_type {
    register ::std::uint64_t a7 __asm__("a7") = syscall_number;
    register ::std::uint64_t a0 __asm__("a0") = details::syscall_argument(p1);
    register ::std::uint64_t a1 __asm__("a1") = details::syscall_argument(p2);
    register ::std::uint64_t a2 __asm__("a2") = details::syscall_argument(p3);
    register ::std::uint64_t a3 __asm__("a3") = details::syscall_argument(p4);
    __asm__ __volatile__("ecall" : "+r"(a0) : "r"(a7), "r"(a1), "r"(a2), "r"(a3) : "memory");
    return static_cast<return_value_type>(a0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1, auto p2, auto p3, auto p4, auto p5) noexcept -> return_value_type {
    register ::std::uint64_t a7 __asm__("a7") = syscall_number;
    register ::std::uint64_t a0 __asm__("a0") = details::syscall_argument(p1);
    register ::std::uint64_t a1 __asm__("a1") = details::syscall_argument(p2);
    register ::std::uint64_t a2 __asm__("a2") = details::syscall_argument(p3);
    register ::std::uint64_t a3 __asm__("a3") = details::syscall_argument(p4);
    register ::std::uint64_t a4 __asm__("a4") = details::syscall_argument(p5);
    __asm__ __volatile__("ecall" : "+r"(a0) : "r"(a7), "r"(a1), "r"(a2), "r"(a3), "r"(a4) : "memory");
    return static_cast<return_value_type>(a0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
inline auto system_call(auto p1, auto p2, auto p3, auto p4, auto p5, auto p6) noexcept -> return_value_type {
    register ::std::uint64_t a7 __asm__("a7") = syscall_number;
    register ::std::uint64_t a0 __asm__("a0") = details::syscall_argument(p1);
    register ::std::uint64_t a1 __asm__("a1") = details::syscall_argument(p2);
    register ::std::uint64_t a2 __asm__("a2") = details::syscall_argument(p3);
    register ::std::uint64_t a3 __asm__("a3") = details::syscall_argument(p4);
    register ::std::uint64_t a4 __asm__("a4") = details::syscall_argument(p5);
    register ::std::uint64_t a5 __asm__("a5") = details::syscall_argument(p6);
    __asm__ __volatile__("ecall" : "+r"(a0) : "r"(a7), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5) : "memory");
    return static_cast<return_value_type>(a0);
}

template<::std::integral I>
[[noreturn]]
inline void fast_exit(I ret) noexcept {
    system_call_no_return<__NR_exit>(ret);
}
