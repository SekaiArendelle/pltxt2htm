#pragma once

#include "doctest_config.hh"

TEST_CASE("unity_i_tag") {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<i>text</i>");
        auto const& answer = u8"<em>text</em>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<I    >text</I  >");
        auto const& answer = u8"<em>text</em>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<i><color=red>text</color></i>");
        auto const& answer = u8"<em><span style=\"color:red;\">text</span></em>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<i><color=red>text</i></color>");
        auto const& answer = u8"<em><span style=\"color:red;\">text&lt;/i&gt;</span></em>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<i>text<i>text</i></i>");
        auto const& answer = u8"<em>texttext</em>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<i>");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<i");
        auto const& answer = u8"&lt;i";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<i></i>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<em>te<i>st</i></em>");
        auto const& answer = u8"<em>test</em>";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<i>test</i>cd");
        auto const& answer = u8"ab<i>test</i>cd";
        CHECK(html == answer);
    }
}
