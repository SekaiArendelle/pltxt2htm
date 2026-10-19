#pragma once

#if !defined(__linux__)
    #error "pltxt2htm Linux syscall symbols are only available on Linux"
#endif

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <unistd.h>
#include <asm/unistd.h>
#include <sys/syscall.h>

#include "../../details/trap.hh"

// The arch .inc files use `register` with asm labels, the GCC/Clang local-register-variable
// extension required to bind syscall arguments to their ABI registers.

namespace pltxt2htm::sys_linux {

namespace details {

/**
 * @brief Normalizes a syscall argument into an unsigned 64-bit integer.
 * @details Kernel syscall arguments are passed in registers; pointers are reinterpreted to
 *          integers, integral arguments are converted with the same value semantics as the
 *          C-style casts used by the original fast_io implementation.
 * @param v The argument in its original form.
 * @return The value to load into a syscall argument register or forward to ::syscall.
 */
template<typename T>
[[nodiscard]]
constexpr ::std::uint_least64_t syscall_argument(T v) noexcept {
    if constexpr (::std::is_null_pointer_v<T>) {
        return static_cast<::std::uint_least64_t>(0);
    }
    else if constexpr (::std::is_pointer_v<T>) {
        return reinterpret_cast<::std::uint_least64_t>(v);
    }
    else {
        return static_cast<::std::uint_least64_t>(v);
    }
}

} // namespace details

#if defined(__x86_64__)
    #include "amd64.inc"
#elif defined(__arm64__) || defined(__aarch64__)
    #include "aarch64.inc"
#elif defined(__riscv) && __SIZEOF_SIZE_T__ == 8
    #include "riscv64.inc"
#elif defined(__loongarch__) && __SIZEOF_SIZE_T__ == 8
    #include "loongarch64.inc"
#else
    #include "generic.inc"
#endif

/**
 * @brief Checks a raw Linux syscall return value for failure.
 * @details Successful syscalls return a non-negative value; failures return the negated errno in
 *          the range [-4095, -1].
 * @param v The raw syscall return value.
 * @return True when v reports a syscall failure.
 */
template<::std::integral int_type>
[[nodiscard]]
constexpr bool system_call_fails(int_type v) noexcept {
    using unsigned_type = ::std::make_unsigned_t<int_type>;
    return static_cast<unsigned_type>(static_cast<unsigned_type>(v) + static_cast<unsigned_type>(4095)) <
           static_cast<unsigned_type>(4095);
}

/**
 * @brief Terminates the program when a raw Linux syscall return value reports failure.
 * @details pltxt2htm core code never throws exceptions, so unlike the fast_io counterpart this
 *          goes straight to ::pltxt2htm::details::trap() instead of throwing a posix error.
 * @param v The raw syscall return value.
 */
template<::std::integral int_type>
inline void system_call_must_succeed(int_type v) noexcept {
    if (system_call_fails(v)) [[unlikely]] {
        ::pltxt2htm::details::trap();
    }
}

} // namespace pltxt2htm::sys_linux
