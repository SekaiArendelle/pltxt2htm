/**
 * @file stacktrace.hh
 * @brief Native stacktrace capture and symbol resolution.
 * @warning This subtree is included by the panic path. It may include the
 *          declaration-only container headers, but the contract-checked
 *          container definitions must stay at the end of this header (and of
 *          `container/vector.hh` / `container/string.hh`) so the
 *          panic/stacktrace include cycle stays broken. `impl/stacktrace_def.inc`
 *          defines members of the class and, in Windows enforce builds, pulls the
 *          assert machinery and with it the panic definition, which names
 *          `Stacktrace`; it must follow the class definition (any later position
 *          is fine).
 * @details Every capture backend is a policy bundle in
 *          `impl/<backend>/backend.hh`, pairing an entry type with its capture
 *          backend behind a `capture::supported` flag; the backend headers are
 *          includable everywhere and the bundle shape is described by
 *          `details::is_stacktrace_backend`. `details::BasicStacktrace` is the
 *          shared front-end parameterized over a bundle, and the public names
 *          alias the selected default.
 */

#pragma once

#include <iterator>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>
#include "../container/impl/vector_decl.hh"
#include "impl/none/backend.hh"
#include "impl/nt/backend.hh"
#include "impl/std/backend.hh"
#include "impl/backend_concept.hh"

// windows.h defines min and max as function-like macros, which would expand
// inside std::numeric_limits<...>::max(). Keep them undefined in this header.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace pltxt2htm::stacktrace {

namespace details {

/**
 * @brief The backend the public front-end selects: the Windows option first,
 *        then the C++23-standard fallback, then the no-resolver fallback.
 */
using DefaultBackend =
    ::std::conditional_t<NtBackend::capture::supported, NtBackend,
                         ::std::conditional_t<StdBackend::capture::supported, StdBackend, NoneBackend>>;

/**
 * @brief Dynamic counterpart of std::basic_stacktrace.
 * @tparam Backend Policy bundle with `entry`, `capture` and `supported`
 *                 through `capture`, as defined by `impl/<backend>/backend.hh`
 *                 and described by `details::is_stacktrace_backend`.
 * @tparam Allocator Stateless fast_io allocator for the entry storage.
 * @details Iterators and indexing expose immutable entries. Capturing is
 *          explicit; queries never unwind again. Storage uses
 *          `container::Vector` and resolved text uses `container::String`;
 *          their checked definitions are included at the end of the header
 *          that includes this one so the panic path can use this API.
 *          Allocation failure terminates through the stateless fast_io
 *          allocator. Because the allocator is stateless, construction always
 *          uses a default-constructed `allocator_type` and no constructor
 *          takes an allocator argument.
 * @note The capture machinery marks the frames it hides as noinline, so skip
 *       counts stacktrace entries starting with the caller of current(), as
 *       for std::stacktrace. Out-of-range operator[] is a contract violation.
 */
template<is_stacktrace_backend Backend, typename Allocator = ::fast_io::native_global_allocator>
class BasicStacktrace {
    static_assert(::std::is_empty_v<Allocator>, "stacktrace requires a stateless fast_io allocator");
    using entries_type =
        ::pltxt2htm::container::Vector<typename Backend::entry, ::fast_io::generic_allocator_adapter<Allocator>>;
    entries_type entries{};

public:
    using value_type = typename Backend::entry;
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

    constexpr BasicStacktrace(BasicStacktrace&& other) noexcept
        : entries{::std::move(other.entries)} {
    }

    constexpr auto operator=(this BasicStacktrace& self, BasicStacktrace const& other) noexcept -> BasicStacktrace&;

    constexpr auto operator=(this BasicStacktrace& self, BasicStacktrace&& other) noexcept -> BasicStacktrace&;

    /**
     * @brief Capture the current native call stack.
     * @param skip Stacktrace entries to skip, starting with the caller of
     *             current().
     * @return Captured frames; empty when the platform has no usable capture
     *         backend or skip is unrepresentable.
     * @details The frames of the capture machinery and of current() itself are
     *          hidden, so entry zero corresponds to the caller of current(), as
     *          for std::stacktrace.
     */
    [[nodiscard]]
#if __has_cpp_attribute(__gnu__::__noinline__)
    [[__gnu__::__noinline__]]
#elif defined(_MSC_VER)
    __declspec(noinline)
#endif
    static auto current(size_type skip = 0) noexcept -> BasicStacktrace {
        static_assert(!Backend::capture::supported || Backend::capture::max_depth >= 1,
                      "supported capture backends need a positive max_depth");
        BasicStacktrace result{};
        if constexpr (!Backend::capture::supported) {
            return result;
        }
        else {
            // capture and current are noinline, so their frames are stable and
            // can be hidden here.
            constexpr size_type hidden_machinery_frames{2};
            if (skip > ::std::numeric_limits<size_type>::max() - hidden_machinery_frames) {
                return result;
            }
            Backend::capture::template capture<allocator_type>(result.entries, skip + hidden_machinery_frames);
            return result;
        }
    }

    [[nodiscard]]
    constexpr auto get_allocator(this BasicStacktrace const&) noexcept -> allocator_type {
        return {};
    }

    [[nodiscard]]
    constexpr auto begin(this BasicStacktrace const& self) noexcept -> const_iterator {
        return self.entries.begin();
    }

    [[nodiscard]]
    constexpr auto end(this BasicStacktrace const& self) noexcept -> const_iterator {
        return self.entries.end();
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
    }

    template<typename RightAllocator>
    [[nodiscard]]
    constexpr auto operator==(this BasicStacktrace const& self,
                              BasicStacktrace<Backend, RightAllocator> const& other) noexcept -> bool {
        if (self.size() != other.size()) {
            return false;
        }
        auto const size = self.size();
        for (::std::size_t i = 0; i < size; ++i) {
            if (self[i] != other[i]) {
                return false;
            }
        }
        return true;
    }

    template<typename RightAllocator>
    [[nodiscard]]
    constexpr auto operator<=>(this BasicStacktrace const& self,
                               BasicStacktrace<Backend, RightAllocator> const& other) noexcept
        -> ::std::strong_ordering {
        auto const size_order = self.size() <=> other.size();
        if (size_order != 0) {
            return size_order;
        }
        auto const size = self.size();
        for (::std::size_t i = 0; i < size; ++i) {
            auto const order = self[i] <=> other[i];
            if (order != 0) {
                return order;
            }
        }
        return ::std::strong_ordering::equal;
    }
};

template<typename Backend, typename Allocator>
constexpr void swap(BasicStacktrace<Backend, Allocator>& left, BasicStacktrace<Backend, Allocator>& right) noexcept {
    left.swap(right);
}

} // namespace details

/**
 * @brief The entry type of the backend the public front-end selects.
 */
using StacktraceEntry = details::DefaultBackend::entry;

/**
 * @brief Alias binding a backend bundle to the shared stacktrace front-end.
 */
template<typename Backend = details::DefaultBackend, typename Allocator = ::fast_io::native_global_allocator>
using BasicStacktrace = details::BasicStacktrace<Backend, Allocator>;

using Stacktrace = details::BasicStacktrace<details::DefaultBackend>;
} // namespace pltxt2htm::stacktrace

#pragma pop_macro("max")
#pragma pop_macro("min")

#include "impl/stacktrace_def.inc"
#include "../container/impl/vector_def.inc"
#include "../container/impl/string_def.inc"
