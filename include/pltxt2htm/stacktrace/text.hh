/**
 * @file text.hh
 * @brief Owning native text for the stacktrace implementation.
 * @details Deliberately independent of the pltxt2htm container layer. The panic
 *          path includes stacktrace headers while container headers include the
 *          panic machinery, so stacktrace text cannot depend on
 *          `container::String`.
 * @warning Never include `container/*` or `details/push_macro.hh` from
 *          `pltxt2htm/stacktrace/`; that would recreate the include cycle.
 */

#pragma once

#include <cstddef>
#include <cstring>
#include <limits>
#include <memory>
#include <utility>

#include <fast_io/fast_io_core.h>

namespace pltxt2htm::stacktrace {

/**
 * @brief Move-only text owned by fast_io's native global allocator.
 * @details Only the operations used by stacktrace are provided. Copies are
 *          rejected so the owning semantics stay explicit; callers move values
 *          out of resolved frames instead of duplicating them. Allocation
 *          failure terminates, as with the pltxt2htm container layer.
 */
class Text {
    using text_allocator = ::fast_io::native_global_allocator;

    char* pointer{};
    ::std::size_t length{};

public:
    constexpr Text() noexcept = default;
    Text(Text const&) = delete;

    constexpr Text(Text&& other) noexcept
        : pointer{::std::exchange(other.pointer, nullptr)},
          length{::std::exchange(other.length, 0)} {
    }

    Text& operator=(Text const&) = delete;

    constexpr auto operator=(this Text& self, Text&& other) noexcept -> Text& {
        if (::std::addressof(self) == ::std::addressof(other)) {
            return self;
        }
        if (self.pointer != nullptr) {
            text_allocator::deallocate(self.pointer);
        }
        self.pointer = ::std::exchange(other.pointer, nullptr);
        self.length = ::std::exchange(other.length, 0);
        return self;
    }

    constexpr ~Text() {
        if (pointer != nullptr) {
            text_allocator::deallocate(pointer);
        }
    }

    [[nodiscard]]
    constexpr auto data(this Text const& self) noexcept -> char const* {
        return self.pointer == nullptr ? "" : self.pointer;
    }

    [[nodiscard]]
    constexpr auto size(this Text const& self) noexcept -> ::std::size_t {
        return self.length;
    }

    [[nodiscard]]
    constexpr auto empty(this Text const& self) noexcept -> bool {
        return self.length == 0;
    }

    constexpr void clear(this Text& self) noexcept {
        self.length = 0;
        if (self.pointer != nullptr) {
            self.pointer[0] = '\0';
        }
    }

    /** Replace the contents; false means the size is unrepresentable and the value is unchanged.
     * @note `source` may alias the current buffer; the new allocation is filled before the
     *       old one is released. Allocation failure terminates through the allocator.
     */
    [[nodiscard]]
    auto assign(this Text& self, char const* source, ::std::size_t size) noexcept -> bool {
        if (size == (::std::numeric_limits<::std::size_t>::max)()) {
            return false;
        }
        auto* allocation = static_cast<char*>(text_allocator::allocate(size + 1));
        if (size != 0) {
            ::std::memcpy(allocation, source, size);
        }
        allocation[size] = '\0';
        if (self.pointer != nullptr) {
            text_allocator::deallocate(self.pointer);
        }
        self.pointer = allocation;
        self.length = size;
        return true;
    }

    /** Replace the contents from a NUL-terminated string; nullptr clears it. */
    [[nodiscard]]
    auto assign(this Text& self, char const* source) noexcept -> bool {
        if (source == nullptr) {
            self.clear();
            return true;
        }
        return self.assign(source, ::std::strlen(source));
    }
};

} // namespace pltxt2htm::stacktrace
