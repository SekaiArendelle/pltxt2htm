#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <utility>

namespace pltxt2htm::details::stacktrace {
/** Minimal fallible scratch storage for the panic path. It deliberately has no
 * contract checks and is not part of the public container API. */
class NativeTextBuffer {
    char* pointer{};
    ::std::size_t length{};

public:
    constexpr NativeTextBuffer() noexcept = default;
    NativeTextBuffer(NativeTextBuffer const&) = delete;

    constexpr NativeTextBuffer(NativeTextBuffer&& other) noexcept
        : pointer{::std::exchange(other.pointer, nullptr)},
          length{::std::exchange(other.length, 0)} {
    }

    NativeTextBuffer& operator=(NativeTextBuffer const&) = delete;

    constexpr auto operator=(this NativeTextBuffer& self, NativeTextBuffer&& other) noexcept -> NativeTextBuffer& {
        if (::std::addressof(self) == ::std::addressof(other)) {
            return self;
        }
        ::std::free(self.pointer);
        self.pointer = ::std::exchange(other.pointer, nullptr);
        self.length = ::std::exchange(other.length, 0);
        return self;
    }

    constexpr ~NativeTextBuffer() {
        ::std::free(pointer);
    }

    [[nodiscard]]
    constexpr auto data(this NativeTextBuffer const& self) noexcept -> char const* {
        return self.pointer == nullptr ? "" : self.pointer;
    }

    [[nodiscard]]
    constexpr auto size(this NativeTextBuffer const& self) noexcept -> ::std::size_t {
        return self.length;
    }

    [[nodiscard]]
    constexpr auto empty(this NativeTextBuffer const& self) noexcept -> bool {
        return self.length == 0;
    }

    constexpr void clear(this NativeTextBuffer& self) noexcept {
        self.length = 0;
        if (self.pointer != nullptr) {
            self.pointer[0] = '\0';
        }
    }

    [[nodiscard]]
    auto assign(this NativeTextBuffer& self, char const* source, ::std::size_t size) noexcept -> bool {
        if (size == (::std::numeric_limits<::std::size_t>::max)()) {
            return false;
        }
        auto* allocation = static_cast<char*>(::std::realloc(self.pointer, size + 1));
        if (allocation == nullptr) {
            return false;
        }
        self.pointer = allocation;
        ::std::memcpy(self.pointer, source, size);
        self.pointer[size] = '\0';
        self.length = size;
        return true;
    }
};

/** Resolver-owned text used below the pltxt2htm container layer. */
struct NativeResolvedFrame {
    void* address{};
    NativeTextBuffer description{};
    NativeTextBuffer source_file{};
    ::std::uint64_t displacement{};
    unsigned long source_line{};
    bool text_truncated{};
};

inline auto assign_text(NativeTextBuffer& destination, char const* source) noexcept -> bool {
    if (source == nullptr) {
        destination.clear();
        return true;
    }
    return destination.assign(source, ::std::strlen(source));
}

} // namespace pltxt2htm::details::stacktrace
