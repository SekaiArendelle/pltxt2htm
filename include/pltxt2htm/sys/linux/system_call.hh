#pragma once

#if !defined(__linux__)
    #error "pltxt2htm Linux syscall symbols are only available on Linux"
#endif

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <asm/unistd.h>

// The arch .inc files use `register` with asm labels, the GCC/Clang local-register-variable
// extension required to bind syscall arguments to their ABI registers. This layer is the lowest
// substrate of pltxt2htm and deliberately depends on nothing above raw compiler builtins, so the
// [[noreturn]] paths use __builtin_unreachable directly instead of the pltxt2htm
// details::unreachable() wrapper.

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
constexpr auto syscall_argument(T v) noexcept -> ::std::uint64_t {
    if constexpr (::std::is_null_pointer_v<T>) {
        return static_cast<::std::uint64_t>(0);
    }
    else if constexpr (::std::is_pointer_v<T>) {
        return reinterpret_cast<::std::uint64_t>(v);
    }
    else {
        return static_cast<::std::uint64_t>(v);
    }
}

/**
 * @brief Constrains the return slot of system_call.
 * @details Every syscall writes exactly one register-width result, so the ABI itself has no void
 *          shape. void is the meaningful spelling for "the caller never consumes the result",
 *          typically because the syscall never returns here (exit), and other returns may be
 *          signed integrals wider than a byte, interpreted by two's-complement errno convention.
 * @param T The requested return slot of a system_call invocation.
 */
template<typename T>
concept syscall_return = ::std::is_void_v<T> || (::std::signed_integral<T> && (1 < sizeof(T)));

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
 * @brief Exits the process immediately with the given exit code.
 * @details A thin wrapper over the exit syscall, defined here rather than per architecture because
 *          it is only a concrete use of system_call and its shape is identical everywhere. The
 *          discarded syscall result is spelled as a void instantiation instead of a
 *          [[maybe_unused]] local: void is simply the honest reading of the ABI return slot here.
 * @param ret The process exit code.
 */
template<::std::integral I>
[[noreturn]]
inline void fast_exit(I ret) noexcept {
    system_call<__NR_exit, void>(ret);
    __builtin_unreachable();
}

/**
 * @brief Checks a raw Linux syscall return value for failure.
 * @details Successful syscalls return a non-negative value; failures return the negated errno in
 *          the range [-4095, -1].
 * @param v The raw syscall return value.
 * @return True when v reports a syscall failure.
 */
template<::std::integral int_type>
[[nodiscard]]
constexpr auto system_call_fails(int_type v) noexcept -> bool {
    using unsigned_type = ::std::make_unsigned_t<int_type>;
    return static_cast<unsigned_type>(static_cast<unsigned_type>(v) + static_cast<unsigned_type>(4095)) <
           static_cast<unsigned_type>(4095);
}

} // namespace pltxt2htm::sys_linux
