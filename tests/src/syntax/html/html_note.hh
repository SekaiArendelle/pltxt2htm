#pragma once

#include "doctest_config.hh"

TEST_CASE("html_note") {
    {
        auto const& pltext = u8"t<!--es-->t";
        auto html = ::pltxt2htm_test::pltxt2common_htmld(pltext);
        auto const& answer = u8"tt";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"tt";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto const& pltext = u8"t<!--es-->t";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<!-- est");
        auto const& answer = u8"t";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<!");
        auto const& answer = u8"&lt;!";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<!-- note -->");
        auto const& answer = u8"";
        CHECK(html == answer);
    }
}
