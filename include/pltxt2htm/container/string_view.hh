/**
 * @file string_view.hh
 * @brief Lightweight non-owning string views for pltxt2htm.
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <limits>

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>

#include "../contracts.hh"
#include "../details/concepts.hh"
#include "../details/literal_string.hh"
#include "../details/push_macro.hh"

namespace pltxt2htm::container {

/**
 * @brief A non-owning view over a contiguous sequence of characters.
 * @tparam CharType Character type stored by the referenced sequence.
 *
 * BasicStringView deliberately implements only the operations currently needed by
 * pltxt2htm. Checked operations take a Contracts template argument so they use the
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

    constexpr BasicStringView(::std::nullptr_t) = delete;

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

    constexpr BasicStringView(::fast_io::basic_string_view<value_type> string) noexcept
        : pointer{string.data()},
          length{string.size()} {
    }

    constexpr BasicStringView(::fast_io::manipulators::basic_os_c_str<value_type> string) noexcept
        : pointer{string.ptr},
          length{::fast_io::cstr_len(string.ptr)} {
    }

    constexpr BasicStringView(::fast_io::manipulators::basic_os_c_str_with_known_size<value_type> string) noexcept
        : pointer{string.ptr},
          length{string.n} {
    }

    constexpr BasicStringView(
        ::fast_io::manipulators::basic_os_str_known_size_without_null_terminated<value_type> string) noexcept
        : pointer{string.ptr},
          length{string.n} {
    }

    template<typename Allocator>
    constexpr BasicStringView(::fast_io::containers::basic_string<value_type, Allocator> const& string) noexcept
        : pointer{string.data()},
          length{string.size()} {
    }

    template<typename Allocator>
    constexpr BasicStringView(::fast_io::containers::basic_string<value_type, Allocator>&&) = delete;

    template<typename Allocator>
    constexpr BasicStringView(::fast_io::containers::basic_string<value_type, Allocator> const&&) = delete;

    template<::std::size_t size>
    constexpr BasicStringView(::pltxt2htm::details::BasicLiteralString<value_type, size> const& string) noexcept
        : pointer{string.data()},
          length{string.size()} {
    }

    template<::std::size_t size>
    constexpr BasicStringView(::pltxt2htm::details::BasicLiteralString<value_type, size>&&) = delete;

    template<::std::size_t size>
    constexpr BasicStringView(::pltxt2htm::details::BasicLiteralString<value_type, size> const&&) = delete;

    [[nodiscard]]
    constexpr auto data(this BasicStringView const& self) noexcept -> const_pointer {
        return self.pointer;
    }

    [[nodiscard]]
    constexpr auto size(this BasicStringView const& self) noexcept -> size_type {
        return self.length;
    }

    [[nodiscard]]
    constexpr auto empty(this BasicStringView const& self) noexcept -> bool {
        return self.length == 0;
    }

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

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto index(this BasicStringView const& self, size_type index) noexcept -> const_reference {
        pltxt2htm_assert(index < self.length, u8"Index of BasicStringView out of bound");
        return self.pointer[index];
    }

    constexpr auto operator[](this BasicStringView const& self, size_type index) noexcept -> const_reference = delete
#if __cpp_deleted_function >= 202403L
        ("operator[] is deleted; use index() instead for bounds-checked access")
#endif
        ;

    template<::pltxt2htm::Contracts ndebug>
    [[nodiscard]]
    constexpr auto subview(this BasicStringView const& self, size_type position, size_type count = npos) noexcept
        -> BasicStringView {
        pltxt2htm_assert(position <= self.length, u8"Subview position of BasicStringView out of bound");

        size_type const remaining_size{self.length - position};
        if (count == npos) {
            count = remaining_size;
        }
        else {
            pltxt2htm_assert(count <= remaining_size, u8"Subview size of BasicStringView out of bound");
        }

        const_pointer const subview_data{position == 0 ? self.pointer : self.pointer + position};
        return BasicStringView{subview_data, count};
    }

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
    constexpr operator ::fast_io::basic_string_view<value_type>() const noexcept {
        return ::fast_io::basic_string_view<value_type>{this->pointer, this->length};
    }

    [[nodiscard]]
    constexpr auto operator==(this BasicStringView const& self, BasicStringView right) noexcept -> bool {
        return self.length == right.length && ::std::equal(self.begin(), self.end(), right.begin());
    }
};

template<::pltxt2htm::details::is_char_type CharType>
BasicStringView(CharType const*, ::std::size_t) -> BasicStringView<CharType>;

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t size_with_null>
BasicStringView(CharType const (&)[size_with_null]) -> BasicStringView<CharType>;

template<::pltxt2htm::details::is_char_type CharType>
BasicStringView(::fast_io::basic_string_view<CharType>) -> BasicStringView<CharType>;

template<::pltxt2htm::details::is_char_type CharType, typename Allocator>
BasicStringView(::fast_io::containers::basic_string<CharType, Allocator> const&) -> BasicStringView<CharType>;

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t size>
BasicStringView(::pltxt2htm::details::BasicLiteralString<CharType, size> const&) -> BasicStringView<CharType>;

template<::pltxt2htm::details::is_char_type CharType>
BasicStringView(::fast_io::manipulators::basic_os_c_str<CharType>) -> BasicStringView<CharType>;

template<::pltxt2htm::details::is_char_type CharType>
BasicStringView(::fast_io::manipulators::basic_os_c_str_with_known_size<CharType>) -> BasicStringView<CharType>;

template<::pltxt2htm::details::is_char_type CharType>
BasicStringView(::fast_io::manipulators::basic_os_str_known_size_without_null_terminated<CharType>)
    -> BasicStringView<CharType>;

using StringView = BasicStringView<char>;
using WStringView = BasicStringView<wchar_t>;
using U8StringView = BasicStringView<char8_t>;
using U16StringView = BasicStringView<char16_t>;
using U32StringView = BasicStringView<char32_t>;

} // namespace pltxt2htm::container

#include "../details/pop_macro.hh"
