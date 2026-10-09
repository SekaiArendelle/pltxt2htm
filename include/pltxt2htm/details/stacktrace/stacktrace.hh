#pragma once

#include <algorithm>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>
#include "../../container/vector.hh"
#include "../../contracts.hh"
#include "../trap.hh"
#include "capture.hh"
#include "stacktrace_entry.hh"

namespace pltxt2htm::details::stacktrace {
/** Dynamic counterpart of std::basic_stacktrace. Iterators and indexing
 * expose immutable entries. Capturing is explicit; queries never unwind again.
 * Uses a stateless fast_io allocator. Allocation failure terminates, as with
 * fast_io containers; at() traps on invalid indices. Native capture
 * may allocate, and exact caller identity depends on inlining and tail calls.
 * Unlike std::stacktrace, skip counts native frames including implementation
 * helpers. Forwarding current() overloads can add frames in unoptimized builds.
 */
template<typename Allocator = ::fast_io::native_global_allocator>
class BasicStacktrace {
    static_assert(::std::is_empty_v<Allocator>, "stacktrace requires a stateless fast_io allocator");
    using storage_allocator = ::fast_io::generic_allocator_adapter<Allocator>;
    ::pltxt2htm::container::Vector<StacktraceEntry, storage_allocator> entries_{};
    CaptureResult captured_{};

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
        : entries_{other.entries_},
          captured_{other.captured_} {
    }

    constexpr BasicStacktrace(BasicStacktrace&& other) noexcept
        : entries_{::std::move(other.entries_)},
          captured_{::std::exchange(other.captured_, {})} {
    }

    constexpr BasicStacktrace(BasicStacktrace&& other, allocator_type const&) noexcept
        : BasicStacktrace{::std::move(other)} {
    }

    constexpr BasicStacktrace& operator=(this BasicStacktrace& self, BasicStacktrace const& other) noexcept {
        if (::std::addressof(self) == ::std::addressof(other)) {
            return self;
        }
        self.entries_ = other.entries_;
        self.captured_ = other.captured_;
        return self;
    }

    constexpr BasicStacktrace& operator=(this BasicStacktrace& self, BasicStacktrace&& other) noexcept {
        if (::std::addressof(self) == ::std::addressof(other)) {
            return self;
        }
        self.entries_ = ::std::move(other.entries_);
        self.captured_ = ::std::exchange(other.captured_, {});
        return self;
    }

    [[nodiscard]] static BasicStacktrace current(allocator_type const& alloc = {}) noexcept {
        return BasicStacktrace::current(0, (::std::numeric_limits<size_type>::max)(), alloc);
    }

    [[nodiscard]] static BasicStacktrace current(size_type skip, allocator_type const& alloc = {}) noexcept {
        return BasicStacktrace::current(skip, (::std::numeric_limits<size_type>::max)(), alloc);
    }

    [[nodiscard]] static BasicStacktrace current(size_type skip, size_type max_depth,
                                                 allocator_type const& alloc = {}) noexcept {
        BasicStacktrace result{alloc};
#if defined(PLTXT2HTM_ENABLE_STACKTRACE) && (defined(_WIN32) || (defined(__linux__) && __has_include(<execinfo.h>)))
    #if defined(_WIN32)
        constexpr size_type native_limit = 65535;
        if (skip > (::std::numeric_limits<unsigned long>::max)()) {
            return result;
        }
    #else
        constexpr auto int_limit = static_cast<size_type>((::std::numeric_limits<int>::max)());
        if (skip >= int_limit) {
            return result;
        }
        auto const native_limit = int_limit - skip;
    #endif
        auto const limit = max_depth < native_limit ? max_depth : native_limit;
        if (limit == 0) {
            return result;
        }
        ::pltxt2htm::container::Vector<void*, storage_allocator> addresses{};
        auto depth = limit < 64 ? limit : 64;
        for (;;) {
            addresses.template reserve<::pltxt2htm::Contracts::ignore>(depth);
            while (addresses.size() < depth) {
                addresses.template emplace_back<::pltxt2htm::Contracts::ignore>(nullptr);
            }
            result.captured_ = ::pltxt2htm::details::stacktrace::capture({addresses.data(), depth}, skip);
            if (!result.captured_.possibly_truncated || depth == limit) {
                break;
            }
            depth = depth > limit / 2 ? limit : depth * 2;
        }
        auto const size = result.captured_.size;
        result.entries_.template reserve<::pltxt2htm::Contracts::ignore>(size);
        for (size_type i = 0; i < size; ++i) {
            auto& entry = result.entries_.template emplace_back<::pltxt2htm::Contracts::ignore>();
            entry.address_ = addresses.template index<::pltxt2htm::Contracts::ignore>(i);
        }
#else
        (void)skip;
        (void)max_depth;
#endif
        return result;
    }

