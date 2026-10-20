/**
 * @file allocation_least_result.hh
 * @brief Result type of `allocate_at_least`-style operations.
 * @details Synced from fast_io `fast_io_core_impl/allocation/common.h` (the
 *          win32 structures there are not needed here).
 */

#pragma once

#include <cstddef>

namespace pltxt2htm::memory {

template<typename Pointer>
struct basic_allocation_least_result {
    using pointer = Pointer;
    using size_type = ::std::size_t;
    pointer ptr;
    size_type count;
};

using allocation_least_result = basic_allocation_least_result<void*>;

} // namespace pltxt2htm::memory
