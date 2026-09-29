#pragma once

#include "doctest_config.hh"

TEST_SUITE("tab") {
    TEST_CASE("tab-only-common-html") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"\t");
        auto const& answer = u8"&nbsp;&nbsp;&nbsp;&nbsp;";
        CHECK(html == answer);
    }

    TEST_CASE("tab-then-text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"\ta");
        auto const& answer = u8"&nbsp;&nbsp;&nbsp;&nbsp;a";
        CHECK(html == answer);
    }
}
