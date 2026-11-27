#pragma once

#include <array>
#include <concepts>
#include <cstddef>
#include <span>
#include <type_traits>

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>

#include <pltxt2htm/container/cstring_view.hh>
#include <pltxt2htm/container/string.hh>
#include <pltxt2htm/details/inplace_string.hh>
#include <pltxt2htm/details/literal_string.hh>

#include "doctest_config.hh"
#include "details/concepts.hh"

using U8CStringView = ::pltxt2htm::container::U8CStringView;
using U8StringView = ::pltxt2htm::container::U8StringView;
using U8String = ::pltxt2htm::container::U8String;
using U8InplaceString = ::pltxt2htm::details::U8InplaceString<8>;

template<typename CharType>
concept can_form_basic_cstring_view = requires { typename ::pltxt2htm::container::BasicCStringView<CharType>; };

// Requires-expressions in a non-dependent context are ill-formed when the body is
// invalid, so the deleted-member check needs a templated entity to become a
// substitution failure.
template<typename ViewType>
concept can_remove_suffix = requires(ViewType& view) { view.remove_suffix(1); };

static_assert(::std::is_trivially_copyable_v<U8CStringView>);
static_assert(::std::is_standard_layout_v<U8CStringView>);
static_assert(::std::same_as<U8CStringView::value_type, char8_t>);
static_assert(::std::same_as<U8CStringView::const_iterator, char8_t const*>);
static_assert(::std::same_as<U8CStringView::iterator, char8_t const*>);
static_assert(::std::same_as<decltype(::std::declval<U8CStringView&>() = ::std::declval<U8CStringView const&>()),
                             U8CStringView&>);
static_assert(::std::is_assignable_v<U8CStringView&, U8CStringView const&>);
static_assert(::std::is_assignable_v<U8CStringView&, U8CStringView&&>);
static_assert(!::std::is_assignable_v<U8CStringView, U8CStringView const&>);
static_assert(!::std::is_constructible_v<U8CStringView, ::std::nullptr_t>);
static_assert(!::std::is_constructible_v<U8CStringView, ::fast_io::u8string_view>);
static_assert(::std::is_constructible_v<U8CStringView, ::fast_io::u8string const&>);
static_assert(!::std::is_constructible_v<U8CStringView, ::fast_io::u8string&&>);
static_assert(!::std::is_constructible_v<U8CStringView, ::std::u8string_view>);
static_assert(::std::is_constructible_v<U8CStringView, ::std::u8string const&>);
static_assert(!::std::is_constructible_v<U8CStringView, ::std::u8string&&>);
static_assert(!::std::is_constructible_v<U8CStringView, U8String&&>);
static_assert(!::std::is_convertible_v<U8String&&, U8CStringView>);
static_assert(!::std::is_constructible_v<U8CStringView, U8String const&&>);
static_assert(!::std::is_constructible_v<U8CStringView, U8InplaceString const&>);
static_assert(!::std::is_constructible_v<U8CStringView, U8InplaceString&>);
static_assert(!::std::is_constructible_v<U8CStringView, U8InplaceString&&>);
static_assert(!::std::is_constructible_v<U8CStringView, U8InplaceString const&&>);
static_assert(!::std::is_constructible_v<U8CStringView, ::pltxt2htm::details::U8LiteralString<8> const&>);
static_assert(::std::is_constructible_v<U8StringView, U8CStringView const&>);
static_assert(::std::is_convertible_v<U8CStringView const&, U8StringView>);
static_assert(::fast_io::alias_printable<::pltxt2htm::container::CStringView>);
static_assert(::fast_io::alias_printable<::pltxt2htm::container::WCStringView>);
static_assert(::fast_io::alias_printable<U8CStringView>);
static_assert(::fast_io::alias_printable<::pltxt2htm::container::U16CStringView>);
static_assert(::fast_io::alias_printable<::pltxt2htm::container::U32CStringView>);
static_assert(can_form_basic_cstring_view<char>);
static_assert(can_form_basic_cstring_view<wchar_t>);
static_assert(can_form_basic_cstring_view<char16_t>);
static_assert(can_form_basic_cstring_view<char32_t>);
static_assert(!can_form_basic_cstring_view<bool>);
static_assert(!can_form_basic_cstring_view<int>);
static_assert(!can_call_empty<U8CStringView>);
static_assert(!can_remove_suffix<U8CStringView>);

