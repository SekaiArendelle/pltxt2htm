#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_em_tag") {
    TEST_CASE("<em>text</em>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<em>text</em>");
        auto const& answer = u8"<em>text</em>";
        CHECK(html == answer);
    }

    TEST_CASE("<Em >text</eM >") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Em    >text</eM  >");
        auto const& answer = u8"<em>text</em>";
        CHECK(html == answer);
    }

    TEST_CASE("<em><color=red>text</color></em>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<em><color=red>text</color></em>");
        auto const& answer = u8"<em><span style=\"color:red;\">text</span></em>";
        CHECK(html == answer);
    }

    TEST_CASE("<em><color=red>text</em></color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<em><color=red>text</em></color>");
        auto const& answer = u8"<em><span style=\"color:red;\">text&lt;/em&gt;</span></em>";
        CHECK(html == answer);
    }

    TEST_CASE("<em>text<em>text</em></em>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<em>text<em>text</em></em>");
        auto const& answer = u8"<em>texttext</em>";
        CHECK(html == answer);
    }

    TEST_CASE("text<em>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text<em>");
        auto const& answer = u8"text";
        CHECK(html == answer);
    }

    TEST_CASE("t<em></em>t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<em></em>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("<em></em") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<em></em");
        auto const& answer = u8"<em>&lt;/em</em>";
        CHECK(html == answer);
    }

    TEST_CASE("ab<em>test</em>cd") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<em>test</em>cd");
        auto const& answer = u8"ab<i>test</i>cd";
        CHECK(html == answer);
    }
}
