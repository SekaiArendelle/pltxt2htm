/**
 * @file cstring_view_decl.hh
 * @brief Declarations for container::BasicCStringView.
 * @details Contract-checked members are declared here and defined in
 *          cstring_view_def.inc. Members defined inline that call those members
 *          also need cstring_view_def.inc in the same translation unit; include
 *          container/cstring_view.hh for the complete API.
 */

#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <limits>
#include <ranges>
#include <type_traits>
#include <utility>

#include <fast_io/fast_io_core.h>

#include "../../details/concepts.hh"
#include "../../details/literal_string.hh"
#include "../../details/impl/inplace_string_decl.hh"
#include "string_view_decl.hh"
#include "string_decl.hh"

// windows.h defines min and max as function-like macros, which would expand
// inside std::numeric_limits<...>::max(). Keep them undefined in this header.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace pltxt2htm::container {

namespace details {

/**
 * @brief A contiguous, sized range holding @p CharType code units.
 * @details Ranges accepted by BasicCStringView's range constructor. The element
 *          one past `ranges::size(r)` must exist and equal `CharType{}`; that
 *          null-termination precondition is documented, cannot be verified
 *          statically, and is asserted at runtime when contracts are enforced.
 *          Callers are responsible for the guarantee, whether the range is a
 *          span into a larger buffer, a view of a string literal, or any other
 *          contiguous range. Ranges excluded structurally are: allocator-backed
 *          owning strings (rvalues would dangle, and the project BasicString has
 *          a dedicated whole-string constructor) and the project literal/inplace
 *          strings (their storage is exactly N code units, so the one-past-the-end
 *          element cannot exist).
 */
template<typename RangeType, typename CharType>
concept is_ntcts_range = ::std::ranges::contiguous_range<RangeType> && ::std::ranges::sized_range<RangeType> &&
                         ::std::same_as<::std::ranges::range_value_t<RangeType>, CharType>;

/**
 * @brief Excludes allocator-backed owning strings (project BasicString, fast_io
 *        container strings, std::basic_string/std::vector and similar) from the
 *        range constructor; their null-termination story is handled by
 *        dedicated constructors or rejected outright.
 */
template<typename RangeType>
concept has_allocator_type = requires { typename ::std::remove_cvref_t<RangeType>::allocator_type; };

} // namespace details

/**
 * @brief A non-owning view over a null-terminated contiguous sequence of characters.
 * @tparam CharType Character type stored by the referenced sequence.
 *
 * BasicCStringView maintains the invariant that, unless the referenced characters
 * are modified externally, a null terminator follows the last viewed character.
 * Operations that would drop that terminator do not compile (remove_suffix) or
 * return a BasicStringView instead (the two-argument subview).
 *
 * Embedded null code units are allowed; has_nulls() and first_piece() manage them
 * at the value level. data() and c_str() never scan, so using their result as a
 * null-terminated string carries the precondition !has_nulls().
 *
 * BasicLiteralString and BasicInplaceString do not reserve terminator storage and
 * are therefore rejected; whole null-terminated sources only. Design follows
 * https://wg21.link/p3655 (std::cstring_view) adapted to pltxt2htm contracts.
 */
template<::pltxt2htm::details::is_char_type CharType>
class BasicCStringView {
public:
    using value_type = CharType;
    using size_type = ::std::size_t;
    using difference_type = ::std::ptrdiff_t;
    using const_reference = value_type const&;
    using const_pointer = value_type const*;
    using const_iterator = const_pointer;
    using iterator = const_iterator;
    using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;
    using reverse_iterator = const_reverse_iterator;

    static constexpr size_type npos{::std::numeric_limits<size_type>::max()};

private:
    // Backs the default-constructed view; always null-terminated.
    static constexpr value_type empty_storage[1]{};

    const_pointer pointer{};
    size_type length{};

public:
    /**
     * @brief Builds a view over the valid empty string.
     * @post size() == 0 and data() addresses a null-terminated empty string.
     */
    constexpr BasicCStringView() noexcept
        : pointer{empty_storage} {
    }

