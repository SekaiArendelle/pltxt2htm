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
    char* data_{};
    ::std::size_t size_{};

public:
    constexpr NativeTextBuffer() noexcept = default;
    NativeTextBuffer(NativeTextBuffer const&) = delete;

    constexpr NativeTextBuffer(NativeTextBuffer&& other) noexcept
        : data_{::std::exchange(other.data_, nullptr)},
          size_{::std::exchange(other.size_, 0)} {
    }

    NativeTextBuffer& operator=(NativeTextBuffer const&) = delete;

    constexpr auto operator=(this NativeTextBuffer& self, NativeTextBuffer&& other) noexcept -> NativeTextBuffer& {
        if (::std::addressof(self) == ::std::addressof(other)) {
            return self;
        }
        ::std::free(self.data_);
        self.data_ = ::std::exchange(other.data_, nullptr);
        self.size_ = ::std::exchange(other.size_, 0);
        return self;
    }

    constexpr ~NativeTextBuffer() {
        ::std::free(data_);
    }

    [[nodiscard]] constexpr auto data(this NativeTextBuffer const& self) noexcept -> char const* {
        return self.data_ == nullptr ? "" : self.data_;
    }

    [[nodiscard]] constexpr auto c_str(this NativeTextBuffer const& self) noexcept -> char const* {
        return self.data();
    }

    [[nodiscard]] constexpr auto size(this NativeTextBuffer const& self) noexcept -> ::std::size_t {
        return self.size_;
    }

    [[nodiscard]] constexpr bool empty(this NativeTextBuffer const& self) noexcept {
        return self.size_ == 0;
    }

    constexpr void clear(this NativeTextBuffer& self) noexcept {
        self.size_ = 0;
        if (self.data_ != nullptr) {
            self.data_[0] = '\0';
        }
    }

    [[nodiscard]] bool assign(this NativeTextBuffer& self, char const* source, ::std::size_t size) noexcept {
        if (size == (::std::numeric_limits<::std::size_t>::max)()) {
            return false;
        }
        auto* allocation = static_cast<char*>(::std::realloc(self.data_, size + 1));
        if (allocation == nullptr) {
            return false;
        }
        self.data_ = allocation;
        ::std::memcpy(self.data_, source, size);
        self.data_[size] = '\0';
        self.size_ = size;
        return true;
    }

    [[nodiscard]] bool append(this NativeTextBuffer& self, char const* source, ::std::size_t size) noexcept {
        if (size > (::std::numeric_limits<::std::size_t>::max)() - self.size_ - 1) {
            return false;
        }
        auto const old_size = self.size_;
        auto* allocation = static_cast<char*>(::std::realloc(self.data_, old_size + size + 1));
        if (allocation == nullptr) {
            return false;
        }
        self.data_ = allocation;
        ::std::memcpy(self.data_ + old_size, source, size);
        self.size_ += size;
        self.data_[self.size_] = '\0';
        return true;
    }

    [[nodiscard]] bool push_back(this NativeTextBuffer& self, char value) noexcept {
        return self.append(::std::addressof(value), 1);
    }

    [[nodiscard]] constexpr char back(this NativeTextBuffer const& self) noexcept {
        return self.data_[self.size_ - 1];
    }
};

/** Resolver-owned text used below the pltxt2htm container layer. */
struct NativeResolvedFrame {
    void* address{};
    NativeTextBuffer description{};
    NativeTextBuffer source_file{};
    NativeTextBuffer module_file{};
    ::std::uint64_t displacement{};
    unsigned long source_line{};
    bool text_truncated{};
};

inline bool assign_text(NativeTextBuffer& destination, char const* source) noexcept {
    if (source == nullptr) {
        destination.clear();
        return true;
    }
    return destination.assign(source, ::std::strlen(source));
}

/** Turn a relative DWARF file name into a path using its compilation directory. */
constexpr void assign_source_location(NativeResolvedFrame& result, char const* file, char const* directory,
                                      int line) noexcept {
    if (file == nullptr || file[0] == '\0') {
        return;
    }
    result.source_file.clear();
    bool complete{true};
    if (file[0] != '/' && directory != nullptr && directory[0] != '\0') {
        complete = result.source_file.append(directory, ::std::strlen(directory));
        if (complete && result.source_file.back() != '/') {
            complete = result.source_file.push_back('/');
        }
    }
    if (complete) {
        complete = result.source_file.append(file, ::std::strlen(file));
    }
    result.text_truncated = result.text_truncated || !complete;
    result.source_line = line > 0 ? static_cast<unsigned long>(line) : 0;
}
} // namespace pltxt2htm::details::stacktrace
