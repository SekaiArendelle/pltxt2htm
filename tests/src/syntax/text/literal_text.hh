#pragma once

#include "doctest_config.hh"

TEST_SUITE("literal_text") {
    TEST_CASE("leading-zero-digits-kept") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"0355");
        auto const& answer = u8"0355";
        CHECK(html == answer);
    }

    TEST_CASE("plain-ascii-words") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"hello world");
        auto const& answer = u8"hello&nbsp;world";
        CHECK(html == answer);
    }

    TEST_CASE("alphanumeric-mixed") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"abc123");
        auto const& answer = u8"abc123";
        CHECK(html == answer);
    }

    TEST_CASE("version-like-string") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"v1.2.3");
        auto const& answer = u8"v1.2.3";
        CHECK(html == answer);
    }

    TEST_CASE("identifier-punctuation") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"snake_case-kebab");
        auto const& answer = u8"snake_case-kebab";
        CHECK(html == answer);
    }

    TEST_CASE("percent-literal") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"100%");
        auto const& answer = u8"100%";
        CHECK(html == answer);
    }

    TEST_CASE("cjk-text") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"中文文本");
        auto const& answer = u8"中文文本";
        CHECK(html == answer);
    }
}