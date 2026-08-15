#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"1. test"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"1. test\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test</li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\ntest");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test</li></ol>test");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"  1. test");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   1. text");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test<ol><li>text</li></ol></li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   - text");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test<ul><li>text</li></ul></li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   1. text");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test<ol><li>text</li></ol></li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 1. test\n   1. text\n     1. test");
        auto answer = ::fast_io::u8string_view(
            u8"<ol><li>test</li><li>test<ol><li>text<ol><li>test</li></ol></li></ol></li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n 1. text");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li></ul><ol><li>text</li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n - text");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test</li></ol><ul><li>text</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   1. t**ex**t");
        auto answer =
            ::fast_io::u8string_view(u8"<ol><li>test</li><li>test<ol><li>t<strong>ex</strong>t</li></ol></li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1 ");
        auto answer = ::fast_io::u8string_view(u8"1");
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1.\ttest");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- mixed ul inside ol ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   * text");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test<ul><li>text</li></ul></li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1x test");
        auto answer = ::fast_io::u8string_view(u8"1x&nbsp;test");
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n2. text\n3. test");
        auto answer = ::fast_io::u8string_view{u8"1. test\n2. text\n3. test\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n2. text\n  1. test\n  2. text");
        auto answer = ::fast_io::u8string_view{u8"1. test\n2. text\n  1. test\n  2. text\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n1. text\n1. test");
        auto answer = ::fast_io::u8string_view{u8"1. test\n2. text\n3. test\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html =
            ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n1. text\n1. test\n\n1. test\n1. text\n1. test\n");
        auto answer = ::fast_io::u8string_view{u8"1. test\n2. text\n3. test\n\n1. test\n2. text\n3. test\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- ) delimiter ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1) test");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1) test\n 2) test");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test</li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1) test\n 2) test\n   1) text");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test<ol><li>text</li></ol></li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // . and ) are different marker types -> separate lists (CommonMark rule);
        // each list starts at its own first item number
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2) test");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li></ol><ol start=\"2\"><li>test</li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // nested ) list inside a . list
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   1) text");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test<ol><li>text</li></ol></li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // ) delimiter after a bullet list starts a new ordered list
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n 1) text");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li></ul><ol><li>text</li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1) test\n2) text");
        auto answer = ::fast_io::u8string_view{u8"1. test\n2. text\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- start attribute from the first item number ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"3. test\n 4. test");
        auto answer = ::fast_io::u8string_view(u8"<ol start=\"3\"><li>test</li><li>test</li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // subsequent item numbers are ignored; items renumber sequentially from start
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"3. test\n 1. test");
        auto answer = ::fast_io::u8string_view(u8"<ol start=\"3\"><li>test</li><li>test</li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // ) delimiter start
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"5) test\n 6) test");
        auto answer = ::fast_io::u8string_view(u8"<ol start=\"5\"><li>test</li><li>test</li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // nested ordered list keeps its own start
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   3. text");
        auto answer =
            ::fast_io::u8string_view(u8"<ol><li>test</li><li>test<ol start=\"3\"><li>text</li></ol></li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // ordered list nested inside an unordered list keeps its start
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   4. text");
        auto answer =
            ::fast_io::u8string_view(u8"<ul><li>test</li><li>test<ol start=\"4\"><li>text</li></ol></li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // start=1 is normalized away
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 1. test");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test</li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // plunity numbering follows start
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"3. test\n4. text\n5. test");
        auto answer = ::fast_io::u8string_view{u8"3. test\n4. text\n5. test\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // plunity numbering of a ) list follows start
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"3) test\n4) text");
        auto answer = ::fast_io::u8string_view{u8"3. test\n4. text\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // plunity nested list numbering follows its own start
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"1. test\n2. text\n  3. test\n  4. text");
        auto answer = ::fast_io::u8string_view{u8"1. test\n2. text\n  3. test\n  4. text\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        // roundtrip: generated <ol start> re-parses back to the same list
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"3. test\n 4. test");
        auto roundtrip = ::pltxt2htm_test::pltxt4unittest(
            ::fast_io::u8string_view(u8"<ol start=\"3\"><li>test</li><li>test</li></ol>"));
        pltxt2htm_test_assert_equal(html, roundtrip);
    }

    return 0;
}
