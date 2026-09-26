#pragma once

#include "doctest_config.hh"

TEST_SUITE("auto_link") {
    TEST_CASE("https://example.com") {
        auto const& pltext = u8"https://example.com";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">https://example.com</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"<external=https://example.com>https://example.com</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }
    TEST_CASE("visit https://example.com now") {
        auto const& pltext = u8"visit https://example.com now";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"visit&nbsp;<a href=\"https://example.com\">https://example.com</a>&nbsp;now";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"visit\u00A0<external=https://example.com>https://example.com</external>\u00A0now";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }
    TEST_CASE("https://example.com/path?query=1&foo=bar") {
        auto const& pltext = u8"https://example.com/path?query=1&foo=bar";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<a "
            u8"href=\"https://example.com/path?query=1&amp;foo=bar\">https://example.com/path?query=1&amp;foo=bar</a>";
        CHECK(html == answer);
    }
    TEST_CASE("https://example.com/?a=1&amp;b=2") {
        auto const& pltext = u8"https://example.com/?a=1&amp;b=2";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com/?a=1&amp;b=2\">https://example.com/?a=1&amp;b=2</a>";
        CHECK(html == answer);
    }
    TEST_CASE("https://example.com/path#fragment") {
        auto const& pltext = u8"https://example.com/path#fragment";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com/path#fragment\">https://example.com/path#fragment</a>";
        CHECK(html == answer);
    }
    TEST_CASE("https://example.com:8080/path") {
        auto const& pltext = u8"https://example.com:8080/path";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com:8080/path\">https://example.com:8080/path</a>";
        CHECK(html == answer);
    }
    TEST_CASE("http://example.com") {
        auto const& pltext = u8"http://example.com";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"http://example.com\">http://example.com</a>";
        CHECK(html == answer);
    }
    TEST_CASE("prefix https://example.com suffix") {
        auto const& pltext = u8"prefix https://example.com suffix";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"prefix&nbsp;<a href=\"https://example.com\">https://example.com</a>&nbsp;suffix";
        CHECK(html == answer);
    }
    TEST_CASE("not-a-url") {
        auto const& pltext = u8"not-a-url";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"not-a-url";
        CHECK(html == answer);
    }
    TEST_CASE("`https://example.com`") {
        auto const& pltext = u8"`https://example.com`";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<code>https://example.com</code>";
        CHECK(html == answer);
    }
    TEST_CASE("[click](https://example.com)") {
        auto const& pltext = u8"[click](https://example.com)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">click</a>";
        CHECK(html == answer);
    }
    TEST_CASE("Chinese chars after the path are excluded from the auto-...") {
        // Chinese chars after the path are excluded from the auto-link
        auto const& pltext = u8"https://example.com/\u4E2D\u6587";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com/\">https://example.com/</a>\u4E2D\u6587";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<external=https://example.com/>https://example.com/</external>\u4E2D\u6587";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }
    TEST_CASE("Space in the URL is treated as end of the auto-link") {
        // Space in the URL is treated as end of the auto-link
        auto const& pltext = u8"https://example.com/a b";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com/a\">https://example.com/a</a>&nbsp;b";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<external=https://example.com/a>https://example.com/a</external>\u00A0b";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }
    TEST_CASE("https://localhost") {
        auto const& pltext = u8"https://localhost";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"https://localhost";
        CHECK(html == answer);
    }
    TEST_CASE("https://localhost:8080") {
        auto const& pltext = u8"https://localhost:8080";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"https://localhost:8080";
        CHECK(html == answer);
    }
    TEST_CASE("https://127.0.0.1") {
        auto const& pltext = u8"https://127.0.0.1";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"https://127.0.0.1";
        CHECK(html == answer);
    }
    TEST_CASE("https://127.0.0.1:8080") {
        auto const& pltext = u8"https://127.0.0.1:8080";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"https://127.0.0.1:8080";
        CHECK(html == answer);
    }
    TEST_CASE("https://www.example.wtf") {
        auto const& pltext = u8"https://www.example.wtf";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"https://www.example.wtf";
        CHECK(html == answer);
    }
    TEST_CASE("javascript:alert(1)") {
        auto const& pltext = u8"javascript:alert(1)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"javascript:alert(1)";
        CHECK(html == answer);
    }
    TEST_CASE("https://example.com (20)") {
        auto const& pltext = u8"https://example.com:";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"https://example.com:";
        CHECK(html == answer);
    }
    TEST_CASE("https://example.com:80javascript:alert(1)") {
        auto const& pltext = u8"https://example.com:80javascript:alert(1)";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"https://example.com:80javascript:alert(1)";
        CHECK(html == answer);
    }
    TEST_CASE("https://example.com@evil.invalid/path") {
        auto const& pltext = u8"https://example.com@evil.invalid/path";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://example.com\">https://example.com</a>@evil.invalid/path";
        CHECK(html == answer);
    }
    TEST_CASE("https://www.bilibili.com/video/BV1df421f7WB") {
        auto const& pltext = u8"https://www.bilibili.com/video/BV1df421f7WB";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"<a "
            u8"href=\"https://www.bilibili.com/video/BV1df421f7WB\">https://www.bilibili.com/video/BV1df421f7WB</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<external=https://www.bilibili.com/video/BV1df421f7WB>https://www.bilibili.com/video/BV1df421f7WB</"
            u8"external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }
    TEST_CASE("https://www.example.com:65535") {
        auto const& pltext = u8"https://www.example.com:65535";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<a href=\"https://www.example.com:65535\">https://www.example.com:65535</a>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<external=https://www.example.com:65535>https://www.example.com:65535</external>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }
    TEST_CASE("https://www.example.com:65536") {
        auto const& pltext = u8"https://www.example.com:65536";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"https://www.example.com:65536";
        CHECK(html == answer);
    }
    TEST_CASE("https://www.example.com:123456") {
        auto const& pltext = u8"https://www.example.com:123456";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"https://www.example.com:123456";
        CHECK(html == answer);
    }
    TEST_CASE("https://-example.com/a/path") {
        auto const& pltext = u8"https://-example.com/a/path";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"https://-example.com/a/path";
        CHECK(html == answer);
    }
    TEST_CASE("https://example-.com/a/path") {
        auto const& pltext = u8"https://example-.com/a/path";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"https://example-.com/a/path";
        CHECK(html == answer);
    }
    TEST_CASE("non-ASCII is not auto-linked: only the ASCII prefix beco...") {
        // non-ASCII is not auto-linked: only the ASCII prefix becomes a link
        auto const& pltext = u8"x https://x.com/中文 y";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"x&nbsp;<a href=\"https://x.com/\">https://x.com/</a>中文&nbsp;y";
        CHECK(html == answer);
    }
}
