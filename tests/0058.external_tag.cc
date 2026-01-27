#include "precompile.hh"

int main() {
    {
        auto html =
            ::pltxt2htm_test::pltxt2advanced_htmld(u8"<external=https://example.com/discussion>physicsLab</external>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://example.com/discussion\">physicsLab</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<eXtErNaL=https://another-example.org/path      >physicsLab</EXTERNAL      >");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://another-example.org/path\">physicsLab</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8R"(
<EXTERNAL=https://site.com/item      >te
 xt</external      >
)");
        auto answer = ::fast_io::u8string_view{
            u8"<br><a href=\"https://site.com/item\">te<br>&nbsp;xt</a><br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<External=https://main.com><external=https://nested.com>physicsLab</external></"
            u8"External>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://main.com\"><a href=\"https://nested.com\">physicsLab</a></a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<External=https://first.com><external=https://second.com>physicsLab</external></External>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://first.com\"><a href=\"https://second.com\">physicsLab</a></a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        // test invalid tag
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<External=");
        auto answer = ::fast_io::u8string_view{u8"test&lt;External="};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<external=>t");
        auto answer = ::fast_io::u8string_view{u8"&lt;external=&gt;t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
