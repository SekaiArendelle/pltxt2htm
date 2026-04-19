#include <pltxt2htm/details/literal_string.hh>
#include <pltxt2htm/details/utils.hh>
#include "precompile.hh"

template<::pltxt2htm::U8LiteralString test>
struct X {};

consteval void test_use_in_template() noexcept {
    [[maybe_unused]] auto _ = X<::pltxt2htm::U8LiteralString{u8"test"}>{};
}

consteval void test_init_and_concat() noexcept {
    constexpr auto str1 = ::pltxt2htm::U8LiteralString{u8"test"};
    constexpr auto str2 = ::pltxt2htm::U8LiteralString{u8"test"};
    constexpr auto str3 = ::pltxt2htm::U8LiteralString{u8"text"};

    constexpr auto concat_str = ::pltxt2htm::details::concat(str1, str2, str3);
    constexpr auto answer = ::pltxt2htm::U8LiteralString{u8"testtesttext"};
    static_assert(concat_str == answer);
}

consteval void test_index() noexcept {
    constexpr auto str = ::pltxt2htm::U8LiteralString{u8"test"};
    static_assert(str[1] == u8'e');
}

consteval void test_uint_to_literal_string() noexcept {
    constexpr auto str = ::pltxt2htm::details::uint_to_literal_string<42>();
    constexpr auto answer = ::pltxt2htm::U8LiteralString{u8"42"};
    static_assert(str.size() == 2);
    static_assert(str == answer);
}

consteval void test_for_loop() noexcept {
    constexpr auto str = ::pltxt2htm::U8LiteralString{u8"test"};
    for ([[maybe_unused]] auto&& _ : str) {
    }
}

int main() noexcept {
    {
        constexpr auto str = ::pltxt2htm::U8LiteralString{u8"test"};
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::is_prefix_match<::pltxt2htm::Contracts::quick_enforce, str>(
            ::fast_io::u8string_view{u8"test"}));
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::is_prefix_match<::pltxt2htm::Contracts::quick_enforce, str>(
            ::fast_io::u8string_view{u8"TEST"}));
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::is_prefix_match<::pltxt2htm::Contracts::quick_enforce, str>(
                                          ::fast_io::u8string_view{u8"kksk"}) == false);
    }

    {
        constexpr auto str = ::pltxt2htm::U8LiteralString{u8"te-1"};
        ::pltxt2htm_test::assert_true(
            ::pltxt2htm::details::is_prefix_match<::pltxt2htm::Contracts::quick_enforce, str>(u8"TE-1"));
        ::pltxt2htm_test::assert_true(
            ::pltxt2htm::details::is_prefix_match<::pltxt2htm::Contracts::quick_enforce, str>(u8"TE_1") == false);
    }
    {
        constexpr auto str = ::pltxt2htm::U8LiteralString{u8"a"};
        ::pltxt2htm_test::assert_true(
            ::pltxt2htm::details::is_prefix_match<::pltxt2htm::Contracts::quick_enforce, str>(u8"b") == false);
    }
    {
        constexpr auto str = ::pltxt2htm::U8LiteralString{u8"-"};
        ::pltxt2htm_test::assert_true(
            ::pltxt2htm::details::is_prefix_match<::pltxt2htm::Contracts::quick_enforce, str>(u8"_") == false);
    }
    {
        constexpr auto str = ::pltxt2htm::U8LiteralString{u8"-"};
        ::pltxt2htm_test::assert_true(
            ::pltxt2htm::details::is_prefix_match<::pltxt2htm::Contracts::quick_enforce, str>(u8"-"));
    }

    {
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::size_t2str(0) == ::fast_io::u8string_view{u8"0"});
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::size_t2str(42) == ::fast_io::u8string_view{u8"42"});
    }

    {
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::u8str2size_t(u8"").has_value() == false);
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::u8str2size_t(u8"12x").has_value() == false);
        ::pltxt2htm_test::assert_true(::pltxt2htm::details::u8str2size_t(u8"42").has_value());
    }

    return 0;
}
