#pragma once

#include "doctest_config.hh"

TEST_CASE("html_strong_tag") {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong>text</strong>");
        auto answer = ::fast_io::u8string_view{u8"<strong>text</strong>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<Strong    >text</sTrong  >");
        auto answer = ::fast_io::u8string_view{u8"<strong>text</strong>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong><color=red>text</color></strong>");
        auto answer = ::fast_io::u8string_view{u8"<strong><span style=\"color:red;\">text</span></strong>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong><color=red>text</strong></color>");
        auto answer =
            ::fast_io::u8string_view{u8"<strong><span style=\"color:red;\">text&lt;/strong&gt;</span></strong>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong>text<strong>text</strong></strong>");
        auto answer = ::fast_io::u8string_view{u8"<strong>texttext</strong>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text<strong>");
        auto answer = ::fast_io::u8string_view{u8"text"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<strong></strong>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<strong></strong");
        auto answer = ::fast_io::u8string_view{u8"<strong>&lt;/strong</strong>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<strong>test</strong>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<b>test</b>cd"};
        CHECK(html == answer);
    }
}

