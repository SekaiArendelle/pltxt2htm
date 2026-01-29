#include "precompile.hh"

int main() {
    {
        auto html =
            ::pltxt2htm_test::pltxt2advanced_htmld(u8"<external=https://example.com/discussion>physicsLab</external>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com/discussion\">physicsLab</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<eXtErNaL=https://another-example.org/path      >physicsLab</EXTERNAL      >");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://another-example.org/path\">physicsLab</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8R"(
<EXTERNAL=https://site.com/item      >te
 xt</external      >
)");
        auto answer = ::fast_io::u8string_view{u8"<br><a href=\"https://site.com/item\">te<br>&nbsp;xt</a><br>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<External=https://main.com><external=https://nested.com>physicsLab</external></"
            u8"External>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://main.com\">&lt;external=https://nested.com&gt;physicsLab</a>&lt;/External&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"ab<External=https://first.com>te<external=https://second.com>physicsLab</external>st</External>cd");
        auto answer = ::fast_io::u8string_view{
            u8"ab<a "
            u8"href=\"https://first.com\">te&lt;external=https://second.com&gt;physicsLab</a>st&lt;/External&gt;cd"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<eXternal=https://example.com>");
        auto answer = ::fast_io::u8string_view{u8"test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"te<eXternal=https://example.com></external>st");
        auto answer = ::fast_io::u8string_view{u8"test"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        // test invalid tag
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"test<external=");
        auto answer = ::fast_io::u8string_view{u8"test&lt;external="};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<external=https://example.com>text<external=https://another-example.com>text</external></"
            u8"external>");
        auto answer = ::fast_io::u8string_view{
            u8"<a "
            u8"href=\"https://example.com\">text&lt;external=https://another-example.com&gt;text</a>&lt;/external&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<external=https://example.com>physics<external=https://another-site.org>L</external>ab</external>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://example.com\">physics"
            u8"&lt;external=https://another-site.org&gt;L</a>ab&lt;/external&gt;"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<external=https://example.com>physicsLab");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">physicsLab</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"t<external=https://example.com></external>t");
        auto answer = ::fast_io::u8string_view{u8"tt"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<external=>t");
        auto answer = ::fast_io::u8string_view{u8"&lt;external=&gt;t"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"<external=https://example.com></external");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">&lt;/external</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(
            u8"<external=https://main.com><i><external=https://nested.com>c</external></i></external>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://main.com\"><em>&lt;external=https://nested.com&gt;c&lt;/external&gt;</em></a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
