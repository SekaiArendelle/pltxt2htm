#pragma once

#include "doctest_config.hh"

TEST_CASE("escape") {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"< >&\'\"");
        CHECK(html == u8"&lt;&nbsp;&gt;&amp;&apos;&quot;");
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"");
        CHECK(html == u8"");
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"< >&\'\"");
        CHECK(html == u8"&lt;&nbsp;&gt;&amp;&apos;&quot;");
    }
}
