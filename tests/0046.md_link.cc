#include "fast_io/fast_io_dsal/string_view.h"
#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"[text](https://example.com)"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">text</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<external=https://example.com>text</external>"};
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[text](https://example.wtf)"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"[text](https://example.wtf)"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[text](url)"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"[text](url)"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[hello](https://example.com)"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com\">hello</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<external=https://example.com>hello</external>"};
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[link](#anchor)"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"[link](#anchor)"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[text with spaces](https://example.com/path)"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"https://example.com/path\">text&nbsp;with&nbsp;spaces</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"<external=https://example.com/path>text with spaces</external>"};
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[escaped \\[brackets\\]](example.com)"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"example.com\">escaped&nbsp;[brackets]</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<external=example.com>escaped [brackets]</external>"};
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[nested [link]](url)"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"[nested&nbsp;[link]](url)"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[nested [link]](example.com)"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"[nested&nbsp;[link]](example.com)"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[t**ex**t](example.com)"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"example.com\">t<strong>ex</strong>t</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<external=example.com>t<b>ex</b>t</external>"};
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        // Escape HTML-sensitive characters in link text.
        auto pltext = ::fast_io::u8string_view{u8"[a&\"'<>](example.com)"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"example.com\">a&amp;&quot;&apos;&lt;&gt;</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<external=example.com>a&\"'<></external>"};
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[text](example.com)"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"example.com\">text</a>"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<external=example.com>text</external>"};
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[text](url)"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"[text](url)"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[text"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"[text"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[text](url"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"[text](url"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[]()"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"[]()"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[](example.com)"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"<a href=\"example.com\"></a>"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"<external=example.com></external>"};
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"[te\nxt](example.com)"};
        auto html = ::pltxt2htm_test::pltxt2advanced_htmld(pltext);
        auto answer = ::fast_io::u8string_view{u8"[te<br>xt](example.com)"};
        ::pltxt2htm_test::assert_true(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        ::pltxt2htm_test::assert_true(plunity_richtext == plunity_richtext_answer);
    }

    return 0;
}
