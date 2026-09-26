#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_note") {
    TEST_CASE("t<!--es-->t") {
        auto const& pltext = u8"t<!--es-->t";
        auto html = ::pltxt2htm_test::pltxt2common_htmld(pltext);
        auto const& answer = u8"tt";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"tt";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("t<!--es-->t (2)") {
        auto const& pltext = u8"t<!--es-->t";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("t<!-- est") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<!-- est");
        auto const& answer = u8"t";
        CHECK(html == answer);
    }

    TEST_CASE("<!") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<!");
        auto const& answer = u8"&lt;!";
        CHECK(html == answer);
    }

    TEST_CASE("<!-- note -->") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<!-- note -->");
        auto const& answer = u8"";
        CHECK(html == answer);
    }
}
