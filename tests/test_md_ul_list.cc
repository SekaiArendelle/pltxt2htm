#include "precompile.hh"

int main() {
    {
        auto pltext = ::fast_io::u8string_view{u8"- test"};
        auto html = ::pltxt2htm_test::pltxt4unittest(pltext);
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
        auto plunity_richtext = ::pltxt2htm_test::pltxt2plunity_introduction(pltext);
        auto plunity_richtext_answer = ::fast_io::u8string_view{u8"• test\n"};
        pltxt2htm_test_assert_equal(plunity_richtext, plunity_richtext_answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   - text");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li><ul><li>text</li></ul></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   - text\n     - test");
        auto answer = ::fast_io::u8string_view(
            u8"<ul><li>test</li><li>test</li><ul><li>text</li><ul><li>test</li></ul></ul></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n  - test\n   - test\n    - test");
        auto answer =
            ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li><li>test</li><li>test</li><li>test</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   - t**ex**t");
        auto answer =
            ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li><ul><li>t<strong>ex</strong>t</li></ul></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"-\ttest");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- + marker tests ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"+ test");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"+ test\n + test");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"+ test\n   + test");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><ul><li>test</li></ul></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- * marker tests ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"* test");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"* test\n * test");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"* test\n   * test");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><ul><li>test</li></ul></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- empty list item ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - ");
        auto answer = ::fast_io::u8string_view(u8"<ul><li></li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - test\n - ");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li></li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- different markers ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - text\n   - text\n   * text");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>text</li><ul><li>text</li><li>text</li></ul></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - text\n   - text\n * text");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>text</li><ul><li>text</li></ul></ul><ul><li>text</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - text\n - text\n * text");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>text</li><li>text</li></ul><ul><li>text</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - text\n + test");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>text</li></ul><ul><li>test</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   + text");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test</li><ul><li>text</li></ul></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- list mixed with text ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"hello\n- foo\n- bar\nworld");
        auto answer = ::fast_io::u8string_view(u8"hello<br><ul><li>foo</li><li>bar</li></ul>world");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"before\n- item\nbetween\n+ another\n- last\nafter");
        auto answer = ::fast_io::u8string_view(
            u8"before<br><ul><li>item</li></ul>between<br><ul><li>another</li></ul><ul><li>last</li></ul>after");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"first\n- a\n- b\n- c\nlast");
        auto answer = ::fast_io::u8string_view(u8"first<br><ul><li>a</li><li>b</li><li>c</li></ul>last");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- a\n+ b\n* c");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>a</li></ul><ul><li>b</li></ul><ul><li>c</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"text\n- item");
        auto answer = ::fast_io::u8string_view(u8"text<br><ul><li>item</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- item\ntext");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>item</li></ul>text");
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- nested different markers in child ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- a\n  - b\n  + c");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>a</li><ul><li>b</li><li>c</li></ul></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- a\n  - b\n+ c");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>a</li><ul><li>b</li></ul></ul><ul><li>c</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- alternating text and single-item lists ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"a\n- 1\nb\n- 2\nc\n- 3");
        auto answer =
            ::fast_io::u8string_view(u8"a<br><ul><li>1</li></ul>b<br><ul><li>2</li></ul>c<br><ul><li>3</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- emphasis inside list ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- **bold**\n- *italic*");
        auto answer = ::fast_io::u8string_view(u8"<ul><li><strong>bold</strong></li><li><em>italic</em></li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }

    // ---- varying indentation ----
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"    - a\n   - b\n      - c");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>a</li><li>b</li><ul><li>c</li></ul></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"     - a\n   - b\n    - c");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>a</li><li>b</li><li>c</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }

    return 0;
}
