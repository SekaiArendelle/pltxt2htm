#include "precompile.hh"

int main() {
    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text](https://example.com)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">text</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text](https://example.wtf)");
        auto answer = ::fast_io::u8string_view{u8"[text](https://example.wtf)"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text](url)");
        auto answer = ::fast_io::u8string_view{u8"[text](url)"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[hello](https://example.com)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">hello</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[link](#anchor)");
        auto answer = ::fast_io::u8string_view{u8"[link](#anchor)"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text with spaces](https://example.com/path)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com/path\">text&nbsp;with&nbsp;spaces</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[escaped \\[brackets\\]](example.com)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"example.com\">escaped&nbsp;[brackets]</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[nested [link]](url)");
        auto answer = ::fast_io::u8string_view{u8"[nested&nbsp;[link]](url)"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[nested [link]](example.com)");
        auto answer = ::fast_io::u8string_view{u8"[nested&nbsp;[link]](example.com)"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[t**ex**t](example.com)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"example.com\">t<strong>ex</strong>t</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        // Escape HTML-sensitive characters in link text.
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[a&\"'<>](example.com)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"example.com\">a&amp;&quot;&apos;&lt;&gt;</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text](example.com)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"example.com\">text</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[text](url)");
        auto answer = ::fast_io::u8string_view{u8"[text](url)"};
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
        auto answer = ::fast_io::u8string_view{u8"[]()"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[](example.com)");
        auto answer = ::fast_io::u8string_view{u8"<a href=\"example.com\"></a>"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(u8"[te\nxt](example.com)");
        auto answer = ::fast_io::u8string_view{u8"[te<br>xt](example.com)"};
        ::pltxt2htm_test::assert_true(html == answer);
    }

    return 0;
}
