/**
 * @file text.hh
 * @brief Owning native text for the stacktrace implementation.
 * @details Deliberately independent of the pltxt2htm container layer. The panic
 *          path includes stacktrace headers while container headers include the
 *          panic machinery, so stacktrace text cannot depend on
 *          `container::String`.
 * @warning Never include `container/` headers or `details/push_macro.hh` from
 *          `pltxt2htm/stacktrace/`; that would recreate the include cycle.
 */

#pragma once

#include <cstddef>
#include <cstring>
#include <limits>
#include <memory>

#include <fast_io/fast_io_core.h>

#include "details/contract.hh"

namespace pltxt2htm::stacktrace {

/**
 * @brief Move-only text owned by fast_io's native global allocator.
 * @details Only the operations used by stacktrace are provided. Copies are
 *          rejected so the owning semantics stay explicit; callers move values
 *          out of resolved frames instead of duplicating them. Allocation
 *          failure terminates, as with the pltxt2htm container layer.
 */
class Text {
    using text_allocator = ::fast_io::typed_generic_allocator_adapter<::fast_io::native_global_allocator, char>;

    char* pointer{};
    ::std::size_t length{};

public:
    constexpr Text() noexcept = default;

    constexpr Text(Text const&) noexcept = delete;

    constexpr Text(Text&& other) noexcept
        : pointer{other.pointer},
          length{other.length} {
        other.pointer = nullptr;
        other.length = 0;
    }

    constexpr Text& operator=(Text const&) noexcept = delete;

    constexpr auto operator=(this Text& self, Text&& other) noexcept -> Text& {
        if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
            details::contract_violation();
        }
        self.swap(other);
        return self;
    }

    constexpr ~Text() noexcept {
        if (pointer != nullptr) {
            text_allocator::deallocate(pointer);
        }
    }

    constexpr void swap(this Text& self, Text& other) noexcept {
        auto const tmp_pointer = self.pointer;
        auto const tmp_length = self.length;
        self.pointer = other.pointer;
        self.length = other.length;
        other.pointer = tmp_pointer;
        other.length = tmp_length;
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
     *       Constant evaluation constructs each character explicitly because the evaluator
     *       tracks object lifetimes; the runtime path copies with `memcpy`.
     */
    [[nodiscard]]
    constexpr auto assign(this Text& self, char const* source, ::std::size_t size) noexcept -> bool {
        if (size == (::std::numeric_limits<::std::size_t>::max)()) {
            return false;
        }
        auto const allocation = text_allocator::allocate_at_least(size + 1);
        if consteval {
            for (::std::size_t index{}; index < size; ++index) {
                ::std::construct_at(allocation.ptr + index, source[index]);
            }
            ::std::construct_at(allocation.ptr + size, '\0');
        }
        else {
            if (size != 0) {
                ::std::memcpy(allocation.ptr, source, size);
            }
            allocation.ptr[size] = '\0';
        }
        if (self.pointer != nullptr) {
            text_allocator::deallocate(self.pointer);
        }
        self.pointer = allocation.ptr;
        self.length = size;
        return true;
    }

    /** Replace the contents from a NUL-terminated string; nullptr clears it. */
    [[nodiscard]]
    constexpr auto assign(this Text& self, char const* source) noexcept -> bool {
        if (source == nullptr) {
            self.clear();
            return true;
        }
        return self.assign(source, ::fast_io::cstr_len(source));
    }
};

} // namespace pltxt2htm::stacktrace
