// External-only subscript coverage for BasicStringView, Array and BasicInplaceString.
//
// Every other test is built as pltxt2htm itself, with PLTXT2HTM_INTERNAL_USE
// defined, where those operator[] overloads stay deleted. This TU is excluded
// from that definition in tests/CMakeLists.txt, so it stands in for a downstream
// user and proves the external-only overloads are usable.

#include <concepts>
#include <cstddef>

#include <pltxt2htm/container/array.hh>
#include <pltxt2htm/container/string_view.hh>
#include <pltxt2htm/contracts.hh>
#include <pltxt2htm/details/inplace_string.hh>

#include "precompile.hh"

using U8StringView = ::pltxt2htm::container::U8StringView;
using U8Array = ::pltxt2htm::container::Array<char8_t, 4>;
using U8InplaceString = ::pltxt2htm::details::U8InplaceString<4, ::pltxt2htm::Contracts::quick_enforce>;

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

static_assert(test_constexpr_container_subscript());

int main() {
    U8StringView const view{u8"abcd"};
    pltxt2htm_test_assert_true(view[2] == u8'c');

    U8Array array{u8'a', u8'b', u8'c', u8'd'};
    array[1] = u8'y';
    pltxt2htm_test_assert_true(array[1] == u8'y');

    return 0;
}