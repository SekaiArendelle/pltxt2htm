#pragma once

#include "doctest_config.hh"

TEST_SUITE("escape") {
    TEST_CASE("all-escapes-web") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"< >&\'\"");
        auto const& answer = u8"&lt;&nbsp;&gt;&amp;&apos;&quot;";
        CHECK(html == answer);
    }

    TEST_CASE("empty-input") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    TEST_CASE("all-escapes-common-html") {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"< >&\'\"");
        auto const& answer = u8"&lt;&nbsp;&gt;&amp;&apos;&quot;";
        CHECK(html == answer);
    }
}