    // Declaring any copy assignment operator, even a defaulted one, deprecates the
    // implicit copy constructor and destructor; default them explicitly to keep the
    // special member set complete.
    constexpr BasicCStringView(BasicCStringView const&) noexcept = default;

    constexpr BasicCStringView(BasicCStringView&&) noexcept = default;

    constexpr ~BasicCStringView() noexcept = default;

    // The standard implements this as a defaulted copy assignment with an implicit
    // object parameter; the reference type has no move assignment. The project
    // convention prefers an explicit object parameter, but clang 23.1.3 crashes in
    // the backend when generating code for a defaulted explicit-object operator= on
    // a class with two or more members; see
    // https://github.com/llvm/llvm-project/issues/242767. Use the implicit object
    // form until that bug is fixed; the lvalue ref-qualifier keeps the convention's
    // intent of rejecting assignment to temporaries. Staying defaulted also keeps
    // the copy assignment trivial, preserving is_trivially_copyable.
    constexpr BasicCStringView& operator=(BasicCStringView const&) & noexcept = default;

    constexpr BasicCStringView(::std::nullptr_t) = delete
#if __cpp_deleted_function >= 202403L
    #if defined __clang__
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wc++26-extensions"
    #endif
        ("use a null-terminated string literal instead of a null pointer")
    #if defined __clang__
        #pragma clang diagnostic pop
    #endif
#endif
        ;

    /**
     * @brief Builds a view over a null-terminated string.
     * @param[in] string Null-terminated string; must not be null.
     * @note The length is determined by scanning for the terminator, so this is O(size).
     * @note A constrained template instead of a plain const_pointer parameter: a plain
     *       pointer parameter would be preferred over the array constructor below for
     *       string literals, silently truncating embedded null code units. String
     *       literals decay before deduction, so the non-template array constructor
     *       below wins the tiebreak for them.
     */
    template<typename PointerType>
        requires (::std::same_as<PointerType, value_type const*> || ::std::same_as<PointerType, value_type*>)
    constexpr BasicCStringView(PointerType string) noexcept;

    /**
     * @brief Builds a view over the first size_with_null - 1 characters of @p string.
     * @pre The last element string[size_with_null - 1] is the terminator (checked O(1)
     *      when contracts are enforced; not a scan).
     */
    template<::std::size_t size_with_null>
    constexpr BasicCStringView(value_type const (&string)[size_with_null]) noexcept;

    /**
     * @brief Builds a view over the first @p length_ characters of @p string.
     * @pre string is a valid pointer and string[length_] == value_type{}.
     * @note O(1); the terminator is asserted at the given offset when contracts are enforced.
     */
    constexpr explicit BasicCStringView(const_pointer string, size_type length_) noexcept;

    /**
     * @brief Builds a view over the contiguous range [begin, end).
     * @pre *(begin + (end - begin)) == value_type{}.
     * @note O(1); the terminator is asserted when contracts are enforced.
     */
    template<typename InputIterator, typename EndSentinel>
        requires (::std::contiguous_iterator<InputIterator> && ::std::sized_sentinel_for<EndSentinel, InputIterator> &&
                  ::std::same_as<::std::iter_value_t<InputIterator>, value_type> &&
                  !::std::convertible_to<EndSentinel, size_type>)
    constexpr BasicCStringView(InputIterator begin, EndSentinel end) noexcept(
        noexcept(::std::to_address(::std::declval<InputIterator&>())) &&
        noexcept(::std::declval<EndSentinel&>() - ::std::declval<InputIterator&>()) &&
        noexcept(::std::declval<InputIterator&>()[::std::size_t{}]));

