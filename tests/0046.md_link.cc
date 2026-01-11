#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text](url)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"url\">text</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[hello](https://example.com)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">hello</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[link](#anchor)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"#anchor\">link</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text with spaces](https://example.com/path)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com/path\">text&nbsp;with&nbsp;spaces</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[escaped \\[brackets\\]](url)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"url\">escaped&nbsp;[brackets]</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[nested [link]](url)");
        auto answer = ::fast_io::u8string_view{u8"[nested&nbsp;[link]](url)"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[t**ex**t](example.com)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"example.com\">t<strong>ex</strong>t</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text](url)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"url\">text</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text");
        auto answer = ::fast_io::u8string_view{u8"[text"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text](url");
        auto answer = ::fast_io::u8string_view{u8"[text](url"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[]()");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"\"></a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[](test)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"test\"></a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
