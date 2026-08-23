#include <concepts>
#include <type_traits>

#include <fast_io/fast_io.h>

#include <pltxt2htm/container/string.hh>

#include "precompile.hh"

using U8String = ::pltxt2htm::container::U8String;
using U8StringView = ::pltxt2htm::container::U8StringView;
using Contracts = ::pltxt2htm::Contracts;

template<typename String>
concept has_subscript_operator = requires(String& string) { string[0]; };

static_assert(::std::same_as<U8String::value_type, char8_t>);
static_assert(::std::is_constructible_v<U8String, ::fast_io::u8string const&>);
static_assert(::std::is_constructible_v<U8StringView, U8String const&>);
static_assert(!::std::is_constructible_v<U8StringView, U8String&&>);
static_assert(::std::is_convertible_v<U8String const&, U8StringView>);
static_assert(!::std::is_convertible_v<U8String&&, U8StringView>);
static_assert(!has_subscript_operator<U8String>);
static_assert(!has_subscript_operator<U8String const>);

consteval auto test_constexpr_string() noexcept -> bool {
    U8String empty_string;
    empty_string.clear();
    empty_string.assign(U8StringView{});
    empty_string.assign_characters(0);
    U8String moved_empty_string{::std::move(empty_string)};
    if (!empty_string.empty() || empty_string.c_str()[0] != u8'\0' || !moved_empty_string.empty() ||
        moved_empty_string.c_str()[0] != u8'\0') {
        return false;
    }
    U8String assigned_empty_string{u8"not empty"};
    assigned_empty_string = ::std::move(moved_empty_string);
    if (!moved_empty_string.empty() || moved_empty_string.c_str()[0] != u8'\0' || !assigned_empty_string.empty() ||
        assigned_empty_string.c_str()[0] != u8'\0') {
        return false;
    }

    constexpr auto literal = ::pltxt2htm::details::U8LiteralString{u8"literal"};
    U8String const literal_string{literal};
    if (literal_string != u8"literal") {
        return false;
    }

    U8String string{u8"abc"};
    string.push_back(u8'd');
    string.append(u8"ef");
    string.insert<Contracts::quick_enforce>(string.begin(), U8StringView{u8"0"});
    if (string != u8"0abcdef" || string.size() != 7 || string.c_str()[string.size()] != u8'\0') {
        return false;
    }

    U8String copy{string};
    U8String moved{::std::move(copy)};
    if (!copy.empty() || moved != string) {
        return false;
    }

    moved.pop_back<Contracts::quick_enforce>();
    moved.assign(U8StringView{u8"xy"});
    return moved == u8"xy" && moved.front<Contracts::quick_enforce>() == u8'x' &&
           moved.back<Contracts::quick_enforce>() == u8'y';
}

static_assert(test_constexpr_string());

int main() {
    U8String empty_string;
    pltxt2htm_test_assert_true(empty_string.data() == ::fast_io::null_terminated_c_str_v<char8_t>);
    pltxt2htm_test_assert_true(empty_string.capacity() == 0);
    empty_string.clear();
    empty_string.assign(U8StringView{});
    empty_string.assign_characters(0);
    U8String moved_empty_string{::std::move(empty_string)};
    U8String assigned_empty_string{u8"not empty"};
    assigned_empty_string = ::std::move(moved_empty_string);
    pltxt2htm_test_assert_true(empty_string.data() == ::fast_io::null_terminated_c_str_v<char8_t>);
    pltxt2htm_test_assert_true(moved_empty_string.data() == ::fast_io::null_terminated_c_str_v<char8_t>);
    pltxt2htm_test_assert_true(assigned_empty_string.data() == ::fast_io::null_terminated_c_str_v<char8_t>);

    U8String const string{u8"abc"};
    auto const printed = ::fast_io::u8concat_fast_io(string, 123);
    pltxt2htm_test_assert_true(printed == u8"abc123");

    ::fast_io::u8string const fast_io_string{u8"fast_io"};
    U8String const compatible{fast_io_string};
    pltxt2htm_test_assert_true(compatible == u8"fast_io");

    return 0;
}
