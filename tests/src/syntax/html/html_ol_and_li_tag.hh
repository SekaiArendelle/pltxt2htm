#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_ol_and_li_tag") {
    TEST_CASE("valid-two-item-list") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li>item1</li><li>item2</li></ol>");
        auto const& answer = u8"<ol><li>item1</li><li>item2</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase-tag-normalization") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<OL    ><LI>item1</LI><LI>item2</LI></OL  >");
        auto const& answer = u8"<ol><li>item1</li><li>item2</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("color-inside-item") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li><color=red>item1</color></li><li>item2</li></ol>");
        auto const& answer = u8"<ol><li><span style=\"color:red;\">item1</span></li><li>item2</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("color-close-after-list") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li><color=red>item1</li></ol></color>");
        auto const& answer = u8"<ol><li><span style=\"color:red;\">item1</span></li></ol>&lt;/color&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("nested-list-in-item") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li>item1<ol><li>item2</li></ol></li></ol>");
        auto const& answer = u8"<ol><li>item1<ol><li>item2</li></ol></li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("lone-open-tag") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol>");
        auto const& answer = u8"&lt;ol&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("mid-text-list-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<ol></ol>t");
        auto const& answer = u8"t&lt;ol&gt;&lt;/ol&gt;t";
        CHECK(html == answer);
    }

    TEST_CASE("incomplete-tag-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<li");
        auto const& answer = u8"t&lt;li";
        CHECK(html == answer);
    }

    TEST_CASE("unknown-tag-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<olt");
        auto const& answer = u8"t&lt;olt";
        CHECK(html == answer);
    }

    TEST_CASE("numbered-lines-output") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol><li>test</li><li>text</li><li>test</li></ol>");
        auto const& answer = u8"1. test\n2. text\n3. test\n";
        CHECK(html == answer);
    }

    TEST_CASE("nested-empty-parent") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol><li><ol><li>xxx</li></ol></li></ol>");
        auto const& answer = u8"1. \n  1. xxx\n";
        CHECK(html == answer);
    }

    TEST_CASE("depth-based-bullet-glyphs") {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li>a<ul><li>b<ul><li>c</li></ul></li></ul></li></ul>");
        auto const& answer = u8"\u2022 a\n  \u2218 b\n    \u25ab c\n";
        CHECK(html == answer);
    }

    TEST_CASE("mixed-nested-list-types") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li>a<ol><li>b</li></ol></li></ul>");
        auto const& answer = u8"\u2022 a\n  1. b\n";
        CHECK(html == answer);
    }

    TEST_CASE("start-attr-preserved") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"5\"><li>item1</li><li>item2</li></ol>");
        auto const& answer = u8"<ol start=\"5\"><li>item1</li><li>item2</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("start-one-dropped") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"1\"><li>item1</li></ol>");
        auto const& answer = u8"<ol><li>item1</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("start-zero-preserved") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"0\"><li>item1</li></ol>");
        auto const& answer = u8"<ol start=\"0\"><li>item1</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("nested-start-attr-preserved") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li>item1<ol start=\"3\"><li>item2</li></ol></li></ol>");
        auto const& answer = u8"<ol><li>item1<ol start=\"3\"><li>item2</li></ol></li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase-with-start-attr") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<OL start=\"5\" ><LI>item1</LI></OL>");
        auto const& answer = u8"<ol start=\"5\"><li>item1</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("uppercase-attr-name-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol START=\"5\"><li>item1</li></ol>");
        auto const& answer = u8"&lt;ol&nbsp;START=&quot;5&quot;&gt;&lt;li&gt;item1&lt;/li&gt;&lt;/ol&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("single-quote-attr-normalized") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start='7'><li>item1</li></ol>");
        auto const& answer = u8"<ol start=\"7\"><li>item1</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("start-offset-numbering") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol start=\"5\"><li>a</li><li>b</li></ol>");
        auto const& answer = u8"5. a\n6. b\n";
        CHECK(html == answer);
    }

    TEST_CASE("nested-start-offset-numbering") {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol><li>a<ol start=\"2\"><li>b</li></ol></li></ol>");
        auto const& answer = u8"1. a\n  2. b\n";
        CHECK(html == answer);
    }

    TEST_CASE("roundtrip-start-attr") {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<ol start=\"5\"><li>a</li><li>b</li></ol>");
        auto const& answer = u8"<ol start=\"5\"><li>a</li><li>b</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("nonnumeric-start-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"abc\"><li>a</li></ol>");
        auto const& answer = u8"&lt;ol&nbsp;start=&quot;abc&quot;&gt;&lt;li&gt;a&lt;/li&gt;&lt;/ol&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unsupported-attr-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol class=\"x\"><li>a</li></ol>");
        auto const& answer = u8"&lt;ol&nbsp;class=&quot;x&quot;&gt;&lt;li&gt;a&lt;/li&gt;&lt;/ol&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("unclosed-tag-with-attr") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"5\">");
        auto const& answer = u8"&lt;ol&nbsp;start=&quot;5&quot;&gt;";
        CHECK(html == answer);
    }

    // ---- empty lists (no <li> items) are malformed and fall back to literal text ----
    TEST_CASE("empty-list-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol></ol>");
        auto const& answer = u8"&lt;ol&gt;&lt;/ol&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("empty-list-with-newline") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol>\n</ol>");
        auto const& answer = u8"&lt;ol&gt;<br>&lt;/ol&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("empty-list-start-attr") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"5\"></ol>");
        auto const& answer = u8"&lt;ol&nbsp;start=&quot;5&quot;&gt;&lt;/ol&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("nested-empty-list-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li>a<ol></ol></li></ol>");
        auto const& answer = u8"&lt;ol&gt;&lt;li&gt;a&lt;ol&gt;&lt;/ol&gt;&lt;/li&gt;&lt;/ol&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("empty-item-still-valid") {
        // an empty <li> item is still a valid list
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li></li></ol>");
        auto const& answer = u8"<ol><li></li></ol>";
        CHECK(html == answer);
    }
    // ---- regression: empty lists must not panic the backends (fuzzer crash) ----
    TEST_CASE("empty-list-no-panic") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol></ol>");
        auto const& answer = u8"<size=20>＜</size>ol<size=20>＞</size><size=20>＜</size>/ol<size=20>＞</size>";
        CHECK(html == answer);
    }
    TEST_CASE("empty-list-backend-plain") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol>\n</ol>");
        auto const& answer = u8"<size=20>＜</size>ol<size=20>＞</size>\n<size=20>＜</size>/ol<size=20>＞</size>";
        CHECK(html == answer);
    }
}
