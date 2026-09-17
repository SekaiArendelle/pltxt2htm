#pragma once

#include "doctest_config.hh"

TEST_CASE("md_del") {
    {
        auto pltext = ::fast_io::u8string_view{u8"~~test~~"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<del>test</del>"};
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<s>test</s>"};
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"te~~st~~");
        auto answer = ::fast_io::u8string_view{u8"te<del>st</del>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"te~~st\n~~");
        auto answer = ::fast_io::u8string_view{u8"te~~st<br>~~"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t~~e~~st");
        auto answer = ::fast_io::u8string_view{u8"t<del>e</del>st"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"*t~~es~~t*");
        auto answer = ::fast_io::u8string_view{u8"<em>t<del>es</del>t</em>"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"~~~a~~a ");
        auto answer = ::fast_io::u8string_view{u8"<del>~a</del>a"};
        CHECK(html == answer);
    }
}

