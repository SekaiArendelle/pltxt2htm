/**
 * @file string_view_decl.hh
 * @brief Declarations for container::BasicStringView.
 * @details Contract-checked members are declared here and defined in
 *          string_view_def.inc. Members defined inline that call those members
 *          also need string_view_def.inc in the same translation unit; include
 *          container/string_view.hh for the complete API.
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <limits>
#include <utility>

#include <fast_io/fast_io_core.h>

#include "../../details/concepts.hh"
#include "../../details/literal_string.hh"
#include "../../details/impl/inplace_string_decl.hh"

// windows.h defines min and max as function-like macros, which would expand
// inside std::numeric_limits<...>::max(). Keep them undefined in this header.
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace pltxt2htm::container {

/**
 * @brief A non-owning view over a contiguous sequence of characters.
 * @tparam CharType Character type stored by the referenced sequence.
 *
 * BasicStringView mirrors std::basic_string_view where the semantics map onto
 * this library: exception-based facilities (at, compare and the ordering
 * operators) and conversions from owning containers are omitted, bounds
 * violations are contract-checked with the same panic/assertion policy as the
 * rest of the library, and the null-terminated flavor with the search
 * operations is container::BasicCStringView.
 */
template<::pltxt2htm::details::is_char_type CharType>
class BasicStringView {
public:
    using value_type = CharType;
    using size_type = ::std::size_t;
    using difference_type = ::std::ptrdiff_t;
    using const_reference = value_type const&;
    using const_pointer = value_type const*;
    using const_iterator = const_pointer;
    using iterator = const_iterator;
    using const_reverse_iterator = ::std::reverse_iterator<const_pointer>;
    using reverse_iterator = const_reverse_iterator;

    static constexpr size_type npos{::std::numeric_limits<size_type>::max()};

private:
    const_pointer pointer{};
    size_type length{};

public:
    constexpr BasicStringView() noexcept = default;

    constexpr BasicStringView(BasicStringView const&) noexcept = default;

    constexpr ~BasicStringView() noexcept = default;

    constexpr BasicStringView(::std::nullptr_t) = delete
#if __cpp_deleted_function >= 202403L
    #if defined __clang__
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wc++26-extensions"
    #endif
        ("use a string literal or a string_view instead of a null pointer")
    #if defined __clang__
        #pragma clang diagnostic pop
    #endif
#endif
        ;

    constexpr explicit BasicStringView(const_pointer pointer_, size_type size_) noexcept
        : pointer{pointer_},
          length{size_} {
    }

    template<::std::size_t size_with_null>
    constexpr BasicStringView(value_type const (&string)[size_with_null]) noexcept
        : pointer{string},
          length{size_with_null - 1} {
        static_assert(size_with_null != 0);
    }

    template<::std::size_t size>
    constexpr BasicStringView(::pltxt2htm::details::BasicLiteralString<value_type, size> const& string) noexcept
        : pointer{string.data()},
          length{string.size()} {
    }

    template<::std::size_t size>
    constexpr BasicStringView(::pltxt2htm::details::BasicLiteralString<value_type, size>&&) = delete
#if __cpp_deleted_function >= 202403L
    #if defined __clang__
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wc++26-extensions"
    #endif
        ("the string_view would outlive the temporary it refers to")
    #if defined __clang__
        #pragma clang diagnostic pop
    #endif
#endif
        ;

    template<::std::size_t size>
    constexpr BasicStringView(::pltxt2htm::details::BasicLiteralString<value_type, size> const&&) = delete
#if __cpp_deleted_function >= 202403L
    #if defined __clang__
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wc++26-extensions"
    #endif
        ("the string_view would outlive the temporary it refers to")
    #if defined __clang__
        #pragma clang diagnostic pop
    #endif
#endif
        ;

    template<::std::size_t extent>
    constexpr BasicStringView(::pltxt2htm::details::BasicInplaceString<value_type, extent> const& string) noexcept
        : pointer{string.data()},
          length{string.size()} {
    }

