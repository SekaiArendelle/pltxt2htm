/**
 * @file backend_concept.hh
 * @brief Concept for the stacktrace policy bundles defined in
 *        `impl/<backend>/backend.hh`.
 * @details A backend bundle pairs a native entry type with the capture backend
 *          that fills `container::Vector`s of it, behind a `capture::supported`
 *          flag. The concept checks the pair with the shared front-end in
 *          view: the entry capabilities that `BasicStacktrace` and the panic
 *          path rely on, and the allocator-templated capture entry point that
 *          `BasicStacktrace::current` calls with its entry storage. It only
 *          needs declarations, so every bundle header includes it to verify
 *          itself.
 */

#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>
#include "../../container/impl/vector_decl.hh"
#include "resolved_frame.hh"

namespace pltxt2htm::stacktrace::details {

/**
 * @brief Shape of one backend policy bundle.
 * @details Requires the bundle to expose `entry` and `capture` (with
 *          `supported` as `bool` and `max_depth` as an integral constant), the
 *          entry to behave as the front-end expects: `strong_ordering`
 *          three-way comparison with `noexcept` equality, `bool` conversion,
 *          `native_handle()` and `resolved()` that yields the shared
 *          `ResolvedFrame`, and the entry type to satisfy the `container`
 *          vector element requirements and the capture loop that value-assigns
 *          entries. The capture probe calls
 *          `capture::capture` with the fixed allocator the shared front-end
 *          instantiates for the concept check; actual captures run through the
 *          caller's allocator.
 */
template<typename Backend>
concept is_stacktrace_backend =
    requires {
        typename Backend::entry;
        typename Backend::capture;
        Backend::capture::supported;
        Backend::capture::max_depth;
        requires ::std::same_as<::std::remove_const_t<decltype(Backend::capture::supported)>, bool>;
        requires ::std::integral<::std::remove_const_t<decltype(Backend::capture::max_depth)>>;
    } &&
    requires(typename Backend::entry const& left, typename Backend::entry const& right) {
        { left <=> right } noexcept -> ::std::same_as<::std::strong_ordering>;
        { left != right } noexcept;
        { static_cast<bool>(left) } noexcept;
        { left.native_handle() } noexcept;
        { left.resolved() } noexcept -> ::std::same_as<::pltxt2htm::stacktrace::ResolvedFrame>;
    } &&
    requires {
        requires ::std::is_nothrow_move_constructible_v<typename Backend::entry>;
        requires ::std::is_nothrow_copy_constructible_v<typename Backend::entry>;
        requires ::std::is_nothrow_destructible_v<typename Backend::entry>;
    } &&
    requires(typename Backend::entry& entry, typename Backend::entry const& right,
             ::pltxt2htm::container::Vector<typename Backend::entry,
                                            ::fast_io::generic_allocator_adapter<::fast_io::native_global_allocator>>&
                 frames) {
        requires ::std::is_nothrow_default_constructible_v<typename Backend::entry>;
        { entry = right } noexcept;
        { Backend::capture::template capture<::fast_io::native_global_allocator>(frames, ::std::size_t{}) } noexcept;
    };

} // namespace pltxt2htm::stacktrace::details
