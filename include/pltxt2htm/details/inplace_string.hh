/**
 * @file inplace_string.hh
 * @brief Fixed-capacity strings whose characters are stored inside the object.
 */

#pragma once

#include <algorithm>
#include <compare>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>

#include "../contracts.hh"
#include "concepts.hh"
#include "push_macro.hh"

namespace pltxt2htm::details {

/**
 * @brief A variable-length string with fixed compile-time capacity and inline storage.
 * @tparam CharType Character code-unit type.
 * @tparam extent Nonzero maximum number of code units. No terminator is reserved.
 * @tparam ndebug Contract policy used by checked operations.
 *
 * BasicInplaceString never allocates and does not reserve space for a null terminator.
 * Embedded null code units are ordinary elements.
 */
template<::pltxt2htm::details::is_char_type CharType, ::std::size_t extent, ::pltxt2htm::Contracts ndebug>
    requires (extent != 0)
class BasicInplaceString {
    static_assert(extent <= ::std::numeric_limits<::std::size_t>::max() / sizeof(CharType),
                  "inplace string capacity is too large");

    using StoredSize = ::std::conditional_t<
        extent <= ::std::numeric_limits<unsigned char>::max(), unsigned char,
        ::std::conditional_t<
            extent <= ::std::numeric_limits<unsigned short>::max(), unsigned short,
            ::std::conditional_t<extent <= ::std::numeric_limits<unsigned>::max(), unsigned, ::std::size_t>>>;

public:
    using value_type = CharType;
    using size_type = ::std::size_t;
    using difference_type = ::std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = ::std::reverse_iterator<iterator>;
    using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;

private:
    value_type storage[extent]{};
    StoredSize size_storage{};

    static constexpr void check_new_size(size_type new_size) noexcept {
        pltxt2htm_assert(new_size <= extent, u8"BasicInplaceString capacity exceeded");
    }

    constexpr void check_additional_size(this BasicInplaceString const& self, size_type additional_size) noexcept {
        pltxt2htm_assert(additional_size <= extent - self.size(), u8"BasicInplaceString capacity exceeded");
    }

    constexpr void set_size(this BasicInplaceString& self, size_type new_size) noexcept {
        self.size_storage = static_cast<StoredSize>(new_size);
    }

    template<typename InputIterator, typename Sentinel>
    [[nodiscard]]
    static consteval auto is_nothrow_range_iteration() noexcept -> bool {
        constexpr bool is_nothrow_iteration =
            noexcept(static_cast<bool>(::std::declval<InputIterator&>() != ::std::declval<Sentinel&>())) &&
            noexcept(++::std::declval<InputIterator&>()) &&
            noexcept(static_cast<value_type>(*::std::declval<InputIterator&>()));
        if constexpr (::std::sized_sentinel_for<Sentinel, InputIterator>) {
            return is_nothrow_iteration &&
                   noexcept(static_cast<size_type>(::std::declval<Sentinel&>() - ::std::declval<InputIterator&>()));
        }
        else {
            return is_nothrow_iteration;
        }
    }

public:
    constexpr BasicInplaceString() noexcept = default;

    constexpr explicit BasicInplaceString(value_type value) noexcept {
        this->push_back(value);
    }

    constexpr BasicInplaceString(size_type count, value_type value) noexcept {
        this->assign(count, value);
    }

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
        requires (::std::same_as<::std::iter_value_t<InputIterator>, value_type> &&
                  ::std::constructible_from<value_type, ::std::iter_reference_t<InputIterator>>)
    constexpr BasicInplaceString(InputIterator first,
                                 Sentinel last) noexcept(::std::is_nothrow_move_constructible_v<InputIterator> &&
                                                         ::std::is_nothrow_move_constructible_v<Sentinel> &&
                                                         is_nothrow_range_iteration<InputIterator, Sentinel>()) {
        this->append(::std::move(first), ::std::move(last));
    }

    constexpr BasicInplaceString(BasicInplaceString const&) = default;

    constexpr BasicInplaceString(BasicInplaceString&&) noexcept = default;

    constexpr auto operator=(this BasicInplaceString&, BasicInplaceString const&) -> BasicInplaceString& = default;

    constexpr auto operator=(this BasicInplaceString&, BasicInplaceString&&) noexcept -> BasicInplaceString& = default;

    constexpr ~BasicInplaceString() noexcept = default;

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
        requires (::std::same_as<::std::iter_value_t<InputIterator>, value_type> &&
                  ::std::constructible_from<value_type, ::std::iter_reference_t<InputIterator>>)
    constexpr void assign(this BasicInplaceString& self, InputIterator first,
                          Sentinel last) noexcept(::std::is_nothrow_move_constructible_v<InputIterator> &&
                                                  ::std::is_nothrow_move_constructible_v<Sentinel> &&
                                                  is_nothrow_range_iteration<InputIterator, Sentinel>()) {
        if constexpr (::std::sized_sentinel_for<Sentinel, InputIterator>) {
            auto const count = static_cast<size_type>(last - first);
            self.check_new_size(count);
        }
        self.clear();
        self.append(::std::move(first), ::std::move(last));
    }

