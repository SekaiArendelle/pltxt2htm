#pragma once

#include "doctest_config.hh"

TEST_SUITE("bound") {
    TEST_CASE("unfinished-open-tag") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<");
        auto const& answer = u8"test&lt;";
        CHECK(html == answer);
    }

    TEST_CASE("backslash-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test\\");
        auto const& answer = u8"test\\";
        CHECK(html == answer);
    }

    TEST_CASE("unfinished-close-tag") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test</");
        auto const& answer = u8"test&lt;/";
        CHECK(html == answer);
    }

    TEST_CASE("lone-newline-becomes-br") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\n");
        auto const& answer = u8"<br>";
        CHECK(html == answer);
    }
}
