#pragma once

#include "doctest_config.hh"

TEST_SUITE("pl_external_tag") {
    TEST_CASE("basic-external-link") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<external=https://example.com/discussion>physicsLab</external>");
        auto const& answer = u8"<a href=\"https://example.com/discussion\">physicsLab</a>";
        CHECK(html == answer);
    }

    TEST_CASE("mixed-case-with-spaces") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<eXtErNaL=https://another-example.org/path      >physicsLab</EXTERNAL      >");
        auto const& answer = u8"<a href=\"https://another-example.org/path\">physicsLab</a>";
        CHECK(html == answer);
    }

    TEST_CASE("newline-in-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8R"(
<EXTERNAL=https://site.com/item      >te
 xt</external      >
)");
        auto const& answer = u8"<br><a href=\"https://site.com/item\">te<br>&nbsp;xt</a><br>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-same-tag-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<External=https://main.com><external=https://nested.com>physicsLab</external></"
            u8"External>");
        auto const& answer =
            u8"<a href=\"https://main.com\">&lt;external=https://nested.com&gt;physicsLab</a>&lt;/External&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("nested-second-url-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"ab<External=https://first.com>te<external=https://second.com>physicsLab</external>st</External>cd");
        auto const& answer =
            u8"ab<a "
            u8"href=\"https://first.com\">te&lt;external=https://second.com&gt;physicsLab</a>st&lt;/External&gt;cd";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-no-content") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<eXternal=https://example.com>");
        auto const& answer = u8"test";
        CHECK(html == answer);
    }

    TEST_CASE("empty-tag-spliced-text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"te<eXternal=https://example.com></external>st");
        auto const& answer = u8"test";
        CHECK(html == answer);
    }

    TEST_CASE("incomplete-attribute-literal") {
        // test invalid tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"test<external=");
        auto const& answer = u8"test&lt;external=";
        CHECK(html == answer);
    }

    TEST_CASE("nested-subtag-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<external=https://example.com>text<external=https://another-example.com>text</external></"
            u8"external>");
        auto const& answer =
            u8"<a "
            u8"href=\"https://example.com\">text&lt;external=https://another-example.com&gt;text</a>&lt;/external&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("nested-different-url-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<external=https://example.com>physics<external=https://another-site.org>L</external>ab</external>");
        auto const& answer =
            u8"<a href=\"https://example.com\">physics"
            u8"&lt;external=https://another-site.org&gt;L</a>ab&lt;/external&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-tag-auto-closed") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<external=https://example.com>physicsLab");
        auto const& answer = u8"<a href=\"https://example.com\">physicsLab</a>";
        CHECK(html == answer);
    }

    TEST_CASE("empty-element-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<external=https://example.com></external>t");
        auto const& answer = u8"tt";
        CHECK(html == answer);
    }

    TEST_CASE("empty-value-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<external=>t");
        auto const& answer = u8"&lt;external=&gt;t";
        CHECK(html == answer);
    }

    TEST_CASE("unterminated-close-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<external=https://example.com></external");
        auto const& answer = u8"<a href=\"https://example.com\">&lt;/external</a>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-inside-italic-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<external=https://main.com><i><external=https://nested.com>c</external></i></external>");
        auto const& answer =
            u8"<a href=\"https://main.com\"><em>&lt;external=https://nested.com&gt;c&lt;/external&gt;</em></a>";
        CHECK(html == answer);
    }

    TEST_CASE("javascript-scheme-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<external=javascript:alert(1)>x</external>");
        auto const& answer = u8"&lt;external=javascript:alert(1)&gt;x&lt;/external&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("quote-injection-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<external=https://a.com\"onclick=\"alert(1)>x</external>");
        auto const& answer = u8"&lt;external=https://a.com&quot;onclick=&quot;alert(1)&gt;x&lt;/external&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("experiment-inside-italic-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<external=https://main.com><i><experiment=a>c</experiment></i></external>");
        auto const& answer = u8"<a href=\"https://main.com\"><em>&lt;experiment=a&gt;c&lt;/experiment&gt;</em></a>";
        CHECK(html == answer);
    }

    TEST_CASE("discussion-inside-italic-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<external=https://main.com><i><discussion=a>c</discussion></i></external>");
        auto const& answer = u8"<a href=\"https://main.com\"><em>&lt;discussion=a&gt;c&lt;/discussion&gt;</em></a>";
        CHECK(html == answer);
    }

    TEST_CASE("space-after-value-autolink") {
        // the space after the value makes this a structural failure (not invalid_url), so with
        // the '=' auto-link guard removed the URL inside the rejected tag is auto-linked
        auto html = ::pltxt2htm_test::pltxt4unittest(
            u8"<external=https://main.com\" onmouseover=\"alert('XSS')\">content</external>");
        auto const& answer =
            u8"&lt;external=<a href=\"https://main.com\">https://main.com</a>&quot;&nbsp;onmouseover=&quot;alert(&apos;"
            u8"XSS&apos;)&quot;&gt;content&lt;/external&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("javascript-xss-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<external=javascript:alert('XSS')>clickme</external>");
        auto const& answer = u8"&lt;external=javascript:alert(&apos;XSS&apos;)&gt;clickme&lt;/external&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("schemeless-url-accepted") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<external=example.com>text</external>");
        auto const& answer = u8"<a href=\"example.com\">text</a>";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-verbatim") {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"ab<external=https://example.com>example</external>cd");
        auto const& answer = u8"ab<external=https://example.com>example</external>cd";
        CHECK(html == answer);
    }

    TEST_CASE("cjk-path-percent-encoded") {
        // non-ASCII (CJK) in the external path is accepted and percent-encoded
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<external=https://x.com/中文>x</external>");
        auto const& answer = u8"<a href=\"https://x.com/%E4%B8%AD%E6%96%87\">x</a>";
        CHECK(html == answer);
    }
}
