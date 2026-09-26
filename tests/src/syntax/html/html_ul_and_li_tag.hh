#pragma once

#include "doctest_config.hh"

TEST_SUITE("html_ul_and_li_tag") {
    TEST_CASE("<ul><li>item1</li><li>item2</li></ul>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>item1</li><li>item2</li></ul>");
        auto const& answer = u8"<ul><li>item1</li><li>item2</li></ul>";
        CHECK(html == answer);
    }

    TEST_CASE("<UL ><LI>item1</LI><LI>item2</LI></UL >") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<UL    ><LI>item1</LI><LI>item2</LI></UL  >");
        auto const& answer = u8"<ul><li>item1</li><li>item2</li></ul>";
        CHECK(html == answer);
    }

    TEST_CASE("<ul><li><color=red>item1</color></li><li>item2</li></ul>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li><color=red>item1</color></li><li>item2</li></ul>");
        auto const& answer = u8"<ul><li><span style=\"color:red;\">item1</span></li><li>item2</li></ul>";
        CHECK(html == answer);
    }

    TEST_CASE("<ul><li><color=red>item1</li></ul></color>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li><color=red>item1</li></ul></color>");
        auto const& answer = u8"<ul><li><span style=\"color:red;\">item1</span></li></ul>&lt;/color&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<ul><li>item1<ul><li>item2</li></ul></li></ul>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>item1<ul><li>item2</li></ul></li></ul>");
        auto const& answer = u8"<ul><li>item1<ul><li>item2</li></ul></li></ul>";
        CHECK(html == answer);
    }

    TEST_CASE("<ul>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul>");
        auto const& answer = u8"&lt;ul&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("t<ul></ul>t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<ul></ul>t");
        auto const& answer = u8"t&lt;ul&gt;&lt;/ul&gt;t";
        CHECK(html == answer);
    }

    TEST_CASE("t<li") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"t<li");
        auto const& answer = u8"t&lt;li";
        CHECK(html == answer);
    }

    TEST_CASE("<ul><li>item1</li><del>test</del></ul>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>item1</li><del>test</del></ul>");
        auto const& answer = u8"&lt;ul&gt;&lt;li&gt;item1&lt;/li&gt;<del>test</del>&lt;/ul&gt;";
        CHECK(html == answer);
    }

    TEST_CASE("<li>test</li>") {
        auto plrichtext = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<li>test</li>");
        auto const& answer = u8"<size=20>＜</size>li<size=20>＞</size>test<size=20>＜</size>/li<size=20>＞</size>";
        CHECK(plrichtext == answer);
    }

    TEST_CASE("<ul><li><ul><li>xxx</li></ul></li></ul>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li><ul><li>xxx</li></ul></li></ul>");
        auto const& answer = u8"\u2022 \n  \u2218 xxx\n";
        CHECK(html == answer);
    }

    TEST_CASE("<ul><li>text<ul><li>sub</li></ul></li></ul>") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul><li>text<ul><li>sub</li></ul></li></ul>");
        auto const& answer = u8"\u2022 text\n  \u2218 sub\n";
        CHECK(html == answer);
    }

    TEST_CASE("<ul> without <li> must not be parsed as the <u> underlin...") {
        // <ul> without <li> must not be parsed as the <u> underline tag
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul>t</ul>");
        auto const& answer = u8"&lt;ul&gt;t&lt;/ul&gt;";
        CHECK(html == answer);
    }

    // ---- block-level lists: whitespace/newlines inside <ul> are ignored ----
    TEST_CASE("---- block-level lists: whitespace/newlines inside <ul>...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul>\n<li>a</li>\n<li>b</li>\n</ul>");
        auto const& answer = u8"<ul><li>a</li><li>b</li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("<ul> <li>a</li> <li>b</li> </ul>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul> <li>a</li>\t<li>b</li> </ul>");
        auto const& answer = u8"<ul><li>a</li><li>b</li></ul>";
        CHECK(html == answer);
    }
    // ---- a list after a line break is still a block ----
    TEST_CASE("---- a list after a line break is still a block ----") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"a\n<ul><li>b</li></ul>");
        auto const& answer = u8"a<br><ul><li>b</li></ul>";
        CHECK(html == answer);
    }
    // ---- non-<li> interior content makes the whole list literal ----
    TEST_CASE("---- non-<li> interior content makes the whole list lite...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>a</li> text <li>b</li></ul>");
        auto const& answer = u8"&lt;ul&gt;&lt;li&gt;a&lt;/li&gt;&nbsp;text&nbsp;&lt;li&gt;b&lt;/li&gt;&lt;/ul&gt;";
        CHECK(html == answer);
    }
    // ---- <li> outside a list context is literal ----
    TEST_CASE("---- <li> outside a list context is literal ----") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<li>a</li>");
        auto const& answer = u8"&lt;li&gt;a&lt;/li&gt;";
        CHECK(html == answer);
    }
    // ---- whitespace/newlines inside items are HTML formatting, not content ----
    TEST_CASE("---- whitespace/newlines inside items are HTML formattin...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>\n<ol>\n  <li>text</li></ol></li></ul>");
        auto const& answer = u8"<ul><li><ol><li>text</li></ol></li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("<ul><li> text</li></ul>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>\n  text</li></ul>");
        auto const& answer = u8"<ul><li>text</li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("a newline in the middle of the item text is real content") {
        // a newline in the middle of the item text is real content
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>first\nsecond</li></ul>");
        auto const& answer = u8"<ul><li>first<br>second</li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("<ul><li>text </li></ul>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>text\n</li></ul>");
        auto const& answer = u8"<ul><li>text</li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("<ul><li> </li></ul>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>\n</li></ul>");
        auto const& answer = u8"<ul><li></li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("<ul><li>text<ul><li>text</li></ul><ul><li>text</li></ul>...") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>text<ul><li>text</li></ul><ul><li>text</li></ul></li></ul>");
        auto const& answer = u8"<ul><li>text<ul><li>text</li></ul><ul><li>text</li></ul></li></ul>";
        CHECK(html == answer);
    }

    // ---- list-item text is parsed with the inline-only parser ----
    TEST_CASE("---- list-item text is parsed with the inline-only parse...") {
        auto html =
            ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>before\n# **heading**</li><li>after\n- *item*</li></ul>");
        auto const& answer =
            u8"<ul><li>before<br>#&nbsp;<strong>heading</strong></li><li>after<br>-&nbsp;<em>item</em></li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("An unclosed inline tag must stop at the current list-ite...") {
        // An unclosed inline tag must stop at the current list-item frame.
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li><b>first</li><li>second</li></ul>");
        auto const& answer = u8"<ul><li><strong>first</strong></li><li>second</li></ul>";
        CHECK(html == answer);
    }

    // ---- mismatched closing tag makes the list literal ----
    TEST_CASE("---- mismatched closing tag makes the list literal ----") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>a</li></ol>");
        auto const& answer = u8"&lt;ul&gt;&lt;li&gt;a&lt;/li&gt;&lt;/ol&gt;";
        CHECK(html == answer);
    }

    // ---- empty lists (no <li> items) are malformed and fall back to literal text ----
    TEST_CASE("---- empty lists (no <li> items) are malformed and fall...") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul></ul>");
        auto const& answer = u8"&lt;ul&gt;&lt;/ul&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("<ul> </ul>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul>\n</ul>");
        auto const& answer = u8"&lt;ul&gt;<br>&lt;/ul&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("<ul><li>a<ul></ul></li></ul>") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li>a<ul></ul></li></ul>");
        auto const& answer = u8"&lt;ul&gt;&lt;li&gt;a&lt;ul&gt;&lt;/ul&gt;&lt;/li&gt;&lt;/ul&gt;";
        CHECK(html == answer);
    }
    TEST_CASE("an empty <li> item is still a valid list") {
        // an empty <li> item is still a valid list
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"<ul><li></li></ul>");
        auto const& answer = u8"<ul><li></li></ul>";
        CHECK(html == answer);
    }
    // ---- regression: empty lists must not panic the backends (fuzzer crash) ----
    TEST_CASE("---- regression: empty lists must not panic the backends...") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul></ul>");
        auto const& answer = u8"<size=20>＜</size>ul<size=20>＞</size><size=20>＜</size>/ul<size=20>＞</size>";
        CHECK(html == answer);
    }
    TEST_CASE("<ul> </ul> (33)") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"<ul>\n</ul>");
        auto const& answer = u8"<size=20>＜</size>ul<size=20>＞</size>\n<size=20>＜</size>/ul<size=20>＞</size>";
        CHECK(html == answer);
    }
}
