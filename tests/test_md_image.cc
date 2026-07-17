#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"![alt text](https://example.com/image.png)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<img src=\"https://example.com/image.png\" alt=\"alt&nbsp;text\">"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"![logo](/images/logo.jpg)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"![logo](/images/logo.jpg)"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"![alt](https://example.com/image.wtf)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<img src=\"https://example.com/image.wtf\" alt=\"alt\">"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"![](https://example.com)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<img src=\"https://example.com\" alt=\"\">"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"![text with spaces](image.gif)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"![text&nbsp;with&nbsp;spaces](image.gif)"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"![special](https://example.com/path with spaces.jpg)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<img src=\"https://example.com/path%20with%20spaces.jpg\" alt=\"special\">"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"![special](https://example.com/path%20with%20spaces.jpg)"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"![special](https://example.com/path%20with%20spaces.jpg)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<img src=\"https://example.com/path%20with%20spaces.jpg\" alt=\"special\">"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"![text](url"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"![text](url"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"![text"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"![text"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"![]()"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"![]()"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        // Avoid XSS Attack
        auto pltext = ::fast_io::u8string_view{u8"![x\" onerror=\"alert('XSS')](example.com/image.jpg)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"<img src=\"example.com/image.jpg\" alt=\"x&quot;&nbsp;onerror=&quot;alert(&apos;XSS&apos;)\">"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        // Escape HTML-sensitive characters in alt text.
        auto pltext = ::fast_io::u8string_view{u8"![a&\"'<>](example.com/image.jpg)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer =
            ::fast_io::u8string_view{u8"<img src=\"example.com/image.jpg\" alt=\"a&amp;&quot;&apos;&lt;&gt;\">"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        // Cover all markdown backslash escapes in alt text.
        auto pltext = ::fast_io::u8string_view{
            u8R"(![\\\!\"\#\$\%\&\'\(\)\*\+\,\-\.\/\:\;\<\=\>\?\@\[\]\^\_\`\{\|\}\~](example.com/image.jpg))"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"<img src=\"example.com/image.jpg\" alt=\"\\!&quot;#$%&amp;&apos;()*+,-./:;&lt;=&gt;?@[]^_`{|}~\">"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"![\\!\"#$%&'()*+,-./:;<=>?@[]^_`{|}~](example.com/image.jpg)"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"text![text](example.com/image.jpg)text"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"text<img src=\"example.com/image.jpg\" alt=\"text\">text"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        // test linkbreak inside alt text
        auto pltext = ::fast_io::u8string_view{u8"![line\nbreak](example.com/image.jpg)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"![line<br>break](example.com/image.jpg)"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"![img](example.com/a.png?x=1&y=2)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<img src=\"example.com/a.png?x=1&amp;y=2\" alt=\"img\">"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        // Chinese characters in URL path are percent-encoded
        auto pltext = ::fast_io::u8string_view{u8"![alt](https://example.com/中文路径)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{
            u8"<img src=\"https://example.com/%E4%B8%AD%E6%96%87%E8%B7%AF%E5%BE%84\" alt=\"alt\">"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer =
            ::fast_io::u8string_view{u8"![alt](https://example.com/%E4%B8%AD%E6%96%87%E8%B7%AF%E5%BE%84)"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"![alt](https://www.example.com:65535/image.png)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<img src=\"https://www.example.com:65535/image.png\" alt=\"alt\">"};
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = pltext;
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }

    {
        auto pltext = ::fast_io::u8string_view{u8"![alt](example.com/image.png?x=1&amp;y=2)"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view{u8"<img src=\"example.com/image.png?x=1&amp;y=2\" alt=\"alt\">"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
