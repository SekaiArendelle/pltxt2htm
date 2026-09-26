#pragma once

#include "doctest_config.hh"

TEST_SUITE("escape") {
    TEST_CASE("< >&/'/'") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"< >&\'\"");
        auto const& answer = u8"&lt;&nbsp;&gt;&amp;&apos;&quot;";
        CHECK(html == answer);
    }

    TEST_CASE("case 2") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    TEST_CASE("< >&/'/' (3)") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"< >&\'\"");
        auto const& answer = u8"&lt;&nbsp;&gt;&amp;&apos;&quot;";
        CHECK(html == answer);
    }
}
