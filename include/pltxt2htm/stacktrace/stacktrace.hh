/**
 * @file stacktrace.hh
 * @brief Native stacktrace capture and symbol resolution.
 * @warning This subtree is included by the panic path, so it must stay below the
 *          `container/` and `details/push_macro.hh` layers: never include
 *          `container/` headers, `details/panic.hh`, or `details/push_macro.hh`
 *          from `pltxt2htm/stacktrace/`.
 */

#pragma once

#include <iterator>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>
#include "details/capture.hh"
#include "details/contract.hh"
#include "details/storage.hh"
#include "stacktrace_entry.hh"

namespace pltxt2htm::stacktrace {
/** Dynamic counterpart of std::basic_stacktrace. Iterators and indexing
 * expose immutable entries. Capturing is explicit; queries never unwind again.
 * Storage and resolved text are owned by the stacktrace implementation rather
 * than the pltxt2htm container layer, so the panic path can use this API.
 * Uses a stateless fast_io allocator. Allocation failure terminates, as with
 * fast_io containers; out-of-range operator[] is a contract violation. Native
 * capture depends on inlining and tail calls.
 * Unlike std::stacktrace, skip counts native frames including implementation
 * helpers. Forwarding current() overloads can add frames in unoptimized builds.
 */
template<typename Allocator = ::fast_io::native_global_allocator>
class BasicStacktrace {
    static_assert(::std::is_empty_v<Allocator>, "stacktrace requires a stateless fast_io allocator");
    details::Buffer<StacktraceEntry, Allocator> entries{};
    details::CaptureResult captured{};

public:
    using value_type = StacktraceEntry;
    using const_reference = value_type const&;
    using reference = value_type&;
    using const_iterator = value_type const*;
    using iterator = const_iterator;
    using reverse_iterator = ::std::reverse_iterator<iterator>;
    using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;
    using difference_type = ::std::ptrdiff_t;
    using size_type = ::std::size_t;
    using allocator_type = Allocator;

    constexpr BasicStacktrace() noexcept = default;
    constexpr BasicStacktrace(BasicStacktrace const&) noexcept = default;

    constexpr explicit BasicStacktrace(allocator_type const&) noexcept {
    }

    constexpr BasicStacktrace(BasicStacktrace const& other, allocator_type const&) noexcept
        : entries{other.entries},
          captured{other.captured} {
    }

    constexpr BasicStacktrace(BasicStacktrace&& other) noexcept
        : entries{::std::move(other.entries)},
          captured{other.captured} {
        other.captured = {};
    }

    constexpr BasicStacktrace(BasicStacktrace&& other, allocator_type const&) noexcept
        : BasicStacktrace{::std::move(other)} {
    }

    constexpr auto operator=(this BasicStacktrace& self, BasicStacktrace const& other) noexcept -> BasicStacktrace& {
        if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
            details::contract_violation();
        }
        self.entries = other.entries;
        self.captured = other.captured;
        return self;
    }

    constexpr auto operator=(this BasicStacktrace& self, BasicStacktrace&& other) noexcept -> BasicStacktrace& {
        if (::std::addressof(self) == ::std::addressof(other)) [[unlikely]] {
            details::contract_violation();
        }
        self.entries = ::std::move(other.entries);
        self.captured = other.captured;
        other.captured = {};
        return self;
    }

    [[nodiscard]]
    static auto current() noexcept -> BasicStacktrace {
        return BasicStacktrace::current(0, (::std::numeric_limits<size_type>::max)());
    }

    [[nodiscard]]
    static auto current(size_type skip) noexcept -> BasicStacktrace {
        return BasicStacktrace::current(skip, (::std::numeric_limits<size_type>::max)());
    }

