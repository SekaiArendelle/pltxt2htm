#pragma once

#include "doctest_config.hh"

TEST_SUITE("md_image") {
    TEST_CASE("![alt text](https://example.com/image.png)") {
        auto const& pltext = u8"![alt text](https://example.com/image.png)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"https://example.com/image.png\" alt=\"alt&nbsp;text\">";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"![alt\u00A0text](https://example.com/image.png)";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("![logo](/images/logo.jpg)") {
        auto const& pltext = u8"![logo](/images/logo.jpg)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"![logo](/images/logo.jpg)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("![alt](https://example.com/image.wtf)") {
        auto const& pltext = u8"![alt](https://example.com/image.wtf)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"https://example.com/image.wtf\" alt=\"alt\">";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("![](https://example.com)") {
        auto const& pltext = u8"![](https://example.com)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"https://example.com\" alt=\"\">";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("![text with spaces](image.gif)") {
        auto const& pltext = u8"![text with spaces](image.gif)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"![text&nbsp;with&nbsp;spaces](image.gif)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"![text\u00A0with\u00A0spaces](image.gif)";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("![special](https://example.com/path with spaces.jpg)") {
        auto const& pltext = u8"![special](https://example.com/path with spaces.jpg)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"https://example.com/path%20with%20spaces.jpg\" alt=\"special\">";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"![special](https://example.com/path%20with%20spaces.jpg)";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("![special](https://example.com/path%20with%20spaces.jpg)") {
        auto const& pltext = u8"![special](https://example.com/path%20with%20spaces.jpg)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"https://example.com/path%20with%20spaces.jpg\" alt=\"special\">";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("![text](url") {
        auto const& pltext = u8"![text](url";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"![text](url";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("![text") {
        auto const& pltext = u8"![text";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"![text";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("![]()") {
        auto const& pltext = u8"![]()";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"![]()";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("Avoid XSS Attack") {
        // Avoid XSS Attack
        auto const& pltext = u8"![x\" onerror=\"alert('XSS')](example.com/image.jpg)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<img src=\"example.com/image.jpg\" alt=\"x&quot;&nbsp;onerror=&quot;alert(&apos;XSS&apos;)\">";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"![x\"\u00A0onerror=\"alert('XSS')](example.com/image.jpg)";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("Escape HTML-sensitive characters in alt text.") {
        // Escape HTML-sensitive characters in alt text.
        auto const& pltext = u8"![a&\"'<>](example.com/image.jpg)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"example.com/image.jpg\" alt=\"a&amp;&quot;&apos;&lt;&gt;\">";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("Cover all markdown backslash escapes in alt text.") {
        // Cover all markdown backslash escapes in alt text.
        auto const& pltext =
            u8R"(![\\\!\"\#\$\%\&\'\(\)\*\+\,\-\.\/\:\;\<\=\>\?\@\[\]\^\_\`\{\|\}\~](example.com/image.jpg))";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<img src=\"example.com/image.jpg\" alt=\"\\!&quot;#$%&amp;&apos;()*+,-./:;&lt;=&gt;?@[]^_`{|}~\">";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"![\\!\"#$%&'()*+,-./:;<=>?@[]^_`{|}~](example.com/image.jpg)";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("text![text](example.com/image.jpg)text") {
        auto const& pltext = u8"text![text](example.com/image.jpg)text";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"text<img src=\"example.com/image.jpg\" alt=\"text\">text";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("test linkbreak inside alt text") {
        // test linkbreak inside alt text
        auto const& pltext = u8"![line\nbreak](example.com/image.jpg)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"![line<br>break](example.com/image.jpg)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("![img](example.com/a.png?x=1&y=2)") {
        auto const& pltext = u8"![img](example.com/a.png?x=1&y=2)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"example.com/a.png?x=1&amp;y=2\" alt=\"img\">";
        CHECK(html == answer);
    }

    TEST_CASE("Chinese characters in URL path are percent-encoded") {
        // Chinese characters in URL path are percent-encoded
        auto const& pltext = u8"![alt](https://example.com/中文路径)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"https://example.com/%E4%B8%AD%E6%96%87%E8%B7%AF%E5%BE%84\" alt=\"alt\">";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"![alt](https://example.com/%E4%B8%AD%E6%96%87%E8%B7%AF%E5%BE%84)";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("![alt](https://www.example.com:65535/image.png)") {
        auto const& pltext = u8"![alt](https://www.example.com:65535/image.png)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"https://www.example.com:65535/image.png\" alt=\"alt\">";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("![alt](example.com/image.png?x=1&amp;y=2)") {
        auto const& pltext = u8"![alt](example.com/image.png?x=1&amp;y=2)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"example.com/image.png?x=1&amp;y=2\" alt=\"alt\">";
        CHECK(html == answer);
    }

    TEST_CASE("![&amp;](https://example.com/image.png)") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"![&amp;](https://example.com/image.png)");
        auto const& answer = u8"![&amp;](https://example.com/image.png)";
        CHECK(html == answer);
    }
}