    constexpr void assign(this BasicInplaceString& self, size_type count, value_type value) noexcept {
        self.check_new_size(count);
        self.clear();
        while (self.size() != count) {
            self.push_back(value);
        }
    }

    template<::std::input_iterator InputIterator, ::std::sentinel_for<InputIterator> Sentinel>
        requires (::std::same_as<::std::iter_value_t<InputIterator>, value_type> &&
                  ::std::constructible_from<value_type, ::std::iter_reference_t<InputIterator>>)
    constexpr void append(this BasicInplaceString& self, InputIterator first,
                          Sentinel last) noexcept(is_nothrow_range_iteration<InputIterator, Sentinel>()) {
        if constexpr (::std::sized_sentinel_for<Sentinel, InputIterator>) {
            auto const count = static_cast<size_type>(last - first);
            self.check_additional_size(count);
        }
        while (first != last) {
            self.push_back(static_cast<value_type>(*first));
            ++first;
        }
    }

    constexpr void append(this BasicInplaceString& self, size_type count, value_type value) noexcept {
        self.check_additional_size(count);
        auto const new_size = self.size() + count;
        while (self.size() != new_size) {
            self.push_back(value);
        }
    }

    [[nodiscard]]
    constexpr auto begin(this BasicInplaceString& self) noexcept -> iterator {
        return self.data();
    }

    [[nodiscard]]
    constexpr auto begin(this BasicInplaceString const& self) noexcept -> const_iterator {
        return self.data();
    }

    [[nodiscard]]
    constexpr auto cbegin(this BasicInplaceString const& self) noexcept -> const_iterator {
        return self.data();
    }

    [[nodiscard]]
    constexpr auto end(this BasicInplaceString& self) noexcept -> iterator {
        return self.data() + self.size();
    }

    [[nodiscard]]
    constexpr auto end(this BasicInplaceString const& self) noexcept -> const_iterator {
        return self.data() + self.size();
    }

    [[nodiscard]]
    constexpr auto cend(this BasicInplaceString const& self) noexcept -> const_iterator {
        return self.end();
    }

