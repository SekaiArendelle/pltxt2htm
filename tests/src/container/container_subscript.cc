// External-only API coverage for BasicString, BasicStringView, Array, Vector and BasicInplaceString.
//
// Every other test is built as pltxt2htm itself, with PLTXT2HTM_INTERNAL_USE
// defined, where these APIs stay deleted. This TU is excluded
// from that definition in tests/CMakeLists.txt, so it stands in for a downstream
// user and proves the external-only APIs are usable.

#include <concepts>
#include <cstddef>

#include <pltxt2htm/container/array.hh>
#include <pltxt2htm/container/string.hh>
#include <pltxt2htm/container/string_view.hh>
#include <pltxt2htm/container/vector.hh>
#include <pltxt2htm/contracts.hh>
#include <pltxt2htm/details/inplace_string.hh>

#include "precompile.hh"

using U8String = ::pltxt2htm::container::U8String;
using U8StringView = ::pltxt2htm::container::U8StringView;
using U8Array = ::pltxt2htm::container::Array<char8_t, 4>;
using IntVector = ::pltxt2htm::container::Vector<int>;
using U8InplaceString = ::pltxt2htm::details::U8InplaceString<4, ::pltxt2htm::Contracts::quick_enforce>;

static_assert(requires(U8String& string, U8String const& const_string, ::std::size_t position) {
    string[position];
    string[position] = u8'a';
    const_string[position];
});
static_assert(requires(U8StringView view, ::std::size_t position) { view[position]; });
static_assert(requires(U8Array& array, U8Array const& const_array, ::std::size_t position) {
    array[position];
    array[position] = u8'a';
    const_array[position];
});
static_assert(requires(U8InplaceString& string, U8InplaceString const& const_string, ::std::size_t position) {
    string[position];
    string[position] = u8'a';
    const_string[position];
});
static_assert(requires(U8String const& string, U8StringView view, U8Array const& array, IntVector const& vector) {
    { string.empty() } -> ::std::same_as<bool>;
    { view.empty() } -> ::std::same_as<bool>;
    { vector.empty() } -> ::std::same_as<bool>;
});

consteval auto test_constexpr_empty() noexcept -> bool {
    U8String const empty_string{};
    U8String const string{u8"a"};
    U8StringView const empty_view{};
    U8StringView const view{u8"a"};
    U8Array const array{};
    IntVector const empty_vector{};
    IntVector const vector{1};
    return empty_string.empty() && !string.empty() && empty_view.empty() && !view.empty() && empty_vector.empty() &&
           !vector.empty();
}

consteval auto test_constexpr_string_subscript() noexcept -> bool {
    U8String string{u8"abcd"};
    if (string[0] != u8'a' || string[1] != u8'b' || string[3] != u8'd') {
        return false;
    }
    U8String const const_string{u8"abcd"};
    if (const_string[0] != u8'a' || const_string[3] != u8'd') {
        return false;
    }
    string[0] = u8'z';
    return string[0] == u8'z' && const_string[0] == u8'a';
}

consteval auto test_constexpr_container_subscript() noexcept -> bool {
    U8StringView const view{u8"abcd"};
    if (view[0] != u8'a' || view[3] != u8'd') {
        return false;
    }

    U8Array array{u8'a', u8'b', u8'c', u8'd'};
    if (array[0] != u8'a' || array[3] != u8'd') {
        return false;
    }
    U8Array const const_array{u8'a', u8'b', u8'c', u8'd'};
    if (const_array[0] != u8'a' || const_array[3] != u8'd') {
        return false;
    }
    array[0] = u8'z';
    if (array[0] != u8'z' || const_array[0] != u8'a') {
        return false;
    }

    U8InplaceString inplace{};
    inplace.push_back(u8'a');
    inplace.push_back(u8'b');
    if (inplace[0] != u8'a' || inplace[1] != u8'b') {
        return false;
    }
    inplace[0] = u8'z';
    if (inplace[0] != u8'z' || inplace[1] != u8'b') {
        return false;
    }
    U8InplaceString const const_inplace{u8'b'};
    return const_inplace[0] == u8'b';
}

static_assert(test_constexpr_string_subscript());
static_assert(test_constexpr_container_subscript());
static_assert(test_constexpr_empty());

int main() {
    U8String string{u8"abcd"};
    pltxt2htm_test_assert_true(string[0] == u8'a');
    pltxt2htm_test_assert_true(string[3] == u8'd');
    string[0] = u8'z';
    pltxt2htm_test_assert_true(string[0] == u8'z');

    U8StringView const view{u8"abcd"};
    pltxt2htm_test_assert_true(view[2] == u8'c');

    U8Array array{u8'a', u8'b', u8'c', u8'd'};
    array[1] = u8'y';
    pltxt2htm_test_assert_true(array[1] == u8'y');

    IntVector empty_vector{};
    IntVector vector{1};
    pltxt2htm_test_assert_true(empty_vector.empty());
    pltxt2htm_test_assert_true(!vector.empty());

    return 0;
}
