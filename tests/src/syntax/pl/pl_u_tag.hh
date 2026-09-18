#pragma once

#include "doctest_config.hh"

TEST_CASE("pl_u_tag") {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>text</u>");
        auto answer = ::fast_io::u8string_view{u8"<u>text</u>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<U    >text</U  >");
        auto answer = ::fast_io::u8string_view{u8"<u>text</u>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u><color=red>text</color></u>");
        auto answer = ::fast_io::u8string_view{u8"<u><span style=\"color:red;\">text</span></u>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u><color=red>text</u></color>");
        auto answer = ::fast_io::u8string_view{u8"<u><span style=\"color:red;\">text&lt;/u&gt;</span></u>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>text<u>text</u></u>");
        auto answer = ::fast_io::u8string_view{u8"<u>texttext</u>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<b><u>text</u></b>");
        auto answer = ::fast_io::u8string_view{u8"<strong><u>text</u></strong>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>");
        auto answer = ::fast_io::u8string_view{u8""};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u");
        auto answer = ::fast_io::u8string_view{u8"&lt;u"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<u></u>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        CHECK(html == answer);
    }

    {
        // unclosed <u> tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<u>text");
        auto answer = ::fast_io::u8string_view{u8"<u>text</u>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<u>test</u>cd");
        auto answer = ::fast_io::u8string_view{u8"ab<u>test</u>cd"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<u>t1<u>t2</u></u>");
        auto answer = ::fast_io::u8string_view{u8"<u>t1t2</u>"};
        CHECK(html == answer);
    }
}