    [[nodiscard]]
    static auto current([[maybe_unused]] size_type skip, [[maybe_unused]] size_type max_depth) noexcept
        -> BasicStacktrace {
        BasicStacktrace result{};
#if defined(_WIN32)
        constexpr size_type native_limit = 65535;
        if (skip > (::std::numeric_limits<::pltxt2htm::sys_nt::Ulong>::max)()) {
            return result;
        }
        auto const limit = max_depth < native_limit ? max_depth : native_limit;
        if (limit == 0) {
            return result;
        }
        details::Buffer<void*, allocator_type> addresses{};
        auto depth = limit < 64 ? limit : 64;
        for (;;) {
            addresses.reserve(depth);
            while (addresses.size() < depth) {
                addresses.emplace_back(nullptr);
            }
            result.captured = ::pltxt2htm::stacktrace::details::capture({addresses.data(), depth}, skip);
            if (!result.captured.possibly_truncated || depth == limit) {
                break;
            }
            depth = depth > limit / 2 ? limit : depth * 2;
        }
        auto const size = result.captured.size;
        result.entries.reserve(size);
        for (size_type i = 0; i < size; ++i) {
            auto& entry = result.entries.emplace_back();
            entry.address = addresses[i];
        }
#endif
        return result;
    }

    [[nodiscard]]
    constexpr auto get_allocator(this BasicStacktrace const&) noexcept -> allocator_type {
        return {};
    }

    [[nodiscard]]
    constexpr auto begin(this BasicStacktrace const& self) noexcept -> const_iterator {
        return self.entries.data();
    }

    [[nodiscard]]
    constexpr auto end(this BasicStacktrace const& self) noexcept -> const_iterator {
        if (self.captured.size == 0) {
            return self.entries.data();
        }
        return self.entries.data() + self.captured.size;
    }

    [[nodiscard]]
    constexpr auto cbegin(this BasicStacktrace const& self) noexcept -> const_iterator {
        return self.begin();
    }

    [[nodiscard]]
    constexpr auto cend(this BasicStacktrace const& self) noexcept -> const_iterator {
        return self.end();
    }

    [[nodiscard]]
    constexpr auto rbegin(this BasicStacktrace const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto rend(this BasicStacktrace const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto crbegin(this BasicStacktrace const& self) noexcept -> const_reverse_iterator {
        return self.rbegin();
    }

    [[nodiscard]]
    constexpr auto crend(this BasicStacktrace const& self) noexcept -> const_reverse_iterator {
        return self.rend();
    }

    [[nodiscard]]
    constexpr auto size(this BasicStacktrace const& self) noexcept -> size_type {
        return self.captured.size;
    }

    [[nodiscard]]
    constexpr auto max_size(this BasicStacktrace const&) noexcept -> size_type {
        return details::Buffer<StacktraceEntry, Allocator>::max_size();
    }

    [[nodiscard]]
    constexpr auto empty(this BasicStacktrace const& self) noexcept -> bool {
        return self.size() == 0;
    }

    [[nodiscard]]
    constexpr auto operator[](this BasicStacktrace const& self, size_type index) noexcept -> const_reference {
        if (index >= self.size()) [[unlikely]] {
            details::contract_violation();
        }
        return self.entries[index];
    }

    constexpr void swap(this BasicStacktrace& self, BasicStacktrace& other) noexcept {
        self.entries.swap(other.entries);
        auto const captured = self.captured;
        self.captured = other.captured;
        other.captured = captured;
    }

    [[nodiscard]]
    constexpr auto possibly_truncated(this BasicStacktrace const& self) noexcept -> bool {
        return self.captured.possibly_truncated;
    }
};

template<typename Left, typename Right>
[[nodiscard]]
constexpr auto operator==(BasicStacktrace<Left> const& left, BasicStacktrace<Right> const& right) noexcept -> bool {
    if (left.size() != right.size()) {
        return false;
    }
    auto const size = left.size();
    for (::std::size_t i = 0; i < size; ++i) {
        if (left[i] != right[i]) {
            return false;
        }
    }
    return true;
}

template<typename Left, typename Right>
[[nodiscard]]
constexpr auto operator<=>(BasicStacktrace<Left> const& left, BasicStacktrace<Right> const& right) noexcept
    -> ::std::strong_ordering {
    auto const size_order = left.size() <=> right.size();
    if (size_order != 0) {
        return size_order;
    }
    auto const size = left.size();
    for (::std::size_t i = 0; i < size; ++i) {
        auto const order = left[i] <=> right[i];
        if (order != 0) {
            return order;
        }
    }
    return ::std::strong_ordering::equal;
}

template<typename Allocator>
constexpr void swap(BasicStacktrace<Allocator>& left, BasicStacktrace<Allocator>& right) noexcept {
    left.swap(right);
}

using Stacktrace = BasicStacktrace<>;
} // namespace pltxt2htm::stacktrace
