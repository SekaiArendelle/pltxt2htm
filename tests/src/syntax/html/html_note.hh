#pragma once

#include "doctest_config.hh"

TEST_CASE("html_note") {
    {
        auto pltext = ::fast_io::u8string_view{u8"t<!--es-->t"};
        auto html = ::pltxt2htm_test::pltxt2common_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"tt"};
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"tt"};
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"t<!--es-->t"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"tt"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<!-- est");
        auto answer = ::fast_io::u8string_view{u8"t"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<!");
        auto answer = ::fast_io::u8string_view{u8"&lt;!"};
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<!-- note -->");
        auto answer = ::fast_io::u8string_view{u8""};
        CHECK(html == answer);
    }
}

