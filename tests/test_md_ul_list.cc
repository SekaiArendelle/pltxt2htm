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
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test<ul><li>text</li></ul></li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- test\n - test\n   - text\n     - test");
        auto answer = ::fast_io::u8string_view(
            u8"<ul><li>test</li><li>test<ul><li>text<ul><li>test</li></ul></li></ul></li></ul>");
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
            ::fast_io::u8string_view(u8"<ul><li>test</li><li>test<ul><li>t<strong>ex</strong>t</li></ul></li></ul>");
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
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test<ul><li>test</li></ul></li></ul>");
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
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test<ul><li>test</li></ul></li></ul>");
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
        auto answer = ::fast_io::u8string_view(u8"<ul><li>text<ul><li>text</li><li>text</li></ul></li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8" - text\n   - text\n * text");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>text<ul><li>text</li></ul></li></ul><ul><li>text</li></ul>");
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
        auto answer = ::fast_io::u8string_view(u8"<ul><li>test</li><li>test<ul><li>text</li></ul></li></ul>");
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
        auto answer = ::fast_io::u8string_view(u8"<ul><li>a<ul><li>b</li><li>c</li></ul></li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"- a\n  - b\n+ c");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>a<ul><li>b</li></ul></li></ul><ul><li>c</li></ul>");
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
        auto answer = ::fast_io::u8string_view(u8"<ul><li>a</li><li>b<ul><li>c</li></ul></li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }
    {
        auto html = ::pltxt2htm_test::pltxt4unittest(u8"     - a\n   - b\n    - c");
        auto answer = ::fast_io::u8string_view(u8"<ul><li>a</li><li>b</li><li>c</li></ul>");
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"- test\n- text");
        auto answer = ::fast_io::u8string_view{u8"• test\n• text\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    {
        auto html = ::pltxt2htm_test::pltxt2plunity_introduction(u8"* test\n  - text\n    + test");
        auto answer = ::fast_io::u8string_view{u8"• test\n  ∘ text\n    ▫ test\n"};
        pltxt2htm_test_assert_equal(html, answer);
    }

    // regression: roundtrip fuzzer finding. Consecutive sibling-nested lists
    // ([li, ul, ul]) must keep the parent item's </li> open across the whole run and
    // emit it only once, otherwise the emitted HTML (<ul> directly inside <ul> plus a
    // stray </li>) is rejected by the HTML list scanner and the roundtrip diverges.
    {
        auto pltext = ::fast_io::u8string_view{u8"*\t&\n\t\t\t\t*\t&*\t\t&*\t&\n\t\t*\t\n\t\t*\t\t\t~~~\t \"  & \n"};
        auto once = ::pltxt2htm_test::pltxt2roundtrip_htmld(pltext);
        auto once_answer = ::fast_io::u8string_view{
            u8"<ul><li>&amp;<ul><li>&amp;<em>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&amp;</"
            u8"em>&nbsp;&nbsp;&nbsp;&nbsp;"
            u8"&amp;</li></ul><ul><li></li><li>~~~&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&quot;&nbsp;&nbsp;&amp;</li></ul></"
            u8"li></ul>"};
        pltxt2htm_test_assert_equal(once, once_answer);
        auto twice = ::pltxt2htm_test::pltxt4htmlunittest(::fast_io::u8string_view{once.data(), once.size()});
        pltxt2htm_test_assert_equal(twice, once);
    }

    return 0;
}
