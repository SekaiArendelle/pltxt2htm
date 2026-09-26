#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_ol_and_li_tag") {
    TEST_CASE("<ol><li>item1</li><li>item2</li></ol>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li>item1</li><li>item2</li></ol>");
        auto const& answer = u8"<ol><li>item1</li><li>item2</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("<OL ><LI>item1</LI><LI>item2</LI></OL >") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<OL    ><LI>item1</LI><LI>item2</LI></OL  >");
        auto const& answer = u8"<ol><li>item1</li><li>item2</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("<ol><li><color=red>item1</color></li><li>item2</li></ol>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li><color=red>item1</color></li><li>item2</li></ol>");
        auto const& answer = u8"<ol><li><span style=\"color:red;\">item1</span></li><li>item2</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("<ol><li><color=red>item1</li></ol></color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li><color=red>item1</li></ol></color>");
        auto const& answer = u8"<ol><li><span style=\"color:red;\">item1</span></li></ol>&lt;/color&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<ol><li>item1<ol><li>item2</li></ol></li></ol>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li>item1<ol><li>item2</li></ol></li></ol>");
        auto const& answer = u8"<ol><li>item1<ol><li>item2</li></ol></li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("<ol>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol>");
        auto const& answer = u8"&lt;ol&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("t<ol></ol>t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<ol></ol>t");
        auto const& answer = u8"t&lt;ol&gt;&lt;/ol&gt;t";
        CHECK(html == answer);
    }

    TEST_CASE("t<li") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<li");
        auto const& answer = u8"t&lt;li";
        CHECK(html == answer);
    }

    TEST_CASE("t<olt") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<olt");
        auto const& answer = u8"t&lt;olt";
        CHECK(html == answer);
    }

    TEST_CASE("<ol><li>test</li><li>text</li><li>test</li></ol>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol><li>test</li><li>text</li><li>test</li></ol>");
        auto const& answer = u8"1. test\n2. text\n3. test\n";
        CHECK(html == answer);
    }

    TEST_CASE("<ol><li><ol><li>xxx</li></ol></li></ol>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol><li><ol><li>xxx</li></ol></li></ol>");
        auto const& answer = u8"1. \n  1. xxx\n";
        CHECK(html == answer);
    }

    TEST_CASE("<ul><li>a<ul><li>b<ul><li>c</li></ul></li></ul></li></ul...") {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li>a<ul><li>b<ul><li>c</li></ul></li></ul></li></ul>");
        auto const& answer = u8"\u2022 a\n  \u2218 b\n    \u25ab c\n";
        CHECK(html == answer);
    }

    TEST_CASE("<ul><li>a<ol><li>b</li></ol></li></ul>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li>a<ol><li>b</li></ol></li></ul>");
        auto const& answer = u8"\u2022 a\n  1. b\n";
        CHECK(html == answer);
    }

    TEST_CASE("<ol start=/'5/'><li>item1</li><li>item2</li></ol>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"5\"><li>item1</li><li>item2</li></ol>");
        auto const& answer = u8"<ol start=\"5\"><li>item1</li><li>item2</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("<ol start=/'1/'><li>item1</li></ol>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"1\"><li>item1</li></ol>");
        auto const& answer = u8"<ol><li>item1</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("<ol start=/'0/'><li>item1</li></ol>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"0\"><li>item1</li></ol>");
        auto const& answer = u8"<ol start=\"0\"><li>item1</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("<ol><li>item1<ol start=/'3/'><li>item2</li></ol></li></o...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li>item1<ol start=\"3\"><li>item2</li></ol></li></ol>");
        auto const& answer = u8"<ol><li>item1<ol start=\"3\"><li>item2</li></ol></li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("<OL start=/'5/' ><LI>item1</LI></OL>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<OL start=\"5\" ><LI>item1</LI></OL>");
        auto const& answer = u8"<ol start=\"5\"><li>item1</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("<ol START=/'5/'><li>item1</li></ol>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol START=\"5\"><li>item1</li></ol>");
        auto const& answer = u8"&lt;ol&nbsp;START=&quot;5&quot;&gt;&lt;li&gt;item1&lt;/li&gt;&lt;/ol&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<ol start='7'><li>item1</li></ol>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start='7'><li>item1</li></ol>");
        auto const& answer = u8"<ol start=\"7\"><li>item1</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("<ol start=/'5/'><li>a</li><li>b</li></ol>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol start=\"5\"><li>a</li><li>b</li></ol>");
        auto const& answer = u8"5. a\n6. b\n";
        CHECK(html == answer);
    }

    TEST_CASE("<ol><li>a<ol start=/'2/'><li>b</li></ol></li></ol>") {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol><li>a<ol start=\"2\"><li>b</li></ol></li></ol>");
        auto const& answer = u8"1. a\n  2. b\n";
        CHECK(html == answer);
    }

    TEST_CASE("<ol start=/'5/'><li>a</li><li>b</li></ol> (23)") {
        auto html = ::pltxt2htm_test::pltxt2roundtrip_htmld(u8"<ol start=\"5\"><li>a</li><li>b</li></ol>");
        auto const& answer = u8"<ol start=\"5\"><li>a</li><li>b</li></ol>";
        CHECK(html == answer);
    }

    TEST_CASE("<ol start=/'abc/'><li>a</li></ol>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"abc\"><li>a</li></ol>");
        auto const& answer = u8"&lt;ol&nbsp;start=&quot;abc&quot;&gt;&lt;li&gt;a&lt;/li&gt;&lt;/ol&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<ol class=/'x/'><li>a</li></ol>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol class=\"x\"><li>a</li></ol>");
        auto const& answer = u8"&lt;ol&nbsp;class=&quot;x&quot;&gt;&lt;li&gt;a&lt;/li&gt;&lt;/ol&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<ol start=/'5/'>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"5\">");
        auto const& answer = u8"&lt;ol&nbsp;start=&quot;5&quot;&gt;";
        CHECK(html == answer);
    }

    // ---- empty lists (no <li> items) are malformed and fall back to literal text ----
    TEST_CASE("---- empty lists (no <li> items) are malformed and fall...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol></ol>");
        auto const& answer = u8"&lt;ol&gt;&lt;/ol&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("<ol> </ol>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol>\n</ol>");
        auto const& answer = u8"&lt;ol&gt;<br>&lt;/ol&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("<ol start=/'5/'></ol>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"5\"></ol>");
        auto const& answer = u8"&lt;ol&nbsp;start=&quot;5&quot;&gt;&lt;/ol&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("<ol><li>a<ol></ol></li></ol>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li>a<ol></ol></li></ol>");
        auto const& answer = u8"&lt;ol&gt;&lt;li&gt;a&lt;ol&gt;&lt;/ol&gt;&lt;/li&gt;&lt;/ol&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("an empty <li> item is still a valid list") {
        // an empty <li> item is still a valid list
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ol><li></li></ol>");
        auto const& answer = u8"<ol><li></li></ol>";
        CHECK(html == answer);
    }
    // ---- regression: empty lists must not panic the backends (fuzzer crash) ----
    TEST_CASE("---- regression: empty lists must not panic the backends...") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol></ol>");
        auto const& answer = u8"<size=20>＜</size>ol<size=20>＞</size><size=20>＜</size>/ol<size=20>＞</size>";
        CHECK(html == answer);
    }
    TEST_CASE("<ol> </ol> (33)") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ol>\n</ol>");
        auto const& answer = u8"<size=20>＜</size>ol<size=20>＞</size>\n<size=20>＜</size>/ol<size=20>＞</size>";
        CHECK(html == answer);
    }
}
