#pragma once

#include "doctest_config.hh"

TEST_SUITE("md_ol_list") {
    TEST_CASE("basic") {
        auto const& pltext = u8"1. test";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<ol><li>test</li></ol>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"1. test\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }
    TEST_CASE("two-items") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test");
        auto const& answer = u8"<ol><li>test</li><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("text-after-list") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\ntest");
        auto const& answer = u8"<ol><li>test</li><li>test</li></ol>test";
        CHECK(html == answer);
    }
    TEST_CASE("indented") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"  1. test");
        auto const& answer = u8"<ol><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("nested-ordered") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   1. text");
        auto const& answer = u8"<ol><li>test</li><li>test<ol><li>text</li></ol></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("nested-unordered") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   - text");
        auto const& answer = u8"<ol><li>test</li><li>test<ul><li>text</li></ul></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("ordered-inside-unordered") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   1. text");
        auto const& answer = u8"<ul><li>test</li><li>test<ol><li>text</li></ol></li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("triple-nested") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 1. test\n   1. text\n     1. test");
        auto const& answer = u8"<ol><li>test</li><li>test<ol><li>text<ol><li>test</li></ol></li></ol></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("same-level-different-markers") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n 1. text");
        auto const& answer = u8"<ul><li>test</li><li>test</li></ul><ol><li>text</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("bullet-after-ordered") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n - text");
        auto const& answer = u8"<ol><li>test</li><li>test</li></ol><ul><li>text</li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("nested-with-bold") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   1. t**ex**t");
        auto const& answer = u8"<ol><li>test</li><li>test<ol><li>t<strong>ex</strong>t</li></ol></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("lone-number-literal") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1 ");
        auto const& answer = u8"1";
        CHECK(html == answer);
    }

    TEST_CASE("tab-after-marker") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1.\ttest");
        auto const& answer = u8"<ol><li>test</li></ol>";
        CHECK(html == answer);
    }

    // ---- mixed ul inside ol ----
    TEST_CASE("asterisk-inside-ordered") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   * text");
        auto const& answer = u8"<ol><li>test</li><li>test<ul><li>text</li></ul></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("no-space-rejected") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1x test");
        auto const& answer = u8"1x&nbsp;test";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-numbering") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n2. text\n3. test");
        auto const& answer = u8"1. test\n2. text\n3. test\n";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-nested-indent") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n2. text\n  1. test\n  2. text");
        auto const& answer = u8"1. test\n2. text\n  1. test\n  2. text\n";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-renumbered") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n1. text\n1. test");
        auto const& answer = u8"1. test\n2. text\n3. test\n";
        CHECK(html == answer);
    }

    TEST_CASE("plunity-two-blocks") {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n1. text\n1. test\n\n1. test\n1. text\n1. test\n");
        auto const& answer = u8"1. test\n2. text\n3. test\n\n1. test\n2. text\n3. test\n";
        CHECK(html == answer);
    }

    // ---- ) delimiter ----
    TEST_CASE("paren-delimiter") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1) test");
        auto const& answer = u8"<ol><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("paren-two-items") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1) test\n 2) test");
        auto const& answer = u8"<ol><li>test</li><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("paren-nested") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1) test\n 2) test\n   1) text");
        auto const& answer = u8"<ol><li>test</li><li>test<ol><li>text</li></ol></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("dot-and-paren-separate-lists") {
        // . and ) are different marker types -> separate lists (CommonMark rule);
        // each list starts at its own first item number
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2) test");
        auto const& answer = u8"<ol><li>test</li></ol><ol start=\"2\"><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("paren-inside-dot-list") {
        // nested ) list inside a . list
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   1) text");
        auto const& answer = u8"<ol><li>test</li><li>test<ol><li>text</li></ol></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("paren-after-bullet") {
        // ) delimiter after a bullet list starts a new ordered list
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n 1) text");
        auto const& answer = u8"<ul><li>test</li></ul><ol><li>text</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("plunity-paren-numbering") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1) test\n2) text");
        auto const& answer = u8"1. test\n2. text\n";
        CHECK(html == answer);
    }

    // ---- start attribute from the first item number ----
    TEST_CASE("start-attribute") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"3. test\n 4. test");
        auto const& answer = u8"<ol start=\"3\"><li>test</li><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("subsequent-numbers-ignored") {
        // subsequent item numbers are ignored; items renumber sequentially from start
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"3. test\n 1. test");
        auto const& answer = u8"<ol start=\"3\"><li>test</li><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("paren-start-attribute") {
        // ) delimiter start
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"5) test\n 6) test");
        auto const& answer = u8"<ol start=\"5\"><li>test</li><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("nested-keeps-start") {
        // nested ordered list keeps its own start
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   3. text");
        auto const& answer = u8"<ol><li>test</li><li>test<ol start=\"3\"><li>text</li></ol></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("ordered-in-unordered-keeps-start") {
        // ordered list nested inside an unordered list keeps its start
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   4. text");
        auto const& answer = u8"<ul><li>test</li><li>test<ol start=\"4\"><li>text</li></ol></li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("start-one-normalized") {
        // start=1 is normalized away
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 1. test");
        auto const& answer = u8"<ol><li>test</li><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("plunity-start-numbering") {
        // plunity numbering follows start
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"3. test\n4. text\n5. test");
        auto const& answer = u8"3. test\n4. text\n5. test\n";
        CHECK(html == answer);
    }
    TEST_CASE("plunity-paren-start") {
        // plunity numbering of a ) list follows start
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"3) test\n4) text");
        auto const& answer = u8"3. test\n4. text\n";
        CHECK(html == answer);
    }
    TEST_CASE("plunity-nested-numbering") {
        // plunity nested list numbering follows its own start
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n2. text\n  3. test\n  4. text");
        auto const& answer = u8"1. test\n2. text\n  3. test\n  4. text\n";
        CHECK(html == answer);
    }
    TEST_CASE("roundtrip-start-attribute") {
        // roundtrip: generated <ol start> re-parses back to the same list
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"3. test\n 4. test");
        auto roundtrip = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"3\"><li>test</li><li>test</li></ol>");
        CHECK(html == roundtrip);
    }
}
