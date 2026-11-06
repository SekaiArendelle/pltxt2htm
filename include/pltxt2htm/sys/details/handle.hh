#pragma once

#include <cstdint>

namespace pltxt2htm::sys::details {

/**
 * @brief Opaque handle value shared by the Win32 and NT layers.
 * @details Handles are opaque to callers: the value refers to an object that
 *          the operating system or the loader resolves, and callers never
 *          dereference it. Storing the value as an integer matches that model.
 *          The type is an aggregate, so it can only be built with braced
 *          initialization such as `Handle{-1}`; there is no implicit conversion
 *          from an integer or pointer, and none back to either.
 * @note This is deliberately an aggregate without user-provided (non-defaulted)
 *       constructors or a destructor. The Microsoft ABI returns classes with
 *       such declarations through a hidden pointer, while the imported
 *       functions return the SDK's handle in a register; a defaulted
 *       constructor is trivial and keeps the register return. Preserving the
 *       aggregate shape therefore preserves the calling convention.
 */
struct Handle {
    ::std::intptr_t value;

    /**
     * @brief Whether this is the null handle.
     */
    [[nodiscard]]
    constexpr auto is_null(this Handle const& self) noexcept -> bool {
        return self.value == 0;
    }

    [[nodiscard]]
    constexpr auto operator==(this Handle const& lhs, Handle rhs) noexcept -> bool {
        return lhs.value == rhs.value;
    }
};

} // namespace pltxt2htm::sys::details