    [[nodiscard]] constexpr allocator_type get_allocator(this BasicStacktrace const&) noexcept {
        return {};
    }

    [[nodiscard]] constexpr const_iterator begin(this BasicStacktrace const& self) noexcept {
        return self.entries_.data();
    }

    [[nodiscard]] constexpr const_iterator end(this BasicStacktrace const& self) noexcept {
        if (self.captured_.size == 0) {
            return self.entries_.data();
        }
        return self.entries_.data() + self.captured_.size;
    }

    [[nodiscard]] constexpr const_iterator cbegin(this BasicStacktrace const& self) noexcept {
        return self.begin();
    }

    [[nodiscard]] constexpr const_iterator cend(this BasicStacktrace const& self) noexcept {
        return self.end();
    }

    [[nodiscard]] constexpr const_reverse_iterator rbegin(this BasicStacktrace const& self) noexcept {
        return const_reverse_iterator{self.end()};
    }

    [[nodiscard]] constexpr const_reverse_iterator rend(this BasicStacktrace const& self) noexcept {
        return const_reverse_iterator{self.begin()};
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin(this BasicStacktrace const& self) noexcept {
        return self.rbegin();
    }

    [[nodiscard]] constexpr const_reverse_iterator crend(this BasicStacktrace const& self) noexcept {
        return self.rend();
    }

    [[nodiscard]] constexpr size_type size(this BasicStacktrace const& self) noexcept {
        return self.captured_.size;
    }

    [[nodiscard]] constexpr size_type max_size(this BasicStacktrace const&) noexcept {
        return ::pltxt2htm::container::Vector<StacktraceEntry, storage_allocator>::max_size();
    }

    [[nodiscard]] constexpr bool empty(this BasicStacktrace const& self) noexcept {
        return self.size() == 0;
    }

    [[nodiscard]] constexpr const_reference operator[](this BasicStacktrace const& self, size_type index) noexcept {
        return self.entries_.template index<::pltxt2htm::Contracts::ignore>(index);
    }

    [[nodiscard]] constexpr const_reference at(this BasicStacktrace const& self, size_type index) noexcept {
        if (index >= self.size()) [[unlikely]] {
            ::pltxt2htm::details::trap();
        }
        return self[index];
    }

    constexpr void swap(this BasicStacktrace& self, BasicStacktrace& other) noexcept {
        self.entries_.swap(other.entries_);
        ::std::swap(self.captured_, other.captured_);
    }

    [[nodiscard]] constexpr bool possibly_truncated(this BasicStacktrace const& self) noexcept {
        return self.captured_.possibly_truncated;
    }
};

template<typename Left, typename Right>
[[nodiscard]] constexpr bool operator==(BasicStacktrace<Left> const& left,
                                        BasicStacktrace<Right> const& right) noexcept {
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
[[nodiscard]] constexpr ::std::strong_ordering operator<=>(BasicStacktrace<Left> const& left,
                                                           BasicStacktrace<Right> const& right) noexcept {
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
} // namespace pltxt2htm::details::stacktrace
