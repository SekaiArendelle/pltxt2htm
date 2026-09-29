#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_strong_tag") {
    TEST_CASE("plain-strong-preserved") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong>text</strong>");
        auto const& answer = u8"<strong>text</strong>";
        CHECK(html == answer);
    }

    TEST_CASE("case-insensitive-tags") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Strong    >text</sTrong  >");
        auto const& answer = u8"<strong>text</strong>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-color-tag") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong><color=red>text</color></strong>");
        auto const& answer = u8"<strong><span style=\"color:red;\">text</span></strong>";
        CHECK(html == answer);
    }

    TEST_CASE("reversed-close-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong><color=red>text</strong></color>");
        auto const& answer = u8"<strong><span style=\"color:red;\">text&lt;/strong&gt;</span></strong>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-open-merged") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong>text<strong>text</strong></strong>");
        auto const& answer = u8"<strong>texttext</strong>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-tag-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text<strong>");
        auto const& answer = u8"text";
        CHECK(html == answer);
    }

    TEST_CASE("empty-strong-removed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<strong></strong>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("unterminated-close-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong></strong");
        auto const& answer = u8"<strong>&lt;/strong</strong>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-strong-to-b") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<strong>test</strong>cd");
        auto const& answer = u8"ab<b>test</b>cd";
        CHECK(html == answer);
    }
}
