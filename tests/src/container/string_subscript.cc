// External-only subscript coverage for BasicString.
//
// Every other test is built as pltxt2htm itself, with PLTXT2HTM_INTERNAL_USE
// defined, where BasicString::operator[] stays deleted. This TU is excluded from
// that definition in tests/CMakeLists.txt, so it stands in for a downstream user
// and proves the external-only overloads (including the const one) are usable.

#include <concepts>
#include <cstddef>

#include <pltxt2htm/container/string.hh>

#include "precompile.hh"

using U8String = ::pltxt2htm::container::U8String;

static_assert(requires(U8String& string, U8String const& const_string, ::std::size_t position) {
    string[position];
    string[position] = u8'a';
    const_string[position];
});

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

static_assert(test_constexpr_string_subscript());

int main() {
    U8String string{u8"abcd"};
    pltxt2htm_test_assert_true(string[0] == u8'a');
    pltxt2htm_test_assert_true(string[3] == u8'd');
    string[0] = u8'z';
    pltxt2htm_test_assert_true(string[0] == u8'z');

    return 0;
}