// LoongArch syscall resembles riscv but issues the syscall instruction like x86_64.
// Adapted from fast_io_hosted/platforms/linux/loongarch64.h.
//
// Argument values are normalized through details::syscall_argument into ordinary locals first: a
// narrow 32-bit value written into the low half of a register would zero-extend, so an argument
// must never reach a register in its original static type.

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
[[__gnu__::__always_inline__]]
inline auto system_call() noexcept -> return_value_type {
    register ::std::uint64_t a7 __asm__("$a7") = syscall_number;
    register ::std::uint64_t a0 __asm__("$a0");
    __asm__ __volatile__("syscall 0"
                         : "+r"(a0)
                         : "r"(a7)
                         : "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "memory");
    return static_cast<return_value_type>(a0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
[[__gnu__::__always_inline__]]
inline auto system_call(auto p1) noexcept -> return_value_type {
    ::std::uint64_t arg0{details::syscall_argument(p1)};
    register ::std::uint64_t a7 __asm__("$a7") = syscall_number;
    register ::std::uint64_t a0 __asm__("$a0") = arg0;
    __asm__ __volatile__("syscall 0"
                         : "+r"(a0)
                         : "r"(a7)
                         : "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "memory");
    return static_cast<return_value_type>(a0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
[[__gnu__::__always_inline__]]
inline auto system_call(auto p1, auto p2) noexcept -> return_value_type {
    ::std::uint64_t arg0{details::syscall_argument(p1)};
    ::std::uint64_t arg1{details::syscall_argument(p2)};
    register ::std::uint64_t a7 __asm__("$a7") = syscall_number;
    register ::std::uint64_t a0 __asm__("$a0") = arg0;
    register ::std::uint64_t a1 __asm__("$a1") = arg1;
    __asm__ __volatile__("syscall 0"
                         : "+r"(a0)
                         : "r"(a7), "r"(a1)
                         : "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "memory");
    return static_cast<return_value_type>(a0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
[[__gnu__::__always_inline__]]
inline auto system_call(auto p1, auto p2, auto p3) noexcept -> return_value_type {
    ::std::uint64_t arg0{details::syscall_argument(p1)};
    ::std::uint64_t arg1{details::syscall_argument(p2)};
    ::std::uint64_t arg2{details::syscall_argument(p3)};
    register ::std::uint64_t a7 __asm__("$a7") = syscall_number;
    register ::std::uint64_t a0 __asm__("$a0") = arg0;
    register ::std::uint64_t a1 __asm__("$a1") = arg1;
    register ::std::uint64_t a2 __asm__("$a2") = arg2;
    __asm__ __volatile__("syscall 0"
                         : "+r"(a0)
                         : "r"(a7), "r"(a1), "r"(a2)
                         : "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "memory");
    return static_cast<return_value_type>(a0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
[[__gnu__::__always_inline__]]
inline auto system_call(auto p1, auto p2, auto p3, auto p4) noexcept -> return_value_type {
    ::std::uint64_t arg0{details::syscall_argument(p1)};
    ::std::uint64_t arg1{details::syscall_argument(p2)};
    ::std::uint64_t arg2{details::syscall_argument(p3)};
    ::std::uint64_t arg3{details::syscall_argument(p4)};
    register ::std::uint64_t a7 __asm__("$a7") = syscall_number;
    register ::std::uint64_t a0 __asm__("$a0") = arg0;
    register ::std::uint64_t a1 __asm__("$a1") = arg1;
    register ::std::uint64_t a2 __asm__("$a2") = arg2;
    register ::std::uint64_t a3 __asm__("$a3") = arg3;
    __asm__ __volatile__("syscall 0"
                         : "+r"(a0)
                         : "r"(a7), "r"(a1), "r"(a2), "r"(a3)
                         : "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "memory");
    return static_cast<return_value_type>(a0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
[[__gnu__::__always_inline__]]
inline auto system_call(auto p1, auto p2, auto p3, auto p4, auto p5) noexcept -> return_value_type {
    ::std::uint64_t arg0{details::syscall_argument(p1)};
    ::std::uint64_t arg1{details::syscall_argument(p2)};
    ::std::uint64_t arg2{details::syscall_argument(p3)};
    ::std::uint64_t arg3{details::syscall_argument(p4)};
    ::std::uint64_t arg4{details::syscall_argument(p5)};
    register ::std::uint64_t a7 __asm__("$a7") = syscall_number;
    register ::std::uint64_t a0 __asm__("$a0") = arg0;
    register ::std::uint64_t a1 __asm__("$a1") = arg1;
    register ::std::uint64_t a2 __asm__("$a2") = arg2;
    register ::std::uint64_t a3 __asm__("$a3") = arg3;
    register ::std::uint64_t a4 __asm__("$a4") = arg4;
    __asm__ __volatile__("syscall 0"
                         : "+r"(a0)
                         : "r"(a7), "r"(a1), "r"(a2), "r"(a3), "r"(a4)
                         : "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "memory");
    return static_cast<return_value_type>(a0);
}

template<::std::uint64_t syscall_number, ::std::signed_integral return_value_type>
    requires (1 < sizeof(return_value_type))
[[__gnu__::__always_inline__]]
inline auto system_call(auto p1, auto p2, auto p3, auto p4, auto p5, auto p6) noexcept -> return_value_type {
    ::std::uint64_t arg0{details::syscall_argument(p1)};
    ::std::uint64_t arg1{details::syscall_argument(p2)};
    ::std::uint64_t arg2{details::syscall_argument(p3)};
    ::std::uint64_t arg3{details::syscall_argument(p4)};
    ::std::uint64_t arg4{details::syscall_argument(p5)};
    ::std::uint64_t arg5{details::syscall_argument(p6)};
    register ::std::uint64_t a7 __asm__("$a7") = syscall_number;
    register ::std::uint64_t a0 __asm__("$a0") = arg0;
    register ::std::uint64_t a1 __asm__("$a1") = arg1;
    register ::std::uint64_t a2 __asm__("$a2") = arg2;
    register ::std::uint64_t a3 __asm__("$a3") = arg3;
    register ::std::uint64_t a4 __asm__("$a4") = arg4;
    register ::std::uint64_t a5 __asm__("$a5") = arg5;
    __asm__ __volatile__("syscall 0"
                         : "+r"(a0)
                         : "r"(a7), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5)
                         : "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "memory");
    return static_cast<return_value_type>(a0);
}

template<::std::integral I>
[[noreturn]]
inline void fast_exit(I ret) noexcept {
    [[maybe_unused]] auto const exit_code = system_call<__NR_exit, ::std::int64_t>(ret);
    __builtin_unreachable();
}
