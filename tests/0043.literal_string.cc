#include <pltxt2htm/details/literal_string.hh>
#include <pltxt2htm/details/utils.hh>
#include "precompile.hh"

template<::pltxt2htm::details::LiteralString test>
struct X {};

consteval void test_use_in_template() noexcept {
    [[maybe_unused]] auto _ = X<::pltxt2htm::details::LiteralString{u8"test"}>{};
}

consteval void test_init_and_concat() noexcept {
    constexpr auto str1 = ::pltxt2htm::details::LiteralString{u8"test"};
    constexpr auto str2 = ::pltxt2htm::details::LiteralString{"test"};
    constexpr auto str3 = ::pltxt2htm::details::LiteralString{"text"};

    constexpr auto concat_str = ::pltxt2htm::details::concat(str1, str2, str3);
    constexpr auto answer = ::pltxt2htm::details::LiteralString{u8"testtesttext"};
    static_assert(concat_str == answer);
}

consteval void test_index() noexcept {
    constexpr auto str = ::pltxt2htm::details::LiteralString{u8"test"};
    static_assert(str[1] == u8'e');
}

consteval void test_uint_to_literal_string() noexcept {
    constexpr auto str = ::pltxt2htm::details::uint_to_literal_string<42>();
    constexpr auto answer = ::pltxt2htm::details::LiteralString{u8"42"};
    static_assert(str.size() == 2);
    static_assert(str == answer);
}

consteval void test_for_loop() noexcept {
    constexpr auto str = ::pltxt2htm::details::LiteralString{u8"test"};
    for ([[maybe_unused]] auto&& _ : str) {
    }
}

int main() noexcept {
    {
        constexpr auto str = ::pltxt2htm::details::LiteralString{u8"test"};
        ::pltxt2htm_test::assert_true(
            ::pltxt2htm::details::is_prefix_match<false, str>(::fast_io::u8string_view{u8"test"}));
        ::pltxt2htm_test::assert_true(
            ::pltxt2htm::details::is_prefix_match<false, str>(::fast_io::u8string_view{u8"TEST"}));
        ::pltxt2htm_test::assert_true(
            !::pltxt2htm::details::is_prefix_match<false, str>(::fast_io::u8string_view{u8"kksk"}));
    }

    return 0;
}
