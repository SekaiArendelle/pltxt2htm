#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"<a>text</a>");
        auto answer = ::fast_io::u8string_view{u8"<span style=\"color:#0000AA;\">text</span>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2fixedadv_htmld(u8"{project}", u8"localhost:5173", u8"<img src=x onerror=alert(1)>",
                                                   u8"visitor", u8"author", u8"coauthors");
        auto answer = ::fast_io::u8string_view{u8"&lt;img src=x onerror=alert(1)&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(
            u8"{visitor}{author}{coauthors}", u8"localhost:5173", u8"project", u8"<svg/onload=alert(2)>",
            u8"<script>alert(3)</script>", u8"<iframe src=javascript:alert(4)></iframe>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;svg/onload=alert(2)&gt;&lt;script&gt;alert(3)&lt;/script&gt;&lt;iframe "
            u8"src=javascript:alert(4)&gt;&lt;/iframe&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(
            u8"{project}{visitor}{author}{coauthors}", u8"localhost:5173", u8"<svg/onload=alert(1)>",
            u8"<img src=x onerror=alert(2)>", u8"<script>alert(3)</script>", u8"<a href=javascript:alert(4)>x</a>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;svg/onload=alert(1)&gt;&lt;img src=x onerror=alert(2)&gt;&lt;script&gt;alert(3)&lt;/script&gt;&lt;a "
            u8"href=javascript:alert(4)&gt;x&lt;/a&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2fixedadv_htmld(
            u8"<experiment=expid>exp</experiment><discussion=discid>dis</discussion>",
            u8"localhost:5173\" onclick=\"alert(1)<img src=x onerror=alert(2)>", u8"project", u8"visitor", u8"author",
            u8"coauthors");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"localhost:5173&quot; onclick=&quot;alert(1)&lt;img src=x "
            u8"onerror=alert(2)&gt;/ExperimentSummary/Experiment/expid\" internal>exp</a><a "
            u8"href=\"localhost:5173&quot; onclick=&quot;alert(1)&lt;img src=x "
            u8"onerror=alert(2)&gt;/ExperimentSummary/Discussion/discid\" internal>dis</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
