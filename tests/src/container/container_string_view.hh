#pragma once

#include <concepts>
#include <type_traits>

#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>

#include <pltxt2htm/container/string_view.hh>
#include <pltxt2htm/container/string.hh>
#include <pltxt2htm/details/inplace_string.hh>
#include <pltxt2htm/details/literal_string.hh>

#include "doctest_config.hh"
#include "container/details/concepts.hh"

using U8StringView = ::pltxt2htm::container::U8StringView;
using U8InplaceString = ::pltxt2htm::details::U8InplaceString<8>;

template<typename CharType>
concept can_form_basic_string_view = requires { typename ::pltxt2htm::container::BasicStringView<CharType>; };

static_assert(::std::is_trivially_copyable_v<U8StringView>);
static_assert(::std::is_standard_layout_v<U8StringView>);
static_assert(::std::same_as<U8StringView::value_type, char8_t>);
static_assert(::std::same_as<U8StringView::const_iterator, char8_t const*>);
static_assert(!::std::is_constructible_v<U8StringView, ::fast_io::u8string_view>);
static_assert(!::std::is_constructible_v<U8StringView, ::fast_io::u8string const&>);
static_assert(!::std::is_constructible_v<U8StringView, ::fast_io::u8string&&>);
static_assert(!::std::is_convertible_v<U8StringView, ::fast_io::u8string_view>);
static_assert(::std::same_as<decltype(U8StringView::from_c_str(static_cast<char8_t const*>(nullptr))), U8StringView>);
static_assert(!::std::is_constructible_v<U8StringView, ::pltxt2htm::container::U8String&&>);
static_assert(!::std::is_convertible_v<::pltxt2htm::container::U8String&&, U8StringView>);
static_assert(!::std::is_constructible_v<U8StringView, U8InplaceString&&>);
static_assert(!::std::is_constructible_v<U8StringView, U8InplaceString const&&>);
static_assert(::fast_io::alias_printable<::pltxt2htm::container::StringView>);
static_assert(::fast_io::alias_printable<::pltxt2htm::container::WStringView>);
static_assert(::fast_io::alias_printable<U8StringView>);
static_assert(::fast_io::alias_printable<::pltxt2htm::container::U16StringView>);
static_assert(::fast_io::alias_printable<::pltxt2htm::container::U32StringView>);
static_assert(can_form_basic_string_view<char>);
static_assert(can_form_basic_string_view<wchar_t>);
static_assert(can_form_basic_string_view<char16_t>);
static_assert(can_form_basic_string_view<char32_t>);
static_assert(!can_form_basic_string_view<bool>);
static_assert(!can_call_empty<U8StringView>);
static_assert(!can_form_basic_string_view<int>);

consteval auto test_constexpr_string_view() noexcept -> bool {
    U8StringView const empty{};
    if (!empty.is_empty() || empty.size() != 0 || empty.data() != nullptr || empty.begin() != empty.end()) {
        return false;
    }

    U8StringView const text{u8"abcdef"};
    if (text.is_empty() || text.size() != 6 || text[2] != u8'c') {
        return false;
    }

    auto const scatter = print_alias_define(::fast_io::io_alias, text);
    if (scatter.base != text.data() || scatter.len != text.size()) {
        return false;
    }

    auto const tail = text.subview(3);
    auto const middle = text.subview(1, 3);
    auto const enforced_contract = text.subview(2, 2);
    if (tail != u8"def" || middle != u8"bcd" || !text.starts_with(u8"abc") || !text.ends_with(u8"def") ||
        text.ends_with(u8"abcdefg") || enforced_contract[0] != u8'c') {
        return false;
    }

    constexpr auto literal = ::pltxt2htm::details::U8LiteralString{u8"literal"};
    auto const literal_view = ::pltxt2htm::container::BasicStringView{literal};
    static_assert(::std::same_as<::std::remove_cvref_t<decltype(literal_view)>, U8StringView>);
    if (literal_view != u8"literal") {
        return false;
    }

    constexpr char8_t inplace_text[]{u8'i', u8'n', u8'p', u8'l', u8'a', u8'c', u8'e'};
    auto inplace = U8InplaceString{inplace_text, inplace_text + 7};
    auto const inplace_view = ::pltxt2htm::container::BasicStringView{inplace};
    static_assert(::std::same_as<::std::remove_cvref_t<decltype(inplace_view)>, U8StringView>);
    if (inplace_view.data() != inplace.data() || inplace_view != u8"inplace") {
        return false;
    }

    inplace[0] = u8'I';
    return inplace_view == u8"Inplace";
}

static_assert(test_constexpr_string_view());

TEST_SUITE("container_string_view") {
    TEST_CASE("construct-from-u8-string") {
        ::pltxt2htm::container::U8String own_string{u8"pltxt2htm"};
        U8StringView const own_string_view{own_string};
        U8StringView const converted_own_string_view = own_string;
        CHECK(own_string_view == u8"pltxt2htm");
        CHECK(converted_own_string_view == own_string_view);
    }

    TEST_CASE("fast-io-printing") {
        U8StringView const view{u8"view"};
        auto const printed = ::fast_io::u8concat_fast_io(u8"[", view, u8"]");
        CHECK(printed == u8"[view]");
    }

    TEST_CASE("construct-from-pointer-and-c-str") {
        char8_t const string[]{u8"pltxt2htm"};
        U8StringView const explicit_view{string, sizeof(string) / sizeof(char8_t) - 1};
        CHECK(explicit_view == u8"pltxt2htm");

        U8StringView const from_c_str_view = U8StringView::from_c_str(string);
        CHECK(from_c_str_view == u8"pltxt2htm");
    }
}