    /**
     * @brief Builds a view over the contiguous, sized range @p range.
     * @pre The element at range[size()] exists, is not reached through a null
     *      data pointer, and equals value_type{}.
     * @note O(1); the terminator is asserted when contracts are enforced. Callers
     *       are responsible for the termination guarantee; structurally impossible
     *       sources are still excluded (see is_ntcts_range).
     */
    template<::pltxt2htm::container::details::is_ntcts_range<value_type> RangeType>
        requires (!::std::same_as<::std::remove_cvref_t<RangeType>, BasicCStringView> &&
                  !::std::same_as<::std::remove_cvref_t<RangeType>, BasicStringView<value_type>> &&
                  !::pltxt2htm::details::is_literal_string<RangeType> &&
                  !::pltxt2htm::details::is_basic_inplace_string<RangeType> &&
                  !::pltxt2htm::container::details::has_allocator_type<RangeType> &&
                  !::std::convertible_to<RangeType, const_pointer>)
    constexpr explicit BasicCStringView(RangeType&& range) noexcept(
        noexcept(::std::ranges::data(::std::declval<RangeType&>())) &&
        noexcept(::std::ranges::size(::std::declval<RangeType&>())));

    /**
     * @brief Builds a view over a whole null-terminated owning string.
     * @tparam AllocatorType Allocator of the source string.
     * @note O(1); BasicString always maintains its trailing terminator.
     */
    template<typename AllocatorType>
    constexpr BasicCStringView(BasicString<value_type, AllocatorType> const& string) noexcept
        : pointer{string.data()},
          length{string.size()} {
    }

    template<typename AllocatorType>
    constexpr BasicCStringView(BasicString<value_type, AllocatorType>&&) = delete
#if __cpp_deleted_function >= 202403L
    #if defined __clang__
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wc++26-extensions"
    #endif
        ("the cstring_view would outlive the temporary it refers to")
    #if defined __clang__
        #pragma clang diagnostic pop
    #endif
#endif
        ;

    template<typename AllocatorType>
    constexpr BasicCStringView(BasicString<value_type, AllocatorType> const&&) = delete
#if __cpp_deleted_function >= 202403L
    #if defined __clang__
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wc++26-extensions"
    #endif
        ("the cstring_view would outlive the temporary it refers to")
    #if defined __clang__
        #pragma clang diagnostic pop
    #endif
#endif
        ;

    /**
     * @brief Builds a view over the whole null-terminated sequence referred to by @p cstring.
     * @note The terminator is retained: the whole cstring view stays null-terminated.
     */
    [[nodiscard]]
    constexpr auto data(this BasicCStringView const& self) noexcept -> const_pointer {
        return self.pointer;
    }

    /**
     * @brief Returns the same pointer as data(); usable as a null-terminated C string.
     * @pre !has_nulls().
     */
    [[nodiscard]]
    constexpr auto c_str(this BasicCStringView const& self) noexcept -> const_pointer {
        return self.pointer;
    }

    [[nodiscard]]
    constexpr auto size(this BasicCStringView const& self) noexcept -> size_type {
        return self.length;
    }

    /**
     * @brief Returns the largest possible number of code units referable by a view.
     */
    [[nodiscard]]
    constexpr auto max_size(this BasicCStringView const&) noexcept -> size_type {
        return ::std::numeric_limits<size_type>::max();
    }

    [[nodiscard]]
    constexpr auto is_empty(this BasicCStringView const& self) noexcept -> bool {
        return self.length == 0;
    }

    /**
     * @brief Tests whether the view has no characters, for downstream users only.
     * @return true when size() is zero.
     */
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto empty(this BasicCStringView const&) noexcept -> bool = delete
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
    constexpr auto empty(this BasicCStringView const& self) noexcept -> bool {
        return self.is_empty();
    }
#endif

    [[nodiscard]]
    constexpr auto begin(this BasicCStringView const& self) noexcept -> const_iterator {
        return self.pointer;
    }

    [[nodiscard]]
    constexpr auto end(this BasicCStringView const& self) noexcept -> const_iterator {
        return self.pointer + self.length;
    }

    [[nodiscard]]
    constexpr auto cbegin(this BasicCStringView const& self) noexcept -> const_iterator {
        return self.begin();
    }

    [[nodiscard]]
    constexpr auto cend(this BasicCStringView const& self) noexcept -> const_iterator {
        return self.end();
    }

