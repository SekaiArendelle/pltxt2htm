#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_img_tag") {
    TEST_CASE("valid-attribute-passthrough") {
        auto const& pltext = u8"<img src=\"https://example.com/image.png\" alt=\"description\">";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"https://example.com/image.png\" alt=\"description\">";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<size=20>\uff1c</size>img src=\"https://example.com/image.png\" alt=\"description\""
            u8"<size=20>\uff1e</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("attribute-reorder-src-alt") {
        auto const& pltext = u8"<img alt=\"description\" src=\"https://example.com/image.png\">";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"https://example.com/image.png\" alt=\"description\">";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<size=20>\uff1c</size>img src=\"https://example.com/image.png\" alt=\"description\""
            u8"<size=20>\uff1e</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("self-closing-normalized") {
        auto const& pltext = u8"<img src=\"https://example.com/image.png\" alt=\"description\"/>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"https://example.com/image.png\" alt=\"description\">";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"<size=20>\uff1c</size>img src=\"https://example.com/image.png\" alt=\"description\""
            u8"<size=20>\uff1e</size>";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("alt-entity-preserved") {
        auto const& pltext = u8"<img src=\"https://example.com/image.png\" alt=\"alt&amp;text\">";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"https://example.com/image.png\" alt=\"alt&amp;text\">";
        CHECK(html == answer);
    }

    TEST_CASE("entity-decode-and-escape") {
        auto const& pltext = u8"<img src=\"https://example.com/?a=1&amp;b=2\" alt=\"&quot;&bogus;&copy;\">";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<img src=\"https://example.com/?a=1&amp;b=2\" alt=\"&quot;&amp;bogus;©\">";
        CHECK(html == answer);
    }

    TEST_CASE("inline-surrounding-text") {
        auto const& pltext = u8"前<img src=\"a.jpg\" alt=\"中间\">后";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"前<img src=\"a.jpg\" alt=\"中间\">后";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer =
            u8"前<size=20>\uff1c</size>img src=\"a.jpg\" alt=\"中间\"<size=20>\uff1e</size>后";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }

    TEST_CASE("missing-attributes-escaped") {
        auto const& pltext = u8"<img>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;img&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("relative-url-rejected") {
        auto const& pltext = u8"<img src=\"a.jpg\">";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"&lt;img&nbsp;src=&quot;a.jpg&quot;&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("htmld-drops-tag") {
        auto html =
            ::pltxt2htm_test::pltxt2common_htmld(u8"<img src=\"https://example.com/image.png\" alt=\"description\">");
        auto const& answer = u8"";
        CHECK(html == answer);
    }

    TEST_CASE("class-attr-url-autolink") {
        auto const& pltext = u8"<img src=\"https://example.com/image.png\" alt=\"description\" class=\"cls\"/>";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer =
            u8"&lt;img&nbsp;src=&quot;<a "
            u8"href=\"https://example.com/image.png\">https://example.com/image.png</"
            u8"a>&quot;&nbsp;alt=&quot;description&quot;&nbsp;class=&quot;cls&quot;/&gt;";
        CHECK(html == answer);
    }
}