    [[nodiscard]]
    constexpr auto rbegin(this BasicInplaceString& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto rbegin(this BasicInplaceString const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto crbegin(this BasicInplaceString const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.end()};
    }

    [[nodiscard]]
    constexpr auto rend(this BasicInplaceString& self) noexcept -> reverse_iterator {
        return reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto rend(this BasicInplaceString const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto crend(this BasicInplaceString const& self) noexcept -> const_reverse_iterator {
        return const_reverse_iterator{self.begin()};
    }

    [[nodiscard]]
    constexpr auto empty(this BasicInplaceString const& self) noexcept -> bool {
        return self.size() == 0;
    }

    [[nodiscard]]
    constexpr auto size(this BasicInplaceString const& self) noexcept -> size_type {
        return self.size_storage;
    }

    [[nodiscard]]
    constexpr auto size_bytes(this BasicInplaceString const& self) noexcept -> size_type {
        return self.size() * sizeof(value_type);
    }

    [[nodiscard]]
    static constexpr auto max_size() noexcept -> size_type {
        return extent;
    }

    [[nodiscard]]
    static constexpr auto max_size_bytes() noexcept -> size_type {
        return extent * sizeof(value_type);
    }

    [[nodiscard]]
    static constexpr auto capacity() noexcept -> size_type {
        return extent;
    }

    [[nodiscard]]
    constexpr auto data(this BasicInplaceString& self) noexcept -> pointer {
        return self.storage;
    }

    [[nodiscard]]
    constexpr auto data(this BasicInplaceString const& self) noexcept -> const_pointer {
        return self.storage;
    }

    [[nodiscard]]
    constexpr auto index(this BasicInplaceString& self, size_type position) noexcept -> reference {
        pltxt2htm_assert(position < self.size(), u8"Index of BasicInplaceString out of bound");
        return self.data()[position];
    }

    [[nodiscard]]
    constexpr auto index(this BasicInplaceString const& self, size_type position) noexcept -> const_reference {
        pltxt2htm_assert(position < self.size(), u8"Index of BasicInplaceString out of bound");
        return self.data()[position];
    }

    constexpr auto operator[](this BasicInplaceString&, size_type) noexcept -> reference = delete
#if __cpp_deleted_function >= 202403L
    #if defined __clang__
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wc++26-extensions"
    #endif
        ("operator[] is deleted; use index() instead for bounds-checked access")
    #if defined __clang__
        #pragma clang diagnostic pop
    #endif
#endif
        ;

    constexpr auto operator[](this BasicInplaceString const&, size_type) noexcept -> const_reference = delete
#if __cpp_deleted_function >= 202403L
    #if defined __clang__
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wc++26-extensions"
    #endif
        ("operator[] is deleted; use index() instead for bounds-checked access")
    #if defined __clang__
        #pragma clang diagnostic pop
    #endif
#endif
        ;

    [[nodiscard]]
    constexpr auto front(this BasicInplaceString& self) noexcept -> reference {
        pltxt2htm_assert(!self.empty(), u8"front() called on an empty BasicInplaceString");
        return self.index(0);
    }

    [[nodiscard]]
    constexpr auto front(this BasicInplaceString const& self) noexcept -> const_reference {
        pltxt2htm_assert(!self.empty(), u8"front() called on an empty BasicInplaceString");
        return self.index(0);
    }

    [[nodiscard]]
    constexpr auto back(this BasicInplaceString& self) noexcept -> reference {
        pltxt2htm_assert(!self.empty(), u8"back() called on an empty BasicInplaceString");
        return self.index(self.size() - 1);
    }

    [[nodiscard]]
    constexpr auto back(this BasicInplaceString const& self) noexcept -> const_reference {
        pltxt2htm_assert(!self.empty(), u8"back() called on an empty BasicInplaceString");
        return self.index(self.size() - 1);
    }

    /**
     * @return true when the character was appended; false when the string was full.
     */
    [[nodiscard]]
    constexpr auto try_push_back(this BasicInplaceString& self, value_type value) noexcept -> bool {
        if (self.size() == extent) {
            return false;
        }
        self.storage[self.size()] = value;
        self.set_size(self.size() + 1);
        return true;
    }

    constexpr auto push_back(this BasicInplaceString& self, value_type value) noexcept -> reference {
        self.check_additional_size(1);
        auto const old_size = self.size();
        self.storage[old_size] = value;
        self.set_size(old_size + 1);
        return self.storage[old_size];
    }

    constexpr void pop_back(this BasicInplaceString& self) noexcept {
        pltxt2htm_assert(!self.empty(), u8"pop_back() called on an empty BasicInplaceString");
        self.set_size(self.size() - 1);
    }

    constexpr void clear(this BasicInplaceString& self) noexcept {
        self.set_size(0);
    }

    constexpr void resize(this BasicInplaceString& self, size_type new_size, value_type value = value_type{}) noexcept {
        self.check_new_size(new_size);
        if (new_size <= self.size()) {
            self.set_size(new_size);
            return;
        }
        self.append(new_size - self.size(), value);
    }
};

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t left_extent, ::pltxt2htm::Contracts left_ndebug,
         ::std::size_t right_extent, ::pltxt2htm::Contracts right_ndebug>
[[nodiscard]]
constexpr auto operator==(BasicInplaceString<CharType, left_extent, left_ndebug> const& left,
                          BasicInplaceString<CharType, right_extent, right_ndebug> const& right) noexcept -> bool {
    return left.size() == right.size() && ::std::equal(left.begin(), left.end(), right.begin());
}

template<::pltxt2htm::details::is_char_type CharType, ::std::size_t left_extent, ::pltxt2htm::Contracts left_ndebug,
         ::std::size_t right_extent, ::pltxt2htm::Contracts right_ndebug>
[[nodiscard]]
constexpr auto operator<=>(BasicInplaceString<CharType, left_extent, left_ndebug> const& left,
                           BasicInplaceString<CharType, right_extent, right_ndebug> const& right) noexcept {
    return ::std::lexicographical_compare_three_way(left.begin(), left.end(), right.begin(), right.end(),
                                                    ::std::compare_three_way{});
}

template<::std::size_t extent, ::pltxt2htm::Contracts ndebug>
using InplaceString = BasicInplaceString<char, extent, ndebug>;

template<::std::size_t extent, ::pltxt2htm::Contracts ndebug>
using WInplaceString = BasicInplaceString<wchar_t, extent, ndebug>;

template<::std::size_t extent, ::pltxt2htm::Contracts ndebug>
using U8InplaceString = BasicInplaceString<char8_t, extent, ndebug>;

template<::std::size_t extent, ::pltxt2htm::Contracts ndebug>
using U16InplaceString = BasicInplaceString<char16_t, extent, ndebug>;

template<::std::size_t extent, ::pltxt2htm::Contracts ndebug>
using U32InplaceString = BasicInplaceString<char32_t, extent, ndebug>;

} // namespace pltxt2htm::details

#include "pop_macro.hh"
