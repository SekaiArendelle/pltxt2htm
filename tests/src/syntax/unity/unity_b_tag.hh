#pragma once

#include "doctest_config.hh"

TEST_CASE("unity_b_tag") {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b>text</b>");
        auto const& answer = u8"<strong>text</strong>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<B    >text</b  >");
        auto const& answer = u8"<strong>text</strong>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b><color=red>text</color></B>");
        auto const& answer = u8"<strong><span style=\"color:red;\">text</span></strong>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b><color=red>text</b></color>");
        auto const& answer = u8"<strong><span style=\"color:red;\">text&lt;/b&gt;</span></strong>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b>text<b>text</b></b>");
        auto const& answer = u8"<strong>texttext</strong>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b>");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    // Optimization example: empty tag
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<b></b>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong>text<b>text</b></strong>");
        auto const& answer = u8"<strong>texttext</strong>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b></b");
        auto const& answer = u8"<strong>&lt;/b</strong>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<b>test</b>cd");
        auto const& answer = u8"ab<b>test</b>cd";
        CHECK(html == answer);
    }
}
