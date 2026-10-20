/**
 * @file byte_access.hh
 * @brief Minimal byte-level helpers, remapped from fast_io's freestanding layer
 *        (`freestanding/bytes.h`). Constexpr allocation paths use `::std::allocator` directly, mirroring fast_io's
 * `freestanding::allocator` which is an alias of `std::allocator`.
 */

#pragma once

#include <cstddef>
#include <cstring>

#include "../../details/trap.hh"

namespace pltxt2htm::memory::details {

inline constexpr ::std::byte* bytes_clear_n(::std::byte* data, ::std::size_t size) noexcept {
    if (__builtin_is_constant_evaluated()) {
        for (::std::size_t i{}; i != size; ++i) {
            data[i] = ::std::byte{};
        }
    }
    else {
#if defined(__has_builtin) && __has_builtin(__builtin_memset)
        __builtin_memset(data, 0, size);
#else
        ::std::memset(data, 0, size);
#endif
    }
    return data + size;
}

inline constexpr ::std::byte* nonoverlapped_bytes_copy_n(::std::byte const* first, ::std::size_t n,
                                                         ::std::byte* dest) noexcept {
    if (__builtin_is_constant_evaluated()) {
        for (::std::size_t i{}; i != n; ++i) {
            dest[i] = first[i];
        }
    }
    else {
        if (n) [[likely]] {
#if defined(__has_builtin) && __has_builtin(__builtin_memcpy)
            __builtin_memcpy(dest, first, n);
#else
            ::std::memcpy(dest, first, n);
#endif
        }
    }
    return dest + n;
}

} // namespace pltxt2htm::memory::details
