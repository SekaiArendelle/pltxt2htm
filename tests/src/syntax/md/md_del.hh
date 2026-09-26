#pragma once

#include "doctest_config.hh"

TEST_SUITE("md_del") {
    TEST_CASE("~~test~~") {
        auto const& pltext = u8"~~test~~";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<del>test</del>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<s>test</s>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("te~~st~~") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"te~~st~~");
        auto const& answer = u8"te<del>st</del>";
        CHECK(html == answer);
    }

    TEST_CASE("te~~st ~~") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"te~~st\n~~");
        auto const& answer = u8"te~~st<br>~~";
        CHECK(html == answer);
    }

    TEST_CASE("t~~e~~st") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t~~e~~st");
        auto const& answer = u8"t<del>e</del>st";
        CHECK(html == answer);
    }

    TEST_CASE("*t~~es~~t*") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"*t~~es~~t*");
        auto const& answer = u8"<em>t<del>es</del>t</em>";
        CHECK(html == answer);
    }

    TEST_CASE("~~~a~~a") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"~~~a~~a ");
        auto const& answer = u8"<del>~a</del>a";
        CHECK(html == answer);
    }
}