    [[nodiscard]]
    constexpr auto rbegin(this BasicCStringView const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto rend(this BasicCStringView const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto crbegin(this BasicCStringView const& self) noexcept -> const_reverse_iterator {
        return self.rbegin();
    }

    [[nodiscard]]
    constexpr auto crend(this BasicCStringView const& self) noexcept -> const_reverse_iterator {
        return self.rend();
    }

    /**
     * @brief Indexed element access, read-only. pos == size() yields the null terminator.
     * @param position Zero-based element position.
     * @return Read-only reference to the requested element.
     * @pre position is in the closed range `[0, size()]`.
     */
    [[nodiscard]]
    constexpr auto operator[](this BasicCStringView const& self, size_type position) noexcept -> const_reference;

    /**
     * @brief First character.
     * @pre size() != 0.
     */
    [[nodiscard]]
    constexpr auto front(this BasicCStringView const& self) noexcept -> const_reference;

    /**
     * @brief Last character before the terminator.
     * @pre size() != 0.
     */
    [[nodiscard]]
    constexpr auto back(this BasicCStringView const& self) noexcept -> const_reference;

    /**
     * @brief Drops the first @p count characters, retaining the terminator.
     * @pre count <= size().
     */
    constexpr auto remove_prefix(this BasicCStringView& self, size_type count) noexcept -> void;

    /**
     * @brief Removing characters from the end in place would drop the null terminator.
     * @note Use the two-argument subview(size_type, size_type), which returns a
     *       BasicStringView instead.
     */
    constexpr auto remove_suffix(this BasicCStringView&, size_type) noexcept -> void = delete
#if __cpp_deleted_function >= 202403L
    #if defined __clang__
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wc++26-extensions"
    #endif
        ("removing characters from the end would drop the null terminator; use subview(size_type, size_type) instead")
    #if defined __clang__
        #pragma clang diagnostic pop
    #endif
#endif
        ;

    /**
     * @brief Exchanges the values of this view and @p other.
     */
    constexpr auto swap(this BasicCStringView& self, BasicCStringView& other) noexcept -> void {
        ::std::swap(self.pointer, other.pointer);
        ::std::swap(self.length, other.length);
    }

    /**
     * @brief Copies [position, position + copy_count) to @p destination, where
     *        copy_count == min(count, size() - position).
     * @param destination Valid range of at least copy_count elements.
     * @return Number of copied elements.
     * @pre position <= size().
     */
    [[nodiscard]]
    constexpr auto copy(this BasicCStringView const& self, value_type* destination, size_type count,
                        size_type position = 0) noexcept -> size_type;

    /**
     * @brief Returns the suffix from @p position, retaining the null terminator.
     * @pre position <= size().
     */
    [[nodiscard]]
    constexpr auto subview(this BasicCStringView const& self, size_type position = 0) noexcept -> BasicCStringView;

    /**
     * @brief Returns a view of the first @p count characters from @p position.
     * @details The result is not guaranteed null-terminated, hence BasicStringView.
     * @pre position <= size() and count <= size() - position.
     */
    [[nodiscard]]
    constexpr auto subview(this BasicCStringView const& self, size_type position, size_type count) noexcept
        -> BasicStringView<CharType>;

    [[nodiscard]]
    constexpr auto starts_with(this BasicCStringView const& self, BasicStringView<CharType> prefix) noexcept -> bool {
        return prefix.size() <= self.length && ::std::equal(prefix.begin(), prefix.end(), self.pointer);
    }

    [[nodiscard]]
    constexpr auto starts_with(this BasicCStringView const& self, value_type prefix) noexcept -> bool {
        return self.length != 0 && self.pointer[0] == prefix;
    }

    [[nodiscard]]
    constexpr auto starts_with(this BasicCStringView const& self, const_pointer prefix) noexcept -> bool {
        return self.starts_with(BasicStringView<CharType>{prefix, ::fast_io::cstr_len(prefix)});
    }

    [[nodiscard]]
    constexpr auto ends_with(this BasicCStringView const& self, BasicStringView<CharType> suffix) noexcept -> bool {
        if (suffix.size() > self.length) {
            return false;
        }
        const_pointer const suffix_begin{suffix.size() == self.length ? self.pointer
                                                                      : self.pointer + self.length - suffix.size()};
        return ::std::equal(suffix.begin(), suffix.end(), suffix_begin);
    }

    [[nodiscard]]
    constexpr auto ends_with(this BasicCStringView const& self, value_type suffix) noexcept -> bool {
        return self.length != 0 && self.pointer[self.length - 1] == suffix;
    }

    [[nodiscard]]
    constexpr auto ends_with(this BasicCStringView const& self, const_pointer suffix) noexcept -> bool {
        return self.ends_with(BasicStringView<CharType>{suffix, ::fast_io::cstr_len(suffix)});
    }

    [[nodiscard]]
    constexpr auto contains(this BasicCStringView const& self, BasicStringView<CharType> needle) noexcept -> bool {
        return self.find(needle) != npos;
    }

    [[nodiscard]]
    constexpr auto contains(this BasicCStringView const& self, value_type needle) noexcept -> bool {
        return self.find(needle) != npos;
    }

    [[nodiscard]]
    constexpr auto contains(this BasicCStringView const& self, const_pointer needle) noexcept -> bool {
        return self.find(needle) != npos;
    }

    [[nodiscard]]
    constexpr auto find(this BasicCStringView const& self, BasicStringView<CharType> needle,
                        size_type position = 0) noexcept -> size_type {
        if (position > self.length) {
            return npos;
        }
        if (needle.is_empty()) {
            return position;
        }
        if (needle.size() > self.length - position) {
            return npos;
        }
        size_type const scan_limit = self.length - needle.size();
        for (size_type index = position; index <= scan_limit; ++index) {
            if (::std::equal(needle.begin(), needle.end(), self.pointer + index)) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto find(this BasicCStringView const& self, value_type needle, size_type position = 0) noexcept
        -> size_type {
        if (position >= self.length) {
            return npos;
        }
        for (size_type index = position; index < self.length; ++index) {
            if (self.pointer[index] == needle) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto find(this BasicCStringView const& self, const_pointer needle, size_type position,
                        size_type count) noexcept -> size_type {
        return self.find(BasicStringView<CharType>{needle, count}, position);
    }

    [[nodiscard]]
    constexpr auto find(this BasicCStringView const& self, const_pointer needle, size_type position = 0) noexcept
        -> size_type {
        return self.find(needle, position, ::fast_io::cstr_len(needle));
    }

    [[nodiscard]]
    constexpr auto rfind(this BasicCStringView const& self, BasicStringView<CharType> needle,
                         size_type position = npos) noexcept -> size_type {
        if (needle.size() > self.length) {
            return npos;
        }
        size_type const clamped_position = ::std::min(position, self.length);
        if (needle.is_empty()) {
            return clamped_position;
        }
        size_type const start = ::std::min(clamped_position, self.length - needle.size());
        for (size_type index = start + 1; index-- > 0;) {
            if (::std::equal(needle.begin(), needle.end(), self.pointer + index)) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto rfind(this BasicCStringView const& self, value_type needle, size_type position = npos) noexcept
        -> size_type {
        if (self.length == 0) {
            return npos;
        }
        size_type const start = ::std::min(position, self.length - 1);
        for (size_type index = start + 1; index-- > 0;) {
            if (self.pointer[index] == needle) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto rfind(this BasicCStringView const& self, const_pointer needle, size_type position,
                         size_type count) noexcept -> size_type {
        return self.rfind(BasicStringView<CharType>{needle, count}, position);
    }

    [[nodiscard]]
    constexpr auto rfind(this BasicCStringView const& self, const_pointer needle, size_type position = npos) noexcept
        -> size_type {
        return self.rfind(needle, position, ::fast_io::cstr_len(needle));
    }

    [[nodiscard]]
    constexpr auto find_first_of(this BasicCStringView const& self, BasicStringView<CharType> set,
                                 size_type position = 0) noexcept -> size_type {
        if (position >= self.length) {
            return npos;
        }
        for (size_type index = position; index < self.length; ++index) {
            if (::std::find(set.begin(), set.end(), self.pointer[index]) != set.end()) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto find_first_of(this BasicCStringView const& self, value_type set, size_type position = 0) noexcept
        -> size_type {
        return self.find(set, position);
    }

    [[nodiscard]]
    constexpr auto find_first_of(this BasicCStringView const& self, const_pointer set, size_type position,
                                 size_type count) noexcept -> size_type {
        return self.find_first_of(BasicStringView<CharType>{set, count}, position);
    }

    [[nodiscard]]
    constexpr auto find_first_of(this BasicCStringView const& self, const_pointer set, size_type position = 0) noexcept
        -> size_type {
        return self.find_first_of(set, position, ::fast_io::cstr_len(set));
    }

    [[nodiscard]]
    constexpr auto find_last_of(this BasicCStringView const& self, BasicStringView<CharType> set,
                                size_type position = npos) noexcept -> size_type {
        if (self.length == 0) {
            return npos;
        }
        size_type const start = ::std::min(position, self.length - 1);
        for (size_type index = start + 1; index-- > 0;) {
            if (::std::find(set.begin(), set.end(), self.pointer[index]) != set.end()) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto find_last_of(this BasicCStringView const& self, value_type set, size_type position = npos) noexcept
        -> size_type {
        return self.rfind(set, position);
    }

    [[nodiscard]]
    constexpr auto find_last_of(this BasicCStringView const& self, const_pointer set, size_type position,
                                size_type count) noexcept -> size_type {
        return self.find_last_of(BasicStringView<CharType>{set, count}, position);
    }

    [[nodiscard]]
    constexpr auto find_last_of(this BasicCStringView const& self, const_pointer set,
                                size_type position = npos) noexcept -> size_type {
        return self.find_last_of(set, position, ::fast_io::cstr_len(set));
    }

    [[nodiscard]]
    constexpr auto find_first_not_of(this BasicCStringView const& self, BasicStringView<CharType> set,
                                     size_type position = 0) noexcept -> size_type {
        if (position >= self.length) {
            return npos;
        }
        for (size_type index = position; index < self.length; ++index) {
            if (::std::find(set.begin(), set.end(), self.pointer[index]) == set.end()) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto find_first_not_of(this BasicCStringView const& self, value_type set, size_type position = 0) noexcept
        -> size_type {
        if (position >= self.length) {
            return npos;
        }
        for (size_type index = position; index < self.length; ++index) {
            if (self.pointer[index] != set) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto find_first_not_of(this BasicCStringView const& self, const_pointer set, size_type position,
                                     size_type count) noexcept -> size_type {
        return self.find_first_not_of(BasicStringView<CharType>{set, count}, position);
    }

    [[nodiscard]]
    constexpr auto find_first_not_of(this BasicCStringView const& self, const_pointer set,
                                     size_type position = 0) noexcept -> size_type {
        return self.find_first_not_of(set, position, ::fast_io::cstr_len(set));
    }

    [[nodiscard]]
    constexpr auto find_last_not_of(this BasicCStringView const& self, BasicStringView<CharType> set,
                                    size_type position = npos) noexcept -> size_type {
        if (self.length == 0) {
            return npos;
        }
        size_type const start = ::std::min(position, self.length - 1);
        for (size_type index = start + 1; index-- > 0;) {
            if (::std::find(set.begin(), set.end(), self.pointer[index]) == set.end()) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto find_last_not_of(this BasicCStringView const& self, value_type set,
                                    size_type position = npos) noexcept -> size_type {
        if (self.length == 0) {
            return npos;
        }
        size_type const start = ::std::min(position, self.length - 1);
        for (size_type index = start + 1; index-- > 0;) {
            if (self.pointer[index] != set) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto find_last_not_of(this BasicCStringView const& self, const_pointer set, size_type position,
                                    size_type count) noexcept -> size_type {
        return self.find_last_not_of(BasicStringView<CharType>{set, count}, position);
    }

    [[nodiscard]]
    constexpr auto find_last_not_of(this BasicCStringView const& self, const_pointer set,
                                    size_type position = npos) noexcept -> size_type {
        return self.find_last_not_of(set, position, ::fast_io::cstr_len(set));
    }

    /**
     * @brief Reports whether any embedded null code unit exists in the view.
     * @note O(size()); embedded nulls are allowed at the type level and managed at
     *       the value level.
     */
    [[nodiscard]]
    constexpr auto has_nulls(this BasicCStringView const& self) noexcept -> bool {
        for (size_type index{}; index < self.length; ++index) {
            if (self.pointer[index] == value_type{}) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Returns the view truncated at the first embedded null code unit.
     * @return The first embedded-null-free piece, or the whole view when there is none.
     * @note O(size()).
     */
    [[nodiscard]]
    constexpr auto first_piece(this BasicCStringView const& self) noexcept -> BasicCStringView {
        for (size_type index{}; index < self.length; ++index) {
            if (self.pointer[index] == value_type{}) {
                return BasicCStringView{self.pointer, index};
            }
        }
        return self;
    }

    /**
     * @brief Converts to a BasicStringView over the same characters, without the terminator.
     * @details Implicit by design: every null-terminated sequence is a valid character
     *          sequence. Mirrors std::basic_string's operator basic_string_view and
     *          P3655's transparent cstring_view to string_view conversion.
     */
    [[nodiscard]]
    constexpr operator BasicStringView<CharType>(this BasicCStringView const& self) noexcept {
        return BasicStringView<CharType>{self.pointer, self.length};
    }

    /**
     * @brief Compares two cstring views by size and character values.
     */
    [[nodiscard]]
    constexpr auto operator==(this BasicCStringView const& self, BasicCStringView right) noexcept -> bool {
        return self.length == right.length && ::std::equal(self.pointer, self.pointer + self.length, right.pointer);
    }
};

/**
 * @brief Builds a BasicStringView over the whole sequence referred to by @p cstring.
 * @details Implicit by design: every null-terminated sequence is a valid character
 *          sequence. The terminator is not part of the result.
 */
template<::pltxt2htm::details::is_char_type CharType>
[[nodiscard]]
constexpr auto print_alias_define(::fast_io::io_alias_t, BasicCStringView<CharType> string) noexcept
    -> ::fast_io::basic_io_scatter_t<CharType> {
    return {string.data(), string.size()};
}

/**
 * @name BasicCStringView deduction guides
 * @{
 * @note There are no guides for the pointer constructor (the pointee cannot prove
 *       null termination at compile time) or for the array constructor (its implicit
 *       guide already deduces string literals; an explicit one would be ambiguous
 *       with it during partial ordering).
 */
template<typename InputIterator, typename EndSentinel>
    requires (::pltxt2htm::details::is_char_type<::std::iter_value_t<InputIterator>>)
BasicCStringView(InputIterator, EndSentinel) -> BasicCStringView<::std::iter_value_t<InputIterator>>;

template<typename RangeType>
    requires (::pltxt2htm::details::is_char_type<::std::ranges::range_value_t<RangeType>>)
BasicCStringView(RangeType&&) -> BasicCStringView<::std::ranges::range_value_t<RangeType>>;

template<::pltxt2htm::details::is_char_type CharType, typename AllocatorType>
BasicCStringView(BasicString<CharType, AllocatorType> const&) -> BasicCStringView<CharType>;
/** @} */

using CStringView = BasicCStringView<char>;
using WCStringView = BasicCStringView<wchar_t>;
using U8CStringView = BasicCStringView<char8_t>;
using U16CStringView = BasicCStringView<char16_t>;
using U32CStringView = BasicCStringView<char32_t>;

} // namespace pltxt2htm::container

#pragma pop_macro("max")
#pragma pop_macro("min")