TEST_SUITE("container_cstring_view") {
    consteval auto test_constexpr_cstring_view() noexcept -> bool {
        U8CStringView const empty{};
        if (!empty.is_empty() || empty.size() != 0 || empty.data() == nullptr || *empty.data() != u8'\0' ||
            empty.begin() != empty.end() || empty.c_str() != empty.data()) {
            return false;
        }

        U8CStringView const text{u8"abcdef"};
        if (text.is_empty() || text.size() != 6 || text[2] != u8'c' || text[6] != u8'\0' || text.front() != u8'a' ||
            text.back() != u8'f' || *text.end() != u8'\0') {
            return false;
        }

        if (text.has_nulls() || text.first_piece() != text) {
            return false;
        }

        U8CStringView const embedded{u8"a\0bc"};
        if (!embedded.has_nulls() || embedded.size() != 4 || embedded.first_piece().size() != 1 ||
            embedded.first_piece()[0] != u8'a') {
            return false;
        }

        auto const tail = text.subview(3);
        auto const middle = text.subview(1, 3);
        if (!::std::same_as<decltype(tail), U8CStringView const> ||
            !::std::same_as<::std::remove_cvref_t<decltype(middle)>, U8StringView> || tail != u8"def" ||
            middle != u8"bcd") {
            return false;
        }

        auto const empty_tail = text.subview(text.size());
        if (!empty_tail.is_empty() || empty_tail.data() != text.data() + text.size() || *empty_tail.data() != u8'\0') {
            return false;
        }

        U8CStringView removed_prefix{text};
        removed_prefix.remove_prefix(4);
        if (removed_prefix != u8"ef" || removed_prefix.size() != 2 || *removed_prefix.data() != u8'e') {
            return false;
        }

        U8CStringView removed_all_prefix{text};
        removed_all_prefix.remove_prefix(removed_all_prefix.size());
        if (!removed_all_prefix.is_empty() || *removed_all_prefix.data() != u8'\0' ||
            removed_all_prefix.c_str() != text.data() + text.size()) {
            return false;
        }

        if (!text.starts_with(u8"abc") || !text.starts_with(u8'a') || !text.starts_with(U8StringView{u8"abc"}) ||
            text.starts_with(u8"abcdefg") || !text.ends_with(u8"def") || !text.ends_with(u8'f') ||
            text.ends_with(u8"abcdefg")) {
            return false;
        }

        if (!text.contains(u8"bcd") || !text.contains(u8'c') || text.contains(u8"xyz")) {
            return false;
        }

        if (text.find(u8"cd") != 2 || text.find(u8'c') != 2 || text.find(u8"cd", 3) != U8CStringView::npos ||
            text.find(u8"") != 0 || text.find(u8"abcdefg") != U8CStringView::npos ||
            text.find(u8"a", text.size() + 1) != U8CStringView::npos ||
            text.find(u8'a', text.size() + 1) != U8CStringView::npos) {
            return false;
        }

        if (text.rfind(u8"cd") != 2 || text.rfind(u8'c') != 2 || text.rfind(u8"c", 1) != U8CStringView::npos ||
            text.rfind(u8"") != 6 || text.rfind(u8"abcdefg") != U8CStringView::npos ||
            text.rfind(u8"abc", text.size() + 1) != 0) {
            return false;
        }

        if (text.find_first_of(u8"fd") != 3 || text.find_first_of(u8'd') != 3 ||
            text.find_first_of(u8"xyz") != U8CStringView::npos) {
            return false;
        }

        if (text.find_last_of(u8"fd") != 5 || text.find_last_of(u8'd') != 3 ||
            text.find_last_of(u8"xyz") != U8CStringView::npos) {
            return false;
        }

        if (text.find_first_not_of(u8"abc") != 3 || text.find_first_not_of(u8'a') != 1 ||
            text.find_first_not_of(u8"abcdef") != U8CStringView::npos) {
            return false;
        }

        if (text.find_last_not_of(u8"ef") != 3 || text.find_last_not_of(u8'f') != 4 ||
            text.find_last_not_of(u8"abcdef") != U8CStringView::npos) {
            return false;
        }

        char8_t buffer[4]{};
        if (text.copy(buffer, 3, 2) != 3 || buffer[0] != u8'c' || buffer[2] != u8'e' || text.copy(buffer, 10, 4) != 2 ||
            buffer[0] != u8'e' || buffer[1] != u8'f') {
            return false;
        }

        U8CStringView swapped_left{u8"left"};
        U8CStringView swapped_right{u8"right"};
        swapped_left.swap(swapped_right);
        if (swapped_left != u8"right" || swapped_right != u8"left") {
            return false;
        }

        U8CStringView const same_content{u8"abcdef"};
        if (!(text == same_content) || text == embedded || same_content != text) {
            return false;
        }

        if (text.max_size() < text.size()) {
            return false;
        }

        constexpr char8_t const reversed_expected[]{u8'f', u8'e', u8'd', u8'c', u8'b', u8'a'};
        for (::std::size_t index{}; index < 6; ++index) {
            if (text.rbegin()[static_cast<::std::ptrdiff_t>(index)] != reversed_expected[index]) {
                return false;
            }
        }

        U8StringView const converted{text};
        if (converted != u8"abcdef" || converted.size() != text.size() || converted.data() != text.data()) {
            return false;
        }

        U8CStringView const from_array_ctor{u8"abcdef"};
        if (from_array_ctor != text) {
            return false;
        }

        ::std::array<char8_t, 7> const array_text{u8'a', u8'b', u8'c', u8'd', u8'e', u8'f', u8'\0'};
        U8CStringView const from_array{array_text.data(), array_text.size() - 1};
        if (from_array != text) {
            return false;
        }

        U8CStringView const from_iterators{array_text.begin(), array_text.begin() + 6};
        if (from_iterators != text) {
            return false;
        }

        ::std::span<char8_t const, 6> const span_text{array_text.data(), 6};
        U8CStringView const from_range{span_text};
        if (from_range != text) {
            return false;
        }

        ::std::u8string_view const std_view{array_text.data(), 6};
        U8CStringView const from_std_view{std_view};
        if (from_std_view != text) {
            return false;
        }

        ::std::u8string const std_string{u8"abcdef"};
        U8CStringView const from_std_string{std_string};
        if (from_std_string != text || from_std_string.data() != std_string.data()) {
            return false;
        }

        ::pltxt2htm::container::BasicCStringView const ctad_literal{u8"abcdef"};
        if (!::std::same_as<decltype(ctad_literal), U8CStringView const> || ctad_literal != text) {
            return false;
        }

        ::pltxt2htm::container::BasicCStringView const ctad_from_span{span_text};
        if (!::std::same_as<::std::remove_cvref_t<decltype(ctad_from_span)>, U8CStringView> || ctad_from_span != text) {
            return false;
        }

        U8String const ctad_source{u8"abcdef"};
        ::pltxt2htm::container::BasicCStringView const ctad_from_string{ctad_source};
        if (!::std::same_as<::std::remove_cvref_t<decltype(ctad_from_string)>, U8CStringView> ||
            ctad_from_string != text) {
            return false;
        }

        auto const scatter = print_alias_define(::fast_io::io_alias, text);
        if (scatter.base != text.data() || scatter.len != text.size()) {
            return false;
        }

        return true;
    }

    static_assert(test_constexpr_cstring_view());

    TEST_CASE("construct-from-u8-string") {
        U8String own_string{u8"pltxt2htm"};
        U8CStringView const own_string_view{own_string};
        U8CStringView const converted_own_string_view = own_string;
        CHECK(own_string_view == u8"pltxt2htm");
        CHECK(converted_own_string_view == own_string_view);
        CHECK(own_string_view.size() == own_string.size());
    }

    TEST_CASE("fast-io-printing") {
        U8CStringView const view{u8"view"};
        auto const printed = ::fast_io::u8concat_fast_io(u8"[", view, u8"]");
        CHECK(printed == u8"[view]");
    }

    TEST_CASE("construct-from-pointer") {
        char8_t const string[]{u8"pltxt2htm"};
        U8CStringView const from_pointer{string};
        U8CStringView const explicit_view{string, 9};
        CHECK(from_pointer == u8"pltxt2htm");
        CHECK(explicit_view == from_pointer);
        CHECK(*explicit_view.end() == u8'\0');
    }

    TEST_CASE("subview-and-string-view-conversion") {
        U8CStringView const text{u8"pltxt2htm"};
        auto const whole = text.subview(0);
        CHECK(whole == text);
        auto const suffix = text.subview(5);
        CHECK(suffix == u8"2htm");
        auto const prefix_pair = text.subview(0, 6);
        CHECK(!::std::same_as<::std::remove_cvref_t<decltype(prefix_pair)>, U8CStringView>);
        CHECK(prefix_pair == u8"pltxt2");
        U8StringView const converted = text;
        CHECK(converted == text);
    }
}
