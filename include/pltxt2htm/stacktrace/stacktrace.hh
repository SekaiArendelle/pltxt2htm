/**
 * @file stacktrace.hh
 * @brief Native stacktrace capture and symbol resolution.
 * @warning This subtree is included by the panic path. It may include the
 *          declaration-only `container/impl/vector_decl.hh` and
 *          `container/impl/string_decl.hh`, but the contract-checked container
 *          definitions must stay at the end of this header (and of
 *          `container/vector.hh` / `container/string.hh`) so the
 *          panic/stacktrace include cycle stays broken. `impl/stacktrace_def.inc`
 *          defines members of the class and, in Windows enforce builds, pulls the
 *          assert machinery and with it the panic definition, which names
 *          `Stacktrace`; it must follow the class definition (any later position
 *          is fine).
 */

#pragma once

#include <iterator>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>
#include "../container/impl/vector_decl.hh"
#include "impl/capture.hh"
#include "impl/stacktrace_entry.hh"

// windows.h defines min and max as function-like macros, which would expand
// inside std::numeric_limits<...>::max(). Keep them undefined in this header.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace pltxt2htm::stacktrace {
/**
 * @brief Dynamic counterpart of std::basic_stacktrace.
 * @details Iterators and indexing expose immutable entries. Capturing is
 *          explicit; queries never unwind again. Storage uses
 *          `container::Vector` and resolved text uses `container::String`;
 *          their checked definitions are included at the end of this header so
 *          the panic path can use this API. Allocation failure terminates
 *          through the stateless fast_io allocator.
 * @note The capture machinery marks the frames it hides as noinline, so skip
 *       counts stacktrace entries starting with the caller of current(), as
 *       for std::stacktrace. Out-of-range operator[] is a contract violation.
 */
template<typename Allocator = ::fast_io::native_global_allocator>
class BasicStacktrace {
    static_assert(::std::is_empty_v<Allocator>, "stacktrace requires a stateless fast_io allocator");
    using entries_type =
        ::pltxt2htm::container::Vector<StacktraceEntry, ::fast_io::generic_allocator_adapter<Allocator>>;
    entries_type entries{};
    bool truncated{};

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
          truncated{other.truncated} {
    }

    constexpr BasicStacktrace(BasicStacktrace&& other) noexcept
        : entries{::std::move(other.entries)},
          truncated{other.truncated} {
        other.truncated = false;
    }

    constexpr BasicStacktrace(BasicStacktrace&& other, allocator_type const&) noexcept
        : BasicStacktrace{::std::move(other)} {
    }

    constexpr auto operator=(this BasicStacktrace& self, BasicStacktrace const& other) noexcept -> BasicStacktrace&;

    constexpr auto operator=(this BasicStacktrace& self, BasicStacktrace&& other) noexcept -> BasicStacktrace&;

    /**
     * @brief Capture the current native call stack.
     * @param skip Stacktrace entries to skip, starting with the caller of
     *             current().
     * @return Captured frames; empty when the platform has no capture backend
     *         or skip is unrepresentable.
     * @details The frames of the capture machinery and of current() itself are
     *          hidden, so entry zero corresponds to the caller of current(), as
     *          for std::stacktrace.
     * @note Capture grows until the stack is complete or the backend limit is
     *       reached; only the latter sets possibly_truncated().
     */
    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__noinline__)
    [[__gnu__::__noinline__]]
#elif defined(_MSC_VER)
    __declspec(noinline)
#endif
    static auto current(size_type skip = 0) noexcept -> BasicStacktrace {
        BasicStacktrace result{};
        // CaptureBackend::capture, capture_all and current are noinline, so
        // their frames are stable and can be hidden here.
        constexpr size_type hidden_machinery_frames{3};
        if (skip > ::std::numeric_limits<size_type>::max() - hidden_machinery_frames) {
            return result;
        }
        auto frames = details::capture_all<allocator_type>(skip + hidden_machinery_frames);
        result.entries.reserve(frames.addresses.size());
        for (auto const handle : frames.addresses) {
            result.entries.push_back(StacktraceEntry{handle});
        }
        result.truncated = frames.possibly_truncated;
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
        auto const count = self.entries.size();
        if (count == 0) {
            return self.entries.data();
        }
        return self.entries.data() + count;
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
        return self.entries.size();
    }

    [[nodiscard]]
    constexpr auto max_size(this BasicStacktrace const&) noexcept -> size_type {
        return entries_type::max_size();
    }

    [[nodiscard]]
    constexpr auto is_empty(this BasicStacktrace const& self) noexcept -> bool {
        return self.size() == 0;
    }

#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto empty(this BasicStacktrace const& self) noexcept -> bool = delete
    #if __cpp_deleted_function >= 202403L
        #if defined __clang__
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wc++26-extensions"
        #endif
        ("empty() is external-only; use is_empty() inside pltxt2htm")
        #if defined __clang__
            #pragma clang diagnostic pop
        #endif
    #endif
        ;
#else
    [[nodiscard]]
    constexpr auto empty(this BasicStacktrace const& self) noexcept -> bool {
        return self.is_empty();
    }
#endif

    [[nodiscard]]
    constexpr auto operator[](this BasicStacktrace const& self, size_type index) noexcept -> const_reference;

    constexpr void swap(this BasicStacktrace& self, BasicStacktrace& other) noexcept {
        self.entries.swap(other.entries);
        auto const truncated = self.truncated;
        self.truncated = other.truncated;
        other.truncated = truncated;
    }

    [[nodiscard]]
    constexpr auto possibly_truncated(this BasicStacktrace const& self) noexcept -> bool {
        return self.truncated;
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

#pragma pop_macro("max")
#pragma pop_macro("min")

#include "impl/stacktrace_def.inc"
#include "../container/impl/vector_def.inc"
#include "../container/impl/string_def.inc"
