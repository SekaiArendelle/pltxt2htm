#pragma once

#include "doctest_config.hh"

TEST_CASE("escape") {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"< >&\'\"");
        auto const& answer = u8"&lt;&nbsp;&gt;&amp;&apos;&quot;";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"< >&\'\"");
        auto const& answer = u8"&lt;&nbsp;&gt;&amp;&apos;&quot;";
        CHECK(html == answer);
    }
}
