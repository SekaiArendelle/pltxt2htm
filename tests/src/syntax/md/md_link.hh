#pragma once

#include "doctest_config.hh"

TEST_SUITE("md_link") {
    TEST_CASE("basic") {
        auto const& pltext = u8"[text](https://example.com)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">text</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<external=https://example.com>text</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("unknown-tld-rejected") {
        auto const& pltext = u8"[text](https://example.wtf)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[text](https://example.wtf)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("userinfo-url-autolinked") {
        // md_link fails (invalid URL), and with the '](' auto-link guard removed the
        // URL inside the parentheses is now auto-linked
        auto const& pltext = u8"[text](https://example.com@evil.invalid/path)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[text](<a href=\"https://example.com\">https://example.com</a>@evil.invalid/path)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"[text](<external=https://example.com>https://example.com</external>@evil.invalid/path)";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("invalid-url-literal") {
        auto const& pltext = u8"[text](url)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[text](url)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("alternate-text") {
        auto const& pltext = u8"[hello](https://example.com)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">hello</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<external=https://example.com>hello</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("anchor-rejected") {
        auto const& pltext = u8"[link](#anchor)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[link](#anchor)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("space-in-text") {
        auto const& pltext = u8"[text with spaces](https://example.com/path)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com/path\">text&nbsp;with&nbsp;spaces</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<external=https://example.com/path>text\u00A0with\u00A0spaces</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("escaped-brackets") {
        auto const& pltext = u8"[escaped \\[brackets\\]](example.com)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"example.com\">escaped&nbsp;[brackets]</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<external=example.com>escaped\u00A0[brackets]</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("nested-brackets-invalid-url") {
        auto const& pltext = u8"[nested [link]](url)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[nested&nbsp;[link]](url)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"[nested\u00A0[link]](url)";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("nested-brackets-rejected") {
        auto const& pltext = u8"[nested [link]](example.com)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[nested&nbsp;[link]](example.com)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"[nested\u00A0[link]](example.com)";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("bold-inside-text") {
        auto const& pltext = u8"[t**ex**t](example.com)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"example.com\">t<strong>ex</strong>t</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<external=example.com>t<b>ex</b>t</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("text-html-escaped") {
        // Escape HTML-sensitive characters in link text.
        auto const& pltext = u8"[a&\"'<>](example.com)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"example.com\">a&amp;&quot;&apos;&lt;&gt;</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<external=example.com>a&\"'<size=20>＜</size><size=20>＞</size></external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("schemeless-url") {
        auto const& pltext = u8"[text](example.com)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"example.com\">text</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<external=example.com>text</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("invalid-url-literal-repeat") {
        auto const& pltext = u8"[text](url)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[text](url)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("unclosed-bracket") {
        auto const& pltext = u8"[text";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[text";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("unclosed-paren") {
        auto const& pltext = u8"[text](url";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[text](url";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("empty-both") {
        auto const& pltext = u8"[]()";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[]()";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("empty-text") {
        auto const& pltext = u8"[](example.com)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"example.com\"></a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<external=example.com></external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("newline-in-text") {
        auto const& pltext = u8"[te\nxt](example.com)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[te<br>xt](example.com)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("query-and-ampersand") {
        auto const& pltext = u8"[q](example.com/?a=1&b=2)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"example.com/?a=1&amp;b=2\">q</a>";
        CHECK(html == answer);
    }

    TEST_CASE("entity-in-query") {
        auto const& pltext = u8"[q](example.com/?a=1&amp;b=2)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"example.com/?a=1&amp;b=2\">q</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<external=example.com/?a=1&b=2>q</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("quote-entity-encoded") {
        auto const& pltext = u8"[q](example.com/?value=&quot;)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"example.com/?value=%22\">q</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<external=example.com/?value=%22>q</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("copy-entity-encoded") {
        auto const& pltext = u8"[q](example.com/?value=&copy;)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"example.com/?value=%C2%A9\">q</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<external=example.com/?value=%C2%A9>q</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("apos-entity-encoded") {
        auto const& pltext = u8"[q](example.com/?value=&apos;)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"example.com/?value=%27\">q</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<external=example.com/?value=%27>q</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("unknown-entity-kept") {
        auto const& pltext = u8"[q](example.com/?value=&bogus;)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"example.com/?value=&amp;bogus;\">q</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<external=example.com/?value=&bogus;>q</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("cjk-path-encoded") {
        // Chinese characters in URL path are percent-encoded
        auto const& pltext = u8"[text](https://example.com/中文路径)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com/%E4%B8%AD%E6%96%87%E8%B7%AF%E5%BE%84\">text</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<external=https://example.com/%E4%B8%AD%E6%96%87%E8%B7%AF%E5%BE%84>text</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("localhost-rejected") {
        auto const& pltext = u8"[text](https://localhost)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[text](https://localhost)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("localhost-with-port-rejected") {
        auto const& pltext = u8"[text](https://localhost:8080)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[text](https://localhost:8080)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("loopback-rejected") {
        auto const& pltext = u8"[text](https://127.0.0.1)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[text](https://127.0.0.1)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("max-port-accepted") {
        auto const& pltext = u8"[text](https://www.example.com:65535)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://www.example.com:65535\">text</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<external=https://www.example.com:65535>text</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("port-out-of-range-rejected") {
        auto const& pltext = u8"[text](https://www.example.com:65536)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[text](https://www.example.com:65536)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("leading-hyphen-rejected") {
        auto const& pltext = u8"[text](https://-example.com/a/path)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[text](https://-example.com/a/path)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("trailing-hyphen-rejected") {
        auto const& pltext = u8"[text](https://example-.com/a/path)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"[text](https://example-.com/a/path)";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = pltext;
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("video-url") {
        auto const& pltext = u8"[text](https://www.bilibili.com/video/BV1df421f7WB)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://www.bilibili.com/video/BV1df421f7WB\">text</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<external=https://www.bilibili.com/video/BV1df421f7WB>text</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }
}