    template<::std::size_t extent>
    constexpr BasicStringView(::pltxt2htm::details::BasicInplaceString<value_type, extent>&&) = delete
#if __cpp_deleted_function >= 202403L
    #if defined __clang__
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wc++26-extensions"
    #endif
        ("the string_view would outlive the temporary it refers to")
    #if defined __clang__
        #pragma clang diagnostic pop
    #endif
#endif
        ;

    template<::std::size_t extent>
    constexpr BasicStringView(::pltxt2htm::details::BasicInplaceString<value_type, extent> const&&) = delete
#if __cpp_deleted_function >= 202403L
    #if defined __clang__
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wc++26-extensions"
    #endif
        ("the string_view would outlive the temporary it refers to")
    #if defined __clang__
        #pragma clang diagnostic pop
    #endif
#endif
        ;

    // The standard implements this as a defaulted copy assignment with an implicit
    // object parameter; std::basic_string_view has no move assignment. The project
    // convention prefers an explicit object parameter, but clang 23.1.3 crashes in
    // the backend when generating code for a defaulted explicit-object operator= on
    // a class with two or more members; see
    // https://github.com/llvm/llvm-project/issues/242767. Use the implicit object
    // form until that bug is fixed; the lvalue ref-qualifier keeps the convention's
    // intent of rejecting assignment to temporaries. Staying defaulted also keeps
    // the copy assignment trivial, preserving is_trivially_copyable.
    constexpr BasicStringView& operator=(BasicStringView const&) & noexcept = default;

    /**
     * @brief Builds a view over a null-terminated string.
     * @param[in] string Null-terminated string; must not be null.
     * @return A view over the string, excluding the terminator.
     * @pre string != nullptr
     * @note The length is determined by scanning for the terminator, so this is O(size).
     */
    [[nodiscard]]
    static constexpr auto from_c_str(const_pointer string) noexcept -> BasicStringView {
        return BasicStringView{string, ::fast_io::cstr_len(string)};
    }

    [[nodiscard]]
    constexpr auto data(this BasicStringView const& self) noexcept -> const_pointer {
        return self.pointer;
    }

    [[nodiscard]]
    constexpr auto size(this BasicStringView const& self) noexcept -> size_type {
        return self.length;
    }

    [[nodiscard]]
    constexpr auto is_empty(this BasicStringView const& self) noexcept -> bool {
        return self.length == 0;
    }

    /**
     * @brief Tests whether the view has no characters, for downstream users only.
     * @return true when size() is zero.
     */
#if defined(PLTXT2HTM_INTERNAL_USE)
    constexpr auto empty(this BasicStringView const&) noexcept -> bool = delete
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
    constexpr auto empty(this BasicStringView const& self) noexcept -> bool {
        return self.is_empty();
    }
#endif

    [[nodiscard]]
    constexpr auto begin(this BasicStringView const& self) noexcept -> const_iterator {
        return self.pointer;
    }

    [[nodiscard]]
    constexpr auto end(this BasicStringView const& self) noexcept -> const_iterator {
        if (self.pointer == nullptr) {
            return nullptr;
        }
        return self.pointer + self.length;
    }

    [[nodiscard]]
    constexpr auto cbegin(this BasicStringView const& self) noexcept -> const_iterator {
        return self.begin();
    }

    [[nodiscard]]
    constexpr auto cend(this BasicStringView const& self) noexcept -> const_iterator {
        return self.end();
    }

