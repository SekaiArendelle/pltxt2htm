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
#include <limits>

#include <fast_io/fast_io_core.h>

#include "../../details/contracts.hh"
#include "../../details/concepts.hh"
#include "../../details/literal_string.hh"
#include "../../details/impl/inplace_string_decl.hh"

namespace pltxt2htm::container {

/**
 * @brief A non-owning view over a contiguous sequence of characters.
 * @tparam CharType Character type stored by the referenced sequence.
 *
 * BasicStringView deliberately implements only the operations currently needed by
 * pltxt2htm. Checked operations take a build-wide contract setting so they use the
 * same panic/assertion policy as the rest of the library.
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

    static constexpr size_type npos{::std::numeric_limits<size_type>::max()};

private:
    const_pointer pointer{};
    size_type length{};

public:
    constexpr BasicStringView() noexcept = default;

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

    /**
     * @brief Indexed element access, read-only, bounds-checked according to the
     * build-wide contract setting.
     * @param position Zero-based element position.
     * @return Read-only reference to the requested element.
     * @pre position is a valid element position.
     */
    [[nodiscard]]
    constexpr auto operator[](this BasicStringView const& self, size_type position) noexcept -> const_reference;

    [[nodiscard]]
    constexpr auto subview(this BasicStringView const& self, size_type position, size_type count = npos) noexcept
        -> BasicStringView;

    [[nodiscard]]
    constexpr auto starts_with(this BasicStringView const& self, BasicStringView prefix) noexcept -> bool {
        return prefix.length <= self.length && ::std::equal(prefix.begin(), prefix.end(), self.begin());
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
