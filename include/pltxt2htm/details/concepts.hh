/**
 * @file concepts.hh
 * @brief Shared concepts for pltxt2htm implementation details.
 */

#pragma once

#include <concepts>
#include <ranges>
#include <iterator>
#include <type_traits>

namespace pltxt2htm::details {

template<typename T>
concept is_char_type = ::std::same_as<T, char> || ::std::same_as<T, wchar_t> || ::std::same_as<T, char8_t> ||
                       ::std::same_as<T, char16_t> || ::std::same_as<T, char32_t>;

/**
 * @brief The equality test performed by container comparisons is nothrow.
 * @details Checks the element `==` together with both boolean tests applied to
 * its result: `static_cast<bool>`, which Vector's operator== uses, and `!`,
 * which std::equal applies inside Array's and Deque's operator== and which may
 * dispatch to the result type's own `operator!`. The element `!=` is not part
 * of the test: container `a != b` is rewritten from `a == b`, and no
 * comparison body compares with `!=`.
 */
template<typename T>
concept nothrow_equality_comparable = requires(T const& left, T const& right) {
    { static_cast<bool>(left == right) } noexcept -> ::std::same_as<bool>;
    { !(left == right) } noexcept;
};

/**
 * @brief Three-way comparison that completes without throwing.
 */
template<typename T>
concept nothrow_three_way_comparable = requires(T const& left, T const& right) {
    { left <=> right } noexcept;
};

/**
 * @brief Iterating the given iterator/sentinel pair cannot throw.
 * @details Refinement of std::sentinel_for that additionally requires
 * the operations a range loop performs - dereference, increment, and
 * the sentinel comparison - to complete without throwing. Element
 * construction and iterator copies are container-specific and are
 * checked by the caller.
 */
template<typename Iterator, typename Sentinel>
concept nothrow_sentinel_iteration = requires(Iterator& iterator, Sentinel& sentinel) {
    requires ::std::sentinel_for<Sentinel, Iterator>;
    { *iterator } noexcept;
    { ++iterator } noexcept;
    { static_cast<bool>(iterator != sentinel) } noexcept;
};

/**
 * @brief Calling begin() and end() on an lvalue of Range cannot throw.
 */
template<typename Range>
concept nothrow_range_access = requires(Range& range) {
    { ::std::ranges::begin(range) } noexcept;
    { ::std::ranges::end(range) } noexcept;
};

/**
 * @brief An iterator/sentinel pair whose traversal completes without throwing.
 * @details Counterpart of the standard's exposition-only nothrow-input-range,
 * with these deliberate differences: the boundaries are given as an
 * iterator/sentinel pair instead of a range, the no-throw requirement is
 * checked syntactically rather than left as a semantic precondition, and the
 * standard's lvalue/value-type restriction that excludes proxy iterators is
 * omitted because container loops construct elements from `*iterator`.
 * Only traversal is covered here. Element construction and boundary ownership
 * (copying or moving the iterator and sentinel) are container-specific and are
 * composed by the caller, so an iterator with a throwing or deleted copy is
 * still a valid input range for a consumer that only iterates (a same-type
 * sentinel separately requires copyability through std::sentinel_for).
 */
template<typename Iterator, typename Sentinel>
concept nothrow_input_range = ::std::input_iterator<Iterator> && nothrow_sentinel_iteration<Iterator, Sentinel>;

/**
 * @brief A nothrow-input-range whose boundaries can also be moved without throwing.
 * @details Used where an implementation moves its by-value iterator and
 * sentinel into another range, for example when materializing a temporary.
 * Kept separate from nothrow_input_range so that traversal-only consumers do
 * not reject iterators with throwing copies or moves.
 */
template<typename Iterator, typename Sentinel>
concept nothrow_move_input_range =
    nothrow_input_range<Iterator, Sentinel> && ::std::is_nothrow_move_constructible_v<Iterator> &&
    ::std::is_nothrow_move_constructible_v<Sentinel>;

template<typename T, typename U>
concept reference_constructs_from_temporary =
#if __cpp_lib_reference_from_temporary >= 202202L
    ::std::reference_constructs_from_temporary_v<T, U>;
#elif defined(__has_builtin)
    #if __has_builtin(__reference_constructs_from_temporary)
    __reference_constructs_from_temporary(T, U);
    #else
        #error "compiler too old for __reference_constructs_from_temporary"
    #endif
#else
    #error "__has_builtin is not supported; compiler too old for reference_constructs_from_temporary_v"
#endif

} // namespace pltxt2htm::details
