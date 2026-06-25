#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://www.example.com\">click here</a>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://www.example.com\">click&nbsp;here</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href='https://www.example.com'>click here</a>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://www.example.com\">click&nbsp;here</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://www.example.com\">a<b>b</b>c</a>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://www.example.com\">a<strong>b</strong>c</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://www.example.com\"><color=red>text</color></a>");
        auto answer = ::fast_io::u8string_view{
            u8"<a href=\"https://www.example.com\"><span style=\"color:red;\">text</span></a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://www.example.com\">text");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://www.example.com\">text</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2common_htmld(u8"<a href=\"https://www.example.com\">click here</a>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://www.example.com\">click&nbsp;here</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"<a href=\"https://www.example.com\">click here</a>");
        auto answer = ::fast_io::u8string_view{u8"<external=https://www.example.com>click here</external>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://www.example.com\" style=\"color:red;\"></a>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;a&nbsp;href=&quot;<a "
            u8"href=\"https://www.example.com\">https://www.example.com</"
            u8"a>&quot;&nbsp;style=&quot;color:red;&quot;&gt;&lt;/a&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // illegal URLs (not http/https) are rejected — the tag is output as literal escaped text
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"javascript:alert(1)\">click</a>");
        auto answer = ::fast_io::u8string_view{u8"&lt;a&nbsp;href=&quot;javascript:alert(1)&quot;&gt;click&lt;/a&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"data:text/html,<script>alert(1)</script>\">click</a>");
        auto answer = ::fast_io::u8string_view{
            u8"&lt;a&nbsp;href=&quot;data:text/html,&lt;script&gt;alert(1)&lt;/script&gt;&quot;&gt;click&lt;/a&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"vbscript:msgbox(1)\">click</a>");
        auto answer = ::fast_io::u8string_view{u8"&lt;a&nbsp;href=&quot;vbscript:msgbox(1)&quot;&gt;click&lt;/a&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://invalid.host\">click</a>");
        auto answer = ::fast_io::u8string_view{u8"&lt;a&nbsp;href=&quot;https://invalid.host&quot;&gt;click&lt;/a&gt;"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // scheme-less URLs are accepted (validated by try_parse_url)
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"example.com\">text</a>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"example.com\">text</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"sub.example.com/path\">text</a>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"sub.example.com/path\">text</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // internal attribute
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<a href=\"https://www.example.com\" internal>text</a>");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://www.example.com\" internal>text</a>"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
