#pragma once

#include "doctest_config.hh"

TEST_SUITE("unity_link_tag") {
    TEST_CASE("basic-link") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://example.com/discussion\">physicslab</link>");
        auto const& answer = u8"<a href=\"https://example.com/discussion\">physicslab</a>";
        CHECK(html == answer);
    }

    TEST_CASE("mixed-case-with-spaces") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<lInK=\"https://another-example.org/path\">physicslab</LINK>");
        auto const& answer = u8"<a href=\"https://another-example.org/path\">physicslab</a>";
        CHECK(html == answer);
    }

    TEST_CASE("newline-in-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8R"(
<LINK="https://site.com/item"      >te
 xt</link      >
)");
        auto const& answer = u8"<br><a href=\"https://site.com/item\">te<br>&nbsp;xt</a><br>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-same-tag-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<Link=\"https://main.com\"><link=\"https://nested.com\">physicslab</link></Link>");
        // The rejected nested tag becomes literal text; auto-link is suppressed because the
        // URL is inside a URL-link frame (unity_link), matching pl_external's behavior.
        auto const& answer =
            u8"<a href=\"https://main.com\">&lt;link=&quot;https://nested.com&quot;&gt;physicslab</a>&lt;/Link&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("no-nested-autolink") {
        // A bare URL in a link tag's text is not auto-linked (no nested <a> inside <a>).
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://a.com\">visit https://b.com here</link>");
        auto const& answer = u8"<a href=\"https://a.com\">visit&nbsp;https://b.com&nbsp;here</a>";
        CHECK(html == answer);
    }

    TEST_CASE("autolink-suppressed-in-external") {
        // Same suppression applies inside pl_external.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<external=https://a.com>see https://b.com</external>");
        auto const& answer = u8"<a href=\"https://a.com\">see&nbsp;https://b.com</a>";
        CHECK(html == answer);
    }

    TEST_CASE("autolink-outside-frame") {
        // Control: outside any URL-link frame auto-link still works.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"x https://a.com y");
        auto const& answer = u8"x&nbsp;<a href=\"https://a.com\">https://a.com</a>&nbsp;y";
        CHECK(html == answer);
    }

    TEST_CASE("nested-second-url-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"ab<Link=\"https://first.com\">te<link=\"https://second.com\">physicslab</link>st</Link>cd");
        auto const& answer =
            u8"ab<a href=\"https://first.com\">te&lt;link=&quot;https://second.com&quot;&gt;physicslab</a>st&lt;/"
            u8"Link&gt;cd";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-no-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<lInK=\"https://example.com\">");
        auto const& answer = u8"test";
        CHECK(html == answer);
    }

    TEST_CASE("empty-tag-spliced-text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"te<lInK=\"https://example.com\"></link>st");
        auto const& answer = u8"test";
        CHECK(html == answer);
    }

    TEST_CASE("incomplete-attribute-literal") {
        // test invalid tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<link=");
        auto const& answer = u8"test&lt;link=";
        CHECK(html == answer);
    }

    TEST_CASE("unquoted-value-autolink") {
        // unquoted value is not a Unity TextMeshPro link tag; with the '=' auto-link guard
        // removed, the URL inside the rejected tag is now auto-linked
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=https://example.com>text</link>");
        auto const& answer = u8"&lt;link=<a href=\"https://example.com\">https://example.com</a>&gt;text&lt;/link&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("nested-subtag-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<link=\"https://example.com\">text<link=\"https://another-example.com\">text</link></link>");
        auto const& answer =
            u8"<a href=\"https://example.com\">text&lt;link=&quot;https://another-example.com&quot;&gt;text</a>&lt;/"
            u8"link&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("nested-different-url-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<link=\"https://example.com\">physics<link=\"https://another-site.org\">L</link>ab</link>");
        auto const& answer =
            u8"<a href=\"https://example.com\">physics&lt;link=&quot;https://another-site.org&quot;&gt;L</a>ab&lt;/"
            u8"link&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-tag-auto-closed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://example.com\">physicslab");
        auto const& answer = u8"<a href=\"https://example.com\">physicslab</a>";
        CHECK(html == answer);
    }

    TEST_CASE("empty-element-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<link=\"https://example.com\"></link>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("empty-value-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"\">t");
        auto const& answer = u8"&lt;link=&quot;&quot;&gt;t";
        CHECK(html == answer);
    }

    TEST_CASE("unterminated-close-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://example.com\"></link");
        auto const& answer = u8"<a href=\"https://example.com\">&lt;/link</a>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-inside-italic-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<link=\"https://main.com\"><i><link=\"https://nested.com\">c</link></i></link>");
        auto const& answer =
            u8"<a href=\"https://main.com\"><em>&lt;link=&quot;https://nested.com&quot;&gt;c&lt;/link&gt;</em></a>";
        CHECK(html == answer);
    }

    TEST_CASE("javascript-scheme-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"javascript:alert(1)\">x</link>");
        auto const& answer = u8"&lt;link=&quot;javascript:alert(1)&quot;&gt;x&lt;/link&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("quote-injection-rejected") {
        // The opening tag is recognized but its URL fails validation, so the whole span
        // becomes literal text (no auto-link inside).
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://a.com\"onclick=\"alert(1)\">x</link>");
        auto const& answer = u8"&lt;link=&quot;https://a.com&quot;onclick=&quot;alert(1)&quot;&gt;x&lt;/link&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("experiment-inside-italic-literal") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://main.com\"><i><experiment=a>c</experiment></i></link>");
        auto const& answer = u8"<a href=\"https://main.com\"><em>&lt;experiment=a&gt;c&lt;/experiment&gt;</em></a>";
        CHECK(html == answer);
    }

    TEST_CASE("discussion-inside-italic-literal") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://main.com\"><i><discussion=a>c</discussion></i></link>");
        auto const& answer = u8"<a href=\"https://main.com\"><em>&lt;discussion=a&gt;c&lt;/discussion&gt;</em></a>";
        CHECK(html == answer);
    }

    TEST_CASE("space-after-value-autolink") {
        // Same as above: rejected at the root, so the URL is auto-linked.
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<link=\"https://main.com\" onmouseover=\"alert('XSS')\">content</link>");
        auto const& answer =
            u8"&lt;link=&quot;<a "
            u8"href=\"https://main.com\">https://main.com</a>&quot;&nbsp;onmouseover=&quot;alert(&apos;"
            u8"XSS&apos;)&quot;&gt;content&lt;/link&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("javascript-xss-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"javascript:alert('XSS')\">clickme</link>");
        auto const& answer = u8"&lt;link=&quot;javascript:alert(&apos;XSS&apos;)&quot;&gt;clickme&lt;/link&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("schemeless-url-accepted") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"example.com\">text</link>");
        auto const& answer = u8"<a href=\"example.com\">text</a>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-verbatim") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<link=\"https://example.com\">example</link>cd");
        auto const& answer = u8"ab<link=\"https://example.com\">example</link>cd";
        CHECK(html == answer);
    }

    // --- rejected <link="..."> tags (recognized structure, invalid URL) become one literal span ---

    TEST_CASE("angle-bracket-in-value-literal") {
        // a '<' inside the rejected value no longer dispatches as a real tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"a<b\">x</link>");
        auto const& answer = u8"&lt;link=&quot;a&lt;b&quot;&gt;x&lt;/link&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("entity-stays-escaped") {
        // '&amp;' inside the rejected span stays an entity (no double escaping)
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"javascript:alert(&amp;x)\">x</link>");
        auto const& answer = u8"&lt;link=&quot;javascript:alert(&amp;x)&quot;&gt;x&lt;/link&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("macro-not-expanded") {
        // macros inside the rejected span are not expanded
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"javascript:alert({Project})\">x</link>");
        auto const& answer = u8"&lt;link=&quot;javascript:alert({Project})&quot;&gt;x&lt;/link&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("backslash-escape-processed") {
        // backslash escapes are still processed inside the rejected span
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"javascript:\\*\">x</link>");
        auto const& answer = u8"&lt;link=&quot;javascript:*&quot;&gt;x&lt;/link&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("markdown-not-interpreted") {
        // markdown link syntax inside the rejected span is not interpreted
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"javascript:[a](https://x.com)\">x</link>");
        auto const& answer = u8"&lt;link=&quot;javascript:[a](https://x.com)&quot;&gt;x&lt;/link&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-fullwidth-brackets") {
        // the PLUnity backend escapes the rejected span's angle brackets as full-width chars
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<link=\"javascript:x\">x</link>");
        auto const& answer =
            u8"<size=20>\uff1c</size>link=\"javascript:x\"<size=20>\uff1e</size>x<size=20>\uff1c</size>/link<size=20>"
            u8"\uff1e</size>";
        CHECK(html == answer);
    }

    TEST_CASE("cjk-path-percent-encoded") {
        // non-ASCII (CJK) in the link path is accepted and percent-encoded
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<link=\"https://x.com/中文\">x</link>");
        auto const& answer = u8"<a href=\"https://x.com/%E4%B8%AD%E6%96%87\">x</a>";
        CHECK(html == answer);
    }
}
