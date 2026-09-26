#pragma once

#include <pltxt2htm/container/array.hh>
#include "doctest_config.hh"

TEST_SUITE("invalid_utf8_char") {
    TEST_CASE("Hello") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"Hello");
        auto const& answer = u8"Hello";
        CHECK(html == answer);
    }

    TEST_CASE("café") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"café");
        auto const& answer = u8"café";
        CHECK(html == answer);
    }

    TEST_CASE("中文") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"中文");
        auto const& answer = u8"中文";
        CHECK(html == answer);
    }

    TEST_CASE("😊😊") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"😊😊");
        auto const& answer = u8"😊😊";
        CHECK(html == answer);
    }

    TEST_CASE("/uFFFD") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xc3)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"\uFFFD";
        CHECK(html == answer);
    }

    TEST_CASE("case 6") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xc3), char8_t(0xa)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("case 7") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xe0), char8_t(0x80), char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("case 8") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xc0), char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("case 9") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xe2), char8_t(0x82)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("case 10") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf4), char8_t(0x90), char8_t(0x80), char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }
    TEST_CASE("invalid 4-byte sequence: 3rd byte is not a continuation...") {
        // invalid 4-byte sequence: 3rd byte is not a continuation byte (0x28)
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0), char8_t(0x90), char8_t('('), char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�(�";
        CHECK(html == answer);
    }

    TEST_CASE("case 12") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xed), char8_t(0xa0), char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("case 13") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("case 14") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0x9f)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("A�B") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t('A'), char8_t(0x80), char8_t('B')};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"A�B";
        CHECK(html == answer);
    }

    TEST_CASE("Control chars now produce InvalidUtf8 (U+FFFD) instead o...") {
        // Control chars now produce InvalidUtf8 (U+FFFD) instead of being silently dropped
        constexpr auto data = ::pltxt2htm::container::Array{char8_t('A'), char8_t(0x1f), char8_t('B')};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"A\uFFFDB";
        CHECK(html == answer);
    }

    TEST_CASE("case 17") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xe2)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("�A") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xe2), char8_t('A')};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�A";
        CHECK(html == answer);
    }

    TEST_CASE("�A (19)") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xe2), char8_t(0x82), char8_t('A')};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�A";
        CHECK(html == answer);
    }

    TEST_CASE("case 20") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("�A (21)") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0), char8_t('A')};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�A";
        CHECK(html == answer);
    }

    TEST_CASE("case 22") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0), char8_t(0x90)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("�A (23)") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0), char8_t(0x90), char8_t('A')};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�A";
        CHECK(html == answer);
    }

    TEST_CASE("case 24") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0), char8_t(0x90), char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("�A��") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0), char8_t('A'), char8_t(0x80), char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�A��";
        CHECK(html == answer);
    }

    TEST_CASE("�A (26)") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0), char8_t(0x90), char8_t(0x80), char8_t('A')};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�A";
        CHECK(html == answer);
    }

    TEST_CASE("/uFFFD (27)") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xc3)};
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            ::pltxt2htm::container::U8StringView{data.data(), data.size()});
        auto const& answer = u8"\uFFFD";
        CHECK(html == answer);
    }
}