    [[nodiscard]]
    constexpr auto rbegin(this BasicStringView const& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto rend(this BasicStringView const& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto crbegin(this BasicStringView const& self) noexcept -> const_reverse_iterator {
        return self.rbegin();
    }

    [[nodiscard]]
    constexpr auto crend(this BasicStringView const& self) noexcept -> const_reverse_iterator {
        return self.rend();
    }

    /**
     * @brief Indexed element access, read-only, bounds-checked according to the
     * build-wide contract setting.
     * @param position Zero-based element position.
     * @return Read-only reference to the requested element.
     * @pre position is a valid element position.
     */
    [[nodiscard]]
    constexpr auto operator[](this BasicStringView const& self, size_type position) noexcept -> const_reference;

    /**
     * @brief Access to the first character, read-only.
     * @return Read-only reference to the character at position zero.
     * @pre size() != 0
     */
    [[nodiscard]]
    constexpr auto front(this BasicStringView const& self) noexcept -> const_reference;

    /**
     * @brief Access to the last character, read-only.
     * @return Read-only reference to the character at size() - 1.
     * @pre size() != 0
     */
    [[nodiscard]]
    constexpr auto back(this BasicStringView const& self) noexcept -> const_reference;

    [[nodiscard]]
    constexpr auto subview(this BasicStringView const& self, size_type position, size_type count = npos) noexcept
        -> BasicStringView;

    /**
     * @brief Drops @p count leading characters.
     * @param count Number of leading characters to drop.
     * @pre count <= size()
     */
    constexpr void remove_prefix(this BasicStringView& self, size_type count) noexcept;

    /**
     * @brief Drops @p count trailing characters.
     * @param count Number of trailing characters to drop.
     * @pre count <= size()
     */
    constexpr void remove_suffix(this BasicStringView& self, size_type count) noexcept;

    /**
     * @brief Exchanges the referenced sequence with @p right.
     */
    constexpr void swap(this BasicStringView& self, BasicStringView& other) noexcept {
        auto const tmp_pointer = self.pointer;
        auto const tmp_length = self.length;
        self.pointer = other.pointer;
        self.length = other.length;
        other.pointer = tmp_pointer;
        other.length = tmp_length;
    }

    [[nodiscard]]
    constexpr auto starts_with(this BasicStringView const& self, BasicStringView prefix) noexcept -> bool {
        return prefix.length <= self.length && ::std::equal(prefix.begin(), prefix.end(), self.begin());
    }

    [[nodiscard]]
    constexpr auto starts_with(this BasicStringView const& self, value_type prefix) noexcept -> bool {
        return !self.is_empty() && self.pointer[0] == prefix;
    }

    [[nodiscard]]
    constexpr auto ends_with(this BasicStringView const& self, BasicStringView suffix) noexcept -> bool {
        if (suffix.length > self.length) {
            return false;
        }
        const_pointer const suffix_begin{suffix.length == self.length ? self.pointer : self.end() - suffix.length};
        return ::std::equal(suffix.begin(), suffix.end(), suffix_begin);
    }

    [[nodiscard]]
    constexpr auto ends_with(this BasicStringView const& self, value_type suffix) noexcept -> bool {
        return !self.is_empty() && self.pointer[self.length - 1] == suffix;
    }

    /**
     * @brief Tests whether @p needle appears as a substring.
     * @return true when @p needle is empty or occurs at some position.
     */
    [[nodiscard]]
    constexpr auto contains(this BasicStringView const& self, BasicStringView needle) noexcept -> bool {
        return self.find(needle) != npos;
    }

    [[nodiscard]]
    constexpr auto contains(this BasicStringView const& self, value_type needle) noexcept -> bool {
        return self.find(needle) != npos;
    }

    [[nodiscard]]
    constexpr auto contains(this BasicStringView const& self, const_pointer needle) noexcept -> bool {
        return self.find(needle) != npos;
    }

    /**
     * @brief Finds the first position where @p needle appears as a substring.
     * @return The match position, or npos. An empty @p needle matches at
     *         @p position itself.
     */
    [[nodiscard]]
    constexpr auto find(this BasicStringView const& self, BasicStringView needle, size_type position = 0) noexcept
        -> size_type {
        if (position > self.length) {
            return npos;
        }
        if (needle.is_empty()) {
            return position;
        }
        if (needle.length > self.length - position) {
            return npos;
        }
        size_type const scan_limit = self.length - needle.length;
        for (size_type index = position; index <= scan_limit; ++index) {
            if (::std::equal(needle.begin(), needle.end(), self.pointer + index)) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto find(this BasicStringView const& self, value_type needle, size_type position = 0) noexcept
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
    constexpr auto find(this BasicStringView const& self, const_pointer needle, size_type position,
                        size_type count) noexcept -> size_type {
        return self.find(BasicStringView{needle, count}, position);
    }

    [[nodiscard]]
    constexpr auto find(this BasicStringView const& self, const_pointer needle, size_type position = 0) noexcept
        -> size_type {
        return self.find(needle, position, ::fast_io::cstr_len(needle));
    }

    /**
     * @brief Finds the last position where @p needle appears as a substring,
     *        starting the search at or before @p position.
     * @return The match position, or npos. An empty @p needle matches at the
     *         clamped @p position.
     */
    [[nodiscard]]
    constexpr auto rfind(this BasicStringView const& self, BasicStringView needle, size_type position = npos) noexcept
        -> size_type {
        if (needle.length > self.length) {
            return npos;
        }
        size_type const clamped_position = ::std::min(position, self.length);
        if (needle.is_empty()) {
            return clamped_position;
        }
        size_type const start = ::std::min(clamped_position, self.length - needle.length);
        for (size_type index = start + 1; index-- > 0;) {
            if (::std::equal(needle.begin(), needle.end(), self.pointer + index)) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto rfind(this BasicStringView const& self, value_type needle, size_type position = npos) noexcept
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
    constexpr auto rfind(this BasicStringView const& self, const_pointer needle, size_type position,
                         size_type count) noexcept -> size_type {
        return self.rfind(BasicStringView{needle, count}, position);
    }

    [[nodiscard]]
    constexpr auto rfind(this BasicStringView const& self, const_pointer needle, size_type position = npos) noexcept
        -> size_type {
        return self.rfind(needle, position, ::fast_io::cstr_len(needle));
    }

    /**
     * @brief Finds the first position whose character appears in @p set.
     * @return The match position, or npos. An empty @p set matches nothing.
     */
    [[nodiscard]]
    constexpr auto find_first_of(this BasicStringView const& self, BasicStringView set, size_type position = 0) noexcept
        -> size_type {
        if (position >= self.length) {
            return npos;
        }
        for (size_type index = position; index < self.length; ++index) {
            if (::std::ranges::find(set, self.pointer[index]) != set.end()) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto find_first_of(this BasicStringView const& self, value_type set, size_type position = 0) noexcept
        -> size_type {
        return self.find(set, position);
    }

    [[nodiscard]]
    constexpr auto find_first_of(this BasicStringView const& self, const_pointer set, size_type position,
                                 size_type count) noexcept -> size_type {
        return self.find_first_of(BasicStringView{set, count}, position);
    }

    [[nodiscard]]
    constexpr auto find_first_of(this BasicStringView const& self, const_pointer set, size_type position = 0) noexcept
        -> size_type {
        return self.find_first_of(set, position, ::fast_io::cstr_len(set));
    }

    /**
     * @brief Finds the last position whose character appears in @p set, starting
     *        the search at or before @p position.
     * @return The match position, or npos. An empty @p set matches nothing.
     */
    [[nodiscard]]
    constexpr auto find_last_of(this BasicStringView const& self, BasicStringView set,
                                size_type position = npos) noexcept -> size_type {
        if (self.length == 0) {
            return npos;
        }
        size_type const start = ::std::min(position, self.length - 1);
        for (size_type index = start + 1; index-- > 0;) {
            if (::std::ranges::find(set, self.pointer[index]) != set.end()) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto find_last_of(this BasicStringView const& self, value_type set, size_type position = npos) noexcept
        -> size_type {
        return self.rfind(set, position);
    }

    [[nodiscard]]
    constexpr auto find_last_of(this BasicStringView const& self, const_pointer set, size_type position,
                                size_type count) noexcept -> size_type {
        return self.find_last_of(BasicStringView{set, count}, position);
    }

    [[nodiscard]]
    constexpr auto find_last_of(this BasicStringView const& self, const_pointer set, size_type position = npos) noexcept
        -> size_type {
        return self.find_last_of(set, position, ::fast_io::cstr_len(set));
    }

    /**
     * @brief Finds the first position whose character does not appear in @p set.
     * @return The match position, or npos. An empty @p set matches the first
     *         character from @p position onward.
     */
    [[nodiscard]]
    constexpr auto find_first_not_of(this BasicStringView const& self, BasicStringView set,
                                     size_type position = 0) noexcept -> size_type {
        if (position >= self.length) {
            return npos;
        }
        for (size_type index = position; index < self.length; ++index) {
            if (::std::ranges::find(set, self.pointer[index]) == set.end()) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto find_first_not_of(this BasicStringView const& self, value_type set, size_type position = 0) noexcept
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
    constexpr auto find_first_not_of(this BasicStringView const& self, const_pointer set, size_type position,
                                     size_type count) noexcept -> size_type {
        return self.find_first_not_of(BasicStringView{set, count}, position);
    }

    [[nodiscard]]
    constexpr auto find_first_not_of(this BasicStringView const& self, const_pointer set,
                                     size_type position = 0) noexcept -> size_type {
        return self.find_first_not_of(set, position, ::fast_io::cstr_len(set));
    }

    /**
     * @brief Finds the last position whose character does not appear in @p set,
     *        starting the search at or before @p position.
     * @return The match position, or npos. An empty @p set matches the last
     *         character before @p position.
     */
    [[nodiscard]]
    constexpr auto find_last_not_of(this BasicStringView const& self, BasicStringView set,
                                    size_type position = npos) noexcept -> size_type {
        if (self.length == 0) {
            return npos;
        }
        size_type const start = ::std::min(position, self.length - 1);
        for (size_type index = start + 1; index-- > 0;) {
            if (::std::ranges::find(set, self.pointer[index]) == set.end()) {
                return index;
            }
        }
        return npos;
    }

    [[nodiscard]]
    constexpr auto find_last_not_of(this BasicStringView const& self, value_type set,
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
    constexpr auto find_last_not_of(this BasicStringView const& self, const_pointer set, size_type position,
                                    size_type count) noexcept -> size_type {
        return self.find_last_not_of(BasicStringView{set, count}, position);
    }

    [[nodiscard]]
    constexpr auto find_last_not_of(this BasicStringView const& self, const_pointer set,
                                    size_type position = npos) noexcept -> size_type {
        return self.find_last_not_of(set, position, ::fast_io::cstr_len(set));
    }

    [[nodiscard]]
    constexpr auto operator==(this BasicStringView const& self, BasicStringView right) noexcept -> bool {
        return self.length == right.length && ::std::equal(self.begin(), self.end(), right.begin());
    }
};

template<::pltxt2htm::details::is_char_type CharType>
[[nodiscard]]
constexpr auto print_alias_define(::fast_io::io_alias_t, BasicStringView<CharType> string) noexcept
    -> ::fast_io::basic_io_scatter_t<CharType> {
    return {string.data(), string.size()};
}

template<::pltxt2htm::details::is_char_type CharType>
BasicStringView(CharType const*, ::std::size_t) -> BasicStringView<CharType>;

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t size_with_null>
BasicStringView(CharType const (&)[size_with_null]) -> BasicStringView<CharType>;

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t size>
BasicStringView(::pltxt2htm::details::BasicLiteralString<CharType, size> const&) -> BasicStringView<CharType>;

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t extent>
BasicStringView(::pltxt2htm::details::BasicInplaceString<CharType, extent> const&) -> BasicStringView<CharType>;

using StringView = BasicStringView<char>;
using WStringView = BasicStringView<wchar_t>;
using U8StringView = BasicStringView<char8_t>;
using U16StringView = BasicStringView<char16_t>;
using U32StringView = BasicStringView<char32_t>;

} // namespace pltxt2htm::container

#pragma pop_macro("max")
#pragma pop_macro("min")
