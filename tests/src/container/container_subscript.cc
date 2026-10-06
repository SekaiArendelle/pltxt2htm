// External-only API coverage for BasicString, BasicStringView, Array, Vector and BasicInplaceString.
//
// Every other test is built as pltxt2htm itself, with PLTXT2HTM_INTERNAL_USE
// defined, where these APIs stay deleted. This TU is excluded
// from that definition in tests/CMakeLists.txt, so it stands in for a downstream
// user and proves the external-only APIs are usable.

#include <concepts>
#include <cstddef>
#include <utility>

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
static_assert(requires(IntVector& vector, IntVector const& const_vector, ::std::size_t position) {
    { vector[position] } noexcept -> ::std::same_as<int&>;
    vector[position] = 1;
    { const_vector[position] } noexcept -> ::std::same_as<int const&>;
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

constexpr auto test_vector_subscript() noexcept -> bool {
    IntVector values{1, 2, 3};
    if (values[0] != 1 || values[2] != 3) {
        return false;
    }
    auto& element = values[1];
    element = 42;
    auto const& const_values = values;
    if (values[1] != 42 || const_values[1] != 42 || const_values[2] != 3) {
        return false;
    }
    IntVector const const_vector{4, 5};
    return const_vector[0] == 4 && const_vector[1] == 5;
}

static_assert(test_vector_subscript());

[[nodiscard]]
constexpr auto test_external_self_assignment() noexcept -> bool {
    U8String string{u8"abc"};
    auto& string_alias = string;
    auto const string_capacity = string.capacity();
    string = string_alias;
    if (string != u8"abc" || string.capacity() != string_capacity) {
        return false;
    }
    string = ::std::move(string_alias);
    if (string != u8"abc" || string.capacity() != string_capacity || string.c_str()[3] != u8'\0') {
        return false;
    }
    U8InplaceString inplace{u8'a'};
    auto& inplace_alias = inplace;
    inplace = inplace_alias;
    inplace = ::std::move(inplace_alias);
    if (inplace.size() != 1 || inplace[0] != u8'a') {
        return false;
    }
    IntVector vector{1, 2, 3};
    auto& vector_alias = vector;
    auto const vector_capacity = vector.capacity();
    vector = vector_alias;
    if (vector != IntVector{1, 2, 3} || vector.capacity() != vector_capacity) {
        return false;
    }
    vector = ::std::move(vector_alias);
    return vector == IntVector{1, 2, 3} && vector.capacity() == vector_capacity;
}

static_assert(test_external_self_assignment());

int main() {
    pltxt2htm_test_assert_true(test_external_self_assignment());
    pltxt2htm_test_assert_true(test_vector_subscript());
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
