#pragma once

#include "doctest_config.hh"

TEST_SUITE("md_ol_list") {
    TEST_CASE("1. test") {
        auto const& pltext = u8"1. test";
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto const& answer = u8"<ol><li>test</li></ol>";
        CHECK(html == answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto const& plunity_richtext_answer = u8"1. test\n";
        CHECK(plunity_richtext == plunity_richtext_answer);
    }
    TEST_CASE("1. test 2. test") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test");
        auto const& answer = u8"<ol><li>test</li><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("1. test 2. test test") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\ntest");
        auto const& answer = u8"<ol><li>test</li><li>test</li></ol>test";
        CHECK(html == answer);
    }
    TEST_CASE("1. test (4)") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"  1. test");
        auto const& answer = u8"<ol><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("1. test 2. test 1. text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   1. text");
        auto const& answer = u8"<ol><li>test</li><li>test<ol><li>text</li></ol></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("1. test 2. test - text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   - text");
        auto const& answer = u8"<ol><li>test</li><li>test<ul><li>text</li></ul></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("- test - test 1. text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   1. text");
        auto const& answer = u8"<ul><li>test</li><li>test<ol><li>text</li></ol></li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("1. test 1. test 1. text 1. test") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 1. test\n   1. text\n     1. test");
        auto const& answer = u8"<ol><li>test</li><li>test<ol><li>text<ol><li>test</li></ol></li></ol></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("- test - test 1. text (9)") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n 1. text");
        auto const& answer = u8"<ul><li>test</li><li>test</li></ul><ol><li>text</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("1. test 2. test - text (10)") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n - text");
        auto const& answer = u8"<ol><li>test</li><li>test</li></ol><ul><li>text</li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("1. test 2. test 1. t**ex**t") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   1. t**ex**t");
        auto const& answer = u8"<ol><li>test</li><li>test<ol><li>t<strong>ex</strong>t</li></ol></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("case 12") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1 ");
        auto const& answer = u8"1";
        CHECK(html == answer);
    }

    TEST_CASE("1. test (13)") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1.\ttest");
        auto const& answer = u8"<ol><li>test</li></ol>";
        CHECK(html == answer);
    }

    // ---- mixed ul inside ol ----
    TEST_CASE("---- mixed ul inside ol ----") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   * text");
        auto const& answer = u8"<ol><li>test</li><li>test<ul><li>text</li></ul></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("1x test") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1x test");
        auto const& answer = u8"1x&nbsp;test";
        CHECK(html == answer);
    }

    TEST_CASE("1. test 2. text 3. test") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n2. text\n3. test");
        auto const& answer = u8"1. test\n2. text\n3. test\n";
        CHECK(html == answer);
    }

    TEST_CASE("1. test 2. text 1. test 2. text") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n2. text\n  1. test\n  2. text");
        auto const& answer = u8"1. test\n2. text\n  1. test\n  2. text\n";
        CHECK(html == answer);
    }

    TEST_CASE("1. test 1. text 1. test") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n1. text\n1. test");
        auto const& answer = u8"1. test\n2. text\n3. test\n";
        CHECK(html == answer);
    }

    TEST_CASE("1. test 1. text 1. test 1. test 1. text 1. test") {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n1. text\n1. test\n\n1. test\n1. text\n1. test\n");
        auto const& answer = u8"1. test\n2. text\n3. test\n\n1. test\n2. text\n3. test\n";
        CHECK(html == answer);
    }

    // ---- ) delimiter ----
    TEST_CASE("---- ) delimiter ----") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1) test");
        auto const& answer = u8"<ol><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("1) test 2) test") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1) test\n 2) test");
        auto const& answer = u8"<ol><li>test</li><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("1) test 2) test 1) text") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1) test\n 2) test\n   1) text");
        auto const& answer = u8"<ol><li>test</li><li>test<ol><li>text</li></ol></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE(". and ) are different marker types -> separate lists (Co...") {
        // . and ) are different marker types -> separate lists (CommonMark rule);
        // each list starts at its own first item number
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2) test");
        auto const& answer = u8"<ol><li>test</li></ol><ol start=\"2\"><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("nested ) list inside a . list") {
        // nested ) list inside a . list
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   1) text");
        auto const& answer = u8"<ol><li>test</li><li>test<ol><li>text</li></ol></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE(") delimiter after a bullet list starts a new ordered lis...") {
        // ) delimiter after a bullet list starts a new ordered list
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n 1) text");
        auto const& answer = u8"<ul><li>test</li></ul><ol><li>text</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("1) test 2) text") {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1) test\n2) text");
        auto const& answer = u8"1. test\n2. text\n";
        CHECK(html == answer);
    }

    // ---- start attribute from the first item number ----
    TEST_CASE("---- start attribute from the first item number ----") {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"3. test\n 4. test");
        auto const& answer = u8"<ol start=\"3\"><li>test</li><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("subsequent item numbers are ignored; items renumber sequ...") {
        // subsequent item numbers are ignored; items renumber sequentially from start
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"3. test\n 1. test");
        auto const& answer = u8"<ol start=\"3\"><li>test</li><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE(") delimiter start") {
        // ) delimiter start
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"5) test\n 6) test");
        auto const& answer = u8"<ol start=\"5\"><li>test</li><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("nested ordered list keeps its own start") {
        // nested ordered list keeps its own start
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   3. text");
        auto const& answer = u8"<ol><li>test</li><li>test<ol start=\"3\"><li>text</li></ol></li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("ordered list nested inside an unordered list keeps its s...") {
        // ordered list nested inside an unordered list keeps its start
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   4. text");
        auto const& answer = u8"<ul><li>test</li><li>test<ol start=\"4\"><li>text</li></ol></li></ul>";
        CHECK(html == answer);
    }
    TEST_CASE("start=1 is normalized away") {
        // start=1 is normalized away
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 1. test");
        auto const& answer = u8"<ol><li>test</li><li>test</li></ol>";
        CHECK(html == answer);
    }
    TEST_CASE("plunity numbering follows start") {
        // plunity numbering follows start
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"3. test\n4. text\n5. test");
        auto const& answer = u8"3. test\n4. text\n5. test\n";
        CHECK(html == answer);
    }
    TEST_CASE("plunity numbering of a ) list follows start") {
        // plunity numbering of a ) list follows start
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"3) test\n4) text");
        auto const& answer = u8"3. test\n4. text\n";
        CHECK(html == answer);
    }
    TEST_CASE("plunity nested list numbering follows its own start") {
        // plunity nested list numbering follows its own start
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n2. text\n  3. test\n  4. text");
        auto const& answer = u8"1. test\n2. text\n  3. test\n  4. text\n";
        CHECK(html == answer);
    }
    TEST_CASE("roundtrip: generated <ol start> re-parses back to the sa...") {
        // roundtrip: generated <ol start> re-parses back to the same list
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"3. test\n 4. test");
        auto roundtrip = ::pltxt2htm_test::pltxt4unittest(u8"<ol start=\"3\"><li>test</li><li>test</li></ol>");
        CHECK(html == roundtrip);
    }
}
