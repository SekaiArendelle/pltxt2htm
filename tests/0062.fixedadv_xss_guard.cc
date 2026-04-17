#include <pltxt2htm/pltxt2htm.hh>
#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm::pltxt2fixedadv_html<::pltxt2htm::Contracts::quick_enforce>(
            u8"{project}", u8"localhost:5173", u8"<img src=x onerror=alert(1)>", u8"v", u8"a", u8"c");
        auto answer = ::fast_io::u8string_view{u8"&lt;img src=x onerror=alert(1)&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm::pltxt2fixedadv_html<::pltxt2htm::Contracts::quick_enforce>(
            u8"<experiment=642cf37a494746375aae306a>x</experiment>",
            u8"x\" onmouseover=\"alert(1)",
            u8"$PROJECT",
            u8"$VISITOR",
            u8"$AUTHOR",
            u8"$COAUTHORS");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"x&quot; onmouseover=&quot;alert(1)/ExperimentSummary/Experiment/642cf37a494746375aae306a\" internal>x</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm::pltxt2fixedadv_html<::pltxt2htm::Contracts::quick_enforce>(
            u8"{project}{visitor}{author}{coauthors}",
            u8"localhost:5173",
            u8"<svg/onload=alert(1)>",
            u8"<img src=x onerror=alert(2)>",
            u8"<script>alert(3)</script>",
            u8"<a href=javascript:alert(4)>x</a>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;svg/onload=alert(1)&gt;&lt;img src=x onerror=alert(2)&gt;&lt;script&gt;alert(3)&lt;/script&gt;&lt;a href=javascript:alert(4)&gt;x&lt;/a&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto richtext = ::pltxt2htm::pltxt2plunity_introduction<::pltxt2htm::Contracts::quick_enforce>(
            u8"{project}{visitor}{author}{coauthors}",
            u8"<svg/onload=alert(1)>",
            u8"<img src=x onerror=alert(2)>",
            u8"<script>alert(3)</script>",
            u8"<a href=javascript:alert(4)>x</a>");
        auto answer = ::fast_io::u8string_view{
            u8"<size=20>＜</size>svg/onload=alert(1)<size=20>＞</size><size=20>＜</size>img src=x onerror=alert(2)<size=20>＞</size><size=20>＜</size>script<size=20>＞</size>alert(3)<size=20>＜</size>/script<size=20>＞</size><size=20>＜</size>a href=javascript:alert(4)<size=20>＞</size>x<size=20>＜</size>/a<size=20>＞</size>"};
        ::pltxt2htm_test::assert_true(richtext == answer);
    }

    return 0;
}
