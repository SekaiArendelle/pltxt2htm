#pragma once

#include "doctest_config.hh"

TEST_SUITE("fixedadv_parser") {
    TEST_CASE("<a>text</a>") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<a>text</a>");
        CHECK(html == u8"<span style=\"color:#0000AA;\">text</span>");
    }
    TEST_CASE("{project}") {
        auto html =
            ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"{project}", u8"localhost:5173", u8"<img src=x onerror=alert(1)>",
                                                   u8"visitor", u8"author", u8"coauthors");
        CHECK(html == u8"&lt;img src=x onerror=alert(1)&gt;");
    }
    TEST_CASE("{visitor}{author}{coauthors}") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(
            u8"{visitor}{author}{coauthors}", u8"localhost:5173", u8"project", u8"<svg/onload=alert(2)>",
            u8"<script>alert(3)</script>", u8"<iframe src=javascript:alert(4)></iframe>");
        CHECK(html ==
              u8"&lt;svg/onload=alert(2)&gt;&lt;script&gt;alert(3)&lt;/script&gt;&lt;iframe "
              u8"src=javascript:alert(4)&gt;&lt;/iframe&gt;");
    }
    TEST_CASE("{project}{visitor}{author}{coauthors}") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(
            u8"{project}{visitor}{author}{coauthors}", u8"localhost:5173", u8"<svg/onload=alert(1)>",
            u8"<img src=x onerror=alert(2)>", u8"<script>alert(3)</script>", u8"<a href=javascript:alert(4)>x</a>");
        CHECK(
            html ==
            u8"&lt;svg/onload=alert(1)&gt;&lt;img src=x onerror=alert(2)&gt;&lt;script&gt;alert(3)&lt;/script&gt;&lt;a "
            u8"href=javascript:alert(4)&gt;x&lt;/a&gt;");
    }
    TEST_CASE("<experiment=expid>exp</experiment><discussion=discid>dis<...") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(
            u8"<experiment=expid>exp</experiment><discussion=discid>dis</discussion>",
            u8"localhost:5173\" onclick=\"alert(1)<img src=x onerror=alert(2)>", u8"project", u8"visitor", u8"author",
            u8"coauthors");
        CHECK(html ==
              u8"<a href=\"localhost:5173&quot; onclick=&quot;alert(1)&lt;img src=x "
              u8"onerror=alert(2)&gt;/p/Experiment/expid\" internal>exp</a><a "
              u8"href=\"localhost:5173&quot; onclick=&quot;alert(1)&lt;img src=x "
              u8"onerror=alert(2)&gt;/p/Discussion/discid\" internal>dis</a>");
    }
    TEST_CASE("0355") {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"0355");
        CHECK(html == u8"0355");
    }
}
