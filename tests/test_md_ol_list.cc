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
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test</li><li><ol><li>text</li></ol></li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 2. test\n   - text");
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test</li><li><ul><li>text</li></ul></li></ol>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   1. text");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li><li><ol><li>text</li></ol></li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"1. test\n 1. test\n   1. text\n     1. test");
        auto answer = ::fast_io::u8string_view(
            u8"<ol><li>test</li><li>test</li><li><ol><li>text</li><li><ol><li>test</li></ol></li></ol></li></ol>");
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
        auto answer = ::fast_io::u8string_view(
            u8"<ol><li>test</li><li>test</li><li><ol><li>t<strong>ex</strong>t</li></ol></li></ol>");
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
        auto answer = ::fast_io::u8string_view(u8"<ol><li>test</li><li>test</li><li><ul><li>text</li></ul></li></ol>");
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

    return 0;
}
