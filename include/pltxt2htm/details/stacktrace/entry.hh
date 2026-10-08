#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace pltxt2htm::details::stacktrace {
/** Owning symbol data: no pointers into a resolver session or temporary buffer.
 * Text is bounded and null-terminated; truncation is reported explicitly.
 */
struct ResolvedFrame {
    void* address{};
    char description[1024]{};
    char source_file[2048]{};
    ::std::uint64_t displacement{};
    unsigned long source_line{};
    bool text_truncated{};
};

/** Bounded view also permits safe formatting of externally populated records
 * whose character storage lacks a null terminator.
 */
[[nodiscard]]
constexpr ::std::span<char const> text_view(::std::span<char const> storage) noexcept {
    auto const capacity = storage.size();
    ::std::size_t size{};
    while (size < capacity && storage[size] != '\0') {
        ++size;
    }
    return storage.first(size);
}

[[nodiscard]]
constexpr bool copy_text(::std::span<char> destination, char const* source) noexcept {
    if (source == nullptr || destination.empty()) {
        return false;
    }
    auto const capacity = destination.size() - 1;
    ::std::size_t size{};
    while (size < capacity && source[size] != '\0') {
        destination[size] = source[size];
        ++size;
    }
    destination[size] = '\0';
    return source[size] != '\0';
}
} // namespace pltxt2htm::details::stacktrace
