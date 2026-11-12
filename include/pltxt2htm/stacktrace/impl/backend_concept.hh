/**
 * @file backend_concept.hh
 * @brief Concepts for the stacktrace entries, capture backends, and policy
 *        bundles defined in `impl/<backend>/`.
 * @details A backend bundle pairs a native entry type with the capture backend
 *          that fills `container::Vector`s of it, behind a `capture::supported`
 *          flag. The bundle concept checks the pair with the shared front-end
 *          in view by composing the standalone entry and capture concepts, so
 *          a new platform backend can also verify each half on its own. Only
 *          declarations are needed.
 */

#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>
#include "../../container/impl/vector_decl.hh"
#include "resolved_frame.hh"

namespace pltxt2htm::stacktrace::details {

/**
 * @brief Contract of one native stacktrace entry.
 * @details The entry capabilities that `BasicStacktrace` and the panic path
 *          rely on: `strong_ordering` three-way comparison with `noexcept`
 *          equality, `bool` conversion, `native_handle()` and `resolved()`
 *          that yields the shared `ResolvedFrame`, `noexcept` value
 *          assignment for the capture loop, and no-throwing default
 *          construction, move and copy construction, and destruction so
 *          capture can value-assign entries into storage.
 */
template<typename Entry>
concept is_stacktrace_entry = requires(Entry const& left, Entry const& right, Entry& entry) {
    { left <=> right } noexcept -> ::std::same_as<::std::strong_ordering>;
    { left != right } noexcept;
    { static_cast<bool>(left) } noexcept;
    { left.native_handle() } noexcept;
    { left.resolved() } noexcept -> ::std::same_as<::pltxt2htm::stacktrace::ResolvedFrame>;
    { entry = right } noexcept;
} && requires {
    requires ::std::is_nothrow_default_constructible_v<Entry>;
    requires ::std::is_nothrow_move_constructible_v<Entry>;
    requires ::std::is_nothrow_copy_constructible_v<Entry>;
    requires ::std::is_nothrow_destructible_v<Entry>;
};

/**
 * @brief Contract of the capture backend for the entry type `Entry`.
 * @details Exposes `supported` as `bool` and `max_depth` as an integral
 *          constant, plus the allocator-templated entry point that
 *          `BasicStacktrace::current` calls with its entry storage. The probe
 *          calls `capture` with the fixed allocator the shared front-end
 *          instantiates for the concept check; actual captures run through
 *          the caller's allocator.
 */
template<typename Capture, typename Entry>
concept is_stacktrace_capture_backend =
    requires {
        Capture::supported;
        Capture::max_depth;
        requires ::std::same_as<::std::remove_const_t<decltype(Capture::supported)>, bool>;
        requires ::std::integral<::std::remove_const_t<decltype(Capture::max_depth)>>;
    } &&
    requires(
        ::pltxt2htm::container::Vector<Entry, ::fast_io::generic_allocator_adapter<::fast_io::native_global_allocator>>&
            frames) {
        { Capture::template capture<::fast_io::native_global_allocator>(frames, ::std::size_t{}) } noexcept;
    };

/**
 * @brief Shape of one backend policy bundle.
 * @details Requires the bundle to expose `entry` and `capture` types that
 *          satisfy the standalone entry and capture concepts above.
 */
template<typename Backend>
concept is_stacktrace_backend =
    requires {
        typename Backend::entry;
        typename Backend::capture;
    } && is_stacktrace_entry<typename Backend::entry> &&
    is_stacktrace_capture_backend<typename Backend::capture, typename Backend::entry>;

} // namespace pltxt2htm::stacktrace::details
