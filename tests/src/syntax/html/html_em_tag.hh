#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_em_tag") {
    TEST_CASE("plain-em-preserved") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<em>text</em>");
        auto const& answer = u8"<em>text</em>";
        CHECK(html == answer);
    }

    TEST_CASE("case-insensitive-tags") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Em    >text</eM  >");
        auto const& answer = u8"<em>text</em>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-color-tag") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<em><color=red>text</color></em>");
        auto const& answer = u8"<em><span style=\"color:red;\">text</span></em>";
        CHECK(html == answer);
    }

    TEST_CASE("reversed-close-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<em><color=red>text</em></color>");
        auto const& answer = u8"<em><span style=\"color:red;\">text&lt;/em&gt;</span></em>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-open-merged") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<em>text<em>text</em></em>");
        auto const& answer = u8"<em>texttext</em>";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-tag-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text<em>");
        auto const& answer = u8"text";
        CHECK(html == answer);
    }

    TEST_CASE("empty-em-removed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<em></em>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("unterminated-close-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<em></em");
        auto const& answer = u8"<em>&lt;/em</em>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-em-to-i") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<em>test</em>cd");
        auto const& answer = u8"ab<i>test</i>cd";
        CHECK(html == answer);
    }
}
