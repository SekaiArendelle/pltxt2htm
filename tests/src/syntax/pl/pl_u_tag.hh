#pragma once

#include "doctest_config.hh"

TEST_SUITE("pl_u_tag") {
    TEST_CASE("<u>text</u>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>text</u>");
        auto const& answer = u8"<u>text</u>";
        CHECK(html == answer);
    }

    TEST_CASE("<U >text</U >") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<U    >text</U  >");
        auto const& answer = u8"<u>text</u>";
        CHECK(html == answer);
    }

    TEST_CASE("<u><color=red>text</color></u>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u><color=red>text</color></u>");
        auto const& answer = u8"<u><span style=\"color:red;\">text</span></u>";
        CHECK(html == answer);
    }

    TEST_CASE("<u><color=red>text</u></color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u><color=red>text</u></color>");
        auto const& answer = u8"<u><span style=\"color:red;\">text&lt;/u&gt;</span></u>";
        CHECK(html == answer);
    }

    TEST_CASE("<u>text<u>text</u></u>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>text<u>text</u></u>");
        auto const& answer = u8"<u>texttext</u>";
        CHECK(html == answer);
    }

    TEST_CASE("<b><u>text</u></b>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b><u>text</u></b>");
        auto const& answer = u8"<strong><u>text</u></strong>";
        CHECK(html == answer);
    }

    TEST_CASE("<u>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    TEST_CASE("<u") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u");
        auto const& answer = u8"&lt;u";
        CHECK(html == answer);
    }

    TEST_CASE("t<u></u>t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<u></u>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed <u> tag") {
        // unclosed <u> tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>text");
        auto const& answer = u8"<u>text</u>";
        CHECK(html == answer);
    }

    TEST_CASE("ab<u>test</u>cd") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<u>test</u>cd");
        auto const& answer = u8"ab<u>test</u>cd";
        CHECK(html == answer);
    }

    TEST_CASE("<u>t1<u>t2</u></u>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<u>t1<u>t2</u></u>");
        auto const& answer = u8"<u>t1t2</u>";
        CHECK(html == answer);
    }
}
