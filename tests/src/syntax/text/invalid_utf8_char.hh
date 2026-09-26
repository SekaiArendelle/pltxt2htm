#pragma once

#include <pltxt2htm/container/array.hh>
#include "doctest_config.hh"

TEST_SUITE("invalid_utf8_char") {
    TEST_CASE("ascii-passthrough") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"Hello");
        auto const& answer = u8"Hello";
        CHECK(html == answer);
    }

    TEST_CASE("two-byte-char") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"café");
        auto const& answer = u8"café";
        CHECK(html == answer);
    }

    TEST_CASE("cjk-passthrough") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"中文");
        auto const& answer = u8"中文";
        CHECK(html == answer);
    }

    TEST_CASE("emoji-passthrough") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"😊😊");
        auto const& answer = u8"😊😊";
        CHECK(html == answer);
    }

    TEST_CASE("truncated-2-byte") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xc3)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"\uFFFD";
        CHECK(html == answer);
    }

    TEST_CASE("2-byte-with-invalid-continuation") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xc3), char8_t(0xa)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("overlong-3-byte") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xe0), char8_t(0x80), char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("overlong-2-byte") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xc0), char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("truncated-3-byte") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xe2), char8_t(0x82)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("out-of-range-4-byte") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf4), char8_t(0x90), char8_t(0x80), char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }
    TEST_CASE("invalid-continuation-in-4-byte") {
        // invalid 4-byte sequence: 3rd byte is not a continuation byte (0x28)
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0), char8_t(0x90), char8_t('('), char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�(�";
        CHECK(html == answer);
    }

    TEST_CASE("surrogate-encoded") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xed), char8_t(0xa0), char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("lone-continuation-byte") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("lone-continuation-byte-9f") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0x9f)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("invalid-byte-between-ascii") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t('A'), char8_t(0x80), char8_t('B')};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"A�B";
        CHECK(html == answer);
    }

    TEST_CASE("control-char-becomes-replacement") {
        // Control chars now produce InvalidUtf8 (U+FFFD) instead of being silently dropped
        constexpr auto data = ::pltxt2htm::container::Array{char8_t('A'), char8_t(0x1f), char8_t('B')};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"A\uFFFDB";
        CHECK(html == answer);
    }

    TEST_CASE("lone-3-byte-lead") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xe2)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("3-byte-lead-then-ascii") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xe2), char8_t('A')};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�A";
        CHECK(html == answer);
    }

    TEST_CASE("3-byte-lead-two-bytes-then-ascii") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xe2), char8_t(0x82), char8_t('A')};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�A";
        CHECK(html == answer);
    }

    TEST_CASE("lone-4-byte-lead") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("4-byte-lead-then-ascii") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0), char8_t('A')};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�A";
        CHECK(html == answer);
    }

    TEST_CASE("truncated-4-byte") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0), char8_t(0x90)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("4-byte-lead-two-bytes-then-ascii") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0), char8_t(0x90), char8_t('A')};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�A";
        CHECK(html == answer);
    }

    TEST_CASE("truncated-4-byte-three-bytes") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0), char8_t(0x90), char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�";
        CHECK(html == answer);
    }

    TEST_CASE("4-byte-lead-then-ascii-then-two") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0), char8_t('A'), char8_t(0x80), char8_t(0x80)};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�A��";
        CHECK(html == answer);
    }

    TEST_CASE("4-byte-three-bytes-then-ascii") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xf0), char8_t(0x90), char8_t(0x80), char8_t('A')};
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(::pltxt2htm::container::U8StringView(data.data(), data.size()));
        auto const& answer = u8"�A";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-truncated-2-byte") {
        constexpr auto data = ::pltxt2htm::container::Array{char8_t(0xc3)};
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(
            ::pltxt2htm::container::U8StringView{data.data(), data.size()});
        auto const& answer = u8"\uFFFD";
        CHECK(html == answer);
    }
}
